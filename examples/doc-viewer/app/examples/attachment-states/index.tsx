import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function AttachmentStatesDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/attachment-states/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Attachment[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentAction[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentActions[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentMedia[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]AttachmentTitle[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Icon[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Spinner[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Example[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"mx-auto flex w-full max-w-sm flex-col gap-2"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Attachment[/color] [color=#0550ae]state[/color]=[color=#0a3069]"idle"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"clock"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentTitle[/color]>[/color]selected-file.pdf[color=#116329]</[color=#0550ae]AttachmentTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentDescription[/color]>[/color]Ready to upload[color=#116329]</[color=#0550ae]AttachmentDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentActions[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentAction[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Remove selected-file.pdf"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]AttachmentAction[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentActions[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Attachment[/color] [color=#0550ae]state[/color]=[color=#0a3069]"uploading"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Spinner[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentTitle[/color]>[/color]design-system.zip[color=#116329]</[color=#0550ae]AttachmentTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentDescription[/color]>[/color]Uploading · 64%[color=#116329]</[color=#0550ae]AttachmentDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentActions[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentAction[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Cancel upload"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]AttachmentAction[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentActions[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Attachment[/color] [color=#0550ae]state[/color]=[color=#0a3069]"processing"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentTitle[/color]>[/color]market-research.pdf[color=#116329]</[color=#0550ae]AttachmentTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentDescription[/color]>[/color]Processing document[color=#116329]</[color=#0550ae]AttachmentDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentActions[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentAction[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Remove market-research.pdf"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]AttachmentAction[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentActions[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Attachment[/color] [color=#0550ae]state[/color]=[color=#0a3069]"error"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-error"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentTitle[/color]>[/color]financial-model.xlsx[color=#116329]</[color=#0550ae]AttachmentTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentDescription[/color]>[/color]Upload failed. Try again.[color=#116329]</[color=#0550ae]AttachmentDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentActions[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentAction[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Retry upload"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"arrow-sync"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]AttachmentAction[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentAction[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Remove financial-model.xlsx"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]AttachmentAction[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentActions[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Attachment[/color] [color=#0550ae]state[/color]=[color=#0a3069]"done"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"checkmark"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentTitle[/color]>[/color]uploaded-report.pdf[color=#116329]</[color=#0550ae]AttachmentTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentDescription[/color]>[/color]Uploaded · 1.8 MB[color=#116329]</[color=#0550ae]AttachmentDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AttachmentActions[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AttachmentAction[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Remove uploaded-report.pdf"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]AttachmentAction[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AttachmentActions[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/attachment-states/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]attachment_states_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, example_load());`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/attachment-states/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]const[/color] [color=#953800]char[/color] *attachment_states_css = [color=#0a3069]"\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069].attachment-states-demo {\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  display: flex;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  flex-direction: column;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  gap: 8px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  width: 384px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069].attachment-states-demo attachment {\\[/color]`}</Text>
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
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_state_attachment[/color][color=#0550ae]([/color]`}</Text>
          <Text className="code-line">{`[color=#0550ae]    [color=#953800]ui_attachment_state_t[/color] state, [color=#953800]const[/color] [color=#953800]char[/color] *icon_name, [color=#953800]const[/color] [color=#953800]char[/color] *title_text,[/color]`}</Text>
          <Text className="code-line">{`[color=#0550ae]    [color=#953800]const[/color] [color=#953800]char[/color] *description_text, [color=#953800]bool[/color] retry)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *attachment = ui_create_attachment();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *media = ui_create_attachment_media();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_attachment_content();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *title = ui_create_attachment_title();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *description = ui_create_attachment_description();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *actions = ui_create_attachment_actions();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_attachment_set_state(attachment, state);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]if[/color] (icon_name) {`}</Text>
          <Text className="code-line">{`                [color=#953800]ui_widget_t[/color] *icon = ui_create_icon();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`                ui_icon_set_name(icon, icon_name);`}</Text>
          <Text className="code-line">{`                ui_widget_append(media, icon);`}</Text>
          <Text className="code-line">{`        } [color=#cf222e]else[/color] {`}</Text>
          <Text className="code-line">{`                ui_widget_append(media, ui_create_spinner());`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`        ui_text_set_content(title, title_text);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(description, description_text);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, title);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, description);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]if[/color] (retry) {`}</Text>
          <Text className="code-line">{`                ui_widget_append(actions, create_action([color=#0a3069]"arrow-sync"[/color],`}</Text>
          <Text className="code-line">{`                                                       [color=#0a3069]"Retry upload"[/color]));`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`        ui_widget_append(actions, create_action([color=#0a3069]"dismiss"[/color],`}</Text>
          <Text className="code-line">{`                                               [color=#0a3069]"Remove attachment"[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(attachment, media);`}</Text>
          <Text className="code-line">{`        ui_widget_append(attachment, content);`}</Text>
          <Text className="code-line">{`        ui_widget_append(attachment, actions);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] attachment;`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]attachment_states_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *box = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_load_css_string(attachment_states_css, [color=#0a3069]"attachment-states-demo"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(box, [color=#0a3069]"attachment-states-demo"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(box, create_state_attachment(`}</Text>
          <Text className="code-line">{`                                  UI_ATTACHMENT_STATE_IDLE, [color=#0a3069]"clock"[/color],`}</Text>
          <Text className="code-line">{`                                  [color=#0a3069]"selected-file.pdf"[/color], [color=#0a3069]"Ready to upload"[/color],`}</Text>
          <Text className="code-line">{`                                  [color=#0550ae]false[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(box, create_state_attachment(`}</Text>
          <Text className="code-line">{`                                  UI_ATTACHMENT_STATE_UPLOADING, [color=#0550ae]NULL[/color],`}</Text>
          <Text className="code-line">{`                                  [color=#0a3069]"design-system.zip"[/color], [color=#0a3069]"Uploading - 64%"[/color],`}</Text>
          <Text className="code-line">{`                                  [color=#0550ae]false[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(box, create_state_attachment(`}</Text>
          <Text className="code-line">{`                                  UI_ATTACHMENT_STATE_PROCESSING, [color=#0a3069]"document-text"[/color],`}</Text>
          <Text className="code-line">{`                                  [color=#0a3069]"market-research.pdf"[/color],`}</Text>
          <Text className="code-line">{`                                  [color=#0a3069]"Processing document"[/color], [color=#0550ae]false[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(box, create_state_attachment(`}</Text>
          <Text className="code-line">{`                                  UI_ATTACHMENT_STATE_ERROR, [color=#0a3069]"document-error"[/color],`}</Text>
          <Text className="code-line">{`                                  [color=#0a3069]"financial-model.xlsx"[/color],`}</Text>
          <Text className="code-line">{`                                  [color=#0a3069]"Upload failed. Try again."[/color], [color=#0550ae]true[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(box,`}</Text>
          <Text className="code-line">{`                         create_state_attachment(UI_ATTACHMENT_STATE_DONE,`}</Text>
          <Text className="code-line">{`                                                 [color=#0a3069]"checkmark"[/color],`}</Text>
          <Text className="code-line">{`                                                 [color=#0a3069]"uploaded-report.pdf"[/color],`}</Text>
          <Text className="code-line">{`                                                 [color=#0a3069]"Uploaded - 1.8 MB"[/color], [color=#0550ae]false[/color]));`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, box);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/attachment-states/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]resource[/color] [color=#0550ae]type[/color]=[color=#0a3069]"text/css"[/color]>[/color]`}</Text>
          <Text className="code-line">{`    .attachment-states-demo {`}</Text>
          <Text className="code-line">{`      display: flex;`}</Text>
          <Text className="code-line">{`      flex-direction: column;`}</Text>
          <Text className="code-line">{`      gap: 8px;`}</Text>
          <Text className="code-line">{`      width: 384px;`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`    .attachment-states-demo attachment {`}</Text>
          <Text className="code-line">{`      width: 100%;`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]resource[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]widget[/color] [color=#0550ae]class[/color]=[color=#0a3069]"attachment-states-demo"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]attachment[/color] [color=#0550ae]state[/color]=[color=#0a3069]"idle"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-media[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"clock"[/color] />[/color][color=#116329]</[color=#0550ae]attachment-media[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-title[/color]>[/color]selected-file.pdf[color=#116329]</[color=#0550ae]attachment-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-description[/color]>[/color]Ready to upload[color=#116329]</[color=#0550ae]attachment-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-actions[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"icon-xs"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"attachment-action"[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Remove selected-file.pdf"[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color][color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment-actions[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]attachment[/color] [color=#0550ae]state[/color]=[color=#0a3069]"uploading"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-media[/color]>[/color][color=#116329]<[color=#0550ae]spinner[/color] />[/color][color=#116329]</[color=#0550ae]attachment-media[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-title[/color]>[/color]design-system.zip[color=#116329]</[color=#0550ae]attachment-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-description[/color]>[/color]Uploading - 64%[color=#116329]</[color=#0550ae]attachment-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-actions[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"icon-xs"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"attachment-action"[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Cancel upload"[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color][color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment-actions[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]attachment[/color] [color=#0550ae]state[/color]=[color=#0a3069]"processing"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-media[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color][color=#116329]</[color=#0550ae]attachment-media[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-title[/color]>[/color]market-research.pdf[color=#116329]</[color=#0550ae]attachment-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-description[/color]>[/color]Processing document[color=#116329]</[color=#0550ae]attachment-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-actions[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"icon-xs"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"attachment-action"[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Remove market-research.pdf"[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color][color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment-actions[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]attachment[/color] [color=#0550ae]state[/color]=[color=#0a3069]"error"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-media[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-error"[/color] />[/color][color=#116329]</[color=#0550ae]attachment-media[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-title[/color]>[/color]financial-model.xlsx[color=#116329]</[color=#0550ae]attachment-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-description[/color]>[/color]Upload failed. Try again.[color=#116329]</[color=#0550ae]attachment-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-actions[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"icon-xs"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"attachment-action"[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Retry upload"[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"arrow-sync"[/color] />[/color][color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"icon-xs"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"attachment-action"[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Remove financial-model.xlsx"[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color][color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment-actions[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]attachment[/color] [color=#0550ae]state[/color]=[color=#0a3069]"done"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-media[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"checkmark"[/color] />[/color][color=#116329]</[color=#0550ae]attachment-media[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-title[/color]>[/color]uploaded-report.pdf[color=#116329]</[color=#0550ae]attachment-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]attachment-description[/color]>[/color]Uploaded - 1.8 MB[color=#116329]</[color=#0550ae]attachment-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]attachment-actions[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"icon-xs"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"attachment-action"[/color] [color=#0550ae]aria-label[/color]=[color=#0a3069]"Remove uploaded-report.pdf"[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"dismiss"[/color] />[/color][color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]attachment-actions[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]attachment[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/attachment-states/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]attachment_states_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, ui_create_widget([color=#0550ae]NULL[/color]));`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
