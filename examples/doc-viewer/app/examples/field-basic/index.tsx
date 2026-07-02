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
          <Text className="code-line">{`  [color=#8250df]TextInput[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-col gap-2 max-w-\\[400px]"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldLegend[/color]>[/color]Payment Method[color=#116329]</[color=#0550ae]FieldLegend[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]All transactions are secure and encrypted[color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
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
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]TextInput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-month"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"MM"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex-1"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"card-year"[/color]>[/color]Year[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]TextInput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-year"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"YYYY"[/color] />[/color]`}</Text>
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
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]The billing address associated with your payment method[color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"billing-same"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"billing-same"[/color]>[/color]Same as shipping address[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Button[/color]>[/color]Submit[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
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
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]field_basic_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *outer;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *set1, *legend1, *desc1, *group1;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *name_field, *name_lbl, *name_input;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *card_field, *card_lbl, *card_input, *card_desc;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *card_row;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *month_field, *month_lbl, *month_input;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *year_field, *year_lbl, *year_input;`}</Text>
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
          <Text className="code-line">{`        month_input = ui_create_widget([color=#0a3069]"textinput"[/color]);`}</Text>
          <Text className="code-line">{`        year_field = ui_create_field();`}</Text>
          <Text className="code-line">{`        year_lbl = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        year_input = ui_create_widget([color=#0a3069]"textinput"[/color]);`}</Text>
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
          <Text className="code-line">{`        actions = ui_create_field();`}</Text>
          <Text className="code-line">{`        submit = ui_create_widget([color=#0a3069]"button"[/color]);`}</Text>
          <Text className="code-line">{`        cancel = ui_create_widget([color=#0a3069]"button"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"display"[/color], [color=#0a3069]"flex"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"flex-direction"[/color], [color=#0a3069]"column"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"gap"[/color], [color=#0a3069]"8px"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"max-width"[/color], [color=#0a3069]"400px"[/color]);`}</Text>
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
          <Text className="code-line">{`        ui_widget_set_style_string(card_row, [color=#0a3069]"display"[/color], [color=#0a3069]"flex"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(card_row, [color=#0a3069]"gap"[/color], [color=#0a3069]"16px"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(month_field, [color=#0a3069]"flex"[/color], [color=#0a3069]"1"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(year_field, [color=#0a3069]"flex"[/color], [color=#0a3069]"1"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(cvv_field, [color=#0a3069]"flex"[/color], [color=#0a3069]"1"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_label_set_for(month_lbl, [color=#0a3069]"card-month"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(month_lbl, [color=#0a3069]"Month"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(month_field, month_lbl);`}</Text>
          <Text className="code-line">{`        ui_textinput_set_placeholder(month_input, [color=#0a3069]"MM"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_id(month_input, [color=#0a3069]"card-month"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(month_field, month_input);`}</Text>
          <Text className="code-line">{`        ui_widget_append(card_row, month_field);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_label_set_for(year_lbl, [color=#0a3069]"card-year"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(year_lbl, [color=#0a3069]"Year"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(year_field, year_lbl);`}</Text>
          <Text className="code-line">{`        ui_textinput_set_placeholder(year_input, [color=#0a3069]"YYYY"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_id(year_input, [color=#0a3069]"card-year"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(year_field, year_input);`}</Text>
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
          <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color]?>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]style[/color]=[color=#0a3069]"display:flex; flex-direction:column; gap:8px; max-width:400px"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-legend[/color]>[/color]Payment Method[color=#116329]</[color=#0550ae]field-legend[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-description[/color]>[/color]All transactions are secure and encrypted[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"name-on-card"[/color]>[/color]Name on Card[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"name-on-card"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"Evil Rabbit"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"card-number"[/color]>[/color]Card Number[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-number"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"1234 5678 9012 3456"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-description[/color]>[/color]Enter your 16-digit card number[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]style[/color]=[color=#0a3069]"display:flex; gap:16px"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]style[/color]=[color=#0a3069]"flex:1"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"card-month"[/color]>[/color]Month[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-month"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"MM"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]style[/color]=[color=#0a3069]"flex:1"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"card-year"[/color]>[/color]Year[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-year"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"YYYY"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]style[/color]=[color=#0a3069]"flex:1"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"card-cvv"[/color]>[/color]CVV[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-cvv"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"123"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-separator[/color] />[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-legend[/color]>[/color]Billing Address[color=#116329]</[color=#0550ae]field-legend[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-description[/color]>[/color]The billing address associated with your payment method[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"billing-same"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"billing-same"[/color]>[/color]Same as shipping address[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]button[/color]>[/color]Submit[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
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
