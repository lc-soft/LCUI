<#
.SYNOPSIS
    离屏渲染 doc-viewer 页面为 PNG，用于无窗口环境下的视觉验证。

.DESCRIPTION
    脚本会临时改写 examples/doc-viewer/app/main.c：
      - 把初始路由替换为 -Route；
      - 用离屏 ui_widget_render 把 root 渲染到 PNG（而不是进入 app_run 主循环）。
    渲染完成后从内存备份恢复 main.c 并重新构建，不依赖 git checkout，
    因此不会丢弃工作区里对 main.c 的未提交改动。

    滚动截图通过 -ScrollTops 指定，取当前页面中高度最大的 scrollarea
    （文档正文滚动区），多个值用逗号分隔。

.PARAMETER Route
    要渲染的路由，必须带 locale，例如 /zh-CN/docs/widgets/button-group。

.PARAMETER ScrollTops
    逗号分隔的滚动位置（px），默认 "0"。

.PARAMETER OutPrefix
    输出文件前缀，最终文件为 <OutPrefix>-<ScrollTop>.png。
    默认输出到 $env:TEMP\opencode\docview。

.PARAMETER Width / Height
    渲染尺寸（像素），默认 1920x1040。

.EXAMPLE
    pwsh examples/doc-viewer/scripts/render-offscreen.ps1 `
        -Route "/zh-CN/docs/widgets/button-group" `
        -OutPrefix "$env:TEMP\opencode\button-group"

.EXAMPLE
    pwsh examples/doc-viewer/scripts/render-offscreen.ps1 `
        -Route "/en/docs/widgets" -ScrollTops "0,600" `
        -OutPrefix "$env:TEMP\opencode\en-widgets"
#>
param(
    [string]$Route = "/zh-CN/docs/widgets/button-group",
    [string]$ScrollTops = "0",
    [string]$OutPrefix = "",
    [int]$Width = 1920,
    [int]$Height = 1040
)

$ErrorActionPreference = "Stop"

$root = (Resolve-Path (Join-Path $PSScriptRoot "..\..\..")).Path
$mainC = Join-Path $root "examples\doc-viewer\app\main.c"
$exe = Join-Path $root "build\windows\x64\debug\doc-viewer.exe"
$utf8 = New-Object System.Text.UTF8Encoding($false)

if (-not (Test-Path $mainC)) {
    throw "main.c not found: $mainC"
}
if (-not $OutPrefix) {
    $outDir = Join-Path $env:TEMP "opencode"
    if (-not (Test-Path $outDir)) {
        New-Item -ItemType Directory -Path $outDir | Out-Null
    }
    $OutPrefix = Join-Path $outDir "docview"
}

$original = [System.IO.File]::ReadAllText($mainC, [System.Text.Encoding]::UTF8)
$originalLf = $original.Replace("`r`n", "`n")

$shots = @()
foreach ($topText in $ScrollTops.Split(",")) {
    $top = $topText.Trim()
    $shots += ("        {{ {0}, `"{1}-{2}.png`" }}," -f $top, $OutPrefix.Replace('\', '/'), $top)
}

$code = @"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pandagl.h>
#include <ui/metrics.h>

/* 取当前树中高度最大的非侧栏 scrollarea，文档页里即正文滚动区。
 * 侧栏自己也有 ScrollArea，且与正文等高，只按高度会误选。 */
static bool debug_inside_sidebar(ui_widget_t *w)
{
        for (w = w->parent; w; w = w->parent) {
                if (ui_check_widget_type(w, "sidebar")) {
                        return true;
                }
        }
        return false;
}

static void debug_find_scrollarea(ui_widget_t *w, void *arg)
{
        ui_widget_t **target = arg;

        if (!ui_check_widget_type(w, "scrollarea") ||
            debug_inside_sidebar(w)) {
                return;
        }
        if (!*target || w->border_box.height > (*target)->border_box.height) {
                *target = w;
        }
}

static void debug_render_root(const char *path)
{
        pd_canvas_t canvas;
        pd_context_t paint;
        pd_rect_t rect;

        rect.x = 0;
        rect.y = 0;
        rect.width = $Width;
        rect.height = $Height;
        pd_canvas_init(&canvas);
        canvas.color_type = PD_COLOR_TYPE_ARGB;
        if (pd_canvas_create(&canvas, (unsigned)rect.width,
                             (unsigned)rect.height) != 0) {
                return;
        }
        pd_canvas_fill(&canvas, pd_rgb(255, 255, 255));
        paint.with_alpha = true;
        paint.rect = rect;
        pd_canvas_init(&paint.canvas);
        pd_canvas_quote(&paint.canvas, &canvas, &rect);
        ui_widget_render(ui_root(), &paint);
        pd_write_png_file(path, &canvas);
        pd_canvas_destroy(&paint.canvas);
        pd_canvas_destroy(&canvas);
}

static void debug_capture(void)
{
        static const struct {
                float top;
                const char *path;
        } shots[] = {
$($shots -join "`n")
        };
        ui_widget_t *scroll = NULL;
        size_t i;
        int j;

        for (i = 0; i < 10; ++i) {
                lcui_ui_update();
        }
        ui_widget_each(ui_root(), debug_find_scrollarea, &scroll);
        for (i = 0; i < sizeof(shots) / sizeof(shots[0]); ++i) {
                if (scroll) {
                        ui_scrollarea_set_scroll_top(scroll, shots[i].top);
                }
                for (j = 0; j < 20; ++j) {
                        lcui_ui_update();
                }
                debug_render_root(shots[i].path);
        }
}
"@

$restore = {
    [System.IO.File]::WriteAllText($mainC, $original, $utf8)
}

try {
    $patched = $originalLf.Replace("#include `"main.h`"", "#include `"main.h`"`n$code")
    $patched = $patched.Replace(
        "        snprintf(initial_path, sizeof(initial_path), `"/%s/home`",`n                 locale ? locale : `"en`");",
        "        snprintf(initial_path, sizeof(initial_path), `"%s`", `"$Route`");")
    $patched = $patched.Replace(
        "        {`n                lcui_window_options_t opt = { 0 };`n                lcui_window_t *window;`n`n                opt.id = `"main`";`n                opt.width = 1280;`n                opt.height = 720;`n                window = lcui_window_create_for_widget(ui_root(), &opt);`n                lcui_load_settings(`"dev.lcui.doc-viewer`");`n                lcui_window_show(window);`n        }`n`n        return app_run();",
        "        ui_widget_resize(ui_root(), $Width / ui_get_actual_scale(),`n                         $Height / ui_get_actual_scale());`n`n        debug_capture();`n        lcui_destroy();`n`n        return 0;")
    if ($patched -eq $originalLf -or $patched.Contains("return app_run();")) {
        throw "patch did not apply; examples/doc-viewer/app/main.c changed upstream"
    }
    [System.IO.File]::WriteAllText($mainC, $patched, $utf8)

    Push-Location $root
    try {
        xmake build doc-viewer 2>&1 | Select-Object -Last 1
    } finally {
        Pop-Location
    }

    $logDir = Join-Path $env:TEMP "opencode"
    if (-not (Test-Path $logDir)) {
        New-Item -ItemType Directory -Path $logDir | Out-Null
    }
    $errLog = Join-Path $logDir "docview-render.log"
    $p = Start-Process -FilePath $exe -WorkingDirectory (Join-Path $root "examples\doc-viewer\dist") -WindowStyle Hidden -RedirectStandardError $errLog -RedirectStandardOutput (Join-Path $logDir "docview-render.out") -PassThru
    $p.WaitForExit(120000) | Out-Null
    if (-not $p.HasExited) {
        Stop-Process -Id $p.Id -Force
    } else {
        Write-Output ("render exited: {0}" -f $p.ExitCode)
    }
    if (Test-Path $errLog) {
        Get-Content $errLog -ErrorAction SilentlyContinue | Select-Object -Last 20
    }
} finally {
    & $restore
    Push-Location $root
    try {
        xmake build doc-viewer 2>&1 | Select-Object -Last 1
    } finally {
        Pop-Location
    }
}

foreach ($topText in $ScrollTops.Split(",")) {
    $top = $topText.Trim()
    $out = "$OutPrefix-$top.png"
    if (Test-Path $out) {
        Write-Output ("saved {0}" -f $out)
    } else {
        Write-Output ("failed: {0}" -f $out)
    }
}
