import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import AnchorBasicDemo from "../../../examples/anchor-basic/index";

export default function AnchorDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Anchor</h1>
      <p className="doc-page-desc">{`A navigation widget similar to the HTML [bgcolor=#eee] <a> [/bgcolor] element. It can open external URLs in the system browser or load an XML view into a target container widget.`}</p>
      <AnchorBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b] — placing external links in a page (opens in the system browser).`}</Text></li>
          <li><Text>{`[b]Not suitable[/b] — performing an action (use [bgcolor=#eee] button [/bgcolor]).`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/anchor/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<a href=[color=#0a3069]"https://example.com"[/color]>[color=#8250df]Link[/color] text</a>`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Composition</h2>
        <p>{`Use the following composition to build an [bgcolor=#eee] Anchor [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/anchor/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`Anchor`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API</h2>
        <h3>`ui_anchor_open`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/anchor/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]ui_anchor_open[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Programmatically triggers the anchor's action (same as a click). Reads the [bgcolor=#eee] href [/bgcolor] attribute from the widget:`}</p>
        <ul>
          <li><Text>{`If [bgcolor=#eee] href [/bgcolor] starts with [bgcolor=#eee] http:// [/bgcolor] or [bgcolor=#eee] https:// [/bgcolor] — opens in the system browser.`}</Text></li>
          <li><Text>{`If [bgcolor=#eee] href [/bgcolor] starts with [bgcolor=#eee] file:/// [/bgcolor] — opens the local file in the system browser.`}</Text></li>
          <li><Text>{`Otherwise — asynchronously loads an XML file at the given path and injects its content into the widget identified by the [bgcolor=#eee] target [/bgcolor] attribute.`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>XML tag</h2>
        <Widget className="code-block" data-language="xml" data-source="en/widgets/anchor/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">XML</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]a[/color] [color=#0550ae]href[/color]=[color=#0a3069]"..."[/color] [color=#0550ae]target[/color]=[color=#0a3069]"..."[/color] [color=#0550ae]key[/color]=[color=#0a3069]"..."[/color]>[/color]Link text[color=#116329]</[color=#0550ae]a[/color]>[/color]`}</Text>
          </Widget>
        </Widget>
        <p>{`Attributes:`}</p>
        <Widget className="mdx-table">
          <Widget className="mdx-table-row mdx-table-head">
            <Widget className="mdx-table-cell mdx-table-head-cell" style={{ width: "96px" }}>
              <Text>{`Attribute`}</Text>
            </Widget>
            <Widget className="mdx-table-cell mdx-table-head-cell" style={{ width: "152px" }}>
              <Text>{`Required`}</Text>
            </Widget>
            <Widget className="mdx-table-cell flex-grow mdx-table-head-cell" style={{ minWidth: "48px" }}>
              <Text>{`Description`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "96px" }}>
              <Text>{`[bgcolor=#eee] href [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "152px" }}>
              <Text>{`Yes`}</Text>
            </Widget>
            <Widget className="mdx-table-cell flex-grow" style={{ minWidth: "48px" }}>
              <Text>{`URL to open, or relative XML file path to load`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "96px" }}>
              <Text>{`[bgcolor=#eee] target [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "152px" }}>
              <Text>{`For view loading`}</Text>
            </Widget>
            <Widget className="mdx-table-cell flex-grow" style={{ minWidth: "48px" }}>
              <Text>{`ID of the container widget where the loaded view is injected`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "96px" }}>
              <Text>{`[bgcolor=#eee] key [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "152px" }}>
              <Text>{`No`}</Text>
            </Widget>
            <Widget className="mdx-table-cell flex-grow" style={{ minWidth: "48px" }}>
              <Text>{`Arbitrary string passed as [bgcolor=#eee] event.data [/bgcolor] to the [bgcolor=#eee] loaded.anchor [/bgcolor] event`}</Text>
            </Widget>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Events</h2>
        <h3>`loaded.anchor`</h3>
        <p>{`Fired on [bgcolor=#eee] ui_root() [/bgcolor] after a view has been loaded and injected:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/widgets/anchor/4">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]on_view_loaded[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w, [color=#953800]ui_event_t[/color] *e, [color=#953800]void[/color] *arg)[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        [color=#6e7781]/* e->data contains the value of the anchor&#x27;s "key" attribute */[/color]`}</Text>
            <Text className="code-line">{`        [color=#953800]printf[/color]([color=#0a3069]"Loaded view key: %s\\n"[/color], ([color=#953800]char[/color] *)e->data);`}</Text>
            <Text className="code-line">{`}`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`ui_widget_on(ui_root(), [color=#0a3069]"loaded.anchor"[/color], on_view_loaded, [color=#0550ae]NULL[/color]);`}</Text>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
