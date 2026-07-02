import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function FieldChoiceCardDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/field-choice-card/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Field[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldGroup[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldLabel[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldSet[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldTitle[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]RadioGroup[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]RadioGroupItem[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-col gap-2 max-w-\\[400px]"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"compute-environment"[/color]>[/color]Compute Environment[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`            Select the compute environment for your cluster.`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]RadioGroup[/color] [color=#0550ae]defaultValue[/color]=[color=#0a3069]"kubernetes"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"kubernetes-r2h"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]FieldContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]FieldTitle[/color]>[/color]Kubernetes[color=#116329]</[color=#0550ae]FieldTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    Run GPU workloads on a K8s configured cluster.`}</Text>
          <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]</[color=#0550ae]FieldContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"kubernetes"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"kubernetes-r2h"[/color] />[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"vm-z4k"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]FieldContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]FieldTitle[/color]>[/color]Virtual Machine[color=#116329]</[color=#0550ae]FieldTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`                    Access a VM configured cluster to run GPU workloads.`}</Text>
          <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]</[color=#0550ae]FieldContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"vm"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"vm-z4k"[/color] />[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]RadioGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/field-choice-card/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * docs/examples/field-choice-card-tsx/main.c: -- Field widget demo (TSX[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * variant)[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] *[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * SPDX-License-Identifier: MIT[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] */[/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]field_choice_card_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        example_load(parent);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/field-choice-card/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * docs/examples/field-choice-card/main.c: -- Field widget demo (C variant)[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] *[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * SPDX-License-Identifier: MIT[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] */[/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]field_choice_card_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *outer, *group, *[color=#953800]set[/color], *lbl, *desc;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *radio;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *card1, *card2;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *row1, *row2;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content1, *content2;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *title1, *title2;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *card_desc1, *card_desc2;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *item1, *item2;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        outer = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        group = ui_create_field_group();`}</Text>
          <Text className="code-line">{`        [color=#953800]set[/color] = ui_create_field_set();`}</Text>
          <Text className="code-line">{`        lbl = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        desc = ui_create_field_description();`}</Text>
          <Text className="code-line">{`        radio = ui_create_radio_group();`}</Text>
          <Text className="code-line">{`        card1 = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        card2 = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        row1 = ui_create_field();`}</Text>
          <Text className="code-line">{`        row2 = ui_create_field();`}</Text>
          <Text className="code-line">{`        content1 = ui_create_field_content();`}</Text>
          <Text className="code-line">{`        content2 = ui_create_field_content();`}</Text>
          <Text className="code-line">{`        title1 = ui_create_field_title();`}</Text>
          <Text className="code-line">{`        title2 = ui_create_field_title();`}</Text>
          <Text className="code-line">{`        card_desc1 = ui_create_field_description();`}</Text>
          <Text className="code-line">{`        card_desc2 = ui_create_field_description();`}</Text>
          <Text className="code-line">{`        item1 = ui_create_radio_group_item();`}</Text>
          <Text className="code-line">{`        item2 = ui_create_radio_group_item();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"display"[/color], [color=#0a3069]"flex"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"flex-direction"[/color], [color=#0a3069]"column"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"gap"[/color], [color=#0a3069]"8px"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"max-width"[/color], [color=#0a3069]"400px"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(group, [color=#953800]set[/color]);`}</Text>
          <Text className="code-line">{`        ui_label_set_for(lbl, [color=#0a3069]"compute-environment"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(lbl, [color=#0a3069]"Compute Environment"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append([color=#953800]set[/color], lbl);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(desc,`}</Text>
          <Text className="code-line">{`                            [color=#0a3069]"Select the compute environment for your cluster."[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append([color=#953800]set[/color], desc);`}</Text>
          <Text className="code-line">{`        ui_widget_append([color=#953800]set[/color], radio);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(radio, [color=#0a3069]"value"[/color], [color=#0a3069]"kubernetes"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_label_set_for(card1, [color=#0a3069]"kubernetes-r2h"[/color]);`}</Text>
          <Text className="code-line">{`        ui_field_set_orientation(row1, [color=#0a3069]"horizontal"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(row1, content1);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(title1, [color=#0a3069]"Kubernetes"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content1, title1);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(card_desc1,`}</Text>
          <Text className="code-line">{`                            [color=#0a3069]"Run GPU workloads on a K8s configured cluster."[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content1, card_desc1);`}</Text>
          <Text className="code-line">{`        ui_widget_set_id(item1, [color=#0a3069]"kubernetes-r2h"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(item1, [color=#0a3069]"value"[/color], [color=#0a3069]"kubernetes"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(row1, item1);`}</Text>
          <Text className="code-line">{`        ui_widget_append(card1, row1);`}</Text>
          <Text className="code-line">{`        ui_widget_append(radio, card1);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_label_set_for(card2, [color=#0a3069]"vm-z4k"[/color]);`}</Text>
          <Text className="code-line">{`        ui_field_set_orientation(row2, [color=#0a3069]"horizontal"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(row2, content2);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(title2, [color=#0a3069]"Virtual Machine"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content2, title2);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(card_desc2,`}</Text>
          <Text className="code-line">{`                            [color=#0a3069]"Access a VM configured cluster to run GPU "[/color]`}</Text>
          <Text className="code-line">{`                            [color=#0a3069]"workloads."[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content2, card_desc2);`}</Text>
          <Text className="code-line">{`        ui_widget_set_id(item2, [color=#0a3069]"vm-z4k"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(item2, [color=#0a3069]"value"[/color], [color=#0a3069]"vm"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(row2, item2);`}</Text>
          <Text className="code-line">{`        ui_widget_append(card2, row2);`}</Text>
          <Text className="code-line">{`        ui_widget_append(radio, card2);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(outer, group);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, outer);`}</Text>
          <Text className="code-line">{`        ui_radio_group_update(radio);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/field-choice-card/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color]?>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]style[/color]=[color=#0a3069]"display:flex; flex-direction:column; gap:8px; max-width:400px"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"compute-environment"[/color]>[/color]Compute Environment[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field-description[/color]>[/color]Select the compute environment for your cluster.[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]radio-group[/color] [color=#0550ae]value[/color]=[color=#0a3069]"kubernetes"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"kubernetes-r2h"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]field-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]field-title[/color]>[/color]Kubernetes[color=#116329]</[color=#0550ae]field-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]field-description[/color]>[/color]Run GPU workloads on a K8s configured cluster.[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]</[color=#0550ae]field-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]radio-group-item[/color] [color=#0550ae]id[/color]=[color=#0a3069]"kubernetes-r2h"[/color] [color=#0550ae]value[/color]=[color=#0a3069]"kubernetes"[/color] />[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"vm-z4k"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]field-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]field-title[/color]>[/color]Virtual Machine[color=#116329]</[color=#0550ae]field-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]field-description[/color]>[/color]Access a VM configured cluster to run GPU workloads.[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]</[color=#0550ae]field-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]radio-group-item[/color] [color=#0550ae]id[/color]=[color=#0a3069]"vm-z4k"[/color] [color=#0550ae]value[/color]=[color=#0a3069]"vm"[/color] />[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]radio-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/field-choice-card/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * docs/examples/field-choice-card-xml/main.c: -- Field widget demo (XML[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * variant)[/color]`}</Text>
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
