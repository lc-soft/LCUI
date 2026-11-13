# CSS 引擎支持范围

> 选择器、优先级、display/white-space 等属性支持边界，以及常见陷阱与替代方案。


## 选择器支持情况

**选择器类型**（源码：`lib/css/src/selector.c:402-461`）

| 选择器 | 语法 | 支持 |
|--------|------|------|
| 通配符 | `*` | ✅ |
| 类型 | `text` `button` | ✅ |
| 类 | `.btn` | ✅ |
| ID | `#main` | ✅ |
| 属性 | `[attr]` `[data-x]` | ❌ |

**组合符**（`selector.c:432-458`，空格即后代，无 combinator 字段）

| 组合符 | 语法 | 支持 |
|--------|------|------|
| 后代 | `A B` | ✅ |
| 子代 | `A > B` | ❌ |
| 相邻兄弟 | `A + B` | ❌ |
| 通用兄弟 | `A ~ B` | ❌ |

**伪类**（`selector.c:241-244` 把 `:` 后文本存入 status 字符串列表，应用层通过 `ui_widget_add/remove_status` 维护）

| 伪类 | 支持 | 备注 |
|------|------|------|
| `:hover` | ✅ | 由 `ui_widget_status.c` 维护 |
| `:active` | ✅ | 同上 |
| `:focus` | ✅ | 同上 |
| `:disabled` | ✅ | 同上 |
| `:first-child` | ✅ | 同上 |
| `:last-child` | ✅ | 同上 |
| `:not()` | ❌ | 解析器不支持 `(` |
| `:is()` `:where()` `:has()` | ❌ | 同上 |
| `:nth-child(n)` | ❌ | 同上 |
| `:checked` `:empty` `:root` | ❌ | 语法可写但无内置语义 |

**多伪类组合** ✅（如 `text:hover:focus`，所有子集必须同时匹配，见 `library.c:130-146`）

**伪元素**：全部 ❌（`::before` `::after` 等，引擎不识别 `::`）

## 优先级规则（rank 体系）

Rank 值（`selector.c:18-24`）：

| 组成 | rank 贡献 |
|------|---------|
| 类型选择器 | +1 |
| 类选择器（每个） | +10 |
| 伪类 / status（每个） | +10 |
| ID 选择器 | +100 |

同 rank 时，`batch_num`（解析顺序号，`selector.c:378,384`）大者胜——即**后声明覆盖先声明**（`library.c:349-351`）。

**注意**：class 与 pclass rank 相同（都是 10）。如果 `.row:hover` 写在前、`.row.field-table-header` 写在后，且两者 rank 相同，后者覆盖前者。

无 `!important` 机制。

## `display` 属性

支持值（见 `lib/css/src/properties.c` 和 `lib/css/src/properties/display.c`）：
- `none`
- `block`（默认）
- `inline-block`
- `flex`
- `inline-flex`
- `table`
- `inline-table`
- `table-row`
- `table-cell`

**注意**：`table*` 值能被解析，但当前没有 table 布局实现（见
`docs/internals/layout-engine.md` 的分发机制）。

**不支持**：`grid`、`inline`、`contents`、`list-item`、`table-row-group` 等。

## `table-layout` 与 `border-spacing` 属性

- `table-layout: auto | fixed`（默认 `auto`）
- `border-spacing: <length>{1,2}`（默认 `0`）

## Flex 交叉轴对齐

- `align-items`：`normal | stretch | flex-start | flex-end | center`，默认 `normal`
  （按 `stretch` 处理）。
- `align-self`：`auto | normal | stretch | flex-start | flex-end | center`，默认
  `auto`。
- `align-self: auto` 使用父容器的 `align-items`；其它值只覆盖当前 flex item 的交叉轴
  对齐，不改变其他子项。
- 当前不支持 `baseline` 取值（枚举存在，但 CSS 解析器尚未注册对应关键字）。

## 布局选择原则

**优先使用 `block` + `inline-block`**。LCUI 的 block/inline-block 布局比 flex 简单、效率高。`flex` 只在以下场景使用：
- 需要跨轴对齐（`align-items: center` 等）
- 需要沿主轴动态伸缩（`flex: 1`）
- 需要换行布局（`flex-wrap`）

对于同一行内同高度的元素（如 tab、inline 按钮、状态指示器），用 `display: inline-block` 即可。

## `white-space` 属性

尽管枚举（`lib/css/include/css/types.h`）定义了 `NORMAL`/`PRE`/`NOWRAP`/`PRE_WRAP`/`PRE_LINE`，实际 cascade 函数（`lib/css/src/properties/white_space.c`）**只处理 `normal` 和 `nowrap`**。`pre`、`pre-wrap`、`pre-line` 静默回退为 `normal`。

text widget（`src/widgets/text.c`）进一步只在设置 autowrap 时区分 `== NOWRAP` 和 `!= NOWRAP`，所以即使 `pre` 枚举存在也不会被尊重。

**后果**：`white-space: pre`（包括 Tailwind 的 `whitespace-pre`）在 LCUI 中是**空操作**。行首和连续 ASCII 空白在布局时会被折叠。如需保留源码缩进等前导空白，需要在生成内容时把前导空格替换为 U+00A0（NO-BREAK SPACE）。

## `font-family: monospace` 别名

LCUI 在 `src/lcui_fonts.c` 里注册 `monospace` 为内置别名：
- Windows → Consolas
- Linux → Ubuntu Mono

CSS 直接写 `font-family: monospace;` 即可，无需列平台特定字体名。

## Style 值 API

`lib/ui/include/ui/style.h` 提供了类型安全的内联样式设置 API：

- `ui_widget_set_style_unit_value(w, key, css_numeric_value_t value, css_unit_t unit)` — 直接设置数值 + 单位，避免 `snprintf` + `ui_widget_set_style_string` 的字符串开销
- 常用 key：`css_prop_padding_*`、`css_prop_margin_*`、`css_prop_width`、`css_prop_height`、`css_prop_top/left/right/bottom`
- 单位：`CSS_UNIT_PX`（`lib/css/include/css/types.h` 第 511 行附近）

例如：
```c
ui_widget_set_style_unit_value(child, css_prop_padding_left, (css_numeric_t)padding, CSS_UNIT_PX);
```

## 常见陷阱与替代方案

| 陷阱 | 替代方案 |
|------|---------|
| `.A:not(.B):hover` 不生效 | 顺序决胜：先写 `.A:hover`，再写 `.A.B { ... }`（同 rank 时 batch_num 决胜） |
| `[data-active]` 属性选择器无效 | 用 `.is-active` class 名 |
| `::before` / `::after` 不支持 | 添加额外子 widget + `content` CSS 属性 |
| `:checked` 不自动关联状态 | app 层增删 `.is-checked` class |
| `:nth-child(n)` 不支持 | app 层为第 n 个子节点添加特定 class（如 `.is-fifth`） |
| 无 `calc()` | 在 C 代码中预计算后写死数值 |
| 无 `em` / `rem` / `vh` / `vw` | 用 `dp` / `px` / `pt` 代替；视口相关在 C 里计算 |
| 无 `!important` | 提高 specificity（增加 class / ID 使 rank 更高）或调整声明顺序 |
| `position: fixed` 不生效 | LCUI 仅支持 `static` / `relative` / `absolute` |
| `white-space: pre` 不生效 | 仅支持 `normal` / `nowrap`；需保留前导空白时用 U+00A0 |
| `overflow: hidden` 不支持 | 通过让前景部件宽度不超过父轨道自然实现裁剪（如 progress indicator 用 `width: 50%` 而不是 `width: 100px; overflow: hidden`） |
| `css_string(...)` 内的 `-`、复合标识符被 clang-format 错乱 | 用 scrollbar.c 的原始字符串模式 `"...\\..."` + `\` 行续行，避开 `ReflowComments: true` 重排 |
| `strcmp(w->type, "...")` 段错误 | `w->type` 可能为 `NULL`（`ui_create_widget(NULL)` 创建的 wrapper div），必须先判 NULL |
| `border-radius: 50%` 被 css-parser 拒绝 | 引擎只支持长度值（`<length>{1,4}`），必须用 `8px` 这类固定值 |
| `cursor: pointer` / `user-select: none` 输出 parser 警告 | 这两个属性未注册值类型，warning 无害，可接受或移除 |
| selector 中 `label : disabled`（空格+冒号）报错 | 必须 `label:disabled`（无空格）；空格即后代选择器，`:disabled` 被当成类型选择器解析失败 |
| `[attr="value"]` 属性选择器静默失效 | 引擎不支持；改用 `:pseudo-class` + 在 setattr 回调里手动 `ui_widget_add_status` / `remove_status`（参考 radio_group_item / checkbox 模式） |

## 渲染裁剪约束（无 overflow:visible）

LCUI 渲染管线**隐式裁剪所有子 widget 到父元素的 padding_box 区域**。核心机制
在 `ui_renderer_render_children()` 中：

1. 子元素的 `canvas_box` 与父元素的 `content_rect`（`padding_box` ∩ paint rect）做交集
2. 不重叠则跳过（`continue`）
3. 重叠则通过 `pd_canvas_quote()` 限制像素写入范围

因此**任何子 widget 都无法渲染超出父元素 padding_box 的区域**。`position: absolute`
的子 widget 仍然受此约束——它只是脱离布局流，但渲染时仍被父级裁剪。

**后果**：需要浮出父容器边界的组件（select 下拉面板、dropdown-menu、tooltip）
必须将浮层内容挂载到 `ui_root()` 层级，不能作为触发器的子 widget。详见
[Portal 浮层模式](#portal-浮层模式)。


