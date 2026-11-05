import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import AlertBasicDemo from "../../../../examples/alert-basic/index";
import AlertVariantsDemo from "../../../../examples/alert-variants/index";
import AlertDestructiveDemo from "../../../../examples/alert-destructive/index";
import AlertActionDemo from "../../../../examples/alert-action/index";
import FieldTable from "../../../../components/field-table";
import { alertFields } from "../../../../widget-fields/alert";
import { alertContentFields } from "../../../../widget-fields/alert-content";
import { alertTitleFields } from "../../../../widget-fields/alert-title";
import { alertDescriptionFields } from "../../../../widget-fields/alert-description";
import { alertActionFields } from "../../../../widget-fields/alert-action";

export default function AlertDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Alert</h1>
      <p className="doc-page-desc">{`Displays a non-blocking status, result, or risk message within the current content area.`}</p>
      <AlertBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: showing a save result, a permission reminder, or a recoverable error`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: offering a follow-up action next to local content`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: notifications that need a global queue or auto-dismiss`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/alert/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Alert[/color], [color=#8250df]AlertContent[/color], [color=#8250df]AlertDescription[/color], [color=#8250df]AlertTitle[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/alert/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "144px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]Alert[/color]>`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]AlertContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]AlertTitle[/color]>[/color]Payment successful[color=#116329]</[color=#0550ae]AlertTitle[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]AlertDescription[/color]>[/color]Your payment has been processed.[color=#116329]</[color=#0550ae]AlertDescription[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]AlertContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`</[color=#8250df]Alert[/color]>`}</Text>
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
        <p>{`Use the following composition to build an [bgcolor=#eee] Alert [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/alert/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "144px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`Alert`}</Text>
                <Text className="code-line">{`├── Icon`}</Text>
                <Text className="code-line">{`├── AlertContent`}</Text>
                <Text className="code-line">{`│   ├── AlertTitle`}</Text>
                <Text className="code-line">{`│   └── AlertDescription`}</Text>
                <Text className="code-line">{`└── AlertAction`}</Text>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Variants</h2>
        <p>{`Use the [bgcolor=#eee] variant [/bgcolor] prop to switch the alert's visual semantics.`}</p>
        <AlertVariantsDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Destructive</h2>
        <p>{`Use [bgcolor=#eee] variant="destructive" [/bgcolor] for errors that cannot be recovered or need immediate attention.`}</p>
        <AlertDestructiveDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Action</h2>
        <p>{`Use [bgcolor=#eee] AlertAction [/bgcolor] to place a follow-up action related to the alert.`}</p>
        <AlertActionDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>Alert</h3>
        <p>{`The alert container; controls the overall visual variant.`}</p>
        <FieldTable fields={alertFields} locale="en" />
        <h3>AlertContent</h3>
        <p>{`A vertical content container for the title and description.`}</p>
        <FieldTable fields={alertContentFields} locale="en" />
        <h3>AlertTitle</h3>
        <p>{`The alert title.`}</p>
        <FieldTable fields={alertTitleFields} locale="en" />
        <h3>AlertDescription</h3>
        <p>{`The alert's supporting description.`}</p>
        <FieldTable fields={alertDescriptionFields} locale="en" />
        <h3>AlertAction</h3>
        <p>{`The layout slot for an action button.`}</p>
        <FieldTable fields={alertActionFields} locale="en" />
      </Widget>
    </Widget>
  );
}
