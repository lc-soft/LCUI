# Shimmer 组件设计

## 目标

`shimmer` 以纯 C 自绘的移动亮带遮罩内容，表示加载中的内容轮廓。

## 结构与 API

```text
shimmer
└── shimmer-content (隐藏于常规渲染，离屏捕获为 alpha mask)
```

提供 `ui_create_shimmer()`、`ui_shimmer_set_running()`、`ui_shimmer_set_speed()`、`ui_shimmer_set_highlight_width()` 和 `ui_shimmer_set_content()`。

## 渲染与状态

- 离屏渲染直属 content，保留其 alpha 作为遮罩。
- 每帧生成平移亮带，用 Canvas 后端的 mask composite 能力调制 alpha 后合成。
- canvas 仅在尺寸/DPI 变化时重建，隐藏或销毁时取消 frame。
- 首版不支持 CSS gradient、任意颜色渐变或多方向扫光。

## 依赖与验证

依赖 Canvas 后端的 alpha-mask 能力。测试 mask alpha、亮带相位、帧取消和高 DPI；提供 preview。

## 文档示例

组件文档提供文本骨架、卡片骨架、自定义亮带宽度和运行状态控制四个示例。