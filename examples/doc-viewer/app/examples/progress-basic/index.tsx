import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function ProgressBasicDemo() {
  return (
    <DemoProvider>
      <Widget $ref="preview" className="demo-preview" />
      <Widget className="code-block">
        <Widget className="code-block-header">
          <Widget className="demo-files">
            <Text className="demo-tab active" data-language="tsx" data-value="example.tsx">example.tsx</Text>
            <Text className="demo-tab" data-language="tsx" data-value="example.c" style={{ display: "none" }}>example.c</Text>
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/progress-basic/6">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Progress[/color], useState, [color=#8250df]CType[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]ProgressDemo[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]const[/color] \\[value] = [color=#8250df]useState[/color]([color=#0550ae]20[/color], [color=#8250df]CType[/color].[color=#0550ae]Int[/color]);`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] [color=#116329]<[color=#0550ae]Progress[/color] [color=#0550ae]value[/color]=[color=#0a3069]{value}[/color] />[/color];`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="example.c" data-source="examples/progress-basic/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.tsx.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]typedef[/color] [color=#cf222e]struct[/color] {`}</Text>
          <Text className="code-line">{`        [color=#953800]progress_demo_react_t[/color] base;`}</Text>
          <Text className="code-line">{`        [color=#953800]int[/color] timer_id;`}</Text>
          <Text className="code-line">{`} [color=#953800]progress_demo_t[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]on_timer[/color][color=#0550ae]([color=#953800]void[/color] *arg)[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]progress_demo_react_init_state[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]progress_demo_t[/color] *_that = ui_widget_get_data(w, progress_demo_proto);`}</Text>
          <Text className="code-line">{`        _that->state.value = [color=#0550ae]20[/color];`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]on_timer[/color][color=#0550ae]([color=#953800]void[/color] *arg)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *w = arg;`}</Text>
          <Text className="code-line">{`        [color=#953800]progress_demo_t[/color] *_that = ui_widget_get_data(w, progress_demo_proto);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        _that->state.value += [color=#0550ae]10[/color];`}</Text>
          <Text className="code-line">{`        [color=#cf222e]if[/color] (_that->state.value > [color=#0550ae]80[/color]) {`}</Text>
          <Text className="code-line">{`                [color=#cf222e]if[/color] (_that->timer_id >= [color=#0550ae]0[/color]) {`}</Text>
          <Text className="code-line">{`                        ptk_clear_timer(_that->timer_id);`}</Text>
          <Text className="code-line">{`                        _that->timer_id = [color=#0550ae]-1[/color];`}</Text>
          <Text className="code-line">{`                }`}</Text>
          <Text className="code-line">{`                [color=#cf222e]return[/color];`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`        progress_demo_react_update(w);`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]progress_demo_react_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]progress_demo_t[/color] *_that = ui_widget_get_data(w, progress_demo_proto);`}</Text>
          <Text className="code-line">{`        progress_demo_load_template(w);`}</Text>
          <Text className="code-line">{`        progress_demo_react_init_state(w);`}</Text>
          <Text className="code-line">{`        _that->timer_id = ptk_set_interval([color=#0550ae]500[/color], on_timer, w);`}</Text>
          <Text className="code-line">{`        progress_demo_react_update(w);`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]progress_demo_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]progress_demo_t[/color] *_that =`}</Text>
          <Text className="code-line">{`            ui_widget_add_data(w, root_page_proto, [color=#cf222e]sizeof[/color]([color=#953800]root_page_t[/color]));`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        root_page_react_init(w);`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]progress_demo_destroy[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]progress_demo_t[/color] *_that = ui_widget_get_data(w, progress_demo_proto);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]if[/color] (_that->timer_id >= [color=#0550ae]0[/color]) {`}</Text>
          <Text className="code-line">{`                ptk_clear_timer(_that->timer_id);`}</Text>
          <Text className="code-line">{`                _that->timer_id = [color=#0550ae]-1[/color];`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`        progress_demo_react_update(w);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/progress-basic/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]main[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        lcui_init();`}</Text>
          <Text className="code-line">{`        ui_root_append(ui_create_progress_demo());`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/progress-basic/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]int[/color] timer_id = [color=#0550ae]0[/color];`}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]float[/color] progress_value = [color=#0550ae]20.0f[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]progress_basic_destroy[/color][color=#0550ae]([color=#953800]void[/color])[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]on_timer[/color][color=#0550ae]([color=#953800]void[/color] *arg)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *progress = arg;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        progress_value += [color=#0550ae]10.0f[/color];`}</Text>
          <Text className="code-line">{`        [color=#cf222e]if[/color] (progress_value > [color=#0550ae]80.0f[/color]) {`}</Text>
          <Text className="code-line">{`                progress_basic_destroy();`}</Text>
          <Text className="code-line">{`                [color=#cf222e]return[/color];`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`        ui_progress_set_value(progress, progress_value);`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]progress_basic_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *progress;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        progress = ui_create_progress();`}</Text>
          <Text className="code-line">{`        ui_progress_set_value(progress, progress_value);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, progress);`}</Text>
          <Text className="code-line">{`        timer_id = ptk_set_interval([color=#0550ae]500[/color], on_timer, progress);`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]progress_basic_destroy[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#cf222e]if[/color] (timer_id) {`}</Text>
          <Text className="code-line">{`                ptk_clear_timer(timer_id);`}</Text>
          <Text className="code-line">{`                timer_id = [color=#0550ae]0[/color];`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/progress-basic/10" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color]?>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]progress[/color] [color=#0550ae]value[/color]=[color=#0a3069]"60"[/color] />[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/progress-basic/11" style={{ display: "none" }}>
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
