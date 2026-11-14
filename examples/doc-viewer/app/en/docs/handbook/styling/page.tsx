import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";

export default function StylingDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Styling</h1>
      <p className="doc-page-desc">{`Use CSS to set colors, borders, padding, margins, dimensions, and layout for widgets.`}</p>
      <Widget className="doc-section">
        <h2>How to set styles</h2>
        <h3>Inline styles</h3>
        <p>{`Use [bgcolor=#7f7f7f29] ui_widget_set_style_string() [/bgcolor] to set individual CSS properties directly:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/styling/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">main.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "124px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *w = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`ui_widget_set_style_string(w, [color=#0a3069]"color"[/color], [color=#0a3069]"#336699"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`ui_widget_set_style_string(w, [color=#0a3069]"font-size"[/color], [color=#0a3069]"20px"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`ui_widget_set_style_string(w, [color=#0a3069]"padding"[/color], [color=#0a3069]"8px 16px"[/color]);`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ffa657]ui_widget_t[/color] *w = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`ui_widget_set_style_string(w, [color=#a5d6ff]"color"[/color], [color=#a5d6ff]"#336699"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`ui_widget_set_style_string(w, [color=#a5d6ff]"font-size"[/color], [color=#a5d6ff]"20px"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`ui_widget_set_style_string(w, [color=#a5d6ff]"padding"[/color], [color=#a5d6ff]"8px 16px"[/color]);`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <h3>CSS class selectors</h3>
        <p>{`Use [bgcolor=#7f7f7f29] ui_load_css_string() [/bgcolor] or [bgcolor=#7f7f7f29] ui_load_css_file() [/bgcolor] to load CSS rules, then apply them to widgets with [bgcolor=#7f7f7f29] ui_widget_add_class() [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/styling/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">main.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "104px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`ui_load_css_file([color=#0a3069]"styles.css"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *w = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`ui_widget_add_class(w, [color=#0a3069]"title"[/color]);`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`ui_load_css_file([color=#a5d6ff]"styles.css"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`[color=#ffa657]ui_widget_t[/color] *w = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`ui_widget_add_class(w, [color=#a5d6ff]"title"[/color]);`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="css" data-source="en/handbook/styling/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">app/styles.css</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "124px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#953800].title[/color] {`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  font-size: [color=#0550ae]24px[/color];`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  font-weight: bold;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  margin-bottom: [color=#0550ae]16px[/color];`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`}`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#79c0ff].title[/color] {`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  font-size: [color=#79c0ff]24px[/color];`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  font-weight: bold;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  margin-bottom: [color=#79c0ff]16px[/color];`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`}`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`You can also use [bgcolor=#7f7f7f29] ui_load_css_string() [/bgcolor] to load CSS rules from a string. The second parameter is a source identifier used for debugging to locate conflicting rules:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/styling/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">main.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`ui_load_css_string([color=#0a3069]".title { font-size: 24px; }"[/color], [color=#0a3069]"app.css"[/color]);`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`ui_load_css_string([color=#a5d6ff]".title { font-size: 24px; }"[/color], [color=#a5d6ff]"app.css"[/color]);`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Supported CSS features</h2>
        <p>{`LCUI's CSS engine implements a subset of the web standard. This section lists all supported features and their supported values; [b]anything not listed is not supported by default[/b].`}</p>
        <h3>At Rules</h3>
        <ul>
          <li><Text>{`[b][bgcolor=#7f7f7f29] @font-face [/bgcolor][/b] — Load external fonts`}</Text></li>
        </ul>
        <h3>Selectors</h3>
        <ul>
          <li><Text>{`[bgcolor=#7f7f7f29] * [/bgcolor] (universal), [bgcolor=#7f7f7f29] type [/bgcolor], [bgcolor=#7f7f7f29] #id [/bgcolor], [bgcolor=#7f7f7f29] .class [/bgcolor]`}</Text></li>
          <li><Text>{`Pseudo-classes [bgcolor=#7f7f7f29] :hover [/bgcolor], [bgcolor=#7f7f7f29] :focus [/bgcolor], [bgcolor=#7f7f7f29] :active [/bgcolor], [bgcolor=#7f7f7f29] :first-child [/bgcolor], [bgcolor=#7f7f7f29] :last-child [/bgcolor]`}</Text></li>
          <li><Text>{`[bgcolor=#7f7f7f29] !important [/bgcolor] is not supported`}</Text></li>
        </ul>
        <h3>Units</h3>
        <ul>
          <li><Text>{`[bgcolor=#7f7f7f29] px [/bgcolor], [bgcolor=#7f7f7f29] dp [/bgcolor], [bgcolor=#7f7f7f29] sp [/bgcolor], [bgcolor=#7f7f7f29] pt [/bgcolor], [bgcolor=#7f7f7f29] % [/bgcolor]`}</Text></li>
        </ul>
        <h3>Properties</h3>
        <h4>Layout</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#7f7f7f29] display [/bgcolor][/b] — [bgcolor=#7f7f7f29] none [/bgcolor], [bgcolor=#7f7f7f29] inline-block [/bgcolor], [bgcolor=#7f7f7f29] block [/bgcolor], [bgcolor=#7f7f7f29] flex [/bgcolor], [bgcolor=#7f7f7f29] inline-flex [/bgcolor], [bgcolor=#7f7f7f29] table [/bgcolor], [bgcolor=#7f7f7f29] inline-table [/bgcolor], [bgcolor=#7f7f7f29] table-row [/bgcolor], [bgcolor=#7f7f7f29] table-cell [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] position [/bgcolor][/b] — [bgcolor=#7f7f7f29] static [/bgcolor], [bgcolor=#7f7f7f29] relative [/bgcolor], [bgcolor=#7f7f7f29] absolute [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] top [/bgcolor] / [bgcolor=#7f7f7f29] right [/bgcolor] / [bgcolor=#7f7f7f29] bottom [/bgcolor] / [bgcolor=#7f7f7f29] left [/bgcolor][/b] — [bgcolor=#7f7f7f29] <length> [/bgcolor] or [bgcolor=#7f7f7f29] <percentage> [/bgcolor] or [bgcolor=#7f7f7f29] auto [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] z-index [/bgcolor][/b] — [bgcolor=#7f7f7f29] auto [/bgcolor] or integer`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] box-sizing [/bgcolor][/b] — [bgcolor=#7f7f7f29] content-box [/bgcolor], [bgcolor=#7f7f7f29] border-box [/bgcolor]`}</Text></li>
        </ul>
        <h4>Box model</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#7f7f7f29] width [/bgcolor] / [bgcolor=#7f7f7f29] height [/bgcolor][/b] — [bgcolor=#7f7f7f29] <length> [/bgcolor], [bgcolor=#7f7f7f29] <percentage> [/bgcolor], [bgcolor=#7f7f7f29] auto [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] min-width [/bgcolor] / [bgcolor=#7f7f7f29] max-width [/bgcolor] / [bgcolor=#7f7f7f29] min-height [/bgcolor] / [bgcolor=#7f7f7f29] max-height [/bgcolor][/b] — same as above`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] padding [/bgcolor][/b] — shorthand, 1-4 [bgcolor=#7f7f7f29] <length> [/bgcolor] values (e.g. [bgcolor=#7f7f7f29] 8px [/bgcolor], [bgcolor=#7f7f7f29] 4px 8px [/bgcolor], [bgcolor=#7f7f7f29] 4px 8px 12px [/bgcolor], [bgcolor=#7f7f7f29] 4px 8px 12px 16px [/bgcolor])`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] padding-top [/bgcolor] / [bgcolor=#7f7f7f29] padding-right [/bgcolor] / [bgcolor=#7f7f7f29] padding-bottom [/bgcolor] / [bgcolor=#7f7f7f29] padding-left [/bgcolor][/b] — longhand properties`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] margin [/bgcolor][/b] — shorthand, same syntax as padding`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] margin-top [/bgcolor] / [bgcolor=#7f7f7f29] margin-right [/bgcolor] / [bgcolor=#7f7f7f29] margin-bottom [/bgcolor] / [bgcolor=#7f7f7f29] margin-left [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] border [/bgcolor][/b] — shorthand [bgcolor=#7f7f7f29] 1px solid #ccc [/bgcolor] (width + style + color)`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] border-color [/bgcolor] / [bgcolor=#7f7f7f29] border-width [/bgcolor] / [bgcolor=#7f7f7f29] border-style [/bgcolor] / [bgcolor=#7f7f7f29] border-radius [/bgcolor][/b] — multi-value shorthand, same syntax as padding`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] border-top [/bgcolor] / [bgcolor=#7f7f7f29] border-right [/bgcolor] / [bgcolor=#7f7f7f29] border-bottom [/bgcolor] / [bgcolor=#7f7f7f29] border-left [/bgcolor][/b] — per-side shorthand`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] border-top-color [/bgcolor] / [bgcolor=#7f7f7f29] border-right-color [/bgcolor] / [bgcolor=#7f7f7f29] border-bottom-color [/bgcolor] / [bgcolor=#7f7f7f29] border-left-color [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] border-top-width [/bgcolor] / [bgcolor=#7f7f7f29] border-right-width [/bgcolor] / [bgcolor=#7f7f7f29] border-bottom-width [/bgcolor] / [bgcolor=#7f7f7f29] border-left-width [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] border-top-style [/bgcolor] / [bgcolor=#7f7f7f29] border-right-style [/bgcolor] / [bgcolor=#7f7f7f29] border-bottom-style [/bgcolor] / [bgcolor=#7f7f7f29] border-left-style [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] border-top-left-radius [/bgcolor] / [bgcolor=#7f7f7f29] border-top-right-radius [/bgcolor] / [bgcolor=#7f7f7f29] border-bottom-left-radius [/bgcolor] / [bgcolor=#7f7f7f29] border-bottom-right-radius [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] border-style [/bgcolor] values[/b] — [bgcolor=#7f7f7f29] none [/bgcolor], [bgcolor=#7f7f7f29] solid [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] table-layout [/bgcolor][/b] — [bgcolor=#7f7f7f29] auto [/bgcolor], [bgcolor=#7f7f7f29] fixed [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] border-spacing [/bgcolor][/b] — [bgcolor=#7f7f7f29] <length>{1,2} [/bgcolor]`}</Text></li>
        </ul>
        <h4>Background</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#7f7f7f29] background [/bgcolor][/b] — shorthand [bgcolor=#7f7f7f29] bg-image || bg-position || bg-size || repeat-style || color [/bgcolor] (single layer only)`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] background-color [/bgcolor][/b] — [bgcolor=#7f7f7f29] <color> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] background-image [/bgcolor][/b] — [bgcolor=#7f7f7f29] none [/bgcolor] or [bgcolor=#7f7f7f29] <url> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] background-position [/bgcolor][/b] — two values: x y`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] background-position-x [/bgcolor] / [bgcolor=#7f7f7f29] background-position-y [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] background-repeat [/bgcolor][/b] — [bgcolor=#7f7f7f29] <repeat-style> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] background-size [/bgcolor][/b] — [bgcolor=#7f7f7f29] <bg-size> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] background-clip [/bgcolor][/b] — [bgcolor=#7f7f7f29] border-box [/bgcolor], [bgcolor=#7f7f7f29] padding-box [/bgcolor], [bgcolor=#7f7f7f29] content-box [/bgcolor]`}</Text></li>
        </ul>
        <h4>Flexbox layout</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#7f7f7f29] flex [/bgcolor][/b] — shorthand ([bgcolor=#7f7f7f29] flex-grow [/bgcolor] + [bgcolor=#7f7f7f29] flex-shrink [/bgcolor] + [bgcolor=#7f7f7f29] flex-basis [/bgcolor])`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] flex-shrink [/bgcolor] / [bgcolor=#7f7f7f29] flex-grow [/bgcolor] / [bgcolor=#7f7f7f29] flex-basis [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] flex-wrap [/bgcolor][/b] — [bgcolor=#7f7f7f29] nowrap [/bgcolor], [bgcolor=#7f7f7f29] wrap [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] flex-direction [/bgcolor][/b] — [bgcolor=#7f7f7f29] row [/bgcolor], [bgcolor=#7f7f7f29] column [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] justify-content [/bgcolor][/b] — [bgcolor=#7f7f7f29] flex-start [/bgcolor], [bgcolor=#7f7f7f29] center [/bgcolor], [bgcolor=#7f7f7f29] flex-end [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] align-items [/bgcolor][/b] — [bgcolor=#7f7f7f29] flex-start [/bgcolor], [bgcolor=#7f7f7f29] center [/bgcolor], [bgcolor=#7f7f7f29] flex-end [/bgcolor], [bgcolor=#7f7f7f29] stretch [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] align-content [/bgcolor][/b] — same as justify-content plus [bgcolor=#7f7f7f29] space-between [/bgcolor], [bgcolor=#7f7f7f29] space-around [/bgcolor], [bgcolor=#7f7f7f29] space-evenly [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] gap [/bgcolor][/b] — shorthand, sets both row-gap and column-gap`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] row-gap [/bgcolor] / [bgcolor=#7f7f7f29] column-gap [/bgcolor][/b] — [bgcolor=#7f7f7f29] normal [/bgcolor] or [bgcolor=#7f7f7f29] <length-percentage> [/bgcolor]`}</Text></li>
        </ul>
        <h4>Typography</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#7f7f7f29] font-face [/bgcolor][/b] — (load fonts via [bgcolor=#7f7f7f29] @font-face [/bgcolor] rule)`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] font-family [/bgcolor][/b] — [bgcolor=#7f7f7f29] <font-family> [/bgcolor] (recommended to use built-in aliases like [bgcolor=#7f7f7f29] monospace [/bgcolor])`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] font-size [/bgcolor][/b] — [bgcolor=#7f7f7f29] <length> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] font-style [/bgcolor][/b] — [bgcolor=#7f7f7f29] normal [/bgcolor], [bgcolor=#7f7f7f29] italic [/bgcolor], [bgcolor=#7f7f7f29] oblique [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] font-weight [/bgcolor][/b] — [bgcolor=#7f7f7f29] normal [/bgcolor], [bgcolor=#7f7f7f29] bold [/bgcolor], [bgcolor=#7f7f7f29] <number> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] text-align [/bgcolor][/b] — [bgcolor=#7f7f7f29] left [/bgcolor], [bgcolor=#7f7f7f29] center [/bgcolor], [bgcolor=#7f7f7f29] right [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] line-height [/bgcolor][/b] — [bgcolor=#7f7f7f29] normal [/bgcolor] or [bgcolor=#7f7f7f29] <length> [/bgcolor] or [bgcolor=#7f7f7f29] <number> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] color [/bgcolor][/b] — [bgcolor=#7f7f7f29] <color> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] white-space [/bgcolor][/b] — [bgcolor=#7f7f7f29] normal [/bgcolor], [bgcolor=#7f7f7f29] nowrap [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] word-break [/bgcolor][/b] — [bgcolor=#7f7f7f29] normal [/bgcolor], [bgcolor=#7f7f7f29] break-all [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] content [/bgcolor][/b] — [bgcolor=#7f7f7f29] <string> [/bgcolor] or [bgcolor=#7f7f7f29] none [/bgcolor]`}</Text></li>
        </ul>
        <h4>Other</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#7f7f7f29] opacity [/bgcolor][/b] — [bgcolor=#7f7f7f29] <number> [/bgcolor] or [bgcolor=#7f7f7f29] <percentage> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] visibility [/bgcolor][/b] — [bgcolor=#7f7f7f29] visible [/bgcolor], [bgcolor=#7f7f7f29] hidden [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] pointer-events [/bgcolor][/b] — [bgcolor=#7f7f7f29] auto [/bgcolor], [bgcolor=#7f7f7f29] none [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] cursor [/bgcolor][/b] — [bgcolor=#7f7f7f29] auto [/bgcolor], [bgcolor=#7f7f7f29] default [/bgcolor], [bgcolor=#7f7f7f29] none [/bgcolor], [bgcolor=#7f7f7f29] context-menu [/bgcolor], [bgcolor=#7f7f7f29] help [/bgcolor], [bgcolor=#7f7f7f29] pointer [/bgcolor], [bgcolor=#7f7f7f29] progress [/bgcolor], [bgcolor=#7f7f7f29] wait [/bgcolor], [bgcolor=#7f7f7f29] cell [/bgcolor], [bgcolor=#7f7f7f29] crosshair [/bgcolor], [bgcolor=#7f7f7f29] text [/bgcolor], [bgcolor=#7f7f7f29] vertical-text [/bgcolor], [bgcolor=#7f7f7f29] alias [/bgcolor], [bgcolor=#7f7f7f29] copy [/bgcolor], [bgcolor=#7f7f7f29] move [/bgcolor], [bgcolor=#7f7f7f29] no-drop [/bgcolor], [bgcolor=#7f7f7f29] not-allowed [/bgcolor], [bgcolor=#7f7f7f29] grab [/bgcolor], [bgcolor=#7f7f7f29] grabbing [/bgcolor], [bgcolor=#7f7f7f29] all-scroll [/bgcolor], [bgcolor=#7f7f7f29] col-resize [/bgcolor], [bgcolor=#7f7f7f29] row-resize [/bgcolor], [bgcolor=#7f7f7f29] n/e/s/w-resize [/bgcolor], [bgcolor=#7f7f7f29] ew/ns-resize [/bgcolor], [bgcolor=#7f7f7f29] ne/nw/se/sw-resize [/bgcolor], [bgcolor=#7f7f7f29] nesw/nwse-resize [/bgcolor], [bgcolor=#7f7f7f29] zoom-in [/bgcolor], [bgcolor=#7f7f7f29] zoom-out [/bgcolor]; [bgcolor=#7f7f7f29] url() [/bgcolor] image cursors are not supported`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] box-shadow [/bgcolor][/b] — [bgcolor=#7f7f7f29] none [/bgcolor] or [bgcolor=#7f7f7f29] <shadow> [/bgcolor] (format [bgcolor=#7f7f7f29] <length>{2,4} && <color>? [/bgcolor])`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Caveats</h2>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#7f7f7f29] white-space: pre [/bgcolor] does not work.[/b] LCUI's [bgcolor=#7f7f7f29] white-space [/bgcolor] only implements [bgcolor=#7f7f7f29] normal [/bgcolor] and [bgcolor=#7f7f7f29] nowrap [/bgcolor]. If you need to preserve leading whitespace from source code, replace spaces with U+00A0 (NBSP).`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#7f7f7f29] overflow [/bgcolor] does not clip content.[/b] To clip or scroll child content, use the [bgcolor=#7f7f7f29] scrollarea [/bgcolor] built-in widget instead.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#7f7f7f29] background [/bgcolor] shorthand does not support multiple layers.[/b] Writing multi-background syntax like [bgcolor=#7f7f7f29] background: url(a.png) no-repeat, url(b.png) center; [/bgcolor] will not work. Use individual properties like [bgcolor=#7f7f7f29] background-image [/bgcolor] / [bgcolor=#7f7f7f29] background-position [/bgcolor] to set each layer separately.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#7f7f7f29] cursor [/bgcolor] maps to system cursors approximately; [bgcolor=#7f7f7f29] url() [/bgcolor] image cursors are not supported.[/b] The [bgcolor=#7f7f7f29] cursor [/bgcolor] value of the hovered widget takes effect in the same frame (including state changes like [bgcolor=#7f7f7f29] :hover [/bgcolor]). On Windows/X11 values map to system cursors, and those without an exact match use the closest shape; [bgcolor=#7f7f7f29] none [/bgcolor] hides the pointer; the framebuffer software cursor is unaffected. Built-in interactive widgets ship with sensible defaults ([bgcolor=#7f7f7f29] button [/bgcolor] etc. [bgcolor=#7f7f7f29] pointer [/bgcolor], [bgcolor=#7f7f7f29] textinput [/bgcolor] [bgcolor=#7f7f7f29] text [/bgcolor], scrollbar thumb [bgcolor=#7f7f7f29] grab [/bgcolor]).`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
