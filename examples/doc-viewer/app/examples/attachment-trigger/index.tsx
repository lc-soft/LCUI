import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function AttachmentTriggerDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/attachment-trigger/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Attachment[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentAction[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentActions[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentMedia[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentTitle[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentTrigger[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogHeader[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogOverlay[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogTitle[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Icon[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Portal[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Example[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"mx-auto w-full max-w-sm p-1"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentTitle[/color]>[/color]research-summary.pdf[color=#116329]</[color=#0550ae]AttachmentTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentDescription[/color]>[/color]Open preview dialog[color=#116329]</[color=#0550ae]AttachmentDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentActions[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentAction[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Copy link"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"copy"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]AttachmentAction[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentAction[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Remove research-summary.pdf"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]AttachmentAction[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentActions[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentTrigger[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]          [color=#0550ae]data-dialog-trigger[/color]=[color=#0a3069]"attachment-preview"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]          [color=#0550ae]aria-label[/color]=[color=#0a3069]"Preview research-summary.pdf"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]        />[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]DialogOverlay[/color] [color=#0550ae]data-dialog-target[/color]=[color=#0a3069]"attachment-preview"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DialogContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DialogHeader[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DialogTitle[/color]>[/color]research-summary.pdf[color=#116329]</[color=#0550ae]DialogTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DialogDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`                The attachment trigger opens this dialog while the actions stay`}</Text>
          <Text className="code-line">{`                independently clickable.`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]DialogDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]DialogHeader[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]data-dialog-close[/color]=[color=#0a3069]"attachment-preview"[/color]>[/color]Close[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DialogContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]DialogOverlay[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/attachment-trigger/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]attachment_trigger_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, example_load());`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/attachment-trigger/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]const[/color] [color=#953800]char[/color] *attachment_trigger_css = [color=#0a3069]"\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069].attachment-trigger-demo {\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  box-sizing: border-box;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  width: 384px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  padding: 4px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069].attachment-trigger-demo attachment {\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  width: 100%;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_action[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *icon_name, [color=#953800]const[/color] [color=#953800]char[/color] *label)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *action = ui_create_attachment_action();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *icon = ui_create_icon();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_icon_set_name(icon, icon_name);`}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(action, [color=#0a3069]"aria-label"[/color], label);`}</Text>
          <Text className="code-line">{`        ui_widget_append(action, icon);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] action;`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]create_preview_dialog[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *overlay = ui_create_dialog_overlay();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_dialog_content();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *header = ui_create_dialog_header();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *title = ui_create_dialog_title();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *description = ui_create_dialog_description();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *footer = ui_create_dialog_footer();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *close = ui_create_button();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *open = ui_create_button();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(overlay, [color=#0a3069]"data-dialog-target"[/color], [color=#0a3069]"attachment-preview"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(title, [color=#0a3069]"research-summary.pdf"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(`}</Text>
          <Text className="code-line">{`            description,`}</Text>
          <Text className="code-line">{`            [color=#0a3069]"The attachment trigger opens this dialog while the actions stay "[/color]`}</Text>
          <Text className="code-line">{`            [color=#0a3069]"independently clickable."[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_text(close, [color=#0a3069]"Close"[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_variant(close, UI_BUTTON_VARIANT_OUTLINE);`}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(close, [color=#0a3069]"data-dialog-close"[/color], [color=#0a3069]"attachment-preview"[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_text(open, [color=#0a3069]"Open"[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_variant(open, UI_BUTTON_VARIANT_PRIMARY);`}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(open, [color=#0a3069]"data-dialog-close"[/color], [color=#0a3069]"attachment-preview"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(header, title);`}</Text>
          <Text className="code-line">{`        ui_widget_append(header, description);`}</Text>
          <Text className="code-line">{`        ui_widget_append(footer, close);`}</Text>
          <Text className="code-line">{`        ui_widget_append(footer, open);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, header);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, footer);`}</Text>
          <Text className="code-line">{`        ui_widget_append(overlay, content);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, overlay);`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]attachment_trigger_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *box = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *attachment = ui_create_attachment();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *media = ui_create_attachment_media();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *icon = ui_create_icon();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_attachment_content();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *title = ui_create_attachment_title();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *description = ui_create_attachment_description();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *actions = ui_create_attachment_actions();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *trigger = ui_create_attachment_trigger();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_load_css_string(attachment_trigger_css, [color=#0a3069]"attachment-trigger-demo"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(box, [color=#0a3069]"attachment-trigger-demo"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_icon_set_name(icon, [color=#0a3069]"document"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(media, icon);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(title, [color=#0a3069]"research-summary.pdf"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(description, [color=#0a3069]"Open preview dialog"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, title);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, description);`}</Text>
          <Text className="code-line">{`        ui_widget_append(actions, create_action([color=#0a3069]"copy"[/color], [color=#0a3069]"Copy link"[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(`}</Text>
          <Text className="code-line">{`            actions, create_action([color=#0a3069]"dismiss"[/color], [color=#0a3069]"Remove research-summary.pdf"[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(trigger, [color=#0a3069]"data-dialog-trigger"[/color],`}</Text>
          <Text className="code-line">{`                           [color=#0a3069]"attachment-preview"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(attachment, media);`}</Text>
          <Text className="code-line">{`        ui_widget_append(attachment, content);`}</Text>
          <Text className="code-line">{`        ui_widget_append(attachment, actions);`}</Text>
          <Text className="code-line">{`        ui_widget_append(attachment, trigger);`}</Text>
          <Text className="code-line">{`        ui_widget_append(box, attachment);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, box);`}</Text>
          <Text className="code-line">{`        create_preview_dialog(parent);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/attachment-trigger/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]resource[/color] [color=#0550ae]type[/color]=[color=#0a3069]"text/css"[/color]>[/color]`}</Text>
          <Text className="code-line">{`    .attachment-trigger-demo {`}</Text>
          <Text className="code-line">{`      box-sizing: border-box;`}</Text>
          <Text className="code-line">{`      width: 384px;`}</Text>
          <Text className="code-line">{`      padding: 4px;`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`    .attachment-trigger-demo attachment {`}</Text>
          <Text className="code-line">{`      width: 100%;`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]resource[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]widget[/color] [color=#0550ae]class[/color]=[color=#0a3069]"attachment-trigger-demo"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-media[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document"[/color] />[/color][color=#116329]</[color=#0550ae]attachment-media[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-title[/color]>[/color]research-summary.pdf[color=#116329]</[color=#0550ae]attachment-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-description[/color]>[/color]Open preview dialog[color=#116329]</[color=#0550ae]attachment-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-actions[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"icon-xs"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"attachment-action"[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Copy link"[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"copy"[/color] />[/color][color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"icon-xs"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"attachment-action"[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Remove research-summary.pdf"[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color][color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment-actions[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-trigger[/color] [color=#0550ae]data-dialog-trigger[/color]=[color=#0a3069]"attachment-preview"[/color] />[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]dialog-overlay[/color] [color=#0550ae]data-dialog-target[/color]=[color=#0a3069]"attachment-preview"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dialog-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dialog-header[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dialog-title[/color]>[/color]research-summary.pdf[color=#116329]</[color=#0550ae]dialog-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dialog-description[/color]>[/color]The attachment trigger opens this dialog while the actions stay independently clickable.[color=#116329]</[color=#0550ae]dialog-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]dialog-header[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dialog-footer[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color] [color=#0550ae]data-dialog-close[/color]=[color=#0a3069]"attachment-preview"[/color]>[/color]Close[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"primary"[/color] [color=#0550ae]data-dialog-close[/color]=[color=#0a3069]"attachment-preview"[/color]>[/color]Open[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]dialog-footer[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dialog-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]dialog-overlay[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/attachment-trigger/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]attachment_trigger_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, ui_create_widget([color=#0550ae]NULL[/color]));`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
