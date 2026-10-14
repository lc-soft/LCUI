# Empty 组件设计

## 目标

`empty` 为空列表、无搜索结果等状态提供居中且可组合的占位内容。

## 结构与 API

```text
empty
├── empty-media
├── empty-title
├── empty-description
└── empty-action
```

提供这些工厂和 `ui_empty_set_media()`、`ui_empty_set_title()`、`ui_empty_set_description()`、`ui_empty_set_action()`。便捷 setter 复用显式子件，缺失时创建相应槽位；JSX 直接声明的结构优先。

## 状态与边界

- 默认纵向居中，不拥有数据加载状态。
- media/action 是可选布局槽位。
- 不实现 loading、重试和空状态插画资源管理。

## 依赖与验证

复用 Button、Text。测试布局结构、便捷 setter、action 事件和销毁。组件文档提供基础空状态、无搜索结果、带操作和自定义媒体四个示例。