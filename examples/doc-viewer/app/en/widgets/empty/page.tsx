import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import EmptyBasicDemo from "../../../examples/empty-basic/index";
import EmptyBackgroundDemo from "../../../examples/empty-background/index";
import FieldTable from "../../../components/field-table";
import { emptyFields } from "../../../widget-fields/empty";
import { emptyMediaFields } from "../../../widget-fields/empty-media";
import { emptyTitleFields } from "../../../widget-fields/empty-title";
import { emptyDescriptionFields } from "../../../widget-fields/empty-description";
import { emptyActionFields } from "../../../widget-fields/empty-action";

export default function EmptyDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Empty</h1>
      <p className="doc-page-desc">{`Provides centered, composable placeholder content for states such as no data or no search results.`}</p>
      <EmptyBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: when a list, search result, or content area is empty`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: guiding the user to create their first item`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: a state where data is still loading (use a loading indicator)`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/empty/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
            <Text className="code-line">{`  [color=#8250df]Button[/color],`}</Text>
            <Text className="code-line">{`  [color=#8250df]Empty[/color],`}</Text>
            <Text className="code-line">{`  [color=#8250df]EmptyAction[/color],`}</Text>
            <Text className="code-line">{`  [color=#8250df]EmptyDescription[/color],`}</Text>
            <Text className="code-line">{`  [color=#8250df]EmptyMedia[/color],`}</Text>
            <Text className="code-line">{`  [color=#8250df]EmptyTitle[/color],`}</Text>
            <Text className="code-line">{`  [color=#8250df]Text[/color],`}</Text>
            <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Folder[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/fluent-icons"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/empty/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Empty[/color]>`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]EmptyMedia[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Folder[/color] />[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]</[color=#0550ae]EmptyMedia[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]EmptyTitle[/color]>[/color]No data[color=#116329]</[color=#0550ae]EmptyTitle[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]EmptyDescription[/color]>[/color]No data found[color=#116329]</[color=#0550ae]EmptyDescription[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]EmptyAction[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Button[/color]>[/color][color=#116329]<[color=#0550ae]Text[/color]>[/color]Add data[color=#116329]</[color=#0550ae]Text[/color]>[/color][color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]</[color=#0550ae]EmptyAction[/color]>[/color]`}</Text>
            <Text className="code-line">{`</[color=#8250df]Empty[/color]>`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Composition</h2>
        <p>{`Use the following composition to build an [bgcolor=#eee] Empty [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/empty/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`Empty`}</Text>
            <Text className="code-line">{`├── EmptyMedia`}</Text>
            <Text className="code-line">{`├── EmptyTitle`}</Text>
            <Text className="code-line">{`├── EmptyDescription`}</Text>
            <Text className="code-line">{`└── EmptyAction`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Background</h2>
        <p>{`Adding a light background softens the boundary between the empty state and the surrounding content.`}</p>
        <EmptyBackgroundDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>Empty</h3>
        <p>{`The empty-state container.`}</p>
        <FieldTable fields={emptyFields} locale="en" />
        <h3>EmptyMedia</h3>
        <p>{`The slot for an icon, illustration, or other media.`}</p>
        <FieldTable fields={emptyMediaFields} locale="en" />
        <h3>EmptyTitle</h3>
        <p>{`The empty-state title.`}</p>
        <FieldTable fields={emptyTitleFields} locale="en" />
        <h3>EmptyDescription</h3>
        <p>{`The empty-state description.`}</p>
        <FieldTable fields={emptyDescriptionFields} locale="en" />
        <h3>EmptyAction</h3>
        <p>{`The layout slot for a follow-up action.`}</p>
        <FieldTable fields={emptyActionFields} locale="en" />
      </Widget>
    </Widget>
  );
}
