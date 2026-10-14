# Message 组件设计

## 目标

`message` 编排头像、作者信息、Bubble、附件和元数据，不承担消息列表滚动策略。

## 结构与 API

```text
message
├── message-avatar
├── message-header
├── message-content
│   ├── bubble
│   └── attachment
└── message-footer
```

提供各布局子件工厂、`ui_message_set_side()`、`ui_message_set_grouped()` 和 build 方法。

## 状态与边界

- side 决定 sender/receiver 布局；grouped 隐藏重复头像/标题。
- 消息正文仍由 Bubble 管样式，附件仍由 Attachment 管状态。
- 不实现消息数据模型、编辑、删除、回复、反应或网络同步。

## 依赖与验证

依赖 Avatar、Bubble、Attachment。测试组合、side、grouped 和事件穿透；中文 demo 展示消息组。

## 文档示例

组件文档提供接收消息、发送消息、连续分组和带附件消息四个示例。