import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function ScrollareaBasicDemo() {
  return (
    <DemoProvider>
      <Widget $ref="preview" className="demo-preview" />
      <Widget className="code-block">
        <Widget className="code-block-header">
          <Widget className="demo-files">
            <Text className="demo-tab active" data-language="tsx" data-value="example.tsx">example.tsx</Text>
            <Text className="demo-tab" data-language="tsx" data-value="main.c" style={{ display: "none" }}>main.c</Text>
            <Text className="demo-tab" data-language="tsx" data-value="example.css" style={{ display: "none" }}>example.css</Text>
            <Text className="demo-tab" data-language="c" data-value="main.c" style={{ display: "none" }}>main.c</Text>
            <Text className="demo-tab" data-language="xml" data-value="main.c" style={{ display: "none" }}>main.c</Text>
            <Text className="demo-tab" data-language="xml" data-value="ui.xml" style={{ display: "none" }}>ui.xml</Text>
          </Widget>
          <Widget className="demo-languages">
            <Text className="demo-tab active" data-value="tsx">TSX</Text>
            <Text className="demo-tab" data-value="c">C</Text>
            <Text className="demo-tab" data-value="xml">XML</Text>
          </Widget>
        </Widget>
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/scrollarea-basic/6">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]ScrollArea[/color], [color=#8250df]ScrollAreaContent[/color], [color=#8250df]Scrollbar[/color], [color=#8250df]Text[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{`[color=#cf222e]import[/color] [color=#0a3069]"./example.css"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]const[/color] content = [color=#0a3069]\`这是一段用于演示滚动区域功能的示例文本。[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]当文本内容超出容器高度时，用户可以通过滚动条来查看其余内容。[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]滚动区域适合阅读长篇文章、展示数据列表。[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]在实际项目中，滚动区域的尺寸通常由其父布局决定，开发者只需关注内容本身。[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]滚动区域可以嵌套使用，构建复杂的多层滚动界面。[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]合理配置滚动方向和样式，可以让界面更加整洁且易于使用。\`[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]ScrollArea[/color] [color=#0550ae]className[/color]=[color=#0a3069]"demo-scrollarea"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]ScrollAreaContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Text[/color]>[/color]{content}[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]ScrollAreaContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Scrollbar[/color] />[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]ScrollArea[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/scrollarea-basic/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI/widgets.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]main[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        lcui_init();`}</Text>
          <Text className="code-line">{`        example_load();`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="example.css" data-source="examples/scrollarea-basic/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#953800].demo-scrollarea[/color] {`}</Text>
          <Text className="code-line">{`  width: [color=#0550ae]300px[/color];`}</Text>
          <Text className="code-line">{`  height: [color=#0550ae]200px[/color];`}</Text>
          <Text className="code-line">{`  border: [color=#0550ae]1px[/color] solid [color=#0550ae]#d0d7de[/color];`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/scrollarea-basic/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`﻿[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI/widgets.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]scrollarea_basic_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *scroll, *content, *vbar, *text;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        scroll = ui_create_scrollarea();`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(scroll, [color=#0a3069]"width"[/color], [color=#0a3069]"300px"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(scroll, [color=#0a3069]"height"[/color], [color=#0a3069]"200px"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_style_string(scroll, [color=#0a3069]"border"[/color], [color=#0a3069]"1px solid #d0d7de"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        content = ui_create_scrollarea_content();`}</Text>
          <Text className="code-line">{`        text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content_w(`}</Text>
          <Text className="code-line">{`            text,`}</Text>
          <Text className="code-line">{`            [color=#0a3069]L"这是一段用于演示滚动区域功能的示例文本。\\n"[/color]`}</Text>
          <Text className="code-line">{`            [color=#0a3069]L"当文本内容超出容器高度时，用户可以通过滚动条来查看其余内容。\\n"[/color]`}</Text>
          <Text className="code-line">{`            [color=#0a3069]L"滚动区域适合阅读长篇文章、展示数据列表。\\n"[/color]`}</Text>
          <Text className="code-line">{`            [color=#0a3069]L"在实际项目中，滚动区域的尺寸通常由其父布局决定，"[/color]`}</Text>
          <Text className="code-line">{`            [color=#0a3069]L"开发者只需关注内容本身。\\n"[/color]`}</Text>
          <Text className="code-line">{`            [color=#0a3069]L"滚动区域可以嵌套使用，构建复杂的多层滚动界面。\\n"[/color]`}</Text>
          <Text className="code-line">{`            [color=#0a3069]L"合理配置滚动方向和样式，可以让界面更加整洁且易于使用。"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, text);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        vbar = ui_create_widget([color=#0a3069]"scrollbar"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(scroll, content);`}</Text>
          <Text className="code-line">{`        ui_widget_append(scroll, vbar);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, scroll);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/scrollarea-basic/10" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI/widgets.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<ui_xml.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]main[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        lcui_init();`}</Text>
          <Text className="code-line">{`        ui_load_xml_file([color=#0a3069]"ui.xml"[/color]);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/scrollarea-basic/11" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color]?>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]scrollarea[/color] [color=#0550ae]style[/color]=[color=#0a3069]"width: 300px; height: 200px;"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]text[/color]>[/color]Item 1[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]text[/color]>[/color]Item 2[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]text[/color]>[/color]Item 3[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]text[/color]>[/color]Item 4[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]text[/color]>[/color]Item 5[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]text[/color]>[/color]Item 6[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]text[/color]>[/color]Item 7[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]text[/color]>[/color]Item 8[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]text[/color]>[/color]Item 9[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]text[/color]>[/color]Item 10[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]scrollarea[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
