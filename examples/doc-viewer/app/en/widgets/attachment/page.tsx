import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import AttachmentBasicDemo from "../../../examples/attachment-basic/index";
import AttachmentImageDemo from "../../../examples/attachment-image/index";
import AttachmentStatesDemo from "../../../examples/attachment-states/index";
import AttachmentSizesDemo from "../../../examples/attachment-sizes/index";
import AttachmentTriggerDemo from "../../../examples/attachment-trigger/index";
import FieldTable from "../../../components/field-table";
import { attachmentFields } from "../../../widget-fields/attachment";
import { attachmentMediaFields } from "../../../widget-fields/attachment-media";
import { attachmentContentFields } from "../../../widget-fields/attachment-content";
import { attachmentTitleFields } from "../../../widget-fields/attachment-title";
import { attachmentDescriptionFields } from "../../../widget-fields/attachment-description";
import { attachmentActionsFields } from "../../../widget-fields/attachment-actions";
import { attachmentActionFields } from "../../../widget-fields/attachment-action";
import { attachmentTriggerFields } from "../../../widget-fields/attachment-trigger";
import { attachmentGroupFields } from "../../../widget-fields/attachment-group";

export default function AttachmentDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Attachment</h1>
      <p className="doc-page-desc">{`A composable attachment surface for files and images, including media, metadata, upload state, and actions.`}</p>
      <AttachmentBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Use[/b]: File attachments in chat composers, message threads, and upload lists`}</Text></li>
          <li><Text>{`[b]Use[/b]: Image attachments with thumbnails, names, sizes, and remove actions`}</Text></li>
          <li><Text>{`[b]Use[/b]: Attachments that need idle, uploading, processing, error, and done states`}</Text></li>
          <li><Text>{`[b]Don't use[/b]: Full file-manager workflows, which belong in a dedicated page`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/attachment/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "204px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#8250df]Attachment[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#8250df]AttachmentAction[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#8250df]AttachmentActions[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#8250df]AttachmentContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#8250df]AttachmentDescription[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#8250df]AttachmentMedia[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#8250df]AttachmentTitle[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/attachment/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]Attachment[/color]>`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]AttachmentTitle[/color]>[/color]sales-dashboard.pdf[color=#116329]</[color=#0550ae]AttachmentTitle[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]AttachmentDescription[/color]>[/color]PDF · 2.4 MB[color=#116329]</[color=#0550ae]AttachmentDescription[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]AttachmentActions[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]AttachmentAction[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Remove sales-dashboard.pdf"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]AttachmentAction[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]AttachmentActions[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`</[color=#8250df]Attachment[/color]>`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Composition</h2>
        <p>{`Use the following composition to build an [bgcolor=#eee] Attachment [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/attachment/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "184px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`Attachment`}</Text>
                <Text className="code-line">{`├── AttachmentMedia`}</Text>
                <Text className="code-line">{`├── AttachmentContent`}</Text>
                <Text className="code-line">{`│   ├── AttachmentTitle`}</Text>
                <Text className="code-line">{`│   └── AttachmentDescription`}</Text>
                <Text className="code-line">{`├── AttachmentActions`}</Text>
                <Text className="code-line">{`│   └── AttachmentAction`}</Text>
                <Text className="code-line">{`└── AttachmentTrigger`}</Text>
              </ScrollAreaContent>
              <Scrollbar />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`Use [bgcolor=#eee] AttachmentGroup [/bgcolor] to lay out multiple attachments in a row:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/attachment/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "84px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`AttachmentGroup`}</Text>
                <Text className="code-line">{`├── Attachment`}</Text>
                <Text className="code-line">{`└── Attachment`}</Text>
              </ScrollAreaContent>
              <Scrollbar />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Examples</h2>
        <h3>Image</h3>
        <p>{`Set [bgcolor=#eee] variant="image" [/bgcolor] on [bgcolor=#eee] AttachmentMedia [/bgcolor] and provide an image through [bgcolor=#eee] src [/bgcolor] or a background image. With vertical orientation the media sits above the content, which suits image thumbnails.`}</p>
        <AttachmentImageDemo />
        <h3>States</h3>
        <p>{`[bgcolor=#eee] state [/bgcolor] describes the upload lifecycle. [bgcolor=#eee] uploading [/bgcolor] and [bgcolor=#eee] processing [/bgcolor] add a Shimmer sweep to the title, and the media slot can hold a Spinner.`}</p>
        <AttachmentStatesDemo />
        <h3>Sizes</h3>
        <p>{`Use [bgcolor=#eee] size [/bgcolor] to switch between [bgcolor=#eee] default [/bgcolor], [bgcolor=#eee] sm [/bgcolor], and [bgcolor=#eee] xs [/bgcolor].`}</p>
        <AttachmentSizesDemo />
        <h3>Group</h3>
        <p>{`The basic example arranges three image attachments in an [bgcolor=#eee] AttachmentGroup [/bgcolor]. Each attachment keeps its own size, and actions remain independently clickable.`}</p>
        <h3>Trigger</h3>
        <p>{`[bgcolor=#eee] AttachmentTrigger [/bgcolor] is an absolutely positioned transparent layer that covers the whole card while leaving [bgcolor=#eee] AttachmentActions [/bgcolor] clickable. Attachments with a trigger show a [bgcolor=#eee] bg-muted/50 [/bgcolor] background on hover and use a box-shadow instead of an outline for active/focus. Wire the click through declarative associations such as [bgcolor=#eee] data-dialog-trigger [/bgcolor] to a preview, link, or dialog.`}</p>
        <AttachmentTriggerDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>Attachment</h3>
        <p>{`The attachment root. Manages state, size, and orientation.`}</p>
        <FieldTable fields={attachmentFields} locale="en" />
        <h3>AttachmentMedia</h3>
        <p>{`The leading or top media slot for an icon or image preview.`}</p>
        <FieldTable fields={attachmentMediaFields} locale="en" />
        <h3>AttachmentContent</h3>
        <p>{`The flexible content slot for the title and description.`}</p>
        <FieldTable fields={attachmentContentFields} locale="en" />
        <h3>AttachmentTitle</h3>
        <p>{`The attachment name. Uploading and processing states show a Shimmer sweep.`}</p>
        <FieldTable fields={attachmentTitleFields} locale="en" />
        <h3>AttachmentDescription</h3>
        <p>{`Secondary metadata such as file type, size, or upload status.`}</p>
        <FieldTable fields={attachmentDescriptionFields} locale="en" />
        <h3>AttachmentActions</h3>
        <p>{`The action slot. Vertically oriented attachments pin it to the top right.`}</p>
        <FieldTable fields={attachmentActionsFields} locale="en" />
        <h3>AttachmentAction</h3>
        <p>{`An icon action button based on the [bgcolor=#eee] ghost [/bgcolor] button variant and [bgcolor=#eee] icon-xs [/bgcolor] size.`}</p>
        <FieldTable fields={attachmentActionFields} locale="en" />
        <h3>AttachmentTrigger</h3>
        <p>{`A transparent full-card trigger layer that sits below the actions.`}</p>
        <FieldTable fields={attachmentTriggerFields} locale="en" />
        <h3>AttachmentGroup</h3>
        <p>{`A row container for multiple attachments.`}</p>
        <FieldTable fields={attachmentGroupFields} locale="en" />
      </Widget>
    </Widget>
  );
}
