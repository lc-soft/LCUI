import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import RadioGroupBasicDemo from "../../../examples/radio-group-basic/index";
import RadioGroupDisabledDemo from "../../../examples/radio-group-disabled/index";
import FieldTable from "../../../components/field-table";
import { radioGroupFields } from "../../../widget-fields/radio-group";
import { radioGroupItemFields } from "../../../widget-fields/radio-group-item";

export default function RadioGroupDocPage() {
  return (
    <Widget className="doc-page">
      <h1>RadioGroup</h1>
      <p className="doc-page-desc">{`A set of mutually exclusive radio buttons—only one item in the group can be selected at a time.`}</p>
      <RadioGroupBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: pick exactly one option from a mutually exclusive set (view density, sort order, single-choice enumerations).`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: simple cases with a fixed 2–7 options.`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: scenarios that allow multiple concurrent selections (use [bgcolor=#eee] checkbox [/bgcolor]).`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: large option lists that need search or filtering (no [bgcolor=#eee] select [/bgcolor] provided yet).`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/radio-group/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]RadioGroup[/color], [color=#8250df]RadioGroupItem[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/radio-group/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]RadioGroup[/color] value=[color=#0a3069]"comfortable"[/color]>`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"default"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"r1"[/color] />[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"comfortable"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"r2"[/color] />[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"compact"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"r3"[/color] />[/color]`}</Text>
            <Text className="code-line">{`</[color=#8250df]RadioGroup[/color]>`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Composition</h2>
        <p>{`Use the following composition to build a [bgcolor=#eee] RadioGroup [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/radio-group/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`RadioGroup`}</Text>
            <Text className="code-line">{`├── RadioGroupItem`}</Text>
            <Text className="code-line">{`└── RadioGroupItem`}</Text>
          </Widget>
        </Widget>
        <p>{`In practice, each item is paired with a [bgcolor=#eee] Label [/bgcolor] placed in the same flex row, with the Label's [bgcolor=#eee] for [/bgcolor] pointing at the RadioGroupItem's [bgcolor=#eee] id [/bgcolor]—clicking the Label forwards the click event to the matching item.`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>Examples</h2>
        <h3>Disabled</h3>
        <RadioGroupDisabledDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>RadioGroup</h3>
        <p>{`The container that manages mutually exclusive selection across its items.`}</p>
        <FieldTable fields={radioGroupFields} locale="en" />
        <h3>RadioGroupItem</h3>
        <p>{`A single selectable option. At most one RadioGroupItem within the same RadioGroup is checked at any time.`}</p>
        <FieldTable fields={radioGroupItemFields} locale="en" />
      </Widget>
    </Widget>
  );
}
