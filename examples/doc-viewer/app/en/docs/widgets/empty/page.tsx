import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import EmptyBasicDemo from "../../../../examples/empty-basic/index";
import EmptyBackgroundDemo from "../../../../examples/empty-background/index";
import FieldTable from "../../../../components/field-table";
import { emptyFields } from "../../../../widget-fields/empty";
import { emptyMediaFields } from "../../../../widget-fields/empty-media";
import { emptyTitleFields } from "../../../../widget-fields/empty-title";
import { emptyDescriptionFields } from "../../../../widget-fields/empty-description";
import { emptyActionFields } from "../../../../widget-fields/empty-action";

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
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#8250df]Button[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#8250df]Empty[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#8250df]EmptyAction[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#8250df]EmptyDescription[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`  [color=#8250df]EmptyMedia[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`  [color=#8250df]EmptyTitle[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`  [color=#8250df]Text[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">10</Text>
                    <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Folder[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/fluent-icons"[/color]`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] {`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]Button[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]Empty[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]EmptyAction[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]EmptyDescription[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]EmptyMedia[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]EmptyTitle[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]Text[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{`} [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">10</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] { [color=#d2a8ff]Folder[/color] } [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/fluent-icons"[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/empty/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#8250df]Empty[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]EmptyMedia[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Folder[/color] />[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#116329]</[color=#0550ae]EmptyMedia[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]EmptyTitle[/color]>[/color]No data[color=#116329]</[color=#0550ae]EmptyTitle[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]EmptyDescription[/color]>[/color]No data found[color=#116329]</[color=#0550ae]EmptyDescription[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]EmptyAction[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Button[/color]>[/color][color=#116329]<[color=#0550ae]Text[/color]>[/color]Add data[color=#116329]</[color=#0550ae]Text[/color]>[/color][color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{`  [color=#116329]</[color=#0550ae]EmptyAction[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">10</Text>
                    <Text className="code-line">{`</[color=#8250df]Empty[/color]>`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]Empty[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]EmptyMedia[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]Folder[/color] />[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]EmptyMedia[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]EmptyTitle[/color]>[/color]No data[color=#7ee787]</[color=#79c0ff]EmptyTitle[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]EmptyDescription[/color]>[/color]No data found[color=#7ee787]</[color=#79c0ff]EmptyDescription[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]EmptyAction[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]Button[/color]>[/color][color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Add data[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color][color=#7ee787]</[color=#79c0ff]Button[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]EmptyAction[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">10</Text>
                    <Text className="code-line">{`</[color=#d2a8ff]Empty[/color]>`}</Text>
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
        <p>{`Use the following composition to build an [bgcolor=#7f7f7f29] Empty [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/empty/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "124px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Text className="code-line">{`Empty`}</Text>
                  <Text className="code-line">{`├── EmptyMedia`}</Text>
                  <Text className="code-line">{`├── EmptyTitle`}</Text>
                  <Text className="code-line">{`├── EmptyDescription`}</Text>
                  <Text className="code-line">{`└── EmptyAction`}</Text>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Text className="code-line">{`Empty`}</Text>
                  <Text className="code-line">{`├── EmptyMedia`}</Text>
                  <Text className="code-line">{`├── EmptyTitle`}</Text>
                  <Text className="code-line">{`├── EmptyDescription`}</Text>
                  <Text className="code-line">{`└── EmptyAction`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
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
