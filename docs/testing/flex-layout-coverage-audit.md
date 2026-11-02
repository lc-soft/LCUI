# Flex 布局测试覆盖审计

> 审计日期：2026-10-31。
> 范围：flexbox 语义、常见布局、特定布局、sizehint。
> 不含 `docs/layout-engine-rules.md` 的 I1–I8 引擎内部不变量，也不含 block / table 布局。
> 本文只记录现状与缺口，不定义新规则、不修改测试代码。

## 1. 方法与判定

- 断言对象限定为可观察几何：`content_box`、`padding_box`、`border_box`、`outer_box`、`layout_x/y`、相对位置、可见性。
- 不把内部字段（`flex_basis`、`measured_content_height`、`has_measured`、约束 generation）计入覆盖；这些属于诊断信息。
- 覆盖状态按“是否存在直接断言主要行为”判断：
  - **完整**：有直接几何断言，且覆盖该规则的主要分支。
  - **部分**：有间接断言，或只覆盖一个分支/一个方向。
  - **缺失**：没有断言该规则，或 fixture 中存在场景但未被断言。
- 本次为静态审计：阅读 `tests/integration/`、`tests/fixtures/`、`tests/scenes/` 中的 `ctest_*` 调用。未使用运行时覆盖率工具。
- 审计期间观察到 `xmake test` 存在偶发不稳定（style-nova badge 崩溃、toast 断言失败），不计入覆盖判定。

## 2. 现有测试资产

| 文件 / 套件 | 负责内容 |
|---|---|
| `test_flex_layout.c` / `flex layout` | XML scene 多宽度几何、约束与测量缓存诊断 |
| `test_flex_contract.c` / `flex contract` | 规则契约：F01–F19 与 C02–C10 的最小几何断言 |
| `test_sizehint.c` / `sizehint` | sizehint 能力矩阵、文本测量、容器组合、缓存与边界 |
| `test_empty_layout.c` / `empty layout` | 默认部件 + `test-*` CSS 的 empty 场景回归，不依赖主题 |
| `test_field_layout.c` / `field layout` | 默认部件 + `test-*` CSS 的 field choice card / checkbox 场景回归 |
| `test_flex_layout_percentage_wrap.c` / `flex layout percentage wrap` | 百分比尺寸、百分比 gap、百分比行内换行 |
| `scenes/flex_layout_scene.c` + `fixtures/test_flex_layout.{xml,css}` | 19 个 flex 场景（justify、auto margin、shrink、grow、wrap、align、column、holy grail、browser、gap、choice card） |
| `test_flex_layout_percentage_wrap` fixture | 百分比场景树 |
| `test_icon_centering.c` | flex 居中、图标垂直居中 |
| `test_field.c` | field 方向、组合、has-class |
| `test_empty.c` | empty 子结构、media 变体（不测几何） |
| `test_card.c` | card header/content/footer 几何、action 右对齐 |
| `test_sidebar.c` | 侧栏宽度、图标/头像尺寸、菜单 action 几何 |
| `test_select.c` | trigger 箭头与 content 宽度 |
| `test_dialog.c` | dialog 视觉几何、header 对齐 |
| `test_breadcrumb.c` | collapse 显隐与菜单内容 |
| `test_badge.c` | 图标不撑高 badge |
| `test_style_nova.c` | 主题字号/行高/高度（样式层，不是 flex 语义） |
| `test_block_layout.c` | block 布局、inline-block、absolute（仅作参考） |

## 3. 规则索引

### 3.1 Flex 语义（F）

| ID | 规则 | 主要断言点 |
|---|---|---|
| F01 | 默认行布局：`row` + `justify-content: flex-start` + `align-items: stretch` | 子项按文档顺序从左排、交叉轴拉伸 |
| F02 | 列布局：`column` 主轴为垂直方向 | 子项自上而下、交叉轴按 align 处理 |
| F03 | `justify-content` 六个取值 | flex-start / center / flex-end / space-between / space-around / space-evenly |
| F04 | `align-items` 四个取值 | stretch / center / flex-start / flex-end |
| F05 | auto margin | 主轴与交叉轴 auto margin 吸收剩余空间 |
| F06 | `flex-grow` 分配 | 单一/多个 grow、比例分配、剩余空间耗尽 |
| F07 | `flex-shrink` 收缩 | 单一/多个 shrink、min-content 下限、按 shrink 权重 |
| F08 | `flex-basis` 解析 | `auto` / `content` / 固定值 / 百分比 |
| F09 | `flex-wrap` | nowrap 不换行、wrap 按主轴可用宽断行 |
| F10 | gap | `gap` 单值/双值、`row-gap`、`column-gap`、`normal` |
| F11 | 百分比尺寸 | 百分比 width/height/basis、百分比 gap |
| F12 | min/max 约束 | `min-width`、`max-width`、`min-height`、`max-height` |
| F13 | `box-sizing` | content-box / border-box 对 used size 的影响 |
| F14 | `display: flex` 与 `inline-flex` | 是否作为 inline-level 参与外部排版 |
| F15 | 流外子项 | `position: absolute`、`display: none` 不参与主轴/交叉轴分配 |
| F16 | `flex-direction` 继承 | LCUI 特有：未显式设置时沿用父级方向 |
| F17 | 重复更新稳定性 | 同一树多次 `ui_update()`、子项独立 reflow 后几何不漂移 |
| F18 | fit-content / 内在尺寸 | auto 宽子项按 max-content 与可用宽取小，并尊重 min-content |
| F19 | 嵌套 flex | 内层容器宽/高向父级传播，外层按内层 used size 分配 |

### 3.2 常见布局（C）

| ID | 场景 | 关键验证 |
|---|---|---|
| C01 | 居中 hero（column + align center） | 标题/描述/按钮水平居中，描述按可用宽折行 |
| C02 | 导航栏（brand + nav + spacer + actions） | `flex:1` 撑开、右侧 action 贴边 |
| C03 | 侧栏 + 内容（固定宽 + flex:1） | 固定列不收缩、内容列占满剩余宽 |
| C04 | Holy Grail（header/body/left/center/right/footer） | 三栏宽高、footer 贴底 |
| C05 | 可换行工具栏 | 窄宽换行、行间/行内 gap |
| C06 | 卡片网格 | wrap + 等宽卡片 + 间隙 |
| C07 | Media object（固定媒体 + 弹性文本） | 媒体不收缩、文本列折行 |
| C08 | 表单行（label + input flex:1 + 按钮） | input 占剩余宽、按钮不压缩 |
| C09 | 等分 tabs | 多个 `flex:1` 子项等宽 |
| C10 | 列表项尾部 action | action 贴右、文本列可收缩 |
| C11 | 全高垂直居中 | `justify-content: center` + 固定高容器 |
| C12 | 页面壳（header + flex:1 main + footer） | main 吃掉剩余高度 |

### 3.3 特定布局（S）

| ID | 场景 | 关键验证 |
|---|---|---|
| S01 | empty-basic | Learn More 按钮居中且不折行 |
| S02 | empty 描述 | 长描述在容器宽内折行、hover/重挂载稳定 |
| S03 | field-checkbox | `field:horizontal` 中 field-content 折行、宽稳定 |
| S04 | field choice card | 卡片宽 100%、radio 贴卡片内容区右缘 |
| S05 | browser demo | 5 个 tab 等宽、frame/page 高度链 |
| S06 | card | action 与标题同行、description 在标题下、footer 宽 |
| S07 | item | media/content/action 的相对位置与尺寸 |
| S08 | attachment | media 尺寸、标题/描述与 action 的排列 |
| S09 | select | trigger 箭头尺寸、content 宽度 |
| S10 | sidebar | 展开/收起宽度、图标/头像尺寸、菜单 action 位置 |
| S11 | dialog | overlay/content 尺寸、header/footer 对齐 |
| S12 | breadcrumb | collapse 时项目显隐与省略菜单 |

### 3.4 Sizehint（H）

| ID | 规则 | 主要断言点 |
|---|---|---|
| H01 | 叶子 text 使用 sizehint | 单行宽高由文本层测得 |
| H02 | 带流内子部件的容器不使用 sizehint | button/label/badge 以子部件布局为准 |
| H03 | 自带文本且无子部件的 button 使用 sizehint | `ui_button_set_text` 路径可测 |
| H04 | 自定义 sizehint 原型 | canvas / textinput 仍走 sizehint |
| H05 | 文本单行 hint | 无约束时宽高等于单行内容 |
| H06 | 文本在可用宽下折行 | 宽约束 → max_width 收缩、max_height 增加 |
| H07 | `max-width` 优先 | 约束宽与 max-width 同时存在时取较小值 |
| H08 | 固定 width 优先 | 自身固定宽覆盖父级可用宽 |
| H09 | 零尺寸合法 | 空文本 0 宽/0 高，不读旧缓存 |
| H10 | 容器组合测量（全 sizehint） | column 容器高度 = 子项 hint 之和 + gap，不触发子项 reflow |
| H11 | 容器组合测量退化路径 | 任一子项无 sizehint 时回退到一次 reflow，结果仍正确 |
| H12 | 流外/隐藏子项 | absolute / display:none 子项不阻止容器的 sizehint 组合 |
| H13 | 测量缓存 | 宽不变复用、宽变化重测、内容变化/`should_reflow` 失效 |
| H14 | 约束不污染内在缓存 | 有有效宽度约束时不更新 `unconstrained_content_*` |
| H15 | 嵌套容器测量 | 容器套容器的宽度/高度传播 |
| H16 | 负可用宽 clamp | 子项可用宽为负时按 0 处理 |

## 4. 覆盖映射

> 本节记录**审计时**（新契约套件创建之前）的状态；新增套件与 P0 修复见第 9 节。

### 4.1 Flex 语义

| ID | 现有测试 | 状态 | 说明 |
|---|---|---|---|
| F01 | `flex layout` → XML 场景 (0) | 部分 | 只断言 example 外框，未断言子项默认位置/拉伸 |
| F02 | `flex layout` → XML 场景 (14) | 完整 | 列方向子项位置有断言 |
| F03 | XML 场景 (1)(2)(3)(4) | 部分 | 覆盖 center/flex-end/between/around；缺默认 flex-start 与 space-evenly |
| F04 | XML 场景 (10)(11)(12) | 部分 | 覆盖 center/flex-end/stretch；flex-start 无直接断言 |
| F05 | XML 场景 (5) | 完整 | 4 个子项位置有断言 |
| F06 | XML 场景 (7)、`grow child main size` | 完整 | 单一/多 grow 与重复更新稳定性 |
| F07 | XML 场景 (6) | **缺失** | fixture 有 `flex-shrink` 场景，但所有宽度测试都未断言其子项 |
| F08 | `grow child main size`、`flex layout percentage wrap` | 部分 | `flex:1` 与百分比 basis 有覆盖；`content`/固定 basis 不完整 |
| F09 | XML 场景 (8)(9)、`flex layout percentage wrap`、`wrap container overflows` | 部分 | 百分比换行与溢出场景有覆盖；XML 基础 wrap 场景 (8)(9) 无断言 |
| F10 | `flex gap` | 完整 | row/column/wrap/normal 均有精确断言 |
| F11 | `flex layout percentage wrap` 全部 | 部分 | 行方向百分比完整；列方向百分比/高度百分比未覆盖 |
| F12 | `max-width limits measurement`、`fixed width child wrap` | 部分 | max-width 有覆盖；min-width/min-height/max-height 缺失 |
| F13 | 无 | **缺失** | 没有 content-box / border-box 的 flex used size 断言 |
| F14 | 无 | **缺失** | 没有 `inline-flex` 参与 inline 排版的断言 |
| F15 | 无（block 布局有 absolute） | **缺失** | flex 容器内的 absolute / display:none 子项未测 |
| F16 | 无 | **缺失** | `flex-direction` 未设置时继承父级的行为未测 |
| F17 | `choice card`、`standalone reflow stays wrapped`、percentage 稳定性断言 | 部分 | 覆盖若干定点；没有通用“多次 update 不漂移”约定 |
| F18 | `empty description wrap`、`empty basic learn more centered`、`indefinite width single line`、`fixed width child wrap` | 完整 | fit-content/min-content/max-content 的主要分支有覆盖 |
| F19 | Holy Grail (15)(16)、browser (17)、field-content、empty-header | 部分 | 有真实嵌套场景；缺少最小嵌套容器的宽高传播约定 |

### 4.2 常见布局

| ID | 现有测试 | 状态 | 说明 |
|---|---|---|---|
| C01 | `empty description wrap`、`empty basic learn more centered`、XML 场景 (13) | 部分 | 水平居中与折行有覆盖；完整 hero 结构未单独验证 |
| C02 | `browser layout`（tabbar） | 部分 | tab 等宽有覆盖；brand + nav + spacer + actions 结构未测 |
| C03 | `test_sidebar` 宽度 | 部分 | 侧栏自身宽度有覆盖；侧栏 + flex:1 内容区未测 |
| C04 | XML 场景 (15)(16) | 完整 | 三栏 + header/footer 尺寸与位置 |
| C05 | 无 | **缺失** | 无换行工具栏场景 |
| C06 | 无 | **缺失** | 无卡片网格（wrap + 等宽）场景 |
| C07 | 无 | **缺失** | item/attachment 测试不测几何 |
| C08 | `field checkbox description stable` | 部分 | horizontal field 有覆盖；input + 按钮的完整表单行未测 |
| C09 | `browser layout` | 完整 | 5 个 tab 等宽 |
| C10 | `card layout` | 完整 | action 与标题同行、右对齐 |
| C11 | XML 场景 (13) | 完整 | 固定高容器垂直居中 |
| C12 | XML 场景 (15)(16) | 完整 | header/body/footer 高度分配 |

### 4.3 特定布局

| ID | 现有测试 | 状态 | 说明 |
|---|---|---|---|
| S01 | `empty basic learn more centered` | 完整 | 居中 + 不折行 + 宽度稳定 |
| S02 | `empty description wrap`、`empty description reflow stable` | 完整 | 折行、hover、重挂载 |
| S03 | `field checkbox description stable` | 完整 | 折行、hover、remount、单独 reflow |
| S04 | `field choice card alignment` | 完整 | 卡片宽、content 宽、radio 右对齐、更新稳定 |
| S05 | `browser layout` | 完整 | tab 宽、frame/page 高度 |
| S06 | `card layout` | 完整 | header/content/footer 几何 |
| S07 | `test_item` | 缺失 | 只测状态/变体，不测 media/content/action 几何 |
| S08 | `test_attachment` | 缺失 | 只测状态/变体/media 类型，不测布局几何 |
| S09 | `select trigger arrow and content width` | 完整 | 箭头与 content 宽 |
| S10 | `test_sidebar` 宽度/头像/action 几何 | 完整 | 展开/收起宽度、图标、头像、action 位置 |
| S11 | `dialog visual geometry`、`dialog header alignment` | 完整 | overlay/content 尺寸与 header 对齐 |
| S12 | `breadcrumb collapse` | 部分 | 显隐与菜单内容有覆盖；折叠后的宽度分配未测 |

### 4.4 Sizehint

| ID | 现有测试 | 状态 | 说明 |
|---|---|---|---|
| H01 | `sizehint capability` | 完整（白盒） | 直接调用 `ui_widget_has_sizehint` |
| H02 | `sizehint capability` | 完整（白盒） | button/label 带子部件返回 false |
| H03 | `sizehint capability` | 完整（白盒） | `ui_button_set_text` 无子部件返回 true |
| H04 | 无 | **缺失** | canvas / textinput 的自定义 sizehint 未验证 |
| H05 | `text resize`、`button text vertical fit` | 部分 | 间接覆盖单行测量；无 sizehint 直接契约 |
| H06 | `flex column text wrap`、`empty description wrap` | 完整 | 宽约束下折行高度增加 |
| H07 | `max-width limits measurement` | 完整 | max-width 限制测量宽 |
| H08 | `fixed width child wrap` | 完整 | 固定宽优先于父可用宽 |
| H09 | `zero height text ignores stale cache` | 完整 | 零高合法且不读旧缓存 |
| H10 | `measure cache`（generic container） | 部分 | 复用/重测路径有覆盖；未验证“全 sizehint 子项 + column”组合 |
| H11 | `measure cache` | 部分 | 退化路径有覆盖；未明确“任一无 sizehint 子项” |
| H12 | 无 | **缺失** | absolute / display:none 子项对容器测量的影响未测 |
| H13 | `measure cache`、`measure cache invalidates on content change`、`zero height text ignores stale cache` | 完整 | 复用、内容变化、零尺寸失效 |
| H14 | `centered container reexpands` | 完整 | 约束解除后宽度恢复 |
| H15 | 无 | **缺失** | 嵌套容器测量的高度/宽度传播未测 |
| H16 | 无 | **缺失** | 负可用宽 clamp 未测 |

## 5. 缺口清单（按优先级）

### P0：纯 flex 语义、成本低、直接补强约定

1. **F07 flex-shrink**：XML 场景 (6) 从未被断言；需要单一/多个 shrink、min-content 下限、shrink 权重三组。
2. **F09 基础 wrap**：XML 场景 (8)(9) 从未断言；需要 nowrap/wrap 的换行位置、行间 gap。
3. **F03 默认与 space-evenly**：补 `flex-start` 默认值、`space-evenly`。
4. **F04 flex-start**：补 cross-axis 起始对齐。
5. **F13 box-sizing**：content-box / border-box 同一结构和内边距下的 used width 差异。
6. **F14 inline-flex**：inline-flex 在 block 行内与 flex 父级中的参与方式。
7. **F15 流外子项**：absolute / display:none 子项不占主轴与交叉轴。
8. **F16 direction 继承**：父 column、子未显式设置方向时的实际方向。

### P1：常见布局与特定布局

9. **C02 导航栏**、**C03 侧栏 + 内容**、**C05 换行工具栏**、**C06 卡片网格**、**C07 Media object**、**C08 表单行**。
10. **C10 列表尾部 action** 的通用约定（现有 card 只是一个实例）。
11. **S07 item** 与 **S08 attachment** 的几何断言。
12. **F12 min-width / min-height / max-height**。
13. **F08 flex-basis** 的完整矩阵（auto/content/fixed/percentage）。

### P2：边界与组合

14. **F11 列方向百分比**、**F19 最小嵌套容器**。
15. **H04 canvas/textinput 的自定义 sizehint**。
16. **H10/H11 容器组合测量**的全 sizehint 快路径与退化路径。
17. **H12 隐藏/流外子项**、**H15 嵌套容器测量**、**H16 负可用宽 clamp**。
18. **S12 breadcrumb 折叠后的宽度分配**。

## 6. 建议的新测试套件

用户已确认：契约层只断言最终几何，场景层保留真实组合；旧 `test_flex_layout.c` 不调整。

### 6.1 `tests/integration/test_flex_contract.c`（suite：`flex contract`）

- 只覆盖 F / C 规则，使用最小 widget 树。
- 每条规则一个 `ctest_describe("<规则ID> <行为>", fn)`，例如：
  - `"F07 shrink respects min-content floor"`
  - `"F09 wrap breaks at container width"`
  - `"F13 border-box includes padding in flex basis"`
- 每个规则至少两个 case：
  - 正例：规则按预期生效。
  - 边界/反例：换一个输入，结果必须按规则变化（防止断言恒真）。
- 常用 helper：
  - `expect_right_edge(child, parent, tolerance)`
  - `expect_same_row(a, b)`
  - `expect_stable_after(n, expected_fn)`
  - `expect_single_line(ref_text, text)`
- 文本尺寸一律用参照文本或相对比较，不写字体相关精确 px。

### 6.2 `tests/integration/test_sizehint.c`（suite：`sizehint`）

- 黑盒组 `intrinsic sizing`：H01–H16 中可用最终几何观察的条目。
- 白盒组 `sizehint capability`：直接查询 `ui_widget_has_sizehint`，保留为诊断，不承载唯一契约。
- 重点覆盖：
  - 叶子 vs 容器能力矩阵（含 canvas/textinput、隐藏子项）。
  - 容器组合测量快路径 vs 退化路径。
  - 缓存复用、宽度变化、内容变化、`should_reflow` 失效。
  - 约束下 `unconstrained_*` 不被 used size 污染（配合“变宽后重新展开”）。

### 6.3 场景层

- `test_empty_layout.c`（suite `empty layout`）与 `test_field_layout.c`（suite `field layout`）承担 empty / field 场景回归；使用默认部件 + `tests/fixtures/test_empty_layout.css` / `test_field_layout.css`，不依赖 style-nova 或具体 widget 类型。
- 其余真实 widget 组合仍由各自 widget 测试与 `test_flex_layout.c` 的 XML scene 承担。
- S07/S08 等缺口可在对应 widget 测试文件补几何断言，或在新 suite 中按场景命名提交。

### 6.4 命名与追溯

- 规则 ID 只出现在 `ctest_describe` 名称前缀与本文档中。
- 文档不重复描述每一条规则的完整语义；以测试名 + 断言为准。
- 新增规则时先加本文档索引行，再写失败测试，最后改实现。

## 7. 明确不在本次范围

- I1–I8 引擎内部不变量。
- block / table 布局。
- 主题（style-nova）字号、颜色、圆角等视觉断言。
- Portal / overlay 定位（tooltip、popover、dialog 的锚定与 outside 行为）。
- 动画、绘制、DPI 像素比较。

## 8. 已知风险

- 偶发崩溃已定位为 `test_toast.c` 的 heap-use-after-free（见 9.3）；修复后普通构建与 ASan 全量均稳定通过。
- 文本尺寸依赖字体环境；契约测试必须使用参照/相对断言，否则在 CI 与本地会产生不同结果。
- 本文的“完整/部分/缺失”基于静态阅读，不代表运行时行覆盖；补测试时应优先补“零断言”的场景（例如 XML 场景 6/8/9）。

## 9. 实施记录（2026-10-31）

新增套件：

- `tests/integration/test_flex_contract.c`（suite `flex contract`）：93 个断言，覆盖 F01–F19 与 C01–C10。
- `tests/integration/test_sizehint.c`（suite `sizehint`）：46 个断言，覆盖 H01–H16 中可观察的部分。
- `tests/integration/test_empty_layout.c`（suite `empty layout`）：8 个断言，用默认部件 + `test_empty_layout.css` 复现 empty-basic 场景。
- `tests/integration/test_field_layout.c`（suite `field layout`）：11 个断言，用默认部件 + `test_field_layout.css` 复现 field choice card / checkbox 场景。
- `test_item` 与 `test_attachment` 增加几何断言（media/content/actions 的相对位置）。
- `breadcrumb` 增加 demo 布局回归（完整宽度、可见项、内建 ellipsis 隐藏）。
- empty / field 的旧测试已从 `test_flex_layout.c` 移出；旧套件保留 XML scene、约束与测量缓存诊断。

P0 实施时发现并修复的问题：

| 问题 | 修复 |
|---|---|
| `justify-content: space-evenly` 首个间距被计算两次，导致整体右移并溢出 | `ui_flexbox_compute_justify_content` 不再预加一个 space |
| `flex-basis: <percentage>` 被 CSS 语法拒绝（只声明了 `auto \| <length>`） | `properties.c` 的 `flex-basis` 增加 `<percentage>` |
| `canvas` 销毁时用 `ui_widget_add_data` 分配新数据并读取未初始化 `contexts` | 改为 `ui_widget_get_data` |
| 从 text 继承的 sizehint 对带子项容器不可靠 | 引入 `sizehint_exact`，仅用于组合测量快路径 |

仍保留在缺口清单（未在本次实施）：

- F10 百分比 gap 的更多边界。
- H10/H11 快路径与退化路径的 reflow 次数（当前只验证结果几何）。
- H13 的 `should_reflow` 强制失效、H14 的内部缓存隔离（由旧套件的诊断测试承担）。

### 9.1 P1/P2 确认与修复（2026-11-01）

已完成：

- F05 交叉轴 auto margin：`ui_flexbox_compute_item_layout` 支持 `margin-top/bottom/left/right: auto`，且 auto margin 阻止 stretch。
- F08 `flex-basis: content`：CSS 关键字与 cascade 支持；契约测试与 auto basis 对比。
- F12 百分比 min/max：契约测试通过。
- C01 hero、C09 等宽 tabs：新增契约测试。
- S07 item、S08 attachment：新增几何断言。
- S12 breadcrumb demo：修复首次布局在父级宽度未就绪时把 min-content 缓存为内在宽的问题；新增完整宽度与可见项断言。
- 最终验证：普通构建连续 3 次 `1890 passing`；ASan 构建 1 次 `1890 passing`，无内存报告。


### 9.2 sizehint 合并模型（2026-11-01）按“先迁移测试，再改 sizehint”的顺序完成：

- `sizehint` 始终可调用，默认原型返回 0；`has_sizehint` 只表示是否有真实实现。
- fit-content / column basis 改为 `max(自有 sizehint, 子项布局尺寸)`。
- 新增 `ui_widget_sizehint_is_exact()`：无流内子项或原型标记 `sizehint_exact` 时 true，只用于组合测量快路径。
- `text`、`canvas`、`textinput` 标记 `sizehint_exact`；`button`/`label` 等继承实现不继承该标记。
- `sizehint` 套件新增 capability/exact 断言与“自有 hint 与子项尺寸合并”用例。

### 9.3 偶发崩溃定位（2026-11-01）

用 MSVC AddressSanitizer 构建全量测试后定位到：

- `test_toast.c` 的 `test_loading_type_uses_spinner()` 在 `ui_toast_set_type()` 之后继续使用旧的 `icon_host` 指针；
- `ui_toast_set_type()` 会销毁并重建 icon host，旧指针成为 heap-use-after-free；
- 释放后的内存有时仍可读，表现为 toast 断言偶发失败；有时触发 access violation，表现为 style-nova 等后续测试崩溃。

修复为在 `set_type` 后重新获取 `content` 和 `icon_host`。验证：

- ASan 全量运行 3 次，均通过，无 ASan 报告；普通全量连续 5 次通过。
- P1/P2 变更后：普通全量连续 3 次 `1882 passing`；ASan 全量 1 次 `1882 passing`。

### 9.4 动态文本与 breadcrumb 状态（2026-11-01）

- `ui_widget_get_unconstrained_sizehint()` 临时清除 used width 与父级宽度约束后取 sizehint，避免文本内容变化后继续用旧约束下折行的宽度。
- auto 宽 flex/inline-block 容器在自身回流后刷新 `unconstrained_*`，使动态内容变化能更新内在宽。
- `breadcrumb` 首次布局时发布完整内在宽并请求父级 reflow；父级宽度未就绪时跳过折叠测量。
- breadcrumb demo 的 `Selected` 状态改为单个 `text` 部件，C/XML/TSX 三变体同步。
- 新增 `breadcrumb demo layout` 与 `breadcrumb status text update` 回归。
- 最终验证：普通全量连续 3 次 `1893 passing`；ASan 全量 1 次 `1893 passing`，无内存报告。



