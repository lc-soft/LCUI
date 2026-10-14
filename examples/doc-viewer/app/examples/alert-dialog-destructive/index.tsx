import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function AlertDialogDestructiveDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/alert-dialog-destructive/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]AlertDialog[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AlertDialogTrigger[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AlertDialogOverlay[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AlertDialogContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AlertDialogHeader[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AlertDialogMedia[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AlertDialogTitle[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AlertDialogDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AlertDialogFooter[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AlertDialogCancel[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AlertDialogAction[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Icon[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Portal[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Example[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]AlertDialog[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]AlertDialogTrigger[/color]>[/color]Delete Chat[color=#116329]</[color=#0550ae]AlertDialogTrigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AlertDialogOverlay[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AlertDialogContent[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]AlertDialogHeader[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]AlertDialogMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"delete"[/color] />[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]AlertDialogMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]AlertDialogTitle[/color]>[/color]Delete chat?[color=#116329]</[color=#0550ae]AlertDialogTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]AlertDialogDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`                This will permanently delete this chat conversation and any`}</Text>
          <Text className="code-line">{`                memories saved during this chat.`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]AlertDialogDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]AlertDialogHeader[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]AlertDialogFooter[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]AlertDialogCancel[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]Cancel[color=#116329]</[color=#0550ae]AlertDialogCancel[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]AlertDialogAction[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"destructive"[/color]>[/color]Delete[color=#116329]</[color=#0550ae]AlertDialogAction[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]AlertDialogFooter[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]AlertDialogContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AlertDialogOverlay[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]AlertDialog[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/alert-dialog-destructive/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]alert_dialog_destructive_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, example_load());`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/alert-dialog-destructive/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]alert_dialog_destructive_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *alert = ui_create_alert_dialog();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *media = ui_create_alert_dialog_media();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *icon = ui_create_widget([color=#0a3069]"icon"[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *action;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_dialog_build_children(alert);`}</Text>
          <Text className="code-line">{`        ui_dialog_set_trigger_text(alert, [color=#0a3069]"Delete Chat"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(`}</Text>
          <Text className="code-line">{`            ui_widget_get_child(ui_widget_get_child(alert, [color=#0550ae]0[/color]), [color=#0550ae]0[/color]), [color=#0a3069]"variant"[/color],`}</Text>
          <Text className="code-line">{`            [color=#0a3069]"destructive"[/color]);`}</Text>
          <Text className="code-line">{`        ui_dialog_set_title(alert, [color=#0a3069]"Delete chat?"[/color]);`}</Text>
          <Text className="code-line">{`        ui_dialog_set_description(`}</Text>
          <Text className="code-line">{`            alert,`}</Text>
          <Text className="code-line">{`            [color=#0a3069]"This will permanently delete this chat conversation and any "[/color]`}</Text>
          <Text className="code-line">{`            [color=#0a3069]"memories saved during this chat."[/color]);`}</Text>
          <Text className="code-line">{`        ui_dialog_set_action_text(alert, [color=#0a3069]"Delete"[/color]);`}</Text>
          <Text className="code-line">{`        action = ui_dialog_get_footer_button(alert, [color=#0550ae]true[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(action, [color=#0a3069]"variant"[/color], [color=#0a3069]"destructive"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(icon, [color=#0a3069]"name"[/color], [color=#0a3069]"delete"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(media, icon);`}</Text>
          <Text className="code-line">{`        ui_alert_dialog_set_media(alert, media);`}</Text>
          <Text className="code-line">{`        ui_alert_dialog_set_size(alert, [color=#0a3069]"sm"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, alert);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/alert-dialog-destructive/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]alert-dialog[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]alert-dialog-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"destructive"[/color]>[/color]Delete Chat[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]alert-dialog-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]alert-dialog-overlay[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]alert-dialog-content[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]alert-dialog-header[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]alert-dialog-media[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"delete"[/color] />[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]alert-dialog-media[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]alert-dialog-title[/color]>[/color]Delete chat?[color=#116329]</[color=#0550ae]alert-dialog-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]alert-dialog-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`                This will permanently delete this chat conversation and any`}</Text>
          <Text className="code-line">{`                memories saved during this chat.`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]alert-dialog-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]alert-dialog-header[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]alert-dialog-footer[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]alert-dialog-cancel[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]Cancel[color=#116329]</[color=#0550ae]alert-dialog-cancel[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]alert-dialog-action[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"destructive"[/color]>[/color]Delete[color=#116329]</[color=#0550ae]alert-dialog-action[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]alert-dialog-footer[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]alert-dialog-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]alert-dialog-overlay[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]alert-dialog[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/alert-dialog-destructive/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]alert_dialog_destructive_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, ui_create_widget([color=#0550ae]NULL[/color]));`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
