# 文本下划线渲染设计

## 目标

为 CSS 和 `text` widget 增加最小可用的文本下划线能力：支持 `text-decoration: underline` 开启下划线、`text-decoration: none` 关闭下划线，并允许用 `text-underline-offset` 调整下划线与文字基线的距离、用 `text-decoration-thickness` 调整下划线粗细。默认不装饰文本。实现应复用现有文本排版、对齐、裁剪和重绘路径，不改变文字排版结果。

## 范围与行为

- 首版仅支持 `text-decoration: none | underline`，不实现 overline、line-through、装饰线颜色或线型。
- 支持 `text-decoration-line` 长写属性（值域与 shorthand 相同：`none | underline`），写入同一 computed 字段，使 Tailwind 等工具链生成的原生 `underline` 类（声明为 `text-decoration-line`）直接生效；`text-decoration-style`、`text-decoration-color` 长写暂不支持。
- `text-decoration` 初始值为 `none`；`text-underline-offset` 支持 `auto` 或 `<length>`，初始值为 `auto`。
- `text-decoration-thickness` 支持 `auto` 或 `<length>`，初始值为 `auto`。`auto` 沿用首版默认粗细一个逻辑像素；显式正长度按当前渲染尺度转换为物理像素，正值不足一个物理像素时至少绘制一个物理像素。零长度不绘制下划线；负长度按零处理。暂不支持 `from-font`、百分比或相对字体单位。
- 正 offset 使线远离基线向下移动，负 offset 使线靠近文字向上移动。长度遵循 LCUI 已支持的绝对长度单位；不支持百分比或相对字体单位。
- 下划线颜色使用该 text widget 的文本前景色。线段宽度等于每行排版宽度；`auto` 粗细为一个逻辑像素，并转换为当前渲染尺度下至少一个物理像素。
- `auto` 使用基线下方一个逻辑像素作为首版默认间距。现有字形度量未暴露字体专用 underline position/thickness，因此不承诺与字体设计度量完全一致。
- 每个 text widget 独立读取自己的计算样式；不在首版增加装饰样式向子 widget 的继承/传播规则。

## 架构与数据流

### CSS 注册和级联

- 在 `lib/css/include/css/types.h` 增加三个属性 key、下划线关键字，以及 computed style 所需的 decoration 状态、offset 和 thickness 类型、数值和单位字段。
- 在 `lib/css/src/keywords.c` 注册 `underline`；复用已有 `none` 和 `auto` 关键字。
- 在 `lib/css/src/properties.c` 注册 `text-decoration`、`text-underline-offset` 与 `text-decoration-thickness`，定义分别为 `none | underline`、`auto | <length>`、`auto | <length>`。
- 在 `lib/css/src/properties/` 添加 cascade 实现，并遵循当前 computed-style 的枚举/数值/单位存储习惯。offset 与 thickness 在 computed style 中统一为逻辑 CSS px：`dp` 按 density、`pt` 按 `96/72` 归一化；UI text style 再按实际 scale 换算为物理像素。这样显式 `px` 与默认 `auto` 在高 DPI/全局缩放下保持一致。
- `text-decoration` 按 CSS 语法注册为 shorthand：`<'text-decoration-line'> || <'text-decoration-thickness'>`（通过 `text-decoration-line`/`text-decoration-thickness` valdef 别名引用分量值域），parse 时按 token 类型展开为对应长写 key；展开使用 `css_style_decl_set`，同一声明块内简写与长写按声明顺序后写覆盖。
- `text-decoration-line` 为普通长写属性（值域 `none | underline`，初始 `none`），cascade 写入原 `type_bits.text_decoration` 字段；`style`、`color` 分量暂不支持，出现在值中会被 valdef 拒绝。

### UI text 样式同步

- `lib/ui/include/ui/text_style.h`、`lib/ui/src/ui_text_style.c` 承载 decoration、offset 与 thickness 的计算结果，并将其纳入初始化、样式相等比较和从 CSS computed style 的转换。
- `src/widgets/text.c::ui_text_on_update_style()` 将样式传入 Pandagl 文本层。样式变化必须导致对应文字区域重绘；变更 offset、decoration 或 thickness 不应触发额外排版，除非现有文本层接口为更新/失效所必需。
- 保持 CSS 属性注册、UI 计算样式与 text widget 同步的职责分层，不在 widget 中重新解析 CSS 字符串。

### Pandagl 下划线绘制

- `lib/pandagl/include/pandagl/text.h` 暴露轻量的文本装饰设置接口；内部 `pd_text_t` 保存 decoration、offset、thickness 与颜色（颜色可直接复用默认文本前景色）。UI widget 通过该接口同步样式。
- `lib/pandagl/src/text/text.c` 保持现有行排版与 `baseline_y` 计算不变。下划线的绘制放在 `lib/pandagl/src/text/text_render.c::pd_text_render_line()`，与实际逐行绘制共用相同的行 y、`pd_text_get_line_start_x()`、`line->width` 和 `layer_pos`。
- 每行线段 y 由 `line top + line->baseline_y + offset` 计算；`auto` 的 offset 为一个逻辑像素对应的物理像素值。横向起点沿用行对齐位置及文本 offset，宽度使用该行的布局宽度。
- 线段通过 `pd_canvas_fill_rect()` 绘制，矩形高度取解析后的 thickness。先于字符位图绘制，使文字前景保持清晰；现有调用方提供的 canvas quote 与 render area 继续承担裁剪，不创建额外画布或缓冲。
- decoration、offset 或 thickness 改变时，Pandagl 标记完整文本层 dirty，使 partial render 也包含跨出常规行框但仍在文本层内的线；UI Text 将该变更提升为整个 content box dirty，确保旧线清除和新线重绘。装饰样式更新不得重置未改变的 CSS content，也不得触发文本 reflow。

## 不做的更改

- 不增加 Pandagl 字体引擎的 underline position/thickness 度量，也不修改字体后端。
- 不改变 `pd_text_update_line_size()` 中的 baseline 或行高算法，不让装饰线参与文本尺寸提示、换行和行高计算。
- 不为 TextInput、HTML inline 文本片段或富文本 style tag 添加 CSS text-decoration 支持。
- CSS `text-decoration` 的 shorthand 仅支持单关键字（`none | underline`），不实现 `underline solid red` 之类的多关键字组合；overline、line-through、`text-decoration-style`/`text-decoration-color` 长写同样暂不支持（解析这些值将按无效处理）。

## 验证

- CSS 单元测试覆盖关键字解析、三个属性的初始值、`none`/`underline` 级联、offset 与 thickness 的 `auto`/长度值，以及零/负 thickness 处理。
- CSS 单元测试覆盖 `text-decoration-line` 长写的 `none`/`underline` 解析、与 shorthand 共存时长写生效，以及 `overline` 等暂不支持值被拒绝。
- Pandagl 文本渲染测试覆盖装饰开关、offset 位移、粗细变化、零粗细、按行宽绘制以及多行文本；使用实际 canvas 像素检查线段位置、厚度和颜色。
- 集成测试通过公开 CSS 样式更新 text widget，覆盖默认无下划线、underline、none、正负 offset、粗细变化、零粗细、换行、居中/右对齐及绘制区域裁剪，并确认样式更新后旧线不会残留。
- 运行相关 CSS、Pandagl 与 LCUI 集成测试，并格式化所有修改的 C 源文件。

## 调研依据

- CSS 属性注册入口：`lib/css/src/properties.c::css_init_properties()`；关键字注册入口：`lib/css/src/keywords.c::css_init_keywords()`。
- computed style 的属性 key、类型位与数值字段定义：`lib/css/include/css/types.h`；绝对长度转换入口：`lib/css/src/computed.c::css_compute_absolute_values()`。
- UI 文本样式转换和 widget 同步：`lib/ui/src/ui_text_style.c`、`lib/ui/include/ui/text_style.h`、`src/widgets/text.c`。
- 行宽和 baseline 计算：`lib/pandagl/src/text/text.c::pd_text_update_line_size()`；行对齐与字形/画布渲染：`lib/pandagl/src/text/text_render.c`。
