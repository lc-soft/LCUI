import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import InputGroupDemoDemo from "../../../../examples/input-group-demo/index";
import InputGroupInlineStartDemo from "../../../../examples/input-group-inline-start/index";
import InputGroupInlineEndDemo from "../../../../examples/input-group-inline-end/index";
import InputGroupBlockStartDemo from "../../../../examples/input-group-block-start/index";
import InputGroupBlockEndDemo from "../../../../examples/input-group-block-end/index";
import InputGroupIconDemo from "../../../../examples/input-group-icon/index";
import InputGroupTextDemo from "../../../../examples/input-group-text/index";
import InputGroupButtonDemo from "../../../../examples/input-group-button/index";
import InputGroupKbdDemo from "../../../../examples/input-group-kbd/index";
import InputGroupDropdownDemo from "../../../../examples/input-group-dropdown/index";
import InputGroupSpinnerDemo from "../../../../examples/input-group-spinner/index";
import InputGroupTextareaDemo from "../../../../examples/input-group-textarea/index";
import InputGroupCustomDemo from "../../../../examples/input-group-custom/index";
import FieldTable from "../../../../components/field-table";
import { inputGroupFields } from "../../../../widget-fields/input-group";
import { inputGroupAddonFields } from "../../../../widget-fields/input-group-addon";
import { inputGroupTextFields } from "../../../../widget-fields/input-group-text";
import { inputGroupKbdFields } from "../../../../widget-fields/input-group-kbd";
import { inputGroupInputFields } from "../../../../widget-fields/input-group-input";
import { inputGroupTextareaFields } from "../../../../widget-fields/input-group-textarea";
import { inputGroupButtonFields } from "../../../../widget-fields/input-group-button";

export default function InputGroupDocPage() {
  return (
    <Widget className="doc-page">
      <h1>InputGroup</h1>
      <p className="doc-page-desc">{`Combines an input, icons, helper text, buttons, and menus into a single bordered input area.`}</p>
      <InputGroupDemoDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: Search fields, amount inputs, password fields, comment boxes, and other inputs with leading/trailing icons or actions`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: Keeping an input, description, counter, unit, or submit button inside one visual container`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: A plain input without addons (use [bgcolor=#7f7f7f29] TextInput [/bgcolor] directly)`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: Multi-field form layouts (compose with [bgcolor=#7f7f7f29] Field [/bgcolor] / [bgcolor=#7f7f7f29] FieldGroup [/bgcolor])`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/input-group/0">
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
                    <Text className="code-line">{`  [color=#8250df]Icon[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#8250df]InputGroup[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#8250df]InputGroupAddon[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#8250df]InputGroupButton[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`  [color=#8250df]InputGroupInput[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`  [color=#8250df]InputGroupKbd[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`  [color=#8250df]InputGroupText[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{`  [color=#8250df]InputGroupTextarea[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">10</Text>
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
                    <Text className="code-line">{`  [color=#d2a8ff]Icon[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]InputGroup[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]InputGroupAddon[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]InputGroupButton[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]InputGroupInput[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]InputGroupKbd[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]InputGroupText[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]InputGroupTextarea[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">10</Text>
                    <Text className="code-line">{`} [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/input-group/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "144px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#8250df]InputGroup[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]InputGroupInput[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"Search..."[/color] />[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]InputGroupAddon[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"search"[/color] />[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#116329]</[color=#0550ae]InputGroupAddon[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`</[color=#8250df]InputGroup[/color]>`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]InputGroup[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]InputGroupInput[/color] [color=#79c0ff]placeholder[/color]=[color=#a5d6ff]"Search..."[/color] />[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]InputGroupAddon[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"search"[/color] />[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]InputGroupAddon[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`</[color=#d2a8ff]InputGroup[/color]>`}</Text>
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
        <p>{`Use the following composition to build an [bgcolor=#7f7f7f29] InputGroup [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/input-group/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "144px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Text className="code-line">{`InputGroup`}</Text>
                  <Text className="code-line">{`├── InputGroupInput or InputGroupTextarea`}</Text>
                  <Text className="code-line">{`├── InputGroupAddon`}</Text>
                  <Text className="code-line">{`├── InputGroupButton`}</Text>
                  <Text className="code-line">{`├── InputGroupText`}</Text>
                  <Text className="code-line">{`└── InputGroupKbd`}</Text>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Text className="code-line">{`InputGroup`}</Text>
                  <Text className="code-line">{`├── InputGroupInput or InputGroupTextarea`}</Text>
                  <Text className="code-line">{`├── InputGroupAddon`}</Text>
                  <Text className="code-line">{`├── InputGroupButton`}</Text>
                  <Text className="code-line">{`├── InputGroupText`}</Text>
                  <Text className="code-line">{`└── InputGroupKbd`}</Text>
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
        <h3>Inline start</h3>
        <p>{`Use [bgcolor=#7f7f7f29] align="inline-start" [/bgcolor] to place an icon at the start of the input.`}</p>
        <InputGroupInlineStartDemo />
        <h3>Inline end</h3>
        <p>{`Use [bgcolor=#7f7f7f29] align="inline-end" [/bgcolor] to place an icon at the end of the input.`}</p>
        <InputGroupInlineEndDemo />
        <h3>Block start</h3>
        <p>{`[bgcolor=#7f7f7f29] block-start [/bgcolor] places the addon above the input and switches the root to a column layout.`}</p>
        <InputGroupBlockStartDemo />
        <h3>Block end</h3>
        <p>{`[bgcolor=#7f7f7f29] block-end [/bgcolor] places the addon below the input, useful for units, counters, and submit buttons.`}</p>
        <InputGroupBlockEndDemo />
        <h3>Icons</h3>
        <p>{`An addon can contain only an icon, or the group can use icons at both ends.`}</p>
        <InputGroupIconDemo />
        <h3>Text</h3>
        <p>{`Use [bgcolor=#7f7f7f29] InputGroupText [/bgcolor] for currency symbols, domain suffixes, and other helper text.`}</p>
        <InputGroupTextDemo />
        <h3>Buttons</h3>
        <p>{`Place copy, favorite, or submit buttons in an addon; use [bgcolor=#7f7f7f29] icon-xs [/bgcolor] for icon-only buttons.`}</p>
        <InputGroupButtonDemo />
        <h3>Kbd</h3>
        <p>{`Use [bgcolor=#7f7f7f29] InputGroupKbd [/bgcolor] to show a keyboard shortcut.`}</p>
        <InputGroupKbdDemo />
        <h3>Dropdown menu</h3>
        <p>{`Place a [bgcolor=#7f7f7f29] DropdownMenu [/bgcolor] inside an addon to expose extra actions for the input.`}</p>
        <InputGroupDropdownDemo />
        <h3>Spinner</h3>
        <p>{`Loading states can sit before or after the input, optionally alongside helper text.`}</p>
        <InputGroupSpinnerDemo />
        <h3>Textarea</h3>
        <p>{`Multi-line input can combine with block-start and block-end addons for file names, counters, and run actions.`}</p>
        <InputGroupTextareaDemo />
        <h3>Custom control</h3>
        <p>{`Third-party or custom focusable controls can join the group's focus and state logic through [bgcolor=#7f7f7f29] ui_input_group_mark_control() [/bgcolor].`}</p>
        <InputGroupCustomDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>InputGroup</h3>
        <p>{`The input group root, providing the shared border, radius, focus frame, and height.`}</p>
        <FieldTable fields={inputGroupFields} locale="en" />
        <h3>InputGroupAddon</h3>
        <p>{`Holds icons, text, buttons, and other extras; [bgcolor=#7f7f7f29] align [/bgcolor] controls its position.`}</p>
        <FieldTable fields={inputGroupAddonFields} locale="en" />
        <h3>InputGroupText</h3>
        <p>{`A muted helper-text container inside the group.`}</p>
        <FieldTable fields={inputGroupTextFields} locale="en" />
        <h3>InputGroupKbd</h3>
        <p>{`A keyboard-shortcut container inside the group.`}</p>
        <FieldTable fields={inputGroupKbdFields} locale="en" />
        <h3>InputGroupInput</h3>
        <p>{`A [bgcolor=#7f7f7f29] TextInput [/bgcolor] with its own border and background removed, stretching to fill the group.`}</p>
        <FieldTable fields={inputGroupInputFields} locale="en" />
        <h3>InputGroupTextarea</h3>
        <p>{`A multi-line input based on the multi-line [bgcolor=#7f7f7f29] TextInput [/bgcolor].`}</p>
        <FieldTable fields={inputGroupTextareaFields} locale="en" />
        <h3>InputGroupButton</h3>
        <p>{`A compact button for the group, defaulting to [bgcolor=#7f7f7f29] ghost [/bgcolor] + [bgcolor=#7f7f7f29] xs [/bgcolor].`}</p>
        <FieldTable fields={inputGroupButtonFields} locale="en" />
      </Widget>
      <Widget className="doc-section">
        <h2>Caveats</h2>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[b]Focus and disabled states are maintained in C[/b]. LCUI has no [bgcolor=#7f7f7f29] :focus-within [/bgcolor] or [bgcolor=#7f7f7f29] :has() [/bgcolor], so the root tracks [bgcolor=#7f7f7f29] focused [/bgcolor] through bubbling [bgcolor=#7f7f7f29] focus [/bgcolor]/[bgcolor=#7f7f7f29] blur [/bgcolor] events and tracks [bgcolor=#7f7f7f29] has-disabled [/bgcolor] from its controls.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-info">
          <Text className="admonition-title">信息</Text>
          <Widget className="admonition-content">
            <p>{`[b]Addons are reordered automatically[/b]. Whether the input or an addon is declared first, the root places inline-start / block-start addons before the input and inline-end / block-end addons after it.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]Do not mix block and inline addons[/b]. If both are used, block layout wins and inline addons are laid out as rows.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]Use one input control per group[/b]. Addon click-to-focus only targets the first matching control; disabled scanning covers the whole subtree.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[b]Multi-line input is still limited[/b]. [bgcolor=#7f7f7f29] InputGroupTextarea [/bgcolor] is based on the multi-line [bgcolor=#7f7f7f29] TextInput [/bgcolor]; the XML variant shows the structure only, while the C/TSX factories enable multi-line editing.`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
