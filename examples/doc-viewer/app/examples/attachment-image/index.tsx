import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function AttachmentImageDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/attachment-image/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Attachment[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentAction[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentActions[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentGroup[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentMedia[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentTitle[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Icon[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]import[/color] deskReference [color=#cf222e]from[/color] [color=#0a3069]"../assets/attachment/desk-reference.jpg"[/color];`}</Text>
          <Text className="code-line">{`[color=#cf222e]import[/color] officeReference [color=#cf222e]from[/color] [color=#0a3069]"../assets/attachment/office-reference.jpg"[/color];`}</Text>
          <Text className="code-line">{`[color=#cf222e]import[/color] workspace [color=#cf222e]from[/color] [color=#0a3069]"../assets/attachment/workspace.jpg"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]const[/color] images = \\[`}</Text>
          <Text className="code-line">{`  {`}</Text>
          <Text className="code-line">{`    [color=#0550ae]name[/color]: [color=#0a3069]"workspace.png"[/color],`}</Text>
          <Text className="code-line">{`    [color=#0550ae]meta[/color]: [color=#0a3069]"PNG · 820 KB"[/color],`}</Text>
          <Text className="code-line">{`    [color=#0550ae]src[/color]: workspace,`}</Text>
          <Text className="code-line">{`    [color=#0550ae]alt[/color]: [color=#0a3069]"Workspace"[/color],`}</Text>
          <Text className="code-line">{`  },`}</Text>
          <Text className="code-line">{`  {`}</Text>
          <Text className="code-line">{`    [color=#0550ae]name[/color]: [color=#0a3069]"desk-reference.jpg"[/color],`}</Text>
          <Text className="code-line">{`    [color=#0550ae]meta[/color]: [color=#0a3069]"JPG · 1.1 MB"[/color],`}</Text>
          <Text className="code-line">{`    [color=#0550ae]src[/color]: deskReference,`}</Text>
          <Text className="code-line">{`    [color=#0550ae]alt[/color]: [color=#0a3069]"Desk"[/color],`}</Text>
          <Text className="code-line">{`  },`}</Text>
          <Text className="code-line">{`  {`}</Text>
          <Text className="code-line">{`    [color=#0550ae]name[/color]: [color=#0a3069]"office-reference.jpg"[/color],`}</Text>
          <Text className="code-line">{`    [color=#0550ae]meta[/color]: [color=#0a3069]"JPG · 940 KB"[/color],`}</Text>
          <Text className="code-line">{`    [color=#0550ae]src[/color]: officeReference,`}</Text>
          <Text className="code-line">{`    [color=#0550ae]alt[/color]: [color=#0a3069]"Office"[/color],`}</Text>
          <Text className="code-line">{`  },`}</Text>
          <Text className="code-line">{`];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Example[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"mx-auto w-full max-w-sm"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]AttachmentGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`        {images.map((image) => (`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Attachment[/color] [color=#0550ae]key[/color]=[color=#0a3069]{image.name}[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"vertical"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]AttachmentMedia[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]variant[/color]=[color=#0a3069]"image"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]aria-label[/color]=[color=#0a3069]{image.alt}[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]style[/color]=[color=#0a3069]{{[/color] [color=#0550ae]backgroundImage:[/color] \`[color=#0550ae]url[/color](\${[color=#0550ae]image.src[/color]})\` }}[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]            />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]AttachmentTitle[/color]>[/color]{image.name}[color=#116329]</[color=#0550ae]AttachmentTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]AttachmentDescription[/color]>[/color]{image.meta}[color=#116329]</[color=#0550ae]AttachmentDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]AttachmentActions[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]AttachmentAction[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]{[/color]\`[color=#0550ae]Remove[/color] \${[color=#0550ae]image.name[/color]}\`}>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]AttachmentAction[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]AttachmentActions[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`        ))}`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]AttachmentGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/attachment-image/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]attachment_image_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, example_load());`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/attachment-image/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]const[/color] [color=#953800]char[/color] *attachment_image_css = [color=#0a3069]"\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069].attachment-image-demo {\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  width: 384px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069].attachment-image-demo attachment-group {\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  width: 100%;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_image_card[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *path, [color=#953800]const[/color] [color=#953800]char[/color] *name,[/color]`}</Text>
          <Text className="code-line">{`[color=#0550ae]                                      [color=#953800]const[/color] [color=#953800]char[/color] *meta)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *attachment = ui_create_attachment();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *media = ui_create_attachment_media();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_attachment_content();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *title = ui_create_attachment_title();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *description = ui_create_attachment_description();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *actions = ui_create_attachment_actions();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *action = ui_create_attachment_action();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *icon = ui_create_icon();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_attachment_set_orientation(attachment,`}</Text>
          <Text className="code-line">{`                                      UI_ATTACHMENT_ORIENTATION_VERTICAL);`}</Text>
          <Text className="code-line">{`        ui_attachment_media_set_image_url(media, path);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(title, name);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(description, meta);`}</Text>
          <Text className="code-line">{`        ui_icon_set_name(icon, [color=#0a3069]"dismiss"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(action, [color=#0a3069]"aria-label"[/color], name);`}</Text>
          <Text className="code-line">{`        ui_widget_append(action, icon);`}</Text>
          <Text className="code-line">{`        ui_widget_append(actions, action);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, title);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, description);`}</Text>
          <Text className="code-line">{`        ui_widget_append(attachment, media);`}</Text>
          <Text className="code-line">{`        ui_widget_append(attachment, content);`}</Text>
          <Text className="code-line">{`        ui_widget_append(attachment, actions);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] attachment;`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]attachment_image_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *box = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *group = ui_create_attachment_group();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_load_css_string(attachment_image_css, [color=#0a3069]"attachment-image-demo"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(box, [color=#0a3069]"attachment-image-demo"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(group,`}</Text>
          <Text className="code-line">{`                         create_image_card(`}</Text>
          <Text className="code-line">{`                             [color=#0a3069]"assets/attachment/workspace.jpg"[/color],`}</Text>
          <Text className="code-line">{`                             [color=#0a3069]"workspace.png"[/color], [color=#0a3069]"PNG - 820 KB"[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(group,`}</Text>
          <Text className="code-line">{`                         create_image_card(`}</Text>
          <Text className="code-line">{`                             [color=#0a3069]"assets/attachment/desk-reference.jpg"[/color],`}</Text>
          <Text className="code-line">{`                             [color=#0a3069]"desk-reference.jpg"[/color], [color=#0a3069]"JPG - 1.1 MB"[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(group,`}</Text>
          <Text className="code-line">{`                         create_image_card(`}</Text>
          <Text className="code-line">{`                             [color=#0a3069]"assets/attachment/office-reference.jpg"[/color],`}</Text>
          <Text className="code-line">{`                             [color=#0a3069]"office-reference.jpg"[/color], [color=#0a3069]"JPG - 940 KB"[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(box, group);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, box);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/attachment-image/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]resource[/color] [color=#0550ae]type[/color]=[color=#0a3069]"text/css"[/color]>[/color]`}</Text>
          <Text className="code-line">{`    .attachment-image-demo {`}</Text>
          <Text className="code-line">{`      width: 384px;`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`    .attachment-image-demo attachment-group {`}</Text>
          <Text className="code-line">{`      width: 100%;`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]resource[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]widget[/color] [color=#0550ae]class[/color]=[color=#0a3069]"attachment-image-demo"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]attachment-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"vertical"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-media[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"image"[/color] [color=#0550ae]src[/color]=[color=#0a3069]"assets/attachment/workspace.jpg"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]attachment-title[/color]>[/color]workspace.png[color=#116329]</[color=#0550ae]attachment-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]attachment-description[/color]>[/color]PNG - 820 KB[color=#116329]</[color=#0550ae]attachment-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-actions[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"icon-xs"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"attachment-action"[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Remove workspace.png"[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color][color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]attachment-actions[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"vertical"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-media[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"image"[/color] [color=#0550ae]src[/color]=[color=#0a3069]"assets/attachment/desk-reference.jpg"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]attachment-title[/color]>[/color]desk-reference.jpg[color=#116329]</[color=#0550ae]attachment-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]attachment-description[/color]>[/color]JPG - 1.1 MB[color=#116329]</[color=#0550ae]attachment-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-actions[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"icon-xs"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"attachment-action"[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Remove desk-reference.jpg"[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color][color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]attachment-actions[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"vertical"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-media[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"image"[/color] [color=#0550ae]src[/color]=[color=#0a3069]"assets/attachment/office-reference.jpg"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]attachment-title[/color]>[/color]office-reference.jpg[color=#116329]</[color=#0550ae]attachment-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]attachment-description[/color]>[/color]JPG - 940 KB[color=#116329]</[color=#0550ae]attachment-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-actions[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"icon-xs"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"attachment-action"[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Remove office-reference.jpg"[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color][color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]attachment-actions[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]attachment-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/attachment-image/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]attachment_image_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, ui_create_widget([color=#0550ae]NULL[/color]));`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
