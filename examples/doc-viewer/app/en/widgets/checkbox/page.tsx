import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import CheckboxBasicDemo from "../../../examples/checkbox-basic/index";
import CheckboxDisabledDemo from "../../../examples/checkbox-disabled/index";
import FieldTable from "../../../components/field-table";
import { checkboxFields } from "../../../widget-fields/checkbox";

export default function CheckboxDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Checkbox</h1>
      <p className="doc-page-desc">{`A control that allows the user to toggle between checked and not checked.`}</p>
      <CheckboxBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: a single binary choice (accepting terms, toggling a subscription).`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: checkable options inside a form.`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: mutually exclusive single-choice scenarios (use [bgcolor=#eee] radio-group [/bgcolor]).`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/checkbox/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Checkbox[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/checkbox/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Checkbox[/color] />`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Caveats</h2>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]indeterminate is a state, not a default value.[/b] The half-selected state ([bgcolor=#eee] indeterminate="true" [/bgcolor]) expresses the visual notion that some sub-items are selected (for example, a parent checkbox shown as indeterminate when only some of its children are checked). Clicking a checkbox in the indeterminate state transitions it to the checked state.`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Examples</h2>
        <h3>Disabled</h3>
        <CheckboxDisabledDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <FieldTable fields={checkboxFields} locale="en" />
      </Widget>
    </Widget>
  );
}
