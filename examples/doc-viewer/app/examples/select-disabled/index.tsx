import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function SelectDisabledDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/select-disabled/5">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
          <Text className="code-line">{`  [color=#8250df]Select[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]SelectContent[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]SelectGroup[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]SelectItem[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]SelectTrigger[/color],`}</Text>
          <Text className="code-line">{`  [color=#8250df]SelectValue[/color],`}</Text>
          <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]const[/color] items = \\[`}</Text>
          <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Apple"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"apple"[/color] },`}</Text>
          <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Banana"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"banana"[/color] },`}</Text>
          <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Blueberry"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"blueberry"[/color] },`}</Text>
          <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Grapes"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"grapes"[/color], [color=#0550ae]disabled[/color]: [color=#0550ae]true[/color] },`}</Text>
          <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Pineapple"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"pineapple"[/color] },`}</Text>
          <Text className="code-line">{`];`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Example[/color]([color=#0550ae][/color]) {`}</Text>
          <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Select[/color] [color=#0550ae]disabled[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]SelectTrigger[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-full max-w-48"[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]SelectValue[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"Select a fruit"[/color] />[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]SelectTrigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`          {items.map((item) => (`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]SelectItem[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]key[/color]=[color=#0a3069]{item.value}[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]value[/color]=[color=#0a3069]{item.value}[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]              [color=#0550ae]disabled[/color]=[color=#0a3069]{item.disabled}[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#116329]            >[/color]`}</Text>
          <Text className="code-line">{`              {item.label}`}</Text>
          <Text className="code-line">{`            [color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
          <Text className="code-line">{`          ))}`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Select[/color]>[/color]`}</Text>
          <Text className="code-line">{`  );`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/select-disabled/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]select_disabled_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, example_load());`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/select-disabled/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]add_item[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *group, [color=#953800]const[/color] [color=#953800]char[/color] *label,[/color]`}</Text>
          <Text className="code-line">{`[color=#0550ae]                             [color=#953800]const[/color] [color=#953800]char[/color] *value)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *item = ui_create_select_item(value);`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_text_set_content(item, label);`}</Text>
          <Text className="code-line">{`        ui_widget_append(group, item);`}</Text>
          <Text className="code-line">{`        [color=#cf222e]return[/color] item;`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]select_disabled_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *select = ui_create_select();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *trigger = ui_create_select_trigger();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_select_content();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *group = ui_create_select_group();`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *grapes;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_add_class(select, [color=#0a3069]"select-disabled-demo"[/color]);`}</Text>
          <Text className="code-line">{`        ui_select_set_placeholder(select, [color=#0a3069]"Select a fruit"[/color]);`}</Text>
          <Text className="code-line">{`        add_item(group, [color=#0a3069]"Apple"[/color], [color=#0a3069]"apple"[/color]);`}</Text>
          <Text className="code-line">{`        add_item(group, [color=#0a3069]"Banana"[/color], [color=#0a3069]"banana"[/color]);`}</Text>
          <Text className="code-line">{`        add_item(group, [color=#0a3069]"Blueberry"[/color], [color=#0a3069]"blueberry"[/color]);`}</Text>
          <Text className="code-line">{`        grapes = add_item(group, [color=#0a3069]"Grapes"[/color], [color=#0a3069]"grapes"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_set_disabled(grapes, [color=#0550ae]true[/color]);`}</Text>
          <Text className="code-line">{`        add_item(group, [color=#0a3069]"Pineapple"[/color], [color=#0a3069]"pineapple"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(content, group);`}</Text>
          <Text className="code-line">{`        ui_widget_append(select, trigger);`}</Text>
          <Text className="code-line">{`        ui_widget_append(select, content);`}</Text>
          <Text className="code-line">{`        ui_select_build_children(select);`}</Text>
          <Text className="code-line">{`        ui_widget_set_disabled(select, [color=#0550ae]true[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, select);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/select-disabled/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]<[color=#0550ae]select[/color] [color=#0550ae]class[/color]=[color=#0a3069]"select-disabled-demo"[/color] [color=#0550ae]disabled[/color]=[color=#0a3069]"true"[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]select-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]select-value[/color] [color=#0550ae]data-select-placeholder[/color]=[color=#0a3069]"Select a fruit"[/color] />[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]select-trigger[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]<[color=#0550ae]portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]<[color=#0550ae]select-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]<[color=#0550ae]select-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"apple"[/color]>[/color]Apple[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"banana"[/color]>[/color]Banana[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"blueberry"[/color]>[/color]Blueberry[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"grapes"[/color] [color=#0550ae]disabled[/color]=[color=#0a3069]"true"[/color]>[/color]Grapes[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"pineapple"[/color]>[/color]Pineapple[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
          <Text className="code-line">{`          [color=#116329]</[color=#0550ae]select-group[/color]>[/color]`}</Text>
          <Text className="code-line">{`        [color=#116329]</[color=#0550ae]select-content[/color]>[/color]`}</Text>
          <Text className="code-line">{`      [color=#116329]</[color=#0550ae]portal[/color]>[/color]`}</Text>
          <Text className="code-line">{`    [color=#116329]</[color=#0550ae]select[/color]>[/color]`}</Text>
          <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
          <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/select-disabled/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]select_disabled_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, ui_create_widget([color=#0550ae]NULL[/color]));`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
