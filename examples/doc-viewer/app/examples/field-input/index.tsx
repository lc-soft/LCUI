import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function FieldInputDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/field-input/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Field[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldGroup[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldLabel[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldSet[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]TextInput[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-col gap-2 max-w-\\[400px]"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"username"[/color]>[/color]Username[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]TextInput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"username"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"Max Leiter"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]Choose a unique username for your account.[color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"password"[/color]>[/color]Password[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]Must be at least 8 characters long.[color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]TextInput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"password"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"••••••••"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/field-input/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * docs/examples/field-input-tsx/main.c: -- Field widget demo (TSX variant)[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] *[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * SPDX-License-Identifier: MIT[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] */[/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]field_input_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        example_load(parent);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/field-input/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`﻿[color=#6e7781]/*[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * docs/examples/field-input/main.c: -- Field widget demo (C variant)[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] *[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * SPDX-License-Identifier: MIT[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] */[/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]field_input_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *outer, *[color=#953800]set[/color], *group;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *user_field, *user_lbl, *user_input, *user_desc;`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *pass_field, *pass_lbl, *pass_desc, *pass_input;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        outer = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]set[/color] = ui_create_field_set();`}</Text>
          <Text className="code-line">{`        group = ui_create_field_group();`}</Text>
          <Text className="code-line">{`        user_field = ui_create_field();`}</Text>
          <Text className="code-line">{`        user_lbl = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        user_input = ui_create_widget([color=#0a3069]"textinput"[/color]);`}</Text>
          <Text className="code-line">{`        user_desc = ui_create_field_description();`}</Text>
          <Text className="code-line">{`        pass_field = ui_create_field();`}</Text>
          <Text className="code-line">{`        pass_lbl = ui_create_field_label();`}</Text>
          <Text className="code-line">{`        pass_desc = ui_create_field_description();`}</Text>
          <Text className="code-line">{`        pass_input = ui_create_widget([color=#0a3069]"textinput"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"display"[/color], [color=#0a3069]"flex"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"flex-direction"[/color], [color=#0a3069]"column"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"gap"[/color], [color=#0a3069]"8px"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(outer, [color=#0a3069]"max-width"[/color], [color=#0a3069]"400px"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append([color=#953800]set[/color], group);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_label_set_for(user_lbl, [color=#0a3069]"username"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(user_lbl, [color=#0a3069]"Username"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(user_field, user_lbl);`}</Text>
          <Text className="code-line">{`        ui_textinput_set_placeholder(user_input, [color=#0a3069]"Max Leiter"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_id(user_input, [color=#0a3069]"username"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(user_field, user_input);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(user_desc,`}</Text>
          <Text className="code-line">{`                            [color=#0a3069]"Choose a unique username for your account."[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(user_field, user_desc);`}</Text>
          <Text className="code-line">{`        ui_widget_append(group, user_field);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_label_set_for(pass_lbl, [color=#0a3069]"password"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(pass_lbl, [color=#0a3069]"Password"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(pass_field, pass_lbl);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(pass_desc, [color=#0a3069]"Must be at least 8 characters long."[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(pass_field, pass_desc);`}</Text>
          <Text className="code-line">{`        ui_textinput_set_placeholder(pass_input, [color=#0a3069]u8"••••••••"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_id(pass_input, [color=#0a3069]"password"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(pass_field, pass_input);`}</Text>
          <Text className="code-line">{`        ui_widget_append(group, pass_field);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(outer, [color=#953800]set[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, outer);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/field-input/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color]?>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]style[/color]=[color=#0a3069]"display:flex; flex-direction:column; gap:8px; max-width:400px"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"username"[/color]>[/color]Username[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"username"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"Max Leiter"[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-description[/color]>[/color]Choose a unique username for your account.[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"password"[/color]>[/color]Password[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-description[/color]>[/color]Must be at least 8 characters long.[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"password"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"••••••••"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field-set[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/field-input/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * docs/examples/field-input-xml/main.c: -- Field widget demo (XML variant)[/color]`}</Text>
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
