# Tooltip 组件设计

## 目标

为 LCUI 提供与 shadcn `Tooltip` 对齐的悬浮提示部件：触发器在 hover 或 focus 后，于 Portal 中显示一段短文本，并支持四个方向的定位和 border 三角箭头。

## 范围

- 三段式 API：`tooltip` / `tooltip-trigger` / `tooltip-content`。
- C 便捷 API：`ui_tooltip_attach()` / `ui_tooltip_attach_ex()`；side/align 使用
  enum，字符串仅用于 `ui_widget_set_attr()`、XML、TSX 等声明层，由 `on_setattr`
  解析成 typed 状态。
- `data-tooltip="<文本>"` 极简属性：任意 widget 加上该属性即可 hover/focus 显示提示，并可用 `data-tooltip-side` / `data-tooltip-align` / `data-tooltip-side-offset` / `data-tooltip-align-offset` / `data-tooltip-delay` 覆盖参数。
- 根级 `delay`，不做 `tooltip-provider`，不做受控 `open`。
- hover + focus 显示，移出、失焦、Escape 立即隐藏。
- 复用 Portal 的 `anchor` / `side` / `align` / `side_offset` / `align_offset`。
- border 三角箭头，参考 `F:\code\lc-design\src\ui\components\tooltip.c` 与 `_tooltip.scss`。
- 中文文档先定稿，再同步英文；示例覆盖 basic、sides、disabled。
- 不包含：边缘碰撞翻转、滚动/resize 重定位、CSS 动画、`tooltip-provider`、`kbd` 部件、keyboard 示例。

## 结构与 API

```text
tooltip
├── tooltip-trigger
│   └── （按钮 / 图标 / 任意可 hover 内容）
└── tooltip-content          （运行时移入 Portal）
    └── （文本 / 任意内容）
        tooltip-arrow        （内部创建）
```

C API：

```c
/* include/LCUI/widgets/placement.h */
typedef enum ui_side {
        UI_SIDE_TOP = 0,
        UI_SIDE_RIGHT,
        UI_SIDE_BOTTOM,
        UI_SIDE_LEFT,
} ui_side_t;

typedef enum ui_align {
        UI_ALIGN_CENTER = 0,
        UI_ALIGN_START,
        UI_ALIGN_END,
} ui_align_t;

/* include/LCUI/widgets/tooltip.h */
typedef ui_side_t ui_tooltip_side_t;
typedef ui_align_t ui_tooltip_align_t;

typedef struct ui_tooltip_options {
        const char *content;
        int delay;
        ui_tooltip_side_t side;
        ui_tooltip_align_t align;
        int side_offset;
        int align_offset;
} ui_tooltip_options_t;

void ui_tooltip_options_init(ui_tooltip_options_t *options);
void ui_tooltip_attach(ui_widget_t *target, const char *text);
void ui_tooltip_attach_ex(ui_widget_t *target,
                          const ui_tooltip_options_t *options);

ui_widget_t *ui_create_tooltip(void);
ui_widget_t *ui_create_tooltip_trigger(void);
ui_widget_t *ui_create_tooltip_content(void);

void ui_tooltip_set_delay(ui_widget_t *w, int delay_ms);
int ui_tooltip_get_delay(ui_widget_t *w);
void ui_tooltip_show(ui_widget_t *w);
void ui_tooltip_hide(ui_widget_t *w);
bool ui_tooltip_is_open(ui_widget_t *w);

void ui_tooltip_content_set_side(ui_widget_t *w, ui_tooltip_side_t side);
void ui_tooltip_content_set_align(ui_widget_t *w, ui_tooltip_align_t align);
void ui_tooltip_content_set_side_offset(ui_widget_t *w, int offset);
void ui_tooltip_content_set_align_offset(ui_widget_t *w, int offset);
```

### C 便捷 API

大多数场景只需要一段纯文本提示，因此提供目标元素 + 文本的便捷入口：

```c
ui_tooltip_attach(button, "Add to library");
```

- 内部等价于设置目标元素的 `data-tooltip`，仍走惰性 controller；首次
  hover/focus 时才创建 content/Portal。
- `text` 为 `NULL` 或空字符串时清除提示；若 controller 已创建，立即隐藏并释放。
- 提示正在显示时更新文本，会重新走 settle 流程，避免气泡和箭头偏移。

需要 delay / side / align / offset 时使用 options：

```c
ui_tooltip_options_t options;

ui_tooltip_options_init(&options);
options.content = "Add to library";
options.delay = 500;
options.side = UI_SIDE_RIGHT;
options.side_offset = 8;
ui_tooltip_attach_ex(button, &options);
```

- `ui_tooltip_options_init()` 填入默认值：delay 0、top、center、side_offset 4、
  align_offset 0。`attach_ex` 按 options 中的值原样应用；`0` 是合法偏移，不会被当作
  “未设置”。
- `attach_ex` 同样写入 `data-tooltip-side` / `data-tooltip-align` /
  `data-tooltip-side-offset` / `data-tooltip-align-offset` / `data-tooltip-delay`，
  因此属性模式与 C 便捷 API 行为一致。
- `options->content` 为空时清除提示；`options == NULL` 时 no-op。
- 显式 `tooltip` 结构或 `data-tooltip-trigger` 的优先级不变。

### 属性类型与 on_setattr

- `ui_tooltip_content_set_side` / `set_align` 使用 enum，参数值由 IDE/编译器检查；
  `set_side_offset` / `set_align_offset` 使用 `int`。公共 setter 只写入属性名/字符串。
- `tooltip-content` 原型的 `setattr` 把 `side` / `align` / `side-offset` /
  `align-offset` 字符串解析成 `ui_tooltip_content_t` 中的 typed 字段；非法
  `side` / `align` 回退 top / center。
- `tooltip` 原型的 `setattr` 解析 `delay` 字符串，使 XML / TSX 的 `delay` 与
  `ui_tooltip_set_delay()` 等价。
- 字符串仅存在于声明层（`ui_widget_set_attr()`、XML、TSX props）；Portal 的
  `ui_portal_set_side` / `set_align` 仍接收字符串，由 Tooltip 内部从 enum 转换。

属性：

| 部件 | 属性 | 默认值 |
| --- | --- | --- |
| `tooltip` | `delay`（毫秒） | `0` |
| `tooltip-content` | `side`（top/right/bottom/left） | `top` |
| `tooltip-content` | `align`（start/center/end） | `center` |
| `tooltip-content` | `side-offset` | `4` |
| `tooltip-content` | `align-offset` | `0` |

TSX 用法：

```tsx
<Tooltip>
  <TooltipTrigger>
    <Button variant="outline">Hover</Button>
  </TooltipTrigger>
  <TooltipContent>Add to library</TooltipContent>
</Tooltip>
```

### 声明式关联（data-*）

与 select / dialog 一致，支持两种绑定方式：

1. **结构绑定**：tooltip 下第一个 `tooltip-trigger` / `tooltip-content`。
2. **松散绑定**：trigger 写 `data-tooltip-trigger="<key>"`，content 写
   `data-tooltip-target="<key>"`。祖先链上找不到 tooltip 时，按 key 在全局
   registry 中查找或创建 loose tooltip controller（`owner = ui_root()`），
   trigger/content 各自在 link 时绑定到同一个 controller。

松散绑定用于 trigger 和 content 不在同一子树、或 content 被移入 Portal 后仍需
反查 owner 的场景；同一 key 的 trigger/content 可以分别放在工具栏和页面底部。
content 的 `data-tooltip-target` 在移入 Portal 后仍保留，作为反查依据。

解析顺序与 select 一致：

```c
tooltip = ui_tooltip_find_owner(w);          /* 沿 parent 找 tooltip */
if (!tooltip) {
        key = ui_widget_get_attr(w, attr);   /* data-tooltip-trigger/target */
        tooltip = ui_tooltip_get_loose(key); /* 全局 registry 查找/创建 */
}
```

### data-tooltip 极简属性

任意 widget 只要设置 `data-tooltip="<文本>"`，就不需要 `tooltip` /
`tooltip-trigger` / `tooltip-content` 结构：

```tsx
<Button data-tooltip="Add to library">Hover</Button>
```

同时支持用 `data-tooltip-*` 覆盖默认参数：

| 属性 | 对应参数 | 默认值 |
| --- | --- | --- |
| `data-tooltip` | 提示文本（必填） | — |
| `data-tooltip-side` | `side`：top / right / bottom / left | `top` |
| `data-tooltip-align` | `align`：start / center / end | `center` |
| `data-tooltip-side-offset` | `side_offset`（px） | `4` |
| `data-tooltip-align-offset` | `align_offset`（px） | `0` |
| `data-tooltip-delay` | `delay`（毫秒） | `0` |

```tsx
<Button
  data-tooltip="Add to library"
  data-tooltip-side="left"
  data-tooltip-side-offset="8"
  data-tooltip-delay="200"
>
  Hover
</Button>
```

- tooltip 模块在 `ui_root()` 上监听 `mouseover` / `mouseout` / `focus` /
  `blur`，命中目标或其祖先上的 `data-tooltip` 时惰性绑定。
- 每个目标缓存一个 loose controller（内部创建 `tooltip-content` 和文本），
  首次 hover/focus 时创建，目标销毁时清理。
- 文本、side、align、offset、delay 在**每次显示时**从目标属性重新读取，支持运行
  时修改；非法 `side`/`align` 回退到默认值，数值解析失败使用默认值。
- `data-tooltip` 为空字符串时忽略，不创建 controller。
- 显式 `tooltip` 结构内部的 widget 不再走该属性，避免和结构绑定重复。
- 该属性只提供文本、定位和延迟；需要自定义内容或复用内容时使用三段式 API 或
  `data-tooltip-trigger`/`data-tooltip-target` 松散绑定。

## 生命周期与 Portal

1. `tooltip` 在 link 后按结构查找第一个 `tooltip-trigger` 与第一个
   `tooltip-content` 后代；找不到 owner 时按 `data-tooltip-*` key 走松散绑定。
2. 创建一个 Portal（tooltip 持有），设置 `anchor = trigger`，把 content 移入 Portal。
3. 在 content 内部创建 `tooltip-arrow`，由 content 布局管理。
4. 显示前把 content 的 `side` / `align` / `side-offset` / `align-offset` 同步到 Portal，并给 content/arrow 添加 `tooltip-side-<side>` 状态。
5. content 由 Portal 管理销毁；tooltip 只持引用，并兼容 content-first 与 owner-first 两种销毁顺序。
6. 多个 tooltip 各自持有独立 Portal，不共享。

## 交互行为

- 在 `tooltip` 根上监听冒泡事件：
  - `mouseover`：`ui_get_mouseover()` 命中 trigger 子树时，按 `delay` 安排显示。
  - `mouseout`：`ui_get_mouseover()` 不再位于 trigger 子树时立即隐藏。
  - `focus` / `blur`：`ui_get_focus()` 位于 trigger 子树时显示，否则隐藏。
  - `keydown`：`e->key.code == KEY_ESCAPE` 且已打开时隐藏。
- `delay <= 0` 时同步显示；`delay > 0` 时使用 `ptk_set_timeout()`，隐藏、销毁或重新进入时用 `ptk_clear_timer()` 取消。
- content 设置 `pointer-events: none`，不接管鼠标；不做 hover content 保持。
- 禁用控件示例用 `tooltip-trigger` 包住 disabled button，hover 落在包装层上。

## 定位与箭头

- 定位完全交给 Portal：`side` 默认 top、`side_offset` 默认 4、`align` 默认 center。
- v1 不做碰撞翻转，也不在滚动/resize 时重新定位。
- 箭头用 0 尺寸元素 + 四边 border：
  - top：content 在触发点上方，箭头在底边，`border-top` 实色、左右透明；
  - bottom：箭头在顶边，`border-bottom` 实色；
  - left：箭头在右边，`border-left` 实色；
  - right：箭头在左边，`border-right` 实色。
- content 按 side 在箭头一侧留出 5px padding，使箭头处于 padding box 内，避免被父级裁剪（LCUI 会裁剪到父级 padding box）。
- content 设置 `position: relative`；arrow 设置 `position: absolute`，按 side 贴边并居中，不参与 flex 布局。
- 箭头颜色与 content 背景一致（`foreground-strong`）。
- 层级由 Portal 管理，tooltip 不额外设置全局 z-index。

## 样式

- content：`inline-flex`、深色背景 `foreground-strong`、浅色文字 `background`、`rounded-md`、`px-3 py-1.5`、`text-xs`、`max-w-xs`。
- 视觉规则放在 `src/styles/style-nova.css`；位置、Portal、状态切换放在 `src/widgets/tooltip.c`。
- v1 不使用动画（CSS 引擎不支持 `transform` / `transition`）。

## 文档与示例

- `docs/zh-CN/widgets/tooltip.mdx` 先定稿；中文确认后再同步 `docs/en/widgets/tooltip.mdx`。
- 字段表：`docs/widget-fields/tooltip.ts`、`tooltip-trigger.ts`、`tooltip-content.ts`。
- 侧栏：`docs/sidebars.json` 增加 Tooltip。
- 示例（每个均提供 C / XML / TSX 三变体）：
  - `tooltip-basic`：**纯属性用法**；TSX 只加 `data-tooltip="Add to library"`，
    C 使用 `ui_tooltip_attach(button, "Add to library")`。
  - `tooltip-sides`：left / top / bottom / right 四个按钮；TSX/XML 使用三段式，
    C 使用 `ui_tooltip_attach_ex()` + `ui_tooltip_side_t`。
  - `tooltip-disabled`：trigger 包装 disabled 按钮 + “This feature is currently unavailable”；
    C 使用 `ui_tooltip_attach(trigger, ...)`。
- 文档在“用法”之后新增“纯属性用法”小节，用表格列出 `data-tooltip-*` 参数、
  默认值和适用场景/限制，并保留三段式 API 的用法/组合说明。
- 不引入 keyboard 示例与 `kbd` 部件。

## 测试

新增 `tests/integration/test_tooltip.c`，注册到 `tests/main.c`：

1. 结构：trigger/content 被识别，content 进入 `ui_get_portal_root()`，arrow 存在。
2. delay：默认 0、自定义值可读取；delay>0 时 hover 不立即显示，hide 取消计时器。
3. hover / focus / blur / Escape 的显示与隐藏。
4. side / align / offset 同步到 Portal，并在布局后验证 content 与 trigger 的相对位置。
5. disabled 包装层 hover 可显示。
6. data-* 松散绑定：trigger 与 content 使用同一 key 且不在 tooltip 子树内时，
   hover 仍能在 Portal 中显示 content；结构绑定与松散绑定不会重复绑定。
7. `data-tooltip`：hover 目标或其后代命中属性时显示对应文本；移出后隐藏；
   空属性不创建 controller；目标销毁后清理缓存。
8. `data-tooltip-*` 参数：side/align/offset/delay 在显示时生效；非法值回退默认；
   修改属性后下一次显示使用新值。
9. content-first 与 owner-first 销毁不崩溃、不重复释放。
10. C 便捷 API：`ui_tooltip_attach()` 显示/更新/清除文本；`ui_tooltip_options_init()`
    默认值；`ui_tooltip_attach_ex()` 的 content/delay/side/align/offset 生效；目标销毁
    后其他 tooltip 不受影响。
11. enum setter 与 `on_setattr`：enum 映射为属性名；直接设置 `side`/`align` 属性
    （XML 路径）能更新 typed 状态；非法值回退 top / center。
12. 不写颜色断言。

## 涉及文件

- `include/LCUI/widgets/tooltip.h`
- `src/widgets/tooltip.c`
- `include/LCUI/widgets.h`
- `src/lcui_widgets.c`
- `src/styles/style-nova.css`
- `tests/integration/test_tooltip.c`
- `tests/main.c`
- `docs/widget-fields/tooltip.ts`、`tooltip-trigger.ts`、`tooltip-content.ts`
- `docs/zh-CN/widgets/tooltip.mdx`、`docs/en/widgets/tooltip.mdx`
- `docs/sidebars.json`
- `docs/examples/tooltip-{basic,sides,disabled}{,-xml,-tsx}/`
