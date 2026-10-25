import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import ItemBasicDemo from "../../../examples/item-basic/index";
import ItemVariantsDemo from "../../../examples/item-variants/index";
import ItemSizeDemo from "../../../examples/item-size/index";
import ItemIconDemo from "../../../examples/item-icon/index";
import ItemImageDemo from "../../../examples/item-image/index";
import FieldTable from "../../../components/field-table";
import { itemFields } from "../../../widget-fields/item";
import { itemMediaFields } from "../../../widget-fields/item-media";
import { itemContentFields } from "../../../widget-fields/item-content";
import { itemTitleFields } from "../../../widget-fields/item-title";
import { itemDescriptionFields } from "../../../widget-fields/item-description";
import { itemActionsFields } from "../../../widget-fields/item-actions";

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
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Item[/color], [color=#8250df]ItemActions[/color], [color=#8250df]ItemContent[/color], [color=#8250df]ItemDescription[/color], [color=#8250df]ItemMedia[/color], [color=#8250df]ItemTitle[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
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
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Composition</h2>
        <p>{`Use the following composition to build an [bgcolor=#eee] Item [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/item/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "144px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`Item`}</Text>
                <Text className="code-line">{`├── ItemMedia`}</Text>
                <Text className="code-line">{`├── ItemContent`}</Text>
                <Text className="code-line">{`│   ├── ItemTitle`}</Text>
                <Text className="code-line">{`│   └── ItemDescription`}</Text>
                <Text className="code-line">{`└── ItemActions`}</Text>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Variants</h2>
        <p>{`Use the [bgcolor=#eee] variant [/bgcolor] prop to change the row's visual style.`}</p>
        <ItemVariantsDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Size</h2>
        <p>{`Use the [bgcolor=#eee] size [/bgcolor] prop to change the row size; the available values are [bgcolor=#eee] default [/bgcolor], [bgcolor=#eee] sm [/bgcolor], and [bgcolor=#eee] xs [/bgcolor].`}</p>
        <ItemSizeDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Icon</h2>
        <p>{`Use an [bgcolor=#eee] ItemMedia [/bgcolor] with [bgcolor=#eee] variant="icon" [/bgcolor] to show an icon.`}</p>
        <ItemIconDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Image</h2>
        <p>{`Use an [bgcolor=#eee] ItemMedia [/bgcolor] with [bgcolor=#eee] variant="image" [/bgcolor] to show an image.`}</p>
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
