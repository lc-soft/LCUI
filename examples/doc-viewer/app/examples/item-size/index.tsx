import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function ItemSizeDemo() {
  return (
    <DemoProvider>
      <Widget $ref="preview" className="demo-preview" />
      <Widget className="code-block">
        <Widget className="code-block-header">
          <Widget className="demo-files">
            <Text className="demo-tab active" data-language="tsx" data-value="example.tsx">example.tsx</Text>
            <Text className="demo-tab" data-language="tsx" data-value="main.c" style={{ display: "none" }}>main.c</Text>
            <Text className="demo-tab" data-language="c" data-value="main.c" style={{ display: "none" }}>main.c</Text>
            <Text className="demo-tab" data-language="xml" data-value="ui.xml" style={{ display: "none" }}>ui.xml</Text>
            <Text className="demo-tab" data-language="xml" data-value="main.c" style={{ display: "none" }}>main.c</Text>
          </Widget>
          <Widget className="demo-languages">
            <Text className="demo-tab active" data-value="tsx">TSX</Text>
            <Text className="demo-tab" data-value="c">C</Text>
            <Text className="demo-tab" data-value="xml">XML</Text>
          </Widget>
        </Widget>
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/item-size/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Item[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]ItemContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]ItemDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]ItemMedia[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]ItemTitle[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Example[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex max-w-md flex-col gap-6"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Item[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]ItemMedia[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"image"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"bg-violet-400"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]ItemContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]ItemTitle[/color]>[/color]Default Size[color=#116329]</[color=#0550ae]ItemTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]ItemDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`            The standard size for most use cases.`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]ItemDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]ItemContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Item[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Item[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]ItemMedia[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"image"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"bg-violet-400"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]ItemContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]ItemTitle[/color]>[/color]Small Size[color=#116329]</[color=#0550ae]ItemTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]ItemDescription[/color]>[/color]A compact size for dense layouts.[color=#116329]</[color=#0550ae]ItemDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]ItemContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Item[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Item[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"xs"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]ItemMedia[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"image"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"bg-violet-400"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]ItemContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]ItemTitle[/color]>[/color]Extra Small Size[color=#116329]</[color=#0550ae]ItemTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]ItemDescription[/color]>[/color]The most compact size available.[color=#116329]</[color=#0550ae]ItemDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]ItemContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Item[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/item-size/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]item_size_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, example_load());`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/item-size/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_item[/color][color=#0550ae]([color=#953800]ui_item_size_t[/color] size, [color=#953800]const[/color] [color=#953800]char[/color] *title,[/color]`}</Text>
          <Text className="code-line">{`[color=#0550ae]                                [color=#953800]const[/color] [color=#953800]char[/color] *description)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *item = ui_create_item();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *media = ui_create_item_media();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_item_content();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *item_title = ui_create_item_title();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *item_desc = ui_create_item_description();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_item_set_variant(item, UI_ITEM_VARIANT_OUTLINE);`}</Text>
          <Text className="code-line">{`        ui_item_set_size(item, size);`}</Text>
          <Text className="code-line">{`        ui_item_media_set_variant(media, UI_ITEM_MEDIA_VARIANT_IMAGE);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(media, [color=#0a3069]"bg-violet-400"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(item_title, title);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(item_desc, description);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, item_title);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, item_desc);`}</Text>
          <Text className="code-line">{`        ui_widget_append(item, media);`}</Text>
          <Text className="code-line">{`        ui_widget_append(item, content);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] item;`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]item_size_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *box = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_add_class(box, [color=#0a3069]"item-size-demo"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(box,`}</Text>
          <Text className="code-line">{`                         create_item(UI_ITEM_SIZE_DEFAULT, [color=#0a3069]"Default Size"[/color],`}</Text>
          <Text className="code-line">{`                                     [color=#0a3069]"The standard size for most use cases."[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(box, create_item(UI_ITEM_SIZE_SM, [color=#0a3069]"Small Size"[/color],`}</Text>
          <Text className="code-line">{`                                          [color=#0a3069]"A compact size for dense layouts."[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(box, create_item(UI_ITEM_SIZE_XS, [color=#0a3069]"Extra Small Size"[/color],`}</Text>
          <Text className="code-line">{`                                          [color=#0a3069]"The most compact size available."[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, box);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/item-size/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]widget[/color] [color=#0550ae]class[/color]=[color=#0a3069]"item-size-demo"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]item[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]item-media[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"image"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"bg-violet-400"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]item-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]item-title[/color]>[/color]Default Size[color=#116329]</[color=#0550ae]item-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]item-description[/color]>[/color]The standard size for most use cases.[color=#116329]</[color=#0550ae]item-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]item-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]item[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]item[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]item-media[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"image"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"bg-violet-400"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]item-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]item-title[/color]>[/color]Small Size[color=#116329]</[color=#0550ae]item-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]item-description[/color]>[/color]A compact size for dense layouts.[color=#116329]</[color=#0550ae]item-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]item-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]item[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]item[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"xs"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]item-media[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"image"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"bg-violet-400"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]item-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]item-title[/color]>[/color]Extra Small Size[color=#116329]</[color=#0550ae]item-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]item-description[/color]>[/color]The most compact size available.[color=#116329]</[color=#0550ae]item-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]item-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]item[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/item-size/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]item_size_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, ui_create_widget([color=#0550ae]NULL[/color]));`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
