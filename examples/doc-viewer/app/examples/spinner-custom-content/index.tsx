import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function SpinnerCustomContentDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/spinner-custom-content/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Spinner[/color], [color=#8250df]SpinnerContent[/color], [color=#8250df]Text[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]SpinnerCustomContentDemo[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-row items-center gap-7"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Spinner[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-10 h-10"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]SpinnerContent[/color] [color=#0550ae]className[/color]=[color=#0a3069]"relative flex items-center justify-center"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]className[/color]=[color=#0a3069]"absolute left-\\[15px] top-\\[3px] w-\\[10px] h-\\[10px] rounded-full bg-blue-500"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]className[/color]=[color=#0a3069]"absolute left-\\[18px] top-\\[6px] w-1 h-5 rounded bg-red-500"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]SpinnerContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Spinner[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Spinner[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-\\[96px] h-8"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]SpinnerContent[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex items-center justify-center"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"whitespace-nowrap text-lg text-\\[#e5484d]"[/color]>[/color]`}</Text>
          <Text className="code-line">{`            Hello`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]SpinnerContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Spinner[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/spinner-custom-content/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#6e7781]/* example.tsx is compiled to example.h by @lcui/cli */[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]main[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        lcui_init();`}</Text>
          <Text className="code-line">{`        ui_root_append(ui_create_spinner_custom_content_demo());`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/spinner-custom-content/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]const[/color] [color=#953800]char[/color] *spinner_custom_content_css = [color=#0a3069]"\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069].spinner-custom-content-demo {\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  display: flex;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  align-items: center;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  gap: 28px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069].spinner-custom-content-demo spinner-content {\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  display: flex;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  align-items: center;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  justify-content: center;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069].spinner-custom-content-demo spinner-content .c-dot {\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  position: absolute;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  left: 15px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  top: 3px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  width: 10px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  height: 10px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  background-color: #4a90d9;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  border-radius: 8px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069].spinner-custom-content-demo spinner-content .c-bar {\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  position: absolute;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  left: 18px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  top: 6px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  width: 4px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  height: 20px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  background-color: #d9534f;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  border-radius: 2px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069].spinner-custom-content-demo spinner-content .c-text {\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  white-space: nowrap;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  font-size: 18px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  color: #e5484d;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_shape_content[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content, *dot, *bar;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        content = ui_create_widget([color=#0a3069]"spinner-content"[/color]);`}</Text>
          <Text className="code-line">{`        dot = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        bar = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(dot, [color=#0a3069]"c-dot"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(bar, [color=#0a3069]"c-bar"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, dot);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, bar);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] content;`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_text_content[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content, *text;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        content = ui_create_widget([color=#0a3069]"spinner-content"[/color]);`}</Text>
          <Text className="code-line">{`        text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(text, [color=#0a3069]"Hello"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(text, [color=#0a3069]"c-text"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, text);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] content;`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]spinner_custom_content_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *row, *spinner;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_load_css_string(spinner_custom_content_css,`}</Text>
          <Text className="code-line">{`                           [color=#0a3069]"spinner-custom-content-demo"[/color]);`}</Text>
          <Text className="code-line">{`        row = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(row, [color=#0a3069]"spinner-custom-content-demo"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        spinner = ui_create_spinner();`}</Text>
          <Text className="code-line">{`        ui_widget_resize(spinner, [color=#0550ae]40[/color], [color=#0550ae]40[/color]);`}</Text>
          <Text className="code-line">{`        ui_spinner_set_content(spinner, create_shape_content());`}</Text>
          <Text className="code-line">{`        ui_widget_append(row, spinner);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        spinner = ui_create_spinner();`}</Text>
          <Text className="code-line">{`        ui_widget_resize(spinner, [color=#0550ae]96[/color], [color=#0550ae]32[/color]);`}</Text>
          <Text className="code-line">{`        ui_spinner_set_content(spinner, create_text_content());`}</Text>
          <Text className="code-line">{`        ui_widget_append(row, spinner);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, row);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/spinner-custom-content/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color] ?>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]resource[/color] [color=#0550ae]type[/color]=[color=#0a3069]"text/css"[/color]>[/color]`}</Text>
          <Text className="code-line">{`    .spinner-custom-content-demo {`}</Text>
          <Text className="code-line">{`        display: flex;`}</Text>
          <Text className="code-line">{`        align-items: center;`}</Text>
          <Text className="code-line">{`        gap: 28px;`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`    .spinner-custom-content-demo .shape {`}</Text>
          <Text className="code-line">{`        width: 40px;`}</Text>
          <Text className="code-line">{`        height: 40px;`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`    .spinner-custom-content-demo .word {`}</Text>
          <Text className="code-line">{`        width: 96px;`}</Text>
          <Text className="code-line">{`        height: 32px;`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`    .spinner-custom-content-demo spinner-content {`}</Text>
          <Text className="code-line">{`        display: flex;`}</Text>
          <Text className="code-line">{`        align-items: center;`}</Text>
          <Text className="code-line">{`        justify-content: center;`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`    .spinner-custom-content-demo .c-dot {`}</Text>
          <Text className="code-line">{`        position: absolute;`}</Text>
          <Text className="code-line">{`        left: 15px;`}</Text>
          <Text className="code-line">{`        top: 3px;`}</Text>
          <Text className="code-line">{`        width: 10px;`}</Text>
          <Text className="code-line">{`        height: 10px;`}</Text>
          <Text className="code-line">{`        background-color: #4a90d9;`}</Text>
          <Text className="code-line">{`        border-radius: 8px;`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`    .spinner-custom-content-demo .c-bar {`}</Text>
          <Text className="code-line">{`        position: absolute;`}</Text>
          <Text className="code-line">{`        left: 18px;`}</Text>
          <Text className="code-line">{`        top: 6px;`}</Text>
          <Text className="code-line">{`        width: 4px;`}</Text>
          <Text className="code-line">{`        height: 20px;`}</Text>
          <Text className="code-line">{`        background-color: #d9534f;`}</Text>
          <Text className="code-line">{`        border-radius: 2px;`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`    .spinner-custom-content-demo .c-text {`}</Text>
          <Text className="code-line">{`        white-space: nowrap;`}</Text>
          <Text className="code-line">{`        font-size: 18px;`}</Text>
          <Text className="code-line">{`        color: #e5484d;`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]resource[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]widget[/color] [color=#0550ae]class[/color]=[color=#0a3069]"spinner-custom-content-demo"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]spinner[/color] [color=#0550ae]class[/color]=[color=#0a3069]"shape"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]spinner-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]w[/color] [color=#0550ae]class[/color]=[color=#0a3069]"c-dot"[/color] />[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]w[/color] [color=#0550ae]class[/color]=[color=#0a3069]"c-bar"[/color] />[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]spinner-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]spinner[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]spinner[/color] [color=#0550ae]class[/color]=[color=#0a3069]"word"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]spinner-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"c-text"[/color]>[/color]Hello[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]spinner-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]spinner[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/spinner-custom-content/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
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
