import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function CompositionDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Composition</h1>
      <p className="doc-page-desc">{`Building complex UIs means composing many widgets together. LCUI provides three ways to do this.`}</p>
      <Widget className="doc-section">
        <h2>Building widget hierarchies in C</h2>
        <p>{`Use [bgcolor=#eee] ui_widget_append() [/bgcolor] to nest widgets:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/composition/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *root = ui_root();`}</Text>
            <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *container = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
            <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *title = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
            <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *btn = ui_create_widget([color=#0a3069]"button"[/color]);`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`ui_text_set_content(title, [color=#0a3069]"My App"[/color]);`}</Text>
            <Text className="code-line">{`ui_button_set_text(btn, [color=#0a3069]"Click me"[/color]);`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`ui_widget_append(container, title);`}</Text>
            <Text className="code-line">{`ui_widget_append(container, btn);`}</Text>
            <Text className="code-line">{`ui_widget_append(root, container);`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Composing with XML</h2>
        <p>{`Define your entire widget tree in XML:`}</p>
        <Widget className="code-block" data-language="xml" data-source="en/handbook/composition/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">XML</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color]?>[/color]`}</Text>
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]widget[/color] [color=#0550ae]style[/color]=[color=#0a3069]"display: flex; flex-direction: column; padding: 16px;"[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]style[/color]=[color=#0a3069]"font-size: 24px; font-weight: bold;"[/color]>[/color]My App[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]id[/color]=[color=#0a3069]"action-btn"[/color]>[/color]Click me[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]</[color=#0550ae]widget[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
            <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          </Widget>
        </Widget>
        <p>{`Load it at runtime:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/composition/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`lcui_init();`}</Text>
            <Text className="code-line">{`ui_load_xml_file([color=#0a3069]"ui.xml"[/color]);`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *btn = ui_get_widget([color=#0a3069]"action-btn"[/color]);`}</Text>
            <Text className="code-line">{`[color=#cf222e]if[/color] (btn) {`}</Text>
            <Text className="code-line">{`        ui_widget_on(btn, [color=#0a3069]"click"[/color], on_click, [color=#0550ae]NULL[/color]);`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Composing with TSX</h2>
        <p>{`With [bgcolor=#eee] @lcui/cli [/bgcolor] you can write reusable components in TSX:`}</p>
        <Widget className="code-block" data-language="tsx" data-source="en/handbook/composition/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#6e7781]// components/Card.tsx[/color]`}</Text>
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Button[/color], [color=#8250df]Text[/color], [color=#cf222e]type[/color] [color=#8250df]ReactNode[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#cf222e]interface[/color] [color=#8250df]CardProps[/color] {`}</Text>
            <Text className="code-line">{`  [color=#0550ae]title[/color]: [color=#953800]string[/color];`}</Text>
            <Text className="code-line">{`  [color=#0550ae]children[/color]?: [color=#8250df]ReactNode[/color];`}</Text>
            <Text className="code-line">{`}`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#cf222e]function[/color] [color=#8250df]Card[/color]([color=#0550ae]{ title, children }: [color=#8250df]CardProps[/color][/color]) {`}</Text>
            <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]style[/color]=[color=#0a3069]{{[/color] [color=#0550ae]border:[/color] "[color=#0550ae]1px[/color] [color=#0550ae]solid[/color] #[color=#0550ae]ccc[/color]", [color=#0550ae]padding:[/color] [color=#0550ae]16[/color], [color=#0550ae]borderRadius:[/color] [color=#0550ae]4[/color] }}>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]style[/color]=[color=#0a3069]{{[/color] [color=#0550ae]fontWeight:[/color] "[color=#0550ae]bold[/color]", [color=#0550ae]fontSize:[/color] [color=#0550ae]18[/color] }}>[/color]{title}[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
            <Text className="code-line">{`      {children}`}</Text>
            <Text className="code-line">{`    [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
            <Text className="code-line">{`  );`}</Text>
            <Text className="code-line">{`}`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#6e7781]// App.tsx[/color]`}</Text>
            <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
            <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Card[/color] [color=#0550ae]title[/color]=[color=#0a3069]"Welcome"[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Button[/color]>[/color]Get started[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Card[/color]>[/color]`}</Text>
            <Text className="code-line">{`  );`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Event handling</h2>
        <p>{`Widgets communicate through events. Bind a handler with [bgcolor=#eee] ui_widget_on() [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/composition/4">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]on_button_click[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w, [color=#953800]ui_event_t[/color] *e, [color=#953800]void[/color] *arg)[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *label = arg;`}</Text>
            <Text className="code-line">{`        ui_text_set_content(label, [color=#0a3069]"Button was clicked!"[/color]);`}</Text>
            <Text className="code-line">{`}`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *btn = ui_create_widget([color=#0a3069]"button"[/color]);`}</Text>
            <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *label = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`ui_button_set_text(btn, [color=#0a3069]"Click me"[/color]);`}</Text>
            <Text className="code-line">{`ui_widget_on(btn, [color=#0a3069]"click"[/color], on_button_click, label);`}</Text>
          </Widget>
        </Widget>
        <p>{`Common events:`}</p>
        <Widget className="mdx-table">
          <Widget className="mdx-table-row mdx-table-head">
            <Widget className="mdx-table-cell mdx-table-head-cell" style={{ width: "120px" }}>
              <Text>{`Event`}</Text>
            </Widget>
            <Widget className="mdx-table-cell mdx-table-head-cell" style={{ width: "216px" }}>
              <Text>{`Trigger`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "120px" }}>
              <Text>{`[bgcolor=#eee] click [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "216px" }}>
              <Text>{`Mouse click`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "120px" }}>
              <Text>{`[bgcolor=#eee] mousedown [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "216px" }}>
              <Text>{`Mouse button pressed`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "120px" }}>
              <Text>{`[bgcolor=#eee] mouseup [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "216px" }}>
              <Text>{`Mouse button released`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "120px" }}>
              <Text>{`[bgcolor=#eee] mousemove [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "216px" }}>
              <Text>{`Mouse moved over widget`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "120px" }}>
              <Text>{`[bgcolor=#eee] mouseenter [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "216px" }}>
              <Text>{`Mouse enters widget area`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "120px" }}>
              <Text>{`[bgcolor=#eee] mouseleave [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "216px" }}>
              <Text>{`Mouse leaves widget area`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "120px" }}>
              <Text>{`[bgcolor=#eee] keydown [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "216px" }}>
              <Text>{`Key pressed`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "120px" }}>
              <Text>{`[bgcolor=#eee] keyup [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "216px" }}>
              <Text>{`Key released`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "120px" }}>
              <Text>{`[bgcolor=#eee] focus [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "216px" }}>
              <Text>{`Widget gains focus`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "120px" }}>
              <Text>{`[bgcolor=#eee] blur [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "216px" }}>
              <Text>{`Widget loses focus`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "120px" }}>
              <Text>{`[bgcolor=#eee] change [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "216px" }}>
              <Text>{`Value changed (inputs)`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "120px" }}>
              <Text>{`[bgcolor=#eee] destroy [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "216px" }}>
              <Text>{`Widget destroyed`}</Text>
            </Widget>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
