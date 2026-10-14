import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function DialogBasicDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/dialog-basic/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Dialog[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogTrigger[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogOverlay[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogHeader[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogTitle[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogFooter[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogClose[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Field[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]FieldGroup[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Label[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]TextInput[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Portal[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Example[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Dialog[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]DialogTrigger[/color]>[/color]Open Dialog[color=#116329]</[color=#0550ae]DialogTrigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]DialogOverlay[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DialogContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DialogHeader[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DialogTitle[/color]>[/color]Edit profile[color=#116329]</[color=#0550ae]DialogTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DialogDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`                Make changes to your profile here. Click save when you&#x27;re done.`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]DialogDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]DialogHeader[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Label[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"name-1"[/color]>[/color]Name[color=#116329]</[color=#0550ae]Label[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]TextInput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"name-1"[/color] [color=#0550ae]name[/color]=[color=#0a3069]"name"[/color] [color=#0550ae]defaultValue[/color]=[color=#0a3069]"Pedro Duarte"[/color] />[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Label[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"username-1"[/color]>[/color]Username[color=#116329]</[color=#0550ae]Label[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]TextInput[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]                  [color=#0550ae]id[/color]=[color=#0a3069]"username-1"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]                  [color=#0550ae]name[/color]=[color=#0a3069]"username"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]                  [color=#0550ae]defaultValue[/color]=[color=#0a3069]"@peduarte"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]                />[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DialogFooter[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DialogClose[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]Cancel[color=#116329]</[color=#0550ae]DialogClose[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"primary"[/color]>[/color]Save changes[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]DialogFooter[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DialogClose[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DialogContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]DialogOverlay[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Dialog[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/dialog-basic/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]dialog_basic_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, example_load());`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/dialog-basic/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]make_field[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *label_text, [color=#953800]const[/color] [color=#953800]char[/color] *value)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *field = ui_create_widget([color=#0a3069]"field"[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *label = ui_create_widget([color=#0a3069]"label"[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *input = ui_create_widget([color=#0a3069]"textinput"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_text_set_content(label, label_text);`}</Text>
          <Text className="code-line">{`        ui_textinput_set_text(input, value);`}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(input, [color=#0a3069]"tabindex"[/color], [color=#0a3069]"0"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(field, label);`}</Text>
          <Text className="code-line">{`        ui_widget_append(field, input);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] field;`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]dialog_basic_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *dialog = ui_create_dialog();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_dialog_build_children(dialog);`}</Text>
          <Text className="code-line">{`        ui_dialog_set_trigger_text(dialog, [color=#0a3069]"Open Dialog"[/color]);`}</Text>
          <Text className="code-line">{`        ui_dialog_set_title(dialog, [color=#0a3069]"Edit profile"[/color]);`}</Text>
          <Text className="code-line">{`        ui_dialog_set_description(`}</Text>
          <Text className="code-line">{`            dialog,`}</Text>
          <Text className="code-line">{`            [color=#0a3069]"Make changes to your profile here. Click save when you&#x27;re done."[/color]);`}</Text>
          <Text className="code-line">{`        ui_dialog_set_cancel_text(dialog, [color=#0a3069]"Cancel"[/color]);`}</Text>
          <Text className="code-line">{`        ui_dialog_set_action_text(dialog, [color=#0a3069]"Save changes"[/color]);`}</Text>
          <Text className="code-line">{`        ui_dialog_append_body(dialog, make_field([color=#0a3069]"Name"[/color], [color=#0a3069]"Pedro Duarte"[/color]));`}</Text>
          <Text className="code-line">{`        ui_dialog_append_body(dialog, make_field([color=#0a3069]"Username"[/color], [color=#0a3069]"@peduarte"[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, dialog);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/dialog-basic/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]dialog[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]dialog-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]Open Dialog[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]dialog-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dialog-overlay[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dialog-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dialog-header[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dialog-title[/color]>[/color]Edit profile[color=#116329]</[color=#0550ae]dialog-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dialog-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`                Make changes to your profile here. Click save when you&#x27;re done.`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]dialog-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]dialog-header[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]label[/color]>[/color]Name[color=#116329]</[color=#0550ae]label[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]tabindex[/color]=[color=#0a3069]"0"[/color]>[/color]Pedro Duarte[color=#116329]</[color=#0550ae]textinput[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]label[/color]>[/color]Username[color=#116329]</[color=#0550ae]label[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]tabindex[/color]=[color=#0a3069]"0"[/color]>[/color]@peduarte[color=#116329]</[color=#0550ae]textinput[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dialog-footer[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color] [color=#0550ae]data-dialog-close[/color]=[color=#0a3069]"true"[/color]>[/color]Cancel[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"primary"[/color]>[/color]Save changes[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]dialog-footer[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"dialog-close"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"icon"[/color] [color=#0550ae]data-dialog-close[/color]=[color=#0a3069]"true"[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color][color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]dialog-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dialog-overlay[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]dialog[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/dialog-basic/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]dialog_basic_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, ui_create_widget([color=#0550ae]NULL[/color]));`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
