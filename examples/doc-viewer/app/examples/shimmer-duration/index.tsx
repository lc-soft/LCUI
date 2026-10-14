import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function ShimmerDurationDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/shimmer-duration/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Shimmer[/color], [color=#8250df]ShimmerContent[/color], [color=#8250df]Text[/color], [color=#8250df]Widget[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]ShimmerDurationDemo[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-col gap-2"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Shimmer[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-\\[220px] h-7"[/color] [color=#0550ae]running[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]ShimmerContent[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex items-center px-2 py-1 text-\\[#46545f]"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Text[/color]>[/color]Default duration[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]ShimmerContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Shimmer[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Shimmer[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-\\[220px] h-7"[/color] [color=#0550ae]running[/color] [color=#0550ae]duration[/color]=[color=#0a3069]{1000}[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]ShimmerContent[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex items-center px-2 py-1 text-\\[#46545f]"[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Text[/color]>[/color]Faster duration[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]ShimmerContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Shimmer[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/shimmer-duration/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#6e7781]/* example.tsx is compiled to example.h by @lcui/cli */[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]main[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        lcui_init();`}</Text>
          <Text className="code-line">{`        ui_root_append(ui_create_shimmer_duration_demo());`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/shimmer-duration/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#6e7781]/* Two identical sweeps at different durations: the default 2000 ms and a[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * snappier 1000 ms. Duration is only observable while the shimmer is running,[/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781] * so both lines keep the animation on. */[/color]`}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]const[/color] [color=#953800]char[/color] *shimmer_duration_css = [color=#0a3069]"\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069].shimmer-duration-demo {\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  display: flex;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  flex-direction: column;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  gap: 8px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069].shimmer-duration-demo shimmer {\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  width: 220px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  height: 28px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069].shimmer-duration-demo shimmer-content {\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  display: flex;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  align-items: center;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  padding: 4px 8px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  font-size: 15px;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]  color: #46545f;\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
          <Text className="code-line">{`[color=#0a3069]"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]caption[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *label)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *shimmer, *content, *text;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        shimmer = ui_create_shimmer();`}</Text>
          <Text className="code-line">{`        content = ui_create_widget([color=#0a3069]"shimmer-content"[/color]);`}</Text>
          <Text className="code-line">{`        text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
          <Text className="code-line">{`        ui_text_set_content(text, label);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, text);`}</Text>
          <Text className="code-line">{`        ui_shimmer_set_content(shimmer, content);`}</Text>
          <Text className="code-line">{`        ui_shimmer_set_running(shimmer, [color=#0550ae]true[/color]);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] shimmer;`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]shimmer_duration_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *box, *normal, *fast;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_load_css_string(shimmer_duration_css, [color=#0a3069]"shimmer-duration-demo"[/color]);`}</Text>
          <Text className="code-line">{`        box = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_add_class(box, [color=#0a3069]"shimmer-duration-demo"[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        normal = caption([color=#0a3069]"Default duration"[/color]);`}</Text>
          <Text className="code-line">{`        fast = caption([color=#0a3069]"Faster duration"[/color]);`}</Text>
          <Text className="code-line">{`        ui_shimmer_set_duration(fast, [color=#0550ae]1000[/color]);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_append(box, normal);`}</Text>
          <Text className="code-line">{`        ui_widget_append(box, fast);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, box);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/shimmer-duration/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color] ?>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]resource[/color] [color=#0550ae]type[/color]=[color=#0a3069]"text/css"[/color]>[/color]`}</Text>
          <Text className="code-line">{`    .shimmer-duration-demo {`}</Text>
          <Text className="code-line">{`        display: flex;`}</Text>
          <Text className="code-line">{`        flex-direction: column;`}</Text>
          <Text className="code-line">{`        gap: 8px;`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`    .shimmer-duration-demo shimmer {`}</Text>
          <Text className="code-line">{`        width: 220px;`}</Text>
          <Text className="code-line">{`        height: 28px;`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`    .shimmer-duration-demo shimmer-content {`}</Text>
          <Text className="code-line">{`        display: flex;`}</Text>
          <Text className="code-line">{`        align-items: center;`}</Text>
          <Text className="code-line">{`        padding: 4px 8px;`}</Text>
          <Text className="code-line">{`        font-size: 15px;`}</Text>
          <Text className="code-line">{`        color: #46545f;`}</Text>
          <Text className="code-line">{`    }`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]resource[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]widget[/color] [color=#0550ae]class[/color]=[color=#0a3069]"shimmer-duration-demo"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]shimmer[/color] [color=#0550ae]running[/color]=[color=#0a3069]"true"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]shimmer-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color]>[/color]Default duration[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]shimmer-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]shimmer[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]shimmer[/color] [color=#0550ae]running[/color]=[color=#0a3069]"true"[/color] [color=#0550ae]duration[/color]=[color=#0a3069]"1000"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]shimmer-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color]>[/color]Faster duration[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]shimmer-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]shimmer[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]widget[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/shimmer-duration/9" style={{ display: "none" }}>
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
