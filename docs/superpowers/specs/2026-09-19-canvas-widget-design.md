# Canvas 部件重设计

## 问题与目标

现有 `canvas` 将 `pd_canvas_t`、物理像素尺寸和 `ui_get_actual_scale()` 快照暴露给调用方。调用方需要自行处理 DPI；resize 后已取得 context 还会指向被销毁的 buffer。该 API 同时把 Canvas 锁定为 Pandagl CPU 位图，无法自然接入 OpenGL 或 Direct2D。

新 Canvas 提供类似 Web Canvas 2D 的保留式绘制体验：调用方在逻辑 UI 坐标中写入命令，Canvas 在重绘时按当前渲染目标回放。调用方不读取、不设置 DPI，也不取得后端像素指针。

## 坐标与 DPI 契约

- 所有公开坐标、尺寸、线宽、圆角半径、变换和平铺间距都是逻辑坐标，即逻辑 UI 单位。
- Canvas 的逻辑绘制区域恒为 widget 的 `content_box`，`ui_canvas_width()`、`ui_canvas_height()` 返回逻辑尺寸。
- 物理 surface 尺寸由当前渲染目标按 $ceil(content\_size \times actual\_scale)$ 决定，其中 $actual\_scale = dpi / 96 \times scale$。
- DPI、窗口或 content size 改变会使后端资源失效；下次 paint 使用同一 display list 在新 target 上回放。调用方不重发绘制命令。
- 多窗口渲染时不得缓存 `ui_get_actual_scale()`；缩放从每次 paint 的 render target 取得。

## 公共 API

API 使用 `ui_draw_t` 表示 Canvas 绑定的 2D 绘制记录器。它比 Web 的
`CanvasRenderingContext2D` 短，也避免 public header 出现 Pandagl 类型或函数指针。

```c
typedef struct ui_draw ui_draw_t;

LCUI_API ui_draw_t *ui_canvas_draw(ui_widget_t *canvas);
LCUI_API float ui_canvas_width(const ui_draw_t *draw);
LCUI_API float ui_canvas_height(const ui_draw_t *draw);
LCUI_API void ui_canvas_reset(ui_widget_t *canvas);

LCUI_API void ui_draw_save(ui_draw_t *draw);
LCUI_API void ui_draw_restore(ui_draw_t *draw);
LCUI_API void ui_draw_color(ui_draw_t *draw, ui_color_t color);
LCUI_API void ui_draw_stroke(ui_draw_t *draw, ui_color_t color, float width);
LCUI_API void ui_draw_alpha(ui_draw_t *draw, float alpha);
LCUI_API void ui_draw_translate(ui_draw_t *draw, float x, float y);
LCUI_API void ui_draw_scale(ui_draw_t *draw, float x, float y);
LCUI_API void ui_draw_rotate(ui_draw_t *draw, float radians);
LCUI_API void ui_draw_clip_rect(ui_draw_t *draw, float x, float y,
                                float width, float height);

LCUI_API void ui_draw_clear(ui_draw_t *draw, float x, float y,
                            float width, float height);
LCUI_API void ui_draw_fill_rect(ui_draw_t *draw, float x, float y,
                                float width, float height);
LCUI_API void ui_draw_stroke_rect(ui_draw_t *draw, float x, float y,
                                  float width, float height);

LCUI_API void ui_draw_path_begin(ui_draw_t *draw);
LCUI_API void ui_draw_move_to(ui_draw_t *draw, float x, float y);
LCUI_API void ui_draw_line_to(ui_draw_t *draw, float x, float y);
LCUI_API void ui_draw_quad_to(ui_draw_t *draw, float cx, float cy,
                               float x, float y);
LCUI_API void ui_draw_cubic_to(ui_draw_t *draw, float cx1, float cy1,
                                float cx2, float cy2, float x, float y);
LCUI_API void ui_draw_arc(ui_draw_t *draw, float x, float y, float radius,
                          float start_radians, float end_radians);
LCUI_API void ui_draw_path_close(ui_draw_t *draw);
LCUI_API void ui_draw_path_fill(ui_draw_t *draw);
LCUI_API void ui_draw_path_stroke(ui_draw_t *draw);
```

`ui_canvas_draw()` 返回 widget 内唯一且由 Canvas 持有的记录器，调用者不得释放或跨
Canvas 销毁后保存它。每个改变 display list 的调用都会使受影响区域失效；无法可靠计算
边界的 path/transform 操作使整个 content box 失效。`ui_canvas_reset()` 删除所有命令，
恢复默认绘制状态并失效整个 content box。

首版保留 `ui_canvas_get_context()` 作为迁移兼容入口，但返回 `ui_draw_t *` 的别名；旧的
`ui_canvas_context_t`、`buffer`、`scale`、`width`、`height`、函数指针和 `release` 一律移除。

## 状态、路径与回放

- 默认 fill 为黑色，stroke 为黑色且线宽为 1，alpha 为 1，无变换、无额外 clip。
- `save`/`restore` 保存和恢复 color、stroke、line width、alpha、transform 与 clip。
- clip 以当前变换应用后与已有 clip 求交；restore 不会撤回此前写入的绘制命令。
- `path_begin` 清空当前临时路径；`path_fill`/`path_stroke` 将当前路径快照为一条 display-list
  命令。路径本身不是跨调用隐式绘制。
- Canvas command list 是保留式内容，直到 `ui_canvas_reset()` 或新增的明确删除 API 改变它；
  它不是每帧回调，也不内置动画。

## 分阶段能力

### 第一阶段

实现 reset、clear、fill/stroke rect、状态栈、translate/scale/rotate、rect clip 和直线路径。
它覆盖 Spinner/Shimmer 的旋转和裁切基础，但不把它们的动画逻辑塞入 Canvas。

### 第二阶段

实现 quadratic/cubic/arc、圆角矩形、图片绘制、source-over alpha。所有能力都由后端接口
表达并有软件回放实现。

### 后续阶段

在内部 brush/resource 模型稳定后增加渐变、mask composite 和其他必要 compositing。文本、
ImageData 读写、OffscreenCanvas、filter、pattern、shadow、虚线、完整 Web blend mode、命中
测试和 CSS 字符串字体不属于当前范围。

## 内部后端架构

公共 Canvas 只记录逻辑命令。内部将命令流和状态快照交给 `ui_draw_backend_t` 回放：

```text
Canvas API (logical display list)
        -> ui_draw_backend_t
           -> software executor (Pandagl)
           -> future renderer executor (OpenGL / Direct2D)
        -> render target / present
```

- Software executor 使用 PandaGL，但 `pd_canvas_t` 仅存在于内部 source 文件。
- 后端接口接收逻辑 display list、物理 target 与实际 scale；负责 clip、transform、alpha 和
  source-over，不要求 CPU buffer 中转。
- OpenGL/Direct2D 不是 Canvas 单独的旁路：在接入前，UI renderer 的 background、border、
  圆角裁剪、opacity layer 与 Canvas 都必须迁移到同一 render context/surface 抽象，避免
  readback/upload 与双渲染语义。
- PTK 只提供窗口、原生 handle、DPI 和 present 生命周期；不得继续成为 `pd_context_t` 的别名。
- 后端可通过内部 adapter 接入成熟开源图形库。PandaGL 保持默认软件参考实现；首个实验性 adapter 选择 Skia，用其 raster surface 验证命令、layer、clip 和资源生命周期，再决定是否启用其 GPU surface。Cairo 仅用于 CPU 互操作/导出，NanoVG 仅用于独立试验；两者均不是主 renderer 候选。

## Spinner 与 Shimmer

- Spinner 使用 Canvas 绘制记录器保存其视觉内容；widget 帧回调只更新 angle、请求重绘，
  Canvas 后端以逻辑 rotate 回放，不暴露离屏像素或 DPI。
- Shimmer 使用 Canvas 的后续 mask/brush 支持表达遮罩与亮带；在该能力实现前，Shimmer 不得
  通过 public `pd_canvas_t` 访问绕过后端抽象。
- 两个部件仍由 `lcui_request_frame()` 管理动画；隐藏、停止、销毁时取消 frame。

## 测试与文档

- 集成测试须覆盖逻辑尺寸、$96$/$192$ DPI 同形状、resize 后保存的 draw 指针、状态栈、变换、
  clip、reset、销毁和 dirty 区域。
- 软件后端增加像素断言；未来 OpenGL/Direct2D 运行相同行为测试，对抗锯齿结果采用容差断言。
- 增加 Canvas 中文字段、MDX、C/XML/TSX demo 与侧栏；中文审核后才同步英文。
- Canvas 文档提供填充/清除、状态栈与变换、裁切、DPI 透明行为和自定义绘制部件五个示例。