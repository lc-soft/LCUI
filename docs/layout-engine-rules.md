# 布局引擎规则与流程

本文档汇总 LCUI 布局引擎当前**生效的规则、布局流程与已知缺口**，面向布局代码贡献者。
设计动机见 `docs/superpowers/specs/2026-07-25-layout-constraint-preservation-design.md`
与 `docs/superpowers/specs/2026-10-11-flex-column-text-wrap-design.md`；本文只描述
「引擎现在遵守什么」。

## 1. 值模型

布局引擎区分三类值：

1. **计算样式值**：CSS 语义值（`auto`、`100%`、固定值等），存于 `computed_style`。
   `reset_size()` 会把它从 `specified_style` 恢复。
2. **布局约束**：父级在本次布局中提供给子项的确定尺寸，带来源父级 `source` 与
   轮次 `generation`。持久化在 widget 上，独立于 `computed_style`。
3. **used size**：前两者经 min/max 与盒模型规则解析出的实际尺寸，落到
   `content_box` / `padding_box` / `border_box` / `outer_box`。

规则：

- 父级分配的可用宽只能写入布局约束，不能依赖 `computed_style` 承载——
  `reset_size()` 会把它擦除。
- `computed_style` 只承载 CSS 语义；布局结果不应永久写回。

> 现状例外：居中 auto 宽子项的 fit-content 钳制、以及居中列容器的交叉轴重推导，
> 仍会把 used width 写入 `computed_style.width`。它们每次都从约束重算、不依赖
> 上一轮残留，但属于已知的设计折中。

## 2. 约束生命周期

### 写入

- 时机：flex column 容器在**主轴测量之前**，由
  `ui_flexbox_record_width_constraints()` 为每个 in-flow 且 `width:auto` 的子项记录。
- 值：`available_width = 容器 content_box.width − margin − border − padding`
  （不小于 0）。
- 门控：仅当容器交叉轴（宽）为确定值（`IS_CSS_FIXED_LENGTH(width)`）。
- 版本：`generation = 容器 layout_generation`（每次 reflow 递增）。

### 读取

- 文本 `sizehint` 的宽优先级：自身固定宽 > 有效约束 > `max-width` > 不限。
- `ui_flexbox_compute_column_item_flex_basis()` 用有效约束宽现算折行高。

### 失效

- 仅当 `source == 当前父级` 且 `generation` 未过期时才复用。
- 过期约束丢弃，退回 intrinsic/fit-content 路径。
- 父级每次重新布局子项时刷新约束并递增 generation。

## 3. 布局流程

### 3.1 reflow 入口（`ui_flexbox_reflow`）

1. `layout_generation++`。
2. **居中列容器交叉轴重推导**：若容器是 column、`width:auto`、父级
   `align-items:center` 且有有效宽约束，则从约束重算 fit-content 宽并落地
   （规范 §9.4「treating used size as its definite cross size」）。
3. 解析 row/column gap，映射 main/cross。
4. 按主轴方向加载 min/max info。
5. 执行六趟布局。
6. 调用 widget 的 resize 回调。

### 3.2 六趟

1. **collect_in_flow_items**：记录宽约束（`record_width_constraints`）、测量主轴
   尺寸；内在分行由 `collect_intrinsic_lines` 处理。
2. **resolve_flex_basis**：在已分好的行上重解析 flex-basis。
3. **break_into_lines**：按主轴可用尺寸分行（非内在路径）。
4. **resolve_main_sizes**：分配主轴空间（grow/shrink/auto margin），只写尺寸。
5. **resolve_cross_sizes**：固化交叉尺寸、定位；把宽高合并为一次条件 reflow。
6. **out_of_flow**：布局流外（绝对定位）子项。

### 3.3 reflow 传播

- 子项 used size 变化 → `request_reflow(自己)` + `request_reflow(parent)`
  （经 `ui_style_diff_end` 的 `inner_changed`）。
- 挂载（`ui_widget_append`）：只 `request_reflow(parent)` + 递归刷新子孙**样式**；
  子孙 reflow 依赖样式差异间接触发，或父级重排时被动布局。

## 4. 不变量清单

| # | 规则 | 代码锚点 | 违反后果 |
|---|---|---|---|
| I1 | 约束先于主轴测量 | `record_width_constraints` 在 collect 测量前 | 文本按单行测高，折行高不传播 |
| I2 | 父级分配的宽走约束，不写子项 `computed_style` | `ui_widget_set_available_width_constraint` | 单独 reflow 被 `reset_size` 擦除，宽度漂移 |
| I3 | 约束仅在 source/generation 有效时复用 | `ui_widget_get_available_width` | 读到过期的父级宽度 |
| I4 | 每子项每次父布局 ≤1 次 reflow | 主轴趟只写尺寸；交叉趟 `reflow_if_size_changed` | 多轮往返、抖动 |
| I5 | row 路径逐字节不变 | row 分支保持单维回流 | 破坏精确几何断言 |
| I6 | 居中 auto 宽子项按 fit-content 钳制 | `ui_flexbox_fit_content_width` | 单行溢出容器 |
| I7 | fit-content 双源：实现 sizehint 的部件用测量值（零也合法）、否则 `max_content` 兜底 | `ui_flexbox_fit_content_width`、`ui_widget_has_sizehint` | button 等 flex 容器被填满；零宽内容读到过期缓存 |
| I8 | 独立 reflow 时从约束重推导交叉轴宽 | `ui_flexbox_reflow` 入口 | hover/路由切换后塌成单行 |

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
| 零尺寸内容 | `zero height text ignores stale cache` |
| 单独重排稳定 | `standalone reflow stays wrapped`、`empty description reflow stable` |
| 重新挂载 / row grow 宽 | `field checkbox description stable`（remount、field-content standalone reflow） |
| 每子项 ≤1 reflow | `child reflow count` |
| 内在 / 固定 / grow / row | `indefinite width single line`、`fixed width child wrap`、`grow child main size`、`browser layout` |
| empty / empty-basic | `empty description wrap`、`empty basic learn more centered` |
| field-checkbox | `field checkbox description stable` |
