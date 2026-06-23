import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import TextBasicDemo from "../../../examples/text-basic/index";
import FieldTable from "../../../components/field-table";
import { textFields } from "../../../widget-fields/text";

export default function TextDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Text</h1>
      <p className="doc-page-desc">{`用于显示文本的部件。`}</p>
      <TextBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b] — 静态文本显示（标题、标签、说明文字）。`}</Text></li>
          <li><Text>{`[b]不适用[/b] — 需要用户编辑文本（用 [bgcolor=#eee] textinput [/bgcolor]）。`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>结构</h2>
        <p>{`导入部件：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/text/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Text[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]Text[/color] />[/color]`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <FieldTable fields={textFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
