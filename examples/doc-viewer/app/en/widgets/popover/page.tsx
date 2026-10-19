import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import PopoverBasicDemo from "../../../examples/popover-basic/index";
import PopoverSimpleDemo from "../../../examples/popover-simple/index";
import PopoverAlignDemo from "../../../examples/popover-align/index";
import FieldTable from "../../../components/field-table";
import { popoverFields } from "../../../widget-fields/popover";
import { popoverTriggerFields } from "../../../widget-fields/popover-trigger";
import { popoverContentFields } from "../../../widget-fields/popover-content";
import { popoverHeaderFields } from "../../../widget-fields/popover-header";
import { popoverTitleFields } from "../../../widget-fields/popover-title";
import { popoverDescriptionFields } from "../../../widget-fields/popover-description";

export default function PopoverDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Popover</h1>
      <p className="doc-page-desc">{`A non-modal overlay that renders interactive content in a Portal after the trigger is clicked.`}</p>
      <PopoverBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: Forms, settings panels, filters, and other cases that host interactive content`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: Showing a structured block of content after a click while keeping the rest of the page usable`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: Plain text or short hints (use [bgcolor=#eee] tooltip [/bgcolor])`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: Confirmation flows that need an overlay and focus trap (use [bgcolor=#eee] dialog [/bgcolor] / [bgcolor=#eee] alert-dialog [/bgcolor])`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: Long content or complex panels that need continuous scrolling (use a page, [bgcolor=#eee] scrollarea [/bgcolor], or a drawer)`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/popover/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Popover[/color], [color=#8250df]PopoverTrigger[/color], [color=#8250df]PopoverContent[/color], [color=#8250df]PopoverHeader[/color], [color=#8250df]PopoverTitle[/color], [color=#8250df]PopoverDescription[/color], [color=#8250df]Button[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/popover/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]Popover[/color]>`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]PopoverTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]Open popover[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]PopoverTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]PopoverContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]PopoverHeader[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]PopoverTitle[/color]>[/color]Dimensions[color=#116329]</[color=#0550ae]PopoverTitle[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]PopoverDescription[/color]>[/color]Set the dimensions for the layer.[color=#116329]</[color=#0550ae]PopoverDescription[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]PopoverHeader[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]PopoverContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`</[color=#8250df]Popover[/color]>`}</Text>
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
        <p>{`Use the following composition to build a [bgcolor=#eee] Popover [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/popover/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "164px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`Popover`}</Text>
                <Text className="code-line">{`├── PopoverTrigger`}</Text>
                <Text className="code-line">{`└── PopoverContent`}</Text>
                <Text className="code-line">{`    ├── PopoverHeader`}</Text>
                <Text className="code-line">{`    │   ├── PopoverTitle`}</Text>
                <Text className="code-line">{`    │   └── PopoverDescription`}</Text>
                <Text className="code-line">{`    └── (any interactive content)`}</Text>
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
        <p>{`A basic popover with a title and a description.`}</p>
        <PopoverSimpleDemo />
        <h3>Alignment</h3>
        <p>{`Use [bgcolor=#eee] align [/bgcolor] to control where the overlay is aligned on the side axis.`}</p>
        <PopoverAlignDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Caveats</h2>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[b]Popover is a non-modal overlay.[/b] It has no overlay backdrop or focus trap; clicking an outside control passes the click through, closes the popover, and still activates the outside control. The bubble follows the trigger when the page scrolls. Press Esc to close the current popover.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`Only one active popover is kept at a time; opening another popover closes the previous one. Clicking inside the content does not close the popover; to close it, call [bgcolor=#eee] ui_popover_close() [/bgcolor] or click the trigger or outside the popover.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]The current implementation does not include edge collision flipping or repositioning on resize.[/b] It also does not provide [bgcolor=#eee] PopoverAnchor [/bgcolor]; to move the anchor, place [bgcolor=#eee] PopoverTrigger [/bgcolor] at the target position.`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>Popover</h3>
        <p>{`The popover state container, which manages open state, the Portal, and focus restoration.`}</p>
        <FieldTable fields={popoverFields} locale="en" />
        <h3>PopoverTrigger</h3>
        <p>{`The wrapper that triggers the popover. Put a button, icon, or other clickable widget inside.`}</p>
        <FieldTable fields={popoverTriggerFields} locale="en" />
        <h3>PopoverContent</h3>
        <p>{`The Portal bubble that hosts interactive content and handles side, alignment, and offset.`}</p>
        <FieldTable fields={popoverContentFields} locale="en" />
        <h3>PopoverHeader</h3>
        <p>{`A header that groups the title and description.`}</p>
        <FieldTable fields={popoverHeaderFields} locale="en" />
        <h3>PopoverTitle</h3>
        <p>{`The popover title.`}</p>
        <FieldTable fields={popoverTitleFields} locale="en" />
        <h3>PopoverDescription</h3>
        <p>{`The popover description text.`}</p>
        <FieldTable fields={popoverDescriptionFields} locale="en" />
      </Widget>
    </Widget>
  );
}
