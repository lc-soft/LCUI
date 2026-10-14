# Card 组件设计

## 目标

`card` 为相关内容提供有边框、背景和间距的语义化容器。

## 结构与 API

```text
card
├── card-header
│   ├── card-title
│   └── card-description
├── card-content
└── card-footer
```

提供五个工厂；`ui_create_card()` 不强制创建子节点，也不提供 `ui_card_build_children()`。为 C 调用方提供 `ui_card_set_title()`、`ui_card_set_description()`、`ui_card_set_action()`、`ui_card_set_content()` 和 `ui_card_set_footer()`：setter 复用对应 slot，缺失时创建 slot，并替换该 slot 的受管内容；JSX 保持直接追加任意 Header、Content、Footer 子树的自由度。

## 状态与边界

- 子件是布局语义，不保存控制器状态。
- 支持 compact status 调整内边距。
- 不实现折叠、选择、拖拽或可点击 Card 行为。

## 依赖与验证

仅依赖容器和 Text。测试工厂、声明式结构、slot setter 的复用/替换、compact 样式和销毁。组件文档提供基础 Card、带描述和操作、表单内容以及自定义 Footer 四个示例。