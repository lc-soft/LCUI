import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function EmptyBasicDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/empty-basic/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Button[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Empty[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]EmptyContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]EmptyHeader[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]EmptyMedia[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]EmptyTitle[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]EmptyDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]EmptyAction[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Text[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]ArrowUpRight[/color], [color=#8250df]Folder[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/fluent-icons"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Example[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Empty[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-full max-w-\\[400px]"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]EmptyHeader[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]EmptyMedia[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"icon"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Folder[/color] />[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]EmptyMedia[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]EmptyTitle[/color]>[/color]No Projects Yet[color=#116329]</[color=#0550ae]EmptyTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]EmptyDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`        You haven&#x27;t created any projects yet. Get started by creating your first`}</Text>
          <Text className="code-line">{`        project.`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]EmptyDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]EmptyHeader[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]EmptyContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]EmptyAction[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"primary"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Text[/color]>[/color]Create Project[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Button[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Text[/color]>[/color]Import Project[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]EmptyAction[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]EmptyContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"text-muted-foreground"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Text[/color]>[/color]Learn More[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]ArrowUpRight[/color] />[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Empty[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/empty-basic/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]empty_basic_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, example_load());`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/empty-basic/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_text_button[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *content)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button = ui_create_button();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_text_set_content(text, content);`}</Text>
          <Text className="code-line">{`        ui_widget_append(button, text);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] button;`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]empty_basic_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *empty = ui_create_empty();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *header = ui_create_empty_header();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *media = ui_create_empty_media();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *folder = ui_create_icon();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *title = ui_create_empty_title();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *description = ui_create_empty_description();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_empty_content();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *actions = ui_create_empty_action();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *create_button = create_text_button([color=#0a3069]"Create Project"[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *import_button = create_text_button([color=#0a3069]"Import Project"[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *learn_more_button = create_text_button([color=#0a3069]"Learn More"[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *arrow_up_right = ui_create_icon();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_add_class(empty, [color=#0a3069]"empty-basic-demo"[/color]);`}</Text>
          <Text className="code-line">{`        ui_empty_media_set_variant(media, UI_EMPTY_MEDIA_VARIANT_ICON);`}</Text>
          <Text className="code-line">{`        ui_icon_set_name(folder, [color=#0a3069]"folder"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(media, folder);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(title, [color=#0a3069]"No Projects Yet"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(`}</Text>
          <Text className="code-line">{`            description,`}</Text>
          <Text className="code-line">{`            [color=#0a3069]"You haven&#x27;t created any projects yet. Get started by creating "[/color]`}</Text>
          <Text className="code-line">{`            [color=#0a3069]"your first project."[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_variant(create_button, UI_BUTTON_VARIANT_PRIMARY);`}</Text>
          <Text className="code-line">{`        ui_button_set_variant(learn_more_button, UI_BUTTON_VARIANT_LINK);`}</Text>
          <Text className="code-line">{`        ui_button_set_size(learn_more_button, UI_BUTTON_SIZE_SM);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(learn_more_button, [color=#0a3069]"text-muted-foreground"[/color]);`}</Text>
          <Text className="code-line">{`        ui_icon_set_name(arrow_up_right, [color=#0a3069]"arrow-up-right"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(learn_more_button, arrow_up_right);`}</Text>
          <Text className="code-line">{`        ui_widget_append(actions, create_button);`}</Text>
          <Text className="code-line">{`        ui_widget_append(actions, import_button);`}</Text>
          <Text className="code-line">{`        ui_widget_append(header, media);`}</Text>
          <Text className="code-line">{`        ui_widget_append(header, title);`}</Text>
          <Text className="code-line">{`        ui_widget_append(header, description);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, actions);`}</Text>
          <Text className="code-line">{`        ui_widget_append(empty, header);`}</Text>
          <Text className="code-line">{`        ui_widget_append(empty, content);`}</Text>
          <Text className="code-line">{`        ui_widget_append(empty, learn_more_button);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, empty);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/empty-basic/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]empty[/color] [color=#0550ae]class[/color]=[color=#0a3069]"empty-basic-demo"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]empty-header[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]empty-media[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"icon"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"folder"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"24"[/color] />[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]empty-media[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]empty-title[/color]>[/color]No Projects Yet[color=#116329]</[color=#0550ae]empty-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]empty-description[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]      >[/color]You haven&#x27;t created any projects yet. Get started by creating your first project.[color=#116329]</[color=#0550ae]empty-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]empty-header[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]empty-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]empty-action[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"primary"[/color]>[/color][color=#116329]<[color=#0550ae]text[/color]>[/color]Create Project[color=#116329]</[color=#0550ae]text[/color]>[/color][color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]button[/color]>[/color][color=#116329]<[color=#0550ae]text[/color]>[/color]Import Project[color=#116329]</[color=#0550ae]text[/color]>[/color][color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]empty-action[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]empty-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"text-muted-foreground"[/color]>[/color][color=#116329]<[color=#0550ae]text[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]        >[/color]Learn More[color=#116329]</[color=#0550ae]text[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"arrow-up-right"[/color] />[/color][color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]empty[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/empty-basic/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]empty_basic_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, ui_create_widget([color=#0550ae]NULL[/color]));`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
