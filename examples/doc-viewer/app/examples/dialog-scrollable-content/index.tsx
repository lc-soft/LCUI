import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function DialogScrollableContentDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/dialog-scrollable-content/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Dialog[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogTrigger[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogOverlay[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogHeader[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogTitle[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]DialogDescription[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]ScrollArea[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]ScrollAreaContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Scrollbar[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Text[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]Portal[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Example[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Dialog[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]DialogTrigger[/color]>[/color]Scrollable Content[color=#116329]</[color=#0550ae]DialogTrigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]DialogOverlay[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DialogContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DialogHeader[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DialogTitle[/color]>[/color]Scrollable Content[color=#116329]</[color=#0550ae]DialogTitle[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DialogDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`                This is a dialog with scrollable content.`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]DialogDescription[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]DialogHeader[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]ScrollArea[/color] [color=#0550ae]className[/color]=[color=#0a3069]"h-50"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]ScrollAreaContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Text[/color]>[/color]Lorem ipsum dolor sit amet, consectetur adipiscing elit.[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  Sed do eiusmod tempor incididunt ut labore et dolore magna`}</Text>
          <Text className="code-line">{`                  aliqua.`}</Text>
          <Text className="code-line">{`                [color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  Ut enim ad minim veniam, quis nostrud exercitation ullamco`}</Text>
          <Text className="code-line">{`                  laboris.`}</Text>
          <Text className="code-line">{`                [color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  Duis aute irure dolor in reprehenderit in voluptate velit`}</Text>
          <Text className="code-line">{`                  esse cillum dolore.`}</Text>
          <Text className="code-line">{`                [color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`                  Excepteur sint occaecat cupidatat non proident, sunt in culpa`}</Text>
          <Text className="code-line">{`                  qui officia deserunt mollit anim id est laborum.`}</Text>
          <Text className="code-line">{`                [color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]ScrollAreaContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Scrollbar[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]ScrollArea[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DialogContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]DialogOverlay[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Dialog[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/dialog-scrollable-content/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]dialog_scrollable_content_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, example_load());`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/dialog-scrollable-content/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]define[/color] LOREM                                                                  \\[/color]`}</Text>
          <Text className="code-line">{`        [color=#0a3069]"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do "[/color]     \\`}</Text>
          <Text className="code-line">{`        [color=#0a3069]"eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim "[/color] \\`}</Text>
          <Text className="code-line">{`        [color=#0a3069]"ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut "[/color]  \\`}</Text>
          <Text className="code-line">{`        [color=#0a3069]"aliquip ex ea commodo consequat."[/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]make_scrolling_body[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *scroll = ui_create_scrollarea();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_scrollarea_content();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *vbar = ui_create_widget([color=#0a3069]"scrollbar"[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *box, *text;`}</Text>
          <Text className="code-line">{`        [color=#953800]int[/color] i;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(scroll, [color=#0a3069]"height"[/color], [color=#0a3069]"200px"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(scroll, [color=#0a3069]"width"[/color], [color=#0a3069]"100%"[/color]);`}</Text>
          <Text className="code-line">{`        box = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(box, [color=#0a3069]"display"[/color], [color=#0a3069]"flex"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(box, [color=#0a3069]"flex-direction"[/color], [color=#0a3069]"column"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(box, [color=#0a3069]"gap"[/color], [color=#0a3069]"16px"[/color]);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]for[/color] (i = [color=#0550ae]0[/color]; i < [color=#0550ae]10[/color]; ++i) {`}</Text>
          <Text className="code-line">{`                text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
          <Text className="code-line">{`                ui_text_set_content(text, LOREM);`}</Text>
          <Text className="code-line">{`                ui_widget_append(box, text);`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, box);`}</Text>
          <Text className="code-line">{`        ui_widget_append(scroll, content);`}</Text>
          <Text className="code-line">{`        ui_widget_append(scroll, vbar);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] scroll;`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]dialog_scrollable_content_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *dialog = ui_create_dialog();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_dialog_show_close_button(dialog, [color=#0550ae]false[/color]);`}</Text>
          <Text className="code-line">{`        ui_dialog_build_children(dialog);`}</Text>
          <Text className="code-line">{`        ui_dialog_set_trigger_text(dialog, [color=#0a3069]"Scrollable Content"[/color]);`}</Text>
          <Text className="code-line">{`        ui_dialog_set_title(dialog, [color=#0a3069]"Scrollable Content"[/color]);`}</Text>
          <Text className="code-line">{`        ui_dialog_set_description(dialog,`}</Text>
          <Text className="code-line">{`                                  [color=#0a3069]"This is a dialog with scrollable content."[/color]);`}</Text>
          <Text className="code-line">{`        [color=#6e7781]/* This dialog has no action buttons: drop the default footer. */[/color]`}</Text>
          <Text className="code-line">{`        ui_widget_remove(ui_dialog_get_footer_button(dialog, [color=#0550ae]false[/color])->parent);`}</Text>
          <Text className="code-line">{`        ui_dialog_append_body(dialog, make_scrolling_body());`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, dialog);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/dialog-scrollable-content/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]dialog[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]dialog-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]Scrollable Content[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]dialog-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dialog-overlay[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dialog-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dialog-header[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dialog-title[/color]>[/color]Scrollable Content[color=#116329]</[color=#0550ae]dialog-title[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dialog-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`                This is a dialog with scrollable content.`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]dialog-description[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]dialog-header[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]scrollarea[/color] [color=#0550ae]style[/color]=[color=#0a3069]"height: 200px"[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]scrollarea-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]                  >[/color]Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed`}</Text>
          <Text className="code-line">{`                  do eiusmod tempor incididunt ut labore et dolore magna`}</Text>
          <Text className="code-line">{`                  aliqua.</text`}</Text>
          <Text className="code-line">{`                >`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]                  >[/color]Ut enim ad minim veniam, quis nostrud exercitation ullamco`}</Text>
          <Text className="code-line">{`                  laboris nisi ut aliquip ex ea commodo consequat.</text`}</Text>
          <Text className="code-line">{`                >`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]                  >[/color]Duis aute irure dolor in reprehenderit in voluptate velit`}</Text>
          <Text className="code-line">{`                  esse cillum dolore eu fugiat nulla pariatur.</text`}</Text>
          <Text className="code-line">{`                >`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]                  >[/color]Excepteur sint occaecat cupidatat non proident, sunt in`}</Text>
          <Text className="code-line">{`                  culpa qui officia deserunt mollit anim id est laborum.</text`}</Text>
          <Text className="code-line">{`                >`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]                  >[/color]Sed ut perspiciatis unde omnis iste natus error sit`}</Text>
          <Text className="code-line">{`                  voluptatem accusantium doloremque laudantium.</text`}</Text>
          <Text className="code-line">{`                >`}</Text>
          <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]                  >[/color]Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut`}</Text>
          <Text className="code-line">{`                  odit aut fugit.</text`}</Text>
          <Text className="code-line">{`                >`}</Text>
          <Text className="code-line">{`              [color=#116329]</[color=#0550ae]scrollarea-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`              [color=#116329]<[color=#0550ae]scrollbar[/color] />[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]scrollarea[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]dialog-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dialog-overlay[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]dialog[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/dialog-scrollable-content/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]dialog_scrollable_content_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, ui_create_widget([color=#0550ae]NULL[/color]));`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
