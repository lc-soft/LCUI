import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function AttachmentSizesDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/attachment-sizes/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Attachment[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentMedia[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentTitle[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Icon[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Example[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"mx-auto flex w-full max-w-sm flex-col gap-3"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Attachment[/color] [color=#0550ae]size[/color]=[color=#0a3069]"default"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentTitle[/color]>[/color]Default attachment[color=#116329]</[color=#0550ae]AttachmentTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentDescription[/color]>[/color]PDF · 2.4 MB[color=#116329]</[color=#0550ae]AttachmentDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Attachment[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentTitle[/color]>[/color]Small attachment[color=#116329]</[color=#0550ae]AttachmentTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentDescription[/color]>[/color]PDF · 2.4 MB[color=#116329]</[color=#0550ae]AttachmentDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Attachment[/color] [color=#0550ae]size[/color]=[color=#0a3069]"xs"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentTitle[/color]>[/color]Extra small attachment[color=#116329]</[color=#0550ae]AttachmentTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/attachment-sizes/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]attachment_sizes_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, example_load());`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/attachment-sizes/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]const[/color] [color=#953800]char[/color] *attachment_sizes_css = [color=#0a3069]"\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069].attachment-sizes-demo {\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  display: flex;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  flex-direction: column;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  gap: 12px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  width: 384px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069].attachment-sizes-demo attachment {\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  width: 100%;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_size_attachment[/color][color=#0550ae]([color=#953800]ui_attachment_size_t[/color] size,[/color]`}</Text>
          <Text className="code-line">{`[color=#0550ae]                                           [color=#953800]const[/color] [color=#953800]char[/color] *title_text,[/color]`}</Text>
          <Text className="code-line">{`[color=#0550ae]                                           [color=#953800]const[/color] [color=#953800]char[/color] *description_text)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *attachment = ui_create_attachment();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *media = ui_create_attachment_media();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *icon = ui_create_icon();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_attachment_content();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *title = ui_create_attachment_title();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_attachment_set_size(attachment, size);`}</Text>
          <Text className="code-line">{`        ui_icon_set_name(icon, [color=#0a3069]"document"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(media, icon);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(title, title_text);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, title);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]if[/color] (description_text) {`}</Text>
          <Text className="code-line">{`                [color=#953800]ui_widget_t[/color] *description =`}</Text>
          <Text className="code-line">{`                    ui_create_attachment_description();`}</Text>
          <Text className="code-line">{`                ui_text_set_content(description, description_text);`}</Text>
          <Text className="code-line">{`                ui_widget_append(content, description);`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`        ui_widget_append(attachment, media);`}</Text>
          <Text className="code-line">{`        ui_widget_append(attachment, content);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] attachment;`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]attachment_sizes_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *box = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_load_css_string(attachment_sizes_css, [color=#0a3069]"attachment-sizes-demo"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(box, [color=#0a3069]"attachment-sizes-demo"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(`}</Text>
          <Text className="code-line">{`            box, create_size_attachment(UI_ATTACHMENT_SIZE_DEFAULT,`}</Text>
          <Text className="code-line">{`                                        [color=#0a3069]"Default attachment"[/color],`}</Text>
          <Text className="code-line">{`                                        [color=#0a3069]"PDF - 2.4 MB"[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(box,`}</Text>
          <Text className="code-line">{`                         create_size_attachment(UI_ATTACHMENT_SIZE_SM,`}</Text>
          <Text className="code-line">{`                                                [color=#0a3069]"Small attachment"[/color],`}</Text>
          <Text className="code-line">{`                                                [color=#0a3069]"PDF - 2.4 MB"[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(box,`}</Text>
          <Text className="code-line">{`                         create_size_attachment(UI_ATTACHMENT_SIZE_XS,`}</Text>
          <Text className="code-line">{`                                                [color=#0a3069]"Extra small attachment"[/color],`}</Text>
          <Text className="code-line">{`                                                [color=#0550ae]NULL[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, box);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/attachment-sizes/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]resource[/color] [color=#0550ae]type[/color]=[color=#0a3069]"text/css"[/color]>[/color]`}</Text>
          <Text className="code-line">{`    .attachment-sizes-demo {`}</Text>
          <Text className="code-line">{`      display: flex;`}</Text>
          <Text className="code-line">{`      flex-direction: column;`}</Text>
          <Text className="code-line">{`      gap: 12px;`}</Text>
          <Text className="code-line">{`      width: 384px;`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`    .attachment-sizes-demo attachment {`}</Text>
          <Text className="code-line">{`      width: 100%;`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]resource[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]widget[/color] [color=#0550ae]class[/color]=[color=#0a3069]"attachment-sizes-demo"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]attachment[/color] [color=#0550ae]size[/color]=[color=#0a3069]"default"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-media[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment-media[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-title[/color]>[/color]Default attachment[color=#116329]</[color=#0550ae]attachment-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-description[/color]>[/color]PDF - 2.4 MB[color=#116329]</[color=#0550ae]attachment-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]attachment[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-media[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment-media[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-title[/color]>[/color]Small attachment[color=#116329]</[color=#0550ae]attachment-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-description[/color]>[/color]PDF - 2.4 MB[color=#116329]</[color=#0550ae]attachment-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]attachment[/color] [color=#0550ae]size[/color]=[color=#0a3069]"xs"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-media[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment-media[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-title[/color]>[/color]Extra small attachment[color=#116329]</[color=#0550ae]attachment-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/attachment-sizes/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]attachment_sizes_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, ui_create_widget([color=#0550ae]NULL[/color]));`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
