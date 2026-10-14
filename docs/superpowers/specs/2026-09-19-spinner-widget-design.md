# Spinner 组件设计

## 目标

`spinner` 通过纯 C 自绘旋转直属内容，提供可组合的加载指示器。

## 结构与 API

```text
spinner
└── spinner-content (隐藏于常规渲染，离屏捕获)
```

提供 `ui_create_spinner()`、`ui_spinner_set_running()`、`ui_spinner_set_speed()` 和 `ui_spinner_set_content()`。

## 渲染与状态

- paint 回调用 `ui_widget_render()` 将 content 捕获到复用的 ARGB canvas。
- 使用 Canvas 后端的旋转能力将内容以中心旋转合成到自身图层。
- 以 `lcui_request_frame()` 推进相位，隐藏或销毁时 cancel frame。
- 首版不提供 CSS animation、任意滤镜或多 spinner 共享时钟。

## 依赖与验证

先增加 Canvas/PandaGL software executor 的旋转能力。测试 $0^\circ$/$90^\circ$ 像素、帧生命周期、DPI 与裁切；提供 preview。

## 文档示例

组件文档提供默认 Spinner、自定义内容、不同尺寸和运行状态控制四个示例。