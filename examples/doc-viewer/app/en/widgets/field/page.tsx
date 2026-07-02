import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import FieldBasicDemo from "../../../examples/field-basic/index";
import FieldCheckboxDemo from "../../../examples/field-checkbox/index";
import FieldChoiceCardDemo from "../../../examples/field-choice-card/index";
import FieldFieldsetDemo from "../../../examples/field-fieldset/index";
import FieldGroupDemo from "../../../examples/field-group/index";
import FieldInputDemo from "../../../examples/field-input/index";
import FieldRadioDemo from "../../../examples/field-radio/index";
import FieldTable from "../../../components/field-table";
import { fieldFields } from "../../../widget-fields/field";
import { fieldSetFields } from "../../../widget-fields/field-set";
import { fieldLegendFields } from "../../../widget-fields/field-legend";
import { fieldGroupFields } from "../../../widget-fields/field-group";
import { fieldLabelFields } from "../../../widget-fields/field-label";
import { fieldContentFields } from "../../../widget-fields/field-content";
import { fieldTitleFields } from "../../../widget-fields/field-title";
import { fieldDescriptionFields } from "../../../widget-fields/field-description";
import { fieldSeparatorFields } from "../../../widget-fields/field-separator";

export default function FieldDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Field</h1>
      <p className="doc-page-desc">{`Composes a label, a control, and helper text into an accessible form field.`}</p>
      <FieldBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Use[/b]: as a unified shell for form fields (label + control + description + error)`}</Text></li>
          <li><Text>{`[b]Use[/b]: when you need to place a label and a control side-by-side in a horizontal layout`}</Text></li>
          <li><Text>{`[b]Use[/b]: to group multiple fields into a semantic group (FieldSet + FieldLegend)`}</Text></li>
          <li><Text>{`[b]Don't[/b]: use for single-line inline text (use the [bgcolor=#eee] text [/bgcolor] widget instead)`}</Text></li>
          <li><Text>{`[b]Don't[/b]: use in places where native HTML [bgcolor=#eee] <form> [/bgcolor] submission semantics are required (LCUI does not replicate them)`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/field/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Field[/color], [color=#8250df]FieldLabel[/color], [color=#8250df]FieldDescription[/color], [color=#8250df]FieldSet[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/field/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Field[/color]>`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"username"[/color]>[/color]Username[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]TextInput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"username"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"evil_rabbit"[/color] />[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]Choose a unique username.[color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
            <Text className="code-line">{`</[color=#8250df]Field[/color]>`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Composition</h2>
        <p>{`Use the following composition to build a Field:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/field/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`Field`}</Text>
            <Text className="code-line">{`├── FieldLabel`}</Text>
            <Text className="code-line">{`├── TextInput / Checkbox / RadioGroup`}</Text>
            <Text className="code-line">{`└── FieldDescription`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`FieldSet`}</Text>
            <Text className="code-line">{`├── FieldLegend`}</Text>
            <Text className="code-line">{`├── FieldDescription`}</Text>
            <Text className="code-line">{`└── FieldGroup`}</Text>
            <Text className="code-line">{`    ├── Field`}</Text>
            <Text className="code-line">{`    ├── FieldSeparator`}</Text>
            <Text className="code-line">{`    └── Field`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Examples</h2>
        <h3>Checkbox</h3>
        <FieldCheckboxDemo />
        <h3>Choice Card</h3>
        <FieldChoiceCardDemo />
        <h3>Fieldset</h3>
        <FieldFieldsetDemo />
        <h3>Field Group</h3>
        <FieldGroupDemo />
        <h3>Input</h3>
        <FieldInputDemo />
        <h3>Radio</h3>
        <FieldRadioDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>Field</h3>
        <p>{`A field container providing orientation control.`}</p>
        <FieldTable fields={fieldFields} locale="en" />
        <h3>FieldSet</h3>
        <p>{`A semantic grouping container.`}</p>
        <FieldTable fields={fieldSetFields} locale="en" />
        <h3>FieldLegend</h3>
        <p>{`The title of a group.`}</p>
        <FieldTable fields={fieldLegendFields} locale="en" />
        <h3>FieldGroup</h3>
        <p>{`A visual stacking container.`}</p>
        <FieldTable fields={fieldGroupFields} locale="en" />
        <h3>FieldLabel</h3>
        <p>{`A field label whose clicks are forwarded to the target input.`}</p>
        <FieldTable fields={fieldLabelFields} locale="en" />
        <h3>FieldContent</h3>
        <p>{`The text content block within a horizontal field.`}</p>
        <FieldTable fields={fieldContentFields} locale="en" />
        <h3>FieldTitle</h3>
        <p>{`The title text within a FieldContent.`}</p>
        <FieldTable fields={fieldTitleFields} locale="en" />
        <h3>FieldDescription</h3>
        <p>{`Helper text for a field.`}</p>
        <FieldTable fields={fieldDescriptionFields} locale="en" />
        <h3>FieldSeparator</h3>
        <p>{`A horizontal divider between field groups.`}</p>
        <FieldTable fields={fieldSeparatorFields} locale="en" />
      </Widget>
      <Widget className="doc-section">
        <h2>Caveats</h2>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[b]FieldLabel's click forwarding depends on the [bgcolor=#eee] for [/bgcolor] attribute[/b]. You must set [bgcolor=#eee] for="input-id" [/bgcolor] on the FieldLabel and assign the same [bgcolor=#eee] id [/bgcolor] to the target control, otherwise the click will not be forwarded.`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
