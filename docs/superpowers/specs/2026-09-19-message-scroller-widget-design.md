# MessageScroller 组件设计

## 目标

`message-scroller` 基于 ScrollArea 承载 Message 列表，并实现不打断阅读的贴底跟随。

## 结构与 API

```text
message-scroller
├── message-scroller-content
│   └── message
└── message-scroller-jump
```

提供 append/prepend、`ui_message_scroller_scroll_to_bottom()` 和 `ui_message_scroller_is_at_bottom()`；JSX 可直接使用 content 和 jump 子件。

## 状态与边界

- 仅在原本位于底部时，追加消息后自动滚到底。
- 用户离开底部时不改变阅读位置，显示 jump 操作。
- 首版不支持虚拟列表、历史分页、未读计数、锚点保留或流式 token 布局。

## 依赖与验证

依赖 ScrollArea 和 Message。测试追加贴底、离底保持、jump 与销毁；中文 demo 展示长消息列表。

## 文档示例

组件文档提供基础消息列表、贴底追加、离底后的跳到底部操作和长列表四个示例。