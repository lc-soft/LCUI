# 布局两阶段（measure / layout）设计

日期：2026-10-25。状态：设计草案（未实现）。
相关：`docs/layout-engine-rules.md`、`docs/superpowers/specs/2026-10-11-flex-column-text-wrap-design.md`。

## 背景

当前布局是「自顶向下、边走边测」：`reflow(父)` 在布局子项时，若子项有 `sizehint` 就直接问；若是无 `sizehint` 的容器（如 `empty-header`），就**嵌套 reflow** 去测。于是 root → 部件1 → 部件2 → … → 部件N 这条链上，每一级都可能把子孙重新排一遍，最坏 O(深度 × 子树规模)。

已做的缓解：

- 约束测量缓存（`measured_content_*` / `has_measured`）；
- 内在尺寸与 used size 分离（`unconstrained_content_*`）。

但这些是「在自顶向下架构上打补丁」。根治方向是方案 D：**测量与布局分离**。

## 目标

1. 把一次布局拆为两个阶段：
   - **测量阶段（measure，自底向上，一次）**：为每个节点计算内在尺寸（min/max-content），缓存。
   - **布局阶段（layout，自顶向下，一次）**：用父级分配尺寸 + 缓存的内在尺寸，确定 used size 与位置。
2. 每层只做一次算术，消除「每级重复测量子孙」。
3. 内在尺寸与 used size 彻底分离。

## 非目标

- 不实现浏览器完整 intrinsic sizing（表格、替换元素、aspect-ratio 等）的全部规则。
- 不在本阶段引入独立布局树；widget 树继续充当盒子树。
- 不改公共 reflow API。

## 设计

### 1. 内在尺寸的表示

复用 `ui_sizehint_t`，但需要扩展输入：

```c
typedef struct ui_sizehint {
        float max_width, max_height;
        float min_width, min_height;
} ui_sizehint_t;
```

问题：当前 `sizehint` **没有输入约束**，"在给定宽度下的折行高"依赖 widget 上已有的约束状态（text 就是这样读 `ui_widget_get_available_width`）。

方案：

- 保留 `sizehint(w, hint)` 签名，但约定：**sizehint 读取 widget 的有效约束**（`available_width`），返回该约束下的 min/max 尺寸。
- 测量阶段先写入约束，再调用 sizehint；这样无需改签名。
- 长期可考虑显式传入 `available_width`，但会破坏 API，暂不做。

### 2. 通用容器的 intrinsic sizing

给默认原型（`ui_widget_on_sizehint`，当前为空操作）实现基于子项的递归计算：

- **block 容器**：
  - `max_width = max(子项 max_width)`；
  - `min_width = max(子项 min_width)`；
  - `max_height = Σ 子项 max_height`（纵向堆叠）。
- **flex column 容器**：
  - `max_width = max(子项 max_width)`（交叉轴）；
  - `max_height = Σ 子项 max_height + gaps`（主轴）；
  - 若容器有有效宽约束，子项的 `sizehint` 会按该约束返回折行高。
- **flex row 容器**：对称处理。
- 递归调用 `ui_widget_get_sizehint(child, &hint)`。

前提：子项的约束必须在调用 sizehint 前写好（测量阶段负责）。

### 3. 两阶段流程

```text
layout(root):
  measure(root)              // 自底向上，一次
  layout_pass(root, 可用尺寸) // 自顶向下，一次
```

- **measure(node)**：
  1. 递归 measure 子项；
  2. 写入父级约束（本阶段已知）；
  3. 调 `sizehint(node)` 得到 node 的内在尺寸；
  4. 缓存到 node（按内容/样式版本失效）。
- **layout_pass(node, used_size)**：
  1. 用 `used_size` + 内在尺寸解析 node 的 used box；
  2. 按布局规则给子项分配尺寸；
  3. 递归 layout_pass 子项；
  4. 写位置与盒模型。

### 4. 缓存与失效

- `intrinsic` 缓存：按「内容版本 + 样式版本」失效；样式/文本变化时递增版本。
- 已有 `unconstrained_content_*` 可作为 measure 阶段的产物。
- `measured_content_*`（约束测量缓存）在 D 落地后可移除。

## 分阶段实施

1. **阶段 1：通用容器 sizehint**（低风险）
   - 实现 `ui_widget_on_sizehint` 的 block/flex 组合逻辑；
   - 用 `sizehint` 替代 `measure_item_height_at_width` 的嵌套 reflow；
   - 用现有 `flex layout` / `block layout` 套件守门。
2. **阶段 2：测量阶段抽取**（中风险）
   - 把 flex/block reflow 中的「测量」部分抽成独立 pass；
   - 布局阶段只消费测量结果；
   - 每子项 reflow 计数保持不变或下降。
3. **阶段 3：缓存失效与增量**（中风险）
   - 按内容/样式版本失效；
   - 支持只重测脏子树。
4. **阶段 4：清理**（前提不成立，暂不实施）
   - ~~移除 `measured_content_*`~~：flex column 的 flex base size（高）必须在父级交叉轴约束下测量折行高，这是 CSS flex「flex base size 在可用空间下计算」的要求。实验验证：把 basis 改为 `unconstrained_content_height` 后，5 处折行测试失败（文本退回单行 basis）。因此 `measured_content_*` 是必要缓存，不应移除。
   - 更新 `docs/layout-engine-rules.md` 的 I4/无副作用表述。

## 风险

- 通用容器 sizehint 依赖子项 sizehint 的完整性；任何 no-op 的 proto 都会让组合结果偏小。
- 两阶段拆分影响 flex/block/resizer 全部入口，回归面大。
- 约束写入时机需要与测量阶段严格对齐，否则重演「测高与显示宽不一致」。
- row 路径必须逐字节不变（I5），阶段 1 需先只覆盖 column。

## 验收

- 阶段 1：`empty` / `empty-basic` / `field-checkbox` 用例仍通过，且 `measure cache` 命中率提升。
- 阶段 2：`child reflow count` 不增加；`flex layout` / `block layout` / `percentage wrap` / `browser` 全绿。
- 阶段 3：约束双向刷新（300→120→600）与重新挂载首帧正确。
- 全程：全量测试 0 failing。
