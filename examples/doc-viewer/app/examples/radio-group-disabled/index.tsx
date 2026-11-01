import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function RadioGroupDisabledDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/radio-group-disabled/0">
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-line-row">
                <Text className="code-line-number">1</Text>
                <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]RadioGroup[/color], [color=#8250df]RadioGroupItem[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">2</Text>
                <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Label[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">3</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`    [color=#116329]<[color=#0550ae]RadioGroup[/color] [color=#0550ae]value[/color]=[color=#0a3069]"option2"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"radio-group-demo"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-row items-center gap-3 flex-nowrap"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"disabled"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"rd1"[/color] [color=#0550ae]disabled[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"rd1"[/color]>[/color]Disabled[color=#116329]</[color=#0550ae]Label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-row items-center gap-3 flex-nowrap"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"option2"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"rd2"[/color] [color=#0550ae]checked[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"rd2"[/color]>[/color]Option 2[color=#116329]</[color=#0550ae]Label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-row items-center gap-3 flex-nowrap"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"option3"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"rd3"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"rd3"[/color]>[/color]Option 3[color=#116329]</[color=#0550ae]Label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`    [color=#116329]</[color=#0550ae]RadioGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{`  );`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/radio-group-disabled/1" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-line-row">
                <Text className="code-line-number">1</Text>
                <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">2</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">3</Text>
                <Text className="code-line">{`[color=#6e7781]/* example.tsx is compiled to example.h by @lcui/cli */[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]main[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{`        lcui_init();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`        example_load();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/radio-group-disabled/2" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-line-row">
                <Text className="code-line-number">1</Text>
                <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">2</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">3</Text>
                <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]radio_group_disabled_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *group;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *rows\\[[color=#0550ae]3[/color]];`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *items\\[[color=#0550ae]3[/color]];`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *labels\\[[color=#0550ae]3[/color]];`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`        [color=#953800]const[/color] [color=#953800]char[/color] *ids\\[[color=#0550ae]3[/color]] = { [color=#0a3069]"rd1"[/color], [color=#0a3069]"rd2"[/color], [color=#0a3069]"rd3"[/color] };`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`        [color=#953800]const[/color] [color=#953800]char[/color] *values\\[[color=#0550ae]3[/color]] = { [color=#0a3069]"disabled"[/color], [color=#0a3069]"option2"[/color], [color=#0a3069]"option3"[/color] };`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`        [color=#953800]const[/color] [color=#953800]char[/color] *texts\\[[color=#0550ae]3[/color]] = { [color=#0a3069]"Disabled"[/color], [color=#0a3069]"Option 2"[/color], [color=#0a3069]"Option 3"[/color] };`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`        [color=#953800]int[/color] i;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`        group = ui_create_radio_group();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`        ui_widget_add_class(group, [color=#0a3069]"radio-group-demo"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`        ui_widget_set_attr(group, [color=#0a3069]"value"[/color], [color=#0a3069]"option2"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`        [color=#cf222e]for[/color] (i = [color=#0550ae]0[/color]; i < [color=#0550ae]3[/color]; ++i) {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`                rows\\[i] = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{`                ui_widget_set_style_string(rows\\[i], [color=#0a3069]"display"[/color], [color=#0a3069]"flex"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`                ui_widget_set_style_string(rows\\[i], [color=#0a3069]"flex-direction"[/color], [color=#0a3069]"row"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`                ui_widget_set_style_string(rows\\[i], [color=#0a3069]"align-items"[/color], [color=#0a3069]"center"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`                ui_widget_set_style_string(rows\\[i], [color=#0a3069]"gap"[/color], [color=#0a3069]"8px"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`                ui_widget_set_style_string(rows\\[i], [color=#0a3069]"flex-wrap"[/color], [color=#0a3069]"nowrap"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`                items\\[i] = ui_create_radio_group_item();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`                ui_widget_set_id(items\\[i], ids\\[i]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{`                ui_widget_set_attr(items\\[i], [color=#0a3069]"value"[/color], values\\[i]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`                [color=#cf222e]if[/color] (i == [color=#0550ae]0[/color]) {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{`                        ui_widget_set_disabled(items\\[i], [color=#0550ae]true[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{`                }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`                [color=#cf222e]if[/color] (i == [color=#0550ae]1[/color]) {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`                        ui_widget_set_attr(items\\[i], [color=#0a3069]"checked"[/color], [color=#0a3069]"true"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`                }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">35</Text>
                <Text className="code-line">{`                ui_widget_append(rows\\[i], items\\[i]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">36</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">37</Text>
                <Text className="code-line">{`                labels\\[i] = ui_create_label();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">38</Text>
                <Text className="code-line">{`                ui_label_set_for(labels\\[i], ids\\[i]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">39</Text>
                <Text className="code-line">{`                ui_text_set_content(labels\\[i], texts\\[i]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">40</Text>
                <Text className="code-line">{`                ui_widget_append(rows\\[i], labels\\[i]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">41</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">42</Text>
                <Text className="code-line">{`                ui_widget_append(group, rows\\[i]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">43</Text>
                <Text className="code-line">{`        }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">44</Text>
                <Text className="code-line">{`        ui_widget_append(parent, group);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">45</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/radio-group-disabled/3" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-line-row">
                <Text className="code-line-number">1</Text>
                <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color] ?>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">2</Text>
                <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">3</Text>
                <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`    [color=#116329]<[color=#0550ae]radio-group[/color] [color=#0550ae]class[/color]=[color=#0a3069]"radio-group-demo"[/color] [color=#0550ae]value[/color]=[color=#0a3069]"option2"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]style[/color]=[color=#0a3069]"display:flex; flex-direction:row; align-items:center; gap:8px; flex-wrap:nowrap"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]radio-group-item[/color] [color=#0550ae]id[/color]=[color=#0a3069]"rd1"[/color] [color=#0550ae]value[/color]=[color=#0a3069]"disabled"[/color] [color=#0550ae]disabled[/color]=[color=#0a3069]"true"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"rd1"[/color]>[/color]Disabled[color=#116329]</[color=#0550ae]label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]style[/color]=[color=#0a3069]"display:flex; flex-direction:row; align-items:center; gap:8px; flex-wrap:nowrap"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]radio-group-item[/color] [color=#0550ae]id[/color]=[color=#0a3069]"rd2"[/color] [color=#0550ae]value[/color]=[color=#0a3069]"option2"[/color] [color=#0550ae]checked[/color]=[color=#0a3069]"true"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"rd2"[/color]>[/color]Option 2[color=#116329]</[color=#0550ae]label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]style[/color]=[color=#0a3069]"display:flex; flex-direction:row; align-items:center; gap:8px; flex-wrap:nowrap"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]radio-group-item[/color] [color=#0550ae]id[/color]=[color=#0a3069]"rd3"[/color] [color=#0550ae]value[/color]=[color=#0a3069]"option3"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"rd3"[/color]>[/color]Option 3[color=#116329]</[color=#0550ae]label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`    [color=#116329]</[color=#0550ae]radio-group[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/radio-group-disabled/4" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "184px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-line-row">
                <Text className="code-line-number">1</Text>
                <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">2</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">3</Text>
                <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]main[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`        lcui_init();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`        ui_load_xml_file([color=#0a3069]"ui.xml"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
