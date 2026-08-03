import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import SelectBasicDemo from "../../../examples/select-basic/index";
import FieldTable from "../../../components/field-table";
import { selectFields } from "../../../widget-fields/select";
import { selectTriggerFields } from "../../../widget-fields/select-trigger";
import { selectContentFields } from "../../../widget-fields/select-content";
import { selectItemFields } from "../../../widget-fields/select-item";

export default function SelectDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Select</h1>
      <p className="doc-page-desc">{`Select provides an expandable list of options for choosing one value from multiple candidates.`}</p>
      <SelectBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: Choosing one value from multiple options`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: Forms with many options where page space should be preserved`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: Choosing multiple values at once (use [bgcolor=#eee] checkbox [/bgcolor])`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/select/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Select[/color], [color=#8250df]SelectTrigger[/color], [color=#8250df]SelectValue[/color], [color=#8250df]SelectContent[/color], [color=#8250df]SelectItem[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/select/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Select[/color]>`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]SelectTrigger[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]SelectValue[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"Choose a theme"[/color] />[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]</[color=#0550ae]SelectTrigger[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"light"[/color]>[/color]Light[color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"dark"[/color]>[/color]Dark[color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"system"[/color]>[/color]System[color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]</[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
            <Text className="code-line">{`</[color=#8250df]Select[/color]>`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Composition</h2>
        <p>{`Use the following composition to build a [bgcolor=#eee] Select [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/select/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`Select`}</Text>
            <Text className="code-line">{`├── SelectTrigger`}</Text>
            <Text className="code-line">{`│   └── SelectValue`}</Text>
            <Text className="code-line">{`└── SelectContent`}</Text>
            <Text className="code-line">{`    └── SelectItem`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>Select</h3>
        <p>{`The Select state container that manages the current value, placeholder, and content panel.`}</p>
        <FieldTable fields={selectFields} locale="en" />
        <h3>SelectTrigger</h3>
        <p>{`The trigger that opens or closes the content panel.`}</p>
        <FieldTable fields={selectTriggerFields} locale="en" />
        <h3>SelectContent</h3>
        <p>{`The option panel mounted at the root level through Portal.`}</p>
        <FieldTable fields={selectContentFields} locale="en" />
        <h3>SelectItem</h3>
        <p>{`An option representing one selectable value.`}</p>
        <FieldTable fields={selectItemFields} locale="en" />
      </Widget>
    </Widget>
  );
}
