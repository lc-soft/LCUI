import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import LabelBasicDemo from "../../../examples/label-basic/index";
import FieldTable from "../../../components/field-table";
import { labelFields } from "../../../widget-fields/label";

export default function LabelDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Label</h1>
      <p className="doc-page-desc">{`Renders an accessible label associated with controls. Click events are forwarded to the target widget referenced by the [bgcolor=#eee] for [/bgcolor] attribute.`}</p>
      <LabelBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: extending the clickable area of interactive widgets such as checkbox and text-input so that clicking the label text also triggers the widget.`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: associating text with a control for accessibility (a11y).`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: purely decorative text display (use the [bgcolor=#eee] text [/bgcolor] widget instead).`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/label/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Label[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/label/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Label[/color] [color=#cf222e]for[/color]=[color=#0a3069]"agree"[/color]>I agree</[color=#8250df]Label[/color]>`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <FieldTable fields={labelFields} locale="en" />
      </Widget>
    </Widget>
  );
}
