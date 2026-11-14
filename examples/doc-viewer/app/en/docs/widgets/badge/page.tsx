import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import BadgeBasicDemo from "../../../../examples/badge-basic/index";
import BadgeVariantsDemo from "../../../../examples/badge-variants/index";
import BadgeIconDemo from "../../../../examples/badge-icon/index";
import BadgeColorsDemo from "../../../../examples/badge-colors/index";
import FieldTable from "../../../../components/field-table";
import { badgeFields } from "../../../../widget-fields/badge";

export default function BadgeDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Badge</h1>
      <p className="doc-page-desc">{`A lightweight text widget for displaying short labels, statuses, and counts.`}</p>
      <BadgeBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: marking a status or category in lists, cards, and forms`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: showing a brief count or hint label`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: command controls that need to be clicked (use [bgcolor=#7f7f7f29] button [/bgcolor] instead)`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/badge/0">
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
                    <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Badge[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] { [color=#d2a8ff]Badge[/color] } [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/badge/1">
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
                    <Text className="code-line">{`<[color=#8250df]Badge[/color] variant=[color=#0a3069]"secondary"[/color]>[color=#8250df]Secondary[/color]</[color=#8250df]Badge[/color]>`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]Badge[/color] variant=[color=#a5d6ff]"secondary"[/color]>[color=#d2a8ff]Secondary[/color]</[color=#d2a8ff]Badge[/color]>`}</Text>
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
        <p>{`Use the [bgcolor=#7f7f7f29] variant [/bgcolor] prop to switch between the preset badge appearances.`}</p>
        <BadgeVariantsDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>With Icon</h2>
        <p>{`Combining Icon and Text child widgets lets an icon and a label express a status or category together. Place the icon before the text for a leading icon, or after it for a trailing icon. Badge lays its children out in a row, keeps a fixed 20px height, and constrains the icon's font size to match the text, so no extra spacing or size classes are needed.`}</p>
        <BadgeIconDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Custom Colors</h2>
        <p>{`Use the [bgcolor=#7f7f7f29] color [/bgcolor] prop or [bgcolor=#7f7f7f29] ui_badge_set_color() [/bgcolor] to apply a named color.`}</p>
        <BadgeColorsDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <FieldTable fields={badgeFields} locale="en" />
      </Widget>
    </Widget>
  );
}
