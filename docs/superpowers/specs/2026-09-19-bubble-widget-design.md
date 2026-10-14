# Bubble 组件设计

## 目标

`bubble` 为聊天正文提供可复用的消息表面和对齐语义。

## 结构与 API

`bubble` 是内容容器，可包含 Text、Attachment 或自定义节点。提供 `ui_create_bubble()`、`ui_bubble_set_side()` 和 `ui_bubble_set_variant()`。

## 状态与边界

- side：start、end、system；variant：default、muted、error。
- 仅管理背景、最大宽度、圆角和对齐，不管理发送者或时间。
- 不实现反应、折叠、编辑或消息动作菜单。

## 依赖与验证

仅依赖 CSS/基础容器。测试 side/variant status 和嵌套附件布局；中文 demo 展示收发消息。

## 文档示例

组件文档提供接收消息、发送消息、系统消息和错误消息四个示例。