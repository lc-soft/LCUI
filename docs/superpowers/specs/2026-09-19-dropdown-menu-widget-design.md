# DropdownMenu 组件设计

> 本文描述 DropdownMenu 首版设计。子菜单、checkbox/radio item、快捷键展示和类型搜索已在后续版本实现，当前设计见 `2026-10-25-dropdown-menu-design.md`。

## 目标

`dropdown-menu` 提供锚定触发器的平面命令菜单。

## 结构与 API

```text
dropdown-menu
├── dropdown-menu-trigger
└── portal
    └── dropdown-menu-content
        ├── dropdown-menu-item
        ├── dropdown-menu-label
        └── dropdown-menu-separator
```

提供 open/close、anchor、side/align setter 及各子件工厂。build 函数复用用户提供的 trigger/content，缺失时才构造默认节点。

## 状态与边界

- 支持点击、Esc、outside-close、上下键和 Enter 激活。
- disabled item 不可激活；item click 后默认关闭。
- 首版不支持子菜单、checkbox/radio item、快捷键展示或类型搜索。

## Portal 与验证

Portal 负责定位和逻辑 click 转发。测试触发、方向键、关闭策略、嵌套 Portal 与逻辑事件路径。

## 文档示例

组件文档提供基础菜单、带标签和分隔线、禁用项、不同定位四个示例。