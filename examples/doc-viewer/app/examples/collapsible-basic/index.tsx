import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function CollapsibleBasicDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/collapsible-basic/0">
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
                <Text className="code-line">{`  [color=#8250df]Collapsible[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`  [color=#8250df]CollapsibleContent[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`  [color=#8250df]CollapsibleTrigger[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{`  [color=#8250df]Icon[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`  [color=#8250df]Text[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-\\[384px]"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Card[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-full"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]CardContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Collapsible[/color] [color=#0550ae]className[/color]=[color=#0a3069]"rounded-md bg-muted"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]CollapsibleTrigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Button[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`[color=#116329]                [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`[color=#116329]                [color=#0550ae]className[/color]=[color=#0a3069]"w-full justify-between border-0 bg-transparent"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`[color=#116329]              >[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Text[/color]>[/color]Product details[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-down"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]CollapsibleTrigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]CollapsibleContent[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-col gap-2 px-2.5 pb-2.5 text-sm"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"text-muted-foreground"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{`                This panel can be expanded or collapsed to reveal additional`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{`                content.`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"primary"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"xs"[/color]>[/color]Learn More[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]CollapsibleContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">35</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Collapsible[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">36</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]CardContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">37</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Card[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">38</Text>
                <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">39</Text>
                <Text className="code-line">{`  );`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">40</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/collapsible-basic/1" style={{ display: "none" }}>
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
                <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]collapsible_basic_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
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
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/collapsible-basic/2" style={{ display: "none" }}>
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
                <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]const[/color] [color=#953800]char[/color] *collapsible_basic_css = [color=#0a3069]"\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`[color=#0a3069].collapsible-basic-demo {\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`[color=#0a3069]  width: 384px;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`[color=#0a3069].collapsible-basic-demo card {\\[/color]`}</Text>
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
                <Text className="code-line">{`[color=#0a3069].collapsible-basic-demo collapsible:open {\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`[color=#0a3069]  background: #f5f5f5;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`[color=#0a3069]  border-radius: 8px;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`[color=#0a3069].collapsible-basic-demo collapsible-trigger button {\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`[color=#0a3069]  width: 100%;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`[color=#0a3069]  border: 0;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`[color=#0a3069]  background: transparent;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`[color=#0a3069]  justify-content: space-between;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{`[color=#0a3069].collapsible-basic-demo collapsible-content {\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`[color=#0a3069]  padding: 0 10px 10px 10px;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`[color=#0a3069].collapsible-basic-demo .details-copy {\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`[color=#0a3069]  display: block;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{`[color=#0a3069]  margin-bottom: 10px;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`[color=#0a3069]  font-size: 14px;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`[color=#0a3069]  color: #666666;\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`[color=#0a3069]"[/color];`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]collapsible_basic_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *box = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *card = ui_create_card();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">35</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_card_content();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">36</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *collapsible = ui_create_collapsible();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">37</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *trigger = ui_create_collapsible_trigger();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">38</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button = ui_create_button();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">39</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *label = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">40</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *chevron = ui_create_fluent_icon([color=#0a3069]"chevron-down"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">41</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *details = ui_create_collapsible_content();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">42</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *copy = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">43</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *learn_more = ui_create_button();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">44</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">45</Text>
                <Text className="code-line">{`        ui_load_css_string(collapsible_basic_css, [color=#0a3069]"collapsible-basic-demo"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">46</Text>
                <Text className="code-line">{`        ui_widget_add_class(box, [color=#0a3069]"collapsible-basic-demo"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">47</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">48</Text>
                <Text className="code-line">{`        ui_button_set_variant(button, UI_BUTTON_VARIANT_GHOST);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">49</Text>
                <Text className="code-line">{`        ui_text_set_content(label, [color=#0a3069]"Product details"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">50</Text>
                <Text className="code-line">{`        ui_widget_append(button, label);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">51</Text>
                <Text className="code-line">{`        ui_widget_append(button, chevron);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">52</Text>
                <Text className="code-line">{`        ui_widget_append(trigger, button);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">53</Text>
                <Text className="code-line">{`        ui_widget_append(collapsible, trigger);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">54</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">55</Text>
                <Text className="code-line">{`        ui_widget_add_class(copy, [color=#0a3069]"details-copy"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">56</Text>
                <Text className="code-line">{`        ui_text_set_content(`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">57</Text>
                <Text className="code-line">{`            copy,`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">58</Text>
                <Text className="code-line">{`            [color=#0a3069]"This panel can be expanded or collapsed to reveal additional "[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">59</Text>
                <Text className="code-line">{`            [color=#0a3069]"content."[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">60</Text>
                <Text className="code-line">{`        ui_button_set_text(learn_more, [color=#0a3069]"Learn More"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">61</Text>
                <Text className="code-line">{`        ui_button_set_variant(learn_more, UI_BUTTON_VARIANT_PRIMARY);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">62</Text>
                <Text className="code-line">{`        ui_button_set_size(learn_more, UI_BUTTON_SIZE_XS);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">63</Text>
                <Text className="code-line">{`        ui_widget_append(details, copy);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">64</Text>
                <Text className="code-line">{`        ui_widget_append(details, learn_more);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">65</Text>
                <Text className="code-line">{`        ui_widget_append(collapsible, details);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">66</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">67</Text>
                <Text className="code-line">{`        ui_widget_append(content, collapsible);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">68</Text>
                <Text className="code-line">{`        ui_widget_append(card, content);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">69</Text>
                <Text className="code-line">{`        ui_widget_append(box, card);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">70</Text>
                <Text className="code-line">{`        ui_widget_append(parent, box);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">71</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/collapsible-basic/3" style={{ display: "none" }}>
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
                <Text className="code-line">{`    .collapsible-basic-demo {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`      width: 384px;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`    }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`    .collapsible-basic-demo card {`}</Text>
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
                <Text className="code-line">{`    .collapsible-basic-demo collapsible:open {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`      background: #f5f5f5;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`      border-radius: 8px;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`    }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`    .collapsible-basic-demo collapsible-trigger button {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`      width: 100%;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`      border: 0;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`      background: transparent;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`      justify-content: space-between;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`    }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{`    .collapsible-basic-demo collapsible-content {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`      padding: 0 10px 10px 10px;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`    }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`    .collapsible-basic-demo .details-copy {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`      display: block;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{`      margin-bottom: 10px;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`      font-size: 14px;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`      color: #666666;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{`    }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`  [color=#116329]</[color=#0550ae]resource[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{`    [color=#116329]<[color=#0550ae]widget[/color] [color=#0550ae]class[/color]=[color=#0a3069]"collapsible-basic-demo"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]card[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]card-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]collapsible[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">35</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">36</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">37</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color]>[/color]Product details[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">38</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-down"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">39</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">40</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">41</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">42</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"details-copy"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">43</Text>
                <Text className="code-line">{`                This panel can be expanded or collapsed to reveal additional`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">44</Text>
                <Text className="code-line">{`                content.`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">45</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">46</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"primary"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"xs"[/color]>[/color]Learn More[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">47</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">48</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]collapsible[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">49</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]card-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">50</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]card[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">51</Text>
                <Text className="code-line">{`    [color=#116329]</[color=#0550ae]widget[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">52</Text>
                <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">53</Text>
                <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/collapsible-basic/4" style={{ display: "none" }}>
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
                <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]collapsible_basic_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
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
