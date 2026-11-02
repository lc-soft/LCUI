# InputGroup Design

## 目标

新增 `InputGroup` 部件族，把输入框、多行输入、图标、说明文字、按钮、下拉菜单等组合成统一边框的输入区域。

覆盖 shadcn `input-group` 的可用能力：

- 根容器统一边框、圆角、focus 外框和高度
- addon 支持 `inline-start` / `inline-end` / `block-start` / `block-end`
- 复用现有 `textinput`、`button`、`dropdown-menu`、`spinner` 等部件
- 提供 `InputGroupInput`、`InputGroupTextarea`、`InputGroupButton`、`InputGroupText`、`InputGroupKbd` 工厂
- 支持自定义可聚焦控件接入

## 非目标

- 不实现 RTL
- 不引入独立 `textarea` 部件；多行输入复用 `textinput` 的多行模式
- 不实现 ToggleGroup / ButtonGroup 语义
- 不承诺第三方输入控件的完整兼容；自定义控件需通过 `ui_input_group_mark_control()` 接入

## 公共 API

`include/LCUI/widgets/input_group.h`：

```c
typedef enum ui_input_group_addon_align {
        UI_INPUT_GROUP_ADDON_INLINE_START = 0,
        UI_INPUT_GROUP_ADDON_INLINE_END,
        UI_INPUT_GROUP_ADDON_BLOCK_START,
        UI_INPUT_GROUP_ADDON_BLOCK_END,
} ui_input_group_addon_align_t;

LCUI_API ui_widget_t *ui_create_input_group(void);
LCUI_API ui_widget_t *ui_create_input_group_addon(void);
LCUI_API void ui_input_group_addon_set_align(
    ui_widget_t *addon, ui_input_group_addon_align_t align);
LCUI_API ui_input_group_addon_align_t
ui_input_group_addon_get_align(ui_widget_t *addon);

LCUI_API ui_widget_t *ui_create_input_group_text(void);
LCUI_API ui_widget_t *ui_create_input_group_kbd(void);
LCUI_API ui_widget_t *ui_create_input_group_input(void);
LCUI_API ui_widget_t *ui_create_input_group_textarea(void);
LCUI_API ui_widget_t *ui_create_input_group_button(void);
LCUI_API void ui_input_group_mark_control(ui_widget_t *control);
LCUI_API void ui_register_input_group(void);
```

## 部件结构

| 原型 | 说明 |
|---|---|
| `input-group` | 根容器 |
| `input-group-addon` | 图标、文本、按钮的承载容器；带 `align` 属性 |
| `input-group-text` | muted 说明文字容器 |
| `input-group-kbd` | 键盘提示样式容器 |

标准部件工厂：

| 工厂 | 返回类型 | 默认配置 |
|---|---|---|
| `ui_create_input_group_input()` | `textinput` | class `input-group-input` + `input-group-control` |
| `ui_create_input_group_textarea()` | 多行 `textinput` | class `input-group-textarea` + `input-group-control`；调用 `ui_textinput_enable_multiline()` |
| `ui_create_input_group_button()` | `button` | `UI_BUTTON_VARIANT_GHOST` + `UI_BUTTON_SIZE_XS` + class `input-group-button` |

`ui_create_input_group_text()` / `kbd()` 返回容器；调用方自行追加 `text`、`icon` 等子节点。

`ui_input_group_mark_control(w)` 为自定义控件追加 `input-group-control` class，使其参与：
- addon 点击聚焦查找
- 根容器 `multiline` / `has-disabled` 扫描
- 输入控件的 flex 布局规则

## 属性

| 节点 | 属性 | 类型 | 默认 | 说明 |
|---|---|---|---|---|
| `input-group-addon` | `align` | enum | `inline-start` | `inline-start` / `inline-end` / `block-start` / `block-end` |
| `input-group` | `disabled` | boolean | `false` | 复用内建 disabled |
| `input-group-input` / `textarea` | `disabled` | boolean | `false` | 复用 `textinput` disabled |

`align` 只作为声明层输入；C setter 通过 `ui_widget_set_attr()`，`on_setattr` 解析后更新内部枚举并设置 addon 的 `align-*` status。

## 布局与状态

### 根容器

- `display: flex; align-items: center; width: 100%; min-width: 0; height: 32px`
- 默认单行高度 32px；存在多行输入或 block addon 时高度改为 auto
- 根节点监听冒泡的 `focus` / `blur`：
  - focus 时增加 `focused` status
  - blur 时移除 `focused` status
- 根节点维护以下派生 status：
  - `focused`：焦点在根节点子树内
  - `block`：存在 `block-start` / `block-end` addon
  - `multiline`：存在 `input-group-textarea` 控件
  - `has-disabled`：直接子控件或 addon 内的按钮带 `disabled`
- 根节点自身 `disabled` 使用内建 `disabled` status

### Addon 顺序

根节点在 `UI_TASK_BEFORE_UPDATE` 中扫描直接子节点，按以下桶顺序物理排列：

1. `block-start` addon
2. `inline-start` addon
3. `input-group-control` 控件及其它普通子节点
4. `inline-end` addon
5. `block-end` addon

同一桶内保持原相对顺序。只有实际顺序不同才移动，避免每帧重排和焦点丢失。

这样调用方可以按 shadcn 习惯先写 input、后写 addon，视觉顺序仍正确；DOM 顺序不影响 Tab 顺序。

### Block 布局

存在任一 block addon 时：

- 根节点设置 `block` status
- `height: auto; flex-direction: column; align-items: stretch`
- block addon 宽度 100%：
  - `block-start`：`padding: 8px 10px 4px`
  - `block-end`：`padding: 4px 10px 8px`
- inline addon 仍按左右 padding 处理

### 多行输入

存在 `input-group-textarea` 时：

- 根节点设置 `multiline` status：`height: auto`
- textarea 控件 `min-height: 64px`，`flex: 1 1 auto`
- addon 保持在行内居中；block addon 与多行输入可同时存在

### Addon 点击聚焦

- addon 监听 `click`
- 如果点击目标是 `button` 原型或其后代，直接返回
- 否则在父级 `input-group` 中查找第一个带 `input-group-control` class 的后代
- 找到后调用 `ui_set_focus(control)`
- 找不到时回退到第一个 `textinput` 子节点

### 输入控件

- `input-group-input` / `input-group-textarea`：
  - 移除自身边框、圆角、背景和 focus 阴影
  - `flex: 1 1 auto; min-width: 0`
- textarea 使用多行 `textinput`，调用 `ui_textinput_enable_multiline()`，设置 `min-height: 64px`

### 按钮

- `input-group-button` 默认 `ghost + xs`
- 主题把高度限制为 24px；调用方可继续设置 `variant` / `size`（如 `icon-xs`）

## 样式分层

结构规则放 `src/widgets/input_group.c`：

- 根容器 flex、宽高、min-width
- addon / text / kbd 的基础 display
- `focused` / `block` / `multiline` / `has-disabled` status 的布局部分

视觉规则放 `src/styles/style-nova.css`：

- 根节点边框、圆角、背景、focus ring、disabled 外观
- input/textarea 去边框、去背景、去阴影
- addon inline/block padding
- text / kbd / button 的字号、间距、高度

修改 `style-nova.css` 后运行 `npm run build-styles`。

## 测试

新增 `tests/integration/test_input_group.c`，注册到 `tests/main.c`。覆盖：

- 四个原型与七个工厂的类型、class、默认值
- addon `align` 默认值与四种取值
- inline-start addon 自动排到 input 前
- inline-end addon 自动排到 input 后
- block-start / block-end 顺序、`block` status、根高度 auto
- 多行输入触发 `multiline` status
- addon 点击聚焦 input；点击 addon 内 button 不抢焦点
- 根 focus/blur 维护 `focused` status
- input/button disabled 时根获得 `has-disabled`
- `ui_input_group_mark_control()` 让自定义控件参与聚焦查找
- text / kbd 工厂结构
- 销毁与 `ui_clear_trash()`

## 文档与示例

新增 `docs/zh-CN/widgets/input-group.mdx`，中文审核通过后再写英文页。

示例 13 组，每组 C / XML / TSX 三变体：

1. `input-group-demo`：Search + 搜索图标 + “12 results”
2. `input-group-inline-start`：Field + 输入框 + 前置搜索图标
3. `input-group-inline-end`：密码输入 + 后置眼睛图标
4. `input-group-block-start`：输入框 + 上方 “Full Name”；多行输入 + 文件名/复制按钮
5. `input-group-block-end`：输入框 + 下方 “USD”；多行输入 + 字数/提交按钮
6. `input-group-icon`
7. `input-group-text`
8. `input-group-button`
9. `input-group-kbd`
10. `input-group-dropdown`
11. `input-group-spinner`
12. `input-group-textarea`
13. `input-group-custom`：用 `ui_input_group_mark_control()` 接入自定义可聚焦控件

字段表 7 个：

- `input-group`
- `input-group-addon`
- `input-group-text`
- `input-group-kbd`
- `input-group-input`
- `input-group-textarea`
- `input-group-button`

`docs/sidebars.json` 插在 `field` 与 `item` 之间。

## 注意事项

- LCUI 没有 `:focus-within` 和 `:has()`，focus 边框与 disabled 外观由 C 侧事件/update 逻辑维护
- 没有独立 textarea 部件；`InputGroupTextarea` 基于多行 `textinput`，多行编辑仍有已知限制
- addon 的 `align` 同时决定视觉顺序、inline/block 布局和 padding；根容器会自动重排直接子节点
- 自定义控件必须通过 `ui_input_group_mark_control()` 标记，才能被 addon 点击聚焦和根容器状态扫描
- 同一个 group 不建议混用 block 与 inline addon；block 布局下 inline addon 会按列布局处理
- 一个 group 只应包含一个输入控件；addon 点击聚焦和 disabled 扫描只针对第一个匹配控件
- 不涉及 RTL
