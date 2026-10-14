# Alert 组件设计

## 目标

`alert` 用于在局部区域呈现非阻塞提示，组合图标、标题、描述和可选操作。

## 结构与 API

```text
alert
├── alert-icon
├── alert-content
│   ├── alert-title
│   └── alert-description
└── alert-action
```

提供各子件工厂、`ui_alert_set_variant()`、`ui_alert_set_icon()`、`ui_alert_set_title()` 和 `ui_alert_set_description()`。Title/Description 的 setter 查找或补齐 `alert-content`，再在其中查找或创建文本子件；JSX 可直接描述完整结构。`alert-content` 负责纵向排列标题和描述，避免 Alert 依赖不支持的 grid 布局。

## 状态与边界

- 支持 default、info、success、warning、destructive variant。
- 不自动关闭，不管理队列，也不替代 toast。
- action 只是布局槽位，事件由调用者绑定。
- Alert 不定义 `alert-icon` 子部件。图标直接使用 `icon`；其字体由应用侧 `@lcui/fluent-icons` 的 `@font-face` CSS 与 TTF 资源提供，`@lcui/cli` 负责将资源编入应用。未来的 `@lcui/lucide-icons` 采用同一资源模型。

## 依赖与验证

复用 Button/Text/Icon。测试声明式装配、Content 自动补齐、便捷 setter 复用/补齐、variant 和 action 点击冒泡。组件文档默认示例复用 shadcn 的 Payment successful 与 New feature available 两个提示，并新增“变体”小节展示各变体。