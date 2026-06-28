import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import ProgressBasicDemo from "../../../examples/progress-basic/index";
import FieldTable from "../../../components/field-table";
import { progressFields } from "../../../widget-fields/progress";

export default function ProgressDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Progress</h1>
      <p className="doc-page-desc">{`A linear progress bar widget used to indicate the completion of a task such as file uploads or loading states.`}</p>
      <ProgressBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: file upload/download, installation, or any long-running task that needs progress feedback`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: form submission or async operations that need a wait indicator`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: scenarios that require precise numeric values or indeterminate progress (use [bgcolor=#eee] text [/bgcolor] to render manually)`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/progress/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Progress[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/progress/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Progress[/color] value={[color=#0550ae]20[/color]} />`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <FieldTable fields={progressFields} locale="en" />
      </Widget>
    </Widget>
  );
}
