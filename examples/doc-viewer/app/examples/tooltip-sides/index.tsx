import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function TooltipSidesDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/tooltip-sides/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Button[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Text[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Tooltip[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]TooltipContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]TooltipTrigger[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]const[/color] sides = \\[[color=#0a3069]"left"[/color], [color=#0a3069]"top"[/color], [color=#0a3069]"bottom"[/color], [color=#0a3069]"right"[/color]] [color=#cf222e]as[/color] [color=#cf222e]const[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"tooltip-sides-demo"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      {sides.map((side) => (`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Tooltip[/color] [color=#0550ae]key[/color]=[color=#0a3069]{side}[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]TooltipTrigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"capitalize"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              {side}`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]TooltipTrigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]TooltipContent[/color] [color=#0550ae]side[/color]=[color=#0a3069]{side}[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Text[/color]>[/color]Add to library[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]TooltipContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]Tooltip[/color]>[/color]`}</Text>
          <Text className="code-line">{`      ))}`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/tooltip-sides/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]tooltip_sides_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, example_load());`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/tooltip-sides/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]add_tooltip[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent, [color=#953800]ui_tooltip_side_t[/color] side,[/color]`}</Text>
          <Text className="code-line">{`[color=#0550ae]                        [color=#953800]const[/color] [color=#953800]char[/color] *label)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button = ui_create_button();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_tooltip_options_t[/color] options;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_tooltip_options_init(&options);`}</Text>
          <Text className="code-line">{`        options.content = [color=#0a3069]"Add to library"[/color];`}</Text>
          <Text className="code-line">{`        options.side = side;`}</Text>
          <Text className="code-line">{`        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);`}</Text>
          <Text className="code-line">{`        ui_button_set_text(button, label);`}</Text>
          <Text className="code-line">{`        ui_tooltip_attach_ex(button, &options);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, button);`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]tooltip_sides_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *row = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_add_class(row, [color=#0a3069]"tooltip-sides-demo"[/color]);`}</Text>
          <Text className="code-line">{`        add_tooltip(row, UI_SIDE_LEFT, [color=#0a3069]"Left"[/color]);`}</Text>
          <Text className="code-line">{`        add_tooltip(row, UI_SIDE_TOP, [color=#0a3069]"Top"[/color]);`}</Text>
          <Text className="code-line">{`        add_tooltip(row, UI_SIDE_BOTTOM, [color=#0a3069]"Bottom"[/color]);`}</Text>
          <Text className="code-line">{`        add_tooltip(row, UI_SIDE_RIGHT, [color=#0a3069]"Right"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, row);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/tooltip-sides/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color] ?>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"tooltip-sides-demo"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]tooltip[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]tooltip-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]Left[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]tooltip-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]tooltip-content[/color] [color=#0550ae]side[/color]=[color=#0a3069]"left"[/color]>[/color][color=#116329]<[color=#0550ae]text[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]          >[/color]Add to library[color=#116329]</[color=#0550ae]text[/color]>[/color][color=#116329]</[color=#0550ae]tooltip-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]tooltip[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]tooltip[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]tooltip-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]Top[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]tooltip-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]tooltip-content[/color] [color=#0550ae]side[/color]=[color=#0a3069]"top"[/color]>[/color][color=#116329]<[color=#0550ae]text[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]          >[/color]Add to library[color=#116329]</[color=#0550ae]text[/color]>[/color][color=#116329]</[color=#0550ae]tooltip-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]tooltip[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]tooltip[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]tooltip-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]Bottom[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]tooltip-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]tooltip-content[/color] [color=#0550ae]side[/color]=[color=#0a3069]"bottom"[/color]>[/color][color=#116329]<[color=#0550ae]text[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]          >[/color]Add to library[color=#116329]</[color=#0550ae]text[/color]>[/color][color=#116329]</[color=#0550ae]tooltip-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]tooltip[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]tooltip[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]tooltip-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]Right[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]tooltip-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]tooltip-content[/color] [color=#0550ae]side[/color]=[color=#0a3069]"right"[/color]>[/color][color=#116329]<[color=#0550ae]text[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]          >[/color]Add to library[color=#116329]</[color=#0550ae]text[/color]>[/color][color=#116329]</[color=#0550ae]tooltip-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]tooltip[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/tooltip-sides/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]tooltip_sides_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, ui_create_widget([color=#0550ae]NULL[/color]));`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
