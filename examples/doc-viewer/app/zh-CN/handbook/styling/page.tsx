import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function StylingDocPage() {
  return (
    <Widget className="doc-page">
      <h1>样式</h1>
      <p className="doc-page-desc">{`用 CSS 给部件设置颜色、边框、内外边距、尺寸和布局。`}</p>
      <Widget className="doc-section">
        <h2>设置方式</h2>
        <h3>内联样式</h3>
        <p>{`用 [bgcolor=#eee] ui_widget_set_style_string() [/bgcolor] 直接设置单个 CSS 属性：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/handbook/styling/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">main.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *w = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`ui_widget_set_style_string(w, [color=#0a3069]"color"[/color], [color=#0a3069]"#336699"[/color]);`}</Text>
            <Text className="code-line">{`ui_widget_set_style_string(w, [color=#0a3069]"font-size"[/color], [color=#0a3069]"20px"[/color]);`}</Text>
            <Text className="code-line">{`ui_widget_set_style_string(w, [color=#0a3069]"padding"[/color], [color=#0a3069]"8px 16px"[/color]);`}</Text>
          </Widget>
        </Widget>
        <h3>CSS 类选择器</h3>
        <p>{`用 [bgcolor=#eee] ui_load_css_string() [/bgcolor] 或 [bgcolor=#eee] ui_load_css_file() [/bgcolor] 加载 CSS 规则，再用 [bgcolor=#eee] ui_widget_add_class() [/bgcolor] 给部件加类名使其生效：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/handbook/styling/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">main.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`ui_load_css_file([color=#0a3069]"styles.css"[/color]);`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *w = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
            <Text className="code-line">{`ui_widget_add_class(w, [color=#0a3069]"title"[/color]);`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="css" data-source="zh-CN/handbook/styling/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">app/styles.css</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800].title[/color] {`}</Text>
            <Text className="code-line">{`  font-size: [color=#0550ae]24px[/color];`}</Text>
            <Text className="code-line">{`  font-weight: bold;`}</Text>
            <Text className="code-line">{`  margin-bottom: [color=#0550ae]16px[/color];`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
        <p>{`也可以用 [bgcolor=#eee] ui_load_css_string() [/bgcolor] 从字符串加载 CSS 规则，第二个参数是来源标识，用于调试定位冲突规则：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/handbook/styling/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">main.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`ui_load_css_string([color=#0a3069]".title { font-size: 24px; }"[/color], [color=#0a3069]"app.css"[/color]);`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>支持的 CSS 特性</h2>
        <p>{`LCUI 的 CSS 引擎实现了 Web 标准的一个子集。本节列出所有已支持的特性和它们支持的值；[b]未列出的特性默认不支持[/b]。`}</p>
        <h3>At Rules</h3>
        <ul>
          <li><Text>{`[b][bgcolor=#eee] @font-face [/bgcolor][/b] — 加载外部字体`}</Text></li>
        </ul>
        <h3>选择器</h3>
        <ul>
          <li><Text>{`[bgcolor=#eee] * [/bgcolor]（通配符）、[bgcolor=#eee] type [/bgcolor]、[bgcolor=#eee] #id [/bgcolor]、[bgcolor=#eee] .class [/bgcolor]`}</Text></li>
          <li><Text>{`伪类 [bgcolor=#eee] :hover [/bgcolor]、[bgcolor=#eee] :focus [/bgcolor]、[bgcolor=#eee] :active [/bgcolor]、[bgcolor=#eee] :first-child [/bgcolor]、[bgcolor=#eee] :last-child [/bgcolor]`}</Text></li>
          <li><Text>{`不支持 [bgcolor=#eee] !important [/bgcolor]`}</Text></li>
        </ul>
        <h3>单位</h3>
        <ul>
          <li><Text>{`[bgcolor=#eee] px [/bgcolor]、[bgcolor=#eee] dp [/bgcolor]、[bgcolor=#eee] sp [/bgcolor]、[bgcolor=#eee] pt [/bgcolor]、[bgcolor=#eee] % [/bgcolor]`}</Text></li>
        </ul>
        <h3>属性</h3>
        <h4>布局</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#eee] display [/bgcolor][/b] — [bgcolor=#eee] none [/bgcolor]、[bgcolor=#eee] inline-block [/bgcolor]、[bgcolor=#eee] block [/bgcolor]、[bgcolor=#eee] flex [/bgcolor]、[bgcolor=#eee] inline-flex [/bgcolor]、[bgcolor=#eee] table [/bgcolor]、[bgcolor=#eee] inline-table [/bgcolor]、[bgcolor=#eee] table-row [/bgcolor]、[bgcolor=#eee] table-cell [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] position [/bgcolor][/b] — [bgcolor=#eee] static [/bgcolor]、[bgcolor=#eee] relative [/bgcolor]、[bgcolor=#eee] absolute [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] top [/bgcolor] / [bgcolor=#eee] right [/bgcolor] / [bgcolor=#eee] bottom [/bgcolor] / [bgcolor=#eee] left [/bgcolor][/b] — [bgcolor=#eee] <length> [/bgcolor] 或 [bgcolor=#eee] <percentage> [/bgcolor] 或 [bgcolor=#eee] auto [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] z-index [/bgcolor][/b] — [bgcolor=#eee] auto [/bgcolor] 或整数`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] box-sizing [/bgcolor][/b] — [bgcolor=#eee] content-box [/bgcolor]、[bgcolor=#eee] border-box [/bgcolor]`}</Text></li>
        </ul>
        <h4>盒模型</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#eee] width [/bgcolor] / [bgcolor=#eee] height [/bgcolor][/b] — [bgcolor=#eee] <length> [/bgcolor]、[bgcolor=#eee] <percentage> [/bgcolor]、[bgcolor=#eee] auto [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] min-width [/bgcolor] / [bgcolor=#eee] max-width [/bgcolor] / [bgcolor=#eee] min-height [/bgcolor] / [bgcolor=#eee] max-height [/bgcolor][/b] — 同上`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] padding [/bgcolor][/b] — 简写，1-4 个 [bgcolor=#eee] <length> [/bgcolor] 值（如 [bgcolor=#eee] 8px [/bgcolor]、[bgcolor=#eee] 4px 8px [/bgcolor]、[bgcolor=#eee] 4px 8px 12px [/bgcolor]、[bgcolor=#eee] 4px 8px 12px 16px [/bgcolor]）`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] padding-top [/bgcolor] / [bgcolor=#eee] padding-right [/bgcolor] / [bgcolor=#eee] padding-bottom [/bgcolor] / [bgcolor=#eee] padding-left [/bgcolor][/b] — 长边属性`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] margin [/bgcolor][/b] — 简写，同 padding`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] margin-top [/bgcolor] / [bgcolor=#eee] margin-right [/bgcolor] / [bgcolor=#eee] margin-bottom [/bgcolor] / [bgcolor=#eee] margin-left [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] border [/bgcolor][/b] — 简写 [bgcolor=#eee] 1px solid #ccc [/bgcolor]（width + style + color）`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] border-color [/bgcolor] / [bgcolor=#eee] border-width [/bgcolor] / [bgcolor=#eee] border-style [/bgcolor] / [bgcolor=#eee] border-radius [/bgcolor][/b] — 同 padding 语法的多值简写`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] border-top [/bgcolor] / [bgcolor=#eee] border-right [/bgcolor] / [bgcolor=#eee] border-bottom [/bgcolor] / [bgcolor=#eee] border-left [/bgcolor][/b] — 单边简写`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] border-top-color [/bgcolor] / [bgcolor=#eee] border-right-color [/bgcolor] / [bgcolor=#eee] border-bottom-color [/bgcolor] / [bgcolor=#eee] border-left-color [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] border-top-width [/bgcolor] / [bgcolor=#eee] border-right-width [/bgcolor] / [bgcolor=#eee] border-bottom-width [/bgcolor] / [bgcolor=#eee] border-left-width [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] border-top-style [/bgcolor] / [bgcolor=#eee] border-right-style [/bgcolor] / [bgcolor=#eee] border-bottom-style [/bgcolor] / [bgcolor=#eee] border-left-style [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] border-top-left-radius [/bgcolor] / [bgcolor=#eee] border-top-right-radius [/bgcolor] / [bgcolor=#eee] border-bottom-left-radius [/bgcolor] / [bgcolor=#eee] border-bottom-right-radius [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] border-style [/bgcolor] 可取值[/b] — [bgcolor=#eee] none [/bgcolor]、[bgcolor=#eee] solid [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] table-layout [/bgcolor][/b] — [bgcolor=#eee] auto [/bgcolor]、[bgcolor=#eee] fixed [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] border-spacing [/bgcolor][/b] — [bgcolor=#eee] <length>{1,2} [/bgcolor]`}</Text></li>
        </ul>
        <h4>背景</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#eee] background [/bgcolor][/b] — 简写形式 [bgcolor=#eee] bg-image || bg-position || bg-size || repeat-style || color [/bgcolor]（仅单图层）`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] background-color [/bgcolor][/b] — [bgcolor=#eee] <color> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] background-image [/bgcolor][/b] — [bgcolor=#eee] none [/bgcolor] 或 [bgcolor=#eee] <url> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] background-position [/bgcolor][/b] — x y 两个值`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] background-position-x [/bgcolor] / [bgcolor=#eee] background-position-y [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] background-repeat [/bgcolor][/b] — [bgcolor=#eee] <repeat-style> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] background-size [/bgcolor][/b] — [bgcolor=#eee] <bg-size> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] background-clip [/bgcolor][/b] — [bgcolor=#eee] border-box [/bgcolor]、[bgcolor=#eee] padding-box [/bgcolor]、[bgcolor=#eee] content-box [/bgcolor]`}</Text></li>
        </ul>
        <h4>Flexbox 布局</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#eee] flex [/bgcolor][/b] — 简写（[bgcolor=#eee] flex-grow [/bgcolor] + [bgcolor=#eee] flex-shrink [/bgcolor] + [bgcolor=#eee] flex-basis [/bgcolor]）`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] flex-shrink [/bgcolor] / [bgcolor=#eee] flex-grow [/bgcolor] / [bgcolor=#eee] flex-basis [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] flex-wrap [/bgcolor][/b] — [bgcolor=#eee] nowrap [/bgcolor]、[bgcolor=#eee] wrap [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] flex-direction [/bgcolor][/b] — [bgcolor=#eee] row [/bgcolor]、[bgcolor=#eee] column [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] justify-content [/bgcolor][/b] — [bgcolor=#eee] flex-start [/bgcolor]、[bgcolor=#eee] center [/bgcolor]、[bgcolor=#eee] flex-end [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] align-items [/bgcolor][/b] — [bgcolor=#eee] flex-start [/bgcolor]、[bgcolor=#eee] center [/bgcolor]、[bgcolor=#eee] flex-end [/bgcolor]、[bgcolor=#eee] stretch [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] align-content [/bgcolor][/b] — 同 justify-content 加 [bgcolor=#eee] space-between [/bgcolor]、[bgcolor=#eee] space-around [/bgcolor]、[bgcolor=#eee] space-evenly [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] gap [/bgcolor][/b] — 简写，同时设 row-gap + column-gap`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] row-gap [/bgcolor] / [bgcolor=#eee] column-gap [/bgcolor][/b] — [bgcolor=#eee] normal [/bgcolor] 或 [bgcolor=#eee] <length-percentage> [/bgcolor]`}</Text></li>
        </ul>
        <h4>排版</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#eee] font-face [/bgcolor][/b] — （通过 [bgcolor=#eee] @font-face [/bgcolor] 规则加载字体）`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] font-family [/bgcolor][/b] — [bgcolor=#eee] <font-family> [/bgcolor]（建议用内置别名如 [bgcolor=#eee] monospace [/bgcolor]）`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] font-size [/bgcolor][/b] — [bgcolor=#eee] <length> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] font-style [/bgcolor][/b] — [bgcolor=#eee] normal [/bgcolor]、[bgcolor=#eee] italic [/bgcolor]、[bgcolor=#eee] oblique [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] font-weight [/bgcolor][/b] — [bgcolor=#eee] normal [/bgcolor]、[bgcolor=#eee] bold [/bgcolor]、[bgcolor=#eee] <number> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] text-align [/bgcolor][/b] — [bgcolor=#eee] left [/bgcolor]、[bgcolor=#eee] center [/bgcolor]、[bgcolor=#eee] right [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] line-height [/bgcolor][/b] — [bgcolor=#eee] normal [/bgcolor] 或 [bgcolor=#eee] <length> [/bgcolor] 或 [bgcolor=#eee] <number> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] color [/bgcolor][/b] — [bgcolor=#eee] <color> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] white-space [/bgcolor][/b] — [bgcolor=#eee] normal [/bgcolor]、[bgcolor=#eee] nowrap [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] word-break [/bgcolor][/b] — [bgcolor=#eee] normal [/bgcolor]、[bgcolor=#eee] break-all [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] content [/bgcolor][/b] — [bgcolor=#eee] <string> [/bgcolor] 或 [bgcolor=#eee] none [/bgcolor]`}</Text></li>
        </ul>
        <h4>其他</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#eee] opacity [/bgcolor][/b] — [bgcolor=#eee] <number> [/bgcolor] 或 [bgcolor=#eee] <percentage> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] visibility [/bgcolor][/b] — [bgcolor=#eee] visible [/bgcolor]、[bgcolor=#eee] hidden [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] pointer-events [/bgcolor][/b] — [bgcolor=#eee] auto [/bgcolor]、[bgcolor=#eee] none [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] box-shadow [/bgcolor][/b] — [bgcolor=#eee] none [/bgcolor] 或 [bgcolor=#eee] <shadow> [/bgcolor]（格式 [bgcolor=#eee] <length>{2,4} && <color>? [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>注意事项</h2>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#eee] white-space: pre [/bgcolor] 无效。[/b] LCUI 的 [bgcolor=#eee] white-space [/bgcolor] 仅实现 [bgcolor=#eee] normal [/bgcolor] 和 [bgcolor=#eee] nowrap [/bgcolor]。如需保留源代码前导缩进，把空格换成 U+00A0 (NBSP)。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#eee] overflow [/bgcolor] 不裁切内容。[/b] 要裁切或滚动子内容，改用 [bgcolor=#eee] scrollarea [/bgcolor] 内置部件。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#eee] background [/bgcolor] 简写不支持多图层。[/b] 写 [bgcolor=#eee] background: url(a.png) no-repeat, url(b.png) center; [/bgcolor] 这种多背景语法不会生效。请拆成 [bgcolor=#eee] background-image [/bgcolor] / [bgcolor=#eee] background-position [/bgcolor] 等分属性逐个设置。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b]CSS 不支持继承。[/b] 像 [bgcolor=#eee] color [/bgcolor]、[bgcolor=#eee] font-family [/bgcolor]、[bgcolor=#eee] font-size [/bgcolor] 不会从父部件流向子部件。每个 [bgcolor=#eee] text [/bgcolor] / [bgcolor=#eee] button [/bgcolor] 等需要文本显示的部件都必须显式设置这些属性。`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
