# Collapsible 组件设计

## 目标

为 LCUI 增加可组合的 `collapsible` 部件，用于展开/收起任意内容，并为 Sidebar 的分组折叠提供基础能力。

## 范围

### 本版实现

- `collapsible`、`collapsible-trigger`、`collapsible-content`
- `default-open` / `open` 状态
- 点击 trigger 或任意带 `data-collapsible-trigger` 的后代节点切换
- 嵌套 Collapsible 只切换最近的一层
- 中英文文档、字段表、C/XML/TSX 示例
- 单元/集成测试

### 明确暂缓

- Sidebar 专用逻辑（由 Sidebar spec 负责）

## CSS 约束

LCUI CSS 不支持属性选择器和功能伪类，因此：

- 部件状态统一映射为 status 选择器，例如 `collapsible:open`、`collapsible:closed`
- `data-collapsible-trigger` 只用于 C 侧行为查找，不进入 CSS 选择器
- 不使用 `[data-open]`、`:has()`、`:not()`、`>`

## 结构

```text
Collapsible
├── CollapsibleTrigger（可选）
└── CollapsibleContent
```

## 行为

- `collapsible` 根节点持有 `open` / `closed` 状态，默认 `closed`
- `default-open` 属性设置初始状态
- `open` 属性可读写当前状态
- closed 时 `collapsible-content` 不参与显示
- 点击 `collapsible-trigger` 或任意带 `data-collapsible-trigger` 的后代节点切换状态
- 嵌套 Collapsible 中，点击只切换最近的 Collapsible
- 根节点在切换时发出 `open` / `close` 事件

## C API

```c
ui_widget_t *ui_create_collapsible(void);
ui_widget_t *ui_create_collapsible_trigger(void);
ui_widget_t *ui_create_collapsible_content(void);
void ui_collapsible_set_open(ui_widget_t *collapsible, bool open);
bool ui_collapsible_get_open(ui_widget_t *collapsible);
void ui_register_collapsible(void);
```

## 状态与样式

- 根节点 status：`open` / `closed`
- CSS：

```css
collapsible:closed collapsible-content {
        display: none;
}
```

- `collapsible-trigger` 可复用 button 样式
- 触发器内的旋转图标通过 `collapsible:open` 的 status 选择器处理

## 与 Sidebar 的接口

- SidebarGroupLabel 通过 `data-collapsible-trigger` 触发外层 Collapsible
- Collapsible 不依赖 Sidebar 类型，也不包含 Sidebar 专用逻辑

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

## 示例与文档

- 顶部默认示例：`collapsible-basic`
- 示例小节：
  - Basic
  - Settings
  - File Tree
- 字段表：Collapsible、CollapsibleTrigger、CollapsibleContent
- 文档中说明 `data-collapsible-trigger` 只作为行为绑定属性，不参与样式
- 中英文同步

## 测试

- 默认 closed
- `default-open` 初始 open
- Trigger 点击切换
- `data-collapsible-trigger` 后代切换
- 嵌套 Collapsible 只切换最近节点
- Content 显隐与 open 事件

## 验收

- 相关集成测试通过
- doc-viewer 示例可交互
- 不引入属性选择器或功能伪类
