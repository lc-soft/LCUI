# 布局引擎内部机制

> 布局分发、reflow 传播、更新周期、block/flexbox 流程、约束模型与测量缓存。不变量速查见 `docs/layout-engine-rules.md`。


## 总体架构

布局引擎采用**闭包分发**（closed dispatch）模式。布局算法在编译期硬编码，
通过 `display` CSS 属性的值在 `ui_widget_reflow_with_resizer()` 中做 `switch`
分发。当前有 block 和 flexbox 两种布局实现。

每个布局只需实现一个入口函数：

```c
void xxx_layout_reflow(ui_widget_t *w, ui_resizer_t *resizer);
```

**关键文件**：

| 文件 | 用途 |
|------|------|
| `lib/ui/src/ui_updater.c` | 更新循环入口、`should_reflow` 检测 |
| `lib/ui/src/ui_widget_layout.c` | 分发入口、reset_layout、reflow_if_* |
| `lib/ui/src/ui_block_layout.c` | Block 布局算法 |
| `lib/ui/src/ui_flexbox_layout.c` | Flexbox 布局算法 |
| `lib/ui/src/ui_resizer.c/h` | 共享的尺寸收集/换行/提交工具 |
| `lib/ui/src/ui_widget_style.h` | 内联辅助函数（in_layout_flow、is_inline 等） |

## 触发与传播

```
某个事件（子 widget 增删、尺寸变化、样式变化、内容变化）
  → ui_widget_request_reflow(w)
      w->update.should_reflow = true
      ui_widget_request_update(w)
          w->update.should_update_self = true
          沿 parent 链向上设置 should_update_children = true
```

## Reflow 触发机制

### 无条件 reflow vs 条件 reflow

- `ui_widget_reflow(w)`：**无条件**触发重排。创建局部 resizer，直接调用
  `ui_widget_reflow_with_resizer`。调用方必须确保所有子元素的尺寸已就绪。
- `ui_widget_reflow_if_width_changed(w)`：条件重排（宽度维度）。先解析
  `fit-content` 为实际值，调用 `update_box_width`，仅当 `content_box.width`
  或 `padding_box.width` 实际变化时才调用 `ui_widget_reflow`。
- `ui_widget_reflow_if_height_changed(w)`：同理，高度维度。

**最佳实践**：布局算法在修改子元素的 `computed_style.width/height` 后，
应调用 `reflow_if_width_changed` / `reflow_if_height_changed`，**不要**直接
调用 `ui_widget_reflow`。条件版本仅在尺寸真正变化时触发重排，避免冗余。

### Bottom-up 重排顺序

updater 先递归处理子元素（②），再做父元素自身的重排（③）。所以当父元素
的 layout reflow 运行时：

- 所有子元素的 `max_content_width/height` 已经就绪
- 所有子元素的 `min_content_width/height` 已经就绪（由 resizer 在子元素
  reflow 时记录）
- 子元素的 `outer_box` 反映的是上一帧的状态——如果父 layout 在这一帧
  修改了子元素的 computed_style，子元素需要被条件重排以反映最新值

### Load phase 纯估算约束

`load_width` / `load_main_size` 阶段应**只读取**子元素已有的尺寸（`max_content_*`、
`min_content_*`、`outer_box.*`），**不得调用** `ui_widget_reflow(child)`。
所有子元素的布局已由 bottom-up updater 完成，它们的尺寸信息已就绪。

违反此规则会导致：
1. load phase 有副作用，破坏布局阶段的单向计算数据流
2. 同一子元素在一帧内被多次 reflow，浪费 CPU（见 pass_block 历史问题）

**受控例外**：flex column 测量无 sizehint 的容器子项时，
`ui_flexbox_measure_item_height_at_width()` 会在给定宽度下 reflow 一次并写入
`measured_content_*` 缓存；后续命中缓存不再 reflow。

### 约束模型与测量缓存（flex column）

布局值分五类，排查尺寸问题时先确认问题落在哪一层：

| 层 | 字段 | 生命周期 |
|---|---|---|
| 计算样式 | `computed_style.width/height/...` | 每轮 `reset_layout` 从 `specified_style` 恢复，布局可临时改写 |
| 布局约束 | `layout_constraint.{has_definite_width, available_width, source, generation}` | 父布局写入；`ui_widget_get_available_width` 校验 `source == parent` 且 `generation == parent->layout_generation` |
| used size | `content_box` / `border_box` / `outer_box` | 每轮布局重算 |
| 内在缓存 | `max_content_*`、`min_content_*`、`unconstrained_content_*` | 子项 `update_size` 写入；有有效宽度约束时不更新 `unconstrained_*`，避免 used size 污染 |
| 测量缓存 | `measured_content_width/height` + `has_measured` | 在给定宽度下测折行高时缓存；`should_reflow` 或宽度变化时失效 |

规则：

- **约束必须在子项 `reset_layout` 之后记录**（`ui_flexbox_record_width_constraint`）。否则会读到上一轮布局写入的 `margin: auto` 实际值，算出过窄的可用宽。
- **column 子项的主轴 basis 必须在父级可用宽下测**。纯 `unconstrained_content_height`（无限宽单行高）会让文本退回单行，实测 5 个折行用例失败，因此 `measured_content_*` 缓存不能删。
- **row 主轴分配结果也要记录为宽度约束**（`ui_apply_row_item_main_size`），否则 `flex: 1` 子项单独 reflow 时会退回 max-content 宽。
- **sizehint 是否实现看 `proto->has_sizehint`**，不看返回数值是否为零。零宽/零高是合法测量结果；自定义 widget 覆盖 `sizehint` 时必须置 `has_sizehint = true`。对应查询是 `ui_widget_has_sizehint()`。
- **sizehint 只负责自有内容层**：`ui_widget_get_sizehint()` 始终可调用（默认原型是空实现，返回 0）。fit-content 与 column basis 都按 `max(自有 hint, 子项布局尺寸)` 合并；两者在 LCUI 中是同一内容区的不同表达，因此取 max 而非相加。
- **sizehint 内在宽缓存刷新**：自动宽叶子部件用 `ui_widget_get_unconstrained_sizehint()`（临时清除 used width 与父约束）刷新 `unconstrained_*`；auto 宽 flex/inline-block 容器用自身 `content_box` 刷新。否则内容变化后会继续复用旧约束下折行的宽度。
- **带流内子部件的容器不参与继承来的 sizehint 快路径**：`button`、`label`、`badge` 等从 `text` 继承原型的容器也带文本层 sizehint 实现，但自身文本层通常为空。`ui_widget_sizehint_is_exact()` 在无流内子项或原型标记 `sizehint_exact`（`text`、`canvas`、`textinput`）时为 true，只用于 `ui_flexbox_try_measure_container_height()` 这类组合测量快路径。
- **`ui_flexbox_try_measure_container_height` 测量前先 `ui_widget_reset_width(child)`**，否则 `text` 会复用上一轮布局的固定宽，不折行。
- **每次 reflow 递增 `layout_generation`**，旧约束自动失效。flex column 单独 reflow 时，只有父级仍是 flex 容器且 `align-items: center` 才从约束重推导交叉轴宽，避免 flex→block 后使用过期约束。
- fit-content 仍会把结果写回 `computed_style.width`（已知临时例外，见项目 `docs/layout-engine-rules.md`）。

flex column 的入口分为测量 / 布局两段：`ui_flexbox_measure()` 收集并解析 basis，`ui_flexbox_layout()` 分行、分配主轴/交叉轴并定位。

相关回归测试：`flex layout` → `measure cache`、`measure cache invalidates on content change`、`zero height text ignores stale cache`、`field checkbox description stable`。

项目内速查：`docs/layout-engine-rules.md`（值模型、I1–I8 不变量、已知缺口）；设计文档：`docs/superpowers/specs/2026-10-11-flex-column-text-wrap-design.md`、`2026-10-25-layout-two-phase-design.md`。

## 更新周期

每帧由 `ui_updater_update()` → `ui_updater_update_widget(updater, root)` 驱动：

```
ui_updater_update_widget(updater, w):
  ① 如果 should_update_self：
     - proto->update(UI_TASK_BEFORE_UPDATE)
     - 如果 should_refresh_style → ui_widget_match_style()
       → should_update_style = true
     - 如果 should_update_style → ui_widget_update_style()
       → ui_widget_update_box_size()
       → ui_widget_update_box_position()
     - proto->update(UI_TASK_AFTER_UPDATE)
  ② 如果 should_update_children → ui_updater_update_children()
     递归对每个 visible 子 widget 调用 update_widget
  ③ 如果 should_reflow → ui_widget_update_size(w)  ← 核心
  ④ ui_widget_update_stacking_context(w)
```

**注意执行顺序**：先更新子元素（②），再做自身重排（③）。这确保子元素的
`max_content_width/height` 在父元素做布局时已经计算好。

## 重排阶段（ui_widget_update_size）

```
ui_widget_update_size(w):                        ← lib/ui/src/ui_updater.c:333
  ① ui_widget_reset_size(w)
     将 specified_style 中的 width/height/padding/flex_basis 复制到
     computed_style（重置布局引擎在上轮可能做的修改）
  ② ui_widget_compute_style(w)
     重新计算 min/max 约束等
  ③ ui_widget_update_box_size(w)
     根据 computed_style 中的 width/height 更新各盒模型矩形
  ④ ui_widget_reflow_with_resizer(w, &resizer)   ← 布局入口
  ⑤ 记录 min/max_content_size（从 resizer 读取）
  ⑥ 如果 outer_box 尺寸变化且 w 在布局流中 → request_reflow(parent)
     否则 → ui_widget_update_box_position(w)
```

## 分发机制

```c
// lib/ui/src/ui_widget_layout.c（当前主线）
void ui_widget_reflow_with_resizer(ui_widget_t *w, ui_resizer_t *resizer)
{
    ui_event_t ev = { .type = UI_EVENT_AFTERLAYOUT, .cancel_bubble = true };

    switch (w->computed_style.type_bits.display) {
    case CSS_DISPLAY_BLOCK:
    case CSS_DISPLAY_INLINE_BLOCK:
        ui_block_layout_reflow(w, resizer);
        break;
    case CSS_DISPLAY_FLEX:
    case CSS_DISPLAY_INLINE_FLEX:
        ui_flexbox_reflow(w, resizer);
        break;
    case CSS_DISPLAY_NONE:
    default:
        break;
    }
    ui_widget_post_event(w, &ev, NULL, NULL);
    ui_widget_add_state(w, UI_WIDGET_STATE_LAYOUTED);
}
```

> `display: table` 系列值在 CSS 枚举中存在，但当前没有 table 布局实现；
> `ui_widget_reflow_with_resizer` 对它们走默认分支，不做布局。

## Block 布局流程

```
ui_block_layout_reflow(w, resizer):
  ① load_width(ctx)   — 遍历子 widget，用 resizer 收集行级宽度
     - 对 inline-block/inline-flex：同排一行，宽度累加
     - 对 block/flex：独占一行，调用 resizer_update 换行
  ② commit_row_main_size — 确定容器宽度
  ③ apply_width(ctx)  — 遍历子 widget，按行分配宽度
     - inline-block/inline-flex：设置 fit-content 宽度，超限则换行
     - block/flex：设置 fill-available 宽度（auto 时）
     - 同时处理 margin: auto居中和换行
     - 计算容器总高度
  ④ update(ctx)       — 计算每个子元素的位置
     - 遍历行列表，累加 x（同行内）和 y（行间）
     - 对 inline 元素做 vertical-align（middle/bottom）
     - 设置 child->layout_x, layout_y → ui_widget_update_box_position()
  ⑤ w->proto->resize(w, w, w->content_box.*)
```

## Flexbox 布局流程

```
ui_flexbox_reflow(w, resizer):
  ① layout_generation++；居中 auto 宽 column 从持久约束重推导交叉轴宽
  ② ui_flexbox_measure(&ctx)
     - collect_in_flow_items：逐子项 reset_layout → 记录宽度约束 →
       重解析 flex_basis（column 会用约束宽测 sizehint）
     - commit_*_main_size、解析 gap、快照 max_main_size / hint_cross_size
     - resolve_flex_basis：在已确定的主轴尺寸上再解析一次
  ③ ui_flexbox_layout(&ctx)
     - break_into_lines（非内在路径）
     - resolve_main_sizes：grow/shrink 分配并落到子项
     - resolve_cross_sizes：提交容器交叉尺寸、定位；流外子项按文档顺序
       在本趟逐项布局
  ④ w->proto->resize(w, w->content_box.*)
```

约束、缓存与 sizehint 的细节见[约束模型与测量缓存](#约束模型与测量缓存flex-column)。

### flex-direction 未显式设置时会沿用父级

LCUI 的计算样式在 `flex-direction` 为 INHERIT（未设置）时会回退到父级的值，
而不是 CSS 标准的初始值 `row`。因此放在 column 容器里的裸 `display: flex`
行会跟着变成 column，表现为 item 与 label 各占一行。嵌套的 flex 行必须显式
写 `flex-direction: row`（或 `flex-row` 类）。

## Resizer（尺寸计算器）

`ui_resizer_t` 是布局算法的共享工具，被 block 和 flexbox 共用：

```c
typedef struct ui_resizer {
    ui_widget_t *target;
    ui_sizehint_t hint;           // 来自 proto->sizehint()（实现与否看 has_sizehint）
    bool wrap;
    float line_min_main_size;     // 当前行最小主轴尺寸
    float line_main_size;         // 当前行主轴尺寸
    float min_main_size;          // 全局最小主轴尺寸（所有行的最大）
    float cross_size;             // 累计交叉轴尺寸
    float main_size;              // 累计主轴尺寸
    float max_main_size;          // 最大主轴尺寸上限
} ui_resizer_t;
```

关键操作：
- `ui_resizer_init()` — 初始化 + 获取 sizehint
- `ui_resizer_load_item_main_size()` — 加入一个元素的尺寸（处理换行）
- `ui_resizer_update()` — 换行（提交当前行，累加到全局）
- `ui_resizer_commit_row_main_size()` — 行方向布局完成后提交容器宽度
- `ui_resizer_commit_column_main_size()` — 列方向布局完成后提交容器高度

## 关键概念

### max_content_width / max_content_height

每轮 `ui_widget_update_size()` 都会写入（当前 content_box 尺寸），可能被 used
size 污染：子项在有父级宽度约束时也会把它写成约束后的尺寸。**布局算法要取
“无限宽下的内在尺寸”时，用 `unconstrained_content_*`**（仅在无有效宽度约束时
更新），不要用 `max_content_*` 当作可靠 intrinsic。工具类 widget 可用
`proto->sizehint()` 现算。

### min_content_width / min_content_height

子元素的最小内容尺寸（不可压缩的下限）。从 resizer 的 `min_main_size` /
`min_cross_size` 获取。

### ui_widget_reset_layout()

将 `specified_style` 中的布局属性（width/height/padding/margin/flex_basis
等）重置到 `computed_style`，然后调用 `ui_widget_compute_style()`。这是为
了让布局算法可以安全地修改 `computed_style` 而不影响用户的 `specified_style`。

### ui_widget_reflow_if_width/height_changed()

布局算法内部在修改子元素的 computed_style 后调用。如果 content_box 实际
变化了，递归调用 `ui_widget_reflow(child)` 让子元素重新布局。这是嵌套布局
的关键：父元素改了子元素的宽度 → 子元素重新布局 → 子元素的子元素也跟着
重新布局。

### ui_widget_in_layout_flow()

返回 `display != none && position != absolute`。布局算法遍历时跳过不在流中
的元素。


