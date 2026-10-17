import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function FieldBasicDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/field-basic/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Button[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Checkbox[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Field[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldGroup[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldLabel[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldLegend[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldSeparator[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldSet[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Select[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]SelectContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]SelectGroup[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]SelectItem[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]SelectTrigger[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]SelectValue[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]TextInput[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]const[/color] months = \\[`}</Text>
          <Text className="code-line">{`  [color=#0a3069]"01"[/color],`}</Text>
          <Text className="code-line">{`  [color=#0a3069]"02"[/color],`}</Text>
          <Text className="code-line">{`  [color=#0a3069]"03"[/color],`}</Text>
          <Text className="code-line">{`  [color=#0a3069]"04"[/color],`}</Text>
          <Text className="code-line">{`  [color=#0a3069]"05"[/color],`}</Text>
          <Text className="code-line">{`  [color=#0a3069]"06"[/color],`}</Text>
          <Text className="code-line">{`  [color=#0a3069]"07"[/color],`}</Text>
          <Text className="code-line">{`  [color=#0a3069]"08"[/color],`}</Text>
          <Text className="code-line">{`  [color=#0a3069]"09"[/color],`}</Text>
          <Text className="code-line">{`  [color=#0a3069]"10"[/color],`}</Text>
          <Text className="code-line">{`  [color=#0a3069]"11"[/color],`}</Text>
          <Text className="code-line">{`  [color=#0a3069]"12"[/color],`}</Text>
          <Text className="code-line">{`];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]const[/color] years = \\[[color=#0a3069]"2024"[/color], [color=#0a3069]"2025"[/color], [color=#0a3069]"2026"[/color], [color=#0a3069]"2027"[/color], [color=#0a3069]"2028"[/color], [color=#0a3069]"2029"[/color]];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-col gap-5 max-w-\\[448px] p-1 -m-1"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldLegend[/color]>[/color]Payment Method[color=#116329]</[color=#0550ae]FieldLegend[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`          All transactions are secure and encrypted`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"name-on-card"[/color]>[/color]Name on Card[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]TextInput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"name-on-card"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"Evil Rabbit"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"card-number"[/color]>[/color]Card Number[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]TextInput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-number"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"1234 5678 9012 3456"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]Enter your 16-digit card number[color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex gap-4"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex-1"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"card-month"[/color]>[/color]Month[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Select[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]SelectTrigger[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-month"[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]SelectValue[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"MM"[/color] />[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]</[color=#0550ae]SelectTrigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    {months.map((item) => (`}</Text>
          <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]key[/color]=[color=#0a3069]{item}[/color] [color=#0550ae]value[/color]=[color=#0a3069]{item}[/color]>[/color]`}</Text>
          <Text className="code-line">{`                        {item}`}</Text>
          <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    ))}`}</Text>
          <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]</[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]Select[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex-1"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"card-year"[/color]>[/color]Year[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Select[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]SelectTrigger[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-year"[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]SelectValue[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"YYYY"[/color] />[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]</[color=#0550ae]SelectTrigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    {years.map((item) => (`}</Text>
          <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]key[/color]=[color=#0a3069]{item}[/color] [color=#0550ae]value[/color]=[color=#0a3069]{item}[/color]>[/color]`}</Text>
          <Text className="code-line">{`                        {item}`}</Text>
          <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    ))}`}</Text>
          <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]</[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]Select[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex-1"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"card-cvv"[/color]>[/color]CVV[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]TextInput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-cvv"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"123"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldSeparator[/color] />[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldLegend[/color]>[/color]Billing Address[color=#116329]</[color=#0550ae]FieldLegend[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`          The billing address associated with your payment method`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"billing-same"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"billing-same"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"font-normal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              Same as shipping address`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"primary"[/color]>[/color]Submit[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Button[/color]>[/color]Cancel[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/field-basic/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * docs/examples/field-basic-tsx/main.c: -- Field widget demo (TSX variant)[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] *[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * SPDX-License-Identifier: MIT[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] */[/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]field_basic_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        example_load(parent);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/field-basic/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * docs/examples/field-basic/main.c: -- Field widget demo (C variant)[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] *[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * SPDX-License-Identifier: MIT[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] */[/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]const[/color] [color=#953800]char[/color] *month_items\\[[color=#0550ae]12[/color]] = { [color=#0a3069]"01"[/color], [color=#0a3069]"02"[/color], [color=#0a3069]"03"[/color], [color=#0a3069]"04"[/color], [color=#0a3069]"05"[/color], [color=#0a3069]"06"[/color],`}</Text>
          <Text className="code-line">{`                                       [color=#0a3069]"07"[/color], [color=#0a3069]"08"[/color], [color=#0a3069]"09"[/color], [color=#0a3069]"10"[/color], [color=#0a3069]"11"[/color], [color=#0a3069]"12"[/color] };`}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]const[/color] [color=#953800]char[/color] *year_items\\[[color=#0550ae]6[/color]] = { [color=#0a3069]"2024"[/color], [color=#0a3069]"2025"[/color], [color=#0a3069]"2026"[/color],`}</Text>
          <Text className="code-line">{`                                     [color=#0a3069]"2027"[/color], [color=#0a3069]"2028"[/color], [color=#0a3069]"2029"[/color] };`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#6e7781]/* Build a select and point the field label at its real trigger button. */[/color]`}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_select[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *id, [color=#953800]const[/color] [color=#953800]char[/color] *placeholder,[/color]`}</Text>
          <Text className="code-line">{`[color=#0550ae]                                  [color=#953800]const[/color] [color=#953800]char[/color] *[color=#953800]const[/color] *items, [color=#953800]int[/color] count)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *select = ui_create_select();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button;`}</Text>
          <Text className="code-line">{`        [color=#953800]int[/color] i;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_select_set_placeholder(select, placeholder);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]for[/color] (i = [color=#0550ae]0[/color]; i < count; ++i) {`}</Text>
          <Text className="code-line">{`                ui_select_add_item(select, items\\[i], items\\[i]);`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`        button = ui_widget_get_child(ui_widget_get_child(select, [color=#0550ae]0[/color]), [color=#0550ae]0[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_id(button, id);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] select;`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]field_basic_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *outer;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *set1, *legend1, *desc1, *group1;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *name_field, *name_lbl, *name_input;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *card_field, *card_lbl, *card_input, *card_desc;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *card_row;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *month_field, *month_lbl, *month_select;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *year_field, *year_lbl, *year_select;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *cvv_field, *cvv_lbl, *cvv_input;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *sep;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *set2, *legend2, *desc2, *group2;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *same_field, *same_cb, *same_lbl;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *actions, *submit, *cancel;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        outer = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        set1 = ui_create_field_set();`}</Text>
          <Text className="code-line">{`        legend1 = ui_create_field_legend();`}</Text>
          <Text className="code-line">{`        desc1 = ui_create_field_description();`}</Text>
          <Text className="code-line">{`        group1 = ui_create_field_group();`}</Text>
          <Text className="code-line">{`        name_field = ui_create_field();`}</Text>
          <Text className="code-line">{`        name_lbl = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        name_input = ui_create_widget([color=#0a3069]"textinput"[/color]);`}</Text>
          <Text className="code-line">{`        card_field = ui_create_field();`}</Text>
          <Text className="code-line">{`        card_lbl = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        card_input = ui_create_widget([color=#0a3069]"textinput"[/color]);`}</Text>
          <Text className="code-line">{`        card_desc = ui_create_field_description();`}</Text>
          <Text className="code-line">{`        card_row = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        month_field = ui_create_field();`}</Text>
          <Text className="code-line">{`        month_lbl = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        year_field = ui_create_field();`}</Text>
          <Text className="code-line">{`        year_lbl = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        cvv_field = ui_create_field();`}</Text>
          <Text className="code-line">{`        cvv_lbl = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        cvv_input = ui_create_widget([color=#0a3069]"textinput"[/color]);`}</Text>
          <Text className="code-line">{`        sep = ui_create_field_separator();`}</Text>
          <Text className="code-line">{`        set2 = ui_create_field_set();`}</Text>
          <Text className="code-line">{`        legend2 = ui_create_field_legend();`}</Text>
          <Text className="code-line">{`        desc2 = ui_create_field_description();`}</Text>
          <Text className="code-line">{`        group2 = ui_create_field_group();`}</Text>
          <Text className="code-line">{`        same_field = ui_create_field();`}</Text>
          <Text className="code-line">{`        same_cb = ui_create_checkbox();`}</Text>
          <Text className="code-line">{`        same_lbl = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(same_lbl, [color=#0a3069]"font-normal"[/color]);`}</Text>
          <Text className="code-line">{`        actions = ui_create_field();`}</Text>
          <Text className="code-line">{`        submit = ui_create_widget([color=#0a3069]"button"[/color]);`}</Text>
          <Text className="code-line">{`        cancel = ui_create_widget([color=#0a3069]"button"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_add_class(outer, [color=#0a3069]"field-demo"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(outer, [color=#0a3069]"field-basic-demo"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_text_set_content(legend1, [color=#0a3069]"Payment Method"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(set1, legend1);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(desc1, [color=#0a3069]"All transactions are secure and encrypted"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(set1, desc1);`}</Text>
          <Text className="code-line">{`        ui_widget_append(set1, group1);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_label_set_for(name_lbl, [color=#0a3069]"name-on-card"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(name_lbl, [color=#0a3069]"Name on Card"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(name_field, name_lbl);`}</Text>
          <Text className="code-line">{`        ui_textinput_set_placeholder(name_input, [color=#0a3069]"Evil Rabbit"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_id(name_input, [color=#0a3069]"name-on-card"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(name_field, name_input);`}</Text>
          <Text className="code-line">{`        ui_widget_append(group1, name_field);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_label_set_for(card_lbl, [color=#0a3069]"card-number"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(card_lbl, [color=#0a3069]"Card Number"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(card_field, card_lbl);`}</Text>
          <Text className="code-line">{`        ui_textinput_set_placeholder(card_input, [color=#0a3069]"1234 5678 9012 3456"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_id(card_input, [color=#0a3069]"card-number"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(card_field, card_input);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(card_desc, [color=#0a3069]"Enter your 16-digit card number"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(card_field, card_desc);`}</Text>
          <Text className="code-line">{`        ui_widget_append(group1, card_field);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_add_class(card_row, [color=#0a3069]"field-basic-card-row"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(month_field, [color=#0a3069]"field-basic-card-field"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(year_field, [color=#0a3069]"field-basic-card-field"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(cvv_field, [color=#0a3069]"field-basic-card-field"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_label_set_for(month_lbl, [color=#0a3069]"card-month"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(month_lbl, [color=#0a3069]"Month"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(month_field, month_lbl);`}</Text>
          <Text className="code-line">{`        month_select = create_select([color=#0a3069]"card-month"[/color], [color=#0a3069]"MM"[/color], month_items, [color=#0550ae]12[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(month_field, month_select);`}</Text>
          <Text className="code-line">{`        ui_widget_append(card_row, month_field);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_label_set_for(year_lbl, [color=#0a3069]"card-year"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(year_lbl, [color=#0a3069]"Year"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(year_field, year_lbl);`}</Text>
          <Text className="code-line">{`        year_select = create_select([color=#0a3069]"card-year"[/color], [color=#0a3069]"YYYY"[/color], year_items, [color=#0550ae]6[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(year_field, year_select);`}</Text>
          <Text className="code-line">{`        ui_widget_append(card_row, year_field);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_label_set_for(cvv_lbl, [color=#0a3069]"card-cvv"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(cvv_lbl, [color=#0a3069]"CVV"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(cvv_field, cvv_lbl);`}</Text>
          <Text className="code-line">{`        ui_textinput_set_placeholder(cvv_input, [color=#0a3069]"123"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_id(cvv_input, [color=#0a3069]"card-cvv"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(cvv_field, cvv_input);`}</Text>
          <Text className="code-line">{`        ui_widget_append(card_row, cvv_field);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(group1, card_row);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(outer, set1);`}</Text>
          <Text className="code-line">{`        ui_widget_append(outer, sep);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_text_set_content(legend2, [color=#0a3069]"Billing Address"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(set2, legend2);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(`}</Text>
          <Text className="code-line">{`            desc2, [color=#0a3069]"The billing address associated with your payment method"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(set2, desc2);`}</Text>
          <Text className="code-line">{`        ui_widget_append(set2, group2);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_field_set_orientation(same_field, [color=#0a3069]"horizontal"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_id(same_cb, [color=#0a3069]"billing-same"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(same_field, same_cb);`}</Text>
          <Text className="code-line">{`        ui_label_set_for(same_lbl, [color=#0a3069]"billing-same"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(same_lbl, [color=#0a3069]"Same as shipping address"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(same_field, same_lbl);`}</Text>
          <Text className="code-line">{`        ui_widget_append(group2, same_field);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(outer, set2);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_field_set_orientation(actions, [color=#0a3069]"horizontal"[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_text(submit, [color=#0a3069]"Submit"[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_variant(submit, UI_BUTTON_VARIANT_PRIMARY);`}</Text>
          <Text className="code-line">{`        ui_widget_append(actions, submit);`}</Text>
          <Text className="code-line">{`        ui_button_set_text(cancel, [color=#0a3069]"Cancel"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(actions, cancel);`}</Text>
          <Text className="code-line">{`        ui_widget_append(outer, actions);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, outer);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/field-basic/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color] ?>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"field-demo field-basic-demo"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-legend[/color]>[/color]Payment Method[color=#116329]</[color=#0550ae]field-legend[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-description[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]        >[/color]All transactions are secure and encrypted[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"name-on-card"[/color]>[/color]Name on Card[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"name-on-card"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"Evil Rabbit"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"card-number"[/color]>[/color]Card Number[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-number"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"1234 5678 9012 3456"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-description[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]            >[/color]Enter your 16-digit card number[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"field-basic-card-row"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]class[/color]=[color=#0a3069]"field-basic-card-field"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"card-month"[/color]>[/color]Month[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]select[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]select-trigger[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-month"[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]select-value[/color] [color=#0550ae]data-select-placeholder[/color]=[color=#0a3069]"MM"[/color] />[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]</[color=#0550ae]select-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]select-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"01"[/color]>[/color]01[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"02"[/color]>[/color]02[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"03"[/color]>[/color]03[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"04"[/color]>[/color]04[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"05"[/color]>[/color]05[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"06"[/color]>[/color]06[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"07"[/color]>[/color]07[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"08"[/color]>[/color]08[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"09"[/color]>[/color]09[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"10"[/color]>[/color]10[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"11"[/color]>[/color]11[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"12"[/color]>[/color]12[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]select-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]</[color=#0550ae]portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]select[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]class[/color]=[color=#0a3069]"field-basic-card-field"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"card-year"[/color]>[/color]Year[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]select[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]select-trigger[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-year"[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]select-value[/color] [color=#0550ae]data-select-placeholder[/color]=[color=#0a3069]"YYYY"[/color] />[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]</[color=#0550ae]select-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]select-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"2024"[/color]>[/color]2024[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"2025"[/color]>[/color]2025[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"2026"[/color]>[/color]2026[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"2027"[/color]>[/color]2027[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"2028"[/color]>[/color]2028[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"2029"[/color]>[/color]2029[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]select-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]</[color=#0550ae]portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]select[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]class[/color]=[color=#0a3069]"field-basic-card-field"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"card-cvv"[/color]>[/color]CVV[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-cvv"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"123"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-separator[/color] />[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-legend[/color]>[/color]Billing Address[color=#116329]</[color=#0550ae]field-legend[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-description[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]        >[/color]The billing address associated with your payment method[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"billing-same"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]class[/color]=[color=#0a3069]"font-normal"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]for[/color]=[color=#0a3069]"billing-same"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]            >[/color]Same as shipping address[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"primary"[/color]>[/color]Submit[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]button[/color]>[/color]Cancel[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/field-basic/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * docs/examples/field-basic-xml/main.c: -- Field widget demo (XML variant)[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] *[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * SPDX-License-Identifier: MIT[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] */[/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]main[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        lcui_init();`}</Text>
          <Text className="code-line">{`        ui_load_xml_file([color=#0a3069]"ui.xml"[/color]);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
