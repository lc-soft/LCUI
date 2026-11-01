import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function CollapsibleSettingsDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/collapsible-settings/0">
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-line-row">
                <Text className="code-line-number">1</Text>
                <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">2</Text>
                <Text className="code-line">{`  [color=#8250df]Button[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">3</Text>
                <Text className="code-line">{`  [color=#8250df]Card[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`  [color=#8250df]CardContent[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`  [color=#8250df]CardDescription[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`  [color=#8250df]CardHeader[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`  [color=#8250df]CardTitle[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{`  [color=#8250df]Collapsible[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`  [color=#8250df]CollapsibleContent[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`  [color=#8250df]CollapsibleTrigger[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`  [color=#8250df]Field[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`  [color=#8250df]FieldLabel[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`  [color=#8250df]Icon[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`  [color=#8250df]Text[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`  [color=#8250df]TextInput[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-\\[320px]"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Card[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-full"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]CardHeader[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]CardTitle[/color]>[/color]Radius[color=#116329]</[color=#0550ae]CardTitle[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]CardDescription[/color]>[/color]Set the corner radius of the element.[color=#116329]</[color=#0550ae]CardDescription[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]CardHeader[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]CardContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Collapsible[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]CollapsibleTrigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Button[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{`[color=#116329]                [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`[color=#116329]                [color=#0550ae]className[/color]=[color=#0a3069]"w-full justify-between border-0 bg-transparent"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`[color=#116329]              >[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Text[/color]>[/color]Advanced settings[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">35</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-down"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">36</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">37</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]CollapsibleTrigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">38</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]CollapsibleContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">39</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex gap-2 pt-2.5"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">40</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex w-1/2 flex-col gap-1"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">41</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"radius-x"[/color]>[/color]Radius X[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">42</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]TextInput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"radius-x"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"0"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">43</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">44</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex w-1/2 flex-col gap-1"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">45</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"radius-y"[/color]>[/color]Radius Y[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">46</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]TextInput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"radius-y"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"0"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">47</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">48</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">49</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]CollapsibleContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">50</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Collapsible[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">51</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]CardContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">52</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Card[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">53</Text>
                <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">54</Text>
                <Text className="code-line">{`  );`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">55</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/collapsible-settings/1" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "164px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-line-row">
                <Text className="code-line-number">1</Text>
                <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">2</Text>
                <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">3</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]collapsible_settings_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`        ui_widget_append(parent, example_load());`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/collapsible-settings/2" style={{ display: "none" }}>
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
                <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]const[/color] [color=#953800]char[/color] *collapsible_settings_css = [color=#0a3069]"\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`[color=#0a3069].collapsible-settings-demo {\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`[color=#0a3069]  width: 320px;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`[color=#0a3069].collapsible-settings-demo card {\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{`[color=#0a3069]  width: 100%;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`[color=#0a3069].collapsible-settings-demo collapsible-trigger button {\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`[color=#0a3069]  width: 100%;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`[color=#0a3069]  border: 0;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`[color=#0a3069]  background: transparent;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`[color=#0a3069]  justify-content: space-between;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`[color=#0a3069].collapsible-settings-demo .settings-fields {\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`[color=#0a3069]  display: flex;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`[color=#0a3069]  gap: 8px;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`[color=#0a3069]  padding-top: 10px;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`[color=#0a3069].collapsible-settings-demo .settings-field {\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`[color=#0a3069]  display: flex;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`[color=#0a3069]  flex-direction: column;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`[color=#0a3069]  gap: 4px;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{`[color=#0a3069]  width: 50%;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`[color=#0a3069].collapsible-settings-demo .settings-field textinput {\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{`[color=#0a3069]  width: 100%;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{`[color=#0a3069]"[/color];`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_radius_field[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *id, [color=#953800]const[/color] [color=#953800]char[/color] *text)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *field = ui_create_field();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">35</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *label = ui_create_field_label();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">36</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *input = ui_create_widget([color=#0a3069]"textinput"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">37</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">38</Text>
                <Text className="code-line">{`        ui_widget_add_class(field, [color=#0a3069]"settings-field"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">39</Text>
                <Text className="code-line">{`        ui_field_set_orientation(field, [color=#0a3069]"vertical"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">40</Text>
                <Text className="code-line">{`        ui_text_set_content(label, text);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">41</Text>
                <Text className="code-line">{`        ui_widget_set_id(input, id);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">42</Text>
                <Text className="code-line">{`        ui_textinput_set_placeholder(input, [color=#0a3069]"0"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">43</Text>
                <Text className="code-line">{`        ui_widget_append(field, label);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">44</Text>
                <Text className="code-line">{`        ui_widget_append(field, input);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">45</Text>
                <Text className="code-line">{`        [color=#cf222e]return[/color] field;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">46</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">47</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">48</Text>
                <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]collapsible_settings_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">49</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">50</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *box = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">51</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *card = ui_create_card();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">52</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *header = ui_create_card_header();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">53</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *title = ui_create_card_title();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">54</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *description = ui_create_card_description();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">55</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_card_content();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">56</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *collapsible = ui_create_collapsible();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">57</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *trigger = ui_create_collapsible_trigger();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">58</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button = ui_create_button();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">59</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *label = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">60</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *chevron = ui_create_fluent_icon([color=#0a3069]"chevron-down"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">61</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *details = ui_create_collapsible_content();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">62</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *fields = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">63</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">64</Text>
                <Text className="code-line">{`        ui_load_css_string(collapsible_settings_css,`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">65</Text>
                <Text className="code-line">{`                           [color=#0a3069]"collapsible-settings-demo"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">66</Text>
                <Text className="code-line">{`        ui_widget_add_class(box, [color=#0a3069]"collapsible-settings-demo"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">67</Text>
                <Text className="code-line">{`        ui_card_set_size(card, UI_CARD_SIZE_SM);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">68</Text>
                <Text className="code-line">{`        ui_text_set_content(title, [color=#0a3069]"Radius"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">69</Text>
                <Text className="code-line">{`        ui_text_set_content(description,`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">70</Text>
                <Text className="code-line">{`                            [color=#0a3069]"Set the corner radius of the element."[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">71</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">72</Text>
                <Text className="code-line">{`        ui_button_set_variant(button, UI_BUTTON_VARIANT_GHOST);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">73</Text>
                <Text className="code-line">{`        ui_text_set_content(label, [color=#0a3069]"Advanced settings"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">74</Text>
                <Text className="code-line">{`        ui_widget_append(button, label);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">75</Text>
                <Text className="code-line">{`        ui_widget_append(button, chevron);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">76</Text>
                <Text className="code-line">{`        ui_widget_append(trigger, button);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">77</Text>
                <Text className="code-line">{`        ui_widget_append(collapsible, trigger);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">78</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">79</Text>
                <Text className="code-line">{`        ui_widget_add_class(fields, [color=#0a3069]"settings-fields"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">80</Text>
                <Text className="code-line">{`        ui_widget_append(fields, create_radius_field([color=#0a3069]"radius-x"[/color], [color=#0a3069]"Radius X"[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">81</Text>
                <Text className="code-line">{`        ui_widget_append(fields, create_radius_field([color=#0a3069]"radius-y"[/color], [color=#0a3069]"Radius Y"[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">82</Text>
                <Text className="code-line">{`        ui_widget_append(details, fields);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">83</Text>
                <Text className="code-line">{`        ui_widget_append(collapsible, details);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">84</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">85</Text>
                <Text className="code-line">{`        ui_widget_append(header, title);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">86</Text>
                <Text className="code-line">{`        ui_widget_append(header, description);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">87</Text>
                <Text className="code-line">{`        ui_widget_append(content, collapsible);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">88</Text>
                <Text className="code-line">{`        ui_widget_append(card, header);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">89</Text>
                <Text className="code-line">{`        ui_widget_append(card, content);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">90</Text>
                <Text className="code-line">{`        ui_widget_append(box, card);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">91</Text>
                <Text className="code-line">{`        ui_widget_append(parent, box);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">92</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/collapsible-settings/3" style={{ display: "none" }}>
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
                <Text className="code-line">{`  [color=#116329]<[color=#0550ae]resource[/color] [color=#0550ae]type[/color]=[color=#0a3069]"text/css"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`    .collapsible-settings-demo {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`      width: 320px;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`    }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`    .collapsible-settings-demo card {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{`      width: 100%;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`    }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`    .collapsible-settings-demo collapsible-trigger button {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`      width: 100%;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`      border: 0;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`      background: transparent;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`      justify-content: space-between;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`    }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`    .collapsible-settings-demo .settings-fields {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`      display: flex;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`      gap: 8px;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`      padding-top: 10px;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{`    }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`    .collapsible-settings-demo .settings-field {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`      display: flex;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`      flex-direction: column;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`      gap: 4px;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{`      width: 50%;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`    }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`    .collapsible-settings-demo .settings-field textinput {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{`      width: 100%;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`    }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{`  [color=#116329]</[color=#0550ae]resource[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`    [color=#116329]<[color=#0550ae]widget[/color] [color=#0550ae]class[/color]=[color=#0a3069]"collapsible-settings-demo"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]card[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]card-header[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">35</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]card-title[/color]>[/color]Radius[color=#116329]</[color=#0550ae]card-title[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">36</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]card-description[/color]>[/color]Set the corner radius of the element.[color=#116329]</[color=#0550ae]card-description[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">37</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]card-header[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">38</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]card-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">39</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]collapsible[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">40</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">41</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">42</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color]>[/color]Advanced settings[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">43</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-down"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">44</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">45</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">46</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">47</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"settings-fields"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">48</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]class[/color]=[color=#0a3069]"settings-field"[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"vertical"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">49</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]field-label[/color]>[/color]Radius X[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">50</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"radius-x"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"0"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">51</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">52</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]class[/color]=[color=#0a3069]"settings-field"[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"vertical"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">53</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]field-label[/color]>[/color]Radius Y[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">54</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"radius-y"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"0"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">55</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">56</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">57</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">58</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]collapsible[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">59</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]card-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">60</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]card[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">61</Text>
                <Text className="code-line">{`    [color=#116329]</[color=#0550ae]widget[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">62</Text>
                <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">63</Text>
                <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/collapsible-settings/4" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "144px" }}>
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
                <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]collapsible_settings_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`        ui_widget_append(parent, ui_create_widget([color=#0550ae]NULL[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
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
