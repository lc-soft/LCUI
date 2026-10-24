import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import DialogBasicDemo from "../../../examples/dialog-basic/index";
import DialogCloseButtonDemo from "../../../examples/dialog-close-button/index";
import DialogNoCloseButtonDemo from "../../../examples/dialog-no-close-button/index";
import DialogStickyFooterDemo from "../../../examples/dialog-sticky-footer/index";
import DialogScrollableContentDemo from "../../../examples/dialog-scrollable-content/index";
import FieldTable from "../../../components/field-table";
import { dialogFields } from "../../../widget-fields/dialog";
import { dialogTriggerFields } from "../../../widget-fields/dialog-trigger";
import { dialogOverlayFields } from "../../../widget-fields/dialog-overlay";
import { dialogContentFields } from "../../../widget-fields/dialog-content";
import { dialogHeaderFields } from "../../../widget-fields/dialog-header";
import { dialogTitleFields } from "../../../widget-fields/dialog-title";
import { dialogDescriptionFields } from "../../../widget-fields/dialog-description";
import { dialogFooterFields } from "../../../widget-fields/dialog-footer";
import { dialogCloseFields } from "../../../widget-fields/dialog-close";

export default function DialogDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Dialog</h1>
      <p className="doc-page-desc">{`A window overlaid on either the primary window or another dialog window, rendering the content underneath inert.`}</p>
      <DialogBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Use for[/b]: form editing, profile updates and other tasks that should be completed on the current page`}</Text></li>
          <li><Text>{`[b]Use for[/b]: detail views and guided flows that temporarily interrupt the current interaction`}</Text></li>
          <li><Text>{`[b]Don't use for[/b]: confirmations that must not be dismissed implicitly (use [bgcolor=#eee] alert-dialog [/bgcolor])`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/dialog/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Dialog[/color], [color=#8250df]DialogTrigger[/color], [color=#8250df]DialogOverlay[/color], [color=#8250df]DialogContent[/color], [color=#8250df]DialogHeader[/color], [color=#8250df]DialogTitle[/color], [color=#8250df]DialogDescription[/color], [color=#8250df]DialogFooter[/color], [color=#8250df]DialogClose[/color], [color=#8250df]Portal[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/dialog/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]Dialog[/color]>`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]DialogTrigger[/color]>[/color]Open[color=#116329]</[color=#0550ae]DialogTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]Portal[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]DialogOverlay[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]DialogContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]DialogHeader[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DialogTitle[/color]>[/color]Are you absolutely sure?[color=#116329]</[color=#0550ae]DialogTitle[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DialogDescription[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`            This action cannot be undone. This will permanently delete your`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`            account and remove your data from our servers.`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DialogDescription[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]DialogHeader[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]DialogContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]DialogOverlay[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]Portal[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`</[color=#8250df]Dialog[/color]>`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Composition</h2>
        <p>{`Use the following composition to build a [bgcolor=#eee] Dialog [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/dialog/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "204px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`Dialog`}</Text>
                <Text className="code-line">{`├── DialogTrigger`}</Text>
                <Text className="code-line">{`└── Portal`}</Text>
                <Text className="code-line">{`    └── DialogOverlay`}</Text>
                <Text className="code-line">{`        └── DialogContent`}</Text>
                <Text className="code-line">{`            ├── DialogHeader`}</Text>
                <Text className="code-line">{`            │   ├── DialogTitle`}</Text>
                <Text className="code-line">{`            │   └── DialogDescription`}</Text>
                <Text className="code-line">{`            └── DialogFooter`}</Text>
              </ScrollAreaContent>
              <Scrollbar />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Custom Close Button</h2>
        <p>{`Put your own close control in the footer (a [bgcolor=#eee] DialogClose [/bgcolor] or any node carrying [bgcolor=#eee] data-dialog-close [/bgcolor]).`}</p>
        <DialogCloseButtonDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>No Close Button</h2>
        <p>{`Simply leave out the [bgcolor=#eee] DialogClose [/bgcolor] node; when building the default subtree, call [bgcolor=#eee] ui_dialog_show_close_button() [/bgcolor] to hide the top-right button.`}</p>
        <DialogNoCloseButtonDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Sticky Footer</h2>
        <p>{`Keep actions visible while the content scrolls.`}</p>
        <DialogStickyFooterDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Scrollable Content</h2>
        <p>{`Long content can scroll inside [bgcolor=#eee] DialogContent [/bgcolor] while the header stays in view.`}</p>
        <DialogScrollableContentDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Declarative Association</h2>
        <p>{`Widgets outside the [bgcolor=#eee] Dialog [/bgcolor] subtree can be bound loosely through association keys, without a unified container:`}</p>
        <Widget className="code-block" data-language="xml" data-source="en/widgets/dialog/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">dialog-xml</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "164px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]data-dialog-trigger[/color]=[color=#0a3069]"profile"[/color]>[/color]Edit Profile[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`[color=#116329]<[color=#0550ae]dialog-overlay[/color] [color=#0550ae]data-dialog-target[/color]=[color=#0a3069]"profile"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]dialog-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"text-sm"[/color]>[/color]Clicking X or Esc closes the dialog.[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]data-dialog-close[/color]=[color=#0a3069]"profile"[/color]>[/color]Dismiss[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]dialog-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`[color=#116329]</[color=#0550ae]dialog-overlay[/color]>[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[bgcolor=#eee] data-dialog-trigger [/bgcolor] toggles the dialog on click, [bgcolor=#eee] data-dialog-target [/bgcolor] designates the floating node, [bgcolor=#eee] data-dialog-close [/bgcolor] closes the dialog on click. One key may have multiple triggers and close nodes, but only one target.`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Caveats</h2>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]Open/close animations and focus traps are not implemented.[/b] The background becomes unclickable once the overlay appears, but Tab can still move focus to background widgets. Handle strict modal behavior in the application layer if needed.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`Pressing Esc, clicking the blank overlay area, or clicking any [bgcolor=#eee] DialogClose [/bgcolor] / [bgcolor=#eee] data-dialog-close [/bgcolor] node closes the dialog; clicking inside the content does not. Esc only works while the focus is inside the dialog content subtree (on open, focus moves to the first focusable element; if nothing is focusable and no close button is injected, Esc does nothing). RTL layouts from shadcn depend on [bgcolor=#eee] direction: rtl [/bgcolor], which LCUI does not support yet.`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>Dialog</h3>
        <p>{`The dialog state container that manages opening, overlay mounting and focus restoration.`}</p>
        <FieldTable fields={dialogFields} locale="en" />
        <h3>DialogTrigger</h3>
        <p>{`The trigger that toggles the dialog; it creates a button by default.`}</p>
        <FieldTable fields={dialogTriggerFields} locale="en" />
        <h3>DialogOverlay</h3>
        <p>{`The full-screen overlay mounted on the portal root through Portal; it blocks background pointer events.`}</p>
        <FieldTable fields={dialogOverlayFields} locale="en" />
        <h3>DialogContent</h3>
        <p>{`The dialog content container centered on the overlay.`}</p>
        <FieldTable fields={dialogContentFields} locale="en" />
        <h3>DialogHeader</h3>
        <p>{`The header combining title and description.`}</p>
        <FieldTable fields={dialogHeaderFields} locale="en" />
        <h3>DialogTitle</h3>
        <p>{`The dialog title.`}</p>
        <FieldTable fields={dialogTitleFields} locale="en" />
        <h3>DialogDescription</h3>
        <p>{`The dialog description text.`}</p>
        <FieldTable fields={dialogDescriptionFields} locale="en" />
        <h3>DialogFooter</h3>
        <p>{`The action button row, right-aligned by default.`}</p>
        <FieldTable fields={dialogFooterFields} locale="en" />
        <h3>DialogClose</h3>
        <p>{`The button that closes the dialog. The default one is a ghost + icon button combination absolutely positioned at the top-right corner; any node carrying [bgcolor=#eee] data-dialog-close [/bgcolor] has the same behavior.`}</p>
        <FieldTable fields={dialogCloseFields} locale="en" />
      </Widget>
    </Widget>
  );
}
