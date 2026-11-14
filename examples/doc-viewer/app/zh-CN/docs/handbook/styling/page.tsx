import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";

export default function StylingDocPage() {
  return (
    <Widget className="doc-page">
      <h1>样式</h1>
      <p className="doc-page-desc">{`用 CSS 给部件设置颜色、边框、内外边距、尺寸和布局。`}</p>
      <Widget className="doc-section">
        <h2>设置方式</h2>
        <h3>内联样式</h3>
        <p>{`用 [bgcolor=#7f7f7f29] ui_widget_set_style_string() [/bgcolor] 直接设置单个 CSS 属性：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/handbook/styling/0">
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
        <h3>CSS 类选择器</h3>
        <p>{`用 [bgcolor=#7f7f7f29] ui_load_css_string() [/bgcolor] 或 [bgcolor=#7f7f7f29] ui_load_css_file() [/bgcolor] 加载 CSS 规则，再用 [bgcolor=#7f7f7f29] ui_widget_add_class() [/bgcolor] 给部件加类名使其生效：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/handbook/styling/1">
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
        <Widget className="code-block" data-language="css" data-source="zh-CN/handbook/styling/2">
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
        <p>{`也可以用 [bgcolor=#7f7f7f29] ui_load_css_string() [/bgcolor] 从字符串加载 CSS 规则，第二个参数是来源标识，用于调试定位冲突规则：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/handbook/styling/3">
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
        <h2>支持的 CSS 特性</h2>
        <p>{`LCUI 的 CSS 引擎实现了 Web 标准的一个子集。本节列出所有已支持的特性和它们支持的值；[b]未列出的特性默认不支持[/b]。`}</p>
        <h3>At Rules</h3>
        <ul>
          <li><Text>{`[b][bgcolor=#7f7f7f29] @font-face [/bgcolor][/b] — 加载外部字体`}</Text></li>
        </ul>
        <h3>选择器</h3>
        <ul>
          <li><Text>{`[bgcolor=#7f7f7f29] * [/bgcolor]（通配符）、[bgcolor=#7f7f7f29] type [/bgcolor]、[bgcolor=#7f7f7f29] #id [/bgcolor]、[bgcolor=#7f7f7f29] .class [/bgcolor]`}</Text></li>
          <li><Text>{`伪类 [bgcolor=#7f7f7f29] :hover [/bgcolor]、[bgcolor=#7f7f7f29] :focus [/bgcolor]、[bgcolor=#7f7f7f29] :active [/bgcolor]、[bgcolor=#7f7f7f29] :first-child [/bgcolor]、[bgcolor=#7f7f7f29] :last-child [/bgcolor]`}</Text></li>
          <li><Text>{`不支持 [bgcolor=#7f7f7f29] !important [/bgcolor]`}</Text></li>
        </ul>
        <h3>单位</h3>
        <ul>
          <li><Text>{`[bgcolor=#7f7f7f29] px [/bgcolor]、[bgcolor=#7f7f7f29] dp [/bgcolor]、[bgcolor=#7f7f7f29] sp [/bgcolor]、[bgcolor=#7f7f7f29] pt [/bgcolor]、[bgcolor=#7f7f7f29] % [/bgcolor]`}</Text></li>
        </ul>
        <h3>属性</h3>
        <h4>布局</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#7f7f7f29] display [/bgcolor][/b] — [bgcolor=#7f7f7f29] none [/bgcolor]、[bgcolor=#7f7f7f29] inline-block [/bgcolor]、[bgcolor=#7f7f7f29] block [/bgcolor]、[bgcolor=#7f7f7f29] flex [/bgcolor]、[bgcolor=#7f7f7f29] inline-flex [/bgcolor]、[bgcolor=#7f7f7f29] table [/bgcolor]、[bgcolor=#7f7f7f29] inline-table [/bgcolor]、[bgcolor=#7f7f7f29] table-row [/bgcolor]、[bgcolor=#7f7f7f29] table-cell [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] position [/bgcolor][/b] — [bgcolor=#7f7f7f29] static [/bgcolor]、[bgcolor=#7f7f7f29] relative [/bgcolor]、[bgcolor=#7f7f7f29] absolute [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] top [/bgcolor] / [bgcolor=#7f7f7f29] right [/bgcolor] / [bgcolor=#7f7f7f29] bottom [/bgcolor] / [bgcolor=#7f7f7f29] left [/bgcolor][/b] — [bgcolor=#7f7f7f29] <length> [/bgcolor] 或 [bgcolor=#7f7f7f29] <percentage> [/bgcolor] 或 [bgcolor=#7f7f7f29] auto [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] z-index [/bgcolor][/b] — [bgcolor=#7f7f7f29] auto [/bgcolor] 或整数`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] box-sizing [/bgcolor][/b] — [bgcolor=#7f7f7f29] content-box [/bgcolor]、[bgcolor=#7f7f7f29] border-box [/bgcolor]`}</Text></li>
        </ul>
        <h4>盒模型</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#7f7f7f29] width [/bgcolor] / [bgcolor=#7f7f7f29] height [/bgcolor][/b] — [bgcolor=#7f7f7f29] <length> [/bgcolor]、[bgcolor=#7f7f7f29] <percentage> [/bgcolor]、[bgcolor=#7f7f7f29] auto [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] min-width [/bgcolor] / [bgcolor=#7f7f7f29] max-width [/bgcolor] / [bgcolor=#7f7f7f29] min-height [/bgcolor] / [bgcolor=#7f7f7f29] max-height [/bgcolor][/b] — 同上`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] padding [/bgcolor][/b] — 简写，1-4 个 [bgcolor=#7f7f7f29] <length> [/bgcolor] 值（如 [bgcolor=#7f7f7f29] 8px [/bgcolor]、[bgcolor=#7f7f7f29] 4px 8px [/bgcolor]、[bgcolor=#7f7f7f29] 4px 8px 12px [/bgcolor]、[bgcolor=#7f7f7f29] 4px 8px 12px 16px [/bgcolor]）`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] padding-top [/bgcolor] / [bgcolor=#7f7f7f29] padding-right [/bgcolor] / [bgcolor=#7f7f7f29] padding-bottom [/bgcolor] / [bgcolor=#7f7f7f29] padding-left [/bgcolor][/b] — 长边属性`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] margin [/bgcolor][/b] — 简写，同 padding`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] margin-top [/bgcolor] / [bgcolor=#7f7f7f29] margin-right [/bgcolor] / [bgcolor=#7f7f7f29] margin-bottom [/bgcolor] / [bgcolor=#7f7f7f29] margin-left [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] border [/bgcolor][/b] — 简写 [bgcolor=#7f7f7f29] 1px solid #ccc [/bgcolor]（width + style + color）`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] border-color [/bgcolor] / [bgcolor=#7f7f7f29] border-width [/bgcolor] / [bgcolor=#7f7f7f29] border-style [/bgcolor] / [bgcolor=#7f7f7f29] border-radius [/bgcolor][/b] — 同 padding 语法的多值简写`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] border-top [/bgcolor] / [bgcolor=#7f7f7f29] border-right [/bgcolor] / [bgcolor=#7f7f7f29] border-bottom [/bgcolor] / [bgcolor=#7f7f7f29] border-left [/bgcolor][/b] — 单边简写`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] border-top-color [/bgcolor] / [bgcolor=#7f7f7f29] border-right-color [/bgcolor] / [bgcolor=#7f7f7f29] border-bottom-color [/bgcolor] / [bgcolor=#7f7f7f29] border-left-color [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] border-top-width [/bgcolor] / [bgcolor=#7f7f7f29] border-right-width [/bgcolor] / [bgcolor=#7f7f7f29] border-bottom-width [/bgcolor] / [bgcolor=#7f7f7f29] border-left-width [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] border-top-style [/bgcolor] / [bgcolor=#7f7f7f29] border-right-style [/bgcolor] / [bgcolor=#7f7f7f29] border-bottom-style [/bgcolor] / [bgcolor=#7f7f7f29] border-left-style [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] border-top-left-radius [/bgcolor] / [bgcolor=#7f7f7f29] border-top-right-radius [/bgcolor] / [bgcolor=#7f7f7f29] border-bottom-left-radius [/bgcolor] / [bgcolor=#7f7f7f29] border-bottom-right-radius [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] border-style [/bgcolor] 可取值[/b] — [bgcolor=#7f7f7f29] none [/bgcolor]、[bgcolor=#7f7f7f29] solid [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] table-layout [/bgcolor][/b] — [bgcolor=#7f7f7f29] auto [/bgcolor]、[bgcolor=#7f7f7f29] fixed [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] border-spacing [/bgcolor][/b] — [bgcolor=#7f7f7f29] <length>{1,2} [/bgcolor]`}</Text></li>
        </ul>
        <h4>背景</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#7f7f7f29] background [/bgcolor][/b] — 简写形式 [bgcolor=#7f7f7f29] bg-image || bg-position || bg-size || repeat-style || color [/bgcolor]（仅单图层）`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] background-color [/bgcolor][/b] — [bgcolor=#7f7f7f29] <color> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] background-image [/bgcolor][/b] — [bgcolor=#7f7f7f29] none [/bgcolor] 或 [bgcolor=#7f7f7f29] <url> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] background-position [/bgcolor][/b] — x y 两个值`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] background-position-x [/bgcolor] / [bgcolor=#7f7f7f29] background-position-y [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] background-repeat [/bgcolor][/b] — [bgcolor=#7f7f7f29] <repeat-style> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] background-size [/bgcolor][/b] — [bgcolor=#7f7f7f29] <bg-size> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] background-clip [/bgcolor][/b] — [bgcolor=#7f7f7f29] border-box [/bgcolor]、[bgcolor=#7f7f7f29] padding-box [/bgcolor]、[bgcolor=#7f7f7f29] content-box [/bgcolor]`}</Text></li>
        </ul>
        <h4>Flexbox 布局</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#7f7f7f29] flex [/bgcolor][/b] — 简写（[bgcolor=#7f7f7f29] flex-grow [/bgcolor] + [bgcolor=#7f7f7f29] flex-shrink [/bgcolor] + [bgcolor=#7f7f7f29] flex-basis [/bgcolor]）`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] flex-shrink [/bgcolor] / [bgcolor=#7f7f7f29] flex-grow [/bgcolor] / [bgcolor=#7f7f7f29] flex-basis [/bgcolor][/b]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] flex-wrap [/bgcolor][/b] — [bgcolor=#7f7f7f29] nowrap [/bgcolor]、[bgcolor=#7f7f7f29] wrap [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] flex-direction [/bgcolor][/b] — [bgcolor=#7f7f7f29] row [/bgcolor]、[bgcolor=#7f7f7f29] column [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] justify-content [/bgcolor][/b] — [bgcolor=#7f7f7f29] flex-start [/bgcolor]、[bgcolor=#7f7f7f29] center [/bgcolor]、[bgcolor=#7f7f7f29] flex-end [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] align-items [/bgcolor][/b] — [bgcolor=#7f7f7f29] flex-start [/bgcolor]、[bgcolor=#7f7f7f29] center [/bgcolor]、[bgcolor=#7f7f7f29] flex-end [/bgcolor]、[bgcolor=#7f7f7f29] stretch [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] align-content [/bgcolor][/b] — 同 justify-content 加 [bgcolor=#7f7f7f29] space-between [/bgcolor]、[bgcolor=#7f7f7f29] space-around [/bgcolor]、[bgcolor=#7f7f7f29] space-evenly [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] gap [/bgcolor][/b] — 简写，同时设 row-gap + column-gap`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] row-gap [/bgcolor] / [bgcolor=#7f7f7f29] column-gap [/bgcolor][/b] — [bgcolor=#7f7f7f29] normal [/bgcolor] 或 [bgcolor=#7f7f7f29] <length-percentage> [/bgcolor]`}</Text></li>
        </ul>
        <h4>排版</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#7f7f7f29] font-face [/bgcolor][/b] — （通过 [bgcolor=#7f7f7f29] @font-face [/bgcolor] 规则加载字体）`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] font-family [/bgcolor][/b] — [bgcolor=#7f7f7f29] <font-family> [/bgcolor]（建议用内置别名如 [bgcolor=#7f7f7f29] monospace [/bgcolor]）`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] font-size [/bgcolor][/b] — [bgcolor=#7f7f7f29] <length> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] font-style [/bgcolor][/b] — [bgcolor=#7f7f7f29] normal [/bgcolor]、[bgcolor=#7f7f7f29] italic [/bgcolor]、[bgcolor=#7f7f7f29] oblique [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] font-weight [/bgcolor][/b] — [bgcolor=#7f7f7f29] normal [/bgcolor]、[bgcolor=#7f7f7f29] bold [/bgcolor]、[bgcolor=#7f7f7f29] <number> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] text-align [/bgcolor][/b] — [bgcolor=#7f7f7f29] left [/bgcolor]、[bgcolor=#7f7f7f29] center [/bgcolor]、[bgcolor=#7f7f7f29] right [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] line-height [/bgcolor][/b] — [bgcolor=#7f7f7f29] normal [/bgcolor] 或 [bgcolor=#7f7f7f29] <length> [/bgcolor] 或 [bgcolor=#7f7f7f29] <number> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] color [/bgcolor][/b] — [bgcolor=#7f7f7f29] <color> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] white-space [/bgcolor][/b] — [bgcolor=#7f7f7f29] normal [/bgcolor]、[bgcolor=#7f7f7f29] nowrap [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] word-break [/bgcolor][/b] — [bgcolor=#7f7f7f29] normal [/bgcolor]、[bgcolor=#7f7f7f29] break-all [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] content [/bgcolor][/b] — [bgcolor=#7f7f7f29] <string> [/bgcolor] 或 [bgcolor=#7f7f7f29] none [/bgcolor]`}</Text></li>
        </ul>
        <h4>其他</h4>
        <ul>
          <li><Text>{`[b][bgcolor=#7f7f7f29] opacity [/bgcolor][/b] — [bgcolor=#7f7f7f29] <number> [/bgcolor] 或 [bgcolor=#7f7f7f29] <percentage> [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] visibility [/bgcolor][/b] — [bgcolor=#7f7f7f29] visible [/bgcolor]、[bgcolor=#7f7f7f29] hidden [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] pointer-events [/bgcolor][/b] — [bgcolor=#7f7f7f29] auto [/bgcolor]、[bgcolor=#7f7f7f29] none [/bgcolor]`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] cursor [/bgcolor][/b] — [bgcolor=#7f7f7f29] auto [/bgcolor]、[bgcolor=#7f7f7f29] default [/bgcolor]、[bgcolor=#7f7f7f29] none [/bgcolor]、[bgcolor=#7f7f7f29] context-menu [/bgcolor]、[bgcolor=#7f7f7f29] help [/bgcolor]、[bgcolor=#7f7f7f29] pointer [/bgcolor]、[bgcolor=#7f7f7f29] progress [/bgcolor]、[bgcolor=#7f7f7f29] wait [/bgcolor]、[bgcolor=#7f7f7f29] cell [/bgcolor]、[bgcolor=#7f7f7f29] crosshair [/bgcolor]、[bgcolor=#7f7f7f29] text [/bgcolor]、[bgcolor=#7f7f7f29] vertical-text [/bgcolor]、[bgcolor=#7f7f7f29] alias [/bgcolor]、[bgcolor=#7f7f7f29] copy [/bgcolor]、[bgcolor=#7f7f7f29] move [/bgcolor]、[bgcolor=#7f7f7f29] no-drop [/bgcolor]、[bgcolor=#7f7f7f29] not-allowed [/bgcolor]、[bgcolor=#7f7f7f29] grab [/bgcolor]、[bgcolor=#7f7f7f29] grabbing [/bgcolor]、[bgcolor=#7f7f7f29] all-scroll [/bgcolor]、[bgcolor=#7f7f7f29] col-resize [/bgcolor]、[bgcolor=#7f7f7f29] row-resize [/bgcolor]、[bgcolor=#7f7f7f29] n/e/s/w-resize [/bgcolor]、[bgcolor=#7f7f7f29] ew/ns-resize [/bgcolor]、[bgcolor=#7f7f7f29] ne/nw/se/sw-resize [/bgcolor]、[bgcolor=#7f7f7f29] nesw/nwse-resize [/bgcolor]、[bgcolor=#7f7f7f29] zoom-in [/bgcolor]、[bgcolor=#7f7f7f29] zoom-out [/bgcolor]；不支持 [bgcolor=#7f7f7f29] url() [/bgcolor] 图像光标`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] box-shadow [/bgcolor][/b] — [bgcolor=#7f7f7f29] none [/bgcolor] 或 [bgcolor=#7f7f7f29] <shadow> [/bgcolor]（格式 [bgcolor=#7f7f7f29] <length>{2,4} && <color>? [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>注意事项</h2>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#7f7f7f29] white-space: pre [/bgcolor] 无效。[/b] LCUI 的 [bgcolor=#7f7f7f29] white-space [/bgcolor] 仅实现 [bgcolor=#7f7f7f29] normal [/bgcolor] 和 [bgcolor=#7f7f7f29] nowrap [/bgcolor]。如需保留源代码前导缩进，把空格换成 U+00A0 (NBSP)。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#7f7f7f29] overflow [/bgcolor] 不裁切内容。[/b] 要裁切或滚动子内容，改用 [bgcolor=#7f7f7f29] scrollarea [/bgcolor] 内置部件。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#7f7f7f29] background [/bgcolor] 简写不支持多图层。[/b] 写 [bgcolor=#7f7f7f29] background: url(a.png) no-repeat, url(b.png) center; [/bgcolor] 这种多背景语法不会生效。请拆成 [bgcolor=#7f7f7f29] background-image [/bgcolor] / [bgcolor=#7f7f7f29] background-position [/bgcolor] 等分属性逐个设置。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#7f7f7f29] cursor [/bgcolor] 按平台近似映射，[bgcolor=#7f7f7f29] url() [/bgcolor] 图像光标不支持。[/b] 悬停部件的 [bgcolor=#7f7f7f29] cursor [/bgcolor] 值当帧生效（含 [bgcolor=#7f7f7f29] :hover [/bgcolor] 等状态变化）。Windows/X11 映射到系统光标，无精确对应的取近似形状；[bgcolor=#7f7f7f29] none [/bgcolor] 隐藏指针；framebuffer 软件指针不受影响。内置交互部件已带合理默认值（button 等 [bgcolor=#7f7f7f29] pointer [/bgcolor]、textinput [bgcolor=#7f7f7f29] text [/bgcolor]、滚动条滑块 [bgcolor=#7f7f7f29] grab [/bgcolor]）。`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
