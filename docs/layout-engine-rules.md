# 布局引擎规则与流程

本文档汇总 LCUI 布局引擎当前**生效的规则、布局流程与已知缺口**，面向布局代码贡献者。
引擎架构、各布局流程与测量缓存细节见 `docs/internals/layout-engine.md`。
设计动机见 `docs/superpowers/specs/2026-07-25-layout-constraint-preservation-design.md`
与 `docs/superpowers/specs/2026-10-11-flex-column-text-wrap-design.md`；本文只描述
「引擎现在遵守什么」。

## 1. 值模型

布局引擎区分五类值：

1. **计算样式值**：CSS 语义值（`auto`、`100%`、固定值等），存于 `computed_style`。
   `reset_size()` 会把它从 `specified_style` 恢复。
2. **布局约束**：父级在本次布局中提供给子项的确定尺寸，带来源父级 `source` 与
   轮次 `generation`。持久化在 widget 上，独立于 `computed_style`。
3. **used size**：前两者经 min/max 与盒模型规则解析出的实际尺寸，落到
   `content_box` / `padding_box` / `border_box` / `outer_box`。
4. **内在缓存**：`max_content_*`、`min_content_*`、`unconstrained_content_*`。
   `max_content_*` 每轮都会被 used size 覆盖；有有效宽度约束时不更新
   `unconstrained_content_*`，它是布局可用的可信内在尺寸。
5. **测量缓存**：`measured_content_width/height` + `has_measured`。用于“在给定宽度下
   测折行高”的快路径；缓存宽度变化时失效。`ui_widget_request_reflow()` 会清空部件
   自身及所有祖先的 `has_measured`，因此同一轮 flex basis 各趟可以复用新结果，而
   内容或样式变化后又必须重新测量；不能用更新期间持续置位的 `should_reflow`
   充当缓存失效条件。

规则：

- 父级分配的可用宽只能写入布局约束，不能依赖 `computed_style` 承载——
  `reset_size()` 会把它擦除。
- `computed_style` 只承载 CSS 语义；布局结果不应永久写回。
- 父级宽度不确定时，块级流内元素的百分比 `width` 计算为 `auto`（由父布局
  分配），行内/绝对定位元素为 fit-content。不得保留原始百分数并把 `100`
  当像素使用，否则整棵子树会按 100px 排版。
- 在 column flex 容器里，`width:auto` 与百分比宽度的 block/flex 容器子项都会在
  父宽确定时记录内容宽约束，并在该宽度下重新测量 auto 高度。父宽不确定时百分比
  仍按既有规则退回 auto/fit-content，不能提前把百分数当像素使用。

> 现状例外：居中 auto 宽子项的 fit-content 钳制、以及居中列容器的交叉轴重推导，
> 仍会把 used width 写入 `computed_style.width`。它们每次都从约束重算、不依赖
> 上一轮残留，但属于已知的设计折中。

## 2. 约束生命周期

### 写入

- 时机：flex column 容器在每个子项 `reset_layout` 之后、主轴测量之前，由
  `ui_flexbox_record_width_constraint()` 为 in-flow 且 `width:auto` 或百分比宽度的
  子项记录。晚于 `reset_layout` 是为了避免读到上一轮布局写入的 `margin:auto`
  实际值。
- 值：由 `ui_flexbox_resolve_measure_width()` 只解析一次。auto 按有效 align 与
  auto-margin 规则选择 stretch 可用宽或 fit-content；百分比先由 `compute_style`
  解析成固定 CSS 宽。两者随后都经 `ui_widget_fix_width()` 应用 min/max，再换算成
  非负的子项内容宽。该值同时用于组合测量、嵌套 reflow、used width 落地和
  `measured_content_width`，不得在后续路径另算另一套宽度。
- 门控：仅当容器交叉轴（宽）为确定值（`IS_CSS_FIXED_LENGTH(width)`）。
- 版本：`generation = 容器 layout_generation`（每次 reflow 递增）。

### 读取

- 文本 `sizehint` 的宽优先级：自身固定宽 > 有效约束 > `max-width` > 不限。
- `ui_flexbox_compute_column_item_flex_basis()` 用有效约束宽现算折行高，适用于
  `width:auto` 和百分比宽度的 flex 容器子项。按 max-content 宽度得到的
  `unconstrained_content_height` 可能少算折行，固定高子项的文本 sizehint 也可能小于
  声明高度；两者都不能直接当作 stacked children 的完整 outer height，否则父容器
  高度偏小、子项被 flex shrink 压扁。
- 无回流的组合测量快路径先对每个子项调用 `ui_widget_reset_width()` 和
  `ui_widget_reset_height()`，再读取 margin、盒模型或 sizehint。这样固定高使用恢复
  后的声明高度而不是上一轮 shrink 结果，auto margin 也不会混入内在高度。
- 快路径按子项 outer height 累加：声明固定高时经 `fix_height` 和盒模型换算，
  auto 高时才使用内容 sizehint。返回值与 reflow 路径一致，都是容器的 content-box
  高度；容器自身 padding/border 在 flex basis 换算时统一加入。
- auto 宽子项的“已测量”提前返回也必须比较当前约束宽与缓存宽；只有同宽结果
  才能复用，不能在宽度解析变化后继续返回旧高度。
- `ui_flexbox_item_max_main_size()` 在同宽测量有效时同样使用
  `measured_content_height`，不得在内容收起后再用更大的旧
  `unconstrained_content_height` 充当主轴尺寸下限。

### 失效

- 仅当 `source == 当前父级` 且 `generation` 未过期时才复用。
- 过期约束丢弃，退回 intrinsic/fit-content 路径。
- 父级每次重新布局子项时刷新约束并递增 generation。

## 3. 布局流程

### 3.1 reflow 入口（`ui_flexbox_reflow`）

1. `layout_generation++`。
2. **居中列容器交叉轴重推导**：若容器是 column、`width:auto`、父级仍是 flex 容器且
   `align-items:center` 且有有效宽约束，则从约束重算 fit-content 宽并落地
   （规范 §9.4「treating used size as its definite cross size」）。
3. 解析 row/column gap，映射 main/cross。
4. 按主轴方向加载 min/max info。
5. 执行测量与布局两段。
6. 调用 widget 的 resize 回调。

### 3.2 测量与布局两段

1. **`ui_flexbox_measure`**：
   - `collect_in_flow_items`：逐子项 `reset_layout` → `record_width_constraint`
     → 首次 basis；内在分行由 `collect_intrinsic_lines` 处理。
   - 提交主轴尺寸、解析 gap、快照 `max_main_size` / `hint_cross_size`。
   - `resolve_flex_basis`：在已确定的主轴尺寸上重解析 basis。
2. **`ui_flexbox_layout`**：
   - `break_into_lines`：按主轴可用尺寸分行（非内在路径）。
   - `resolve_main_sizes`：分配主轴空间（grow/shrink/auto margin），只写尺寸。
   - `resolve_cross_sizes`：固化交叉尺寸、定位；把宽高合并为一次条件 reflow。
   - `ui_flexbox_layout_out_of_flow`：主布局结束后定位绝对定位子项；静态位置按
     “容器中唯一 flex item”计算（`justify-content` + `align-items`）。
   - `display:none` 子项在收集阶段直接跳过，不进入 `line->items`，gap、justify
     和分行都只统计流内子项。

### 3.3 reflow 传播

- 子项 used size 变化 → `request_reflow(自己)` + `request_reflow(parent)`
  （经 `ui_style_diff_end` 的 `inner_changed`）。
- 挂载（`ui_widget_append`）：只 `request_reflow(parent)` + 递归刷新子孙**样式**；
  子孙 reflow 依赖样式差异间接触发，或父级重排时被动布局。
- block 定位阶段遇到百分比 `height`、`min-height` 或 `max-height` 时都会重置高度
  并条件 reflow，确保父级内容高变化后，百分比最小/最大高度也重新解析。

## 4. 不变量清单

| # | 规则 | 代码锚点 | 违反后果 |
|---|---|---|---|
| I1 | 约束在子项 reset_layout 之后、主轴测量之前记录 | `record_width_constraint` | 文本按单行测高，或读到陈旧的 auto margin |
| I2 | 父级分配的宽走约束，不写子项 `computed_style` | `ui_widget_set_available_width_constraint` | 单独 reflow 被 `reset_size` 擦除，宽度漂移 |
| I3 | 约束仅在 source/generation 有效时复用 | `ui_widget_get_available_width` | 读到过期的父级宽度 |
| I4 | 每子项每次父布局 ≤1 次 reflow | 主轴趟只写尺寸；交叉趟 `reflow_if_size_changed` | 多轮往返、抖动 |
| I5 | row 路径逐字节不变 | row 分支保持单维回流 | 破坏精确几何断言 |
| I6 | 居中 auto 宽子项按 fit-content 钳制 | `ui_flexbox_fit_content_width` | 单行溢出容器 |
| I7 | fit-content 合并：自有 `sizehint` 与子项布局尺寸按轴取 `max`；`sizehint` 始终可调用，零值合法；`sizehint_exact` 仅用于组合测量快路径 | `ui_flexbox_fit_content_width`、`ui_widget_sizehint_is_exact` | button 等 flex 容器被填满；零宽内容读到过期缓存 |
| I8 | 独立 reflow 时从约束重推导交叉轴宽 | `ui_flexbox_reflow` 入口 | hover/路由切换后塌成单行 |
| I9 | 测量前恢复子项声明的宽高和双向 margin | `ui_flexbox_try_measure_container_height` | shrink 高度、旧 auto margin 污染内在高度 |
| I10 | 测量宽只解析一次，快路径、reflow、缓存键共用 | `ui_flexbox_resolve_measure_width` | 同结构因路径不同而折行/父高不一致 |

## 5. 已知缺口 / deferred

- **高度约束未实现**：row 容器的交叉轴（高）没有对应的持久约束。
- **block 布局未接入**：约束保持设计的 block 路径尚未实现。
- **`flex-start` / `flex-end` 未钳制**：非居中、非 stretch 的 auto 宽子项仍可能溢出。
- **definite + wrap 的交叉轴约束用容器宽**：多列场景应按行宽（`line_max_cross_size`）约束。
- **无 sizehint 的嵌套容器测量仍触发一次 reflow**：sizehint 组合快路径只覆盖可测子项。
- **`max_content_width` 污染**：used size 被当作 intrinsic contribution
  （见 `docs/issues/used-size-pollutes-intrinsic-contribution.md`）。
- **fit-content 写 `computed_style`**：现阶段的折中，详见第 1 节。

## 6. 测试映射

| 规则 | 测试 |
|---|---|
| 换行 / 约束 | `flex layout` → `flex column text wrap`、`constraint refresh` |
| 百分比 / auto 宽度 | `css computed` → `percentage width with indefinite parent`；`home layout` |
| 零尺寸内容 | `zero height text ignores stale cache` |
| 单独重排稳定 | `standalone reflow stays wrapped`、`empty description reflow stable` |
| 重新挂载 / row grow 宽 | `field checkbox description stable`（remount、field-content standalone reflow） |
| 每子项 ≤1 reflow | `child reflow count` |
| 内在 / 固定 / grow / row | `indefinite width single line`、`fixed width child wrap`、`grow child main size`、`browser layout` |
| empty / empty-basic | `empty description wrap`、`empty basic learn more centered` |
| field-checkbox | `field checkbox description stable` |
| field-choice-card | `field choice card alignment` |
| sizehint 能力 | `sizehint`（`sizehint capability`、`text single line hint`、`nested container height`） |
| flex 语义契约 | `flex contract`（`F01`–`F19`、`C02`–`C10`） |
| widget 场景 | `empty layout`（`empty basic`）、`field layout`（`field choice card`、`field checkbox row`） |
| 百分比父级 / 固定高子项 | `gallery layout`（`gallery empty hover`）、`card`（`card basic header`） |
| shrink / auto margin 重测 | `flex layout`（`fixed height intrinsic restoration`、`auto margin intrinsic height`） |
| 测量路径与盒模型一致 | `flex layout`（`measurement path width consistency`、`percentage measurement constraints`） |
| 百分比高度失效 | `flex layout`（`browser layout`）、`block layout` |
| Home clip-safe 间距 | `gallery layout`（`clip-safe body should keep ... padding`） |
| block 内容展开/收起 | `gallery layout`（`gallery accordion resize`） |
| text sizehint / hover 绘制稳定 | `gallery layout`（`production description hover`） |
| 捕获 scale / 文本完整性 | `shimmer`（`shimmer text capture`） |
| Home 首挂与重挂布局 | `gallery layout`（`gallery initial mount`、`gallery remount`）、`production empty icon` |
