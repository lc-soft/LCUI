import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function DropdownMenuSubmenuDemo() {
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
        <Widget className="code-block-body active" data-language="c" data-file="main.c" data-source="examples/dropdown-menu-submenu/0">
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
                  <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_item[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *label, [color=#953800]const[/color] [color=#953800]char[/color] *shortcut)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *item = ui_create_dropdown_menu_item();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *hint;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{` `}</Text>
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
                  <Text className="code-line">{`        [color=#cf222e]if[/color] (shortcut) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`                hint = ui_create_dropdown_menu_shortcut();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`                ui_text_set_content(hint, shortcut);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`                ui_widget_append(item, hint);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`        }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        [color=#cf222e]return[/color] item;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_sub_trigger[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *label)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *trigger = ui_create_dropdown_menu_sub_trigger();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *chevron = ui_create_fluent_icon([color=#0a3069]"chevron-right"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`        ui_text_set_content(text, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`        ui_widget_add_class(chevron, [color=#0a3069]"dropdown-menu-chevron"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`        ui_widget_append(trigger, text);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`        ui_widget_append(trigger, chevron);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`        [color=#cf222e]return[/color] trigger;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]dropdown_menu_submenu_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *menu = ui_create_dropdown_menu();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *trigger = ui_create_dropdown_menu_trigger();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_dropdown_menu_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *group = ui_create_dropdown_menu_group();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *sub = ui_create_dropdown_menu_sub();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *sub_content = ui_create_dropdown_menu_sub_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *more = ui_create_dropdown_menu_sub();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *more_content = ui_create_dropdown_menu_sub_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`        ui_button_set_text(button, [color=#0a3069]"Open"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`        ui_widget_append(trigger, button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`        ui_widget_append(group, create_item([color=#0a3069]"Team"[/color], [color=#0550ae]NULL[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`        ui_widget_append(sub, create_sub_trigger([color=#0a3069]"Invite users"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`        ui_widget_append(sub_content, create_item([color=#0a3069]"Email"[/color], [color=#0550ae]NULL[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`        ui_widget_append(sub_content, create_item([color=#0a3069]"Message"[/color], [color=#0550ae]NULL[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`        ui_widget_append(more, create_sub_trigger([color=#0a3069]"More options"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`        ui_widget_append(more_content, create_item([color=#0a3069]"Calendly"[/color], [color=#0550ae]NULL[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`        ui_widget_append(more_content, create_item([color=#0a3069]"Slack"[/color], [color=#0550ae]NULL[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`        ui_widget_append(more_content, ui_create_dropdown_menu_separator());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`        ui_widget_append(more_content, create_item([color=#0a3069]"Webhook"[/color], [color=#0550ae]NULL[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`        ui_widget_append(more, more_content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`        ui_widget_append(sub_content, more);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`        ui_widget_append(sub_content, ui_create_dropdown_menu_separator());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`        ui_widget_append(sub_content, create_item([color=#0a3069]"Advanced..."[/color], [color=#0550ae]NULL[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`        ui_widget_append(sub, sub_content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`        ui_widget_append(group, sub);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`        ui_widget_append(group, create_item([color=#0a3069]"New Team"[/color], [color=#0a3069]"Ctrl+T"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`        ui_widget_append(content, group);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, trigger);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`        ui_widget_append(parent, menu);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
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
                  <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]ui_widget_t[/color] *[color=#d2a8ff]create_item[/color][color=#79c0ff]([color=#ffa657]const[/color] [color=#ffa657]char[/color] *label, [color=#ffa657]const[/color] [color=#ffa657]char[/color] *shortcut)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *item = ui_create_dropdown_menu_item();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *text = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *hint;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{` `}</Text>
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
                  <Text className="code-line">{`        [color=#ff7b72]if[/color] (shortcut) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`                hint = ui_create_dropdown_menu_shortcut();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`                ui_text_set_content(hint, shortcut);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`                ui_widget_append(item, hint);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`        }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        [color=#ff7b72]return[/color] item;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]ui_widget_t[/color] *[color=#d2a8ff]create_sub_trigger[/color][color=#79c0ff]([color=#ffa657]const[/color] [color=#ffa657]char[/color] *label)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *trigger = ui_create_dropdown_menu_sub_trigger();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *text = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *chevron = ui_create_fluent_icon([color=#a5d6ff]"chevron-right"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`        ui_text_set_content(text, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`        ui_widget_add_class(chevron, [color=#a5d6ff]"dropdown-menu-chevron"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`        ui_widget_append(trigger, text);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`        ui_widget_append(trigger, chevron);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`        [color=#ff7b72]return[/color] trigger;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]dropdown_menu_submenu_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *menu = ui_create_dropdown_menu();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *trigger = ui_create_dropdown_menu_trigger();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *content = ui_create_dropdown_menu_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *group = ui_create_dropdown_menu_group();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *sub = ui_create_dropdown_menu_sub();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *sub_content = ui_create_dropdown_menu_sub_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *more = ui_create_dropdown_menu_sub();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *more_content = ui_create_dropdown_menu_sub_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`        ui_button_set_text(button, [color=#a5d6ff]"Open"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`        ui_widget_append(trigger, button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`        ui_widget_append(group, create_item([color=#a5d6ff]"Team"[/color], [color=#79c0ff]NULL[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`        ui_widget_append(sub, create_sub_trigger([color=#a5d6ff]"Invite users"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`        ui_widget_append(sub_content, create_item([color=#a5d6ff]"Email"[/color], [color=#79c0ff]NULL[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`        ui_widget_append(sub_content, create_item([color=#a5d6ff]"Message"[/color], [color=#79c0ff]NULL[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`        ui_widget_append(more, create_sub_trigger([color=#a5d6ff]"More options"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`        ui_widget_append(more_content, create_item([color=#a5d6ff]"Calendly"[/color], [color=#79c0ff]NULL[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`        ui_widget_append(more_content, create_item([color=#a5d6ff]"Slack"[/color], [color=#79c0ff]NULL[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`        ui_widget_append(more_content, ui_create_dropdown_menu_separator());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`        ui_widget_append(more_content, create_item([color=#a5d6ff]"Webhook"[/color], [color=#79c0ff]NULL[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`        ui_widget_append(more, more_content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`        ui_widget_append(sub_content, more);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`        ui_widget_append(sub_content, ui_create_dropdown_menu_separator());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`        ui_widget_append(sub_content, create_item([color=#a5d6ff]"Advanced..."[/color], [color=#79c0ff]NULL[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`        ui_widget_append(sub, sub_content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`        ui_widget_append(group, sub);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`        ui_widget_append(group, create_item([color=#a5d6ff]"New Team"[/color], [color=#a5d6ff]"Ctrl+T"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`        ui_widget_append(content, group);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, trigger);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`        ui_widget_append(parent, menu);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/dropdown-menu-submenu/1" style={{ display: "none" }}>
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
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]dropdown-menu-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]dropdown-menu-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color][color=#116329]<[color=#0550ae]text[/color]>[/color]Team[color=#116329]</[color=#0550ae]text[/color]>[/color][color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dropdown-menu-sub-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]text[/color]>[/color]Invite users[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"dropdown-menu-chevron"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]dropdown-menu-sub-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dropdown-menu-sub-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color][color=#116329]<[color=#0550ae]text[/color]>[/color]Email[color=#116329]</[color=#0550ae]text[/color]>[/color][color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color][color=#116329]<[color=#0550ae]text[/color]>[/color]Message[color=#116329]</[color=#0550ae]text[/color]>[/color][color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dropdown-menu-sub-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color]>[/color]More options[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"dropdown-menu-chevron"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]dropdown-menu-sub-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dropdown-menu-sub-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color][color=#116329]<[color=#0550ae]text[/color]>[/color]Calendly[color=#116329]</[color=#0550ae]text[/color]>[/color][color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color][color=#116329]<[color=#0550ae]text[/color]>[/color]Slack[color=#116329]</[color=#0550ae]text[/color]>[/color][color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]dropdown-menu-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color][color=#116329]<[color=#0550ae]text[/color]>[/color]Webhook[color=#116329]</[color=#0550ae]text[/color]>[/color][color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]dropdown-menu-sub-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]dropdown-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color][color=#116329]<[color=#0550ae]text[/color]>[/color]Advanced...[color=#116329]</[color=#0550ae]text[/color]>[/color][color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]dropdown-menu-sub-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color]>[/color]New Team[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dropdown-menu-shortcut[/color]>[/color]Ctrl+T[color=#116329]</[color=#0550ae]dropdown-menu-shortcut[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]dropdown-menu-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]dropdown-menu-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]dropdown-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
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
                  <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]dropdown-menu-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]dropdown-menu-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color][color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Team[color=#7ee787]</[color=#79c0ff]text[/color]>[/color][color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]dropdown-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]dropdown-menu-sub-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Invite users[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-right"[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"dropdown-menu-chevron"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]dropdown-menu-sub-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]dropdown-menu-sub-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color][color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Email[color=#7ee787]</[color=#79c0ff]text[/color]>[/color][color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color][color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Message[color=#7ee787]</[color=#79c0ff]text[/color]>[/color][color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]dropdown-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]dropdown-menu-sub-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]More options[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-right"[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"dropdown-menu-chevron"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]dropdown-menu-sub-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]dropdown-menu-sub-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color][color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Calendly[color=#7ee787]</[color=#79c0ff]text[/color]>[/color][color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color][color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Slack[color=#7ee787]</[color=#79c0ff]text[/color]>[/color][color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]dropdown-menu-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color][color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Webhook[color=#7ee787]</[color=#79c0ff]text[/color]>[/color][color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]dropdown-menu-sub-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]dropdown-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]dropdown-menu-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color][color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Advanced...[color=#7ee787]</[color=#79c0ff]text[/color]>[/color][color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]dropdown-menu-sub-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]dropdown-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]New Team[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]dropdown-menu-shortcut[/color]>[/color]Ctrl+T[color=#7ee787]</[color=#79c0ff]dropdown-menu-shortcut[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]dropdown-menu-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`    [color=#7ee787]</[color=#79c0ff]dropdown-menu-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]dropdown-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`[color=#7ee787]</[color=#79c0ff]lcui-app[/color]>[/color]`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/dropdown-menu-submenu/2" style={{ display: "none" }}>
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
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]dropdown_menu_submenu_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
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
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]dropdown_menu_submenu_xml_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
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
        <Widget className="code-block-body" data-language="tsx" data-file="example.tsx" data-source="examples/dropdown-menu-submenu/3" style={{ display: "none" }}>
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
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuGroup[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuItem[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuSeparator[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuShortcut[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuSub[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuSubContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuSubTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuTrigger[/color],`}</Text>
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
                  <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Example[/color]([color=#0550ae][/color]) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]DropdownMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Button[/color]>[/color]Open[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]DropdownMenuContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]DropdownMenuGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Text[/color]>[/color]Team[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuSub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DropdownMenuSubTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Text[/color]>[/color]Invite users[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"dropdown-menu-chevron"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]DropdownMenuSubTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DropdownMenuSubContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Text[/color]>[/color]Email[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Text[/color]>[/color]Message[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuSub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]DropdownMenuSubTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Text[/color]>[/color]More options[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"dropdown-menu-chevron"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]DropdownMenuSubTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]DropdownMenuSubContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Text[/color]>[/color]Calendly[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Text[/color]>[/color]Slack[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuSeparator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Text[/color]>[/color]Webhook[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]DropdownMenuSubContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]DropdownMenuSub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuSeparator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Text[/color]>[/color]Advanced...[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]DropdownMenuSubContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuSub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Text[/color]>[/color]New Team[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DropdownMenuShortcut[/color]>[/color]Ctrl+T[color=#116329]</[color=#0550ae]DropdownMenuShortcut[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]DropdownMenuGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]DropdownMenuContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]DropdownMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`  );`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
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
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenuGroup[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenuItem[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenuSeparator[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenuShortcut[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenuSub[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenuSubContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenuSubTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenuTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]Icon[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]Text[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`} [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`[color=#ff7b72]export[/color] [color=#ff7b72]default[/color] [color=#ff7b72]function[/color] [color=#d2a8ff]Example[/color]([color=#79c0ff][/color]) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`  [color=#ff7b72]return[/color] (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]DropdownMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]Button[/color]>[/color]Open[color=#7ee787]</[color=#79c0ff]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]DropdownMenuContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]DropdownMenuGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Team[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]DropdownMenuSub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]DropdownMenuSubTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Invite users[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-right"[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"dropdown-menu-chevron"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]DropdownMenuSubTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]DropdownMenuSubContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Email[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Message[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]DropdownMenuSub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]DropdownMenuSubTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]More options[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-right"[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"dropdown-menu-chevron"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]DropdownMenuSubTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]DropdownMenuSubContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Calendly[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Slack[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuSeparator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Webhook[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]DropdownMenuSubContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]DropdownMenuSub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]DropdownMenuSeparator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Advanced...[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]DropdownMenuSubContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]DropdownMenuSub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]New Team[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]DropdownMenuShortcut[/color]>[/color]Ctrl+T[color=#7ee787]</[color=#79c0ff]DropdownMenuShortcut[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]DropdownMenuGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]DropdownMenuContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`    [color=#7ee787]</[color=#79c0ff]DropdownMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`  );`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/dropdown-menu-submenu/4" style={{ display: "none" }}>
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
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]dropdown_menu_submenu_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
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
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]dropdown_menu_submenu_tsx_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
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
