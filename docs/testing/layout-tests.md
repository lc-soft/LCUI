# 布局测试编写方法

> XML fixture / scene / 多宽度验证 / 断言约定。以 block、flex 布局测试为例。

> XML fixture / scene / 多宽度验证 / 断言约定。可用于 block、flex、table 布局测试。


布局测试（integration tests）遵循一套统一的写法，可参考 `test_block_layout.c`
和 `test_flex_layout.c` 的实现。

## 核心原则

1. **数据与测试代码分离**：所有测试场景静态地写在 XML 里（如
   `tests/fixtures/test_block_layout.xml`），C 代码只负责取引用 + 切换宽度 + 断言。
2. **Scene 共享**：preview 和 integration test 用同一个 scene + 同一份 XML/CSS；
   scene 在 `tests/scenes/` 下，入口函数名为 `<layout>_scene_build()`。
3. **Widget 树通过 id 引用**：每个需要被测试引用的元素必须有 `id` 属性。通过
   `ui_get_widget("id")` + `ui_widget_get_child()` + `ui_widget_next()` 获取引用。
4. **多宽度验证**：同一棵树，通过 `ui_widget_resize(ui_root(), W, H)` +
   `ui_update()` 切换宽度，每个宽度一个 describe 块。
5. **字体无关断言**：文本内容长度依赖字体系统，**禁止**用精确 px 值断言文本
   相关尺寸。用相对判断：`> 0`、单调递增、比例相等、宽松区间。

## C 测试文件结构

```c
#include <LCUI.h>
#include <stdio.h>
#include <ctest-custom.h>
#include "../scenes/<layout>_scene.h"

static void test_xxx(void)
{
        ui_widget_t *w = ui_get_widget("xxx");
        ui_widget_t *child0 = ui_widget_get_child(w, 0);
        /* 断言 child0->border_box 等 */
}

void test_<layout>(void)
{
        lcui_init();
        <layout>_scene_build();   /* 加载 XML + CSS */
        ui_widget_resize(ui_root(), 1024, 768);  /* 初始宽度 */
        ui_update();

        ctest_describe("scenario 1", test_xxx);
        /* 多宽度场景 */
        ctest_describe("width 800", test_width_800);
        ctest_describe("width 600", test_width_600);
        ctest_describe("width 320", test_width_320);

        lcui_destroy();
}
```

## 规则契约测试

除了 XML scene 场景回归，flex 布局还有一套按规则组织的契约测试：

- `tests/integration/test_flex_contract.c`（suite `flex contract`）：覆盖 flex 语义（`F01`–`F19`）和常见布局（`C02`–`C10`）。
- `tests/integration/test_sizehint.c`（suite `sizehint`）：覆盖内在尺寸（`H01`–`H16`）与 sizehint 能力矩阵。
- `tests/integration/test_empty_layout.c`（suite `empty layout`）与 `tests/integration/test_field_layout.c`（suite `field layout`）：用默认部件 + `tests/fixtures/test_empty_layout.css` / `test_field_layout.css` 的 `test-*` class 复现 empty / field 场景，不依赖 style-nova 或具体 widget 类型。

约定：

- 规则 ID 与缺口清单见 `docs/testing/flex-layout-coverage-audit.md`。
- `ctest_describe` 名称以规则 ID 开头，例如 `"F07 flex shrink"`。
- 只断言最终几何（box、相对位置、显隐），不读 `flex_basis`、分行中间值或测量缓存字段。
- 文本相关断言使用参照文本或相对比较，不写字体相关精确 px。
- 场景层（真实 widget 组合）仍由 `test_flex_layout.c` 和各 widget 测试承担，契约测试不复制真实结构。

## XML fixture 三层 wrapper 约定

所有布局测试的 XML 必须遵循以下三层包裹结构（参考
`tests/fixtures/test_block_layout.xml`）：

```xml
<lcui-app>
  <resource type="text/css" src="test_layout_common.css"/>  <!-- 共用 CSS -->
  <resource type="text/css" src="test_<layout>_layout.css"/> <!-- 私有 CSS -->
  <ui>
    <scrollarea class="h-full">              <!-- 第 1 层：滚动容器 -->
      <scrollarea-content id="main">         <!-- 第 2 层：滚动内容 -->
        <w class="container">                <!-- 第 3 层：居中容器 -->
          <w class="example">                <!-- 每个测试场景一个 example 块 -->
            <text class="example__title">(0) First scenario</text>
            <w class="example__content">
              <!-- 实际的布局内容（box / flex 容器）-->
            </w>
          </w>
          <w class="example">
            <text class="example__title">(1) Second scenario</text>
            <w class="example__content">...</w>
          </w>
        </w>
      </scrollarea-content>
      <scrollbar />
    </scrollarea>
  </ui>
</lcui-app>
```

**关键约定**：

- `<scrollarea-content>` **必须**设置 `id="main"` —— block/flex 测试通过
  `ui_get_widget("main")` 取到这个节点来定位场景容器。
- `<w class="container">` 负责限制最大宽度并居中；`max-width` 的具体值在
  各个 `<layout>_layout.css` 中覆盖（block: 800px, flex: 600px）。
- 每个测试场景必须用 `<w class="example">` 包裹，内部结构为
  `<text class="example__title">(N) Title</text>` + `<w class="example__content">`。
- `.example__content` 是实际布局内容的直接父元素，不同布局对其有轻微差异
  （block 用 `font-size: 0` 消除 inline-block 间距，flex 用 `overflow: hidden`）。
- ScrollArea 只跟踪**第一个** `scrollarea-content`（`ui_scrollarea_update()`
  找到后立即 `break`）。不要在同一个 `scrollarea` 下放多个 content 用
  `display` 切换（例如按语言切换的侧栏）；改用单个 content，内部再按视图分
  容器，由 CSS 只显示一个。

## 共用 CSS 文件约定

`tests/fixtures/test_layout_common.css` 包含三个布局 fixture 共享的基础样式，
必须在每个布局 fixture XML 中作为第一个 `<resource>` 引入。

**共用内容**：

- `text, body` 基础字体样式（Ubuntu Mono / Consola，14px）
- `body { margin: 0; }`
- `scrollarea-content { min-width: 0; }`（**必须**，否则 min-width 默认会阻止收缩）
- `.example`（灰色背景 + 1px 边框，用于包裹每个测试场景）
- `.container` 基础样式（`width: 100%; padding: 10px; box-sizing: border-box; margin: 0 auto;`，
  不含 max-width）
- `.example__title`, `.example__content`（padding、背景）
- `.h-full`、`.d-inline-block`、`.d-flex`、`.d-block` 显示工具类

**布局 fixture 各自的 CSS 文件**仅包含：

- `.container { max-width: Npx; }`（覆盖共用文件的 max-width）
- `.example__content { ... }` 的布局特定属性（font-size / overflow 等）
- 该布局独有的工具类和 widget 样式（如 block 的 `.box`、flex 的 `.flex-1`）

## 常用断言 API

| API | 用途 |
|-----|------|
| `ctest_equal_int(name, actual, expected)` | 整数精确匹配 |
| `ctest_equal_float(name, actual, expected)` | 浮点精确匹配 |
| `ctest_equal_bool(name, actual, expected)` | 布尔判断 |
| `ctest_equal_ui_rect(name, actual, expected)` | 同时比对 `x, y, w, h` |

描述字符串规则：
- `ctest_describe(name, fn)`：名词性主题，全小写空格分词（如 `"download tasks actions"`）
- `ctest_equal_*(name, ...)`：行为陈述（如 `"should fill root width"`、`"cell width=124"`）
- 当上下文清晰时可用 jQuery 选择器风格：`"$('.example:eq(3) .box')[5].border_box"`

## 关键 widget box 概念

| Box | 含义 |
|-----|------|
| `content_box.width/height` | 纯内容区域 |
| `padding_box.width` | content + padding |
| `border_box.width` | content + padding + border |
| `outer_box.width` | content + padding + border + margin |
| `outer_box.x/y` | layout_x/y（不含 margin） |
| `border_box.x/y` | layout + margin_left / margin_top |

注意：`border_box.x = layout_x + margin_left`，所以断言"两个 widget 的位置关系"
时，应使用 `outer_box.x` 而不是 `border_box.x`（后者包含了自身 margin，会干扰
相邻元素的相对位置判断）。

## UI XML 的 style 属性限制

**重要**：`lib/ui-xml/src/ui_xml.c` 当前**不处理** `<widget style="...">` 属性。
所有 `style="..."` 会被错误地当作 widget attribute 处理，不会生效。
因此：
- **禁止**在 XML 中使用 `style="..."` 设置 CSS 属性
- **改用** CSS class 集中管理样式（在 fixture 的 `.css` 文件中定义）



