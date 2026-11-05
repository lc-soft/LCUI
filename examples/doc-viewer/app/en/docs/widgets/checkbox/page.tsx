import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import CheckboxBasicDemo from "../../../../examples/checkbox-basic/index";
import CheckboxDescriptionDemo from "../../../../examples/checkbox-description/index";
import CheckboxGroupDemo from "../../../../examples/checkbox-group/index";
import CheckboxIndeterminateDemo from "../../../../examples/checkbox-indeterminate/index";
import CheckboxDisabledDemo from "../../../../examples/checkbox-disabled/index";
import FieldTable from "../../../../components/field-table";
import { checkboxFields } from "../../../../widget-fields/checkbox";

export default function CheckboxDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Checkbox</h1>
      <p className="doc-page-desc">{`A control that allows the user to toggle between checked and not checked.`}</p>
      <CheckboxBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: A single binary choice (accepting terms, a subscription toggle)`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: Checkable items in a form`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: Mutually exclusive choices (use [bgcolor=#eee] radio-group [/bgcolor])`}</Text></li>
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
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Checkbox[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/checkbox/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]Checkbox[/color] />`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`Use [bgcolor=#eee] checked [/bgcolor] to control the checked state; a [bgcolor=#eee] change [/bgcolor] event is emitted when it changes.`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>Examples</h2>
        <h3>Description</h3>
        <p>{`Pair [bgcolor=#eee] FieldContent [/bgcolor] and [bgcolor=#eee] FieldDescription [/bgcolor] to add helper text to the option.`}</p>
        <CheckboxDescriptionDemo />
        <h3>Group</h3>
        <p>{`Combine several [bgcolor=#eee] Field [/bgcolor]s to build a checkbox list.`}</p>
        <CheckboxGroupDemo />
        <h3>Indeterminate</h3>
        <p>{`[bgcolor=#eee] indeterminate [/bgcolor] expresses the "some children are selected" visual state, showing a minus icon.`}</p>
        <CheckboxIndeterminateDemo />
        <h3>Disabled</h3>
        <CheckboxDisabledDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Caveats</h2>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]Indeterminate is a state, not a default.[/b] The indeterminate state ([bgcolor=#eee] indeterminate="true" [/bgcolor]) expresses the "some children are selected" visual state (for example, a parent checkbox when only some children are checked). Clicking an indeterminate checkbox transitions it to the checked state.`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <FieldTable fields={checkboxFields} locale="en" />
      </Widget>
    </Widget>
  );
}
