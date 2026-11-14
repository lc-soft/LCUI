import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import ButtonGroupDemoDemo from "../../../../examples/button-group-demo/index";
import ButtonGroupOrientationDemo from "../../../../examples/button-group-orientation/index";
import ButtonGroupSizeDemo from "../../../../examples/button-group-size/index";
import ButtonGroupNestedDemo from "../../../../examples/button-group-nested/index";
import ButtonGroupSeparatorDemo from "../../../../examples/button-group-separator/index";
import ButtonGroupSplitDemo from "../../../../examples/button-group-split/index";
import ButtonGroupInputDemo from "../../../../examples/button-group-input/index";
import ButtonGroupInputGroupDemo from "../../../../examples/button-group-input-group/index";
import ButtonGroupDropdownDemo from "../../../../examples/button-group-dropdown/index";
import ButtonGroupSelectDemo from "../../../../examples/button-group-select/index";
import ButtonGroupPopoverDemo from "../../../../examples/button-group-popover/index";
import FieldTable from "../../../../components/field-table";
import { buttonGroupFields } from "../../../../widget-fields/button-group";
import { buttonGroupTextFields } from "../../../../widget-fields/button-group-text";
import { buttonGroupSeparatorFields } from "../../../../widget-fields/button-group-separator";

export default function ButtonGroupDocPage() {
  return (
    <Widget className="doc-page">
      <h1>ButtonGroup</h1>
      <p className="doc-page-desc">{`Group related buttons, inputs or popup controls into a visually continuous button group.`}</p>
      <ButtonGroupDemoDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Use[/b]: merging adjacent actions of the same kind, such as Archive / Report / Snooze`}</Text></li>
          <li><Text>{`[b]Use[/b]: split buttons, input + action button, or Select + input + send button`}</Text></li>
          <li><Text>{`[b]Use[/b]: nesting multiple ButtonGroups to create group spacing`}</Text></li>
          <li><Text>{`[b]Don't use[/b]: switching state between several options (use ToggleGroup or RadioGroup)`}</Text></li>
          <li><Text>{`[b]Don't use[/b]: buttons that need independent spacing and must not share borders (use Button directly)`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/button-group/0">
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
                    <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#8250df]Button[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#8250df]ButtonGroup[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#8250df]ButtonGroupSeparator[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#8250df]ButtonGroupText[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
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
                    <Text className="code-line">{`  [color=#d2a8ff]Button[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]ButtonGroup[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]ButtonGroupSeparator[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#d2a8ff]ButtonGroupText[/color],`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`} [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/button-group/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "104px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#8250df]ButtonGroup[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]Button[/color]>[/color]Button 1[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]Button[/color]>[/color]Button 2[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`</[color=#8250df]ButtonGroup[/color]>`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]ButtonGroup[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]Button[/color]>[/color]Button 1[color=#7ee787]</[color=#79c0ff]Button[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]Button[/color]>[/color]Button 2[color=#7ee787]</[color=#79c0ff]Button[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`</[color=#d2a8ff]ButtonGroup[/color]>`}</Text>
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
        <p>{`Use the following composition to build a [bgcolor=#7f7f7f29] ButtonGroup [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/button-group/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "124px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Text className="code-line">{`ButtonGroup`}</Text>
                  <Text className="code-line">{`├── Button or Input or InputGroup or Select`}</Text>
                  <Text className="code-line">{`├── ButtonGroupText`}</Text>
                  <Text className="code-line">{`├── ButtonGroupSeparator`}</Text>
                  <Text className="code-line">{`└── ButtonGroup`}</Text>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Text className="code-line">{`ButtonGroup`}</Text>
                  <Text className="code-line">{`├── Button or Input or InputGroup or Select`}</Text>
                  <Text className="code-line">{`├── ButtonGroupText`}</Text>
                  <Text className="code-line">{`├── ButtonGroupSeparator`}</Text>
                  <Text className="code-line">{`└── ButtonGroup`}</Text>
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
        <h3>Orientation</h3>
        <p>{`Set [bgcolor=#7f7f7f29] orientation="vertical" [/bgcolor] to stack buttons vertically; the first/last corner radii and adjacent borders switch automatically.`}</p>
        <ButtonGroupOrientationDemo />
        <h3>Size</h3>
        <p>{`The group does not enforce a single size; each child button keeps its own [bgcolor=#7f7f7f29] size [/bgcolor].`}</p>
        <ButtonGroupSizeDemo />
        <h3>Nested</h3>
        <p>{`Nested ButtonGroups automatically get 8px spacing, which suits combinations such as an icon group plus an InputGroup.`}</p>
        <ButtonGroupNestedDemo />
        <h3>Separator</h3>
        <p>{`[bgcolor=#7f7f7f29] ButtonGroupSeparator [/bgcolor] adds a visual divider between buttons in the same group.`}</p>
        <ButtonGroupSeparatorDemo />
        <h3>Split</h3>
        <p>{`A primary button, a separator and an icon button form a split button.`}</p>
        <ButtonGroupSplitDemo />
        <h3>Input</h3>
        <p>{`An input can be a direct child of ButtonGroup and stretches with [bgcolor=#7f7f7f29] flex: 1 [/bgcolor].`}</p>
        <ButtonGroupInputDemo />
        <h3>Input Group</h3>
        <p>{`ButtonGroup can nest an InputGroup and combine it with icon buttons or other input controls.`}</p>
        <ButtonGroupInputGroupDemo />
        <h3>Dropdown Menu</h3>
        <p>{`Put a DropdownMenu inside a ButtonGroup to build a primary action plus an additional actions menu.`}</p>
        <ButtonGroupDropdownDemo />
        <h3>Select</h3>
        <p>{`A Select and an input can live in the same nested ButtonGroup, combined with a send button.`}</p>
        <ButtonGroupSelectDemo />
        <h3>Popover</h3>
        <p>{`A Popover works as an additional action entry for a ButtonGroup.`}</p>
        <ButtonGroupPopoverDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>ButtonGroup</h3>
        <p>{`Button group root. Manages the orientation and the borders/corner radii of its direct children.`}</p>
        <FieldTable fields={buttonGroupFields} locale="en" />
        <h3>ButtonGroupText</h3>
        <p>{`Text container with a muted background and a border.`}</p>
        <FieldTable fields={buttonGroupTextFields} locale="en" />
        <h3>ButtonGroupSeparator</h3>
        <p>{`A 1px divider between buttons.`}</p>
        <FieldTable fields={buttonGroupSeparatorFields} locale="en" />
      </Widget>
      <Widget className="doc-section">
        <h2>Caveats</h2>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[b]ButtonGroup is not a ToggleGroup.[/b] It only handles layout and visual merging and keeps no selection state; use RadioGroup or a future ToggleGroup for state switching.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-info">
          <Text className="admonition-title">信息</Text>
          <Widget className="admonition-content">
            <p>{`[b]Edge radii are based on direct children.[/b] The root adds an internal marker class to direct children and uses LCUI's [bgcolor=#7f7f7f29] first-child [/bgcolor] / [bgcolor=#7f7f7f29] last-child [/bgcolor] statuses to compute edge radii and adjacent borders. Buttons wrapped in an ordinary wrapper do not participate.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]Keep separators in the middle.[/b] Placing them at the start or end would consume the first/last button's corner radius.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]Horizontal separators in a vertical group must be set explicitly.[/b] [bgcolor=#7f7f7f29] ButtonGroupSeparator [/bgcolor] is vertical by default; set [bgcolor=#7f7f7f29] orientation="horizontal" [/bgcolor] inside a vertical group.`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
