# ButtonGroup Design

## 目标

新增 `ButtonGroup` 部件族，把多个相关按钮、输入框、Select、Dropdown、Popover 和 InputGroup 组合成一个视觉连续的按钮组。

覆盖 shadcn `button-group` 的可用能力：

- 水平 / 垂直排列
- 统一首项、中间项、末项的圆角与相邻边框
- 嵌套 ButtonGroup 的间距
- `ButtonGroupText` 与 `ButtonGroupSeparator`
- 与 Button、Input、InputGroup、Select、Dropdown、Popover 组合
- split button

## 非目标

- 不新增 Button 子类型
- 不实现 ToggleGroup 语义；ButtonGroup 只组织动作按钮，不维护选中状态
- 不提供 group 级别的 `size`；尺寸由内部按钮各自的 `size` 决定
- 不实现 RTL
- 不支持把任意深层后代当作 item；只有直接子项参与边框/圆角规则

## 公共 API

`include/LCUI/widgets/button_group.h`：

```c
typedef enum ui_button_group_orientation {
        UI_BUTTON_GROUP_HORIZONTAL = 0,
        UI_BUTTON_GROUP_VERTICAL,
} ui_button_group_orientation_t;

typedef enum ui_button_group_separator_orientation {
        UI_BUTTON_GROUP_SEPARATOR_VERTICAL = 0,
        UI_BUTTON_GROUP_SEPARATOR_HORIZONTAL,
} ui_button_group_separator_orientation_t;

LCUI_API ui_widget_t *ui_create_button_group(void);
LCUI_API void ui_button_group_set_orientation(
    ui_widget_t *group, ui_button_group_orientation_t orientation);
LCUI_API ui_button_group_orientation_t
ui_button_group_get_orientation(ui_widget_t *group);

LCUI_API ui_widget_t *ui_create_button_group_text(void);

LCUI_API ui_widget_t *ui_create_button_group_separator(void);
LCUI_API void ui_button_group_separator_set_orientation(
    ui_widget_t *separator,
    ui_button_group_separator_orientation_t orientation);
LCUI_API ui_button_group_separator_orientation_t
ui_button_group_separator_get_orientation(ui_widget_t *separator);

LCUI_API void ui_register_button_group(void);
```

## 部件结构

| 原型 | 说明 |
|---|---|
| `button-group` | 根容器，维护 `orientation`、`horizontal`/`vertical`、`has-group` |
| `button-group-text` | muted 背景、边框、说明文字容器 |
| `button-group-separator` | 1px 分隔线，维护自己的 `orientation` |

不新增 `button-group-*` 按钮子类型；按钮、输入框、InputGroup、Select、Dropdown、Popover 全部使用现成部件。

## 属性

| 节点 | 属性 | 类型 | 默认 | 说明 |
|---|---|---|---|---|
| `button-group` | `orientation` | enum | `horizontal` | `horizontal` / `vertical` |
| `button-group-separator` | `orientation` | enum | `vertical` | `vertical` / `horizontal` |

字符串只作为声明层输入；C setter 通过 `ui_widget_set_attr()`，`on_setattr` 解析后更新内部枚举和 status。

## 直接子项标记

LCUI 没有 `>` 选择器，不能只靠 `button-group button` 限制直接子项。根节点在 `UI_TASK_BEFORE_UPDATE` 中给每个直接子项加：

- `button-group-item`：通用契约 class，用于 input/select/focus 等组内规则
- `button-group-item-horizontal` 或 `button-group-item-vertical`：当前 group 的排列方向

方向 class 必须加在直接子项上，而不是只靠根节点的 `:horizontal` 后代选择器，否则嵌套 group 中内层按钮会同时匹配外层方向规则。每个子项只会由它的直接父 group 标记一个方向 class。

- 只有直接子项有这些 class
- 子项 `unlink` 时移除三个 class，并请求根节点更新
- 避免子项移动到其它 group 后残留边框/圆角样式
- 首项/末项位置由 LCUI 自动维护的 `first-child` / `last-child` status 表达，不需要 root 逐项维护 first/middle/last
- orientation 变化时，根节点更新直接子项的方向 class

`button-group-item` / `button-group-item-horizontal` / `button-group-item-vertical` 是内部契约 class，不作为公开 API。

## 布局与状态

### 根节点

- `display: flex; width: fit-content; align-items: stretch`
- 默认水平排列
- `vertical` status：`flex-direction: column`
- `has-group` status：直接子项中存在嵌套 `button-group` 时设置，主题加 8px 间距
- 根节点 update 扫描直接子项：
  - 添加 `button-group-item` class 和 unlink 监听
  - 检测嵌套 `button-group`，维护 `has-group`
- `orientation` setattr 维护互斥的 `horizontal` / `vertical` status

### 水平布局

```css
button-group .button-group-item-horizontal {
  border-radius: 0;
  border-left-width: 0;
}
button-group .button-group-item-horizontal:first-child {
  border-top-left-radius: 8px;
  border-bottom-left-radius: 8px;
  border-left-width: 1px;
}
button-group .button-group-item-horizontal:last-child {
  border-top-right-radius: 8px;
  border-bottom-right-radius: 8px;
}
```

### 垂直布局

```css
button-group .button-group-item-vertical {
  border-radius: 0;
  border-top-width: 0;
}
button-group .button-group-item-vertical:first-child {
  border-top-left-radius: 8px;
  border-top-right-radius: 8px;
  border-top-width: 1px;
}
button-group .button-group-item-vertical:last-child {
  border-bottom-left-radius: 8px;
  border-bottom-right-radius: 8px;
}
```

### 焦点与尺寸

- `.button-group-item:focus`：`position: relative; z-index: 1`，避免 focus ring 被相邻项盖住
- `textinput.button-group-item`：`flex: 1 1 auto; min-width: 0`
- `input-group.button-group-item`：`flex: 1 1 auto; min-width: 0; width: auto`
- `select.button-group-item select-trigger`：`min-width: 0`，允许 Select 在组内收窄
- 按钮尺寸完全由各按钮自己的 `size` 决定

### Text 与 Separator

`button-group-text`：

- flex、gap 8、padding 10px
- muted 背景、1px 边框、14px medium
- 与按钮一样参与首尾圆角和相邻边框规则

`button-group-separator`：

- `vertical`：宽 1px、高度拉伸、背景 `#e5e5e5`
- `horizontal`：高 1px、宽度拉伸
- 分隔线放在按钮之间，不放在组首尾；否则会占掉首/末按钮的圆角位置
- 垂直 group 中需要横向分隔线时，调用方显式设置 `orientation="horizontal"`

## 样式分层

结构规则放 `src/widgets/button_group.c`：

- 根容器 flex、width、align-items
- vertical / has-group 的布局部分
- text / separator 的基础 display

视觉规则放 `src/styles/style-nova.css`：

- 首尾圆角、相邻边框
- text 的背景、边框、padding
- separator 的尺寸与颜色
- focus z-index
- textinput / input-group / select 的组内尺寸调整

修改 `style-nova.css` 后运行 `npm run build-styles`。

## 测试

新增 `tests/integration/test_button_group.c`，注册到 `tests/main.c`。覆盖：

- 三个原型和工厂的类型、默认 orientation
- `orientation` setter/getter 与 `horizontal`/`vertical` status 切换
- separator orientation setter/getter 与 status
- 根节点给直接子项加 `button-group-item` 和方向 class
- orientation 变化时更新方向 class
- 子项 unlink 后移除三个 class
- 嵌套 `button-group` 触发 `has-group`，且内层子项保留自己的方向 class
- 水平首项/中间项/末项的圆角和左边框
- 垂直首项/末项的圆角和上边框
- textinput / input-group 的 flex 拉伸
- 焦点项 z-index
- text / separator 工厂结构
- 销毁与 `ui_clear_trash()`

## 文档与示例

新增 `docs/zh-CN/widgets/button-group.mdx`，中文审核通过后再写英文页。

示例 11 组，每组 C / XML / TSX：

1. `button-group-demo`：Archive / Report / Snooze + 下拉菜单
2. `button-group-orientation`：纵向 +/- 图标按钮
3. `button-group-size`：sm / default / lg 三组
4. `button-group-nested`：嵌套 group + InputGroup
5. `button-group-separator`：Copy / Paste
6. `button-group-split`：主按钮 + 图标按钮
7. `button-group-input`：Input + 搜索按钮
8. `button-group-input-group`：嵌套 group + InputGroup + 图标按钮
9. `button-group-dropdown`：Follow + 下拉菜单
10. `button-group-select`：Select + Input + Send
11. `button-group-popover`：Copilot + Popover

字段表 3 个：

- `button-group`
- `button-group-text`
- `button-group-separator`

`docs/sidebars.json` 插在 `button` 与 `card` 之间。

## 注意事项

- LCUI 没有 `>`、`:has()`、属性选择器；直接子项通过 `button-group-item` class 标记，首尾通过 `first-child` / `last-child` status 表达
- 首尾圆角只作用于直接子项；包在普通 wrapper 里的按钮不会参与
- `button-group-item` 是内部 class；调用方不要依赖它做自定义样式
- separator 应放在组中间；放在首尾会占掉按钮的圆角位置
- 垂直 group 中的横向分隔线需要显式设置 separator `orientation="horizontal"`
- 不涉及 RTL
