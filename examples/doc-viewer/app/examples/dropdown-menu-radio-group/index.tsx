import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function DropdownMenuRadioGroupDemo() {
  return (
    <DemoProvider>
      <Widget $ref="preview" className="demo-preview" />
      <Widget className="code-block">
        <Widget className="code-block-header">
          <Widget className="demo-files">
            <Text className="demo-tab active" data-language="c" data-value="main.c">main.c</Text>
            <Text className="demo-tab" data-language="xml" data-value="ui.xml" style={{ display: "none" }}>ui.xml</Text>
            <Text className="demo-tab" data-language="xml" data-value="main.c" style={{ display: "none" }}>main.c</Text>
            <Text className="demo-tab" data-language="tsx" data-value="example.tsx" style={{ display: "none" }}>example.tsx</Text>
            <Text className="demo-tab" data-language="tsx" data-value="main.c" style={{ display: "none" }}>main.c</Text>
          </Widget>
          <Widget className="demo-languages">
            <Text className="demo-tab active" data-value="c">C</Text>
            <Text className="demo-tab" data-value="xml">XML</Text>
            <Text className="demo-tab" data-value="tsx">TSX</Text>
          </Widget>
        </Widget>
        <Widget className="code-block-body active" data-language="c" data-file="main.c" data-source="examples/dropdown-menu-radio-group/0">
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-block-lines code-block-lines-light">
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
                  <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_radio[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *label, [color=#953800]const[/color] [color=#953800]char[/color] *value)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *item = ui_create_dropdown_menu_radio_item();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(item, [color=#0a3069]"value"[/color], value);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`        ui_text_set_content(text, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`        ui_widget_append(item, text);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`        [color=#cf222e]return[/color] item;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]dropdown_menu_radio_group_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *menu = ui_create_dropdown_menu();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *trigger = ui_create_dropdown_menu_trigger();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_dropdown_menu_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *label = ui_create_dropdown_menu_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *group = ui_create_dropdown_menu_radio_group();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`        ui_widget_add_class(content, [color=#0a3069]"dropdown-menu-radio-group-demo-content"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`        ui_button_set_text(button, [color=#0a3069]"Open"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`        ui_widget_append(trigger, button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`        ui_text_set_content(label, [color=#0a3069]"Panel position"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`        ui_widget_append(content, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(group, [color=#0a3069]"value"[/color], [color=#0a3069]"bottom"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`        ui_widget_append(group, create_radio([color=#0a3069]"Top"[/color], [color=#0a3069]"top"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`        ui_widget_append(group, create_radio([color=#0a3069]"Bottom"[/color], [color=#0a3069]"bottom"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`        ui_widget_append(group, create_radio([color=#0a3069]"Right"[/color], [color=#0a3069]"right"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`        ui_widget_append(content, group);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, trigger);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`        ui_widget_append(parent, menu);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </Widget>
              <Widget className="code-block-lines code-block-lines-dark">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#8b949e]#[color=#ff7b72]include[/color] [color=#a5d6ff]<LCUI.h>[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]ui_widget_t[/color] *[color=#d2a8ff]create_radio[/color][color=#79c0ff]([color=#ffa657]const[/color] [color=#ffa657]char[/color] *label, [color=#ffa657]const[/color] [color=#ffa657]char[/color] *value)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *item = ui_create_dropdown_menu_radio_item();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *text = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(item, [color=#a5d6ff]"value"[/color], value);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`        ui_text_set_content(text, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`        ui_widget_append(item, text);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`        [color=#ff7b72]return[/color] item;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]dropdown_menu_radio_group_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *menu = ui_create_dropdown_menu();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *trigger = ui_create_dropdown_menu_trigger();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *content = ui_create_dropdown_menu_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *label = ui_create_dropdown_menu_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *group = ui_create_dropdown_menu_radio_group();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`        ui_widget_add_class(content, [color=#a5d6ff]"dropdown-menu-radio-group-demo-content"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`        ui_button_set_text(button, [color=#a5d6ff]"Open"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`        ui_widget_append(trigger, button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`        ui_text_set_content(label, [color=#a5d6ff]"Panel position"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`        ui_widget_append(content, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(group, [color=#a5d6ff]"value"[/color], [color=#a5d6ff]"bottom"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`        ui_widget_append(group, create_radio([color=#a5d6ff]"Top"[/color], [color=#a5d6ff]"top"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`        ui_widget_append(group, create_radio([color=#a5d6ff]"Bottom"[/color], [color=#a5d6ff]"bottom"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`        ui_widget_append(group, create_radio([color=#a5d6ff]"Right"[/color], [color=#a5d6ff]"right"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`        ui_widget_append(content, group);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, trigger);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`        ui_widget_append(parent, menu);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/dropdown-menu-radio-group/1" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-block-lines code-block-lines-light">
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
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]dropdown-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]dropdown-menu-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]button[/color]>[/color]Open[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]dropdown-menu-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]dropdown-menu-content[/color] [color=#0550ae]class[/color]=[color=#0a3069]"dropdown-menu-radio-group-demo-content"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]dropdown-menu-label[/color]>[/color]Panel position[color=#116329]</[color=#0550ae]dropdown-menu-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]dropdown-menu-radio-group[/color] [color=#0550ae]value[/color]=[color=#0a3069]"bottom"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-radio-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"top"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color]>[/color]Top[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-radio-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-radio-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"bottom"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color]>[/color]Bottom[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-radio-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-radio-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"right"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color]>[/color]Right[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-radio-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]dropdown-menu-radio-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]dropdown-menu-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]dropdown-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
                </Widget>
              </Widget>
              <Widget className="code-block-lines code-block-lines-dark">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#8b949e]<?xml version=[color=#a5d6ff]"1.0"[/color] encoding=[color=#a5d6ff]"UTF-8"[/color] ?>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`[color=#7ee787]<[color=#79c0ff]lcui-app[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]dropdown-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]dropdown-menu-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]button[/color]>[/color]Open[color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`    [color=#7ee787]</[color=#79c0ff]dropdown-menu-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]dropdown-menu-content[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"dropdown-menu-radio-group-demo-content"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]dropdown-menu-label[/color]>[/color]Panel position[color=#7ee787]</[color=#79c0ff]dropdown-menu-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]dropdown-menu-radio-group[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"bottom"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]dropdown-menu-radio-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"top"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Top[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]dropdown-menu-radio-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]dropdown-menu-radio-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"bottom"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Bottom[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]dropdown-menu-radio-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]dropdown-menu-radio-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"right"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Right[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]dropdown-menu-radio-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]dropdown-menu-radio-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`    [color=#7ee787]</[color=#79c0ff]dropdown-menu-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]dropdown-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`[color=#7ee787]</[color=#79c0ff]lcui-app[/color]>[/color]`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/dropdown-menu-radio-group/2" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "144px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-block-lines code-block-lines-light">
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
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]dropdown_menu_radio_group_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
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
              </Widget>
              <Widget className="code-block-lines code-block-lines-dark">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#8b949e]#[color=#ff7b72]include[/color] [color=#a5d6ff]<LCUI.h>[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]dropdown_menu_radio_group_xml_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`        ui_widget_append(parent, ui_create_widget([color=#79c0ff]NULL[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="example.tsx" data-source="examples/dropdown-menu-radio-group/3" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-block-lines code-block-lines-light">
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
                  <Text className="code-line">{`  [color=#8250df]DropdownMenu[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuLabel[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuRadioGroup[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuRadioItem[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`  [color=#8250df]Text[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Example[/color]([color=#0550ae][/color]) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]DropdownMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Button[/color]>[/color]Open[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]DropdownMenuContent[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-32"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]DropdownMenuLabel[/color]>[/color]Panel position[color=#116329]</[color=#0550ae]DropdownMenuLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]DropdownMenuRadioGroup[/color] [color=#0550ae]value[/color]=[color=#0a3069]"bottom"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuRadioItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"top"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Text[/color]>[/color]Top[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuRadioItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuRadioItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"bottom"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Text[/color]>[/color]Bottom[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuRadioItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuRadioItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"right"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Text[/color]>[/color]Right[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuRadioItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]DropdownMenuRadioGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]DropdownMenuContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]DropdownMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`  );`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </Widget>
              <Widget className="code-block-lines code-block-lines-dark">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#ff7b72]import[/color] {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]Button[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenu[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenuContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenuLabel[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenuRadioGroup[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenuRadioItem[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenuTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]Text[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`} [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`[color=#ff7b72]export[/color] [color=#ff7b72]default[/color] [color=#ff7b72]function[/color] [color=#d2a8ff]Example[/color]([color=#79c0ff][/color]) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`  [color=#ff7b72]return[/color] (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]DropdownMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]Button[/color]>[/color]Open[color=#7ee787]</[color=#79c0ff]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]DropdownMenuContent[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"w-32"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]DropdownMenuLabel[/color]>[/color]Panel position[color=#7ee787]</[color=#79c0ff]DropdownMenuLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]DropdownMenuRadioGroup[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"bottom"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]DropdownMenuRadioItem[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"top"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Top[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]DropdownMenuRadioItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]DropdownMenuRadioItem[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"bottom"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Bottom[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]DropdownMenuRadioItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]DropdownMenuRadioItem[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"right"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Right[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]DropdownMenuRadioItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]DropdownMenuRadioGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]DropdownMenuContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`    [color=#7ee787]</[color=#79c0ff]DropdownMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`  );`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/dropdown-menu-radio-group/4" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "164px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-block-lines code-block-lines-light">
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
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]dropdown_menu_radio_group_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
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
              </Widget>
              <Widget className="code-block-lines code-block-lines-dark">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#8b949e]#[color=#ff7b72]include[/color] [color=#a5d6ff]<LCUI.h>[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`[color=#8b949e]#[color=#ff7b72]include[/color] [color=#a5d6ff]"example.h"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]dropdown_menu_radio_group_tsx_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
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
