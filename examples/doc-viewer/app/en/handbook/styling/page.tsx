import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function StylingDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Styling</h1>
      <p className="doc-page-desc">{`Use CSS to set colors, borders, padding, margins, dimensions, and layout for widgets.`}</p>
      <Widget className="doc-section">
        <h2>How to set styles</h2>
        <h3>Inline styles</h3>
        <p>{`Use [bgcolor=#eee] ui_widget_set_style_string() [/bgcolor] to set individual CSS properties directly:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/styling/0">
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
        <h3>CSS class selectors</h3>
        <p>{`Use [bgcolor=#eee] ui_load_css_string() [/bgcolor] or [bgcolor=#eee] ui_load_css_file() [/bgcolor] to load CSS rules, then apply them to widgets with [bgcolor=#eee] ui_widget_add_class() [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/styling/1">
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
        <Widget className="code-block" data-language="css" data-source="en/handbook/styling/2">
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
        <p>{`You can also use [bgcolor=#eee] ui_load_css_string() [/bgcolor] to load CSS rules from a string. The second parameter is a source identifier used for debugging to locate conflicting rules:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/styling/3">
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
        <h2>Supported CSS features</h2>
        <p>{`LCUI's CSS engine implements a subset of the web standard. This section lists all supported features and their supported values; [b]anything not listed is not supported by default[/b].`}</p>
        <h3>At Rules</h3>
        <ul>
          <li><Text>{`[b][bgcolor=#eee] @font-face [/bgcolor][/b] — Load external fonts`}</Text></li>
        </ul>
        <h3>Selectors</h3>
        <ul>
          <li><Text>{`[bgcolor=#eee] * [/bgcolor] (universal), [bgcolor=#eee] type [/bgcolor], [bgcolor=#eee] #id [/bgcolor], [bgcolor=#eee] .class [/bgcolor]`}</Text></li>
          <li><Text>{`Pseudo-classes [bgcolor=#eee] :hover [/bgcolor], [bgcolor=#eee] :focus [/bgcolor], [bgcolor=#eee] :active [/bgcolor], [bgcolor=#eee] :first-child [/bgcolor], [bgcolor=#eee] :last-child [/bgcolor]`}</Text></li>
          <li><Text>{`[bgcolor=#eee] !important [/bgcolor] is not supported`}</Text></li>
        </ul>
        <h3>Units</h3>
        <ul>
          <li><Text>{`[bgcolor=#eee] px [/bgcolor], [bgcolor=#eee] dp [/bgcolor], [bgcolor=#eee] sp [/bgcolor], [bgcolor=#eee] pt [/bgcolor], [bgcolor=#eee] % [/bgcolor]`}</Text></li>
        </ul>
        <h3>Properties</h3>
        <h4>Layout</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#eee] display [/bgcolor][/b] — [bgcolor=#eee] none [/bgcolor], [bgcolor=#eee] inline-block [/bgcolor], [bgcolor=#eee] block [/bgcolor], [bgcolor=#eee] flex [/bgcolor], [bgcolor=#eee] inline-flex [/bgcolor], [bgcolor=#eee] table [/bgcolor], [bgcolor=#eee] inline-table [/bgcolor], [bgcolor=#eee] table-row [/bgcolor], [bgcolor=#eee] table-cell [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] position [/bgcolor][/b] — [bgcolor=#eee] static [/bgcolor], [bgcolor=#eee] relative [/bgcolor], [bgcolor=#eee] absolute [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] top [/bgcolor] / [bgcolor=#eee] right [/bgcolor] / [bgcolor=#eee] bottom [/bgcolor] / [bgcolor=#eee] left [/bgcolor][/b] — [bgcolor=#eee] <length> [/bgcolor] or [bgcolor=#eee] <percentage> [/bgcolor] or [bgcolor=#eee] auto [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] z-index [/bgcolor][/b] — [bgcolor=#eee] auto [/bgcolor] or integer`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] box-sizing [/bgcolor][/b] — [bgcolor=#eee] content-box [/bgcolor], [bgcolor=#eee] border-box [/bgcolor]`}</Text></li>
        </ul>
        <h4>Box model</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#eee] width [/bgcolor] / [bgcolor=#eee] height [/bgcolor][/b] — [bgcolor=#eee] <length> [/bgcolor], [bgcolor=#eee] <percentage> [/bgcolor], [bgcolor=#eee] auto [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] min-width [/bgcolor] / [bgcolor=#eee] max-width [/bgcolor] / [bgcolor=#eee] min-height [/bgcolor] / [bgcolor=#eee] max-height [/bgcolor][/b] — same as above`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] padding [/bgcolor][/b] — shorthand, 1-4 [bgcolor=#eee] <length> [/bgcolor] values (e.g. [bgcolor=#eee] 8px [/bgcolor], [bgcolor=#eee] 4px 8px [/bgcolor], [bgcolor=#eee] 4px 8px 12px [/bgcolor], [bgcolor=#eee] 4px 8px 12px 16px [/bgcolor])`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] padding-top [/bgcolor] / [bgcolor=#eee] padding-right [/bgcolor] / [bgcolor=#eee] padding-bottom [/bgcolor] / [bgcolor=#eee] padding-left [/bgcolor][/b] — longhand properties`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] margin [/bgcolor][/b] — shorthand, same syntax as padding`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] margin-top [/bgcolor] / [bgcolor=#eee] margin-right [/bgcolor] / [bgcolor=#eee] margin-bottom [/bgcolor] / [bgcolor=#eee] margin-left [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] border [/bgcolor][/b] — shorthand [bgcolor=#eee] 1px solid #ccc [/bgcolor] (width + style + color)`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] border-color [/bgcolor] / [bgcolor=#eee] border-width [/bgcolor] / [bgcolor=#eee] border-style [/bgcolor] / [bgcolor=#eee] border-radius [/bgcolor][/b] — multi-value shorthand, same syntax as padding`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] border-top [/bgcolor] / [bgcolor=#eee] border-right [/bgcolor] / [bgcolor=#eee] border-bottom [/bgcolor] / [bgcolor=#eee] border-left [/bgcolor][/b] — per-side shorthand`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] border-top-color [/bgcolor] / [bgcolor=#eee] border-right-color [/bgcolor] / [bgcolor=#eee] border-bottom-color [/bgcolor] / [bgcolor=#eee] border-left-color [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] border-top-width [/bgcolor] / [bgcolor=#eee] border-right-width [/bgcolor] / [bgcolor=#eee] border-bottom-width [/bgcolor] / [bgcolor=#eee] border-left-width [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] border-top-style [/bgcolor] / [bgcolor=#eee] border-right-style [/bgcolor] / [bgcolor=#eee] border-bottom-style [/bgcolor] / [bgcolor=#eee] border-left-style [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] border-top-left-radius [/bgcolor] / [bgcolor=#eee] border-top-right-radius [/bgcolor] / [bgcolor=#eee] border-bottom-left-radius [/bgcolor] / [bgcolor=#eee] border-bottom-right-radius [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] border-style [/bgcolor] values[/b] — [bgcolor=#eee] none [/bgcolor], [bgcolor=#eee] solid [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] table-layout [/bgcolor][/b] — [bgcolor=#eee] auto [/bgcolor], [bgcolor=#eee] fixed [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] border-spacing [/bgcolor][/b] — [bgcolor=#eee] <length>{1,2} [/bgcolor]`}</Text></li>
        </ul>
        <h4>Background</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#eee] background [/bgcolor][/b] — shorthand [bgcolor=#eee] bg-image || bg-position || bg-size || repeat-style || color [/bgcolor] (single layer only)`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] background-color [/bgcolor][/b] — [bgcolor=#eee] <color> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] background-image [/bgcolor][/b] — [bgcolor=#eee] none [/bgcolor] or [bgcolor=#eee] <url> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] background-position [/bgcolor][/b] — two values: x y`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] background-position-x [/bgcolor] / [bgcolor=#eee] background-position-y [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] background-repeat [/bgcolor][/b] — [bgcolor=#eee] <repeat-style> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] background-size [/bgcolor][/b] — [bgcolor=#eee] <bg-size> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] background-clip [/bgcolor][/b] — [bgcolor=#eee] border-box [/bgcolor], [bgcolor=#eee] padding-box [/bgcolor], [bgcolor=#eee] content-box [/bgcolor]`}</Text></li>
        </ul>
        <h4>Flexbox layout</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#eee] flex [/bgcolor][/b] — shorthand ([bgcolor=#eee] flex-grow [/bgcolor] + [bgcolor=#eee] flex-shrink [/bgcolor] + [bgcolor=#eee] flex-basis [/bgcolor])`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] flex-shrink [/bgcolor] / [bgcolor=#eee] flex-grow [/bgcolor] / [bgcolor=#eee] flex-basis [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] flex-wrap [/bgcolor][/b] — [bgcolor=#eee] nowrap [/bgcolor], [bgcolor=#eee] wrap [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] flex-direction [/bgcolor][/b] — [bgcolor=#eee] row [/bgcolor], [bgcolor=#eee] column [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] justify-content [/bgcolor][/b] — [bgcolor=#eee] flex-start [/bgcolor], [bgcolor=#eee] center [/bgcolor], [bgcolor=#eee] flex-end [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] align-items [/bgcolor][/b] — [bgcolor=#eee] flex-start [/bgcolor], [bgcolor=#eee] center [/bgcolor], [bgcolor=#eee] flex-end [/bgcolor], [bgcolor=#eee] stretch [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] align-content [/bgcolor][/b] — same as justify-content plus [bgcolor=#eee] space-between [/bgcolor], [bgcolor=#eee] space-around [/bgcolor], [bgcolor=#eee] space-evenly [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] gap [/bgcolor][/b] — shorthand, sets both row-gap and column-gap`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] row-gap [/bgcolor] / [bgcolor=#eee] column-gap [/bgcolor][/b] — [bgcolor=#eee] normal [/bgcolor] or [bgcolor=#eee] <length-percentage> [/bgcolor]`}</Text></li>
        </ul>
        <h4>Typography</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#eee] font-face [/bgcolor][/b] — (load fonts via [bgcolor=#eee] @font-face [/bgcolor] rule)`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] font-family [/bgcolor][/b] — [bgcolor=#eee] <font-family> [/bgcolor] (recommended to use built-in aliases like [bgcolor=#eee] monospace [/bgcolor])`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] font-size [/bgcolor][/b] — [bgcolor=#eee] <length> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] font-style [/bgcolor][/b] — [bgcolor=#eee] normal [/bgcolor], [bgcolor=#eee] italic [/bgcolor], [bgcolor=#eee] oblique [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] font-weight [/bgcolor][/b] — [bgcolor=#eee] normal [/bgcolor], [bgcolor=#eee] bold [/bgcolor], [bgcolor=#eee] <number> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] text-align [/bgcolor][/b] — [bgcolor=#eee] left [/bgcolor], [bgcolor=#eee] center [/bgcolor], [bgcolor=#eee] right [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] line-height [/bgcolor][/b] — [bgcolor=#eee] normal [/bgcolor] or [bgcolor=#eee] <length> [/bgcolor] or [bgcolor=#eee] <number> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] color [/bgcolor][/b] — [bgcolor=#eee] <color> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] white-space [/bgcolor][/b] — [bgcolor=#eee] normal [/bgcolor], [bgcolor=#eee] nowrap [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] word-break [/bgcolor][/b] — [bgcolor=#eee] normal [/bgcolor], [bgcolor=#eee] break-all [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] content [/bgcolor][/b] — [bgcolor=#eee] <string> [/bgcolor] or [bgcolor=#eee] none [/bgcolor]`}</Text></li>
        </ul>
        <h4>Other</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#eee] opacity [/bgcolor][/b] — [bgcolor=#eee] <number> [/bgcolor] or [bgcolor=#eee] <percentage> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] visibility [/bgcolor][/b] — [bgcolor=#eee] visible [/bgcolor], [bgcolor=#eee] hidden [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] pointer-events [/bgcolor][/b] — [bgcolor=#eee] auto [/bgcolor], [bgcolor=#eee] none [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] box-shadow [/bgcolor][/b] — [bgcolor=#eee] none [/bgcolor] or [bgcolor=#eee] <shadow> [/bgcolor] (format [bgcolor=#eee] <length>{2,4} && <color>? [/bgcolor])`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Caveats</h2>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#eee] white-space: pre [/bgcolor] does not work.[/b] LCUI's [bgcolor=#eee] white-space [/bgcolor] only implements [bgcolor=#eee] normal [/bgcolor] and [bgcolor=#eee] nowrap [/bgcolor]. If you need to preserve leading whitespace from source code, replace spaces with U+00A0 (NBSP).`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#eee] overflow [/bgcolor] does not clip content.[/b] To clip or scroll child content, use the [bgcolor=#eee] scrollarea [/bgcolor] built-in widget instead.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#eee] background [/bgcolor] shorthand does not support multiple layers.[/b] Writing multi-background syntax like [bgcolor=#eee] background: url(a.png) no-repeat, url(b.png) center; [/bgcolor] will not work. Use individual properties like [bgcolor=#eee] background-image [/bgcolor] / [bgcolor=#eee] background-position [/bgcolor] to set each layer separately.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b]CSS does not support inheritance.[/b] Properties like [bgcolor=#eee] color [/bgcolor], [bgcolor=#eee] font-family [/bgcolor], and [bgcolor=#eee] font-size [/bgcolor] do not cascade from parent to child widgets. Every widget that displays text (e.g. [bgcolor=#eee] text [/bgcolor], [bgcolor=#eee] button [/bgcolor]) must explicitly set these properties.`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
