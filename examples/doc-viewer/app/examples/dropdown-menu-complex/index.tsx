import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function DropdownMenuComplexDemo() {
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
        <Widget className="code-block-body active" data-language="c" data-file="main.c" data-source="examples/dropdown-menu-complex/0">
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
                <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_item[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *icon_name, [color=#953800]const[/color] [color=#953800]char[/color] *label,[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`[color=#0550ae]                                [color=#953800]const[/color] [color=#953800]char[/color] *shortcut, [color=#953800]bool[/color] disabled,[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`[color=#0550ae]                                [color=#953800]bool[/color] destructive)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *item = ui_create_dropdown_menu_item();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *hint;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`        [color=#cf222e]if[/color] (icon_name) {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`                ui_widget_append(item, ui_create_fluent_icon(icon_name));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`        }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`        ui_text_set_content(text, label);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`        ui_widget_append(item, text);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`        [color=#cf222e]if[/color] (shortcut) {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`                hint = ui_create_dropdown_menu_shortcut();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`                ui_text_set_content(hint, shortcut);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`                ui_widget_append(item, hint);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{`        }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`        [color=#cf222e]if[/color] (disabled) {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`                ui_widget_set_disabled(item, [color=#0550ae]true[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`        }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`        [color=#cf222e]if[/color] (destructive) {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{`                ui_dropdown_menu_item_set_variant(`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`                    item, UI_DROPDOWN_MENU_ITEM_DESTRUCTIVE);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`        }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{`        [color=#cf222e]return[/color] item;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_checkbox[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *icon_name, [color=#953800]const[/color] [color=#953800]char[/color] *label,[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`[color=#0550ae]                                    [color=#953800]bool[/color] checked)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *item = ui_create_dropdown_menu_checkbox_item();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">35</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">36</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">37</Text>
                <Text className="code-line">{`        ui_widget_append(item, ui_create_fluent_icon(icon_name));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">38</Text>
                <Text className="code-line">{`        ui_text_set_content(text, label);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">39</Text>
                <Text className="code-line">{`        ui_widget_append(item, text);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">40</Text>
                <Text className="code-line">{`        [color=#cf222e]if[/color] (checked) {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">41</Text>
                <Text className="code-line">{`                ui_widget_set_attr(item, [color=#0a3069]"checked"[/color], [color=#0a3069]"true"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">42</Text>
                <Text className="code-line">{`        }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">43</Text>
                <Text className="code-line">{`        [color=#cf222e]return[/color] item;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">44</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">45</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">46</Text>
                <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_radio[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *icon_name, [color=#953800]const[/color] [color=#953800]char[/color] *label,[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">47</Text>
                <Text className="code-line">{`[color=#0550ae]                                 [color=#953800]const[/color] [color=#953800]char[/color] *value)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">48</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">49</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *item = ui_create_dropdown_menu_radio_item();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">50</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">51</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">52</Text>
                <Text className="code-line">{`        ui_widget_set_attr(item, [color=#0a3069]"value"[/color], value);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">53</Text>
                <Text className="code-line">{`        ui_widget_append(item, ui_create_fluent_icon(icon_name));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">54</Text>
                <Text className="code-line">{`        ui_text_set_content(text, label);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">55</Text>
                <Text className="code-line">{`        ui_widget_append(item, text);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">56</Text>
                <Text className="code-line">{`        [color=#cf222e]return[/color] item;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">57</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">58</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">59</Text>
                <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_sub_trigger[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *icon_name, [color=#953800]const[/color] [color=#953800]char[/color] *label)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">60</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">61</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *trigger = ui_create_dropdown_menu_sub_trigger();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">62</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">63</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *chevron = ui_create_fluent_icon([color=#0a3069]"chevron-right"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">64</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">65</Text>
                <Text className="code-line">{`        ui_widget_append(trigger, ui_create_fluent_icon(icon_name));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">66</Text>
                <Text className="code-line">{`        ui_text_set_content(text, label);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">67</Text>
                <Text className="code-line">{`        ui_widget_add_class(chevron, [color=#0a3069]"dropdown-menu-chevron"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">68</Text>
                <Text className="code-line">{`        ui_widget_append(trigger, text);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">69</Text>
                <Text className="code-line">{`        ui_widget_append(trigger, chevron);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">70</Text>
                <Text className="code-line">{`        [color=#cf222e]return[/color] trigger;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">71</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">72</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">73</Text>
                <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_sub[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *trigger, [color=#953800]ui_widget_t[/color] *content)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">74</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">75</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *sub = ui_create_dropdown_menu_sub();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">76</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">77</Text>
                <Text className="code-line">{`        ui_widget_append(sub, trigger);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">78</Text>
                <Text className="code-line">{`        ui_widget_append(sub, content);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">79</Text>
                <Text className="code-line">{`        [color=#cf222e]return[/color] sub;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">80</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">81</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">82</Text>
                <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_label[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *text)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">83</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">84</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *label = ui_create_dropdown_menu_label();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">85</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">86</Text>
                <Text className="code-line">{`        ui_text_set_content(label, text);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">87</Text>
                <Text className="code-line">{`        [color=#cf222e]return[/color] label;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">88</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">89</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">90</Text>
                <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]dropdown_menu_complex_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">91</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">92</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *menu = ui_create_dropdown_menu();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">93</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *trigger = ui_create_dropdown_menu_trigger();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">94</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button = ui_create_button();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">95</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_dropdown_menu_content();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">96</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *group;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">97</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *sub_content;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">98</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *sub_sub_content;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">99</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *radios;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">100</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">101</Text>
                <Text className="code-line">{`        ui_widget_add_class(content, [color=#0a3069]"dropdown-menu-complex-demo-content"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">102</Text>
                <Text className="code-line">{`        ui_button_set_text(button, [color=#0a3069]"Complex Menu"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">103</Text>
                <Text className="code-line">{`        ui_widget_append(trigger, button);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">104</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">105</Text>
                <Text className="code-line">{`        group = ui_create_dropdown_menu_group();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">106</Text>
                <Text className="code-line">{`        ui_widget_append(group, create_label([color=#0a3069]"File"[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">107</Text>
                <Text className="code-line">{`        ui_widget_append(`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">108</Text>
                <Text className="code-line">{`            group, create_item([color=#0a3069]"document"[/color], [color=#0a3069]"New File"[/color], [color=#0a3069]"Ctrl+N"[/color], [color=#0550ae]false[/color], [color=#0550ae]false[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">109</Text>
                <Text className="code-line">{`        ui_widget_append(group, create_item([color=#0a3069]"folder"[/color], [color=#0a3069]"New Folder"[/color], [color=#0a3069]"Shift+N"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">110</Text>
                <Text className="code-line">{`                                            [color=#0550ae]false[/color], [color=#0550ae]false[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">111</Text>
                <Text className="code-line">{`        sub_content = ui_create_dropdown_menu_sub_content();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">112</Text>
                <Text className="code-line">{`        ui_widget_append(sub_content, create_label([color=#0a3069]"Recent Projects"[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">113</Text>
                <Text className="code-line">{`        ui_widget_append(sub_content, create_item([color=#0a3069]"code"[/color], [color=#0a3069]"Project Alpha"[/color], [color=#0550ae]NULL[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">114</Text>
                <Text className="code-line">{`                                                  [color=#0550ae]false[/color], [color=#0550ae]false[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">115</Text>
                <Text className="code-line">{`        ui_widget_append(sub_content, create_item([color=#0a3069]"code"[/color], [color=#0a3069]"Project Beta"[/color], [color=#0550ae]NULL[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">116</Text>
                <Text className="code-line">{`                                                  [color=#0550ae]false[/color], [color=#0550ae]false[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">117</Text>
                <Text className="code-line">{`        sub_sub_content = ui_create_dropdown_menu_sub_content();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">118</Text>
                <Text className="code-line">{`        ui_widget_append(sub_sub_content, create_item([color=#0a3069]"code"[/color], [color=#0a3069]"Project Gamma"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">119</Text>
                <Text className="code-line">{`                                                      [color=#0550ae]NULL[/color], [color=#0550ae]false[/color], [color=#0550ae]false[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">120</Text>
                <Text className="code-line">{`        ui_widget_append(sub_sub_content, create_item([color=#0a3069]"code"[/color], [color=#0a3069]"Project Delta"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">121</Text>
                <Text className="code-line">{`                                                      [color=#0550ae]NULL[/color], [color=#0550ae]false[/color], [color=#0550ae]false[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">122</Text>
                <Text className="code-line">{`        ui_widget_append(`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">123</Text>
                <Text className="code-line">{`            sub_content,`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">124</Text>
                <Text className="code-line">{`            create_sub(create_sub_trigger([color=#0a3069]"more-horizontal"[/color], [color=#0a3069]"More Projects"[/color]),`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">125</Text>
                <Text className="code-line">{`                       sub_sub_content));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">126</Text>
                <Text className="code-line">{`        ui_widget_append(sub_content, ui_create_dropdown_menu_separator());`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">127</Text>
                <Text className="code-line">{`        ui_widget_append(sub_content, create_item([color=#0a3069]"search"[/color], [color=#0a3069]"Browse..."[/color], [color=#0550ae]NULL[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">128</Text>
                <Text className="code-line">{`                                                  [color=#0550ae]false[/color], [color=#0550ae]false[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">129</Text>
                <Text className="code-line">{`        ui_widget_append(`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">130</Text>
                <Text className="code-line">{`            group, create_sub(create_sub_trigger([color=#0a3069]"folder-open"[/color], [color=#0a3069]"Open Recent"[/color]),`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">131</Text>
                <Text className="code-line">{`                              sub_content));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">132</Text>
                <Text className="code-line">{`        ui_widget_append(group, ui_create_dropdown_menu_separator());`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">133</Text>
                <Text className="code-line">{`        ui_widget_append(group,`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">134</Text>
                <Text className="code-line">{`                         create_item([color=#0a3069]"save"[/color], [color=#0a3069]"Save"[/color], [color=#0a3069]"Ctrl+S"[/color], [color=#0550ae]false[/color], [color=#0550ae]false[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">135</Text>
                <Text className="code-line">{`        ui_widget_append(group, create_item([color=#0a3069]"arrow-download"[/color], [color=#0a3069]"Export"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">136</Text>
                <Text className="code-line">{`                                            [color=#0a3069]"Shift+E"[/color], [color=#0550ae]false[/color], [color=#0550ae]false[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">137</Text>
                <Text className="code-line">{`        ui_widget_append(content, group);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">138</Text>
                <Text className="code-line">{`        ui_widget_append(content, ui_create_dropdown_menu_separator());`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">139</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">140</Text>
                <Text className="code-line">{`        group = ui_create_dropdown_menu_group();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">141</Text>
                <Text className="code-line">{`        ui_widget_append(group, create_label([color=#0a3069]"View"[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">142</Text>
                <Text className="code-line">{`        ui_widget_append(group, create_checkbox([color=#0a3069]"eye"[/color], [color=#0a3069]"Show Sidebar"[/color], [color=#0550ae]true[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">143</Text>
                <Text className="code-line">{`        ui_widget_append(group,`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">144</Text>
                <Text className="code-line">{`                         create_checkbox([color=#0a3069]"grid"[/color], [color=#0a3069]"Show Status Bar"[/color], [color=#0550ae]false[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">145</Text>
                <Text className="code-line">{`        sub_content = ui_create_dropdown_menu_sub_content();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">146</Text>
                <Text className="code-line">{`        ui_widget_append(sub_content, create_label([color=#0a3069]"Appearance"[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">147</Text>
                <Text className="code-line">{`        radios = ui_create_dropdown_menu_radio_group();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">148</Text>
                <Text className="code-line">{`        ui_widget_set_attr(radios, [color=#0a3069]"value"[/color], [color=#0a3069]"light"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">149</Text>
                <Text className="code-line">{`        ui_widget_append(radios,`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">150</Text>
                <Text className="code-line">{`                         create_radio([color=#0a3069]"weather-sunny"[/color], [color=#0a3069]"Light"[/color], [color=#0a3069]"light"[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">151</Text>
                <Text className="code-line">{`        ui_widget_append(radios, create_radio([color=#0a3069]"weather-moon"[/color], [color=#0a3069]"Dark"[/color], [color=#0a3069]"dark"[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">152</Text>
                <Text className="code-line">{`        ui_widget_append(radios, create_radio([color=#0a3069]"desktop"[/color], [color=#0a3069]"System"[/color], [color=#0a3069]"system"[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">153</Text>
                <Text className="code-line">{`        ui_widget_append(sub_content, radios);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">154</Text>
                <Text className="code-line">{`        ui_widget_append(group,`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">155</Text>
                <Text className="code-line">{`                         create_sub(create_sub_trigger([color=#0a3069]"paint-brush"[/color], [color=#0a3069]"Theme"[/color]),`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">156</Text>
                <Text className="code-line">{`                                    sub_content));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">157</Text>
                <Text className="code-line">{`        ui_widget_append(content, group);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">158</Text>
                <Text className="code-line">{`        ui_widget_append(content, ui_create_dropdown_menu_separator());`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">159</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">160</Text>
                <Text className="code-line">{`        group = ui_create_dropdown_menu_group();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">161</Text>
                <Text className="code-line">{`        ui_widget_append(group, create_label([color=#0a3069]"Account"[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">162</Text>
                <Text className="code-line">{`        ui_widget_append(`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">163</Text>
                <Text className="code-line">{`            group, create_item([color=#0a3069]"person"[/color], [color=#0a3069]"Profile"[/color], [color=#0a3069]"Shift+P"[/color], [color=#0550ae]false[/color], [color=#0550ae]false[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">164</Text>
                <Text className="code-line">{`        ui_widget_append(group,`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">165</Text>
                <Text className="code-line">{`                         create_item([color=#0a3069]"payment"[/color], [color=#0a3069]"Billing"[/color], [color=#0550ae]NULL[/color], [color=#0550ae]false[/color], [color=#0550ae]false[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">166</Text>
                <Text className="code-line">{`        sub_content = ui_create_dropdown_menu_sub_content();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">167</Text>
                <Text className="code-line">{`        ui_widget_append(sub_content, create_label([color=#0a3069]"Preferences"[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">168</Text>
                <Text className="code-line">{`        ui_widget_append(`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">169</Text>
                <Text className="code-line">{`            sub_content,`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">170</Text>
                <Text className="code-line">{`            create_item([color=#0a3069]"keyboard"[/color], [color=#0a3069]"Keyboard Shortcuts"[/color], [color=#0550ae]NULL[/color], [color=#0550ae]false[/color], [color=#0550ae]false[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">171</Text>
                <Text className="code-line">{`        ui_widget_append(sub_content, create_item([color=#0a3069]"local-language"[/color], [color=#0a3069]"Language"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">172</Text>
                <Text className="code-line">{`                                                  [color=#0550ae]NULL[/color], [color=#0550ae]false[/color], [color=#0550ae]false[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">173</Text>
                <Text className="code-line">{`        sub_sub_content = ui_create_dropdown_menu_sub_content();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">174</Text>
                <Text className="code-line">{`        ui_widget_append(sub_sub_content, create_label([color=#0a3069]"Notification Types"[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">175</Text>
                <Text className="code-line">{`        ui_widget_append(sub_sub_content,`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">176</Text>
                <Text className="code-line">{`                         create_checkbox([color=#0a3069]"alert"[/color], [color=#0a3069]"Push Notifications"[/color], [color=#0550ae]true[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">177</Text>
                <Text className="code-line">{`        ui_widget_append(sub_sub_content,`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">178</Text>
                <Text className="code-line">{`                         create_checkbox([color=#0a3069]"mail"[/color], [color=#0a3069]"Email Notifications"[/color], [color=#0550ae]true[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">179</Text>
                <Text className="code-line">{`        ui_widget_append(sub_content, create_sub(create_sub_trigger(`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">180</Text>
                <Text className="code-line">{`                                                     [color=#0a3069]"alert"[/color], [color=#0a3069]"Notifications"[/color]),`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">181</Text>
                <Text className="code-line">{`                                                 sub_sub_content));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">182</Text>
                <Text className="code-line">{`        ui_widget_append(sub_content, ui_create_dropdown_menu_separator());`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">183</Text>
                <Text className="code-line">{`        ui_widget_append(`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">184</Text>
                <Text className="code-line">{`            sub_content,`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">185</Text>
                <Text className="code-line">{`            create_item([color=#0a3069]"shield"[/color], [color=#0a3069]"Privacy & Security"[/color], [color=#0550ae]NULL[/color], [color=#0550ae]false[/color], [color=#0550ae]false[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">186</Text>
                <Text className="code-line">{`        ui_widget_append(group,`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">187</Text>
                <Text className="code-line">{`                         create_sub(create_sub_trigger([color=#0a3069]"settings"[/color], [color=#0a3069]"Settings"[/color]),`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">188</Text>
                <Text className="code-line">{`                                    sub_content));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">189</Text>
                <Text className="code-line">{`        ui_widget_append(content, group);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">190</Text>
                <Text className="code-line">{`        ui_widget_append(content, ui_create_dropdown_menu_separator());`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">191</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">192</Text>
                <Text className="code-line">{`        group = ui_create_dropdown_menu_group();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">193</Text>
                <Text className="code-line">{`        ui_widget_append(group, create_item([color=#0a3069]"question-circle"[/color], [color=#0a3069]"Help & Support"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">194</Text>
                <Text className="code-line">{`                                            [color=#0550ae]NULL[/color], [color=#0550ae]false[/color], [color=#0550ae]false[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">195</Text>
                <Text className="code-line">{`        ui_widget_append(group, create_item([color=#0a3069]"document-text"[/color], [color=#0a3069]"Documentation"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">196</Text>
                <Text className="code-line">{`                                            [color=#0550ae]NULL[/color], [color=#0550ae]false[/color], [color=#0550ae]false[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">197</Text>
                <Text className="code-line">{`        ui_widget_append(content, group);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">198</Text>
                <Text className="code-line">{`        ui_widget_append(content, ui_create_dropdown_menu_separator());`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">199</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">200</Text>
                <Text className="code-line">{`        group = ui_create_dropdown_menu_group();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">201</Text>
                <Text className="code-line">{`        ui_widget_append(`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">202</Text>
                <Text className="code-line">{`            group, create_item([color=#0a3069]"sign-out"[/color], [color=#0a3069]"Sign Out"[/color], [color=#0a3069]"Shift+Q"[/color], [color=#0550ae]false[/color], [color=#0550ae]true[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">203</Text>
                <Text className="code-line">{`        ui_widget_append(content, group);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">204</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">205</Text>
                <Text className="code-line">{`        ui_widget_append(menu, trigger);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">206</Text>
                <Text className="code-line">{`        ui_widget_append(menu, content);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">207</Text>
                <Text className="code-line">{`        ui_widget_append(parent, menu);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">208</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/dropdown-menu-complex/1" style={{ display: "none" }}>
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
                <Text className="code-line">{`  [color=#116329]<[color=#0550ae]dropdown-menu[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`    [color=#116329]<[color=#0550ae]dropdown-menu-trigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]button[/color]>[/color]Complex Menu[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`    [color=#116329]</[color=#0550ae]dropdown-menu-trigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`    [color=#116329]<[color=#0550ae]dropdown-menu-content[/color] [color=#0550ae]class[/color]=[color=#0a3069]"dropdown-menu-complex-demo-content"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]dropdown-menu-group[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-label[/color]>[/color]File[color=#116329]</[color=#0550ae]dropdown-menu-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color]>[/color]New File[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dropdown-menu-shortcut[/color]>[/color]Ctrl+N[color=#116329]</[color=#0550ae]dropdown-menu-shortcut[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"folder"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color]>[/color]New Folder[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dropdown-menu-shortcut[/color]>[/color]Shift+N[color=#116329]</[color=#0550ae]dropdown-menu-shortcut[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-sub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dropdown-menu-sub-trigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"folder-open"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]text[/color]>[/color]Open Recent[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"dropdown-menu-chevron"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]dropdown-menu-sub-trigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dropdown-menu-sub-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-label[/color]>[/color]Recent Projects[color=#116329]</[color=#0550ae]dropdown-menu-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"code"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]text[/color]>[/color]Project Alpha[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"code"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]text[/color]>[/color]Project Beta[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">35</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">36</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-sub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">37</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dropdown-menu-sub-trigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">38</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"more-horizontal"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">39</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color]>[/color]More Projects[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">40</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"dropdown-menu-chevron"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">41</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]dropdown-menu-sub-trigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">42</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dropdown-menu-sub-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">43</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">44</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"code"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">45</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]text[/color]>[/color]Project Gamma[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">46</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">47</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">48</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"code"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">49</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]text[/color]>[/color]Project Delta[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">50</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">51</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]dropdown-menu-sub-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">52</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]dropdown-menu-sub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">53</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-separator[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">54</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">55</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"search"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">56</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]text[/color]>[/color]Browse...[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">57</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">58</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]dropdown-menu-sub-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">59</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-sub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">60</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-separator[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">61</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">62</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"save"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">63</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color]>[/color]Save[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">64</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dropdown-menu-shortcut[/color]>[/color]Ctrl+S[color=#116329]</[color=#0550ae]dropdown-menu-shortcut[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">65</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">66</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">67</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"arrow-download"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">68</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color]>[/color]Export[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">69</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dropdown-menu-shortcut[/color]>[/color]Shift+E[color=#116329]</[color=#0550ae]dropdown-menu-shortcut[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">70</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">71</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]dropdown-menu-group[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">72</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]dropdown-menu-separator[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">73</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]dropdown-menu-group[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">74</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-label[/color]>[/color]View[color=#116329]</[color=#0550ae]dropdown-menu-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">75</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-checkbox-item[/color] [color=#0550ae]checked[/color]=[color=#0a3069]"true"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">76</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"eye"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">77</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color]>[/color]Show Sidebar[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">78</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-checkbox-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">79</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-checkbox-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">80</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"grid"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">81</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color]>[/color]Show Status Bar[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">82</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-checkbox-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">83</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-sub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">84</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dropdown-menu-sub-trigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">85</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"paint-brush"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">86</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]text[/color]>[/color]Theme[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">87</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"dropdown-menu-chevron"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">88</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]dropdown-menu-sub-trigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">89</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dropdown-menu-sub-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">90</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-label[/color]>[/color]Appearance[color=#116329]</[color=#0550ae]dropdown-menu-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">91</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-radio-group[/color] [color=#0550ae]value[/color]=[color=#0a3069]"light"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">92</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dropdown-menu-radio-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"light"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">93</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"weather-sunny"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">94</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color]>[/color]Light[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">95</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]dropdown-menu-radio-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">96</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dropdown-menu-radio-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"dark"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">97</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"weather-moon"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">98</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color]>[/color]Dark[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">99</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]dropdown-menu-radio-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">100</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dropdown-menu-radio-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"system"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">101</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"desktop"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">102</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color]>[/color]System[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">103</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]dropdown-menu-radio-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">104</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]dropdown-menu-radio-group[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">105</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]dropdown-menu-sub-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">106</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-sub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">107</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]dropdown-menu-group[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">108</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]dropdown-menu-separator[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">109</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]dropdown-menu-group[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">110</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-label[/color]>[/color]Account[color=#116329]</[color=#0550ae]dropdown-menu-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">111</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">112</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"person"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">113</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color]>[/color]Profile[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">114</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dropdown-menu-shortcut[/color]>[/color]Shift+P[color=#116329]</[color=#0550ae]dropdown-menu-shortcut[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">115</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">116</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">117</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"payment"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">118</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color]>[/color]Billing[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">119</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">120</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-sub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">121</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dropdown-menu-sub-trigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">122</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"settings"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">123</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]text[/color]>[/color]Settings[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">124</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"dropdown-menu-chevron"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">125</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]dropdown-menu-sub-trigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">126</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dropdown-menu-sub-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">127</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-label[/color]>[/color]Preferences[color=#116329]</[color=#0550ae]dropdown-menu-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">128</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">129</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"keyboard"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">130</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]text[/color]>[/color]Keyboard Shortcuts[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">131</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">132</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">133</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"local-language"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">134</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]text[/color]>[/color]Language[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">135</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">136</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-sub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">137</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dropdown-menu-sub-trigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">138</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"alert"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">139</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color]>[/color]Notifications[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">140</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"dropdown-menu-chevron"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">141</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]dropdown-menu-sub-trigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">142</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dropdown-menu-sub-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">143</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]dropdown-menu-label[/color]>[/color]Notification Types[color=#116329]</[color=#0550ae]dropdown-menu-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">144</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]dropdown-menu-checkbox-item[/color] [color=#0550ae]checked[/color]=[color=#0a3069]"true"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">145</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"alert"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">146</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]text[/color]>[/color]Push Notifications[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">147</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]dropdown-menu-checkbox-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">148</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]dropdown-menu-checkbox-item[/color] [color=#0550ae]checked[/color]=[color=#0a3069]"true"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">149</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"mail"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">150</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]text[/color]>[/color]Email Notifications[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">151</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]dropdown-menu-checkbox-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">152</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]dropdown-menu-sub-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">153</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]dropdown-menu-sub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">154</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-separator[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">155</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">156</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"shield"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">157</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]text[/color]>[/color]Privacy [color=#0550ae]&amp;[/color] Security[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">158</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">159</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]dropdown-menu-sub-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">160</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-sub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">161</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]dropdown-menu-group[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">162</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]dropdown-menu-separator[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">163</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]dropdown-menu-group[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">164</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">165</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"question-circle"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">166</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color]>[/color]Help [color=#0550ae]&amp;[/color] Support[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">167</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">168</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">169</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">170</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color]>[/color]Documentation[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">171</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">172</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]dropdown-menu-group[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">173</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]dropdown-menu-separator[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">174</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]dropdown-menu-group[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">175</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]dropdown-menu-item[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"destructive"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">176</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"sign-out"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">177</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color]>[/color]Sign Out[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">178</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dropdown-menu-shortcut[/color]>[/color]Shift+Q[color=#116329]</[color=#0550ae]dropdown-menu-shortcut[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">179</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">180</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]dropdown-menu-group[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">181</Text>
                <Text className="code-line">{`    [color=#116329]</[color=#0550ae]dropdown-menu-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">182</Text>
                <Text className="code-line">{`  [color=#116329]</[color=#0550ae]dropdown-menu[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">183</Text>
                <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/dropdown-menu-complex/2" style={{ display: "none" }}>
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
                <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]dropdown_menu_complex_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
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
        <Widget className="code-block-body" data-language="tsx" data-file="example.tsx" data-source="examples/dropdown-menu-complex/3" style={{ display: "none" }}>
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
                <Text className="code-line">{`  [color=#8250df]DropdownMenu[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`  [color=#8250df]DropdownMenuCheckboxItem[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`  [color=#8250df]DropdownMenuContent[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`  [color=#8250df]DropdownMenuGroup[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`  [color=#8250df]DropdownMenuItem[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{`  [color=#8250df]DropdownMenuLabel[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`  [color=#8250df]DropdownMenuRadioGroup[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`  [color=#8250df]DropdownMenuRadioItem[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`  [color=#8250df]DropdownMenuSeparator[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`  [color=#8250df]DropdownMenuShortcut[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`  [color=#8250df]DropdownMenuSub[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`  [color=#8250df]DropdownMenuSubContent[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`  [color=#8250df]DropdownMenuSubTrigger[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`  [color=#8250df]DropdownMenuTrigger[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`  [color=#8250df]Icon[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`  [color=#8250df]Text[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Example[/color]([color=#0550ae][/color]) {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`    [color=#116329]<[color=#0550ae]DropdownMenu[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]DropdownMenuTrigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Button[/color]>[/color]Complex Menu[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]DropdownMenuTrigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]DropdownMenuContent[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-44"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]DropdownMenuGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuLabel[/color]>[/color]File[color=#116329]</[color=#0550ae]DropdownMenuLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Text[/color]>[/color]New File[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DropdownMenuShortcut[/color]>[/color]Ctrl+N[color=#116329]</[color=#0550ae]DropdownMenuShortcut[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">35</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">36</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"folder"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">37</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Text[/color]>[/color]New Folder[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">38</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DropdownMenuShortcut[/color]>[/color]Shift+N[color=#116329]</[color=#0550ae]DropdownMenuShortcut[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">39</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">40</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuSub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">41</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DropdownMenuSubTrigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">42</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"folder-open"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">43</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Text[/color]>[/color]Open Recent[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">44</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"dropdown-menu-chevron"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">45</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]DropdownMenuSubTrigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">46</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DropdownMenuSubContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">47</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuLabel[/color]>[/color]Recent Projects[color=#116329]</[color=#0550ae]DropdownMenuLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">48</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">49</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"code"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">50</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Text[/color]>[/color]Project Alpha[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">51</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">52</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">53</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"code"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">54</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Text[/color]>[/color]Project Beta[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">55</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">56</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuSub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">57</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]DropdownMenuSubTrigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">58</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"more-horizontal"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">59</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Text[/color]>[/color]More Projects[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">60</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"dropdown-menu-chevron"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">61</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]DropdownMenuSubTrigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">62</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]DropdownMenuSubContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">63</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">64</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"code"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">65</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Text[/color]>[/color]Project Gamma[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">66</Text>
                <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">67</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">68</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"code"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">69</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Text[/color]>[/color]Project Delta[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">70</Text>
                <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">71</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]DropdownMenuSubContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">72</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]DropdownMenuSub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">73</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuSeparator[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">74</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">75</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"search"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">76</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Text[/color]>[/color]Browse...[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">77</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">78</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]DropdownMenuSubContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">79</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuSub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">80</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuSeparator[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">81</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">82</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"save"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">83</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Text[/color]>[/color]Save[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">84</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DropdownMenuShortcut[/color]>[/color]Ctrl+S[color=#116329]</[color=#0550ae]DropdownMenuShortcut[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">85</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">86</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">87</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"arrow-download"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">88</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Text[/color]>[/color]Export[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">89</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DropdownMenuShortcut[/color]>[/color]Shift+E[color=#116329]</[color=#0550ae]DropdownMenuShortcut[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">90</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">91</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]DropdownMenuGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">92</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]DropdownMenuSeparator[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">93</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]DropdownMenuGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">94</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuLabel[/color]>[/color]View[color=#116329]</[color=#0550ae]DropdownMenuLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">95</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuCheckboxItem[/color] [color=#0550ae]checked[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">96</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"eye"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">97</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Text[/color]>[/color]Show Sidebar[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">98</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuCheckboxItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">99</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuCheckboxItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">100</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"grid"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">101</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Text[/color]>[/color]Show Status Bar[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">102</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuCheckboxItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">103</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuSub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">104</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DropdownMenuSubTrigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">105</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"paint-brush"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">106</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Text[/color]>[/color]Theme[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">107</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"dropdown-menu-chevron"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">108</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]DropdownMenuSubTrigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">109</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DropdownMenuSubContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">110</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuLabel[/color]>[/color]Appearance[color=#116329]</[color=#0550ae]DropdownMenuLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">111</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuRadioGroup[/color] [color=#0550ae]value[/color]=[color=#0a3069]"light"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">112</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]DropdownMenuRadioItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"light"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">113</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"weather-sunny"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">114</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Text[/color]>[/color]Light[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">115</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]DropdownMenuRadioItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">116</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]DropdownMenuRadioItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"dark"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">117</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"weather-moon"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">118</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Text[/color]>[/color]Dark[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">119</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]DropdownMenuRadioItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">120</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]DropdownMenuRadioItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"system"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">121</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"desktop"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">122</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Text[/color]>[/color]System[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">123</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]DropdownMenuRadioItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">124</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]DropdownMenuRadioGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">125</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]DropdownMenuSubContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">126</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuSub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">127</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]DropdownMenuGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">128</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]DropdownMenuSeparator[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">129</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]DropdownMenuGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">130</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuLabel[/color]>[/color]Account[color=#116329]</[color=#0550ae]DropdownMenuLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">131</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">132</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"person"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">133</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Text[/color]>[/color]Profile[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">134</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DropdownMenuShortcut[/color]>[/color]Shift+P[color=#116329]</[color=#0550ae]DropdownMenuShortcut[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">135</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">136</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">137</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"payment"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">138</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Text[/color]>[/color]Billing[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">139</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">140</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuSub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">141</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DropdownMenuSubTrigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">142</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"settings"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">143</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Text[/color]>[/color]Settings[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">144</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"dropdown-menu-chevron"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">145</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]DropdownMenuSubTrigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">146</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DropdownMenuSubContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">147</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuLabel[/color]>[/color]Preferences[color=#116329]</[color=#0550ae]DropdownMenuLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">148</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">149</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"keyboard"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">150</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Text[/color]>[/color]Keyboard Shortcuts[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">151</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">152</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">153</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"local-language"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">154</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Text[/color]>[/color]Language[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">155</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">156</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuSub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">157</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]DropdownMenuSubTrigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">158</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"alert"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">159</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Text[/color]>[/color]Notifications[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">160</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"dropdown-menu-chevron"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">161</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]DropdownMenuSubTrigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">162</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]DropdownMenuSubContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">163</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuLabel[/color]>[/color]Notification Types[color=#116329]</[color=#0550ae]DropdownMenuLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">164</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuCheckboxItem[/color] [color=#0550ae]checked[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">165</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"alert"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">166</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Text[/color]>[/color]Push Notifications[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">167</Text>
                <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]DropdownMenuCheckboxItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">168</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuCheckboxItem[/color] [color=#0550ae]checked[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">169</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"mail"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">170</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Text[/color]>[/color]Email Notifications[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">171</Text>
                <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]DropdownMenuCheckboxItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">172</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]DropdownMenuSubContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">173</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]DropdownMenuSub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">174</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuSeparator[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">175</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">176</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"shield"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">177</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Text[/color]>[/color]Privacy [color=#0550ae]&amp;[/color] Security[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">178</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">179</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]DropdownMenuSubContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">180</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuSub[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">181</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]DropdownMenuGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">182</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]DropdownMenuSeparator[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">183</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]DropdownMenuGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">184</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">185</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"question-circle"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">186</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Text[/color]>[/color]Help [color=#0550ae]&amp;[/color] Support[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">187</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">188</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">189</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">190</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Text[/color]>[/color]Documentation[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">191</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">192</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]DropdownMenuGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">193</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]DropdownMenuSeparator[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">194</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]DropdownMenuGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">195</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DropdownMenuItem[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"destructive"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">196</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"sign-out"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">197</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Text[/color]>[/color]Sign Out[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">198</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]DropdownMenuShortcut[/color]>[/color]Shift+Q[color=#116329]</[color=#0550ae]DropdownMenuShortcut[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">199</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">200</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]DropdownMenuGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">201</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]DropdownMenuContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">202</Text>
                <Text className="code-line">{`    [color=#116329]</[color=#0550ae]DropdownMenu[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">203</Text>
                <Text className="code-line">{`  );`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">204</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/dropdown-menu-complex/4" style={{ display: "none" }}>
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
                <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]dropdown_menu_complex_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
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
      </Widget>
    </DemoProvider>
  );
}
