# Flex 列布局文本换行设计

## 背景

`flex-direction: column` 容器内的文本子项，在容器交叉轴（宽）已确定的情况下，长文本不换行：子项主轴（高）在测量时用的是"无宽约束"的单行 `max_content_height`，而子项的宽要到布局后段才被 stretch 定下，定宽后文本虽折行、但主轴高已被锁成单行，导致文本溢出容器。

另一个问题是子项会被**单独 reflow**（hover、内容变化等，不重跑父级布局趟）。`ui_widget_update_size()` 开头会调 `reset_size()`，把 `computed_style.width` 从 `specified_style` 恢复（擦回 AUTO）。所以只要父级分配的可用宽依赖 `computed_style` 承载，就会在这个 reset 窗口里丢失：子项先按 AUTO 错测成单行、再把错误尺寸经 `request_reflow(parent)` 向上传递，引发多轮往返、高度在折行/单行间跳动。

结论：**父级分配的可用宽必须独立于 `computed_style` 持久化**，不被 `reset_size` 擦除，且子项单独 reflow 时能校验并复用它。

本设计是 `2026-07-25-layout-constraint-preservation-design.md`（下称"约束保持设计"，尚未实现）的一个**收窄落地**：采用其"布局约束"值模型与约束失效校验机制，范围限定在 flex column 文本换行与单独重排稳定。

## 目标

- column 容器交叉轴确定时，其 stretch/auto 宽文本子项按容器可用宽折行，主轴高反映折行结果。
- 子项单独 reflow（hover、内容变化等）时复用父级约束，高度稳定、不抖动、不与兄弟重叠。
- 约束独立于 `computed_style`，不被 `reset_size` 擦除；父级不覆写子项 `computed_style.width`。
- 父级不回写子项 `min_content_*` / `max_content_*`（它们是子项脱离容器独立重排的固有结果）。
- 每个子项在一次父级布局内最多触发一次 reflow。
- flex 布局流程重整为六趟，职责清晰、顺序单向。

## 非目标

- 不实现约束保持设计的高度约束路径、block 布局接入、完整样式签名与定向失效传播（其阶段 2/3）；本设计只做宽度约束路径（其阶段 1）在 flex column 的落地。
- 不实现浏览器级完整 intrinsic sizing 规范。
- 不增加无界的"反复重排祖先直到稳定"循环；收敛依赖父级约束的有界复用（有效性校验见"布局约束表示"）。
- 不永久修改 `computed_style.width/height` 来保存父级分配的 used value。

## 值模型

沿用约束保持设计的三分：

1. **计算样式值**：CSS 语义，如 `auto`、`100%`、`fit-content`、固定值。存于 `computed_style`，`reset_size` 会从 `specified_style` 恢复它。
2. **布局约束**：父级提供的临时确定尺寸（本设计只涉及**宽**），带来源父级 `source` 与轮次版本号 `generation`（标识这条约束出自父级第几轮布局，用于判断它是否已过期）。独立存放，不进 `computed_style`。
3. **used box size**：计算样式值结合约束、再经 min/max 与盒模型规则得出的实际尺寸。

父级提供约束时不修改子项计算样式：

```text
child computed width   = auto
parent content width   = 418px
child layout constraint = 418px (source=parent, generation=N)
child used width        = resolve(auto, 418px) = 418px
child text height       = sizehint(available_width=418px) = 折行高
```

## 布局约束表示

约束状态**必须持久化在 widget 上**（跨 reflow 存活），因为子项单独 reflow 时父级不在场、`ui_resizer_t` 已重建，只有 widget 上的约束可被复用；父级布局趟内可经 `ui_resizer_t` / reflow context 传递，但权威副本在 widget。最小字段：

- `has_definite_width`：是否存在确定的宽约束；
- `available_width`：受约束的内容宽（已扣父级分配给该子的 margin/border/padding）；
- `constraint_source`：提供约束的父级 widget；
- `constraint_generation`：标识父级布局轮次的版本号。

约束是盒尺寸计算与文本测量的输入，**不复制到 `specified_style`、不替换 `computed_style` 的语义值**。落点优先在现有 `ui_resizer_t` 与 `ui_widget_reflow_with_resizer()` 流程上做最小状态扩展。

**有效性规则**：子项因自身样式/内容更新而单独 reflow 时，只有 `constraint_source` 仍是其当前父级、且 `constraint_generation` 未过期，才复用该约束；过期约束必须丢弃，退回 intrinsic/fit-content 路径，不得静默当作当前包含块尺寸。父级每次重新布局子项时刷新约束并递增 generation。

## 六趟布局流程

`ui_flexbox_reflow` 重整为六个职责单一的趟（命名即产出）：

1. `collect_in_flow_items`：遍历子项，排除布局流外者，测量主轴尺寸。宽度约束记录与内在分行已拆为独立函数（`record_width_constraints` / `collect_intrinsic_lines`）。
2. `resolve_flex_basis`：计算各子项主轴 flex-basis。column 时主轴为高：对交叉轴 AUTO 的子项，用**布局约束的 `available_width`** 调 `sizehint` 现算折行高作为 basis 来源；无约束（indefinite）时退回 `max_content_height`（单行）。
3. `break_into_lines`：按主轴尺寸分行（合并现状分散在 `load_main_size` 与 `apply_main_size` 的两处分箱逻辑）。
4. `resolve_main_sizes`：固化主轴尺寸，处理 flex-grow/shrink、justify-content/justify-self 位置，并据以定容器自身主轴尺寸。
5. `resolve_cross_sizes`：固化交叉轴尺寸，处理 align-items/align-self、stretch；把最终 used width 落到子项并触发子项**唯一一次**条件 reflow。子项的 `available_width` 约束已在本趟之前（见下"顺序要点"）记录、本轮 generation 有效，供此后单独 reflow 复用。
6. `layout_out_of_flow`：处理布局流外（绝对定位）子项的位置与尺寸。

**顺序要点（策略 C）**：容器自身的确定内容宽 `W` 在本次 reflow 入口即已知（来自其父级约束或自身样式）。在第一趟测量子项之前（`record_width_constraints`），为每个 in-flow 子项记录 `available_width = W − 子项 margin/border/padding`（source=容器、generation=本轮）；`resolve_flex_basis` 据此现算折行高。`resolve_cross_sizes` 只负责把 used width 落到子项并触发唯一 reflow。单向链：

1. `W`：父 flex 容器的确定内容宽；
2. `cw`：子项可用内容宽 = `W` − 子项 margin/border/padding，记入子项约束；
3. `h_i`：子项在 `cw` 下折行后的内容高 = `sizehint(available_width=cw).max_height`；
4. `main`：容器主轴尺寸 = Σ `h_i` + gap，再经 grow/shrink 调整。

每步只依赖上游已定值（`cw` 来自父的 `W`，不来自子项主轴），无回环、无需不动点迭代。

**策略 A（wrap）**：`flex-wrap != nowrap` 且交叉轴 indefinite 时，交叉轴按行内容尺寸累加、超出容器则按 `align-content` 分布或溢出，**不钳制子项宽、不回退 measure**，避免"钳制→重测→重新分箱"的不动点循环。

## 文本测量

`ui_text_on_sizehint` 取宽优先级改为：

1. `IS_CSS_FIXED_LENGTH(s, width)` → 用 `s->width`（子项自主要宽，最高优先）；
2. 否则若存在有效布局约束（`has_definite_width` 且 source/generation 有效）→ 用 `available_width`；
3. 否则若 `max-width` 属性存在 → 用之；
4. 否则不限（单行 intrinsic）。

`sizehint` 原型签名 `(ui_widget_t*, ui_sizehint_t*)` 不变；约束经 widget/resizer 状态读取，不靠覆写 `computed_style`。折行高的 device 转换仍由 text 内部 `ui_compute` 负责，布局 box 用逻辑像素。

## 单独重排稳定性

- 父级布局子项时记录约束（source=父、generation=N）。
- 子项单独 reflow（hover/内容变化）：`reset_size` 只恢复 `computed_style`，**不触及约束状态**；子项读到 source/generation 有效的约束 → 按 `available_width` 折行测高 → 与父级在场时结果一致 → 不产生错误尺寸、不触发多余向上传递。
- 约束过期（父级已重排、generation 前进，或 source 变更）→ 丢弃约束、退回 intrinsic，并由正常 update 路径请求父级重排，父级重排时刷新约束。

## 测试

集成测试优先断言尺寸关系与稳定性，不依赖字体精确尺寸。开发期用**独立复现程序**（alert-description 场景）驱动 RED→GREEN；修复通过后，用例**并入 `test_flex_layout`**，不长期保留独立测试文件：

1. **换行**：definite 宽 column 容器内长文本子项，`content_box.height` 显著大于单行、宽不溢出容器。
2. **单独重排稳定**：对文本子项及其兄弟分别 `ui_widget_request_reflow` 后 `ui_update`，断言子项高与容器高稳定、兄弟不重叠、多次重排不振荡。
3. **intrinsic 保持**：交叉轴 indefinite（宽 auto、内容撑）的 column 容器内长文本保持单行，容器宽 = 内容 max-content。
4. **固定宽子项**：声明 `width` 的子项按自身宽折行，不被父约束覆盖（`sizehint` 优先级 1）。
5. **grow 子项**：`flex:1 1 auto` 子项主轴由剩余空间分配，不因约束复用而发散。
6. **row 不变**：`flex-direction: row` 结构尺寸与基线一致。
7. **reflow 次数**：一次父级布局内每个子项 reflow ≤1 次（计数断言）。
8. **empty / empty-basic**：居中列容器内的长描述在可用宽内折行；empty-basic 的 learn-more 按钮在 empty 内水平居中。
9. **field-checkbox**：`field:horizontal` 下 `field-content` 内的长描述在可用宽内折行，且 hover 前后保持稳定。
10. 保留并通过现有 `flex layout` / `block layout` / `flex layout percentage wrap` / `browser` 回归。

## 实施阶段

1. 引入布局约束表示（source/generation/available_width），先把 flex column 的宽度路径接入：父记录约束、`resolve_flex_basis` 与 text `sizehint` 读约束。
2. 六趟重整（行为保持），确立"约束先于主轴测量"的顺序（策略 C）与策略 A 的 wrap 分流。
3. 收敛到每子项 ≤1 次 reflow：`resolve_main_sizes` 不触发 reflow，`resolve_cross_sizes` 设 used width 后统一一次条件 reflow。
4. 将父级影响子项尺寸的路径改为走约束：不在布局趟内覆写子项 `computed_style.width`，也不回写子项 `max_content_*`。
5. 补齐回归 fixture，运行现有布局测试套件。

每阶段保持公共 reflow API 不变，且 intrinsic 测量不产生子项 reflow 副作用。
