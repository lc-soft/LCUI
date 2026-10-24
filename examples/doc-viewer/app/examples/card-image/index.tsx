import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function CardImageDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/card-image/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Badge[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Button[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Card[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]CardAction[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]CardDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]CardFooter[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]CardHeader[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]CardTitle[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Card[/color] [color=#0550ae]className[/color]=[color=#0a3069]"relative w-full max-w-sm pt-0"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"relative h-\\[216px] w-full rounded-t-xl bg-slate-400"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"absolute left-0 top-0 h-full w-full bg-black/35"[/color] />[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]CardHeader[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]CardTitle[/color]>[/color]Design systems meetup[color=#116329]</[color=#0550ae]CardTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]CardDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`          A practical talk on component APIs, accessibility, and shipping`}</Text>
          <Text className="code-line">{`          faster.`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]CardDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]CardAction[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Badge[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"secondary"[/color]>[/color]Featured[color=#116329]</[color=#0550ae]Badge[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]CardAction[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]CardHeader[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]CardFooter[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-full"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"primary"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          View Event`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]CardFooter[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Card[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/card-image/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]card_image_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, example_load());`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/card-image/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]card_image_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *card = ui_create_card();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *cover = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *overlay = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *header = ui_create_card_header();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *title = ui_create_card_title();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *description = ui_create_card_description();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *header_action = ui_create_card_action();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *badge = ui_create_badge();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *footer = ui_create_card_footer();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button = ui_create_button();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_add_class(card, [color=#0a3069]"card-image-demo"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(cover, [color=#0a3069]"card-image-cover"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(overlay, [color=#0a3069]"card-image-overlay"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(cover, overlay);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(title, [color=#0a3069]"Design systems meetup"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(`}</Text>
          <Text className="code-line">{`            description,`}</Text>
          <Text className="code-line">{`            [color=#0a3069]"A practical talk on component APIs, accessibility, and shipping "[/color]`}</Text>
          <Text className="code-line">{`            [color=#0a3069]"faster."[/color]);`}</Text>
          <Text className="code-line">{`        ui_badge_set_variant(badge, UI_BADGE_VARIANT_SECONDARY);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(badge, [color=#0a3069]"Featured"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(header_action, badge);`}</Text>
          <Text className="code-line">{`        ui_widget_append(header, title);`}</Text>
          <Text className="code-line">{`        ui_widget_append(header, description);`}</Text>
          <Text className="code-line">{`        ui_widget_append(header, header_action);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(footer, [color=#0a3069]"card-footer-stack"[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_text(button, [color=#0a3069]"View Event"[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_variant(button, UI_BUTTON_VARIANT_PRIMARY);`}</Text>
          <Text className="code-line">{`        ui_widget_append(footer, button);`}</Text>
          <Text className="code-line">{`        ui_widget_append(card, cover);`}</Text>
          <Text className="code-line">{`        ui_widget_append(card, header);`}</Text>
          <Text className="code-line">{`        ui_widget_append(card, footer);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, card);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/card-image/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color] ?>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]card[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-image-demo"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-image-cover"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-image-overlay"[/color]>[/color][color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]card-header[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]card-title[/color]>[/color]Design systems meetup[color=#116329]</[color=#0550ae]card-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]card-description[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]        >[/color]A practical talk on component APIs, accessibility, and shipping faster.[color=#116329]</[color=#0550ae]card-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]card-action[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]badge[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"secondary"[/color]>[/color]Featured[color=#116329]</[color=#0550ae]badge[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]card-action[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]card-header[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]card-footer[/color] [color=#0550ae]class[/color]=[color=#0a3069]"card-footer-stack"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"primary"[/color]>[/color]View Event[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]card-footer[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]card[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/card-image/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]card_image_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, ui_create_widget([color=#0550ae]NULL[/color]));`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
