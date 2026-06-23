import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import TextInputBasicDemo from "../../../examples/text-input-basic/index";
import FieldTable from "../../../components/field-table";
import { textInputFields } from "../../../widget-fields/text-input";

export default function TextInputDocPage() {
  return (
    <Widget className="doc-page">
      <h1>TextInput</h1>
      <p className="doc-page-desc">{`可编辑的文本输入部件。`}</p>
      <TextInputBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b] — 用户输入文本（表单字段、搜索框）。`}</Text></li>
          <li><Text>{`[b]不适用[/b] — 仅需要展示文本（用 [bgcolor=#eee] text [/bgcolor]）。`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>结构</h2>
        <p>{`导入部件：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/text-input/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]TextInput[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]TextInput[/color] />[/color]`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <FieldTable fields={textInputFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
