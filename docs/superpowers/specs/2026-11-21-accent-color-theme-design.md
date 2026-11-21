# 强调色主题设计

日期：2026-11-21
状态：待实现

## 背景

当前 `primary` 固定为 `#171717`（浅色）/ `#fafafa`（深色），用户不能切换强调色。需要：

- 提供 26 个 Tailwind 配色预设（22 个 v3 颜色 + 4 个 v4 中性色）；
- 支持传入自定义颜色；
- 由基色衍生 `primary-foreground`、`accent`、`ring`、`sidebar-*` 以及半透明变体；
- 在 doc-viewer 的 navbar 提供色板选择器，并持久化选择。

## 现有约束

- LCUI 的 CSS 引擎不支持 `var()` / CSS 自定义属性，运行时不能只改一个变量；
- Tailwind utility 的颜色在编译期展开为字面量；
- `ui_load_css_string()` 只能追加规则，没有卸载接口；
- 现有 `dark` 主题通过 root 的 `.dark` class 切换。

因此强调色切换采用两种路径：

- **预设色**：构建期生成每个强调色的覆盖 CSS（`src/styles/accents/<name>.css`），运行时按需加载当前预设并卸载上一份；切换时同时更新 root 的 `.accent-<name>` class。
- **自定义颜色**：无法预生成，运行时计算衍生色并调用 `ui_load_css_string()` 加载覆盖规则；切换时先卸载上一份强调色规则，样式表不会无限增长。

## API（lib/ui）

新增 `lib/ui/include/ui/theme.h`：

```c
typedef struct ui_theme_accent {
        const char *name;
        pd_color_t light; /* 浅色主题主色 */
        pd_color_t dark;  /* 深色主题主色 */
} ui_theme_accent_t;

/* 预设名；NULL 或 "default" 恢复默认强调色 */
int ui_theme_set_accent(const char *name);

/* 自定义颜色，接受 #RRGGBB */
int ui_theme_set_accent_color(const char *hex);

/* 返回当前预设名，自定义颜色返回 "#RRGGBB" */
const char *ui_theme_get_accent(void);
```

实现要点：

- 预设：root 加 class `.accent-<name>`；对应 CSS 已在启动时加载；
- 自定义：root 加 class `.accent-custom`，生成覆盖 CSS 后调用 `ui_load_css_string(css, "ui-theme-accent")`；
- 切换时移除旧 class；预设切换不会新增规则；
- 主题（`.dark`）切换不需要重新生成或加载 CSS。

命名上，库层使用 `ui_theme_*`（与 `ui_*` 系列一致）；doc-viewer 的 `app_get_accent()` / `app_set_accent()` 只负责读取和持久化设置，内部调用库 API。`lcui_*` 保留给顶层生命周期与设置接口（`lcui_init`、`lcui_settings_*`）。

## 预设色与 CSS 文件

生成脚本 `scripts/generate-accent-themes.js`：

- 读取 `tailwindcss/colors` 的 17 个有彩色，加上 `neutral` 色阶；
- 输出两类文件：
  - `src/styles/accent-palette.h`（C 色表）
  - `src/styles/accents/<name>.css`（18 个覆盖规则文件）

色阶选择：

- 有彩色：浅色用 600，深色用 400；
- `neutral`：浅色用 900，深色用 100；
- `default` / `neutral` 作为默认强调色，保持接近当前 `#171717` 的观感。

生成的 C 色表使用 C11 指定初始化器：

```c
static const ui_theme_accent_t ui_theme_accents[] = {
        { "red",
          { .b = 0x26, .g = 0x26, .r = 0xdc, .a = 255 },
          { .b = 0x71, .g = 0x71, .r = 0xf8, .a = 255 } },
        /* ... */
};
```

注意：

- `pd_color_t` 是 union，匿名 struct 的字段顺序是 `b, g, r, a`，用指定初始化器可避免顺序错误；
- `pd_rgb()` 展开为 `pd_color()` inline 函数，不能用于静态初始化；
- `PD_RGB` 不存在。

生成的 `.css`、`accent-palette.h` 与脚本本身都提交到仓库；新增颜色时重新生成。

### 预置 CSS 的加载

- `src/styles/accents/<name>.css` 只包含该强调色的覆盖规则；
- `lcui build` 为每个 CSS 生成对应的 `.css.h`；
- 生成 `src/styles/accents/accent-themes.h`（含 `ui_load_accent_theme(name)` 与 `ui_load_accent_swatches()`）和仅供声明使用的 `accent-themes-api.h`；
- `ui_load_accent_theme()` 通过 `ui_load_css_string()` 加载单个预设；卸载入口为 `ui_unload_css_string(space)`；
- 强调色只保留一份样式：切换时先卸载上一份（预设置载来源为 `<name>.css`，自定义颜色为 `ui-theme-accent`），再加载目标；
- `src/lcui_widgets.c` 启动时只加载色板色块（`accent-swatches.css`），不再解析 18 个预设；
- 切换成本为解析单个约 7KB 的预设并刷新样式；常驻内存固定为一份强调色规则。

## 衍生色规则

给定浅色主色 `L`、深色主色 `D`：

| token | light | dark |
|---|---|---|
| `primary` | `L` | `D` |
| `primary-foreground` | 对比色 | 对比色 |
| `ring` | `L` | `D` |
| `accent` | `rgba(L, 0.10)` | `rgba(D, 0.15)` |
| `accent-foreground` | `L` | `D` |
| `sidebar-primary` | `L` | `D` |
| `sidebar-primary-foreground` | `primary-foreground` | 同 |
| `sidebar-accent` | `accent` | `accent` |
| `sidebar-accent-foreground` | `accent-foreground` | 同 |

半透明变体：`primary/90`、`/80`、`/30`、`/20`、`/15`、`/10` 使用 `rgba(L or D, alpha)`。

对比色算法：

- WCAG 相对亮度 > 0.5 → `#171717`；
- 否则 `#fafafa`。

自定义颜色：

- 浅色主色 = 输入 hex；
- 深色主色 = 与白色按 20% 混合（或在 HSL 中提升亮度），保证深色背景上可见；
- 预设颜色直接使用对应色阶，不再次计算深色主色。

## 覆盖规则来源

`style-nova.css` 的 `@apply` 在构建时已经内联为具体颜色，生成文件里没有 `.bg-primary` 等 utility 类；因此不能通过覆盖 utility 类切换强调色。

生成器改为解析 `src/styles/style-nova.css` 源文件：

- 遍历每条规则，读取 `@apply` 中的 token 类（`bg-primary`、`text-primary`、`border-primary`、`bg-primary/90`、`bg-sidebar-accent`、`text-sidebar-accent-foreground`、`dark:*` 等）；
- 提取「选择器 + CSS 属性 + token + alpha」；
- 为每个预设生成 `.accent-<name> <selector>`（浅色 token）或 `.dark .accent-<name> <selector>`（`dark:` token）的覆盖规则。

属性映射：

```text
bg-*                → background-color
text-*              → color
border-*            → border-color
border-t-*          → border-top-color
border-b-*          → border-bottom-color
border-l-*          → border-left-color
border-r-*          → border-right-color
```

覆盖规则示例：

```css
.accent-blue button:primary,
.accent-blue .button-primary {
  background-color: #2563eb;
}

.dark .accent-blue button:primary,
.dark .accent-blue .button-primary {
  background-color: #60a5fa;
}
```

规则选择器带 `.accent-<name>`（rank +10）或 `.dark .accent-<name>`（rank +10），高于原规则。

同时生成 `src/styles/accent-rules.h`（C 规则表）：

```c
typedef struct ui_theme_rule {
        const char *selector;
        const char *property;
        ui_theme_token_t token;
        int dark;
        int alpha; /* 0-255 */
} ui_theme_rule_t;
```

自定义颜色在运行时遍历该表，按 token 替换颜色并生成同样的 CSS。

## doc-viewer 集成

- `theme.c` 新增：
  - `app_get_accent()` / `app_set_accent()`，读写 `settings.ini` 的 `[app] accent`；
  - `app_apply_accent()`，调用 `ui_theme_set_accent` 或 `ui_theme_set_accent_color`。
- 启动时 `app_apply_theme()` 后调用 `app_apply_accent()`。
- navbar 主题按钮旁增加色板按钮（调色盘图标，具体 fluent-icons 名称实现时确认），点击打开 `popover`/`dropdown-menu`：
  - 18 个色块，圆角矩形网格；
  - 色块背景来自 `.accent-swatch-<name>`（生成 CSS）；
  - 当前选中项显示 checkmark（`.accent-swatch-check`）；
  - 选择后立即应用并持久化。

## 测试

库测试（`tests/integration/test_theme_accent.c`）：

- 18 个预设都能设置，root 出现对应 `.accent-<name>`；
- `bg-primary` 的 computed color 等于对应色阶（浅色 600，深色 400）；
- `.dark` 下 `bg-primary-dark` 使用深色色阶；
- 自定义 `#2563eb`：
  - 浅色 `bg-primary` = `#2563eb`；
  - `bg-primary/90` alpha = 230 左右；
  - `primary-foreground` 为对比色；
- `ui_theme_set_accent(NULL)` 恢复默认并移除 class；
- 切换两次后旧 class 不再生效；
- 启动加载完成后，18 个 `.accent-<name>` 规则都可用；预设切换不新增样式规则（自定义颜色除外）。

doc-viewer 测试：

- `app_set_accent("blue")` 写入 settings 并可读回；
- 色板存在 18 个色块，选中项与设置一致。

## 风险

- 强调色样式表采用单槽加载：切换时卸载旧规则再加载新预设，常驻内存固定为一份；卸载依赖 `css_remove_style_rules(space)` 按来源删除并刷新全树样式。
- 无 CSS 变量，衍生色必须在 C 端计算并生成规则。
- 预设色值与 Tailwind 色板一致性依赖生成脚本，改色板需要重新生成；生成脚本与 C 端的衍生算法需要保持同步。
