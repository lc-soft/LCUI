import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import TextInputBasicDemo from "../../../examples/text-input-basic/index";

export default function TextInputDocPage() {
  return (
    <Widget className="doc-page">
      <h1>TextInput</h1>
      <p className="doc-page-desc">{`An editable text input widget. Supports single-line, multiline, password, placeholder, and readonly modes.`}</p>
      <TextInputBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b] — user text input (form fields, search boxes).`}</Text></li>
          <li><Text>{`[b]Not suitable[/b] — text display only (use [bgcolor=#eee] text [/bgcolor]).`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/text-input/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]TextInput[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/text-input/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]TextInput[/color] placeholder=[color=#0a3069]"Enter text"[/color] />`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API</h2>
        <h3>`ui_textinput_set_text`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/text-input/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]ui_textinput_set_text[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *widget, [color=#953800]const[/color] [color=#953800]char[/color] *utf8_str)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Sets the input value from a UTF-8 string.`}</p>
        <h3>`ui_textinput_set_text_w`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/text-input/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]ui_textinput_set_text_w[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *widget, [color=#953800]const[/color] [color=#953800]wchar_t[/color] *wstr)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Sets the input value from a wide character string.`}</p>
        <h3>`ui_textinput_get_text_w`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/text-input/4">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]size_t[/color] [color=#8250df]ui_textinput_get_text_w[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w, [color=#953800]size_t[/color] start,[/color]`}</Text>
            <Text className="code-line">{`[color=#0550ae]                               [color=#953800]size_t[/color] max_len, [color=#953800]wchar_t[/color] *buf)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Reads the current input value. Returns the number of characters written.`}</p>
        <h3>`ui_textinput_get_text_length`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/text-input/5">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]size_t[/color] [color=#8250df]ui_textinput_get_text_length[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Returns the current text length in characters.`}</p>
        <h3>`ui_textinput_clear_text`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/text-input/6">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]ui_textinput_clear_text[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *widget)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Clears all text from the input.`}</p>
        <h3>`ui_textinput_set_placeholder`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/text-input/7">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]ui_textinput_set_placeholder[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w, [color=#953800]const[/color] [color=#953800]char[/color] *str)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Sets the placeholder text shown when the input is empty.`}</p>
        <h3>`ui_textinput_set_placeholder_w`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/text-input/8">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]ui_textinput_set_placeholder_w[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w, [color=#953800]const[/color] [color=#953800]wchar_t[/color] *wstr)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Sets the placeholder text from a wide character string.`}</p>
        <h3>`ui_textinput_set_password_char`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/text-input/9">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]ui_textinput_set_password_char[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w, [color=#953800]wchar_t[/color] ch)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Masks the input with the given character (e.g. [bgcolor=#eee] L'*' [/bgcolor] for password fields).`}</p>
        <h3>`ui_textinput_enable_multiline`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/text-input/10">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]ui_textinput_enable_multiline[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *widget, [color=#953800]bool[/color] enable)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Enables or disables multiline input mode.`}</p>
        <h3>`ui_textinput_enable_style_tag`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/text-input/11">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]ui_textinput_enable_style_tag[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *widget, [color=#953800]bool[/color] enable)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Enables or disables inline style tag parsing in the input.`}</p>
        <h3>`ui_textinput_append_text_w`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/text-input/12">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]ui_textinput_append_text_w[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *widget, [color=#953800]const[/color] [color=#953800]wchar_t[/color] *wstr)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Appends text at the end of the current content.`}</p>
        <h3>`ui_textinput_insert_text_w`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/text-input/13">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]ui_textinput_insert_text_w[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *widget, [color=#953800]const[/color] [color=#953800]wchar_t[/color] *wstr)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Inserts text at the current caret position.`}</p>
        <h3>`ui_textinput_set_caret_blink`</h3>
        <Widget className="code-block" data-language="c" data-source="en/widgets/text-input/14">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]ui_textinput_set_caret_blink[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w, [color=#953800]bool[/color] visible, [color=#953800]int[/color] time)[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Controls the caret blink animation. [bgcolor=#eee] time [/bgcolor] is the blink interval in milliseconds.`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>XML tag</h2>
        <Widget className="code-block" data-language="xml" data-source="en/widgets/text-input/15">
          <Widget className="code-block-header">
            <Text className="code-block-title">XML</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]textedit[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"..."[/color] [color=#0550ae]style[/color]=[color=#0a3069]"width: 200px;"[/color] />[/color]`}</Text>
          </Widget>
        </Widget>
        <p>{`Attributes:`}</p>
        <Widget className="mdx-table">
          <Widget className="mdx-table-row mdx-table-head">
            <Widget className="mdx-table-cell mdx-table-head-cell" style={{ width: "128px" }}>
              <Text>{`Attribute`}</Text>
            </Widget>
            <Widget className="mdx-table-cell flex-grow mdx-table-head-cell" style={{ minWidth: "48px" }}>
              <Text>{`Description`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "128px" }}>
              <Text>{`[bgcolor=#eee] placeholder [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell flex-grow" style={{ minWidth: "48px" }}>
              <Text>{`Placeholder text`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "128px" }}>
              <Text>{`[bgcolor=#eee] multiline [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell flex-grow" style={{ minWidth: "48px" }}>
              <Text>{`[bgcolor=#eee] "true" [/bgcolor] for multiline mode`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "128px" }}>
              <Text>{`[bgcolor=#eee] readonly [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell flex-grow" style={{ minWidth: "48px" }}>
              <Text>{`[bgcolor=#eee] "true" [/bgcolor] to make the input read-only`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "128px" }}>
              <Text>{`[bgcolor=#eee] type [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell flex-grow" style={{ minWidth: "48px" }}>
              <Text>{`[bgcolor=#eee] "password" [/bgcolor] to mask input`}</Text>
            </Widget>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
