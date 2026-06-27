import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function ScrollareaDualScrollbarsDemo() {
  return (
    <DemoProvider>
      <Widget $ref="preview" className="demo-preview" />
      <Widget className="code-block">
        <Widget className="code-block-header">
          <Widget className="demo-files">
            <Text className="demo-tab active" data-language="tsx" data-value="example.tsx">example.tsx</Text>
            <Text className="demo-tab" data-language="tsx" data-value="example.css" style={{ display: "none" }}>example.css</Text>
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/scrollarea-dual-scrollbars/0">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]ScrollArea[/color], [color=#8250df]ScrollAreaContent[/color], [color=#8250df]Scrollbar[/color], [color=#8250df]Text[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{`[color=#cf222e]import[/color] [color=#0a3069]"./example.css"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]ScrollArea[/color] [color=#0550ae]className[/color]=[color=#0a3069]"demo-scrollarea"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]ScrollAreaContent[/color] [color=#0550ae]className[/color]=[color=#0a3069]"demo-grid"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        {Array.from({ length: 100 }, (_, i) => (`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]key[/color]=[color=#0a3069]{i}[/color] [color=#0550ae]className[/color]=[color=#0a3069]"demo-cell"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            {i + 1}`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        ))}`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]ScrollAreaContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Scrollbar[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color] />[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Scrollbar[/color] />[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]ScrollArea[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="example.css" data-source="examples/scrollarea-dual-scrollbars/1" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#953800].demo-scrollarea[/color] {`}</Text>
          <Text className="code-line">{`  width: [color=#0550ae]400px[/color];`}</Text>
          <Text className="code-line">{`  height: [color=#0550ae]300px[/color];`}</Text>
          <Text className="code-line">{`  border: [color=#0550ae]1px[/color] solid [color=#0550ae]#d0d7de[/color];`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800].demo-grid[/color] {`}</Text>
          <Text className="code-line">{`  display: flex;`}</Text>
          <Text className="code-line">{`  flex-wrap: wrap;`}</Text>
          <Text className="code-line">{`  width: [color=#0550ae]1088px[/color];`}</Text>
          <Text className="code-line">{`  height: [color=#0550ae]1088px[/color];`}</Text>
          <Text className="code-line">{`  gap: [color=#0550ae]8px[/color];`}</Text>
          <Text className="code-line">{`  padding: [color=#0550ae]8px[/color];`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800].demo-cell[/color] {`}</Text>
          <Text className="code-line">{`  width: [color=#0550ae]100px[/color];`}</Text>
          <Text className="code-line">{`  height: [color=#0550ae]100px[/color];`}</Text>
          <Text className="code-line">{`  background: [color=#0550ae]#e1e5e9[/color];`}</Text>
          <Text className="code-line">{`  border: [color=#0550ae]1px[/color] solid [color=#0550ae]#b0b8c0[/color];`}</Text>
          <Text className="code-line">{`  text-align: center;`}</Text>
          <Text className="code-line">{`  line-height: [color=#0550ae]100px[/color];`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/scrollarea-dual-scrollbars/2" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]main[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        lcui_init();`}</Text>
          <Text className="code-line">{`        example_load();`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/scrollarea-dual-scrollbars/3" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<stdio.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]scrollarea_dual_scrollbars_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]int[/color] i;`}</Text>
          <Text className="code-line">{`        [color=#953800]char[/color] buf\\[[color=#0550ae]8[/color]];`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *area, *content, *hbar, *vbar, *box;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        area = ui_create_scrollarea();`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(area, [color=#0a3069]"width"[/color], [color=#0a3069]"420px"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(area, [color=#0a3069]"height"[/color], [color=#0a3069]"300px"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(area, [color=#0a3069]"border"[/color], [color=#0a3069]"1px solid #d0d7de"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        content = ui_create_scrollarea_content();`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(content, [color=#0a3069]"display"[/color], [color=#0a3069]"flex"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(content, [color=#0a3069]"flex-wrap"[/color], [color=#0a3069]"wrap"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(content, [color=#0a3069]"width"[/color], [color=#0a3069]"1088px"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(content, [color=#0a3069]"height"[/color], [color=#0a3069]"1088px"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(content, [color=#0a3069]"gap"[/color], [color=#0a3069]"8px"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(content, [color=#0a3069]"padding"[/color], [color=#0a3069]"8px"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        [color=#cf222e]for[/color] (i = [color=#0550ae]0[/color]; i < [color=#0550ae]100[/color]; ++i) {`}</Text>
          <Text className="code-line">{`                [color=#953800]snprintf[/color](buf, [color=#cf222e]sizeof[/color](buf), [color=#0a3069]"%d"[/color], i + [color=#0550ae]1[/color]);`}</Text>
          <Text className="code-line">{`                box = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
          <Text className="code-line">{`                ui_text_set_content(box, buf);`}</Text>
          <Text className="code-line">{`                ui_widget_set_style_string(box, [color=#0a3069]"width"[/color], [color=#0a3069]"100px"[/color]);`}</Text>
          <Text className="code-line">{`                ui_widget_set_style_string(box, [color=#0a3069]"height"[/color], [color=#0a3069]"100px"[/color]);`}</Text>
          <Text className="code-line">{`                ui_widget_set_style_string(box, [color=#0a3069]"background"[/color], [color=#0a3069]"#e1e5e9"[/color]);`}</Text>
          <Text className="code-line">{`                ui_widget_set_style_string(box, [color=#0a3069]"border"[/color], [color=#0a3069]"1px solid #b0b8c0"[/color]);`}</Text>
          <Text className="code-line">{`                ui_widget_set_style_string(box, [color=#0a3069]"text-align"[/color], [color=#0a3069]"center"[/color]);`}</Text>
          <Text className="code-line">{`                ui_widget_set_style_string(box, [color=#0a3069]"line-height"[/color], [color=#0a3069]"100px"[/color]);`}</Text>
          <Text className="code-line">{`                ui_widget_append(content, box);`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        hbar = ui_create_widget([color=#0a3069]"scrollbar"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_attr(hbar, [color=#0a3069]"orientation"[/color], [color=#0a3069]"horizontal"[/color]);`}</Text>
          <Text className="code-line">{`        vbar = ui_create_widget([color=#0a3069]"scrollbar"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(area, content);`}</Text>
          <Text className="code-line">{`        ui_widget_append(area, hbar);`}</Text>
          <Text className="code-line">{`        ui_widget_append(area, vbar);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, area);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/scrollarea-dual-scrollbars/4" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color]?>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]style[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#953800].demo-scrollarea[/color] {`}</Text>
          <Text className="code-line">{`        width: [color=#0550ae]400px[/color];`}</Text>
          <Text className="code-line">{`        height: [color=#0550ae]300px[/color];`}</Text>
          <Text className="code-line">{`        border: [color=#0550ae]1px[/color] solid [color=#0550ae]#d0d7de[/color];`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`    [color=#953800].demo-grid[/color] {`}</Text>
          <Text className="code-line">{`        display: flex;`}</Text>
          <Text className="code-line">{`        flex-wrap: wrap;`}</Text>
          <Text className="code-line">{`        width: [color=#0550ae]1088px[/color];`}</Text>
          <Text className="code-line">{`        height: [color=#0550ae]1088px[/color];`}</Text>
          <Text className="code-line">{`        gap: [color=#0550ae]8px[/color];`}</Text>
          <Text className="code-line">{`        padding: [color=#0550ae]8px[/color];`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`    [color=#953800].demo-cell[/color] {`}</Text>
          <Text className="code-line">{`        width: [color=#0550ae]100px[/color];`}</Text>
          <Text className="code-line">{`        height: [color=#0550ae]100px[/color];`}</Text>
          <Text className="code-line">{`        background: [color=#0550ae]#e1e5e9[/color];`}</Text>
          <Text className="code-line">{`        border: [color=#0550ae]1px[/color] solid [color=#0550ae]#b0b8c0[/color];`}</Text>
          <Text className="code-line">{`        text-align: center;`}</Text>
          <Text className="code-line">{`        line-height: [color=#0550ae]100px[/color];`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]style[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]scrollarea[/color] [color=#0550ae]class[/color]=[color=#0a3069]"demo-scrollarea"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]scrollarea-content[/color] [color=#0550ae]class[/color]=[color=#0a3069]"demo-grid"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"demo-cell"[/color]>[/color]1[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"demo-cell"[/color]>[/color]2[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"demo-cell"[/color]>[/color]3[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"demo-cell"[/color]>[/color]4[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"demo-cell"[/color]>[/color]5[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"demo-cell"[/color]>[/color]6[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"demo-cell"[/color]>[/color]7[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"demo-cell"[/color]>[/color]8[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"demo-cell"[/color]>[/color]9[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"demo-cell"[/color]>[/color]10[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]scrollarea-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]scrollbar[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color] />[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]scrollbar[/color] />[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]scrollarea[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/scrollarea-dual-scrollbars/5" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<ui_xml.h>[/color][/color]`}</Text>
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
