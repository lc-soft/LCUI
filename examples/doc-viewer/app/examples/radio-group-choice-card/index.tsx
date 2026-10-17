import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function RadioGroupChoiceCardDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/radio-group-choice-card/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Field[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldLabel[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldTitle[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]RadioGroup[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]RadioGroupItem[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]RadioGroup[/color] [color=#0550ae]value[/color]=[color=#0a3069]"plus"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"radio-group-demo"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"plus-plan"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]FieldContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldTitle[/color]>[/color]Plus[color=#116329]</[color=#0550ae]FieldTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`              For individuals and small teams.`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]FieldContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"plus"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"plus-plan"[/color] [color=#0550ae]checked[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"pro-plan"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]FieldContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldTitle[/color]>[/color]Pro[color=#116329]</[color=#0550ae]FieldTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]For growing businesses.[color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]FieldContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"pro"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"pro-plan"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"enterprise-plan"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]FieldContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldTitle[/color]>[/color]Enterprise[color=#116329]</[color=#0550ae]FieldTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`              For large teams and enterprises.`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]FieldContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"enterprise"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"enterprise-plan"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]RadioGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/radio-group-choice-card/6" style={{ display: "none" }}>
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
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/radio-group-choice-card/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]add_card[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *group, [color=#953800]const[/color] [color=#953800]char[/color] *id, [color=#953800]const[/color] [color=#953800]char[/color] *value,[/color]`}</Text>
          <Text className="code-line">{`[color=#0550ae]                     [color=#953800]const[/color] [color=#953800]char[/color] *title, [color=#953800]const[/color] [color=#953800]char[/color] *desc, [color=#953800]int[/color] checked)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *card = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *row = ui_create_field();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_field_content();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *title_w = ui_create_field_title();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *desc_w = ui_create_field_description();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *item = ui_create_radio_group_item();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_label_set_for(card, id);`}</Text>
          <Text className="code-line">{`        ui_field_set_orientation(row, [color=#0a3069]"horizontal"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(title_w, title);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(desc_w, desc);`}</Text>
          <Text className="code-line">{`        ui_widget_set_id(item, id);`}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(item, [color=#0a3069]"value"[/color], value);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]if[/color] (checked) {`}</Text>
          <Text className="code-line">{`                ui_widget_set_attr(item, [color=#0a3069]"checked"[/color], [color=#0a3069]"true"[/color]);`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, title_w);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, desc_w);`}</Text>
          <Text className="code-line">{`        ui_widget_append(row, content);`}</Text>
          <Text className="code-line">{`        ui_widget_append(row, item);`}</Text>
          <Text className="code-line">{`        ui_widget_append(card, row);`}</Text>
          <Text className="code-line">{`        ui_widget_append(group, card);`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]radio_group_choice_card_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *group = ui_create_radio_group();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_add_class(group, [color=#0a3069]"radio-group-demo"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(group, [color=#0a3069]"value"[/color], [color=#0a3069]"plus"[/color]);`}</Text>
          <Text className="code-line">{`        add_card(group, [color=#0a3069]"plus-plan"[/color], [color=#0a3069]"plus"[/color], [color=#0a3069]"Plus"[/color],`}</Text>
          <Text className="code-line">{`                 [color=#0a3069]"For individuals and small teams."[/color], [color=#0550ae]1[/color]);`}</Text>
          <Text className="code-line">{`        add_card(group, [color=#0a3069]"pro-plan"[/color], [color=#0a3069]"pro"[/color], [color=#0a3069]"Pro"[/color], [color=#0a3069]"For growing businesses."[/color], [color=#0550ae]0[/color]);`}</Text>
          <Text className="code-line">{`        add_card(group, [color=#0a3069]"enterprise-plan"[/color], [color=#0a3069]"enterprise"[/color], [color=#0a3069]"Enterprise"[/color],`}</Text>
          <Text className="code-line">{`                 [color=#0a3069]"For large teams and enterprises."[/color], [color=#0550ae]0[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, group);`}</Text>
          <Text className="code-line">{`        ui_radio_group_update(group);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/radio-group-choice-card/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color] ?>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]radio-group[/color] [color=#0550ae]class[/color]=[color=#0a3069]"radio-group-demo"[/color] [color=#0550ae]value[/color]=[color=#0a3069]"plus"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"plus-plan"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-title[/color]>[/color]Plus[color=#116329]</[color=#0550ae]field-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-description[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]            >[/color]For individuals and small teams.[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]radio-group-item[/color] [color=#0550ae]id[/color]=[color=#0a3069]"plus-plan"[/color] [color=#0550ae]value[/color]=[color=#0a3069]"plus"[/color] [color=#0550ae]checked[/color]=[color=#0a3069]"true"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"pro-plan"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-title[/color]>[/color]Pro[color=#116329]</[color=#0550ae]field-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-description[/color]>[/color]For growing businesses.[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]radio-group-item[/color] [color=#0550ae]id[/color]=[color=#0a3069]"pro-plan"[/color] [color=#0550ae]value[/color]=[color=#0a3069]"pro"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"enterprise-plan"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-title[/color]>[/color]Enterprise[color=#116329]</[color=#0550ae]field-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-description[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]            >[/color]For large teams and enterprises.[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]radio-group-item[/color] [color=#0550ae]id[/color]=[color=#0a3069]"enterprise-plan"[/color] [color=#0550ae]value[/color]=[color=#0a3069]"enterprise"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]radio-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/radio-group-choice-card/9" style={{ display: "none" }}>
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
