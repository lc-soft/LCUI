import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import RadioGroupBasicDemo from "../../../../examples/radio-group-basic/index";
import RadioGroupDescriptionDemo from "../../../../examples/radio-group-description/index";
import RadioGroupChoiceCardDemo from "../../../../examples/radio-group-choice-card/index";
import RadioGroupFieldsetDemo from "../../../../examples/radio-group-fieldset/index";
import RadioGroupDisabledDemo from "../../../../examples/radio-group-disabled/index";
import FieldTable from "../../../../components/field-table";
import { radioGroupFields } from "../../../../widget-fields/radio-group";
import { radioGroupItemFields } from "../../../../widget-fields/radio-group-item";

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
          <li><Text>{`[b]Not suitable[/b]: scenarios that allow multiple concurrent selections (use [bgcolor=#7f7f7f29] checkbox [/bgcolor]).`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: large option lists that need search or filtering (no [bgcolor=#7f7f7f29] select [/bgcolor] provided yet).`}</Text></li>
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
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]RadioGroup[/color], [color=#8250df]RadioGroupItem[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] { [color=#d2a8ff]RadioGroup[/color], [color=#d2a8ff]RadioGroupItem[/color] } [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/radio-group/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "124px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#8250df]RadioGroup[/color] value=[color=#0a3069]"comfortable"[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"default"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"r1"[/color] />[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"comfortable"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"r2"[/color] />[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"compact"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"r3"[/color] />[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`</[color=#8250df]RadioGroup[/color]>`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]RadioGroup[/color] value=[color=#a5d6ff]"comfortable"[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]RadioGroupItem[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"default"[/color] [color=#79c0ff]id[/color]=[color=#a5d6ff]"r1"[/color] />[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]RadioGroupItem[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"comfortable"[/color] [color=#79c0ff]id[/color]=[color=#a5d6ff]"r2"[/color] />[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]RadioGroupItem[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"compact"[/color] [color=#79c0ff]id[/color]=[color=#a5d6ff]"r3"[/color] />[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`</[color=#d2a8ff]RadioGroup[/color]>`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Composition</h2>
        <p>{`Use the following composition to build a [bgcolor=#7f7f7f29] RadioGroup [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/radio-group/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "84px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Text className="code-line">{`RadioGroup`}</Text>
                  <Text className="code-line">{`├── RadioGroupItem`}</Text>
                  <Text className="code-line">{`└── RadioGroupItem`}</Text>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Text className="code-line">{`RadioGroup`}</Text>
                  <Text className="code-line">{`├── RadioGroupItem`}</Text>
                  <Text className="code-line">{`└── RadioGroupItem`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`In practice, each item is paired with a [bgcolor=#7f7f7f29] Label [/bgcolor] placed in the same flex row, with the Label's [bgcolor=#7f7f7f29] for [/bgcolor] pointing at the RadioGroupItem's [bgcolor=#7f7f7f29] id [/bgcolor]—clicking the Label forwards the click event to the matching item.`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>Examples</h2>
        <h3>Description</h3>
        <RadioGroupDescriptionDemo />
        <h3>Choice card</h3>
        <RadioGroupChoiceCardDemo />
        <h3>Fieldset</h3>
        <RadioGroupFieldsetDemo />
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
