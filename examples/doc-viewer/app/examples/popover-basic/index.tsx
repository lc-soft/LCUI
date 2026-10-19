import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function PopoverBasicDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/popover-basic/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Button[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Field[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldLabel[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Popover[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]PopoverContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]PopoverDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]PopoverHeader[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]PopoverTitle[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]PopoverTrigger[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]TextInput[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Popover[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]PopoverTrigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]Open popover[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]PopoverTrigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]PopoverContent[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-80"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]PopoverHeader[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]PopoverTitle[/color]>[/color]Dimensions[color=#116329]</[color=#0550ae]PopoverTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]PopoverDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`            Set the dimensions for the layer.`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]PopoverDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]PopoverHeader[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"popover-form-grid clip-safe"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"popover-width"[/color]>[/color]Width[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]TextInput[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]id[/color]=[color=#0a3069]"popover-width"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]className[/color]=[color=#0a3069]"popover-form-input"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]defaultValue[/color]=[color=#0a3069]"100%"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]            />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"popover-max-width"[/color]>[/color]Max. width[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]TextInput[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]id[/color]=[color=#0a3069]"popover-max-width"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]className[/color]=[color=#0a3069]"popover-form-input"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]defaultValue[/color]=[color=#0a3069]"300px"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]            />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"popover-height"[/color]>[/color]Height[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]TextInput[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]id[/color]=[color=#0a3069]"popover-height"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]className[/color]=[color=#0a3069]"popover-form-input"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]defaultValue[/color]=[color=#0a3069]"25px"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]            />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"popover-max-height"[/color]>[/color]Max. height[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]TextInput[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]id[/color]=[color=#0a3069]"popover-max-height"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]className[/color]=[color=#0a3069]"popover-form-input"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]defaultValue[/color]=[color=#0a3069]"none"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]            />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]PopoverContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Popover[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/popover-basic/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]popover_basic_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, example_load());`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/popover-basic/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]make_field[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *id, [color=#953800]const[/color] [color=#953800]char[/color] *label_text,[/color]`}</Text>
          <Text className="code-line">{`[color=#0550ae]                               [color=#953800]const[/color] [color=#953800]char[/color] *value)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *field = ui_create_field();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *label = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *input = ui_create_widget([color=#0a3069]"textinput"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_field_set_orientation(field, [color=#0a3069]"horizontal"[/color]);`}</Text>
          <Text className="code-line">{`        ui_label_set_for(label, id);`}</Text>
          <Text className="code-line">{`        ui_widget_set_id(input, id);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(label, label_text);`}</Text>
          <Text className="code-line">{`        ui_textinput_set_text(input, value);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(input, [color=#0a3069]"popover-form-input"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(field, label);`}</Text>
          <Text className="code-line">{`        ui_widget_append(field, input);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] field;`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]popover_basic_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *popover = ui_create_popover();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *form;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_popover_build_children(popover);`}</Text>
          <Text className="code-line">{`        ui_popover_set_trigger_text(popover, [color=#0a3069]"Open popover"[/color]);`}</Text>
          <Text className="code-line">{`        ui_popover_set_title(popover, [color=#0a3069]"Dimensions"[/color]);`}</Text>
          <Text className="code-line">{`        ui_popover_set_description(popover,`}</Text>
          <Text className="code-line">{`                                   [color=#0a3069]"Set the dimensions for the layer."[/color]);`}</Text>
          <Text className="code-line">{`        content = ui_popover_get_content(popover);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(content, [color=#0a3069]"w-80"[/color]);`}</Text>
          <Text className="code-line">{`        form = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(form, [color=#0a3069]"popover-form-grid"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(form, [color=#0a3069]"clip-safe"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(form, make_field([color=#0a3069]"popover-width"[/color], [color=#0a3069]"Width"[/color], [color=#0a3069]"100%"[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(`}</Text>
          <Text className="code-line">{`            form, make_field([color=#0a3069]"popover-max-width"[/color], [color=#0a3069]"Max. width"[/color], [color=#0a3069]"300px"[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(form, make_field([color=#0a3069]"popover-height"[/color], [color=#0a3069]"Height"[/color], [color=#0a3069]"25px"[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(`}</Text>
          <Text className="code-line">{`            form, make_field([color=#0a3069]"popover-max-height"[/color], [color=#0a3069]"Max. height"[/color], [color=#0a3069]"none"[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, form);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, popover);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/popover-basic/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color] ?>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]popover[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]popover-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]Open popover[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]popover-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]popover-content[/color] [color=#0550ae]class[/color]=[color=#0a3069]"w-80"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]popover-header[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]popover-title[/color]>[/color]Dimensions[color=#116329]</[color=#0550ae]popover-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]popover-description[/color]>[/color]Set the dimensions for the layer.[color=#116329]</[color=#0550ae]popover-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]popover-header[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"popover-form-grid clip-safe"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"popover-width"[/color]>[/color]Width[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"popover-width"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"popover-form-input"[/color] [color=#0550ae]tabindex[/color]=[color=#0a3069]"0"[/color]>[/color]100%[color=#116329]</[color=#0550ae]textinput[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"popover-max-width"[/color]>[/color]Max. width[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"popover-max-width"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"popover-form-input"[/color] [color=#0550ae]tabindex[/color]=[color=#0a3069]"0"[/color]>[/color]300px[color=#116329]</[color=#0550ae]textinput[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"popover-height"[/color]>[/color]Height[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"popover-height"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"popover-form-input"[/color] [color=#0550ae]tabindex[/color]=[color=#0a3069]"0"[/color]>[/color]25px[color=#116329]</[color=#0550ae]textinput[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"popover-max-height"[/color]>[/color]Max. height[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"popover-max-height"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"popover-form-input"[/color] [color=#0550ae]tabindex[/color]=[color=#0a3069]"0"[/color]>[/color]none[color=#116329]</[color=#0550ae]textinput[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]popover-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]popover[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/popover-basic/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]popover_basic_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, ui_create_widget([color=#0550ae]NULL[/color]));`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
