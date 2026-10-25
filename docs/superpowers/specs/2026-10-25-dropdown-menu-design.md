# DropdownMenu 组件设计

## 目标

为 LCUI 增加对齐 shadcn 的 `dropdown-menu` 部件族，覆盖菜单、子菜单、勾选项、单选项、快捷提示和复杂菜单组合。

## 范围

### 本版实现

- `dropdown-menu`
- `dropdown-menu-trigger`
- `dropdown-menu-content`
- `dropdown-menu-group`
- `dropdown-menu-label`
- `dropdown-menu-item`
- `dropdown-menu-checkbox-item`
- `dropdown-menu-radio-group`
- `dropdown-menu-radio-item`
- `dropdown-menu-separator`
- `dropdown-menu-shortcut`
- `dropdown-menu-sub`
- `dropdown-menu-sub-trigger`
- `dropdown-menu-sub-content`
- Content 自动使用 Portal；同时提供 `ui_create_dropdown_menu_portal()`
- Basic、Submenu、Shortcuts、Icons、Checkboxes、Checkboxes Icons、Radio Group、Radio Icons、Destructive、Avatar、Complex 示例
- 中英文文档、字段表、C/XML/TSX 示例
- 外部点击、Escape、方向键、Home/End、Enter/Space、子菜单左右方向键
- 禁用项、destructive 变体、checkbox 勾选状态、radio 互斥值

### 明确暂缓

- RTL 示例
- 类型 ahead 搜索（快速键入匹配菜单项）
- 自动碰撞翻转和可用高度裁剪
- CheckboxItem 的 indeterminate 状态

## CSS 约束

- LCUI CSS 不支持属性选择器和功能伪类；状态使用 status/class。
- 不使用 `[data-*]`、`:has()`、`:not()`、`:nth-child()`、`>`、`+`、`~`。
- 打开、高亮、选中、禁用状态使用 status：
  - `dropdown-menu:open` / `dropdown-menu:closed`
  - `dropdown-menu-item:highlighted`
  - `dropdown-menu-item:checked`
  - `dropdown-menu-item:disabled`
  - `dropdown-menu-sub:open` / `dropdown-menu-sub:closed`
- `data-*` 只用于行为查找，不进入 CSS 选择器。
- 本部件的结构规则和视觉规则统一放在 `src/styles/style-nova.css`，部件源码不再
  单独加载 CSS 字符串，避免同一类型规则存在两份来源。


## 结构

```text
DropdownMenu
├── DropdownMenuTrigger
└── DropdownMenuContent（Portal）
    ├── DropdownMenuLabel
    ├── DropdownMenuGroup
    │   ├── DropdownMenuItem
    │   └── DropdownMenuCheckboxItem
    ├── DropdownMenuSeparator
    ├── DropdownMenuGroup
    │   ├── DropdownMenuLabel
    │   └── DropdownMenuRadioGroup
    │       ├── DropdownMenuRadioItem
    │       └── DropdownMenuRadioItem
    └── DropdownMenuSub
        ├── DropdownMenuSubTrigger
        └── DropdownMenuSubContent（Portal）
            └── DropdownMenuGroup
                ├── DropdownMenuLabel
                └── DropdownMenuItem
```

## 状态与属性

- Root：`default-open`、`open`，状态 `open` / `closed`
- Trigger：点击切换；Escape 关闭；外部点击关闭
- Content：`side`（`top` / `bottom` / `left` / `right`，默认 `bottom`）、`align`（`start` / `center` / `end`，默认 `start`）、`side-offset`、`align-offset`
- Item：`disabled`、`variant`（`default` / `destructive`）、`inset`
- CheckboxItem：`checked`，点击切换
- RadioGroup：`value`，RadioItem：`value`，点击后更新组值
- Sub：`default-open` / `open`；SubTrigger 使用 `mouseover` 或点击打开
- Shortcut：纯文本，右对齐
- Label：纯文本/图标容器，不可聚焦
- Separator：视觉分隔

## 交互

- 点击 Trigger 打开/关闭；打开后焦点进入第一个可交互 Item
- `ArrowDown` / `ArrowUp` 在可见 Item 之间循环高亮，跳过 disabled、separator 和 label
- `Home` / `End` 到第一个/最后一个可交互 Item
- `Enter` / `Space` 激活当前高亮 Item
- `Escape` 关闭当前菜单；有打开的子菜单时先关闭子菜单
- `ArrowRight` 打开 SubTrigger 对应子菜单；`ArrowLeft` 关闭子菜单并返回父级
- 激活普通 Item 后关闭整个菜单
- 激活 CheckboxItem 后切换状态并关闭菜单
- 激活 RadioItem 后更新 RadioGroup 值并关闭菜单
- 点击 Content 外部关闭；点击 Trigger 不二次打开
- 同一时间只允许一个顶层 DropdownMenu 打开
- 根节点在打开/关闭时发射 `open` / `close`

## C API

```c
typedef enum ui_dropdown_menu_item_variant {
        UI_DROPDOWN_MENU_ITEM_DEFAULT = 0,
        UI_DROPDOWN_MENU_ITEM_DESTRUCTIVE,
} ui_dropdown_menu_item_variant_t;

ui_widget_t *ui_create_dropdown_menu(void);
ui_widget_t *ui_create_dropdown_menu_trigger(void);
ui_widget_t *ui_create_dropdown_menu_content(void);
ui_widget_t *ui_create_dropdown_menu_portal(void);
ui_widget_t *ui_create_dropdown_menu_group(void);
ui_widget_t *ui_create_dropdown_menu_label(void);
ui_widget_t *ui_create_dropdown_menu_item(void);
ui_widget_t *ui_create_dropdown_menu_checkbox_item(void);
ui_widget_t *ui_create_dropdown_menu_radio_group(void);
ui_widget_t *ui_create_dropdown_menu_radio_item(void);
ui_widget_t *ui_create_dropdown_menu_separator(void);
ui_widget_t *ui_create_dropdown_menu_shortcut(void);
ui_widget_t *ui_create_dropdown_menu_sub(void);
ui_widget_t *ui_create_dropdown_menu_sub_trigger(void);
ui_widget_t *ui_create_dropdown_menu_sub_content(void);

void ui_dropdown_menu_set_open(ui_widget_t *menu, bool open);
bool ui_dropdown_menu_is_open(ui_widget_t *menu);
void ui_dropdown_menu_item_set_variant(ui_widget_t *item,
                                       ui_dropdown_menu_item_variant_t variant);
void ui_register_dropdown_menu(void);

/* 窄封装，内部只调用 ui_dropdown_menu_set_open() */
static inline void ui_dropdown_menu_open(ui_widget_t *menu)
{
        ui_dropdown_menu_set_open(menu, true);
}

static inline void ui_dropdown_menu_close(ui_widget_t *menu)
{
        ui_dropdown_menu_set_open(menu, false);
}
```

`ui_dropdown_menu_set_open()` / `ui_dropdown_menu_is_open()` 同时适用于
`dropdown-menu-sub`，因此子菜单复用同一套公开状态 API。

### 状态 API 的取舍

- 开关只有 `ui_dropdown_menu_set_open()` 一个实现；`open` / `close` 是内联语法糖，
  不重复维护第二份状态机。
- 禁用状态不另设 API，直接使用 `ui_widget_set_disabled()` 和
  `ui_widget_has_status(item, "disabled")`。部件内部同样读取 `w->disabled`，点击与键盘
  导航都会跳过禁用项。
- `variant` 只在创建/声明时设置，不提供 getter；C 侧保留类型化 setter
  `ui_dropdown_menu_item_set_variant()`，声明层写 `variant="destructive"`。
- CheckboxItem 的勾选状态用 `ui_widget_set_attr(item, "checked", "true")` /
  `ui_widget_get_attr(item, "checked")`，与现有 `checkbox` 部件一致，不再新增一组长名字
  包装函数。部件在 `setattr` 里同步 `checked` status。
- RadioGroup 的值用 `ui_widget_set_attr(group, "value", value)` /
  `ui_widget_get_attr(group, "value")`；组在更新时同步子项的 `checked` status，
  不提供 `*_set_value()` / `*_get_value()`。
- 所有长名字状态函数在实现前被删除，避免同一状态出现两套写入入口。

### Trigger 的声明方式

部件按以下顺序查找 trigger：

1. 子部件中的 `dropdown-menu-trigger` 原型；
2. 子部件中带 `data-dropdown-menu-trigger="true"` 的任意部件。

Submenu 对应使用 `dropdown-menu-sub-trigger` 原型或
`data-dropdown-menu-sub-trigger="true"`。因此可以直接把既有 `button` /
`sidebar-menu-action` 标记为 trigger，避免额外包一层
`dropdown-menu-trigger`。

## 事件

- Root `open` / `close`
- Root `select`：普通 Item 激活，第三个回调参数为 item
- Root `checked-change`：CheckboxItem 切换，第三个回调参数为 item
- Root `value-change`：RadioGroup 值变化，第三个回调参数为 item
- Sub 也发射 `open` / `close`，不向父菜单冒泡

## 示例与文档

- 顶部默认示例：`dropdown-menu-demo`
- 示例小节：
  - Basic
  - Submenu
  - Shortcuts
  - Icons
  - Checkboxes
  - Checkboxes Icons
  - Radio Group
  - Radio Icons
  - Destructive
  - Avatar
  - Complex
- 字段表：
  - Root、Trigger、Content、Group、Label、Item、CheckboxItem、RadioGroup、RadioItem、Separator、Shortcut、Sub、SubTrigger、SubContent
- `DropdownMenuContent` 不提供字段表之外的 Portal 配置；Portal 细节由部件内部管理
- 中英文同步

## 示例对照清单

以 `apps/v4/examples/base/` 下的独立示例文件为准，不使用
`apps/v4/registry/bases/base/examples/dropdown-menu-example.tsx` 中的聚合演示替代。

| shadcn 小节 | 源文件 | demo | 处理 |
|-------------|--------|------|------|
| 顶部示例 | `dropdown-menu-demo.tsx` | `dropdown-menu-demo` | 直接移植 |
| Basic | `dropdown-menu-basic.tsx` | `dropdown-menu-basic-demo` | 直接移植 |
| Submenu | `dropdown-menu-submenu.tsx` | `dropdown-menu-submenu` | 直接移植，包含嵌套的 More options 子菜单 |
| Shortcuts | `dropdown-menu-shortcuts.tsx` | `dropdown-menu-shortcuts` | 直接移植；快捷键仅展示，typeahead 由部件内置 |
| Icons | `dropdown-menu-icons.tsx` | `dropdown-menu-icons` | 直接移植 |
| Checkboxes | `dropdown-menu-checkboxes.tsx` | `dropdown-menu-checkboxes` | 直接移植 |
| Checkboxes Icons | `dropdown-menu-checkboxes-icons.tsx` | `dropdown-menu-checkboxes-icons` | 直接移植 |
| Radio Group | `dropdown-menu-radio-group.tsx` | `dropdown-menu-radio-group` | 直接移植 |
| Radio Icons | `dropdown-menu-radio-icons.tsx` | `dropdown-menu-radio-icons` | 直接移植 |
| Destructive | `dropdown-menu-destructive.tsx` | `dropdown-menu-destructive` | 直接移植 |
| Avatar | `dropdown-menu-avatar.tsx` | `dropdown-menu-avatar` | LCUI 替代：暂无 Avatar 部件，使用圆形 Button + fallback 文本 |
| Complex | `dropdown-menu-complex.tsx` | `dropdown-menu-complex` | 直接移植，包含多层子菜单、复选项和单选组 |
| RTL | `dropdown-menu-rtl.tsx` | - | 舍弃：RTL 未实现，已在注意事项说明 |

## 测试

- 默认关闭；`default-open` 初始打开
- Trigger 点击开关；外部点击关闭；Escape 关闭
- 普通 Item 激活关闭并发射 `select`
- disabled Item 不激活
- CheckboxItem 切换 `checked-change`
- RadioGroup/RadioItem 互斥和 `value-change`
- Submenu 打开/关闭、父菜单关闭时子菜单关闭
- 键盘 Up/Down/Home/End/Enter/Escape/Right/Left
- Content Portal 锚定、side/align、content-first 和 owner-first 销毁

## 注意事项

- RTL、类型 ahead、自动碰撞翻转、Checkbox indeterminate 暂缓，文档注意事项需说明。
- `DropdownMenuContent` 由 Portal 管理，业务代码不要手动挂到 root。
