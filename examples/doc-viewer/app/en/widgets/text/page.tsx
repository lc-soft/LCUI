import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import TextBasicDemo from "../../../examples/text-basic/index";

export default function TextDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Text</h1>
      <p className="doc-page-desc">{`A widget for displaying text. Supports single-line and multiline modes, inline style tags, and CSS styling.`}</p>
      <TextBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b] — static text display (titles, labels, descriptions).`}</Text></li>
          <li><Text>{`[b]Not suitable[/b] — editable text input (use [bgcolor=#eee] textinput [/bgcolor]).`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/text/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Text[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/text/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Text[/color]>[color=#8250df]Hello[/color]</[color=#8250df]Text[/color]>`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Composition</h2>
        <p>{`Use the following composition to build a [bgcolor=#eee] Text [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/text/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`Text`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API</h2>
        <h3>`ui_text_set_content`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/text/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]ui_text_set_content[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w, [color=#953800]const[/color] [color=#953800]char[/color] *utf8_text)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Sets the text content from a UTF-8 string. Supports inline style tags.`}</p>
        <h3>`ui_text_set_content_w`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/text/4">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]ui_text_set_content_w[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w, [color=#953800]const[/color] [color=#953800]wchar_t[/color] *text)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Sets the text content from a wide character string.`}</p>
        <h3>`ui_text_get_content_w`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/text/5">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]size_t[/color] [color=#8250df]ui_text_get_content_w[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w, [color=#953800]wchar_t[/color] *buf, [color=#953800]size_t[/color] size)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Reads the current text content into [bgcolor=#eee] buf [/bgcolor] (up to [bgcolor=#eee] size [/bgcolor] wide characters). Returns the number of characters written.`}</p>
        <h3>`ui_text_set_multiline`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/text/6">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]ui_text_set_multiline[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w, [color=#953800]bool[/color] enable)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Enables or disables multiline mode. When enabled, [bgcolor=#eee] \\n [/bgcolor] characters create new lines.`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>XML tag</h2>
        <Widget className="code-block" data-language="xml" data-source="en/widgets/text/7">
          <Widget className="code-block-header">
            <Text className="code-block-title">XML</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]text[/color]>[/color]Text content here[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          </Widget>
        </Widget>
        <p>{`Attributes:`}</p>
        <Widget className="mdx-table">
          <Widget className="mdx-table-row mdx-table-head">
            <Widget className="mdx-table-cell mdx-table-head-cell" style={{ width: "112px" }}>
              <Text>{`Attribute`}</Text>
            </Widget>
            <Widget className="mdx-table-cell flex-grow mdx-table-head-cell" style={{ minWidth: "48px" }}>
              <Text>{`Description`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "112px" }}>
              <Text>{`[bgcolor=#eee] multiline [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell flex-grow" style={{ minWidth: "48px" }}>
              <Text>{`Set to [bgcolor=#eee] "true" [/bgcolor] to enable multiline mode`}</Text>
            </Widget>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Inline style tags</h2>
        <p>{`The text widget supports a BBCode-like inline styling syntax:`}</p>
        <Widget className="mdx-table">
          <Widget className="mdx-table-row mdx-table-head">
            <Widget className="mdx-table-cell mdx-table-head-cell" style={{ width: "112px" }}>
              <Text>{`Tag`}</Text>
            </Widget>
            <Widget className="mdx-table-cell mdx-table-head-cell" style={{ width: "248px" }}>
              <Text>{`Example`}</Text>
            </Widget>
            <Widget className="mdx-table-cell mdx-table-head-cell" style={{ width: "144px" }}>
              <Text>{`Effect`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "112px" }}>
              <Text>{`[bgcolor=#eee] [color=X] [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "248px" }}>
              <Text>{`[bgcolor=#eee] [color=#ff0000]Red[/color] [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "144px" }}>
              <Text>{`Text color`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "112px" }}>
              <Text>{`[bgcolor=#eee] [b] [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "248px" }}>
              <Text>{`[bgcolor=#eee] [b]Bold[/b] [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "144px" }}>
              <Text>{`Bold font`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "112px" }}>
              <Text>{`[bgcolor=#eee] [i] [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "248px" }}>
              <Text>{`[bgcolor=#eee] [i]Italic[/i] [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "144px" }}>
              <Text>{`Italic font`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "112px" }}>
              <Text>{`[bgcolor=#eee] [size=X] [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "248px" }}>
              <Text>{`[bgcolor=#eee] [size=20]Large[/size] [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "144px" }}>
              <Text>{`Font size in px`}</Text>
            </Widget>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
