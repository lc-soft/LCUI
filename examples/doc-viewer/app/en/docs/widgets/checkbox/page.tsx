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
          <li><Text>{`[b]Not suitable[/b]: Mutually exclusive choices (use [bgcolor=#7f7f7f29] radio-group [/bgcolor])`}</Text></li>
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
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Checkbox[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] { [color=#d2a8ff]Checkbox[/color] } [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color]`}</Text>
                  </Widget>
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
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#8250df]Checkbox[/color] />`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]Checkbox[/color] />`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`Use [bgcolor=#7f7f7f29] checked [/bgcolor] to control the checked state; a [bgcolor=#7f7f7f29] change [/bgcolor] event is emitted when it changes.`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>Examples</h2>
        <h3>Description</h3>
        <p>{`Pair [bgcolor=#7f7f7f29] FieldContent [/bgcolor] and [bgcolor=#7f7f7f29] FieldDescription [/bgcolor] to add helper text to the option.`}</p>
        <CheckboxDescriptionDemo />
        <h3>Group</h3>
        <p>{`Combine several [bgcolor=#7f7f7f29] Field [/bgcolor]s to build a checkbox list.`}</p>
        <CheckboxGroupDemo />
        <h3>Indeterminate</h3>
        <p>{`[bgcolor=#7f7f7f29] indeterminate [/bgcolor] expresses the "some children are selected" visual state, showing a minus icon.`}</p>
        <CheckboxIndeterminateDemo />
        <h3>Disabled</h3>
        <CheckboxDisabledDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Caveats</h2>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]Indeterminate is a state, not a default.[/b] The indeterminate state ([bgcolor=#7f7f7f29] indeterminate="true" [/bgcolor]) expresses the "some children are selected" visual state (for example, a parent checkbox when only some children are checked). Clicking an indeterminate checkbox transitions it to the checked state.`}</p>
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
