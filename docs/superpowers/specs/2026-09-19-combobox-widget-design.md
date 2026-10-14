# Combobox 组件设计

## 目标

`combobox` 提供输入过滤的本地单选列表，输入框为锚点，候选面板通过 Portal 显示。

## 结构与 API

```text
combobox
├── combobox-input
└── portal
    └── combobox-content
        └── combobox-item
```

提供 value、placeholder、open/close、append/clear item API。C 端通过 `ui_combobox_build_children()` 构造默认 input/content；JSX 可完整声明子树。

## 状态与边界

- 状态包含 value、query、active index、open 和 disabled。
- 支持本地 UTF-8 子串过滤、上下键、Enter、Esc、点击选择。
- 首版仅单选；不支持异步数据、多选、分组、创建新值或虚拟列表。

## Portal 与验证

依赖已收敛的 Portal，input 为 anchor，采用 bottom/start。测试过滤、键盘、outside、跨树 click 和销毁。

## 文档示例

组件文档提供基础单选、placeholder、禁用选项和键盘过滤四个示例。