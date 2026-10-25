# Sidebar 组件设计

## 目标

为 LCUI 增加可组合的 `sidebar` 部件族，覆盖 shadcn 侧栏的核心导航能力，并为后续将 doc-viewer 文档侧栏迁移到 Sidebar 部件提供基础。

## 依赖

- 分组折叠依赖 `docs/superpowers/specs/2026-10-24-collapsible-design.md`
- Sidebar 只使用 Collapsible 的公开行为，不复制其状态逻辑

## 范围

### 本版实现

- `sidebar-provider`
- `sidebar`、`sidebar-trigger`、`sidebar-rail`
- `sidebar-header`、`sidebar-footer`、`sidebar-content`
- `sidebar-group`、`sidebar-group-label`、`sidebar-group-action`、`sidebar-group-content`
- `sidebar-menu`、`sidebar-menu-item`、`sidebar-menu-action`、`sidebar-menu-sub`、`sidebar-menu-sub-item`、`sidebar-menu-badge`
- `sidebar-menu-button` 样式类
- 分组折叠、整体展开/收起、offcanvas/icon/none 三种收起模式
- 中英文文档、字段表、C/XML/TSX 示例
- 单元/集成测试与 doc-viewer 视觉验证

### 明确暂缓

- 移动端 Sheet
- `floating` / `inset` variant
- `SidebarInset`
- 键盘快捷键（Ctrl/Cmd+B）
- cookie / session 持久化
- `useSidebar` hook
- SidebarInput、SidebarSeparator、SidebarMenuSkeleton
- doc-viewer 文档侧栏迁移（单独任务）

## CSS 约束

LCUI CSS 不支持属性选择器和功能伪类，因此：

- 部件状态统一映射为 status 选择器，例如 `sidebar-provider:collapsed`、`sidebar:icon`
- 持久激活态统一使用 class，例如 `.sidebar-menu-button.active`
- `data-*` 只用于 C 侧行为查找，不进入 CSS 选择器
- 不使用 `[data-active]`、`:has()`、`:not()`、`>`

## 结构

```text
SidebarProvider
├── Sidebar
│   ├── SidebarHeader
│   ├── SidebarContent
│   │   └── SidebarGroup
│   │       ├── SidebarGroupLabel
│   │       ├── SidebarGroupAction
│   │       └── SidebarGroupContent
│   │           └── SidebarMenu
│   │               └── SidebarMenuItem
│   │                   ├── SidebarMenuButton（class）
│   │                   ├── SidebarMenuAction
│   │                   ├── SidebarMenuBadge
│   │                   └── SidebarMenuSub
│   │                       └── SidebarMenuSubItem
│   ├── SidebarFooter
│   └── SidebarRail
└── SidebarTrigger
```

## Provider 与状态

- `sidebar-provider` 是唯一的上下文容器
- `Sidebar`、`SidebarTrigger`、`SidebarRail` 向上查找最近的 `sidebar-provider`
- `default-open` 属性默认 `true`
- Provider 持有 open 状态，关闭时加 status `collapsed`
- Provider 在状态变化时发出 `open-change` 事件
- `open` 属性可读写当前状态

```c
ui_widget_t *ui_create_sidebar_provider(void);
ui_widget_t *ui_create_sidebar(void);
ui_widget_t *ui_create_sidebar_trigger(void);
ui_widget_t *ui_create_sidebar_rail(void);
void ui_sidebar_provider_set_open(ui_widget_t *provider, bool open);
bool ui_sidebar_provider_get_open(ui_widget_t *provider);
void ui_sidebar_provider_toggle(ui_widget_t *provider);
```

其余 Sidebar 结构部件各提供对应的 `ui_create_sidebar_*` 工厂函数。

## Sidebar 收起模式

`sidebar` 的 `collapsible` 属性：

- `offcanvas`（默认）：收起时宽度为 0，隐藏侧栏
- `icon`：收起时宽度为 48px，隐藏菜单文本，保留图标
- `none`：始终展开，Trigger/Rail 不改变宽度

`side` 属性：

- `left`（默认）
- `right`

状态都通过 `sidebar` 的 status 和 Provider 的 `collapsed` status 组合实现，不依赖属性选择器。

## SidebarTrigger 与 SidebarRail

- `SidebarTrigger` 是 button，默认 `ghost` + `icon-sm`
- 图标使用 Fluent `panel-left`
- 点击切换最近 Provider 的 open 状态
- `SidebarRail` 是位于侧栏边缘的窄 button，点击同样切换状态
- 找不到 Provider 的 Trigger/Rail 不执行切换
- 两者都保留 C 事件回调能力；业务层可监听 Provider 的 `open-change` 事件

## SidebarMenuButton

`SidebarMenuButton` 不注册为 widget，只定义 class 契约：

- 默认由调用方创建 `button`、`a` 或 `RouterLink`，加 `sidebar-menu-button`
- 激活项再加 `active` class
- 未来 `@lcui/react` 的 `SidebarMenuButton` 组件：
  - 默认创建带 `sidebar-menu-button` class 的 `button`
  - 指定 `render` 时，把 `sidebar-menu-button` class 追加到渲染出的子元素
- 不提供 `ui_create_sidebar_menu_button()`
- 文档中按 class + `active` 描述，不提供字段表
- 按钮文本放在带 `sidebar-menu-label` class 的 text 子节点中；icon 收起模式通过 `.sidebar-menu-label` 隐藏文字，保留图标。未来 React 组件也遵循该结构

## SidebarContent 与滚动

- `sidebar-content` 负责 flex 布局和高度占位，不自动创建 ScrollArea
- 需要滚动时由调用方在 `sidebar-content` 内组合 `ScrollArea` / `ScrollAreaContent`
- doc-viewer 迁移时沿用现有 ScrollArea 组合

## 菜单激活态

- 使用 `.sidebar-menu-button.active`
- C 通过 `ui_widget_add_class(button, "sidebar-menu-button")` 和 `active`
- XML 通过 `class="sidebar-menu-button active"`
- RouterLink 通过 `activeClass="active"`
- 不使用 `data-active` 样式选择器

## SidebarGroup 折叠

- 推荐结构：

```xml
<collapsible default-open>
  <sidebar-group>
    <sidebar-group-label data-collapsible-trigger="true">Help</sidebar-group-label>
    <collapsible-content>
      <sidebar-group-content>...</sidebar-group-content>
    </collapsible-content>
  </sidebar-group>
</collapsible>
```

- `sidebar-group-label` 保留 label 语义和样式
- `data-collapsible-trigger` 只由 Collapsible 的事件逻辑读取
- 嵌套分组各自切换，不连带切换父级

## 样式方案

- 在 `style-nova.css` 新增 `sidebar-*` 主题规则
- 结构规则（flex、尺寸、定位）放在各部件源码，主题规则放在 style-nova
- 使用 `@apply` 复用 Tailwind 配色，不硬编码颜色
- Sidebar 默认浅色背景、左侧或右侧分隔线
- MenuButton：
  - 高度 30px
  - 圆角 6px
  - 文本 0.8rem、medium
  - hover 浅灰背景
  - active 浅灰背景 + 细边框 + medium
- icon 收起模式通过 `sidebar-provider:collapsed sidebar:icon` 隐藏 `.sidebar-menu-label`
- 无属性选择器

## 示例与文档

- 顶部默认示例：`sidebar-basic`，包含 Header、Group、Menu、Trigger
- 示例小节：
  - Collapsible Group
  - Collapsed / Icon mode
- 字段表：Provider、Sidebar、Trigger、Rail、Header、Footer、Content、Group、GroupLabel、GroupAction、GroupContent、Menu、MenuItem、MenuAction、MenuSub、MenuSubItem、MenuBadge
- `SidebarMenuButton` 作为 class 在正文说明
- 中英文同步

## 测试

- Provider 默认 open
- Trigger 切换最近 Provider
- 无 Provider 时安全 no-op
- offcanvas 宽度为 0
- icon 模式宽度 48px，文本隐藏
- none 模式不收起
- 分组 Collapsible 嵌套
- MenuButton class/active 样式与布局
- SidebarContent 内部 ScrollArea 组合

## 迁移计划

Sidebar 与 Collapsible 完成并通过文档检查后，另开任务将 doc-viewer 文档侧栏迁移到 Sidebar 部件，并保留：

- 透明背景
- 文档侧栏专用的 MenuButton `w-fit`
- 当前 Search/Sections/分组结构
- 现有 ScrollArea 滚动行为

## 验收

- 相关集成测试通过
- doc-viewer 侧栏示例可交互
- 分组折叠与 icon/offcanvas 收起正常
- 不引入属性选择器或功能伪类
