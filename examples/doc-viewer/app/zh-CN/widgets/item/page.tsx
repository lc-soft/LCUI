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
      <p className="doc-page-desc">{`用于设置、列表和快捷操作的通用内容行容器。`}</p>
      <ItemBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：设置项、资源列表和带操作的内容行`}</Text></li>
          <li><Text>{`[b]适用[/b]：组合媒体、标题、说明和尾部操作`}</Text></li>
          <li><Text>{`[b]不适用[/b]：带选中值和键盘选择逻辑的菜单项`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/item/0">
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
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/item/1">
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
        <h2>组合</h2>
        <p>{`使用以下组合来构建 [bgcolor=#eee] Item [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/item/2">
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
        <h2>变体</h2>
        <p>{`使用 [bgcolor=#eee] variant [/bgcolor] 属性更改条目的视觉样式。`}</p>
        <ItemVariantsDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>尺寸</h2>
        <p>{`使用 [bgcolor=#eee] size [/bgcolor] 属性更改条目尺寸，可选值为 [bgcolor=#eee] default [/bgcolor]、[bgcolor=#eee] sm [/bgcolor] 和 [bgcolor=#eee] xs [/bgcolor]。`}</p>
        <ItemSizeDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>图标</h2>
        <p>{`使用 [bgcolor=#eee] variant="icon" [/bgcolor] 的 [bgcolor=#eee] ItemMedia [/bgcolor] 显示图标。`}</p>
        <ItemIconDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>图片</h2>
        <p>{`使用 [bgcolor=#eee] variant="image" [/bgcolor] 的 [bgcolor=#eee] ItemMedia [/bgcolor] 显示图片。`}</p>
        <ItemImageDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>Item</h3>
        <p>{`内容行容器。`}</p>
        <FieldTable fields={itemFields} locale="zh-CN" />
        <h3>ItemMedia</h3>
        <p>{`行首的图标或缩略图槽位。`}</p>
        <FieldTable fields={itemMediaFields} locale="zh-CN" />
        <h3>ItemContent</h3>
        <p>{`标题和说明的可伸缩内容槽位。`}</p>
        <FieldTable fields={itemContentFields} locale="zh-CN" />
        <h3>ItemTitle</h3>
        <p>{`内容行标题。`}</p>
        <FieldTable fields={itemTitleFields} locale="zh-CN" />
        <h3>ItemDescription</h3>
        <p>{`内容行补充说明。`}</p>
        <FieldTable fields={itemDescriptionFields} locale="zh-CN" />
        <h3>ItemActions</h3>
        <p>{`行尾操作的布局槽位。`}</p>
        <FieldTable fields={itemActionsFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
