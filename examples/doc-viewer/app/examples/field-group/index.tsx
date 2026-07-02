import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function FieldGroupDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/field-group/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Checkbox[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Field[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldGroup[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldLabel[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldSeparator[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldSet[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-col gap-2 max-w-\\[400px]"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]FieldLabel[/color]>[/color]Responses[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`            Get notified when ChatGPT responds to requests that take time,`}</Text>
          <Text className="code-line">{`            like research or image generation.`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"push"[/color] [color=#0550ae]defaultChecked[/color] [color=#0550ae]disabled[/color] />[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"push"[/color]>[/color]Push notifications[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldSeparator[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]FieldLabel[/color]>[/color]Tasks[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`            Get notified when tasks you[color=#0550ae]&apos;[/color]ve created have updates.`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"push-tasks"[/color] />[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"push-tasks"[/color]>[/color]Push notifications[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"email-tasks"[/color] />[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"email-tasks"[/color]>[/color]Email notifications[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/field-group/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * docs/examples/field-group-tsx/main.c: -- Field widget demo (TSX variant)[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] *[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * SPDX-License-Identifier: MIT[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] */[/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]field_group_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        example_load(parent);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/field-group/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * docs/examples/field-group/main.c: -- Field widget demo (C variant)[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] *[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * SPDX-License-Identifier: MIT[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] */[/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]make_check_field[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent, [color=#953800]const[/color] [color=#953800]char[/color] *id,[/color]`}</Text>
          <Text className="code-line">{`[color=#0550ae]                             [color=#953800]const[/color] [color=#953800]char[/color] *text, [color=#953800]int[/color] checked, [color=#953800]int[/color] disabled)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *row = ui_create_field();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *cb = ui_create_checkbox();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *lbl = ui_create_field_label();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_field_set_orientation(row, [color=#0a3069]"horizontal"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_id(cb, id);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]if[/color] (checked) {`}</Text>
          <Text className="code-line">{`                ui_widget_set_attr(cb, [color=#0a3069]"checked"[/color], [color=#0a3069]"true"[/color]);`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`        [color=#cf222e]if[/color] (disabled) {`}</Text>
          <Text className="code-line">{`                ui_widget_set_attr(cb, [color=#0a3069]"disabled"[/color], [color=#0a3069]"true"[/color]);`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`        ui_widget_append(row, cb);`}</Text>
          <Text className="code-line">{`        ui_label_set_for(lbl, id);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(lbl, text);`}</Text>
          <Text className="code-line">{`        ui_widget_append(row, lbl);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, row);`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]field_group_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *outer, *group;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *set1, *lbl1, *desc1, *sub1;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *set2, *lbl2, *desc2, *sub2;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *sep;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        outer = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        group = ui_create_field_group();`}</Text>
          <Text className="code-line">{`        set1 = ui_create_field_set();`}</Text>
          <Text className="code-line">{`        lbl1 = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        desc1 = ui_create_field_description();`}</Text>
          <Text className="code-line">{`        sub1 = ui_create_field_group();`}</Text>
          <Text className="code-line">{`        set2 = ui_create_field_set();`}</Text>
          <Text className="code-line">{`        lbl2 = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        desc2 = ui_create_field_description();`}</Text>
          <Text className="code-line">{`        sub2 = ui_create_field_group();`}</Text>
          <Text className="code-line">{`        sep = ui_create_field_separator();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"display"[/color], [color=#0a3069]"flex"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"flex-direction"[/color], [color=#0a3069]"column"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"gap"[/color], [color=#0a3069]"8px"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"max-width"[/color], [color=#0a3069]"400px"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(group, set1);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(lbl1, [color=#0a3069]"Responses"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(set1, lbl1);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(`}</Text>
          <Text className="code-line">{`            desc1,`}</Text>
          <Text className="code-line">{`            [color=#0a3069]"Get notified when ChatGPT responds to requests that take time, "[/color]`}</Text>
          <Text className="code-line">{`            [color=#0a3069]"like research or image generation."[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(set1, desc1);`}</Text>
          <Text className="code-line">{`        ui_widget_append(set1, sub1);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        make_check_field(sub1, [color=#0a3069]"push"[/color], [color=#0a3069]"Push notifications"[/color], [color=#0550ae]1[/color], [color=#0550ae]1[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(group, sep);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(group, set2);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(lbl2, [color=#0a3069]"Tasks"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(set2, lbl2);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(desc2,`}</Text>
          <Text className="code-line">{`                            [color=#0a3069]"Get notified when tasks you&#x27;ve created have "[/color]`}</Text>
          <Text className="code-line">{`                            [color=#0a3069]"updates."[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(set2, desc2);`}</Text>
          <Text className="code-line">{`        ui_widget_append(set2, sub2);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        make_check_field(sub2, [color=#0a3069]"push-tasks"[/color], [color=#0a3069]"Push notifications"[/color], [color=#0550ae]0[/color], [color=#0550ae]0[/color]);`}</Text>
          <Text className="code-line">{`        make_check_field(sub2, [color=#0a3069]"email-tasks"[/color], [color=#0a3069]"Email notifications"[/color], [color=#0550ae]0[/color], [color=#0550ae]0[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(outer, group);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, outer);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/field-group/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color]?>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]style[/color]=[color=#0a3069]"display:flex; flex-direction:column; gap:8px; max-width:400px"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field-label[/color]>[/color]Responses[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field-description[/color]>[/color]Get notified when ChatGPT responds to requests that take time, like research or image generation.[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"push"[/color] [color=#0550ae]checked[/color]=[color=#0a3069]"true"[/color] [color=#0550ae]disabled[/color]=[color=#0a3069]"true"[/color] />[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"push"[/color]>[/color]Push notifications[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-separator[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field-label[/color]>[/color]Tasks[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field-description[/color]>[/color]Get notified when tasks you&#x27;ve created have updates.[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"push-tasks"[/color] />[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"push-tasks"[/color]>[/color]Push notifications[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"email-tasks"[/color] />[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"email-tasks"[/color]>[/color]Email notifications[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/field-group/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * docs/examples/field-group-xml/main.c: -- Field widget demo (XML variant)[/color]`}</Text>
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
