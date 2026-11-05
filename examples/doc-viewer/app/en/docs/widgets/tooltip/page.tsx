import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import TooltipBasicDemo from "../../../../examples/tooltip-basic/index";
import TooltipSidesDemo from "../../../../examples/tooltip-sides/index";
import TooltipDisabledDemo from "../../../../examples/tooltip-disabled/index";
import FieldTable from "../../../../components/field-table";
import { tooltipFields } from "../../../../widget-fields/tooltip";
import { tooltipTriggerFields } from "../../../../widget-fields/tooltip-trigger";
import { tooltipContentFields } from "../../../../widget-fields/tooltip-content";

export default function TooltipDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Tooltip</h1>
      <p className="doc-page-desc">{`An overlay that shows a short description on hover or focus.`}</p>
      <TooltipBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: Icon buttons, truncated text, toolbars, and form labels that need extra context`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: Short, non-interactive hints`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: Content that requires confirmation or user action (use [bgcolor=#eee] dialog [/bgcolor] / [bgcolor=#eee] alert-dialog [/bgcolor])`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: Long explanations or rich text (use body text or [bgcolor=#eee] field-description [/bgcolor])`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/tooltip/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Tooltip[/color], [color=#8250df]TooltipTrigger[/color], [color=#8250df]TooltipContent[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/tooltip/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "144px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]Tooltip[/color]>`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]TooltipTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]Hover[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]TooltipTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]TooltipContent[/color]>[/color]Add to library[color=#116329]</[color=#0550ae]TooltipContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`</[color=#8250df]Tooltip[/color]>`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Attribute-only usage</h2>
        <p>{`Any widget can use the [bgcolor=#eee] data-tooltip [/bgcolor] attribute without the [bgcolor=#eee] Tooltip [/bgcolor] structure:`}</p>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/tooltip/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]Button[/color] data-tooltip=[color=#0a3069]"Add to library"[/color]>[color=#8250df]Hover[/color]</[color=#8250df]Button[/color]>`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`Use [bgcolor=#eee] data-tooltip-* [/bgcolor] to override the defaults:`}</p>
        <Widget className="mdx-table">
          <Widget className="mdx-table-row mdx-table-head">
            <Widget className="mdx-table-cell mdx-table-head-cell" style={{ width: "240px" }}>
              <Text>{`Attribute`}</Text>
            </Widget>
            <Widget className="mdx-table-cell mdx-table-head-cell" style={{ width: "240px" }}>
              <Text>{`Option`}</Text>
            </Widget>
            <Widget className="mdx-table-cell mdx-table-head-cell" style={{ width: "88px" }}>
              <Text>{`Default`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`[bgcolor=#eee] data-tooltip [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`Hint text (required)`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "88px" }}>
              <Text>{`-`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`[bgcolor=#eee] data-tooltip-side [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`top / right / bottom / left`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "88px" }}>
              <Text>{`[bgcolor=#eee] top [/bgcolor]`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`[bgcolor=#eee] data-tooltip-align [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`start / center / end`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "88px" }}>
              <Text>{`[bgcolor=#eee] center [/bgcolor]`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`[bgcolor=#eee] data-tooltip-side-offset [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`Side offset (px)`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "88px" }}>
              <Text>{`[bgcolor=#eee] 4 [/bgcolor]`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`[bgcolor=#eee] data-tooltip-align-offset [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`Align offset (px)`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "88px" }}>
              <Text>{`[bgcolor=#eee] 0 [/bgcolor]`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`[bgcolor=#eee] data-tooltip-delay [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`Show delay (ms)`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "88px" }}>
              <Text>{`[bgcolor=#eee] 0 [/bgcolor]`}</Text>
            </Widget>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/tooltip/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "184px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]Button[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  data-tooltip=[color=#0a3069]"Add to library"[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  data-tooltip-side=[color=#0a3069]"left"[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  data-tooltip-side-offset=[color=#0a3069]"8"[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  data-tooltip-delay=[color=#0a3069]"200"[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`>`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#8250df]Hover[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`</[color=#8250df]Button[/color]>`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <ul>
          <li><Text>{`The text, side, offset, and delay are read from the attributes again every time the tooltip is shown.`}</Text></li>
          <li><Text>{`An empty [bgcolor=#eee] data-tooltip [/bgcolor] string does not show the tooltip.`}</Text></li>
          <li><Text>{`Widgets inside an explicit [bgcolor=#eee] Tooltip [/bgcolor] structure do not trigger attribute mode again.`}</Text></li>
          <li><Text>{`When the trigger and content need to live in different subtrees, link them with [bgcolor=#eee] data-tooltip-trigger="key" [/bgcolor] and [bgcolor=#eee] data-tooltip-target="key" [/bgcolor].`}</Text></li>
        </ul>
        <h3>C convenience API</h3>
        <p>{`The C side does not need to write attributes by hand:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/widgets/tooltip/4">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`ui_tooltip_attach(button, [color=#0a3069]"Add to library"[/color]);`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`To adjust the side, offset, or delay:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/widgets/tooltip/5">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "184px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#953800]ui_tooltip_options_t[/color] options;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`ui_tooltip_options_init(&options);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`options.content = [color=#0a3069]"Add to library"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`options.side = UI_SIDE_RIGHT;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`options.side_offset = [color=#0550ae]8[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`options.delay = [color=#0550ae]200[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`ui_tooltip_attach_ex(button, &options);`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`[bgcolor=#eee] ui_tooltip_options_init() [/bgcolor] fills in the defaults (top, center, side offset 4, align offset 0, delay 0), so fields left untouched keep the default behavior.`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>Composition</h2>
        <p>{`Use the following composition to build a [bgcolor=#eee] Tooltip [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/tooltip/6">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "84px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`Tooltip`}</Text>
                <Text className="code-line">{`├── TooltipTrigger`}</Text>
                <Text className="code-line">{`└── TooltipContent (Portal)`}</Text>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Examples</h2>
        <h3>Sides</h3>
        <p>{`Use the [bgcolor=#eee] side [/bgcolor] prop to control where the tooltip appears.`}</p>
        <TooltipSidesDemo />
        <h3>Disabled controls</h3>
        <p>{`A disabled control does not receive pointer events; wrap it in [bgcolor=#eee] TooltipTrigger [/bgcolor] to show the hint normally.`}</p>
        <TooltipDisabledDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>Tooltip</h3>
        <p>{`The tooltip root, which manages the show delay and the Portal.`}</p>
        <FieldTable fields={tooltipFields} locale="en" />
        <h3>TooltipTrigger</h3>
        <p>{`The wrapper that triggers the tooltip. Put a button, icon, or other hoverable widget inside.`}</p>
        <FieldTable fields={tooltipTriggerFields} locale="en" />
        <h3>TooltipContent</h3>
        <p>{`The tooltip bubble, which handles side, alignment, and content.`}</p>
        <FieldTable fields={tooltipContentFields} locale="en" />
      </Widget>
    </Widget>
  );
}
