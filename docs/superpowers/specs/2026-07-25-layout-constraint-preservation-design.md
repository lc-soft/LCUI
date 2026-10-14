# 布局约束保持设计

## 背景

LCUI 当前同时使用 `computed_style.width` 和 `computed_style.height` 表示
CSS 计算值，以及父级布局临时分配的尺寸。这对于百分比尺寸是不正确的。
父级可能将子元素的 `width: 100%` 解析为已使用宽度，随后子元素因自身样式
刷新而被重置为 `fit-content`。此时子元素重新布局时已经丢失了父级此前提供的
包含块约束。

当容器根据内容自适应尺寸时，这个问题尤其明显。全量布局能够再次为子元素分配
尺寸，因此结果正确；但子元素单独 reflow 时会产生不同的尺寸，并错误地向父级
传播变化。

## 目标

- 在 `computed_style` 中保留 `width: 100%` 等 CSS 语义。
- 将父级解析出的尺寸作为明确的临时布局约束传给子元素。
- 当子元素的样式或内容变化，但影响布局的样式语义没有变化时，保持父级上次
  分配的 used size。
- 检测 `width: 100%` 变为 `width: 50%` 等变化，并使相关父级重新布局。
- 避免无条件的父子 reflow 循环和重复的整棵祖先布局。
- 同时覆盖 block 和 flex 布局；第一阶段重点处理 width、height 以及 min/max
  约束。

## 非目标

- 不实现浏览器级别的完整布局树或完整 CSS intrinsic sizing 规范。
- 不在本次改动中重做 updater 的自底向上遍历顺序。
- 不增加一个反复重排祖先直到稳定的通用 fixed-point 循环。
- 不再永久修改 `computed_style.width` 或 `computed_style.height` 来保存父级
  分配的 used value。

## 值模型

布局引擎区分以下三类值：

1. **计算样式值**：CSS 语义，例如 `100%`、`50%`、`auto` 或 `fit-content`。
2. **布局约束**：父级提供的临时确定尺寸，例如 120px 的内容宽度。
3. **used box size**：计算样式值结合可用约束，并应用 min/max 及盒模型规则后
   得出的实际布局尺寸。

父级提供约束时，不修改子元素的计算样式：

```text
child computed width = 100%
parent content width = 120px
child layout constraint = 120px
child used width = resolve(100%, 120px) = 120px
```

当包含块在 intrinsic measurement 阶段尚未确定时，百分比尺寸不能被当作确定的
固定宽度。此时继续使用已有的 intrinsic 值作为 auto/fit-content 父级的尺寸贡献；
父级尺寸确定后，再在受约束的布局阶段解析百分比。

## 布局约束

为 widget 增加布局约束状态，或者增加由布局上下文持有、并在子元素 reflow
期间保留的等价状态。至少需要表示：

- 是否存在确定的宽度约束；
- 是否存在确定的高度约束；
- 受约束的内容宽度；
- 受约束的内容高度；
- 提供约束的父级/source widget；
- 标识父级布局轮次的 generation/version。

约束是盒尺寸计算和布局分发的输入，不能复制到 `specified_style`，也不能用来
替换 `computed_style` 中的语义值。

父级确定子元素尺寸时，在调用子元素的条件 reflow 前记录或传入约束。子元素因
自身样式或内容更新而 reflow 时，只有在 source parent 和 generation 仍然有效时
才能复用该约束。过期约束必须丢弃，不能静默地当作当前包含块尺寸使用。

实现应优先在现有 `ui_resizer_t` 和 `ui_widget_reflow_with_resizer()` 流程上做
最小状态扩展。约束必须同时可用于 block 和 flex 布局，且 intrinsic/load 阶段
不能通过 reflow 子元素来获取数据。

## 样式签名与失效传播

每个 widget 需要一个表示 CSS 布局语义的 layout-style signature，用于判断尺寸
或布局贡献是否发生变化。不能直接对 `css_computed_style_t` 使用 `memcmp`，因为
该结构可能包含无关状态或实现细节。

第一阶段比较以下属性组：

- `width`、`height`；
- `min-width`、`max-width`、`min-height`、`max-height`；
- padding 和 margin 值；
- `box-sizing`；
- `flex-basis`、`flex-grow`、`flex-shrink`；
- `display`、`position`、flex direction、wrap、对齐方式和 gap；
- 已由现有 widget 更新路径表示的、会影响布局的文本度量和内容变化。

比较时必须保留单位和 type bits。例如，虽然 `100%` 和某个固定值可能解析出
相同的像素值，但二者语义并不相同。因此从 `100%` 变为 `50%` 即使当前 used
size 暂时未变，也必须使布局失效。

样式匹配和计算完成后，updater 将当前签名与上次已提交的签名比较：

- 签名未变化：子元素单独 reflow 时保留有效的父级约束；
- 签名变化：使依赖约束的布局失效，并在子元素处于布局流中时请求父级布局；
- intrinsic contribution 变化且父级为 auto/fit-content：向父级传播 reflow；
- 在确定的父级尺寸下 intrinsic contribution 变化：只更新子树，除非子元素的
  used outer size 变化，否则不强制父级重新测量宽度或高度。

具体传播判断应放在现有 `ui_widget_update_size()` 的尺寸比较附近，使其能同时
使用样式签名和前后 used box size。

## Reflow 数据流

目标流程如下：

```text
样式/内容更新
  -> 计算语义样式
  -> 比较 layout-style signature
  -> 自底向上更新子树
  -> intrinsic measure 读取 max/min content 值
  -> 父级确定自身尺寸
  -> 父级记录子元素约束
  -> 子元素根据约束解析百分比/fill 尺寸
  -> 比较 used size 和 intrinsic contribution
  -> 只传播必要的失效状态
```

父级布局代码不能再通过永久修改 computed style 来传递解析后的子元素宽度或
高度。当前在调用 `reflow_if_*` 前设置子元素 width/height 的代码，应改为设置
对应的布局约束，并调用相同的条件 reflow。

公共 reflow API 保持不变。内部 helper 可以接收布局约束或 reflow context，但
布局实现之外的调用方不需要了解约束的具体表示。

## 向父级传播规则

- 子元素从 `width: 100%` 变为 `width: 50%` 时，使父级布局失效，因为子元素的
  布局贡献和 used size 规则都发生了变化。
- 子元素刷新后 layout signature 未变化时，复用有效的父级约束；不能仅因为
  内部重置了 computed style 就让父级重新测量。
- 子元素 used size 变化时，只有父级是内容自适应，或以该尺寸作为 intrinsic
  contribution 时，才向父级传播。
- 在确定的父级约束下，若父级自身 used size 和布局贡献不变，子元素 used size
  变化不应触发整条祖先链级联。
- constraint generation 防止子元素使用过期父级布局轮次解析出的尺寸。
- 防止意外循环时必须使用基于 generation 的有界机制，不能添加无界重试循环。

## 兼容与回退

没有有效父级约束的 widget 继续使用现有 intrinsic/fit-content 路径，以保留根
widget、脱离布局流的 widget 以及不确定包含块的现有行为。

第一阶段保留已有的盒模型和 min/max 裁剪行为。约束解析必须复用当前宽高计算
所使用的转换 helper，不能重复实现 border、padding 和 margin 的计算。

## 测试

增加以下集成测试：

1. 内容自适应容器包含 `width: 100%` 的子元素。子元素样式刷新但 layout-style
   signature 不变时，子元素 used width 仍等于父级分配的宽度。
2. 在同一 fixture 中将子元素从 `width: 100%` 改为 `width: 50%`。必要时父级
   重新布局，子元素解析为当前包含块宽度的一半，而不是保留旧宽度。
3. 修改 `max-width` 和 `min-width`，验证会改变 resolved used size。
4. 在确定高度的容器中增加 `height: 100%` 的等价测试。
5. 增加嵌套容器，验证有效约束可在局部复用，而不会无条件重排所有祖先。
6. 保留并验证现有 choice-card 和通用 flex 布局回归测试。

测试应优先断言尺寸关系和稳定性，不依赖字体相关的精确文本尺寸。需要刷新样式
时，应走正常 update 路径，不要直接调用私有布局函数。

## 实施阶段

分阶段实现：

1. 引入约束表示，并先将一个宽度路径接入约束机制。
2. 将高度和其余 block/flex 路径接入相同机制。
3. 增加样式签名和定向的父级失效传播。
4. 增加回归 fixture，运行现有布局测试套件。
5. 移除仅用于保存父级 used dimension 的旧 computed-style 修改逻辑。

每个阶段都必须保持现有公共 reflow API 不变，并确保 intrinsic measurement 不
产生子元素 reflow 副作用。
