import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import SelectBasicDemo from "../../../examples/select-basic/index";
import SelectGroupsDemo from "../../../examples/select-groups/index";
import SelectAlignItemDemo from "../../../examples/select-align-item/index";
import SelectScrollableDemo from "../../../examples/select-scrollable/index";
import SelectDisabledDemo from "../../../examples/select-disabled/index";
import FieldTable from "../../../components/field-table";
import { selectFields } from "../../../widget-fields/select";
import { selectTriggerFields } from "../../../widget-fields/select-trigger";
import { selectContentFields } from "../../../widget-fields/select-content";
import { selectGroupFields } from "../../../widget-fields/select-group";
import { selectLabelFields } from "../../../widget-fields/select-label";
import { selectSeparatorFields } from "../../../widget-fields/select-separator";
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
          <li><Text>{`[b]Suitable[/b]: Options that can be grouped by category`}</Text></li>
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
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Select[/color], [color=#8250df]SelectTrigger[/color], [color=#8250df]SelectValue[/color], [color=#8250df]SelectContent[/color], [color=#8250df]SelectGroup[/color], [color=#8250df]SelectLabel[/color], [color=#8250df]SelectItem[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
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
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]SelectValue[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"Select a theme"[/color] />[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]</[color=#0550ae]SelectTrigger[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]SelectLabel[/color]>[/color]Theme[color=#116329]</[color=#0550ae]SelectLabel[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"light"[/color]>[/color]Light[color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"dark"[/color]>[/color]Dark[color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"system"[/color]>[/color]System[color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]</[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
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
            <Text className="code-line">{`    ├── SelectGroup`}</Text>
            <Text className="code-line">{`    │   ├── SelectLabel`}</Text>
            <Text className="code-line">{`    │   ├── SelectItem`}</Text>
            <Text className="code-line">{`    │   └── SelectItem`}</Text>
            <Text className="code-line">{`    ├── SelectSeparator`}</Text>
            <Text className="code-line">{`    └── SelectGroup`}</Text>
            <Text className="code-line">{`        ├── SelectLabel`}</Text>
            <Text className="code-line">{`        ├── SelectItem`}</Text>
            <Text className="code-line">{`        └── SelectItem`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Groups</h2>
        <p>{`Wrap a set of related options in [bgcolor=#eee] SelectGroup [/bgcolor] and use [bgcolor=#eee] SelectLabel [/bgcolor] as the group heading; when the panel holds several groups, use [bgcolor=#eee] SelectSeparator [/bgcolor] to draw a divider between them.`}</p>
        <SelectGroupsDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Align Item With Trigger</h2>
        <p>{`By default, opening a [bgcolor=#eee] Select [/bgcolor] makes the popup [b]overlap the trigger[/b] so the [b]selected item[/b] sits at the same height as it; when nothing is selected the first item is aligned instead. If the trigger is near the bottom of the window and there is not enough room to align, the popup flips above the trigger and shows a scrollbar. Set [bgcolor=#eee] align-item-with-trigger [/bgcolor] to [bgcolor=#eee] false [/bgcolor] to disable aligning and expand the popup against the trigger edge.`}</p>
        <SelectAlignItemDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Scrollable</h2>
        <p>{`When the options exceed the available height the popup is capped to that height and shows a scrollbar; when the trigger is near the bottom of the window the popup flips above it and uses the space available on that side.`}</p>
        <SelectScrollableDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Disabled</h2>
        <p>{`Setting [bgcolor=#eee] disabled [/bgcolor] on [bgcolor=#eee] Select [/bgcolor] lowers the opacity of the trigger and stops the content panel from opening; you can also disable an individual [bgcolor=#eee] SelectItem [/bgcolor], which then cannot be selected.`}</p>
        <SelectDisabledDemo />
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
        <h3>SelectGroup</h3>
        <p>{`A container that groups related options, optionally headed by a [bgcolor=#eee] SelectLabel [/bgcolor].`}</p>
        <FieldTable fields={selectGroupFields} locale="en" />
        <h3>SelectLabel</h3>
        <p>{`The small heading text for an option group.`}</p>
        <FieldTable fields={selectLabelFields} locale="en" />
        <h3>SelectSeparator</h3>
        <p>{`A thin divider between option groups.`}</p>
        <FieldTable fields={selectSeparatorFields} locale="en" />
        <h3>SelectItem</h3>
        <p>{`An option representing one selectable value.`}</p>
        <FieldTable fields={selectItemFields} locale="en" />
      </Widget>
    </Widget>
  );
}
