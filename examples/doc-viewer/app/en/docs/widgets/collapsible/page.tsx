import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import CollapsibleDemoDemo from "../../../../examples/collapsible-demo/index";
import CollapsibleBasicDemo from "../../../../examples/collapsible-basic/index";
import CollapsibleSettingsDemo from "../../../../examples/collapsible-settings/index";
import CollapsibleFileTreeDemo from "../../../../examples/collapsible-file-tree/index";
import FieldTable from "../../../../components/field-table";
import { collapsibleFields } from "../../../../widget-fields/collapsible";
import { collapsibleTriggerFields } from "../../../../widget-fields/collapsible-trigger";
import { collapsibleContentFields } from "../../../../widget-fields/collapsible-content";

export default function CollapsibleDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Collapsible</h1>
      <p className="doc-page-desc">{`An interactive component for expanding and collapsing a section of content.`}</p>
      <CollapsibleDemoDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: Product details, settings panels, file trees, and other content that should be shown on demand`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: Cases with a lot of content where only a summary needs to be visible by default`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: Cases that need modal interaction or block the page (use [bgcolor=#eee] dialog [/bgcolor])`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: Switching between a few tabs or options (use [bgcolor=#eee] select [/bgcolor] or [bgcolor=#eee] radio-group [/bgcolor])`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/collapsible/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Collapsible[/color], [color=#8250df]CollapsibleContent[/color], [color=#8250df]CollapsibleTrigger[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/collapsible/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "144px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]Collapsible[/color] defaultOpen>`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]CollapsibleTrigger[/color]>[/color]Product details[color=#116329]</[color=#0550ae]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`    This panel can be expanded or collapsed.`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`</[color=#8250df]Collapsible[/color]>`}</Text>
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
        <p>{`Use the following composition to build a [bgcolor=#eee] Collapsible [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/collapsible/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "84px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`Collapsible`}</Text>
                <Text className="code-line">{`├── CollapsibleTrigger`}</Text>
                <Text className="code-line">{`└── CollapsibleContent`}</Text>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Examples</h2>
        <h3>Basic</h3>
        <p>{`A summary row with a border and a details panel that show order information.`}</p>
        <CollapsibleBasicDemo />
        <h3>Settings panel</h3>
        <p>{`Use the trigger to reveal additional inputs; [bgcolor=#eee] open [/bgcolor] can be used for controlled state.`}</p>
        <CollapsibleSettingsDemo />
        <h3>File tree</h3>
        <p>{`Nest Collapsible inside folder nodes to build independent tree expansion.`}</p>
        <CollapsibleFileTreeDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>Collapsible</h3>
        <p>{`The collapsible root, which holds the open/closed state.`}</p>
        <FieldTable fields={collapsibleFields} locale="en" />
        <h3>CollapsibleTrigger</h3>
        <p>{`The node that toggles the panel. You can also set [bgcolor=#eee] data-collapsible-trigger [/bgcolor] on any descendant.`}</p>
        <FieldTable fields={collapsibleTriggerFields} locale="en" />
        <h3>CollapsibleContent</h3>
        <p>{`The content area of the panel; it does not participate in display while closed.`}</p>
        <FieldTable fields={collapsibleContentFields} locale="en" />
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[bgcolor=#eee] data-collapsible-trigger [/bgcolor] is only used for event binding and does not participate in CSS selectors. To style a node, use a class or the [bgcolor=#eee] collapsible:open [/bgcolor] / [bgcolor=#eee] collapsible:closed [/bgcolor] states.`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
