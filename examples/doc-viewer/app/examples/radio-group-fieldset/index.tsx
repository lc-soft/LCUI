import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function RadioGroupFieldsetDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/radio-group-fieldset/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Field[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldLabel[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldSet[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]RadioGroup[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]RadioGroupItem[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"radio-group-demo"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldLabel[/color]>[/color]Subscription Plan[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`          Yearly and lifetime plans offer significant savings.`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]RadioGroup[/color] [color=#0550ae]value[/color]=[color=#0a3069]"monthly"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"monthly"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"plan-monthly"[/color] [color=#0550ae]checked[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"plan-monthly"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"font-normal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              Monthly (\$9.99/month)`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"yearly"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"plan-yearly"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"plan-yearly"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"font-normal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              Yearly (\$99.99/year)`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"lifetime"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"plan-lifetime"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"plan-lifetime"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"font-normal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              Lifetime (\$299.99)`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]RadioGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/radio-group-fieldset/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#6e7781]/* example.tsx is compiled to example.h by @lcui/cli */[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]main[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        lcui_init();`}</Text>
          <Text className="code-line">{`        example_load();`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/radio-group-fieldset/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]radio_group_fieldset_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *outer, *[color=#953800]set[/color], *lbl, *desc, *group;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *row\\[[color=#0550ae]3[/color]];`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *item\\[[color=#0550ae]3[/color]];`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *row_lbl\\[[color=#0550ae]3[/color]];`}</Text>
          <Text className="code-line">{`        [color=#953800]const[/color] [color=#953800]char[/color] *ids\\[[color=#0550ae]3[/color]] = { [color=#0a3069]"plan-monthly"[/color], [color=#0a3069]"plan-yearly"[/color], [color=#0a3069]"plan-lifetime"[/color] };`}</Text>
          <Text className="code-line">{`        [color=#953800]const[/color] [color=#953800]char[/color] *values\\[[color=#0550ae]3[/color]] = { [color=#0a3069]"monthly"[/color], [color=#0a3069]"yearly"[/color], [color=#0a3069]"lifetime"[/color] };`}</Text>
          <Text className="code-line">{`        [color=#953800]const[/color] [color=#953800]char[/color] *texts\\[[color=#0550ae]3[/color]] = { [color=#0a3069]"Monthly (\$9.99/month)"[/color],`}</Text>
          <Text className="code-line">{`                                 [color=#0a3069]"Yearly (\$99.99/year)"[/color], [color=#0a3069]"Lifetime (\$299.99)"[/color] };`}</Text>
          <Text className="code-line">{`        [color=#953800]int[/color] i;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        outer = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]set[/color] = ui_create_field_set();`}</Text>
          <Text className="code-line">{`        lbl = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        desc = ui_create_field_description();`}</Text>
          <Text className="code-line">{`        group = ui_create_radio_group();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        [color=#cf222e]for[/color] (i = [color=#0550ae]0[/color]; i < [color=#0550ae]3[/color]; ++i) {`}</Text>
          <Text className="code-line">{`                row\\[i] = ui_create_field();`}</Text>
          <Text className="code-line">{`                item\\[i] = ui_create_radio_group_item();`}</Text>
          <Text className="code-line">{`                row_lbl\\[i] = ui_create_field_label();`}</Text>
          <Text className="code-line">{`                ui_widget_add_class(row_lbl\\[i], [color=#0a3069]"font-normal"[/color]);`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_add_class(outer, [color=#0a3069]"radio-group-demo"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(lbl, [color=#0a3069]"Subscription Plan"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append([color=#953800]set[/color], lbl);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(desc, [color=#0a3069]"Yearly and lifetime plans offer significant "[/color]`}</Text>
          <Text className="code-line">{`                                  [color=#0a3069]"savings."[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append([color=#953800]set[/color], desc);`}</Text>
          <Text className="code-line">{`        ui_widget_append([color=#953800]set[/color], group);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        [color=#cf222e]for[/color] (i = [color=#0550ae]0[/color]; i < [color=#0550ae]3[/color]; ++i) {`}</Text>
          <Text className="code-line">{`                ui_field_set_orientation(row\\[i], [color=#0a3069]"horizontal"[/color]);`}</Text>
          <Text className="code-line">{`                ui_widget_set_id(item\\[i], ids\\[i]);`}</Text>
          <Text className="code-line">{`                ui_widget_set_attr(item\\[i], [color=#0a3069]"value"[/color], values\\[i]);`}</Text>
          <Text className="code-line">{`                ui_widget_append(row\\[i], item\\[i]);`}</Text>
          <Text className="code-line">{`                ui_label_set_for(row_lbl\\[i], ids\\[i]);`}</Text>
          <Text className="code-line">{`                ui_text_set_content(row_lbl\\[i], texts\\[i]);`}</Text>
          <Text className="code-line">{`                ui_widget_append(row\\[i], row_lbl\\[i]);`}</Text>
          <Text className="code-line">{`                ui_widget_append(group, row\\[i]);`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(group, [color=#0a3069]"value"[/color], [color=#0a3069]"monthly"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(item\\[[color=#0550ae]0[/color]], [color=#0a3069]"checked"[/color], [color=#0a3069]"true"[/color]);`}</Text>
          <Text className="code-line">{`        ui_radio_group_update(group);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(outer, [color=#953800]set[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, outer);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/radio-group-fieldset/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color] ?>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"radio-group-demo"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-label[/color]>[/color]Subscription Plan[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-description[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]        >[/color]Yearly and lifetime plans offer significant savings.[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]radio-group[/color] [color=#0550ae]value[/color]=[color=#0a3069]"monthly"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]radio-group-item[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]id[/color]=[color=#0a3069]"plan-monthly"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]value[/color]=[color=#0a3069]"monthly"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]checked[/color]=[color=#0a3069]"true"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]            />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]class[/color]=[color=#0a3069]"font-normal"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]for[/color]=[color=#0a3069]"plan-monthly"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]            >[/color]Monthly (\$9.99/month)[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]radio-group-item[/color] [color=#0550ae]id[/color]=[color=#0a3069]"plan-yearly"[/color] [color=#0550ae]value[/color]=[color=#0a3069]"yearly"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]class[/color]=[color=#0a3069]"font-normal"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]for[/color]=[color=#0a3069]"plan-yearly"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]            >[/color]Yearly (\$99.99/year)[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]radio-group-item[/color] [color=#0550ae]id[/color]=[color=#0a3069]"plan-lifetime"[/color] [color=#0550ae]value[/color]=[color=#0a3069]"lifetime"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]class[/color]=[color=#0a3069]"font-normal"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]for[/color]=[color=#0a3069]"plan-lifetime"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]            >[/color]Lifetime (\$299.99)[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]radio-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/radio-group-fieldset/9" style={{ display: "none" }}>
          <CodeBlockCopy />
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
