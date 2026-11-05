import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import AccordionDemoDemo from "../../../../examples/accordion-demo/index";
import AccordionBasicDemoDemo from "../../../../examples/accordion-basic-demo/index";
import AccordionMultipleDemo from "../../../../examples/accordion-multiple/index";
import AccordionDisabledDemo from "../../../../examples/accordion-disabled/index";
import AccordionBordersDemo from "../../../../examples/accordion-borders/index";
import AccordionCardDemo from "../../../../examples/accordion-card/index";
import AccordionCustomTriggerDemo from "../../../../examples/accordion-custom-trigger/index";
import FieldTable from "../../../../components/field-table";
import { accordionFields } from "../../../../widget-fields/accordion";
import { accordionItemFields } from "../../../../widget-fields/accordion-item";
import { accordionTriggerFields } from "../../../../widget-fields/accordion-trigger";
import { accordionContentFields } from "../../../../widget-fields/accordion-content";

export default function AccordionDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Accordion</h1>
      <p className="doc-page-desc">{`Vertically stacked sections with headers and content, expanding one or more panels at a time.`}</p>
      <AccordionDemoDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: FAQ pages, grouped settings, billing details, and other pages where content is revealed by a header`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: Long content where only part of it needs to be read at a time`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: Side-by-side tabs (use [bgcolor=#eee] select [/bgcolor] or a future tabs widget)`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: Modal confirmation or blocking flows (use [bgcolor=#eee] dialog [/bgcolor])`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/accordion/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "144px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#8250df]Accordion[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#8250df]AccordionContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#8250df]AccordionItem[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#8250df]AccordionTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/accordion/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "184px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]Accordion[/color] defaultValue=[color=#0a3069]"shipping"[/color]>`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]AccordionItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"shipping"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]AccordionTrigger[/color]>[/color]What are your shipping options?[color=#116329]</[color=#0550ae]AccordionTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]AccordionContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`      We offer standard, express, and overnight shipping.`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]AccordionContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]AccordionItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`</[color=#8250df]Accordion[/color]>`}</Text>
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
        <p>{`Use the following composition to build an [bgcolor=#eee] Accordion [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/accordion/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "164px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`Accordion`}</Text>
                <Text className="code-line">{`├── AccordionItem`}</Text>
                <Text className="code-line">{`│   ├── AccordionTrigger`}</Text>
                <Text className="code-line">{`│   └── AccordionContent`}</Text>
                <Text className="code-line">{`└── AccordionItem`}</Text>
                <Text className="code-line">{`    ├── AccordionTrigger`}</Text>
                <Text className="code-line">{`    └── AccordionContent`}</Text>
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
        <p>{`Single-open mode with the first item expanded by default; clicking an expanded item collapses it.`}</p>
        <AccordionBasicDemoDemo />
        <h3>Multiple</h3>
        <p>{`With [bgcolor=#eee] multiple [/bgcolor], each item expands independently and several sections can be open at once.`}</p>
        <AccordionMultipleDemo />
        <h3>Disabled</h3>
        <p>{`Set [bgcolor=#eee] disabled [/bgcolor] on an [bgcolor=#eee] AccordionItem [/bgcolor] to ignore click and Enter/Space for that item.`}</p>
        <AccordionDisabledDemo />
        <h3>Borders</h3>
        <p>{`The theme already gives each item a bottom divider; the example adds an outer border and rounded corners to the root and horizontal padding to each item.`}</p>
        <AccordionBordersDemo />
        <h3>Card</h3>
        <p>{`Place the Accordion inside a Card for billing, subscription, and other settings blocks.`}</p>
        <AccordionCardDemo />
        <h3>Custom trigger</h3>
        <p>{`When the header needs full custom markup, skip [bgcolor=#eee] AccordionTrigger [/bgcolor] and declare [bgcolor=#eee] data-accordion-trigger="true" [/bgcolor] on any item descendant. No chevron is created automatically; focus and expanded/collapsed visuals are the caller's responsibility.`}</p>
        <AccordionCustomTriggerDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>Accordion</h3>
        <p>{`The accordion root, which manages the open set, single/multiple mode, and the [bgcolor=#eee] change [/bgcolor] event.`}</p>
        <FieldTable fields={accordionFields} locale="en" />
        <h3>AccordionItem</h3>
        <p>{`A single section that holds its [bgcolor=#eee] open [/bgcolor] state and item [bgcolor=#eee] value [/bgcolor].`}</p>
        <FieldTable fields={accordionItemFields} locale="en" />
        <h3>AccordionTrigger</h3>
        <p>{`The header row with a chevron on the right by default; set [bgcolor=#eee] chevron="false" [/bgcolor] to remove it.`}</p>
        <FieldTable fields={accordionTriggerFields} locale="en" />
        <h3>AccordionContent</h3>
        <p>{`The section content; it does not participate in display while the item is closed.`}</p>
        <FieldTable fields={accordionContentFields} locale="en" />
      </Widget>
      <Widget className="doc-section">
        <h2>Caveats</h2>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[b]No expand/collapse animation[/b]. LCUI's CSS engine does not support transitions or animations, so state changes are instant.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-info">
          <Text className="admonition-title">信息</Text>
          <Widget className="admonition-content">
            <p>{`[b]Initialization does not emit events[/b]. [bgcolor=#eee] defaultValue [/bgcolor] / [bgcolor=#eee] value [/bgcolor] and item [bgcolor=#eee] open [/bgcolor] during parsing only synchronize state; [bgcolor=#eee] change [/bgcolor] is emitted only for user clicks, Enter/Space, or [bgcolor=#eee] ui_accordion_item_set_open() [/bgcolor]. [bgcolor=#eee] ui_accordion_set_value() [/bgcolor] is a silent controlled sync.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-info">
          <Text className="admonition-title">信息</Text>
          <Widget className="admonition-content">
            <p>{`[b]The [bgcolor=#eee] change [/bgcolor] payload is the value of the toggled item[/b]. The root [bgcolor=#eee] value [/bgcolor] attribute holds the full comma-separated set; read it with [bgcolor=#eee] ui_accordion_get_value(root) [/bgcolor] inside a listener. Setting multiple values at once does not emit one event per item.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-tip">
          <Text className="admonition-title">技巧</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#eee] data-accordion-trigger [/bgcolor] is an LCUI extension[/b]. It is a behavior marker only and does not participate in CSS selectors; custom triggers must handle focus themselves and use [bgcolor=#eee] accordion-item:open [/bgcolor] / [bgcolor=#eee] accordion-item:closed [/bgcolor] for expanded-state styles.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]Keyboard support is limited to Enter/Space[/b]. Triggers can be focused with Tab; arrow keys and Home/End roving tabindex are not implemented. When single mode auto-closes other items, those items do not emit additional [bgcolor=#eee] open [/bgcolor]/[bgcolor=#eee] close [/bgcolor] events.`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
