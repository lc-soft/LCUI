import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import AttachmentBasicDemo from "../../../../examples/attachment-basic/index";
import AttachmentImageDemo from "../../../../examples/attachment-image/index";
import AttachmentStatesDemo from "../../../../examples/attachment-states/index";
import AttachmentSizesDemo from "../../../../examples/attachment-sizes/index";
import AttachmentTriggerDemo from "../../../../examples/attachment-trigger/index";
import FieldTable from "../../../../components/field-table";
import { attachmentFields } from "../../../../widget-fields/attachment";
import { attachmentMediaFields } from "../../../../widget-fields/attachment-media";
import { attachmentContentFields } from "../../../../widget-fields/attachment-content";
import { attachmentTitleFields } from "../../../../widget-fields/attachment-title";
import { attachmentDescriptionFields } from "../../../../widget-fields/attachment-description";
import { attachmentActionsFields } from "../../../../widget-fields/attachment-actions";
import { attachmentActionFields } from "../../../../widget-fields/attachment-action";
import { attachmentTriggerFields } from "../../../../widget-fields/attachment-trigger";
import { attachmentGroupFields } from "../../../../widget-fields/attachment-group";

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
                <Widget className="code-block-lines code-block-lines-light">
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
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] {`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]Attachment[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]AttachmentAction[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]AttachmentActions[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]AttachmentContent[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]AttachmentDescription[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]AttachmentMedia[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]AttachmentTitle[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{`} [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
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
                <Widget className="code-block-lines code-block-lines-light">
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
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]Attachment[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]AttachmentMedia[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"document"[/color] />[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]AttachmentMedia[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]AttachmentContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]AttachmentTitle[/color]>[/color]sales-dashboard.pdf[color=#7ee787]</[color=#79c0ff]AttachmentTitle[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]AttachmentDescription[/color]>[/color]PDF · 2.4 MB[color=#7ee787]</[color=#79c0ff]AttachmentDescription[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]AttachmentContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]AttachmentActions[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">10</Text>
                    <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]AttachmentAction[/color] [color=#79c0ff]aria-label[/color]=[color=#a5d6ff]"Remove sales-dashboard.pdf"[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">11</Text>
                    <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"dismiss"[/color] />[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">12</Text>
                    <Text className="code-line">{`    [color=#7ee787]</[color=#79c0ff]AttachmentAction[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">13</Text>
                    <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]AttachmentActions[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">14</Text>
                    <Text className="code-line">{`</[color=#d2a8ff]Attachment[/color]>`}</Text>
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
        <p>{`Use the following composition to build an [bgcolor=#7f7f7f29] Attachment [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/attachment/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "184px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Text className="code-line">{`Attachment`}</Text>
                  <Text className="code-line">{`├── AttachmentMedia`}</Text>
                  <Text className="code-line">{`├── AttachmentContent`}</Text>
                  <Text className="code-line">{`│   ├── AttachmentTitle`}</Text>
                  <Text className="code-line">{`│   └── AttachmentDescription`}</Text>
                  <Text className="code-line">{`├── AttachmentActions`}</Text>
                  <Text className="code-line">{`│   └── AttachmentAction`}</Text>
                  <Text className="code-line">{`└── AttachmentTrigger`}</Text>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Text className="code-line">{`Attachment`}</Text>
                  <Text className="code-line">{`├── AttachmentMedia`}</Text>
                  <Text className="code-line">{`├── AttachmentContent`}</Text>
                  <Text className="code-line">{`│   ├── AttachmentTitle`}</Text>
                  <Text className="code-line">{`│   └── AttachmentDescription`}</Text>
                  <Text className="code-line">{`├── AttachmentActions`}</Text>
                  <Text className="code-line">{`│   └── AttachmentAction`}</Text>
                  <Text className="code-line">{`└── AttachmentTrigger`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`Use [bgcolor=#7f7f7f29] AttachmentGroup [/bgcolor] to lay out multiple attachments in a row:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/attachment/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "84px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Text className="code-line">{`AttachmentGroup`}</Text>
                  <Text className="code-line">{`├── Attachment`}</Text>
                  <Text className="code-line">{`└── Attachment`}</Text>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Text className="code-line">{`AttachmentGroup`}</Text>
                  <Text className="code-line">{`├── Attachment`}</Text>
                  <Text className="code-line">{`└── Attachment`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Examples</h2>
        <h3>Image</h3>
        <p>{`Set [bgcolor=#7f7f7f29] variant="image" [/bgcolor] on [bgcolor=#7f7f7f29] AttachmentMedia [/bgcolor] and provide an image through [bgcolor=#7f7f7f29] src [/bgcolor] or a background image. With vertical orientation the media sits above the content, which suits image thumbnails.`}</p>
        <AttachmentImageDemo />
        <h3>States</h3>
        <p>{`[bgcolor=#7f7f7f29] state [/bgcolor] describes the upload lifecycle. [bgcolor=#7f7f7f29] uploading [/bgcolor] and [bgcolor=#7f7f7f29] processing [/bgcolor] add a Shimmer sweep to the title, and the media slot can hold a Spinner.`}</p>
        <AttachmentStatesDemo />
        <h3>Sizes</h3>
        <p>{`Use [bgcolor=#7f7f7f29] size [/bgcolor] to switch between [bgcolor=#7f7f7f29] default [/bgcolor], [bgcolor=#7f7f7f29] sm [/bgcolor], and [bgcolor=#7f7f7f29] xs [/bgcolor].`}</p>
        <AttachmentSizesDemo />
        <h3>Group</h3>
        <p>{`The basic example arranges three image attachments in an [bgcolor=#7f7f7f29] AttachmentGroup [/bgcolor]. Each attachment keeps its own size, and actions remain independently clickable.`}</p>
        <h3>Trigger</h3>
        <p>{`[bgcolor=#7f7f7f29] AttachmentTrigger [/bgcolor] is an absolutely positioned transparent layer that covers the whole card while leaving [bgcolor=#7f7f7f29] AttachmentActions [/bgcolor] clickable. Attachments with a trigger show a [bgcolor=#7f7f7f29] bg-muted/50 [/bgcolor] background on hover and use a box-shadow instead of an outline for active/focus. Wire the click through declarative associations such as [bgcolor=#7f7f7f29] data-dialog-trigger [/bgcolor] to a preview, link, or dialog.`}</p>
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
        <p>{`An icon action button based on the [bgcolor=#7f7f7f29] ghost [/bgcolor] button variant and [bgcolor=#7f7f7f29] icon-xs [/bgcolor] size.`}</p>
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
