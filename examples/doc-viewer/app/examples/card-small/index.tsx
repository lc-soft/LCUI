import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function CardSmallDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/card-small/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Button[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Card[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]CardContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]CardDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]CardFooter[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]CardHeader[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]CardTitle[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Icon[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Text[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]const[/color] features = \\[`}</Text>
          <Text className="code-line">{`    [color=#0a3069]"Choose a schedule (daily, or weekly)."[/color],`}</Text>
          <Text className="code-line">{`    [color=#0a3069]"Send to channels or specific teammates."[/color],`}</Text>
          <Text className="code-line">{`    [color=#0a3069]"Include charts, tables, and key metrics."[/color],`}</Text>
          <Text className="code-line">{`  ];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Card[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-full max-w-xs"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]CardHeader[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]CardTitle[/color]>[/color]Scheduled reports[color=#116329]</[color=#0550ae]CardTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]CardDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`          Weekly snapshots. No more manual exports.`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]CardDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]CardHeader[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]CardContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-col gap-2 py-2"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          {features.map((feature) => (`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]key[/color]=[color=#0a3069]{feature}[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex gap-2"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Icon[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]                [color=#0550ae]className[/color]=[color=#0a3069]"mt-0.5 shrink-0 text-muted-foreground"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]                [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]                [color=#0550ae]size[/color]=[color=#0a3069]{16}[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              />[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex-1"[/color]>[/color]{feature}[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`          ))}`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]CardContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]CardFooter[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex-col gap-2"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-full"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"primary"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          Set up scheduled reports`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-full"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          See what[color=#0550ae]&apos;[/color]s new`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]CardFooter[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Card[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/card-small/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]card_small_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, example_load());`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/card-small/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_feature[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *content)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *row = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *icon = ui_create_icon();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_add_class(row, [color=#0a3069]"card-small-row"[/color]);`}</Text>
          <Text className="code-line">{`        ui_icon_set_name(icon, [color=#0a3069]"chevron-right"[/color]);`}</Text>
          <Text className="code-line">{`        ui_icon_set_size(icon, [color=#0550ae]16[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(icon, [color=#0a3069]"card-small-chevron"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(text, content);`}</Text>
          <Text className="code-line">{`        ui_widget_append(row, icon);`}</Text>
          <Text className="code-line">{`        ui_widget_append(row, text);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] row;`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]card_small_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *card = ui_create_card();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *header = ui_create_card_header();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *title = ui_create_card_title();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *description = ui_create_card_description();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_card_content();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *[color=#953800]list[/color] = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *footer = ui_create_card_footer();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *primary = ui_create_button();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *secondary = ui_create_button();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_add_class(card, [color=#0a3069]"card-small-demo"[/color]);`}</Text>
          <Text className="code-line">{`        ui_card_set_size(card, UI_CARD_SIZE_SM);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(title, [color=#0a3069]"Scheduled reports"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(description,`}</Text>
          <Text className="code-line">{`                            [color=#0a3069]"Weekly snapshots. No more manual exports."[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(header, title);`}</Text>
          <Text className="code-line">{`        ui_widget_append(header, description);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class([color=#953800]list[/color], [color=#0a3069]"card-small-list"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append([color=#953800]list[/color], create_feature([color=#0a3069]"Choose a schedule (daily, or "[/color]`}</Text>
          <Text className="code-line">{`                                              [color=#0a3069]"weekly)."[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(`}</Text>
          <Text className="code-line">{`            [color=#953800]list[/color], create_feature([color=#0a3069]"Send to channels or specific teammates."[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append([color=#953800]list[/color], create_feature([color=#0a3069]"Include charts, tables, and key "[/color]`}</Text>
          <Text className="code-line">{`                                              [color=#0a3069]"metrics."[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, [color=#953800]list[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(footer, [color=#0a3069]"card-footer-stack"[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_text(primary, [color=#0a3069]"Set up scheduled reports"[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_variant(primary, UI_BUTTON_VARIANT_PRIMARY);`}</Text>
          <Text className="code-line">{`        ui_button_set_size(primary, UI_BUTTON_SIZE_SM);`}</Text>
          <Text className="code-line">{`        ui_button_set_text(secondary, [color=#0a3069]"See what&#x27;s new"[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_variant(secondary, UI_BUTTON_VARIANT_OUTLINE);`}</Text>
          <Text className="code-line">{`        ui_button_set_size(secondary, UI_BUTTON_SIZE_SM);`}</Text>
          <Text className="code-line">{`        ui_widget_append(footer, primary);`}</Text>
          <Text className="code-line">{`        ui_widget_append(footer, secondary);`}</Text>
          <Text className="code-line">{`        ui_widget_append(card, header);`}</Text>
          <Text className="code-line">{`        ui_widget_append(card, content);`}</Text>
          <Text className="code-line">{`        ui_widget_append(card, footer);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, card);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/card-small/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color] ?>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]card[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-small-demo"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]card-header[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]card-title[/color]>[/color]Scheduled reports[color=#116329]</[color=#0550ae]card-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]card-description[/color]>[/color]Weekly snapshots. No more manual exports.[color=#116329]</[color=#0550ae]card-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]card-header[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]card-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-small-list"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-small-row"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-small-chevron"[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"16"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]text[/color]>[/color]Choose a schedule (daily, or weekly).[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-small-row"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-small-chevron"[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"16"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]text[/color]>[/color]Send to channels or specific teammates.[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-small-row"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-small-chevron"[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"16"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]text[/color]>[/color]Include charts, tables, and key metrics.[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]card-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]card-footer[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-footer-stack"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"primary"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]Set up scheduled reports[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]See what&#x27;s new[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]card-footer[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]card[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/card-small/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]card_small_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, ui_create_widget([color=#0550ae]NULL[/color]));`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
