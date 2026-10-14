import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import AlertDialogBasicDemo from "../../../examples/alert-dialog-basic/index";
import AlertDialogSmallDemo from "../../../examples/alert-dialog-small/index";
import AlertDialogDestructiveDemo from "../../../examples/alert-dialog-destructive/index";
import AlertDialogCallbackDemo from "../../../examples/alert-dialog-callback/index";
import FieldTable from "../../../components/field-table";
import { alertDialogFields } from "../../../widget-fields/alert-dialog";
import { alertDialogTriggerFields } from "../../../widget-fields/alert-dialog-trigger";
import { alertDialogOverlayFields } from "../../../widget-fields/alert-dialog-overlay";
import { alertDialogContentFields } from "../../../widget-fields/alert-dialog-content";
import { alertDialogHeaderFields } from "../../../widget-fields/alert-dialog-header";
import { alertDialogMediaFields } from "../../../widget-fields/alert-dialog-media";
import { alertDialogTitleFields } from "../../../widget-fields/alert-dialog-title";
import { alertDialogDescriptionFields } from "../../../widget-fields/alert-dialog-description";
import { alertDialogFooterFields } from "../../../widget-fields/alert-dialog-footer";
import { alertDialogCancelFields } from "../../../widget-fields/alert-dialog-cancel";
import { alertDialogActionFields } from "../../../widget-fields/alert-dialog-action";

export default function AlertDialogDocPage() {
  return (
    <Widget className="doc-page">
      <h1>AlertDialog</h1>
      <p className="doc-page-desc">{`A modal dialog that interrupts the user with important content and expects a response.`}</p>
      <AlertDialogBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Use for[/b]: confirming destructive or irreversible operations such as deletion`}</Text></li>
          <li><Text>{`[b]Use for[/b]: flows that may continue only after an explicit "confirm / cancel" choice`}</Text></li>
          <li><Text>{`[b]Don't use for[/b]: ordinary panels that may be dismissed with Esc or an overlay click (use [bgcolor=#eee] dialog [/bgcolor])`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/alert-dialog/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]AlertDialog[/color], [color=#8250df]AlertDialogTrigger[/color], [color=#8250df]AlertDialogOverlay[/color], [color=#8250df]AlertDialogContent[/color], [color=#8250df]AlertDialogHeader[/color], [color=#8250df]AlertDialogMedia[/color], [color=#8250df]AlertDialogTitle[/color], [color=#8250df]AlertDialogDescription[/color], [color=#8250df]AlertDialogFooter[/color], [color=#8250df]AlertDialogCancel[/color], [color=#8250df]AlertDialogAction[/color], [color=#8250df]Portal[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/alert-dialog/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]AlertDialog[/color]>`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]AlertDialogTrigger[/color]>[/color]Show Dialog[color=#116329]</[color=#0550ae]AlertDialogTrigger[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]Portal[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]AlertDialogOverlay[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]AlertDialogContent[/color]>[/color]`}</Text>
            <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AlertDialogHeader[/color]>[/color]`}</Text>
            <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AlertDialogTitle[/color]>[/color]Are you absolutely sure?[color=#116329]</[color=#0550ae]AlertDialogTitle[/color]>[/color]`}</Text>
            <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AlertDialogDescription[/color]>[/color]`}</Text>
            <Text className="code-line">{`            This action cannot be undone. This will permanently delete your`}</Text>
            <Text className="code-line">{`            account and remove your data from our servers.`}</Text>
            <Text className="code-line">{`          [color=#116329]</[color=#0550ae]AlertDialogDescription[/color]>[/color]`}</Text>
            <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AlertDialogHeader[/color]>[/color]`}</Text>
            <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AlertDialogFooter[/color]>[/color]`}</Text>
            <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AlertDialogCancel[/color]>[/color]Cancel[color=#116329]</[color=#0550ae]AlertDialogCancel[/color]>[/color]`}</Text>
            <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AlertDialogAction[/color]>[/color]Continue[color=#116329]</[color=#0550ae]AlertDialogAction[/color]>[/color]`}</Text>
            <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AlertDialogFooter[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]</[color=#0550ae]AlertDialogContent[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]</[color=#0550ae]AlertDialogOverlay[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]</[color=#0550ae]Portal[/color]>[/color]`}</Text>
            <Text className="code-line">{`</[color=#8250df]AlertDialog[/color]>`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Composition</h2>
        <p>{`Use the following composition to build an [bgcolor=#eee] AlertDialog [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/alert-dialog/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`AlertDialog`}</Text>
            <Text className="code-line">{`├── AlertDialogTrigger`}</Text>
            <Text className="code-line">{`└── Portal`}</Text>
            <Text className="code-line">{`    └── AlertDialogOverlay`}</Text>
            <Text className="code-line">{`        └── AlertDialogContent`}</Text>
            <Text className="code-line">{`            ├── AlertDialogHeader`}</Text>
            <Text className="code-line">{`            │   ├── AlertDialogMedia`}</Text>
            <Text className="code-line">{`            │   ├── AlertDialogTitle`}</Text>
            <Text className="code-line">{`            │   └── AlertDialogDescription`}</Text>
            <Text className="code-line">{`            └── AlertDialogFooter`}</Text>
            <Text className="code-line">{`                ├── AlertDialogCancel`}</Text>
            <Text className="code-line">{`                └── AlertDialogAction`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Small</h2>
        <p>{`Use the [bgcolor=#eee] size="sm" [/bgcolor] prop on the [bgcolor=#eee] AlertDialogContent [/bgcolor] component to make the alert dialog narrower; the two footer buttons then stretch equally across the content area.`}</p>
        <AlertDialogSmallDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Destructive</h2>
        <p>{`Use an [bgcolor=#eee] AlertDialogAction [/bgcolor] with [bgcolor=#eee] variant="destructive" [/bgcolor] to express destructive actions such as deletion.`}</p>
        <AlertDialogDestructiveDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Cancel and Confirm Callbacks</h2>
        <p>{`[bgcolor=#eee] AlertDialogCancel [/bgcolor] and [bgcolor=#eee] AlertDialogAction [/bgcolor] close the dialog and then emit the [bgcolor=#eee] cancel [/bgcolor] / [bgcolor=#eee] action [/bgcolor] events respectively; handling the business result is up to the caller. Plain buttons outside the subtree can join the flow through the [bgcolor=#eee] data-dialog-cancel [/bgcolor] / [bgcolor=#eee] data-dialog-action [/bgcolor] association keys.`}</p>
        <AlertDialogCallbackDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Caveats</h2>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]Pressing Esc or clicking the overlay does not close the alert dialog[/b]; the user can only choose cancel or confirm, while an explicit [bgcolor=#eee] ui_dialog_close() [/bgcolor] still works. Open/close animations and focus traps are not implemented.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`Confirm actions are synchronous events; waiting on async results or multi-step confirmations is out of scope and can be built on top of the [bgcolor=#eee] action [/bgcolor] event in the application layer. The Media section from shadcn relies on a two-column CSS grid layout, which LCUI does not support; the [bgcolor=#eee] AlertDialogMedia [/bgcolor] icon stacks vertically and centers (see the Destructive example). RTL layouts are not supported either.`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>AlertDialog</h3>
        <p>{`The alert dialog state container; it reuses the Dialog open/close APIs and rejects implicit dismissal by default.`}</p>
        <FieldTable fields={alertDialogFields} locale="en" />
        <h3>AlertDialogTrigger</h3>
        <p>{`The trigger that opens the alert dialog; it creates a button by default.`}</p>
        <FieldTable fields={alertDialogTriggerFields} locale="en" />
        <h3>AlertDialogOverlay</h3>
        <p>{`The full-screen overlay mounted on the portal root through Portal; it never closes on click.`}</p>
        <FieldTable fields={alertDialogOverlayFields} locale="en" />
        <h3>AlertDialogContent</h3>
        <p>{`The confirmation content container centered on the overlay.`}</p>
        <FieldTable fields={alertDialogContentFields} locale="en" />
        <h3>AlertDialogHeader</h3>
        <p>{`The header combining media, title and description; centered only under [bgcolor=#eee] size="sm" [/bgcolor].`}</p>
        <FieldTable fields={alertDialogHeaderFields} locale="en" />
        <h3>AlertDialogMedia</h3>
        <p>{`The media slot in the header for an icon or an image.`}</p>
        <FieldTable fields={alertDialogMediaFields} locale="en" />
        <h3>AlertDialogTitle</h3>
        <p>{`The alert dialog title.`}</p>
        <FieldTable fields={alertDialogTitleFields} locale="en" />
        <h3>AlertDialogDescription</h3>
        <p>{`The alert dialog description text.`}</p>
        <FieldTable fields={alertDialogDescriptionFields} locale="en" />
        <h3>AlertDialogFooter</h3>
        <p>{`The cancel and confirm button row, right-aligned by default; with [bgcolor=#eee] size="sm" [/bgcolor] the buttons stretch equally.`}</p>
        <FieldTable fields={alertDialogFooterFields} locale="en" />
        <h3>AlertDialogCancel</h3>
        <p>{`The cancel button: closes the dialog and emits the [bgcolor=#eee] cancel [/bgcolor] event.`}</p>
        <FieldTable fields={alertDialogCancelFields} locale="en" />
        <h3>AlertDialogAction</h3>
        <p>{`The confirm button: closes the dialog and emits the [bgcolor=#eee] action [/bgcolor] event.`}</p>
        <FieldTable fields={alertDialogActionFields} locale="en" />
      </Widget>
    </Widget>
  );
}
