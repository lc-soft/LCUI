import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import ItemBasicDemo from "../../../../examples/item-basic/index";
import ItemVariantsDemo from "../../../../examples/item-variants/index";
import ItemSizeDemo from "../../../../examples/item-size/index";
import ItemIconDemo from "../../../../examples/item-icon/index";
import ItemImageDemo from "../../../../examples/item-image/index";
import FieldTable from "../../../../components/field-table";
import { itemFields } from "../../../../widget-fields/item";
import { itemMediaFields } from "../../../../widget-fields/item-media";
import { itemContentFields } from "../../../../widget-fields/item-content";
import { itemTitleFields } from "../../../../widget-fields/item-title";
import { itemDescriptionFields } from "../../../../widget-fields/item-description";
import { itemActionsFields } from "../../../../widget-fields/item-actions";

export default function ItemDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Item</h1>
      <p className="doc-page-desc">{`A general content-row container for settings, lists, and quick actions.`}</p>
      <ItemBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: settings entries, resource lists, and content rows with actions`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: combining media, a title, a description, and trailing actions`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: menu items with a selected value and keyboard selection logic`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/item/0">
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
                    <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Item[/color], [color=#8250df]ItemActions[/color], [color=#8250df]ItemContent[/color], [color=#8250df]ItemDescription[/color], [color=#8250df]ItemMedia[/color], [color=#8250df]ItemTitle[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] { [color=#d2a8ff]Item[/color], [color=#d2a8ff]ItemActions[/color], [color=#d2a8ff]ItemContent[/color], [color=#d2a8ff]ItemDescription[/color], [color=#d2a8ff]ItemMedia[/color], [color=#d2a8ff]ItemTitle[/color] } [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/item/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "164px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#8250df]Item[/color] variant=[color=#0a3069]"outline"[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ItemContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`    [color=#116329]<[color=#0550ae]ItemTitle[/color]>[/color]Basic Item[color=#116329]</[color=#0550ae]ItemTitle[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`    [color=#116329]<[color=#0550ae]ItemDescription[/color]>[/color]A simple item with title and description.[color=#116329]</[color=#0550ae]ItemDescription[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ItemContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ItemActions[/color]>[/color][color=#116329]<[color=#0550ae]button[/color]>[/color]Action[color=#116329]</[color=#0550ae]button[/color]>[/color][color=#116329]</[color=#0550ae]ItemActions[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`</[color=#8250df]Item[/color]>`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]Item[/color] variant=[color=#a5d6ff]"outline"[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]ItemContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]ItemTitle[/color]>[/color]Basic Item[color=#7ee787]</[color=#79c0ff]ItemTitle[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]ItemDescription[/color]>[/color]A simple item with title and description.[color=#7ee787]</[color=#79c0ff]ItemDescription[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]ItemContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]ItemActions[/color]>[/color][color=#7ee787]<[color=#79c0ff]button[/color]>[/color]Action[color=#7ee787]</[color=#79c0ff]button[/color]>[/color][color=#7ee787]</[color=#79c0ff]ItemActions[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`</[color=#d2a8ff]Item[/color]>`}</Text>
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
        <p>{`Use the following composition to build an [bgcolor=#7f7f7f29] Item [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/item/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "144px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Text className="code-line">{`Item`}</Text>
                  <Text className="code-line">{`├── ItemMedia`}</Text>
                  <Text className="code-line">{`├── ItemContent`}</Text>
                  <Text className="code-line">{`│   ├── ItemTitle`}</Text>
                  <Text className="code-line">{`│   └── ItemDescription`}</Text>
                  <Text className="code-line">{`└── ItemActions`}</Text>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Text className="code-line">{`Item`}</Text>
                  <Text className="code-line">{`├── ItemMedia`}</Text>
                  <Text className="code-line">{`├── ItemContent`}</Text>
                  <Text className="code-line">{`│   ├── ItemTitle`}</Text>
                  <Text className="code-line">{`│   └── ItemDescription`}</Text>
                  <Text className="code-line">{`└── ItemActions`}</Text>
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
        <p>{`Use the [bgcolor=#7f7f7f29] variant [/bgcolor] prop to change the row's visual style.`}</p>
        <ItemVariantsDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Size</h2>
        <p>{`Use the [bgcolor=#7f7f7f29] size [/bgcolor] prop to change the row size; the available values are [bgcolor=#7f7f7f29] default [/bgcolor], [bgcolor=#7f7f7f29] sm [/bgcolor], and [bgcolor=#7f7f7f29] xs [/bgcolor].`}</p>
        <ItemSizeDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Icon</h2>
        <p>{`Use an [bgcolor=#7f7f7f29] ItemMedia [/bgcolor] with [bgcolor=#7f7f7f29] variant="icon" [/bgcolor] to show an icon.`}</p>
        <ItemIconDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Image</h2>
        <p>{`Use an [bgcolor=#7f7f7f29] ItemMedia [/bgcolor] with [bgcolor=#7f7f7f29] variant="image" [/bgcolor] to show an image.`}</p>
        <ItemImageDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>Item</h3>
        <p>{`The content-row container.`}</p>
        <FieldTable fields={itemFields} locale="en" />
        <h3>ItemMedia</h3>
        <p>{`The icon or thumbnail slot at the start of the row.`}</p>
        <FieldTable fields={itemMediaFields} locale="en" />
        <h3>ItemContent</h3>
        <p>{`The flexible content slot for the title and description.`}</p>
        <FieldTable fields={itemContentFields} locale="en" />
        <h3>ItemTitle</h3>
        <p>{`The row title.`}</p>
        <FieldTable fields={itemTitleFields} locale="en" />
        <h3>ItemDescription</h3>
        <p>{`The row's supporting description.`}</p>
        <FieldTable fields={itemDescriptionFields} locale="en" />
        <h3>ItemActions</h3>
        <p>{`The layout slot for actions at the end of the row.`}</p>
        <FieldTable fields={itemActionsFields} locale="en" />
      </Widget>
    </Widget>
  );
}
