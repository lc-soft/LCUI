# CSS 属性继承设计

## 目标

为 LCUI CSS 引擎提供属性值继承：所有已注册长属性都可显式使用 `inherit`；
CSS 规范中默认继承的属性自动继承父级的计算值；未声明且默认不继承的属性使用
其注册初始值。

## 范围

本设计只实现 `inherit`。`initial`、`unset`、`revert`、`revert-layer`、自定义
属性以及选择器/继承边界之外的级联行为均不在本设计范围内。

## 语义

CSS 继承沿元素树的直接父子关系进行：子元素继承的是父元素该属性的 **computed
value**，而不是父元素布局之后的 used value。computed value 不保证是绝对值：
宽度百分比仍可保留为百分比，`auto` 等关键字也可继续保留；只有 `font-size`
百分比在计算阶段就相对父级字号求值。层叠/计算按父到子顺序进行，但父级属性
并不一定都已变成绝对长度。

### 显式 `inherit`

所有已注册长属性均接受独立出现的 `inherit`，不依赖该属性是否默认继承。该声明
取父级该属性的计算值；若元素没有父级，则取该属性当前注册的初始值。

简写属性上的单独 `inherit` 展开为它所覆盖的每个长属性的 `inherit` 声明。例如
`margin: inherit` 使四个 margin 长属性分别继承，而不是把一个简写值写入计算
样式。

### 默认继承

按 CSS 语义默认继承的属性为：

| 属性 | 继承理由 |
| --- | --- |
| `color` | 文本前景色 |
| `font-family` | 字体族 |
| `font-size` | 字号 |
| `font-style` | 字体样式 |
| `font-weight` | 字重 |
| `line-height` | 行高 |
| `text-align` | 文本对齐 |
| `visibility` | 可见性 |
| `white-space` | 空白与换行处理 |
| `word-break` | 单词断行行为 |
| `pointer-events` | 指针事件命中行为 |

这是"缺省继承"名单，不限制显式 `inherit` 的可用范围。其余已注册属性
（`display`、尺寸、margin/padding、border/background、`opacity`、`position`、
Flex 属性、`vertical-align`、`content` 等）缺省使用各自的初始值。

`font-size: <percentage>` 在根元素上以 medium 基准 `14px` 计算。该基准由 CSS 与
UI 共用，使根字号百分比与 UI 对 medium 默认字号采用同一取值。

## 关键约束

以下约束来自 LCUI 现有代码事实，决定了继承与传播的实现方式：

1. **继承源是 `specified_style`，不是布局后的 `computed_style`。** cascade
   输出保存在 `specified_style`；`computed_style` 是其副本，之后经过绝对单位
   换算并被布局过程改写，不等价于 CSSOM 的 computed style，不能作为继承源。

2. **`css_computed_style_t` 含位域、padding 与 owning 指针**（字体列表、背景图、
   内容字符串）。属性相等判断不能用 `memcmp`；继承合成也不能靠浅拷贝整个父样式
   ——那会造成资源别名、重复释放或相互覆盖。

3. **`specified_style` 天然是"上一版值"容器。** `css_computed_style_destroy()` 只
   释放 owning 指针、不清数值/位域，因此可在被覆盖前就地取一份旧值用于比较。

4. **调度模型不可违背。** `ui_updater_update_widget` 先处理节点自身、再按
   `should_update_children` 下钻子树，脏由位标志驱动。继承传播只能挂到
   `should_update_style` / `should_update_children` 上，**不能**用整棵子树的
   `should_refresh_style`——那会连带触发 `ui_widget_match_style`（选择器重匹配），
   比单纯重解析继承值更贵，且与增量更新机制冲突。

5. **传播只标节点、不改共享对象。** LCUI 用 `ui_style_cache` 按选择器 hash 共享
   `matched_style`。传播只能对受影响子节点 `ui_widget_request_update_style(child)` 标脏，
   **不得**就地修改被共享的 style 对象。

## 实现设计

### 1. 在解析边界识别 CSS-wide `inherit`

在属性值入口统一识别 `inherit`：仅当整个属性值（忽略首尾空白、关键字大小写不
敏感）是 `inherit` 时编码为通用关键字 `CSS_KEYWORD_VALUE / CSS_KEYWORD_INHERIT`，
其他值仍交给各属性自身的 value definition。这样不会把 `inherit` 错误地允许进入
普通复合语法的任意位置。

统一入口覆盖样式表规则、公开的属性值解析 API、内联/自定义样式声明，以及简写
属性解析；简写解析把该关键字展开到它组成的每个长属性。

### 2. 在属性定义中登记默认继承策略

属性定义（`css_propdef_t`）带默认继承标记，仅对上述 11 项启用。简写属性不参与
计算样式继承，其声明在解析时展开为长属性。

### 3. 在 cascade 中合成父值与初始值

cascade 接收可选的父级计算样式。保留现有 `css_cascade_style(style, computed)`
作为等价于"无父级"的兼容包装。逐项处理长属性：

1. 声明值是显式 `inherit`：有父级时把父级对应属性复制到目标样式；无父级时应用
   注册初始值。
2. 有普通声明值：照常调用该属性的 cascade 函数；若属性是 `font-size`，随后按父级
   字号对其百分比求 computed value。
3. 没有声明值：有父级且属性标记为默认继承时复制父值；否则应用注册初始值。

继承合成一律读取父级 `specified_style`（CSS computed-value 快照），而非父级布局
后的 used values。CSS computed-value 阶段与 LCUI 的绝对单位换算、百分比 used-value
解析、布局计算明确分开：百分比、`auto` 等值只在具备相应布局上下文时才进入
used-value 处理；子级继承合成完成后继续沿现有流程产生自己的 used values。

### 4. 属性的复制与相等判断由属性表生成

复制（cascade 把父值合成到子样式）与相等判断（传播时判断父值是否变化）是继承
流程的基础操作。它们不携带每属性的独立语义，只是按该属性在
`css_computed_style_t` 中所占字段做固定的拷贝/比较。这些字段是 C 位域，只能在
编译期按字段名访问，因此用一张 X-macro 属性表在编译期展开生成 copy/equals：

- `CSS_PROP_TABLE(X)` 为每个 `css_prop_key_t` 登记一行 `(属性名, 形态)`，由表展开
  生成两个 `switch` 的所有分支。新增属性只需在表里加一行。
- **形态宏**按字段布局分组：`LENGTH`（值 + type_bits + unit_bits）、`VALUE_UNIT`、
  `VALUE_TYPE`、`VALUE`、`TYPE_BITS`。owning 与复合属性使用各自形态：独占字符串
  （深拷 + `strcmp`）、字体列表（专用 copy/destroy + 逐元素比较）、
  `background_size` / `box_shadow` 等多字段复合。个别存储字段名与属性名不一致者由
  专属形态处理；其余遵循"字段名 == 属性名"约定。
- **编译期覆盖契约**：以表长度对 `STYLE_KEY_TOTAL` 做编译期校验，保证每个已注册
  属性都有对应的 copy/equals 分支；未登记则构建失败。

`cascade` 本身保留按属性的函数指针分派——那里每个属性的解析/合成逻辑确实各异
（margin 展开四边、background 多层、`font-size` 关键字→数值等）。

### 5. 依赖位图 `style_dependencies`

`ui_widget_t::style_dependencies` 是描述"本节点哪些属性依赖父值"的位图，每
`bit` 对应一个 `css_prop_key_t`，以 32 位 word 存储
（`(STYLE_KEY_TOTAL + 31) / 32`，当前 73 个属性 → 3 个 word）。word 存储使"整词
为零"粗筛与稀疏提取都能自然进行。

置位发生在 cascade 声明阶段，条件为以下任一：

- 该属性取显式 `inherit`；
- 未声明且属性默认继承；
- `font-size` 为百分比。

本节点有独立普通值时不置位，从而截断该属性的依赖传播。该位图只描述 cascade
继承依赖，不代替文本度量或布局失效判断。

### 6. 沿子依赖并集增量传播

传播挂在父节点样式更新的 `ui_style_diff_begin` / `ui_style_diff_end` 上，比较与
"抓旧值"的范围一起收缩到被子节点实际依赖的属性。

**begin（层叠前）**

1. 遍历直接子节点，把各自的 `style_dependencies` 按位 OR，得到依赖并集
   `dep_union`。并集为空说明没有任何子节点依赖继承 → 不设快照、零堆分配、
   整段传播早退（多数节点走这条路径）。
2. 并集非空时，把父节点当前 `specified_style` 值拷贝一份作为旧快照。值拷贝会
   共享三个 owning 堆字段（字体列表 / 背景图 / 内容串），而 cascade 随后会释放
   父样式自己的这些字段；因此对每个 owning 字段按位判断：**命中并集的**用属性级
   copy 深拷一份使快照独立持有，**未命中的**置空。快照因此只拥有并恰好释放它
   自己拷入的内存。实践中唯一既是继承属性又 owning 的是 `font_family`，故多数
   节点这一步零 `malloc`。

**end（层叠后传播）**

3. 沿 `dep_union` 的置位做 word + `ctz` 稀疏遍历（只迭代被置位的属性，不扫全部
   属性），对每个候选用生成的相等判断比较旧快照与新的 `specified_style`；不相等
   的属性记入 `changed` 位图。
4. `changed` 为空则不传播；否则对每个直接子节点判断
   `child.style_dependencies ∩ changed` 是否非空，非空才 `ui_widget_request_update_style(child)`。
   子节点稍后按父先子顺序重算时，用它自己的 `dep_union` / `changed` 继续向下传播。
   值未变的属性自然截断其依赖分支。
5. 用完释放旧快照（只释放其持有部分）。

继承依赖判断与布局失效判断正交：`ui_style_diff_end` 里 `flow_changed` /
`inner_changed` / `IS_PROP_*_CHANGED(...)` 用的是 `diff->style`（`computed_style`
浅拷贝），服务布局/绘制的 reflow/repaint，与继承传播互不触发对方的额外开销。

### 7. 文本度量与 reflow 连接

只有影响固有尺寸的文本属性（字号、字体族/字形/字重、行高、空白/断行行为等）
变化时，文本节点才重新 typeset 并请求自身 reflow；`ui_widget_request_reflow()`
再经现有布局传播使受影响父容器更新。只影响绘制的变化（如 `color`）不应单独触发
尺寸重排。

文本样式度量比较属于 `ui_text_style_t` 语义，因此
`ui_text_style_metrics_equal()` 放在 `ui_text_style.c` 并由 `text_style.h`
声明，供 text widget 的 reflow 决策复用；完整的 `ui_text_style_is_equal()` 组合
度量、content 与绘制属性比较，各类字段分组到具名 helper，避免在调用点维护一串
混合语义的字段条件。

## 测试设计

### CSS 库单元测试（`lib/css/tests/`）

- 全局 `inherit` 能被长属性值入口解析，其他非完整关键字形式仍由原属性语法拒绝。
- 缺省继承属性复制父值；子级显式普通值覆盖父值。
- 非默认继承属性缺省用初始值，但显式 `inherit` 时复制父值。
- 根级显式 `inherit` 回退到属性初始值。
- 简写 `inherit` 展开到其全部组成长属性。
- **属性表覆盖**：遍历 `0..STYLE_KEY_TOTAL` 每个 key，对两份零值计算样式断言
  相等判断成立、属性级复制可安全执行。任何属性漏进表都会命中 copy/equals 的
  默认分支并在 debug 下 `assert(false)` 当场暴露。

### UI 集成测试（`tests/integration/`）

- 父子及多层后代继承、非继承属性默认不传播、显式 `inherit` 跨层传播、子级声明
  覆盖继承值、父级样式改变后后代在下一次 update 中取得新值；至少覆盖 `color` 或
  字体属性，以及一个默认不继承但显式 `inherit` 的属性。
- 父级字号变化：百分比字号继续依赖新父字号、显式固定字号截断传播、
  percentage/无单位/normal 行高的自适应文本更新自身固有尺寸及父布局，
  不相关/被覆盖分支不做无效 reflow。
- 传播正确性依赖"逐边依赖"不变量：
  - 三层链改根 `color` → 三层都更新。
  - 中间层显式覆盖 `color` → 改根不影响该层及其下（截断）。
  - 中间层不依赖某属性但孙依赖：中间层 resolve 值不变 → 该位不进 `changed`、
    不置位 → 不向下传播，是正确的截断。
- 性能断言（可选）：改根 `color` 时统计 `ui_widget_update_style` 调用次数，断言只
  命中依赖子树而非全树。

## 验收标准

- 11 个默认继承属性在无子级声明时取父级计算值。
- 所有已注册长属性可显式使用 `inherit`；简写属性的 `inherit` 按其组成长属性展开。
- 非继承属性缺省行为、CSS 属性初始值和现有 cascade 优先级不变。
- 根节点的 `inherit` 使用 LCUI 当前属性初始值。
- 继承源始终是 CSS computed value，而不是父元素布局后的 used value；百分比与
  关键字等 computed values 保持正确语义。
- 每个已注册属性都有由属性表生成的 copy/equals 分支；新增属性未登记则构建失败。
- 父样式更新不会留下过期的后代继承值；依赖传播在显式独立值处停止；文本固有尺寸
  变化触发必要的 typeset/reflow，未改变尺寸的纯绘制变化不引发无效重排。
- 传播只标节点脏位、不修改被共享的 `matched_style` 对象。
- 动态计算样式资源无泄漏、双重释放或悬垂引用。
- 现有 CSS 与 UI 测试继续通过，新增单元及集成测试通过。
