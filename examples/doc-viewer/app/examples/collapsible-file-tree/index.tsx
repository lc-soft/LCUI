import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function CollapsibleFileTreeDemo() {
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
        <Widget className="code-block-body active" data-language="c" data-file="main.c" data-source="examples/collapsible-file-tree/0">
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
                  <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]const[/color] [color=#953800]char[/color] *collapsible_file_tree_css = [color=#0a3069]"\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`[color=#0a3069].collapsible-file-tree-demo {\\[/color]`}</Text>
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
                  <Text className="code-line">{`[color=#0a3069].collapsible-file-tree-demo card {\\[/color]`}</Text>
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
                  <Text className="code-line">{`[color=#0a3069].collapsible-file-tree-demo .file-tree {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`[color=#0a3069]  display: flex;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`[color=#0a3069]  flex-direction: column;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`[color=#0a3069]  gap: 2px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`[color=#0a3069]  padding-top: 8px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`[color=#0a3069].collapsible-file-tree-demo .file-row {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`[color=#0a3069]  width: 100%;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`[color=#0a3069]  justify-content: flex-start;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`[color=#0a3069]  border: 0;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`[color=#0a3069]  background: transparent;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`[color=#0a3069].collapsible-file-tree-demo .file-children {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`[color=#0a3069]  display: flex;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`[color=#0a3069]  flex-direction: column;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`[color=#0a3069]  gap: 2px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`[color=#0a3069]  padding-left: 20px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`[color=#0a3069]"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_file_row[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *name)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *icon = ui_create_fluent_icon([color=#0a3069]"document-text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *label = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`        ui_button_set_variant(button, UI_BUTTON_VARIANT_LINK);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`        ui_button_set_size(button, UI_BUTTON_SIZE_SM);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`        ui_widget_add_class(button, [color=#0a3069]"file-row"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`        ui_text_set_content(label, name);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`        ui_widget_append(button, icon);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`        ui_widget_append(button, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`        [color=#cf222e]return[/color] button;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_folder_row[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *name, [color=#953800]bool[/color] open,[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`[color=#0550ae]                                      [color=#953800]ui_widget_t[/color] **children)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *collapsible = ui_create_collapsible();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *trigger = ui_create_collapsible_trigger();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *chevron = ui_create_fluent_icon([color=#0a3069]"chevron-right"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *folder = ui_create_fluent_icon([color=#0a3069]"folder-open"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *label = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_collapsible_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`        ui_button_set_variant(button, UI_BUTTON_VARIANT_GHOST);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`        ui_button_set_size(button, UI_BUTTON_SIZE_SM);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`        ui_widget_add_class(button, [color=#0a3069]"file-row"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`        ui_text_set_content(label, name);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`        ui_widget_append(button, chevron);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`        ui_widget_append(button, folder);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`        ui_widget_append(button, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`        ui_widget_append(trigger, button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`        ui_widget_add_class(content, [color=#0a3069]"file-children"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`        ui_widget_append(collapsible, trigger);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`        ui_widget_append(collapsible, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`        [color=#cf222e]if[/color] (open) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`                ui_collapsible_set_open(collapsible, [color=#0550ae]true[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`        }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`        *children = content;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`        [color=#cf222e]return[/color] collapsible;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">73</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">74</Text>
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]collapsible_file_tree_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">75</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">76</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *box = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">77</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *card = ui_create_card();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">78</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *header = ui_create_card_header();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">79</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *title = ui_create_card_title();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">80</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_card_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">81</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *tree = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">82</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *components;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">83</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *components_children;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">84</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *ui_folder;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">85</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *ui_children;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">86</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *lib;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">87</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *lib_children;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">88</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">89</Text>
                  <Text className="code-line">{`        ui_load_css_string(collapsible_file_tree_css,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">90</Text>
                  <Text className="code-line">{`                           [color=#0a3069]"collapsible-file-tree-demo"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">91</Text>
                  <Text className="code-line">{`        ui_widget_add_class(box, [color=#0a3069]"collapsible-file-tree-demo"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">92</Text>
                  <Text className="code-line">{`        ui_card_set_size(card, UI_CARD_SIZE_SM);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">93</Text>
                  <Text className="code-line">{`        ui_text_set_content(title, [color=#0a3069]"Project files"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">94</Text>
                  <Text className="code-line">{`        ui_widget_add_class(tree, [color=#0a3069]"file-tree"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">95</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">96</Text>
                  <Text className="code-line">{`        components =`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">97</Text>
                  <Text className="code-line">{`            create_folder_row([color=#0a3069]"components"[/color], [color=#0550ae]true[/color], &components_children);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">98</Text>
                  <Text className="code-line">{`        ui_folder = create_folder_row([color=#0a3069]"ui"[/color], [color=#0550ae]true[/color], &ui_children);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">99</Text>
                  <Text className="code-line">{`        ui_widget_append(ui_children, create_file_row([color=#0a3069]"button.tsx"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">100</Text>
                  <Text className="code-line">{`        ui_widget_append(ui_children, create_file_row([color=#0a3069]"card.tsx"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">101</Text>
                  <Text className="code-line">{`        ui_widget_append(ui_children, create_file_row([color=#0a3069]"dialog.tsx"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">102</Text>
                  <Text className="code-line">{`        ui_widget_append(components_children, ui_folder);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">103</Text>
                  <Text className="code-line">{`        ui_widget_append(components_children,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">104</Text>
                  <Text className="code-line">{`                         create_file_row([color=#0a3069]"login-form.tsx"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">105</Text>
                  <Text className="code-line">{`        ui_widget_append(components_children,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">106</Text>
                  <Text className="code-line">{`                         create_file_row([color=#0a3069]"register-form.tsx"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">107</Text>
                  <Text className="code-line">{`        ui_widget_append(tree, components);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">108</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">109</Text>
                  <Text className="code-line">{`        lib = create_folder_row([color=#0a3069]"lib"[/color], [color=#0550ae]false[/color], &lib_children);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">110</Text>
                  <Text className="code-line">{`        ui_widget_append(lib_children, create_file_row([color=#0a3069]"utils.ts"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">111</Text>
                  <Text className="code-line">{`        ui_widget_append(lib_children, create_file_row([color=#0a3069]"api.ts"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">112</Text>
                  <Text className="code-line">{`        ui_widget_append(tree, lib);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">113</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">114</Text>
                  <Text className="code-line">{`        ui_widget_append(tree, create_file_row([color=#0a3069]"app.tsx"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">115</Text>
                  <Text className="code-line">{`        ui_widget_append(tree, create_file_row([color=#0a3069]"package.json"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">116</Text>
                  <Text className="code-line">{`        ui_widget_append(tree, create_file_row([color=#0a3069]"README.md"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">117</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">118</Text>
                  <Text className="code-line">{`        ui_widget_append(header, title);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">119</Text>
                  <Text className="code-line">{`        ui_widget_append(content, tree);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">120</Text>
                  <Text className="code-line">{`        ui_widget_append(card, header);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">121</Text>
                  <Text className="code-line">{`        ui_widget_append(card, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">122</Text>
                  <Text className="code-line">{`        ui_widget_append(box, card);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">123</Text>
                  <Text className="code-line">{`        ui_widget_append(parent, box);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">124</Text>
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
                  <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]const[/color] [color=#ffa657]char[/color] *collapsible_file_tree_css = [color=#a5d6ff]"\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`[color=#a5d6ff].collapsible-file-tree-demo {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  width: 320px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`[color=#a5d6ff]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`[color=#a5d6ff].collapsible-file-tree-demo card {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  width: 100%;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`[color=#a5d6ff]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`[color=#a5d6ff].collapsible-file-tree-demo .file-tree {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  display: flex;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  flex-direction: column;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  gap: 2px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  padding-top: 8px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`[color=#a5d6ff]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`[color=#a5d6ff].collapsible-file-tree-demo .file-row {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  width: 100%;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  justify-content: flex-start;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  border: 0;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  background: transparent;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`[color=#a5d6ff]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`[color=#a5d6ff].collapsible-file-tree-demo .file-children {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  display: flex;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  flex-direction: column;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  gap: 2px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  padding-left: 20px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`[color=#a5d6ff]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`[color=#a5d6ff]"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]ui_widget_t[/color] *[color=#d2a8ff]create_file_row[/color][color=#79c0ff]([color=#ffa657]const[/color] [color=#ffa657]char[/color] *name)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *icon = ui_create_fluent_icon([color=#a5d6ff]"document-text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *label = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`        ui_button_set_variant(button, UI_BUTTON_VARIANT_LINK);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`        ui_button_set_size(button, UI_BUTTON_SIZE_SM);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`        ui_widget_add_class(button, [color=#a5d6ff]"file-row"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`        ui_text_set_content(label, name);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`        ui_widget_append(button, icon);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`        ui_widget_append(button, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`        [color=#ff7b72]return[/color] button;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]ui_widget_t[/color] *[color=#d2a8ff]create_folder_row[/color][color=#79c0ff]([color=#ffa657]const[/color] [color=#ffa657]char[/color] *name, [color=#ffa657]bool[/color] open,[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`[color=#79c0ff]                                      [color=#ffa657]ui_widget_t[/color] **children)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *collapsible = ui_create_collapsible();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *trigger = ui_create_collapsible_trigger();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *chevron = ui_create_fluent_icon([color=#a5d6ff]"chevron-right"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *folder = ui_create_fluent_icon([color=#a5d6ff]"folder-open"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *label = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *content = ui_create_collapsible_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`        ui_button_set_variant(button, UI_BUTTON_VARIANT_GHOST);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`        ui_button_set_size(button, UI_BUTTON_SIZE_SM);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`        ui_widget_add_class(button, [color=#a5d6ff]"file-row"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`        ui_text_set_content(label, name);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`        ui_widget_append(button, chevron);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`        ui_widget_append(button, folder);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`        ui_widget_append(button, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`        ui_widget_append(trigger, button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`        ui_widget_add_class(content, [color=#a5d6ff]"file-children"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`        ui_widget_append(collapsible, trigger);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`        ui_widget_append(collapsible, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`        [color=#ff7b72]if[/color] (open) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`                ui_collapsible_set_open(collapsible, [color=#79c0ff]true[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`        }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`        *children = content;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`        [color=#ff7b72]return[/color] collapsible;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">73</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">74</Text>
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]collapsible_file_tree_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">75</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">76</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *box = ui_create_widget([color=#79c0ff]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">77</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *card = ui_create_card();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">78</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *header = ui_create_card_header();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">79</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *title = ui_create_card_title();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">80</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *content = ui_create_card_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">81</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *tree = ui_create_widget([color=#79c0ff]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">82</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *components;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">83</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *components_children;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">84</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *ui_folder;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">85</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *ui_children;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">86</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *lib;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">87</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *lib_children;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">88</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">89</Text>
                  <Text className="code-line">{`        ui_load_css_string(collapsible_file_tree_css,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">90</Text>
                  <Text className="code-line">{`                           [color=#a5d6ff]"collapsible-file-tree-demo"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">91</Text>
                  <Text className="code-line">{`        ui_widget_add_class(box, [color=#a5d6ff]"collapsible-file-tree-demo"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">92</Text>
                  <Text className="code-line">{`        ui_card_set_size(card, UI_CARD_SIZE_SM);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">93</Text>
                  <Text className="code-line">{`        ui_text_set_content(title, [color=#a5d6ff]"Project files"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">94</Text>
                  <Text className="code-line">{`        ui_widget_add_class(tree, [color=#a5d6ff]"file-tree"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">95</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">96</Text>
                  <Text className="code-line">{`        components =`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">97</Text>
                  <Text className="code-line">{`            create_folder_row([color=#a5d6ff]"components"[/color], [color=#79c0ff]true[/color], &components_children);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">98</Text>
                  <Text className="code-line">{`        ui_folder = create_folder_row([color=#a5d6ff]"ui"[/color], [color=#79c0ff]true[/color], &ui_children);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">99</Text>
                  <Text className="code-line">{`        ui_widget_append(ui_children, create_file_row([color=#a5d6ff]"button.tsx"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">100</Text>
                  <Text className="code-line">{`        ui_widget_append(ui_children, create_file_row([color=#a5d6ff]"card.tsx"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">101</Text>
                  <Text className="code-line">{`        ui_widget_append(ui_children, create_file_row([color=#a5d6ff]"dialog.tsx"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">102</Text>
                  <Text className="code-line">{`        ui_widget_append(components_children, ui_folder);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">103</Text>
                  <Text className="code-line">{`        ui_widget_append(components_children,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">104</Text>
                  <Text className="code-line">{`                         create_file_row([color=#a5d6ff]"login-form.tsx"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">105</Text>
                  <Text className="code-line">{`        ui_widget_append(components_children,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">106</Text>
                  <Text className="code-line">{`                         create_file_row([color=#a5d6ff]"register-form.tsx"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">107</Text>
                  <Text className="code-line">{`        ui_widget_append(tree, components);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">108</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">109</Text>
                  <Text className="code-line">{`        lib = create_folder_row([color=#a5d6ff]"lib"[/color], [color=#79c0ff]false[/color], &lib_children);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">110</Text>
                  <Text className="code-line">{`        ui_widget_append(lib_children, create_file_row([color=#a5d6ff]"utils.ts"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">111</Text>
                  <Text className="code-line">{`        ui_widget_append(lib_children, create_file_row([color=#a5d6ff]"api.ts"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">112</Text>
                  <Text className="code-line">{`        ui_widget_append(tree, lib);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">113</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">114</Text>
                  <Text className="code-line">{`        ui_widget_append(tree, create_file_row([color=#a5d6ff]"app.tsx"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">115</Text>
                  <Text className="code-line">{`        ui_widget_append(tree, create_file_row([color=#a5d6ff]"package.json"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">116</Text>
                  <Text className="code-line">{`        ui_widget_append(tree, create_file_row([color=#a5d6ff]"README.md"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">117</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">118</Text>
                  <Text className="code-line">{`        ui_widget_append(header, title);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">119</Text>
                  <Text className="code-line">{`        ui_widget_append(content, tree);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">120</Text>
                  <Text className="code-line">{`        ui_widget_append(card, header);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">121</Text>
                  <Text className="code-line">{`        ui_widget_append(card, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">122</Text>
                  <Text className="code-line">{`        ui_widget_append(box, card);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">123</Text>
                  <Text className="code-line">{`        ui_widget_append(parent, box);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">124</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/collapsible-file-tree/1" style={{ display: "none" }}>
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
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]resource[/color] [color=#0550ae]type[/color]=[color=#0a3069]"text/css"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`    .collapsible-file-tree-demo {`}</Text>
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
                  <Text className="code-line">{`    .collapsible-file-tree-demo card {`}</Text>
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
                  <Text className="code-line">{`    .collapsible-file-tree-demo .file-tree {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`      display: flex;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`      flex-direction: column;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`      gap: 2px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`      padding-top: 8px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`    .collapsible-file-tree-demo .file-row {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`      width: 100%;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`      justify-content: flex-start;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`      border: 0;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`      background: transparent;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`    .collapsible-file-tree-demo .file-children {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`      display: flex;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`      flex-direction: column;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`      gap: 2px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`      padding-left: 20px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]resource[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]widget[/color] [color=#0550ae]class[/color]=[color=#0a3069]"collapsible-file-tree-demo"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]card[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]card-header[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]card-title[/color]>[/color]Project files[color=#116329]</[color=#0550ae]card-title[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]card-header[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]card-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"file-tree"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]collapsible[/color] [color=#0550ae]default-open[/color]=[color=#0a3069]"true"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"file-row"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"folder-open"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]text[/color]>[/color]components[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"file-children"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]collapsible[/color] [color=#0550ae]default-open[/color]=[color=#0a3069]"true"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"file-row"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"folder-open"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]text[/color]>[/color]ui[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"file-children"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"file-row"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]text[/color]>[/color]button.tsx[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"file-row"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]text[/color]>[/color]card.tsx[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"file-row"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]text[/color]>[/color]dialog.tsx[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"file-row"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">73</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">74</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]text[/color]>[/color]login-form.tsx[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">75</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">76</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"file-row"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">77</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">78</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]text[/color]>[/color]register-form.tsx[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">79</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">80</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">81</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">82</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">83</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">84</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">85</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"file-row"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">86</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">87</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"folder-open"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">88</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]text[/color]>[/color]lib[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">89</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">90</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">91</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">92</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"file-children"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">93</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"file-row"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">94</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">95</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]text[/color]>[/color]utils.ts[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">96</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">97</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"file-row"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">98</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">99</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]text[/color]>[/color]api.ts[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">100</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">101</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">102</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">103</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">104</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"file-row"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">105</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">106</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]text[/color]>[/color]app.tsx[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">107</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">108</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"file-row"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">109</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">110</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]text[/color]>[/color]package.json[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">111</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">112</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"file-row"[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">113</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">114</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]text[/color]>[/color]README.md[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">115</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">116</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">117</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]card-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">118</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]card[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">119</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">120</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">121</Text>
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
                  <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]resource[/color] [color=#79c0ff]type[/color]=[color=#a5d6ff]"text/css"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`    .collapsible-file-tree-demo {`}</Text>
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
                  <Text className="code-line">{`    .collapsible-file-tree-demo card {`}</Text>
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
                  <Text className="code-line">{`    .collapsible-file-tree-demo .file-tree {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`      display: flex;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`      flex-direction: column;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`      gap: 2px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`      padding-top: 8px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`    .collapsible-file-tree-demo .file-row {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`      width: 100%;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`      justify-content: flex-start;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`      border: 0;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`      background: transparent;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`    .collapsible-file-tree-demo .file-children {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`      display: flex;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`      flex-direction: column;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`      gap: 2px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`      padding-left: 20px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]resource[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]ui[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]widget[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"collapsible-file-tree-demo"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]card[/color] [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]card-header[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]card-title[/color]>[/color]Project files[color=#7ee787]</[color=#79c0ff]card-title[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]card-header[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]card-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]div[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"file-tree"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]collapsible[/color] [color=#79c0ff]default-open[/color]=[color=#a5d6ff]"true"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"file-row"[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"ghost"[/color] [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"folder-open"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]components[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]div[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"file-children"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]collapsible[/color] [color=#79c0ff]default-open[/color]=[color=#a5d6ff]"true"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"file-row"[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"ghost"[/color] [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"folder-open"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]ui[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]div[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"file-children"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"file-row"[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"link"[/color] [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]button.tsx[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"file-row"[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"link"[/color] [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]card.tsx[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"file-row"[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"link"[/color] [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]dialog.tsx[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"file-row"[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"link"[/color] [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">73</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">74</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]login-form.tsx[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">75</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">76</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"file-row"[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"link"[/color] [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">77</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">78</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]register-form.tsx[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">79</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">80</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">81</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">82</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">83</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">84</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">85</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"file-row"[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"ghost"[/color] [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">86</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">87</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"folder-open"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">88</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]lib[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">89</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">90</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">91</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">92</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]div[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"file-children"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">93</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"file-row"[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"link"[/color] [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">94</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">95</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]utils.ts[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">96</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">97</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"file-row"[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"link"[/color] [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">98</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">99</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]api.ts[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">100</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">101</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">102</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">103</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">104</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"file-row"[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"link"[/color] [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">105</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">106</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]app.tsx[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">107</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">108</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"file-row"[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"link"[/color] [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">109</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">110</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]package.json[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">111</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">112</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"file-row"[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"link"[/color] [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">113</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">114</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]README.md[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">115</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">116</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">117</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]card-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">118</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]card[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">119</Text>
                  <Text className="code-line">{`    [color=#7ee787]</[color=#79c0ff]widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">120</Text>
                  <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]ui[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">121</Text>
                  <Text className="code-line">{`[color=#7ee787]</[color=#79c0ff]lcui-app[/color]>[/color]`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/collapsible-file-tree/2" style={{ display: "none" }}>
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
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]collapsible_file_tree_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
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
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]collapsible_file_tree_xml_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
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
        <Widget className="code-block-body" data-language="tsx" data-file="example.tsx" data-source="examples/collapsible-file-tree/3" style={{ display: "none" }}>
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
                  <Text className="code-line">{`  [color=#8250df]Card[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#8250df]CardContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#8250df]CardHeader[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#8250df]CardTitle[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#8250df]Collapsible[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#8250df]CollapsibleContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`  [color=#8250df]CollapsibleTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`  [color=#8250df]Icon[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`  [color=#8250df]Text[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-\\[320px]"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Card[/color] [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-full"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]CardHeader[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]CardTitle[/color]>[/color]Project files[color=#116329]</[color=#0550ae]CardTitle[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]CardHeader[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]CardContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-col gap-0.5 pt-2"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Collapsible[/color] [color=#0550ae]defaultOpen[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Button[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`[color=#116329]                  [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`[color=#116329]                  [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`[color=#116329]                  [color=#0550ae]className[/color]=[color=#0a3069]"w-full justify-start border-0 bg-transparent"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`[color=#116329]                >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"folder-open"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Text[/color]>[/color]components[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"ml-5 flex flex-col gap-0.5"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Collapsible[/color] [color=#0550ae]defaultOpen[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]Button[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`[color=#116329]                        [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`[color=#116329]                        [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`[color=#116329]                        [color=#0550ae]className[/color]=[color=#0a3069]"w-full justify-start border-0 bg-transparent"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`[color=#116329]                      >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"folder-open"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]Text[/color]>[/color]ui[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"ml-5 flex flex-col gap-0.5"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`                        {\\["button.tsx", "card.tsx", "dialog.tsx"].map((name) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]Button[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`[color=#116329]                            [color=#0550ae]key[/color]=[color=#0a3069]{name}[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`[color=#116329]                            [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`[color=#116329]                            [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`[color=#116329]                            [color=#0550ae]className[/color]=[color=#0a3069]"w-full justify-start"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`[color=#116329]                          >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]Text[/color]>[/color]{name}[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`                        ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]Collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`                  {\\["login-form.tsx", "register-form.tsx"].map((name) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Button[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`[color=#116329]                      [color=#0550ae]key[/color]=[color=#0a3069]{name}[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`[color=#116329]                      [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`[color=#116329]                      [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`[color=#116329]                      [color=#0550ae]className[/color]=[color=#0a3069]"w-full justify-start"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`[color=#116329]                    >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">73</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">74</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]Text[/color]>[/color]{name}[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">75</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">76</Text>
                  <Text className="code-line">{`                  ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">77</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">78</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">79</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]Collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">80</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">81</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">82</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Button[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">83</Text>
                  <Text className="code-line">{`[color=#116329]                  [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">84</Text>
                  <Text className="code-line">{`[color=#116329]                  [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">85</Text>
                  <Text className="code-line">{`[color=#116329]                  [color=#0550ae]className[/color]=[color=#0a3069]"w-full justify-start border-0 bg-transparent"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">86</Text>
                  <Text className="code-line">{`[color=#116329]                >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">87</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">88</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"folder-open"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">89</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Text[/color]>[/color]lib[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">90</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">91</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">92</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">93</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"ml-5 flex flex-col gap-0.5"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">94</Text>
                  <Text className="code-line">{`                  {\\["utils.ts", "api.ts"].map((name) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">95</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Button[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">96</Text>
                  <Text className="code-line">{`[color=#116329]                      [color=#0550ae]key[/color]=[color=#0a3069]{name}[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">97</Text>
                  <Text className="code-line">{`[color=#116329]                      [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">98</Text>
                  <Text className="code-line">{`[color=#116329]                      [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">99</Text>
                  <Text className="code-line">{`[color=#116329]                      [color=#0550ae]className[/color]=[color=#0a3069]"w-full justify-start"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">100</Text>
                  <Text className="code-line">{`[color=#116329]                    >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">101</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">102</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]Text[/color]>[/color]{name}[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">103</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">104</Text>
                  <Text className="code-line">{`                  ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">105</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">106</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">107</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]Collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">108</Text>
                  <Text className="code-line">{`            {\\["app.tsx", "package.json", "README.md"].map((name) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">109</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Button[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">110</Text>
                  <Text className="code-line">{`[color=#116329]                [color=#0550ae]key[/color]=[color=#0a3069]{name}[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">111</Text>
                  <Text className="code-line">{`[color=#116329]                [color=#0550ae]variant[/color]=[color=#0a3069]"link"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">112</Text>
                  <Text className="code-line">{`[color=#116329]                [color=#0550ae]size[/color]=[color=#0a3069]"sm"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">113</Text>
                  <Text className="code-line">{`[color=#116329]                [color=#0550ae]className[/color]=[color=#0a3069]"w-full justify-start"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">114</Text>
                  <Text className="code-line">{`[color=#116329]              >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">115</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">116</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]Text[/color]>[/color]{name}[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">117</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">118</Text>
                  <Text className="code-line">{`            ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">119</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">120</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]CardContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">121</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Card[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">122</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">123</Text>
                  <Text className="code-line">{`  );`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">124</Text>
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
                  <Text className="code-line">{`  [color=#d2a8ff]Card[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]CardContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]CardHeader[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]CardTitle[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]Collapsible[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]CollapsibleContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]CollapsibleTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]Icon[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]Text[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]Widget[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`} [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`[color=#ff7b72]export[/color] [color=#ff7b72]default[/color] [color=#ff7b72]function[/color] [color=#d2a8ff]App[/color]([color=#79c0ff][/color]) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`  [color=#ff7b72]return[/color] (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]Widget[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"w-\\[320px]"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]Card[/color] [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"w-full"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]CardHeader[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]CardTitle[/color]>[/color]Project files[color=#7ee787]</[color=#79c0ff]CardTitle[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]CardHeader[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]CardContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]Widget[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"flex flex-col gap-0.5 pt-2"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]Collapsible[/color] [color=#79c0ff]defaultOpen[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]Button[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`[color=#7ee787]                  [color=#79c0ff]variant[/color]=[color=#a5d6ff]"ghost"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`[color=#7ee787]                  [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`[color=#7ee787]                  [color=#79c0ff]className[/color]=[color=#a5d6ff]"w-full justify-start border-0 bg-transparent"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`[color=#7ee787]                >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"folder-open"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]components[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]Widget[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"ml-5 flex flex-col gap-0.5"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]Collapsible[/color] [color=#79c0ff]defaultOpen[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]Button[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`[color=#7ee787]                        [color=#79c0ff]variant[/color]=[color=#a5d6ff]"ghost"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`[color=#7ee787]                        [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`[color=#7ee787]                        [color=#79c0ff]className[/color]=[color=#a5d6ff]"w-full justify-start border-0 bg-transparent"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`[color=#7ee787]                      >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"folder-open"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]ui[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]Widget[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"ml-5 flex flex-col gap-0.5"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`                        {\\["button.tsx", "card.tsx", "dialog.tsx"].map((name) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]Button[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`[color=#7ee787]                            [color=#79c0ff]key[/color]=[color=#a5d6ff]{name}[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`[color=#7ee787]                            [color=#79c0ff]variant[/color]=[color=#a5d6ff]"link"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`[color=#7ee787]                            [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`[color=#7ee787]                            [color=#79c0ff]className[/color]=[color=#a5d6ff]"w-full justify-start"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`[color=#7ee787]                          >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]{name}[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`                        ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]Collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`                  {\\["login-form.tsx", "register-form.tsx"].map((name) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Button[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`[color=#7ee787]                      [color=#79c0ff]key[/color]=[color=#a5d6ff]{name}[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`[color=#7ee787]                      [color=#79c0ff]variant[/color]=[color=#a5d6ff]"link"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`[color=#7ee787]                      [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`[color=#7ee787]                      [color=#79c0ff]className[/color]=[color=#a5d6ff]"w-full justify-start"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`[color=#7ee787]                    >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">73</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">74</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]{name}[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">75</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">76</Text>
                  <Text className="code-line">{`                  ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">77</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">78</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">79</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]Collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">80</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]Collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">81</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">82</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]Button[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">83</Text>
                  <Text className="code-line">{`[color=#7ee787]                  [color=#79c0ff]variant[/color]=[color=#a5d6ff]"ghost"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">84</Text>
                  <Text className="code-line">{`[color=#7ee787]                  [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">85</Text>
                  <Text className="code-line">{`[color=#7ee787]                  [color=#79c0ff]className[/color]=[color=#a5d6ff]"w-full justify-start border-0 bg-transparent"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">86</Text>
                  <Text className="code-line">{`[color=#7ee787]                >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">87</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">88</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"folder-open"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">89</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]lib[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">90</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">91</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">92</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">93</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]Widget[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"ml-5 flex flex-col gap-0.5"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">94</Text>
                  <Text className="code-line">{`                  {\\["utils.ts", "api.ts"].map((name) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">95</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Button[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">96</Text>
                  <Text className="code-line">{`[color=#7ee787]                      [color=#79c0ff]key[/color]=[color=#a5d6ff]{name}[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">97</Text>
                  <Text className="code-line">{`[color=#7ee787]                      [color=#79c0ff]variant[/color]=[color=#a5d6ff]"link"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">98</Text>
                  <Text className="code-line">{`[color=#7ee787]                      [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">99</Text>
                  <Text className="code-line">{`[color=#7ee787]                      [color=#79c0ff]className[/color]=[color=#a5d6ff]"w-full justify-start"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">100</Text>
                  <Text className="code-line">{`[color=#7ee787]                    >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">101</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">102</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]{name}[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">103</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">104</Text>
                  <Text className="code-line">{`                  ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">105</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">106</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">107</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]Collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">108</Text>
                  <Text className="code-line">{`            {\\["app.tsx", "package.json", "README.md"].map((name) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">109</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]Button[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">110</Text>
                  <Text className="code-line">{`[color=#7ee787]                [color=#79c0ff]key[/color]=[color=#a5d6ff]{name}[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">111</Text>
                  <Text className="code-line">{`[color=#7ee787]                [color=#79c0ff]variant[/color]=[color=#a5d6ff]"link"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">112</Text>
                  <Text className="code-line">{`[color=#7ee787]                [color=#79c0ff]size[/color]=[color=#a5d6ff]"sm"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">113</Text>
                  <Text className="code-line">{`[color=#7ee787]                [color=#79c0ff]className[/color]=[color=#a5d6ff]"w-full justify-start"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">114</Text>
                  <Text className="code-line">{`[color=#7ee787]              >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">115</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">116</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]{name}[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">117</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">118</Text>
                  <Text className="code-line">{`            ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">119</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">120</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]CardContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">121</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]Card[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">122</Text>
                  <Text className="code-line">{`    [color=#7ee787]</[color=#79c0ff]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">123</Text>
                  <Text className="code-line">{`  );`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">124</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/collapsible-file-tree/4" style={{ display: "none" }}>
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
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]collapsible_file_tree_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
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
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]collapsible_file_tree_tsx_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
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
