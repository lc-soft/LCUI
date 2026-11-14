import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import ButtonBasicDemo from "../../../../examples/button-basic/index";
import ButtonVariantsDemo from "../../../../examples/button-variants/index";
import ButtonIconDemo from "../../../../examples/button-icon/index";
import ButtonWithIconDemo from "../../../../examples/button-with-icon/index";
import ButtonSizeDemo from "../../../../examples/button-size/index";
import ButtonDisabledDemo from "../../../../examples/button-disabled/index";
import FieldTable from "../../../../components/field-table";
import { buttonFields } from "../../../../widget-fields/button";

export default function ButtonDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Button</h1>
      <p className="doc-page-desc">{`A clickable button widget, typically used to trigger an action.`}</p>
      <ButtonBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: users click to perform an action (submit a form, open a page, toggle a state)`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: interactive entry points in toolbars, dialogs, and navbars`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: non-interactive text display (use [bgcolor=#7f7f7f29] text [/bgcolor] instead)`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/button/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Button[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] { [color=#d2a8ff]Button[/color] } [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/button/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#8250df]Button[/color]>[color=#8250df]Button[/color]</[color=#8250df]Button[/color]>`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]Button[/color]>[color=#d2a8ff]Button[/color]</[color=#d2a8ff]Button[/color]>`}</Text>
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
        <h2>Variants</h2>
        <p>{`Buttons use the outline appearance by default, so there is no need to specify it. Use [bgcolor=#7f7f7f29] primary [/bgcolor] for the main action and [bgcolor=#7f7f7f29] secondary [/bgcolor] for a lower-emphasis filled action. Choose [bgcolor=#7f7f7f29] ghost [/bgcolor] for a subtle action, [bgcolor=#7f7f7f29] destructive [/bgcolor] for a dangerous action, or [bgcolor=#7f7f7f29] link [/bgcolor] for a text-style button that becomes underlined on hover.`}</p>
        <ButtonVariantsDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Icon</h2>
        <p>{`[bgcolor=#7f7f7f29] icon [/bgcolor] is a size, not a visual variant. Set [bgcolor=#7f7f7f29] size="icon" [/bgcolor] to render a compact square button, and combine it with any variant.`}</p>
        <ButtonIconDemo />
        <h3>With Icon</h3>
        <p>{`Icons and labels are laid out side by side by the button's inner flex layout. Place the icon before the text for a leading icon, or after it for a trailing icon; the gap between them is controlled by the button.`}</p>
        <ButtonWithIconDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Size</h2>
        <p>{`Use the [bgcolor=#7f7f7f29] size [/bgcolor] prop to change the button size.`}</p>
        <ButtonSizeDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Disabled</h2>
        <p>{`Setting [bgcolor=#7f7f7f29] disabled [/bgcolor] on a button lowers its opacity and stops it from responding to mouse and keyboard interaction; all variants look the same way when disabled.`}</p>
        <ButtonDisabledDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <FieldTable fields={buttonFields} locale="en" />
      </Widget>
    </Widget>
  );
}
