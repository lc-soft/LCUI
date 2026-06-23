import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import ButtonBasicDemo from "../../../examples/button-basic/index";
import FieldTable from "../../../components/field-table";
import { buttonFields } from "../../../widget-fields/button";

export default function ButtonDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Button</h1>
      <p className="doc-page-desc">{`A clickable button widget, typically used to trigger an action.`}</p>
      <ButtonBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: users click to perform an action (submit a form, open a page, toggle a state)`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: interactive entry points in toolbars, dialogs, and navbars`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: non-interactive text display (use [bgcolor=#eee] text [/bgcolor] instead)`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Anatomy</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/button/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">Anatomy</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Button[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]Button[/color] />[/color]`}</Text>
          </Widget>
        </Widget>
        <p>{`Button is a single-line widget composed of a background layer and text content. The [bgcolor=#eee] :hover [/bgcolor] / [bgcolor=#eee] :active [/bgcolor] pseudo-classes toggle the background color to provide click feedback.`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <FieldTable fields={buttonFields} locale="en" />
      </Widget>
    </Widget>
  );
}
