# Item 组件设计

## 目标

`item` 是用于设置、列表和快捷操作的通用单行/多行布局容器，不表达选择值。

## 结构与 API

```text
item
├── item-media
├── item-content
│   ├── item-title
│   └── item-description
└── item-actions
```

提供各子件工厂、`ui_item_set_disabled()` 和 `ui_item_set_interactive()`。C 端 build 方法只构造缺失布局节点，JSX 直接组合。

## 状态与边界

- 支持 default、outline、muted 三个 `variant`，分别对应透明、带边框和低强调背景；interactive 决定 hover/active 样式；disabled 阻止 Item 自身触发的激活事件。
- `item-media` 支持 default、icon、image 三个 `variant`，只负责媒体槽位尺寸和裁切；不引入 Avatar 依赖。
- 不实现 selected、checked、value 或键盘列表导航。
- 菜单项和 Select 选项使用各自组件，不复用 Item 的状态机。

## 依赖与验证

测试结构、disabled、click 和嵌套 action；中文 demo 展示设置项。

## 文档示例

组件文档至少提供基础 Item、Variant、Icon、Image、带描述与操作示例，并复用 shadcn Item 文档的结构和文案。