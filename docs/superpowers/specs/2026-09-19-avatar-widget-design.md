# Avatar 组件设计

## 目标

`avatar` 以统一的圆形或圆角容器展示本地或远程头像资源，并在加载中或图片不可用时展示声明式后备内容。

## 结构与 API

```text
avatar
├── avatar-image
├── avatar-fallback
└── avatar-badge
```

提供 `ui_create_avatar()`、`ui_create_avatar_image()`、`ui_create_avatar_fallback()`、`ui_avatar_set_src()` 和 `ui_avatar_set_alt()`。`src` 接受本地路径或 HTTPS URI；JSX 保留相同子节点结构。

## 状态与边界

- 本地路径交给现有 `ui_image` 异步解码；HTTPS URI 交给独立 `ui-resource` 模块下载、缓存后再交给 `ui_image`。
- `avatar-image` 成功加载时显示；加载中、失败或没有 src 时显示 fallback。
- 首版仅支持单头像、静态图片和文本/图标 fallback，不让 Avatar 管理下载线程、缓存或重试。
- 不实现头像组、溢出计数或在线状态业务语义。

## 依赖与验证

依赖 `2026-09-19-resource-module-design.md` 和现有背景图像加载能力。集成测试覆盖本地/远程 src、共享请求、loading、fallback、错误和销毁。组件文档提供本地头像、远程头像、缩写 fallback 和头像状态徽标四个示例。