import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function SidebarBasicDemo() {
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
        <Widget className="code-block-body active" data-language="c" data-file="main.c" data-source="examples/sidebar-basic/0">
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
                  <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_profile_button[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *icon_name,[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`[color=#0550ae]                                          [color=#953800]const[/color] [color=#953800]char[/color] *name, [color=#953800]const[/color] [color=#953800]char[/color] *meta,[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`[color=#0550ae]                                          [color=#953800]const[/color] [color=#953800]char[/color] *extra_class,[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`[color=#0550ae]                                          [color=#953800]const[/color] [color=#953800]char[/color] *logo_class)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *logo = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *icon = ui_create_fluent_icon(icon_name);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *text_box = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *name_text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *meta_text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *chevron = ui_create_fluent_icon([color=#0a3069]"chevron-up-down"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        ui_widget_add_class(button, [color=#0a3069]"sidebar-menu-button"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`        ui_widget_add_class(button, extra_class);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(button, [color=#0a3069]"data-sidebar-tooltip"[/color], name);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`        ui_widget_add_class(logo, logo_class);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`        ui_widget_add_class(text_box, [color=#0a3069]"sidebar-menu-text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`        ui_widget_add_class(name_text, [color=#0a3069]"sidebar-menu-name"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`        ui_widget_add_class(meta_text, [color=#0a3069]"sidebar-menu-meta"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`        ui_widget_add_class(chevron, [color=#0a3069]"sidebar-menu-chevron"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`        ui_text_set_content(name_text, name);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`        ui_text_set_content(meta_text, meta);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`        ui_widget_append(logo, icon);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`        ui_widget_append(text_box, name_text);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`        ui_widget_append(text_box, meta_text);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`        ui_widget_append(button, logo);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`        ui_widget_append(button, text_box);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`        ui_widget_append(button, chevron);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`        [color=#cf222e]return[/color] button;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_menu_item[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *icon_name, [color=#953800]const[/color] [color=#953800]char[/color] *label,[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`[color=#0550ae]                                     [color=#953800]bool[/color] destructive)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *item = ui_create_dropdown_menu_item();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`        ui_widget_append(item, ui_create_fluent_icon(icon_name));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`        ui_text_set_content(text, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`        ui_widget_append(item, text);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`        [color=#cf222e]if[/color] (destructive) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`                ui_dropdown_menu_item_set_variant(`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`                    item, UI_DROPDOWN_MENU_ITEM_DESTRUCTIVE);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`        }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`        [color=#cf222e]return[/color] item;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_team_menu[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *menu = ui_create_dropdown_menu();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button =`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`            create_profile_button([color=#0a3069]"people-team"[/color], [color=#0a3069]"Acme Inc"[/color], [color=#0a3069]"Enterprise"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`                                  [color=#0a3069]"sidebar-team"[/color], [color=#0a3069]"sidebar-team-logo"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_dropdown_menu_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *label = ui_create_dropdown_menu_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`        ui_widget_add_class(menu, [color=#0a3069]"sidebar-team-menu"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`        ui_widget_add_class(content, [color=#0a3069]"sidebar-team-menu-content"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(button, [color=#0a3069]"data-dropdown-menu-trigger"[/color], [color=#0a3069]"true"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(content, [color=#0a3069]"side"[/color], [color=#0a3069]"right"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(content, [color=#0a3069]"align"[/color], [color=#0a3069]"start"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`        ui_text_set_content(label, [color=#0a3069]"Teams"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`        ui_widget_append(content, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`        ui_widget_append(content,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`                         create_menu_item([color=#0a3069]"people-team"[/color], [color=#0a3069]"Acme Inc"[/color], [color=#0550ae]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`        ui_widget_append(content,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`                         create_menu_item([color=#0a3069]"building"[/color], [color=#0a3069]"Acme Corp"[/color], [color=#0550ae]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`        ui_widget_append(content,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`                         create_menu_item([color=#0a3069]"shield"[/color], [color=#0a3069]"Evil Corp"[/color], [color=#0550ae]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">73</Text>
                  <Text className="code-line">{`        ui_widget_append(content, ui_create_dropdown_menu_separator());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">74</Text>
                  <Text className="code-line">{`        ui_widget_append(content, create_menu_item([color=#0a3069]"add"[/color], [color=#0a3069]"Add team"[/color], [color=#0550ae]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">75</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">76</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">77</Text>
                  <Text className="code-line">{`        [color=#cf222e]return[/color] menu;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">78</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">79</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">80</Text>
                  <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_user_menu[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">81</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">82</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *menu = ui_create_dropdown_menu();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">83</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button =`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">84</Text>
                  <Text className="code-line">{`            create_profile_button([color=#0a3069]"person-circle"[/color], [color=#0a3069]"shadcn"[/color], [color=#0a3069]"m@example.com"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">85</Text>
                  <Text className="code-line">{`                                  [color=#0a3069]"sidebar-user"[/color], [color=#0a3069]"sidebar-avatar"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">86</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_dropdown_menu_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">87</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *label = ui_create_dropdown_menu_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">88</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *email = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">89</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">90</Text>
                  <Text className="code-line">{`        ui_widget_add_class(menu, [color=#0a3069]"sidebar-user-menu"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">91</Text>
                  <Text className="code-line">{`        ui_widget_add_class(content, [color=#0a3069]"sidebar-user-menu-content"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">92</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(button, [color=#0a3069]"data-dropdown-menu-trigger"[/color], [color=#0a3069]"true"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">93</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(content, [color=#0a3069]"side"[/color], [color=#0a3069]"right"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">94</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(content, [color=#0a3069]"align"[/color], [color=#0a3069]"end"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">95</Text>
                  <Text className="code-line">{`        ui_text_set_content(label, [color=#0a3069]"shadcn"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">96</Text>
                  <Text className="code-line">{`        ui_widget_append(content, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">97</Text>
                  <Text className="code-line">{`        ui_text_set_content(email, [color=#0a3069]"m@example.com"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">98</Text>
                  <Text className="code-line">{`        ui_widget_add_class(email, [color=#0a3069]"sidebar-user-menu-email"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">99</Text>
                  <Text className="code-line">{`        ui_widget_append(content, email);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">100</Text>
                  <Text className="code-line">{`        ui_widget_append(content, ui_create_dropdown_menu_separator());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">101</Text>
                  <Text className="code-line">{`        ui_widget_append(content,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">102</Text>
                  <Text className="code-line">{`                         create_menu_item([color=#0a3069]"premium"[/color], [color=#0a3069]"Upgrade to Pro"[/color], [color=#0550ae]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">103</Text>
                  <Text className="code-line">{`        ui_widget_append(content, ui_create_dropdown_menu_separator());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">104</Text>
                  <Text className="code-line">{`        ui_widget_append(content, create_menu_item([color=#0a3069]"person"[/color], [color=#0a3069]"Account"[/color], [color=#0550ae]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">105</Text>
                  <Text className="code-line">{`        ui_widget_append(content,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">106</Text>
                  <Text className="code-line">{`                         create_menu_item([color=#0a3069]"payment"[/color], [color=#0a3069]"Billing"[/color], [color=#0550ae]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">107</Text>
                  <Text className="code-line">{`        ui_widget_append(content,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">108</Text>
                  <Text className="code-line">{`                         create_menu_item([color=#0a3069]"alert"[/color], [color=#0a3069]"Notifications"[/color], [color=#0550ae]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">109</Text>
                  <Text className="code-line">{`        ui_widget_append(content, ui_create_dropdown_menu_separator());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">110</Text>
                  <Text className="code-line">{`        ui_widget_append(content,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">111</Text>
                  <Text className="code-line">{`                         create_menu_item([color=#0a3069]"sign-out"[/color], [color=#0a3069]"Sign Out"[/color], [color=#0550ae]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">112</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">113</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">114</Text>
                  <Text className="code-line">{`        [color=#cf222e]return[/color] menu;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">115</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">116</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">117</Text>
                  <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_nav_item[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *icon_name, [color=#953800]const[/color] [color=#953800]char[/color] *title,[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">118</Text>
                  <Text className="code-line">{`[color=#0550ae]                                    [color=#953800]bool[/color] active, [color=#953800]bool[/color] open,[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">119</Text>
                  <Text className="code-line">{`[color=#0550ae]                                    [color=#953800]const[/color] [color=#953800]char[/color] **sub_titles, [color=#953800]int[/color] sub_count)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">120</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">121</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *collapsible = ui_create_collapsible();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">122</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *item = ui_create_sidebar_menu_item();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">123</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *trigger = ui_create_collapsible_trigger();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">124</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">125</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *icon = ui_create_fluent_icon(icon_name);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">126</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *label = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">127</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *chevron = ui_create_fluent_icon([color=#0a3069]"chevron-right"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">128</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_collapsible_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">129</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *sub = ui_create_sidebar_menu_sub();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">130</Text>
                  <Text className="code-line">{`        [color=#953800]int[/color] i;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">131</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">132</Text>
                  <Text className="code-line">{`        ui_widget_add_class(button, [color=#0a3069]"sidebar-menu-button"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">133</Text>
                  <Text className="code-line">{`        [color=#cf222e]if[/color] (active) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">134</Text>
                  <Text className="code-line">{`                ui_widget_add_class(button, [color=#0a3069]"active"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">135</Text>
                  <Text className="code-line">{`        }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">136</Text>
                  <Text className="code-line">{`        ui_widget_add_class(label, [color=#0a3069]"sidebar-menu-label"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">137</Text>
                  <Text className="code-line">{`        ui_widget_add_class(chevron, [color=#0a3069]"sidebar-menu-chevron"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">138</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(button, [color=#0a3069]"data-sidebar-tooltip"[/color], title);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">139</Text>
                  <Text className="code-line">{`        ui_text_set_content(label, title);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">140</Text>
                  <Text className="code-line">{`        ui_widget_append(button, icon);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">141</Text>
                  <Text className="code-line">{`        ui_widget_append(button, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">142</Text>
                  <Text className="code-line">{`        ui_widget_append(button, chevron);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">143</Text>
                  <Text className="code-line">{`        ui_widget_append(trigger, button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">144</Text>
                  <Text className="code-line">{`        ui_widget_append(item, trigger);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">145</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">146</Text>
                  <Text className="code-line">{`        [color=#cf222e]for[/color] (i = [color=#0550ae]0[/color]; i < sub_count; ++i) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">147</Text>
                  <Text className="code-line">{`                [color=#953800]ui_widget_t[/color] *sub_item = ui_create_sidebar_menu_sub_item();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">148</Text>
                  <Text className="code-line">{`                [color=#953800]ui_widget_t[/color] *sub_button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">149</Text>
                  <Text className="code-line">{`                [color=#953800]ui_widget_t[/color] *sub_label = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">150</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">151</Text>
                  <Text className="code-line">{`                ui_widget_add_class(sub_button, [color=#0a3069]"sidebar-menu-button"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">152</Text>
                  <Text className="code-line">{`                ui_widget_add_class(sub_button, [color=#0a3069]"sidebar-sub-button"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">153</Text>
                  <Text className="code-line">{`                ui_widget_add_class(sub_label, [color=#0a3069]"sidebar-menu-label"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">154</Text>
                  <Text className="code-line">{`                ui_text_set_content(sub_label, sub_titles\\[i]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">155</Text>
                  <Text className="code-line">{`                ui_widget_append(sub_button, sub_label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">156</Text>
                  <Text className="code-line">{`                ui_widget_append(sub_item, sub_button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">157</Text>
                  <Text className="code-line">{`                ui_widget_append(sub, sub_item);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">158</Text>
                  <Text className="code-line">{`        }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">159</Text>
                  <Text className="code-line">{`        ui_widget_append(content, sub);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">160</Text>
                  <Text className="code-line">{`        ui_widget_append(item, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">161</Text>
                  <Text className="code-line">{`        ui_widget_append(collapsible, item);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">162</Text>
                  <Text className="code-line">{`        [color=#cf222e]if[/color] (open) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">163</Text>
                  <Text className="code-line">{`                ui_collapsible_set_open(collapsible, [color=#0550ae]true[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">164</Text>
                  <Text className="code-line">{`        }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">165</Text>
                  <Text className="code-line">{`        [color=#cf222e]return[/color] collapsible;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">166</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">167</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">168</Text>
                  <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_project_item[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *icon_name,[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">169</Text>
                  <Text className="code-line">{`[color=#0550ae]                                        [color=#953800]const[/color] [color=#953800]char[/color] *title)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">170</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">171</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *item = ui_create_sidebar_menu_item();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">172</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">173</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *icon = ui_create_fluent_icon(icon_name);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">174</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *label = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">175</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *menu = ui_create_dropdown_menu();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">176</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *action = ui_create_sidebar_menu_action();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">177</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *more = ui_create_fluent_icon([color=#0a3069]"more-horizontal"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">178</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_dropdown_menu_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">179</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">180</Text>
                  <Text className="code-line">{`        ui_widget_add_class(button, [color=#0a3069]"sidebar-menu-button"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">181</Text>
                  <Text className="code-line">{`        ui_widget_add_class(label, [color=#0a3069]"sidebar-menu-label"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">182</Text>
                  <Text className="code-line">{`        ui_text_set_content(label, title);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">183</Text>
                  <Text className="code-line">{`        ui_widget_append(button, icon);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">184</Text>
                  <Text className="code-line">{`        ui_widget_append(button, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">185</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">186</Text>
                  <Text className="code-line">{`        ui_widget_add_class(menu, [color=#0a3069]"sidebar-project-menu"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">187</Text>
                  <Text className="code-line">{`        ui_widget_add_class(action, [color=#0a3069]"sidebar-menu-action-trigger"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">188</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(action, [color=#0a3069]"data-dropdown-menu-trigger"[/color], [color=#0a3069]"true"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">189</Text>
                  <Text className="code-line">{`        ui_widget_append(action, more);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">190</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(content, [color=#0a3069]"side"[/color], [color=#0a3069]"right"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">191</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(content, [color=#0a3069]"align"[/color], [color=#0a3069]"start"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">192</Text>
                  <Text className="code-line">{`        ui_widget_append(content,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">193</Text>
                  <Text className="code-line">{`                         create_menu_item([color=#0a3069]"folder"[/color], [color=#0a3069]"View Project"[/color], [color=#0550ae]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">194</Text>
                  <Text className="code-line">{`        ui_widget_append(`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">195</Text>
                  <Text className="code-line">{`            content, create_menu_item([color=#0a3069]"arrow-right"[/color], [color=#0a3069]"Share Project"[/color], [color=#0550ae]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">196</Text>
                  <Text className="code-line">{`        ui_widget_append(content, ui_create_dropdown_menu_separator());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">197</Text>
                  <Text className="code-line">{`        ui_widget_append(content,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">198</Text>
                  <Text className="code-line">{`                         create_menu_item([color=#0a3069]"delete"[/color], [color=#0a3069]"Delete Project"[/color], [color=#0550ae]true[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">199</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, action);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">200</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">201</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">202</Text>
                  <Text className="code-line">{`        ui_widget_append(item, button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">203</Text>
                  <Text className="code-line">{`        ui_widget_append(item, menu);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">204</Text>
                  <Text className="code-line">{`        [color=#cf222e]return[/color] item;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">205</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">206</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">207</Text>
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]sidebar_basic_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">208</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">209</Text>
                  <Text className="code-line">{`        [color=#953800]const[/color] [color=#953800]char[/color] *playground\\[] = { [color=#0a3069]"History"[/color], [color=#0a3069]"Starred"[/color], [color=#0a3069]"Settings"[/color] };`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">210</Text>
                  <Text className="code-line">{`        [color=#953800]const[/color] [color=#953800]char[/color] *models\\[] = { [color=#0a3069]"Genesis"[/color], [color=#0a3069]"Explorer"[/color], [color=#0a3069]"Quantum"[/color] };`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">211</Text>
                  <Text className="code-line">{`        [color=#953800]const[/color] [color=#953800]char[/color] *documentation\\[] = { [color=#0a3069]"Introduction"[/color], [color=#0a3069]"Get Started"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">212</Text>
                  <Text className="code-line">{`                                        [color=#0a3069]"Tutorials"[/color], [color=#0a3069]"Changelog"[/color] };`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">213</Text>
                  <Text className="code-line">{`        [color=#953800]const[/color] [color=#953800]char[/color] *settings\\[] = { [color=#0a3069]"General"[/color], [color=#0a3069]"Team"[/color], [color=#0a3069]"Billing"[/color], [color=#0a3069]"Limits"[/color] };`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">214</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *provider = ui_create_sidebar_provider();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">215</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *sidebar = ui_create_sidebar();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">216</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *header = ui_create_sidebar_header();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">217</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_sidebar_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">218</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *scroll = ui_create_scrollarea();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">219</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *scroll_content = ui_create_scrollarea_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">220</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *scrollbar = ui_create_scrollbar();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">221</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *group = ui_create_sidebar_group();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">222</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *group_label = ui_create_sidebar_group_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">223</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *group_label_text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">224</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *group_content = ui_create_sidebar_group_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">225</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *menu = ui_create_sidebar_menu();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">226</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *projects = ui_create_sidebar_group();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">227</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *projects_label = ui_create_sidebar_group_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">228</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *projects_label_text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">229</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *projects_content = ui_create_sidebar_group_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">230</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *projects_menu = ui_create_sidebar_menu();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">231</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *footer = ui_create_sidebar_footer();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">232</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *footer_menu = ui_create_sidebar_menu();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">233</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *footer_item = ui_create_sidebar_menu_item();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">234</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *main = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">235</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *trigger = ui_create_sidebar_trigger();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">236</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *rail;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">237</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">238</Text>
                  <Text className="code-line">{`        ui_widget_add_class(provider, [color=#0a3069]"sidebar-basic-demo"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">239</Text>
                  <Text className="code-line">{`        ui_sidebar_set_collapsible(sidebar, UI_SIDEBAR_COLLAPSIBLE_ICON);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">240</Text>
                  <Text className="code-line">{`        ui_widget_add_class(scroll, [color=#0a3069]"sidebar-scroll"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">241</Text>
                  <Text className="code-line">{`        ui_scrollarea_set_wheel_scroll_direction(scroll,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">242</Text>
                  <Text className="code-line">{`                                                 UI_SCROLLAREA_VERTICAL);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">243</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">244</Text>
                  <Text className="code-line">{`        ui_widget_append(header, create_team_menu());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">245</Text>
                  <Text className="code-line">{`        ui_text_set_content(group_label_text, [color=#0a3069]"Platform"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">246</Text>
                  <Text className="code-line">{`        ui_widget_append(group_label, group_label_text);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">247</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, create_nav_item([color=#0a3069]"code-block"[/color], [color=#0a3069]"Playground"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">248</Text>
                  <Text className="code-line">{`                                               [color=#0550ae]false[/color], [color=#0550ae]true[/color], playground, [color=#0550ae]3[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">249</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, create_nav_item([color=#0a3069]"document-text"[/color], [color=#0a3069]"Models"[/color], [color=#0550ae]false[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">250</Text>
                  <Text className="code-line">{`                                               [color=#0550ae]false[/color], models, [color=#0550ae]3[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">251</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, create_nav_item([color=#0a3069]"book-open"[/color], [color=#0a3069]"Documentation"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">252</Text>
                  <Text className="code-line">{`                                               [color=#0550ae]false[/color], [color=#0550ae]false[/color], documentation, [color=#0550ae]4[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">253</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, create_nav_item([color=#0a3069]"calendar-settings"[/color], [color=#0a3069]"Settings"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">254</Text>
                  <Text className="code-line">{`                                               [color=#0550ae]false[/color], [color=#0550ae]false[/color], settings, [color=#0550ae]4[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">255</Text>
                  <Text className="code-line">{`        ui_widget_append(group_content, menu);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">256</Text>
                  <Text className="code-line">{`        ui_widget_append(group, group_label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">257</Text>
                  <Text className="code-line">{`        ui_widget_append(group, group_content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">258</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">259</Text>
                  <Text className="code-line">{`        ui_widget_add_class(projects, [color=#0a3069]"sidebar-projects"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">260</Text>
                  <Text className="code-line">{`        ui_text_set_content(projects_label_text, [color=#0a3069]"Projects"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">261</Text>
                  <Text className="code-line">{`        ui_widget_append(projects_label, projects_label_text);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">262</Text>
                  <Text className="code-line">{`        ui_widget_append(`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">263</Text>
                  <Text className="code-line">{`            projects_menu,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">264</Text>
                  <Text className="code-line">{`            create_project_item([color=#0a3069]"code-block"[/color], [color=#0a3069]"Design Engineering"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">265</Text>
                  <Text className="code-line">{`        ui_widget_append(projects_menu,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">266</Text>
                  <Text className="code-line">{`                         create_project_item([color=#0a3069]"book-open"[/color], [color=#0a3069]"Sales & Marketing"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">267</Text>
                  <Text className="code-line">{`        ui_widget_append(projects_menu,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">268</Text>
                  <Text className="code-line">{`                         create_project_item([color=#0a3069]"document-text"[/color], [color=#0a3069]"Travel"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">269</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">270</Text>
                  <Text className="code-line">{`        {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">271</Text>
                  <Text className="code-line">{`                [color=#953800]ui_widget_t[/color] *more_item = ui_create_sidebar_menu_item();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">272</Text>
                  <Text className="code-line">{`                [color=#953800]ui_widget_t[/color] *more_button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">273</Text>
                  <Text className="code-line">{`                [color=#953800]ui_widget_t[/color] *more_icon =`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">274</Text>
                  <Text className="code-line">{`                    ui_create_fluent_icon([color=#0a3069]"more-horizontal"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">275</Text>
                  <Text className="code-line">{`                [color=#953800]ui_widget_t[/color] *more_label = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">276</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">277</Text>
                  <Text className="code-line">{`                ui_widget_add_class(more_button, [color=#0a3069]"sidebar-menu-button"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">278</Text>
                  <Text className="code-line">{`                ui_widget_add_class(more_button, [color=#0a3069]"sidebar-more"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">279</Text>
                  <Text className="code-line">{`                ui_widget_add_class(more_label, [color=#0a3069]"sidebar-menu-label"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">280</Text>
                  <Text className="code-line">{`                ui_text_set_content(more_label, [color=#0a3069]"More"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">281</Text>
                  <Text className="code-line">{`                ui_widget_append(more_button, more_icon);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">282</Text>
                  <Text className="code-line">{`                ui_widget_append(more_button, more_label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">283</Text>
                  <Text className="code-line">{`                ui_widget_append(more_item, more_button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">284</Text>
                  <Text className="code-line">{`                ui_widget_append(projects_menu, more_item);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">285</Text>
                  <Text className="code-line">{`        }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">286</Text>
                  <Text className="code-line">{`        ui_widget_append(projects_content, projects_menu);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">287</Text>
                  <Text className="code-line">{`        ui_widget_append(projects, projects_label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">288</Text>
                  <Text className="code-line">{`        ui_widget_append(projects, projects_content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">289</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">290</Text>
                  <Text className="code-line">{`        ui_widget_append(scroll_content, group);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">291</Text>
                  <Text className="code-line">{`        ui_widget_append(scroll_content, projects);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">292</Text>
                  <Text className="code-line">{`        ui_widget_append(scroll, scroll_content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">293</Text>
                  <Text className="code-line">{`        ui_widget_append(scroll, scrollbar);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">294</Text>
                  <Text className="code-line">{`        ui_widget_append(content, scroll);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">295</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">296</Text>
                  <Text className="code-line">{`        ui_widget_append(footer_item, create_user_menu());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">297</Text>
                  <Text className="code-line">{`        ui_widget_append(footer_menu, footer_item);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">298</Text>
                  <Text className="code-line">{`        ui_widget_append(footer, footer_menu);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">299</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">300</Text>
                  <Text className="code-line">{`        ui_widget_add_class(main, [color=#0a3069]"sidebar-main"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">301</Text>
                  <Text className="code-line">{`        ui_widget_append(main, trigger);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">302</Text>
                  <Text className="code-line">{`        ui_widget_append(sidebar, header);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">303</Text>
                  <Text className="code-line">{`        ui_widget_append(sidebar, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">304</Text>
                  <Text className="code-line">{`        ui_widget_append(sidebar, footer);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">305</Text>
                  <Text className="code-line">{`        rail = ui_create_sidebar_rail();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">306</Text>
                  <Text className="code-line">{`        ui_widget_append(sidebar, rail);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">307</Text>
                  <Text className="code-line">{`        ui_widget_append(provider, sidebar);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">308</Text>
                  <Text className="code-line">{`        ui_widget_append(provider, main);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">309</Text>
                  <Text className="code-line">{`        ui_widget_append(parent, provider);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">310</Text>
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
                  <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]ui_widget_t[/color] *[color=#d2a8ff]create_profile_button[/color][color=#79c0ff]([color=#ffa657]const[/color] [color=#ffa657]char[/color] *icon_name,[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`[color=#79c0ff]                                          [color=#ffa657]const[/color] [color=#ffa657]char[/color] *name, [color=#ffa657]const[/color] [color=#ffa657]char[/color] *meta,[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`[color=#79c0ff]                                          [color=#ffa657]const[/color] [color=#ffa657]char[/color] *extra_class,[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`[color=#79c0ff]                                          [color=#ffa657]const[/color] [color=#ffa657]char[/color] *logo_class)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *logo = ui_create_widget([color=#79c0ff]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *icon = ui_create_fluent_icon(icon_name);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *text_box = ui_create_widget([color=#79c0ff]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *name_text = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *meta_text = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *chevron = ui_create_fluent_icon([color=#a5d6ff]"chevron-up-down"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        ui_widget_add_class(button, [color=#a5d6ff]"sidebar-menu-button"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`        ui_widget_add_class(button, extra_class);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(button, [color=#a5d6ff]"data-sidebar-tooltip"[/color], name);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`        ui_widget_add_class(logo, logo_class);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`        ui_widget_add_class(text_box, [color=#a5d6ff]"sidebar-menu-text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`        ui_widget_add_class(name_text, [color=#a5d6ff]"sidebar-menu-name"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`        ui_widget_add_class(meta_text, [color=#a5d6ff]"sidebar-menu-meta"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`        ui_widget_add_class(chevron, [color=#a5d6ff]"sidebar-menu-chevron"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`        ui_text_set_content(name_text, name);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`        ui_text_set_content(meta_text, meta);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`        ui_widget_append(logo, icon);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`        ui_widget_append(text_box, name_text);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`        ui_widget_append(text_box, meta_text);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`        ui_widget_append(button, logo);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`        ui_widget_append(button, text_box);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`        ui_widget_append(button, chevron);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`        [color=#ff7b72]return[/color] button;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]ui_widget_t[/color] *[color=#d2a8ff]create_menu_item[/color][color=#79c0ff]([color=#ffa657]const[/color] [color=#ffa657]char[/color] *icon_name, [color=#ffa657]const[/color] [color=#ffa657]char[/color] *label,[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`[color=#79c0ff]                                     [color=#ffa657]bool[/color] destructive)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *item = ui_create_dropdown_menu_item();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *text = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`        ui_widget_append(item, ui_create_fluent_icon(icon_name));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`        ui_text_set_content(text, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`        ui_widget_append(item, text);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`        [color=#ff7b72]if[/color] (destructive) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`                ui_dropdown_menu_item_set_variant(`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`                    item, UI_DROPDOWN_MENU_ITEM_DESTRUCTIVE);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`        }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`        [color=#ff7b72]return[/color] item;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]ui_widget_t[/color] *[color=#d2a8ff]create_team_menu[/color][color=#79c0ff]([color=#ffa657]void[/color])[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *menu = ui_create_dropdown_menu();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *button =`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`            create_profile_button([color=#a5d6ff]"people-team"[/color], [color=#a5d6ff]"Acme Inc"[/color], [color=#a5d6ff]"Enterprise"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`                                  [color=#a5d6ff]"sidebar-team"[/color], [color=#a5d6ff]"sidebar-team-logo"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *content = ui_create_dropdown_menu_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *label = ui_create_dropdown_menu_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`        ui_widget_add_class(menu, [color=#a5d6ff]"sidebar-team-menu"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`        ui_widget_add_class(content, [color=#a5d6ff]"sidebar-team-menu-content"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(button, [color=#a5d6ff]"data-dropdown-menu-trigger"[/color], [color=#a5d6ff]"true"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(content, [color=#a5d6ff]"side"[/color], [color=#a5d6ff]"right"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(content, [color=#a5d6ff]"align"[/color], [color=#a5d6ff]"start"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`        ui_text_set_content(label, [color=#a5d6ff]"Teams"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`        ui_widget_append(content, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`        ui_widget_append(content,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`                         create_menu_item([color=#a5d6ff]"people-team"[/color], [color=#a5d6ff]"Acme Inc"[/color], [color=#79c0ff]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`        ui_widget_append(content,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`                         create_menu_item([color=#a5d6ff]"building"[/color], [color=#a5d6ff]"Acme Corp"[/color], [color=#79c0ff]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`        ui_widget_append(content,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`                         create_menu_item([color=#a5d6ff]"shield"[/color], [color=#a5d6ff]"Evil Corp"[/color], [color=#79c0ff]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">73</Text>
                  <Text className="code-line">{`        ui_widget_append(content, ui_create_dropdown_menu_separator());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">74</Text>
                  <Text className="code-line">{`        ui_widget_append(content, create_menu_item([color=#a5d6ff]"add"[/color], [color=#a5d6ff]"Add team"[/color], [color=#79c0ff]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">75</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">76</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">77</Text>
                  <Text className="code-line">{`        [color=#ff7b72]return[/color] menu;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">78</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">79</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">80</Text>
                  <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]ui_widget_t[/color] *[color=#d2a8ff]create_user_menu[/color][color=#79c0ff]([color=#ffa657]void[/color])[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">81</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">82</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *menu = ui_create_dropdown_menu();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">83</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *button =`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">84</Text>
                  <Text className="code-line">{`            create_profile_button([color=#a5d6ff]"person-circle"[/color], [color=#a5d6ff]"shadcn"[/color], [color=#a5d6ff]"m@example.com"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">85</Text>
                  <Text className="code-line">{`                                  [color=#a5d6ff]"sidebar-user"[/color], [color=#a5d6ff]"sidebar-avatar"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">86</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *content = ui_create_dropdown_menu_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">87</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *label = ui_create_dropdown_menu_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">88</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *email = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">89</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">90</Text>
                  <Text className="code-line">{`        ui_widget_add_class(menu, [color=#a5d6ff]"sidebar-user-menu"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">91</Text>
                  <Text className="code-line">{`        ui_widget_add_class(content, [color=#a5d6ff]"sidebar-user-menu-content"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">92</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(button, [color=#a5d6ff]"data-dropdown-menu-trigger"[/color], [color=#a5d6ff]"true"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">93</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(content, [color=#a5d6ff]"side"[/color], [color=#a5d6ff]"right"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">94</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(content, [color=#a5d6ff]"align"[/color], [color=#a5d6ff]"end"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">95</Text>
                  <Text className="code-line">{`        ui_text_set_content(label, [color=#a5d6ff]"shadcn"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">96</Text>
                  <Text className="code-line">{`        ui_widget_append(content, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">97</Text>
                  <Text className="code-line">{`        ui_text_set_content(email, [color=#a5d6ff]"m@example.com"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">98</Text>
                  <Text className="code-line">{`        ui_widget_add_class(email, [color=#a5d6ff]"sidebar-user-menu-email"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">99</Text>
                  <Text className="code-line">{`        ui_widget_append(content, email);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">100</Text>
                  <Text className="code-line">{`        ui_widget_append(content, ui_create_dropdown_menu_separator());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">101</Text>
                  <Text className="code-line">{`        ui_widget_append(content,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">102</Text>
                  <Text className="code-line">{`                         create_menu_item([color=#a5d6ff]"premium"[/color], [color=#a5d6ff]"Upgrade to Pro"[/color], [color=#79c0ff]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">103</Text>
                  <Text className="code-line">{`        ui_widget_append(content, ui_create_dropdown_menu_separator());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">104</Text>
                  <Text className="code-line">{`        ui_widget_append(content, create_menu_item([color=#a5d6ff]"person"[/color], [color=#a5d6ff]"Account"[/color], [color=#79c0ff]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">105</Text>
                  <Text className="code-line">{`        ui_widget_append(content,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">106</Text>
                  <Text className="code-line">{`                         create_menu_item([color=#a5d6ff]"payment"[/color], [color=#a5d6ff]"Billing"[/color], [color=#79c0ff]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">107</Text>
                  <Text className="code-line">{`        ui_widget_append(content,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">108</Text>
                  <Text className="code-line">{`                         create_menu_item([color=#a5d6ff]"alert"[/color], [color=#a5d6ff]"Notifications"[/color], [color=#79c0ff]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">109</Text>
                  <Text className="code-line">{`        ui_widget_append(content, ui_create_dropdown_menu_separator());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">110</Text>
                  <Text className="code-line">{`        ui_widget_append(content,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">111</Text>
                  <Text className="code-line">{`                         create_menu_item([color=#a5d6ff]"sign-out"[/color], [color=#a5d6ff]"Sign Out"[/color], [color=#79c0ff]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">112</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">113</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">114</Text>
                  <Text className="code-line">{`        [color=#ff7b72]return[/color] menu;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">115</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">116</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">117</Text>
                  <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]ui_widget_t[/color] *[color=#d2a8ff]create_nav_item[/color][color=#79c0ff]([color=#ffa657]const[/color] [color=#ffa657]char[/color] *icon_name, [color=#ffa657]const[/color] [color=#ffa657]char[/color] *title,[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">118</Text>
                  <Text className="code-line">{`[color=#79c0ff]                                    [color=#ffa657]bool[/color] active, [color=#ffa657]bool[/color] open,[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">119</Text>
                  <Text className="code-line">{`[color=#79c0ff]                                    [color=#ffa657]const[/color] [color=#ffa657]char[/color] **sub_titles, [color=#ffa657]int[/color] sub_count)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">120</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">121</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *collapsible = ui_create_collapsible();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">122</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *item = ui_create_sidebar_menu_item();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">123</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *trigger = ui_create_collapsible_trigger();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">124</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">125</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *icon = ui_create_fluent_icon(icon_name);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">126</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *label = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">127</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *chevron = ui_create_fluent_icon([color=#a5d6ff]"chevron-right"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">128</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *content = ui_create_collapsible_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">129</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *sub = ui_create_sidebar_menu_sub();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">130</Text>
                  <Text className="code-line">{`        [color=#ffa657]int[/color] i;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">131</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">132</Text>
                  <Text className="code-line">{`        ui_widget_add_class(button, [color=#a5d6ff]"sidebar-menu-button"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">133</Text>
                  <Text className="code-line">{`        [color=#ff7b72]if[/color] (active) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">134</Text>
                  <Text className="code-line">{`                ui_widget_add_class(button, [color=#a5d6ff]"active"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">135</Text>
                  <Text className="code-line">{`        }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">136</Text>
                  <Text className="code-line">{`        ui_widget_add_class(label, [color=#a5d6ff]"sidebar-menu-label"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">137</Text>
                  <Text className="code-line">{`        ui_widget_add_class(chevron, [color=#a5d6ff]"sidebar-menu-chevron"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">138</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(button, [color=#a5d6ff]"data-sidebar-tooltip"[/color], title);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">139</Text>
                  <Text className="code-line">{`        ui_text_set_content(label, title);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">140</Text>
                  <Text className="code-line">{`        ui_widget_append(button, icon);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">141</Text>
                  <Text className="code-line">{`        ui_widget_append(button, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">142</Text>
                  <Text className="code-line">{`        ui_widget_append(button, chevron);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">143</Text>
                  <Text className="code-line">{`        ui_widget_append(trigger, button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">144</Text>
                  <Text className="code-line">{`        ui_widget_append(item, trigger);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">145</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">146</Text>
                  <Text className="code-line">{`        [color=#ff7b72]for[/color] (i = [color=#79c0ff]0[/color]; i < sub_count; ++i) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">147</Text>
                  <Text className="code-line">{`                [color=#ffa657]ui_widget_t[/color] *sub_item = ui_create_sidebar_menu_sub_item();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">148</Text>
                  <Text className="code-line">{`                [color=#ffa657]ui_widget_t[/color] *sub_button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">149</Text>
                  <Text className="code-line">{`                [color=#ffa657]ui_widget_t[/color] *sub_label = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">150</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">151</Text>
                  <Text className="code-line">{`                ui_widget_add_class(sub_button, [color=#a5d6ff]"sidebar-menu-button"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">152</Text>
                  <Text className="code-line">{`                ui_widget_add_class(sub_button, [color=#a5d6ff]"sidebar-sub-button"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">153</Text>
                  <Text className="code-line">{`                ui_widget_add_class(sub_label, [color=#a5d6ff]"sidebar-menu-label"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">154</Text>
                  <Text className="code-line">{`                ui_text_set_content(sub_label, sub_titles\\[i]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">155</Text>
                  <Text className="code-line">{`                ui_widget_append(sub_button, sub_label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">156</Text>
                  <Text className="code-line">{`                ui_widget_append(sub_item, sub_button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">157</Text>
                  <Text className="code-line">{`                ui_widget_append(sub, sub_item);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">158</Text>
                  <Text className="code-line">{`        }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">159</Text>
                  <Text className="code-line">{`        ui_widget_append(content, sub);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">160</Text>
                  <Text className="code-line">{`        ui_widget_append(item, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">161</Text>
                  <Text className="code-line">{`        ui_widget_append(collapsible, item);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">162</Text>
                  <Text className="code-line">{`        [color=#ff7b72]if[/color] (open) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">163</Text>
                  <Text className="code-line">{`                ui_collapsible_set_open(collapsible, [color=#79c0ff]true[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">164</Text>
                  <Text className="code-line">{`        }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">165</Text>
                  <Text className="code-line">{`        [color=#ff7b72]return[/color] collapsible;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">166</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">167</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">168</Text>
                  <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]ui_widget_t[/color] *[color=#d2a8ff]create_project_item[/color][color=#79c0ff]([color=#ffa657]const[/color] [color=#ffa657]char[/color] *icon_name,[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">169</Text>
                  <Text className="code-line">{`[color=#79c0ff]                                        [color=#ffa657]const[/color] [color=#ffa657]char[/color] *title)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">170</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">171</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *item = ui_create_sidebar_menu_item();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">172</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">173</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *icon = ui_create_fluent_icon(icon_name);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">174</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *label = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">175</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *menu = ui_create_dropdown_menu();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">176</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *action = ui_create_sidebar_menu_action();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">177</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *more = ui_create_fluent_icon([color=#a5d6ff]"more-horizontal"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">178</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *content = ui_create_dropdown_menu_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">179</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">180</Text>
                  <Text className="code-line">{`        ui_widget_add_class(button, [color=#a5d6ff]"sidebar-menu-button"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">181</Text>
                  <Text className="code-line">{`        ui_widget_add_class(label, [color=#a5d6ff]"sidebar-menu-label"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">182</Text>
                  <Text className="code-line">{`        ui_text_set_content(label, title);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">183</Text>
                  <Text className="code-line">{`        ui_widget_append(button, icon);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">184</Text>
                  <Text className="code-line">{`        ui_widget_append(button, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">185</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">186</Text>
                  <Text className="code-line">{`        ui_widget_add_class(menu, [color=#a5d6ff]"sidebar-project-menu"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">187</Text>
                  <Text className="code-line">{`        ui_widget_add_class(action, [color=#a5d6ff]"sidebar-menu-action-trigger"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">188</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(action, [color=#a5d6ff]"data-dropdown-menu-trigger"[/color], [color=#a5d6ff]"true"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">189</Text>
                  <Text className="code-line">{`        ui_widget_append(action, more);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">190</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(content, [color=#a5d6ff]"side"[/color], [color=#a5d6ff]"right"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">191</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(content, [color=#a5d6ff]"align"[/color], [color=#a5d6ff]"start"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">192</Text>
                  <Text className="code-line">{`        ui_widget_append(content,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">193</Text>
                  <Text className="code-line">{`                         create_menu_item([color=#a5d6ff]"folder"[/color], [color=#a5d6ff]"View Project"[/color], [color=#79c0ff]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">194</Text>
                  <Text className="code-line">{`        ui_widget_append(`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">195</Text>
                  <Text className="code-line">{`            content, create_menu_item([color=#a5d6ff]"arrow-right"[/color], [color=#a5d6ff]"Share Project"[/color], [color=#79c0ff]false[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">196</Text>
                  <Text className="code-line">{`        ui_widget_append(content, ui_create_dropdown_menu_separator());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">197</Text>
                  <Text className="code-line">{`        ui_widget_append(content,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">198</Text>
                  <Text className="code-line">{`                         create_menu_item([color=#a5d6ff]"delete"[/color], [color=#a5d6ff]"Delete Project"[/color], [color=#79c0ff]true[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">199</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, action);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">200</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">201</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">202</Text>
                  <Text className="code-line">{`        ui_widget_append(item, button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">203</Text>
                  <Text className="code-line">{`        ui_widget_append(item, menu);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">204</Text>
                  <Text className="code-line">{`        [color=#ff7b72]return[/color] item;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">205</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">206</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">207</Text>
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]sidebar_basic_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">208</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">209</Text>
                  <Text className="code-line">{`        [color=#ffa657]const[/color] [color=#ffa657]char[/color] *playground\\[] = { [color=#a5d6ff]"History"[/color], [color=#a5d6ff]"Starred"[/color], [color=#a5d6ff]"Settings"[/color] };`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">210</Text>
                  <Text className="code-line">{`        [color=#ffa657]const[/color] [color=#ffa657]char[/color] *models\\[] = { [color=#a5d6ff]"Genesis"[/color], [color=#a5d6ff]"Explorer"[/color], [color=#a5d6ff]"Quantum"[/color] };`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">211</Text>
                  <Text className="code-line">{`        [color=#ffa657]const[/color] [color=#ffa657]char[/color] *documentation\\[] = { [color=#a5d6ff]"Introduction"[/color], [color=#a5d6ff]"Get Started"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">212</Text>
                  <Text className="code-line">{`                                        [color=#a5d6ff]"Tutorials"[/color], [color=#a5d6ff]"Changelog"[/color] };`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">213</Text>
                  <Text className="code-line">{`        [color=#ffa657]const[/color] [color=#ffa657]char[/color] *settings\\[] = { [color=#a5d6ff]"General"[/color], [color=#a5d6ff]"Team"[/color], [color=#a5d6ff]"Billing"[/color], [color=#a5d6ff]"Limits"[/color] };`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">214</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *provider = ui_create_sidebar_provider();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">215</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *sidebar = ui_create_sidebar();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">216</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *header = ui_create_sidebar_header();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">217</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *content = ui_create_sidebar_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">218</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *scroll = ui_create_scrollarea();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">219</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *scroll_content = ui_create_scrollarea_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">220</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *scrollbar = ui_create_scrollbar();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">221</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *group = ui_create_sidebar_group();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">222</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *group_label = ui_create_sidebar_group_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">223</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *group_label_text = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">224</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *group_content = ui_create_sidebar_group_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">225</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *menu = ui_create_sidebar_menu();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">226</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *projects = ui_create_sidebar_group();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">227</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *projects_label = ui_create_sidebar_group_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">228</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *projects_label_text = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">229</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *projects_content = ui_create_sidebar_group_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">230</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *projects_menu = ui_create_sidebar_menu();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">231</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *footer = ui_create_sidebar_footer();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">232</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *footer_menu = ui_create_sidebar_menu();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">233</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *footer_item = ui_create_sidebar_menu_item();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">234</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *main = ui_create_widget([color=#79c0ff]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">235</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *trigger = ui_create_sidebar_trigger();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">236</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *rail;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">237</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">238</Text>
                  <Text className="code-line">{`        ui_widget_add_class(provider, [color=#a5d6ff]"sidebar-basic-demo"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">239</Text>
                  <Text className="code-line">{`        ui_sidebar_set_collapsible(sidebar, UI_SIDEBAR_COLLAPSIBLE_ICON);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">240</Text>
                  <Text className="code-line">{`        ui_widget_add_class(scroll, [color=#a5d6ff]"sidebar-scroll"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">241</Text>
                  <Text className="code-line">{`        ui_scrollarea_set_wheel_scroll_direction(scroll,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">242</Text>
                  <Text className="code-line">{`                                                 UI_SCROLLAREA_VERTICAL);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">243</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">244</Text>
                  <Text className="code-line">{`        ui_widget_append(header, create_team_menu());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">245</Text>
                  <Text className="code-line">{`        ui_text_set_content(group_label_text, [color=#a5d6ff]"Platform"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">246</Text>
                  <Text className="code-line">{`        ui_widget_append(group_label, group_label_text);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">247</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, create_nav_item([color=#a5d6ff]"code-block"[/color], [color=#a5d6ff]"Playground"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">248</Text>
                  <Text className="code-line">{`                                               [color=#79c0ff]false[/color], [color=#79c0ff]true[/color], playground, [color=#79c0ff]3[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">249</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, create_nav_item([color=#a5d6ff]"document-text"[/color], [color=#a5d6ff]"Models"[/color], [color=#79c0ff]false[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">250</Text>
                  <Text className="code-line">{`                                               [color=#79c0ff]false[/color], models, [color=#79c0ff]3[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">251</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, create_nav_item([color=#a5d6ff]"book-open"[/color], [color=#a5d6ff]"Documentation"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">252</Text>
                  <Text className="code-line">{`                                               [color=#79c0ff]false[/color], [color=#79c0ff]false[/color], documentation, [color=#79c0ff]4[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">253</Text>
                  <Text className="code-line">{`        ui_widget_append(menu, create_nav_item([color=#a5d6ff]"calendar-settings"[/color], [color=#a5d6ff]"Settings"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">254</Text>
                  <Text className="code-line">{`                                               [color=#79c0ff]false[/color], [color=#79c0ff]false[/color], settings, [color=#79c0ff]4[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">255</Text>
                  <Text className="code-line">{`        ui_widget_append(group_content, menu);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">256</Text>
                  <Text className="code-line">{`        ui_widget_append(group, group_label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">257</Text>
                  <Text className="code-line">{`        ui_widget_append(group, group_content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">258</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">259</Text>
                  <Text className="code-line">{`        ui_widget_add_class(projects, [color=#a5d6ff]"sidebar-projects"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">260</Text>
                  <Text className="code-line">{`        ui_text_set_content(projects_label_text, [color=#a5d6ff]"Projects"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">261</Text>
                  <Text className="code-line">{`        ui_widget_append(projects_label, projects_label_text);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">262</Text>
                  <Text className="code-line">{`        ui_widget_append(`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">263</Text>
                  <Text className="code-line">{`            projects_menu,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">264</Text>
                  <Text className="code-line">{`            create_project_item([color=#a5d6ff]"code-block"[/color], [color=#a5d6ff]"Design Engineering"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">265</Text>
                  <Text className="code-line">{`        ui_widget_append(projects_menu,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">266</Text>
                  <Text className="code-line">{`                         create_project_item([color=#a5d6ff]"book-open"[/color], [color=#a5d6ff]"Sales & Marketing"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">267</Text>
                  <Text className="code-line">{`        ui_widget_append(projects_menu,`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">268</Text>
                  <Text className="code-line">{`                         create_project_item([color=#a5d6ff]"document-text"[/color], [color=#a5d6ff]"Travel"[/color]));`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">269</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">270</Text>
                  <Text className="code-line">{`        {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">271</Text>
                  <Text className="code-line">{`                [color=#ffa657]ui_widget_t[/color] *more_item = ui_create_sidebar_menu_item();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">272</Text>
                  <Text className="code-line">{`                [color=#ffa657]ui_widget_t[/color] *more_button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">273</Text>
                  <Text className="code-line">{`                [color=#ffa657]ui_widget_t[/color] *more_icon =`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">274</Text>
                  <Text className="code-line">{`                    ui_create_fluent_icon([color=#a5d6ff]"more-horizontal"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">275</Text>
                  <Text className="code-line">{`                [color=#ffa657]ui_widget_t[/color] *more_label = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">276</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">277</Text>
                  <Text className="code-line">{`                ui_widget_add_class(more_button, [color=#a5d6ff]"sidebar-menu-button"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">278</Text>
                  <Text className="code-line">{`                ui_widget_add_class(more_button, [color=#a5d6ff]"sidebar-more"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">279</Text>
                  <Text className="code-line">{`                ui_widget_add_class(more_label, [color=#a5d6ff]"sidebar-menu-label"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">280</Text>
                  <Text className="code-line">{`                ui_text_set_content(more_label, [color=#a5d6ff]"More"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">281</Text>
                  <Text className="code-line">{`                ui_widget_append(more_button, more_icon);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">282</Text>
                  <Text className="code-line">{`                ui_widget_append(more_button, more_label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">283</Text>
                  <Text className="code-line">{`                ui_widget_append(more_item, more_button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">284</Text>
                  <Text className="code-line">{`                ui_widget_append(projects_menu, more_item);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">285</Text>
                  <Text className="code-line">{`        }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">286</Text>
                  <Text className="code-line">{`        ui_widget_append(projects_content, projects_menu);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">287</Text>
                  <Text className="code-line">{`        ui_widget_append(projects, projects_label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">288</Text>
                  <Text className="code-line">{`        ui_widget_append(projects, projects_content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">289</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">290</Text>
                  <Text className="code-line">{`        ui_widget_append(scroll_content, group);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">291</Text>
                  <Text className="code-line">{`        ui_widget_append(scroll_content, projects);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">292</Text>
                  <Text className="code-line">{`        ui_widget_append(scroll, scroll_content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">293</Text>
                  <Text className="code-line">{`        ui_widget_append(scroll, scrollbar);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">294</Text>
                  <Text className="code-line">{`        ui_widget_append(content, scroll);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">295</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">296</Text>
                  <Text className="code-line">{`        ui_widget_append(footer_item, create_user_menu());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">297</Text>
                  <Text className="code-line">{`        ui_widget_append(footer_menu, footer_item);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">298</Text>
                  <Text className="code-line">{`        ui_widget_append(footer, footer_menu);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">299</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">300</Text>
                  <Text className="code-line">{`        ui_widget_add_class(main, [color=#a5d6ff]"sidebar-main"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">301</Text>
                  <Text className="code-line">{`        ui_widget_append(main, trigger);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">302</Text>
                  <Text className="code-line">{`        ui_widget_append(sidebar, header);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">303</Text>
                  <Text className="code-line">{`        ui_widget_append(sidebar, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">304</Text>
                  <Text className="code-line">{`        ui_widget_append(sidebar, footer);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">305</Text>
                  <Text className="code-line">{`        rail = ui_create_sidebar_rail();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">306</Text>
                  <Text className="code-line">{`        ui_widget_append(sidebar, rail);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">307</Text>
                  <Text className="code-line">{`        ui_widget_append(provider, sidebar);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">308</Text>
                  <Text className="code-line">{`        ui_widget_append(provider, main);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">309</Text>
                  <Text className="code-line">{`        ui_widget_append(parent, provider);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">310</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/sidebar-basic/1" style={{ display: "none" }}>
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
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]sidebar-provider[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-basic-demo"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]sidebar[/color] [color=#0550ae]collapsible[/color]=[color=#0a3069]"icon"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]sidebar-header[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]dropdown-menu[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-team-menu"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button sidebar-team"[/color] [color=#0550ae]data-sidebar-tooltip[/color]=[color=#0a3069]"Acme Inc"[/color] [color=#0550ae]data-dropdown-menu-trigger[/color]=[color=#0a3069]"true"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-team-logo"[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"people-team"[/color] />[/color][color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-text"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-name"[/color]>[/color]Acme Inc[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-meta"[/color]>[/color]Enterprise[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-chevron"[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-up-down"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]dropdown-menu-content[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-team-menu-content"[/color] [color=#0550ae]side[/color]=[color=#0a3069]"right"[/color] [color=#0550ae]align[/color]=[color=#0a3069]"start"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dropdown-menu-label[/color]>[/color]Teams[color=#116329]</[color=#0550ae]dropdown-menu-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"people-team"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color]>[/color]Acme Inc[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"building"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color]>[/color]Acme Corp[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"shield"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color]>[/color]Evil Corp[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dropdown-menu-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"add"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]text[/color]>[/color]Add team[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]dropdown-menu-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]dropdown-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]sidebar-header[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]sidebar-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]scrollarea[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-scroll"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]scrollarea-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]sidebar-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]sidebar-group-label[/color]>[/color]Platform[color=#116329]</[color=#0550ae]sidebar-group-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]sidebar-group-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]sidebar-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]collapsible[/color] [color=#0550ae]default-open[/color]=[color=#0a3069]"true"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button"[/color] [color=#0550ae]data-sidebar-tooltip[/color]=[color=#0a3069]"Playground"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"code-block"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Playground[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-chevron"[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]sidebar-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]History[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Starred[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Settings[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]sidebar-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">73</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">74</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">75</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">76</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">77</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">78</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button"[/color] [color=#0550ae]data-sidebar-tooltip[/color]=[color=#0a3069]"Models"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">79</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">80</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Models[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">81</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-chevron"[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">82</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">83</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">84</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">85</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]sidebar-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">86</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">87</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">88</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Genesis[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">89</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">90</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">91</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">92</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">93</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Explorer[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">94</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">95</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">96</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">97</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">98</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Quantum[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">99</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">100</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">101</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]sidebar-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">102</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">103</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">104</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">105</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">106</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">107</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">108</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button"[/color] [color=#0550ae]data-sidebar-tooltip[/color]=[color=#0a3069]"Documentation"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">109</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"book-open"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">110</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Documentation[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">111</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-chevron"[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">112</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">113</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">114</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">115</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]sidebar-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">116</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">117</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">118</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Introduction[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">119</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">120</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">121</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">122</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">123</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Get Started[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">124</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">125</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">126</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">127</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">128</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Tutorials[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">129</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">130</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">131</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">132</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">133</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Changelog[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">134</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">135</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">136</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]sidebar-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">137</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">138</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">139</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">140</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">141</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">142</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">143</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button"[/color] [color=#0550ae]data-sidebar-tooltip[/color]=[color=#0a3069]"Settings"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">144</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"calendar-settings"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">145</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Settings[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">146</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-chevron"[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">147</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">148</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">149</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">150</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]sidebar-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">151</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">152</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">153</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]General[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">154</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">155</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">156</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">157</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">158</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Team[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">159</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">160</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">161</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">162</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">163</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Billing[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">164</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">165</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">166</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">167</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">168</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Limits[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">169</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">170</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">171</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]sidebar-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">172</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">173</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">174</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">175</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]sidebar-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">176</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]sidebar-group-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">177</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]sidebar-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">178</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]sidebar-group[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-projects"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">179</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]sidebar-group-label[/color]>[/color]Projects[color=#116329]</[color=#0550ae]sidebar-group-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">180</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]sidebar-group-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">181</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]sidebar-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">182</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">183</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">184</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"code-block"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">185</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Design Engineering[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">186</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">187</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]dropdown-menu[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-project-menu"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">188</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]sidebar-menu-action[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-action-trigger"[/color] [color=#0550ae]data-dropdown-menu-trigger[/color]=[color=#0a3069]"true"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">189</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"more-horizontal"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">190</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]sidebar-menu-action[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">191</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]dropdown-menu-content[/color] [color=#0550ae]side[/color]=[color=#0a3069]"right"[/color] [color=#0550ae]align[/color]=[color=#0a3069]"start"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">192</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">193</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"folder"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">194</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]text[/color]>[/color]View Project[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">195</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">196</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">197</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"arrow-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">198</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]text[/color]>[/color]Share Project[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">199</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">200</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]dropdown-menu-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">201</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]dropdown-menu-item[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"destructive"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">202</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"delete"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">203</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]text[/color]>[/color]Delete Project[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">204</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">205</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]dropdown-menu-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">206</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]dropdown-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">207</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">208</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">209</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">210</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"book-open"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">211</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Sales [color=#0550ae]&amp;[/color] Marketing[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">212</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">213</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]dropdown-menu[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-project-menu"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">214</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]sidebar-menu-action[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-action-trigger"[/color] [color=#0550ae]data-dropdown-menu-trigger[/color]=[color=#0a3069]"true"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">215</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"more-horizontal"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">216</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]sidebar-menu-action[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">217</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]dropdown-menu-content[/color] [color=#0550ae]side[/color]=[color=#0a3069]"right"[/color] [color=#0550ae]align[/color]=[color=#0a3069]"start"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">218</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">219</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"folder"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">220</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]text[/color]>[/color]View Project[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">221</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">222</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">223</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"arrow-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">224</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]text[/color]>[/color]Share Project[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">225</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">226</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]dropdown-menu-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">227</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]dropdown-menu-item[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"destructive"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">228</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"delete"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">229</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]text[/color]>[/color]Delete Project[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">230</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">231</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]dropdown-menu-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">232</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]dropdown-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">233</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">234</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">235</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">236</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">237</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]Travel[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">238</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">239</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]dropdown-menu[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-project-menu"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">240</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]sidebar-menu-action[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-action-trigger"[/color] [color=#0550ae]data-dropdown-menu-trigger[/color]=[color=#0a3069]"true"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">241</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"more-horizontal"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">242</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]sidebar-menu-action[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">243</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]dropdown-menu-content[/color] [color=#0550ae]side[/color]=[color=#0a3069]"right"[/color] [color=#0550ae]align[/color]=[color=#0a3069]"start"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">244</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">245</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"folder"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">246</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]text[/color]>[/color]View Project[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">247</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">248</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">249</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"arrow-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">250</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]text[/color]>[/color]Share Project[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">251</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">252</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]dropdown-menu-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">253</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]dropdown-menu-item[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"destructive"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">254</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"delete"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">255</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]text[/color]>[/color]Delete Project[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">256</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">257</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]dropdown-menu-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">258</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]dropdown-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">259</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">260</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">261</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button sidebar-more"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">262</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"more-horizontal"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">263</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]More[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">264</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">265</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">266</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]sidebar-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">267</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]sidebar-group-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">268</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]sidebar-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">269</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]scrollarea-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">270</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]scrollbar[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">271</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]scrollarea[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">272</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]sidebar-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">273</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]sidebar-footer[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">274</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]sidebar-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">275</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">276</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]dropdown-menu[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-user-menu"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">277</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-button sidebar-user"[/color] [color=#0550ae]data-sidebar-tooltip[/color]=[color=#0a3069]"shadcn"[/color] [color=#0550ae]data-dropdown-menu-trigger[/color]=[color=#0a3069]"true"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">278</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-avatar"[/color]>[/color][color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"person-circle"[/color] />[/color][color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">279</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-text"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">280</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-name"[/color]>[/color]shadcn[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">281</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-meta"[/color]>[/color]m@example.com[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">282</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">283</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-menu-chevron"[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-up-down"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">284</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">285</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]dropdown-menu-content[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-user-menu-content"[/color] [color=#0550ae]side[/color]=[color=#0a3069]"right"[/color] [color=#0550ae]align[/color]=[color=#0a3069]"end"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">286</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]dropdown-menu-label[/color]>[/color]shadcn[color=#116329]</[color=#0550ae]dropdown-menu-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">287</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-user-menu-email"[/color]>[/color]m@example.com[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">288</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]dropdown-menu-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">289</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">290</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"premium"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">291</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]text[/color]>[/color]Upgrade to Pro[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">292</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">293</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]dropdown-menu-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">294</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">295</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"person"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">296</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]text[/color]>[/color]Account[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">297</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">298</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">299</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"payment"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">300</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]text[/color]>[/color]Billing[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">301</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">302</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">303</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"alert"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">304</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]text[/color]>[/color]Notifications[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">305</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">306</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]dropdown-menu-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">307</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">308</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"sign-out"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">309</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]text[/color]>[/color]Sign Out[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">310</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">311</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]dropdown-menu-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">312</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]dropdown-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">313</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">314</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]sidebar-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">315</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]sidebar-footer[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">316</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]sidebar-rail[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">317</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]sidebar[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">318</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"sidebar-main"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">319</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]sidebar-trigger[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">320</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">321</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]sidebar-provider[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">322</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">323</Text>
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
                  <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]ui[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]sidebar-provider[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-basic-demo"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]sidebar[/color] [color=#79c0ff]collapsible[/color]=[color=#a5d6ff]"icon"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]sidebar-header[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]dropdown-menu[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-team-menu"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button sidebar-team"[/color] [color=#79c0ff]data-sidebar-tooltip[/color]=[color=#a5d6ff]"Acme Inc"[/color] [color=#79c0ff]data-dropdown-menu-trigger[/color]=[color=#a5d6ff]"true"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]div[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-team-logo"[/color]>[/color][color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"people-team"[/color] />[/color][color=#7ee787]</[color=#79c0ff]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]div[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-text"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-name"[/color]>[/color]Acme Inc[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-meta"[/color]>[/color]Enterprise[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-chevron"[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-up-down"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]dropdown-menu-content[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-team-menu-content"[/color] [color=#79c0ff]side[/color]=[color=#a5d6ff]"right"[/color] [color=#79c0ff]align[/color]=[color=#a5d6ff]"start"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]dropdown-menu-label[/color]>[/color]Teams[color=#7ee787]</[color=#79c0ff]dropdown-menu-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"people-team"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Acme Inc[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"building"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Acme Corp[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"shield"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Evil Corp[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]dropdown-menu-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"add"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Add team[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]dropdown-menu-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]dropdown-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]sidebar-header[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]sidebar-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]scrollarea[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-scroll"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]scrollarea-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]sidebar-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]sidebar-group-label[/color]>[/color]Platform[color=#7ee787]</[color=#79c0ff]sidebar-group-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]sidebar-group-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]sidebar-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]collapsible[/color] [color=#79c0ff]default-open[/color]=[color=#a5d6ff]"true"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button"[/color] [color=#79c0ff]data-sidebar-tooltip[/color]=[color=#a5d6ff]"Playground"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"code-block"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Playground[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-chevron"[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]sidebar-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]History[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Starred[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Settings[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]sidebar-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">73</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">74</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">75</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">76</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">77</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">78</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button"[/color] [color=#79c0ff]data-sidebar-tooltip[/color]=[color=#a5d6ff]"Models"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">79</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">80</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Models[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">81</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-chevron"[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">82</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">83</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">84</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">85</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]sidebar-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">86</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">87</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">88</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Genesis[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">89</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">90</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">91</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">92</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">93</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Explorer[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">94</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">95</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">96</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">97</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">98</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Quantum[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">99</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">100</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">101</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]sidebar-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">102</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">103</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">104</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">105</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">106</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">107</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">108</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button"[/color] [color=#79c0ff]data-sidebar-tooltip[/color]=[color=#a5d6ff]"Documentation"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">109</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"book-open"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">110</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Documentation[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">111</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-chevron"[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">112</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">113</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">114</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">115</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]sidebar-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">116</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">117</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">118</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Introduction[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">119</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">120</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">121</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">122</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">123</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Get Started[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">124</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">125</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">126</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">127</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">128</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Tutorials[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">129</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">130</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">131</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">132</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">133</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Changelog[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">134</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">135</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">136</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]sidebar-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">137</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">138</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">139</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">140</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">141</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">142</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">143</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button"[/color] [color=#79c0ff]data-sidebar-tooltip[/color]=[color=#a5d6ff]"Settings"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">144</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"calendar-settings"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">145</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Settings[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">146</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-chevron"[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">147</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">148</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">149</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">150</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]sidebar-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">151</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">152</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">153</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]General[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">154</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">155</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">156</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">157</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">158</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Team[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">159</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">160</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">161</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">162</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">163</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Billing[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">164</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">165</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">166</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">167</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button sidebar-sub-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">168</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Limits[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">169</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">170</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]sidebar-menu-sub-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">171</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]sidebar-menu-sub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">172</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">173</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">174</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">175</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]sidebar-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">176</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]sidebar-group-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">177</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]sidebar-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">178</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]sidebar-group[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-projects"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">179</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]sidebar-group-label[/color]>[/color]Projects[color=#7ee787]</[color=#79c0ff]sidebar-group-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">180</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]sidebar-group-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">181</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]sidebar-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">182</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">183</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">184</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"code-block"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">185</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Design Engineering[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">186</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">187</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]dropdown-menu[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-project-menu"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">188</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]sidebar-menu-action[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-action-trigger"[/color] [color=#79c0ff]data-dropdown-menu-trigger[/color]=[color=#a5d6ff]"true"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">189</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"more-horizontal"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">190</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]sidebar-menu-action[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">191</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]dropdown-menu-content[/color] [color=#79c0ff]side[/color]=[color=#a5d6ff]"right"[/color] [color=#79c0ff]align[/color]=[color=#a5d6ff]"start"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">192</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">193</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"folder"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">194</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]View Project[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">195</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">196</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">197</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"arrow-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">198</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Share Project[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">199</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">200</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]dropdown-menu-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">201</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"destructive"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">202</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"delete"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">203</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Delete Project[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">204</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">205</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]dropdown-menu-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">206</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]dropdown-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">207</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">208</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">209</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">210</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"book-open"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">211</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Sales [color=#79c0ff]&amp;[/color] Marketing[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">212</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">213</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]dropdown-menu[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-project-menu"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">214</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]sidebar-menu-action[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-action-trigger"[/color] [color=#79c0ff]data-dropdown-menu-trigger[/color]=[color=#a5d6ff]"true"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">215</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"more-horizontal"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">216</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]sidebar-menu-action[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">217</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]dropdown-menu-content[/color] [color=#79c0ff]side[/color]=[color=#a5d6ff]"right"[/color] [color=#79c0ff]align[/color]=[color=#a5d6ff]"start"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">218</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">219</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"folder"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">220</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]View Project[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">221</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">222</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">223</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"arrow-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">224</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Share Project[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">225</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">226</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]dropdown-menu-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">227</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"destructive"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">228</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"delete"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">229</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Delete Project[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">230</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">231</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]dropdown-menu-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">232</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]dropdown-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">233</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">234</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">235</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">236</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"document-text"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">237</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]Travel[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">238</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">239</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]dropdown-menu[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-project-menu"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">240</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]sidebar-menu-action[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-action-trigger"[/color] [color=#79c0ff]data-dropdown-menu-trigger[/color]=[color=#a5d6ff]"true"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">241</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"more-horizontal"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">242</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]sidebar-menu-action[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">243</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]dropdown-menu-content[/color] [color=#79c0ff]side[/color]=[color=#a5d6ff]"right"[/color] [color=#79c0ff]align[/color]=[color=#a5d6ff]"start"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">244</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">245</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"folder"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">246</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]View Project[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">247</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">248</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">249</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"arrow-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">250</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Share Project[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">251</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">252</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]dropdown-menu-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">253</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"destructive"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">254</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"delete"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">255</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Delete Project[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">256</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">257</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]dropdown-menu-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">258</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]dropdown-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">259</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">260</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">261</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button sidebar-more"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">262</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"more-horizontal"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">263</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]More[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">264</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">265</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">266</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]sidebar-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">267</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]sidebar-group-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">268</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]sidebar-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">269</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]scrollarea-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">270</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]scrollbar[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">271</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]scrollarea[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">272</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]sidebar-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">273</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]sidebar-footer[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">274</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]sidebar-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">275</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">276</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]dropdown-menu[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-user-menu"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">277</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-button sidebar-user"[/color] [color=#79c0ff]data-sidebar-tooltip[/color]=[color=#a5d6ff]"shadcn"[/color] [color=#79c0ff]data-dropdown-menu-trigger[/color]=[color=#a5d6ff]"true"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">278</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]div[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-avatar"[/color]>[/color][color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"person-circle"[/color] />[/color][color=#7ee787]</[color=#79c0ff]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">279</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]div[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-text"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">280</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-name"[/color]>[/color]shadcn[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">281</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-meta"[/color]>[/color]m@example.com[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">282</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">283</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-menu-chevron"[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-up-down"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">284</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">285</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]dropdown-menu-content[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-user-menu-content"[/color] [color=#79c0ff]side[/color]=[color=#a5d6ff]"right"[/color] [color=#79c0ff]align[/color]=[color=#a5d6ff]"end"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">286</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]dropdown-menu-label[/color]>[/color]shadcn[color=#7ee787]</[color=#79c0ff]dropdown-menu-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">287</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-user-menu-email"[/color]>[/color]m@example.com[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">288</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]dropdown-menu-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">289</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">290</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"premium"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">291</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Upgrade to Pro[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">292</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">293</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]dropdown-menu-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">294</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">295</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"person"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">296</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Account[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">297</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">298</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">299</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"payment"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">300</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Billing[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">301</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">302</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">303</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"alert"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">304</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Notifications[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">305</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">306</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]dropdown-menu-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">307</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">308</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"sign-out"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">309</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Sign Out[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">310</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]dropdown-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">311</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]dropdown-menu-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">312</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]dropdown-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">313</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]sidebar-menu-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">314</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]sidebar-menu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">315</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]sidebar-footer[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">316</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]sidebar-rail[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">317</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]sidebar[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">318</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]div[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"sidebar-main"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">319</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]sidebar-trigger[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">320</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">321</Text>
                  <Text className="code-line">{`    [color=#7ee787]</[color=#79c0ff]sidebar-provider[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">322</Text>
                  <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]ui[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">323</Text>
                  <Text className="code-line">{`[color=#7ee787]</[color=#79c0ff]lcui-app[/color]>[/color]`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/sidebar-basic/2" style={{ display: "none" }}>
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
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]sidebar_basic_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
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
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]sidebar_basic_xml_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
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
        <Widget className="code-block-body" data-language="tsx" data-file="example.tsx" data-source="examples/sidebar-basic/3" style={{ display: "none" }}>
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
                  <Text className="code-line">{`  [color=#8250df]Collapsible[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#8250df]CollapsibleContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#8250df]CollapsibleTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenu[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuContent[/color],`}</Text>
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
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuSeparator[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`  [color=#8250df]DropdownMenuTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`  [color=#8250df]Icon[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`  [color=#8250df]ScrollArea[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`  [color=#8250df]ScrollAreaContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`  [color=#8250df]Scrollbar[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`  [color=#8250df]Sidebar[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`  [color=#8250df]SidebarContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`  [color=#8250df]SidebarFooter[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`  [color=#8250df]SidebarGroup[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`  [color=#8250df]SidebarGroupContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`  [color=#8250df]SidebarGroupLabel[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`  [color=#8250df]SidebarHeader[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`  [color=#8250df]SidebarMenu[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`  [color=#8250df]SidebarMenuAction[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`  [color=#8250df]SidebarMenuButton[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`  [color=#8250df]SidebarMenuItem[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`  [color=#8250df]SidebarMenuSub[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`  [color=#8250df]SidebarMenuSubButton[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`  [color=#8250df]SidebarMenuSubItem[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`  [color=#8250df]SidebarProvider[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`  [color=#8250df]SidebarRail[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`  [color=#8250df]SidebarTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`  [color=#8250df]Text[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`[color=#cf222e]const[/color] nav = \\[`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`  {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`    [color=#0550ae]title[/color]: [color=#0a3069]"Playground"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`    [color=#0550ae]icon[/color]: [color=#0a3069]"code-block"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`    [color=#0550ae]open[/color]: [color=#0550ae]true[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`    [color=#0550ae]items[/color]: \\[[color=#0a3069]"History"[/color], [color=#0a3069]"Starred"[/color], [color=#0a3069]"Settings"[/color]],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`  },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`  {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`    [color=#0550ae]title[/color]: [color=#0a3069]"Models"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`    [color=#0550ae]icon[/color]: [color=#0a3069]"document-text"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`    [color=#0550ae]items[/color]: \\[[color=#0a3069]"Genesis"[/color], [color=#0a3069]"Explorer"[/color], [color=#0a3069]"Quantum"[/color]],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`  },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`  {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`    [color=#0550ae]title[/color]: [color=#0a3069]"Documentation"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`    [color=#0550ae]icon[/color]: [color=#0a3069]"book-open"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`    [color=#0550ae]items[/color]: \\[[color=#0a3069]"Introduction"[/color], [color=#0a3069]"Get Started"[/color], [color=#0a3069]"Tutorials"[/color], [color=#0a3069]"Changelog"[/color]],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`  },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`  {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`    [color=#0550ae]title[/color]: [color=#0a3069]"Settings"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`    [color=#0550ae]icon[/color]: [color=#0a3069]"calendar-settings"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`    [color=#0550ae]items[/color]: \\[[color=#0a3069]"General"[/color], [color=#0a3069]"Team"[/color], [color=#0a3069]"Billing"[/color], [color=#0a3069]"Limits"[/color]],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`  },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`[color=#cf222e]const[/color] projects = \\[`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`  { [color=#0550ae]title[/color]: [color=#0a3069]"Design Engineering"[/color], [color=#0550ae]icon[/color]: [color=#0a3069]"code-block"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`  { [color=#0550ae]title[/color]: [color=#0a3069]"Sales & Marketing"[/color], [color=#0550ae]icon[/color]: [color=#0a3069]"book-open"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`  { [color=#0550ae]title[/color]: [color=#0a3069]"Travel"[/color], [color=#0550ae]icon[/color]: [color=#0a3069]"document-text"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]SidebarProvider[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex h-\\[500px] w-full rounded-lg border"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Sidebar[/color] [color=#0550ae]collapsible[/color]=[color=#0a3069]"icon"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]SidebarHeader[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]SidebarMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]SidebarMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">73</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenu[/color] [color=#0550ae]className[/color]=[color=#0a3069]"sidebar-team-menu"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">74</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">75</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]SidebarMenuButton[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">76</Text>
                  <Text className="code-line">{`[color=#116329]                    [color=#0550ae]className[/color]=[color=#0a3069]"sidebar-team h-12 p-2"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">77</Text>
                  <Text className="code-line">{`[color=#116329]                    [color=#0550ae]data-sidebar-tooltip[/color]=[color=#0a3069]"Acme Inc"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">78</Text>
                  <Text className="code-line">{`[color=#116329]                  >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">79</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"sidebar-team-logo"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">80</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"people-team"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">81</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">82</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-1 flex-col gap-0.5 text-left"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">83</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"text-sm font-medium"[/color]>[/color]Acme Inc[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">84</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"text-xs text-muted-foreground"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">85</Text>
                  <Text className="code-line">{`                        Enterprise`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">86</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">87</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">88</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Icon[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">89</Text>
                  <Text className="code-line">{`[color=#116329]                      [color=#0550ae]className[/color]=[color=#0a3069]"sidebar-menu-chevron"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">90</Text>
                  <Text className="code-line">{`[color=#116329]                      [color=#0550ae]name[/color]=[color=#0a3069]"chevron-up-down"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">91</Text>
                  <Text className="code-line">{`[color=#116329]                    />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">92</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]SidebarMenuButton[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">93</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">94</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]DropdownMenuContent[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-56"[/color] [color=#0550ae]side[/color]=[color=#0a3069]"right"[/color] [color=#0550ae]align[/color]=[color=#0a3069]"start"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">95</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuLabel[/color]>[/color]Teams[color=#116329]</[color=#0550ae]DropdownMenuLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">96</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">97</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"people-team"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">98</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Text[/color]>[/color]Acme Inc[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">99</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">100</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">101</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"building"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">102</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Text[/color]>[/color]Acme Corp[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">103</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">104</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">105</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"shield"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">106</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Text[/color]>[/color]Evil Corp[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">107</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">108</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuSeparator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">109</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">110</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"add"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">111</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Text[/color]>[/color]Add team[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">112</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">113</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]DropdownMenuContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">114</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]DropdownMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">115</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]SidebarMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">116</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]SidebarMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">117</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]SidebarHeader[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">118</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]SidebarContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">119</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]ScrollArea[/color] [color=#0550ae]className[/color]=[color=#0a3069]"sidebar-scroll"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">120</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]ScrollAreaContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">121</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]SidebarGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">122</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]SidebarGroupLabel[/color]>[/color]Platform[color=#116329]</[color=#0550ae]SidebarGroupLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">123</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]SidebarGroupContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">124</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]SidebarMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">125</Text>
                  <Text className="code-line">{`                    {nav.map((item) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">126</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]Collapsible[/color] [color=#0550ae]key[/color]=[color=#0a3069]{item.title}[/color] [color=#0550ae]defaultOpen[/color]=[color=#0a3069]{item.open}[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">127</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]SidebarMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">128</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">129</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]SidebarMenuButton[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">130</Text>
                  <Text className="code-line">{`[color=#116329]                              [color=#0550ae]data-sidebar-tooltip[/color]=[color=#0a3069]{item.title}[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">131</Text>
                  <Text className="code-line">{`[color=#116329]                            >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">132</Text>
                  <Text className="code-line">{`                              [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]{item.icon}[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">133</Text>
                  <Text className="code-line">{`                              [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">134</Text>
                  <Text className="code-line">{`                                {item.title}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">135</Text>
                  <Text className="code-line">{`                              [color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">136</Text>
                  <Text className="code-line">{`                              [color=#116329]<[color=#0550ae]Icon[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">137</Text>
                  <Text className="code-line">{`[color=#116329]                                [color=#0550ae]className[/color]=[color=#0a3069]"sidebar-menu-chevron"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">138</Text>
                  <Text className="code-line">{`[color=#116329]                                [color=#0550ae]name[/color]=[color=#0a3069]"chevron-right"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">139</Text>
                  <Text className="code-line">{`[color=#116329]                              />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">140</Text>
                  <Text className="code-line">{`                            [color=#116329]</[color=#0550ae]SidebarMenuButton[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">141</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">142</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">143</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]SidebarMenuSub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">144</Text>
                  <Text className="code-line">{`                              {item.items.map((sub) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">145</Text>
                  <Text className="code-line">{`                                [color=#116329]<[color=#0550ae]SidebarMenuSubItem[/color] [color=#0550ae]key[/color]=[color=#0a3069]{sub}[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">146</Text>
                  <Text className="code-line">{`                                  [color=#116329]<[color=#0550ae]SidebarMenuSubButton[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">147</Text>
                  <Text className="code-line">{`                                    [color=#116329]<[color=#0550ae]Text[/color]>[/color]{sub}[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">148</Text>
                  <Text className="code-line">{`                                  [color=#116329]</[color=#0550ae]SidebarMenuSubButton[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">149</Text>
                  <Text className="code-line">{`                                [color=#116329]</[color=#0550ae]SidebarMenuSubItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">150</Text>
                  <Text className="code-line">{`                              ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">151</Text>
                  <Text className="code-line">{`                            [color=#116329]</[color=#0550ae]SidebarMenuSub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">152</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">153</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]SidebarMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">154</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]Collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">155</Text>
                  <Text className="code-line">{`                    ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">156</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]SidebarMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">157</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]SidebarGroupContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">158</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]SidebarGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">159</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]SidebarGroup[/color] [color=#0550ae]className[/color]=[color=#0a3069]"sidebar-projects"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">160</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]SidebarGroupLabel[/color]>[/color]Projects[color=#116329]</[color=#0550ae]SidebarGroupLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">161</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]SidebarGroupContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">162</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]SidebarMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">163</Text>
                  <Text className="code-line">{`                    {projects.map((project) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">164</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]SidebarMenuItem[/color] [color=#0550ae]key[/color]=[color=#0a3069]{project.title}[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">165</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]SidebarMenuButton[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">166</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]{project.icon}[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">167</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">168</Text>
                  <Text className="code-line">{`                            {project.title}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">169</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">170</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]SidebarMenuButton[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">171</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]DropdownMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">172</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]DropdownMenuTrigger[/color] [color=#0550ae]className[/color]=[color=#0a3069]"sidebar-menu-action-trigger"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">173</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]SidebarMenuAction[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">174</Text>
                  <Text className="code-line">{`                              [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"more-horizontal"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">175</Text>
                  <Text className="code-line">{`                            [color=#116329]</[color=#0550ae]SidebarMenuAction[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">176</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">177</Text>
                  <Text className="code-line">{`                          [color=#116329]<[color=#0550ae]DropdownMenuContent[/color] [color=#0550ae]side[/color]=[color=#0a3069]"right"[/color] [color=#0550ae]align[/color]=[color=#0a3069]"start"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">178</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">179</Text>
                  <Text className="code-line">{`                              [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"folder"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">180</Text>
                  <Text className="code-line">{`                              [color=#116329]<[color=#0550ae]Text[/color]>[/color]View Project[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">181</Text>
                  <Text className="code-line">{`                            [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">182</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">183</Text>
                  <Text className="code-line">{`                              [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"arrow-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">184</Text>
                  <Text className="code-line">{`                              [color=#116329]<[color=#0550ae]Text[/color]>[/color]Share Project[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">185</Text>
                  <Text className="code-line">{`                            [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">186</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]DropdownMenuSeparator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">187</Text>
                  <Text className="code-line">{`                            [color=#116329]<[color=#0550ae]DropdownMenuItem[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"destructive"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">188</Text>
                  <Text className="code-line">{`                              [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"delete"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">189</Text>
                  <Text className="code-line">{`                              [color=#116329]<[color=#0550ae]Text[/color]>[/color]Delete Project[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">190</Text>
                  <Text className="code-line">{`                            [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">191</Text>
                  <Text className="code-line">{`                          [color=#116329]</[color=#0550ae]DropdownMenuContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">192</Text>
                  <Text className="code-line">{`                        [color=#116329]</[color=#0550ae]DropdownMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">193</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]SidebarMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">194</Text>
                  <Text className="code-line">{`                    ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">195</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]SidebarMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">196</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]SidebarMenuButton[/color] [color=#0550ae]className[/color]=[color=#0a3069]"sidebar-more"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">197</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"more-horizontal"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">198</Text>
                  <Text className="code-line">{`                        [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"sidebar-menu-label"[/color]>[/color]More[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">199</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]SidebarMenuButton[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">200</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]SidebarMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">201</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]SidebarMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">202</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]SidebarGroupContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">203</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]SidebarGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">204</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]ScrollAreaContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">205</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Scrollbar[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">206</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]ScrollArea[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">207</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]SidebarContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">208</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]SidebarFooter[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">209</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]SidebarMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">210</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]SidebarMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">211</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]DropdownMenu[/color] [color=#0550ae]className[/color]=[color=#0a3069]"sidebar-user-menu"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">212</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">213</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]SidebarMenuButton[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">214</Text>
                  <Text className="code-line">{`[color=#116329]                    [color=#0550ae]className[/color]=[color=#0a3069]"sidebar-user h-11 p-2"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">215</Text>
                  <Text className="code-line">{`[color=#116329]                    [color=#0550ae]data-sidebar-tooltip[/color]=[color=#0a3069]"shadcn"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">216</Text>
                  <Text className="code-line">{`[color=#116329]                  >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">217</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"sidebar-avatar"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">218</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"person-circle"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">219</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">220</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-1 flex-col gap-0.5 text-left"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">221</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"text-sm font-medium"[/color]>[/color]shadcn[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">222</Text>
                  <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"text-xs text-muted-foreground"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">223</Text>
                  <Text className="code-line">{`                        m@example.com`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">224</Text>
                  <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">225</Text>
                  <Text className="code-line">{`                    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">226</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Icon[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">227</Text>
                  <Text className="code-line">{`[color=#116329]                      [color=#0550ae]className[/color]=[color=#0a3069]"sidebar-menu-chevron"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">228</Text>
                  <Text className="code-line">{`[color=#116329]                      [color=#0550ae]name[/color]=[color=#0a3069]"chevron-up-down"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">229</Text>
                  <Text className="code-line">{`[color=#116329]                    />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">230</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]SidebarMenuButton[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">231</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">232</Text>
                  <Text className="code-line">{`                [color=#116329]<[color=#0550ae]DropdownMenuContent[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">233</Text>
                  <Text className="code-line">{`[color=#116329]                  [color=#0550ae]className[/color]=[color=#0a3069]"w-56"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">234</Text>
                  <Text className="code-line">{`[color=#116329]                  [color=#0550ae]side[/color]=[color=#0a3069]"right"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">235</Text>
                  <Text className="code-line">{`[color=#116329]                  [color=#0550ae]align[/color]=[color=#0a3069]"end"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">236</Text>
                  <Text className="code-line">{`[color=#116329]                >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">237</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuLabel[/color]>[/color]shadcn[color=#116329]</[color=#0550ae]DropdownMenuLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">238</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"px-2 pb-1.5 text-xs text-muted-foreground"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">239</Text>
                  <Text className="code-line">{`                    m@example.com`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">240</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">241</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuSeparator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">242</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">243</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"premium"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">244</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Text[/color]>[/color]Upgrade to Pro[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">245</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">246</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuSeparator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">247</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">248</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"person"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">249</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Text[/color]>[/color]Account[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">250</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">251</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">252</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"payment"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">253</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Text[/color]>[/color]Billing[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">254</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">255</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">256</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"alert"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">257</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Text[/color]>[/color]Notifications[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">258</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">259</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuSeparator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">260</Text>
                  <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">261</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"sign-out"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">262</Text>
                  <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]Text[/color]>[/color]Sign Out[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">263</Text>
                  <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">264</Text>
                  <Text className="code-line">{`                [color=#116329]</[color=#0550ae]DropdownMenuContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">265</Text>
                  <Text className="code-line">{`              [color=#116329]</[color=#0550ae]DropdownMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">266</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]SidebarMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">267</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]SidebarMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">268</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]SidebarFooter[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">269</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]SidebarRail[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">270</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Sidebar[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">271</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-1 flex-col gap-2 p-3"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">272</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]SidebarTrigger[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">273</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">274</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]SidebarProvider[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">275</Text>
                  <Text className="code-line">{`  );`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">276</Text>
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
                  <Text className="code-line">{`  [color=#d2a8ff]Collapsible[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]CollapsibleContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]CollapsibleTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenu[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenuContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenuItem[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenuLabel[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenuSeparator[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]DropdownMenuTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]Icon[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]ScrollArea[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]ScrollAreaContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]Scrollbar[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]Sidebar[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SidebarContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SidebarFooter[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SidebarGroup[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SidebarGroupContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SidebarGroupLabel[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SidebarHeader[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SidebarMenu[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SidebarMenuAction[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SidebarMenuButton[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SidebarMenuItem[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SidebarMenuSub[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SidebarMenuSubButton[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SidebarMenuSubItem[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SidebarProvider[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SidebarRail[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SidebarTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]Text[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]Widget[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`} [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`[color=#ff7b72]const[/color] nav = \\[`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`  {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`    [color=#79c0ff]title[/color]: [color=#a5d6ff]"Playground"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`    [color=#79c0ff]icon[/color]: [color=#a5d6ff]"code-block"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`    [color=#79c0ff]open[/color]: [color=#79c0ff]true[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`    [color=#79c0ff]items[/color]: \\[[color=#a5d6ff]"History"[/color], [color=#a5d6ff]"Starred"[/color], [color=#a5d6ff]"Settings"[/color]],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`  },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`  {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`    [color=#79c0ff]title[/color]: [color=#a5d6ff]"Models"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`    [color=#79c0ff]icon[/color]: [color=#a5d6ff]"document-text"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`    [color=#79c0ff]items[/color]: \\[[color=#a5d6ff]"Genesis"[/color], [color=#a5d6ff]"Explorer"[/color], [color=#a5d6ff]"Quantum"[/color]],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`  },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`  {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`    [color=#79c0ff]title[/color]: [color=#a5d6ff]"Documentation"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`    [color=#79c0ff]icon[/color]: [color=#a5d6ff]"book-open"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`    [color=#79c0ff]items[/color]: \\[[color=#a5d6ff]"Introduction"[/color], [color=#a5d6ff]"Get Started"[/color], [color=#a5d6ff]"Tutorials"[/color], [color=#a5d6ff]"Changelog"[/color]],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`  },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`  {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`    [color=#79c0ff]title[/color]: [color=#a5d6ff]"Settings"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`    [color=#79c0ff]icon[/color]: [color=#a5d6ff]"calendar-settings"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`    [color=#79c0ff]items[/color]: \\[[color=#a5d6ff]"General"[/color], [color=#a5d6ff]"Team"[/color], [color=#a5d6ff]"Billing"[/color], [color=#a5d6ff]"Limits"[/color]],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`  },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`[color=#ff7b72]const[/color] projects = \\[`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]title[/color]: [color=#a5d6ff]"Design Engineering"[/color], [color=#79c0ff]icon[/color]: [color=#a5d6ff]"code-block"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]title[/color]: [color=#a5d6ff]"Sales & Marketing"[/color], [color=#79c0ff]icon[/color]: [color=#a5d6ff]"book-open"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]title[/color]: [color=#a5d6ff]"Travel"[/color], [color=#79c0ff]icon[/color]: [color=#a5d6ff]"document-text"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`[color=#ff7b72]export[/color] [color=#ff7b72]default[/color] [color=#ff7b72]function[/color] [color=#d2a8ff]App[/color]([color=#79c0ff][/color]) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`  [color=#ff7b72]return[/color] (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]SidebarProvider[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"flex h-\\[500px] w-full rounded-lg border"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]Sidebar[/color] [color=#79c0ff]collapsible[/color]=[color=#a5d6ff]"icon"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]SidebarHeader[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]SidebarMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]SidebarMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">73</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]DropdownMenu[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"sidebar-team-menu"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">74</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">75</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]SidebarMenuButton[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">76</Text>
                  <Text className="code-line">{`[color=#7ee787]                    [color=#79c0ff]className[/color]=[color=#a5d6ff]"sidebar-team h-12 p-2"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">77</Text>
                  <Text className="code-line">{`[color=#7ee787]                    [color=#79c0ff]data-sidebar-tooltip[/color]=[color=#a5d6ff]"Acme Inc"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">78</Text>
                  <Text className="code-line">{`[color=#7ee787]                  >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">79</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Widget[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"sidebar-team-logo"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">80</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"people-team"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">81</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">82</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Widget[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"flex flex-1 flex-col gap-0.5 text-left"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">83</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]Text[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"text-sm font-medium"[/color]>[/color]Acme Inc[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">84</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]Text[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"text-xs text-muted-foreground"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">85</Text>
                  <Text className="code-line">{`                        Enterprise`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">86</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">87</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">88</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Icon[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">89</Text>
                  <Text className="code-line">{`[color=#7ee787]                      [color=#79c0ff]className[/color]=[color=#a5d6ff]"sidebar-menu-chevron"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">90</Text>
                  <Text className="code-line">{`[color=#7ee787]                      [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-up-down"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">91</Text>
                  <Text className="code-line">{`[color=#7ee787]                    />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">92</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]SidebarMenuButton[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">93</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">94</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]DropdownMenuContent[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"w-56"[/color] [color=#79c0ff]side[/color]=[color=#a5d6ff]"right"[/color] [color=#79c0ff]align[/color]=[color=#a5d6ff]"start"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">95</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuLabel[/color]>[/color]Teams[color=#7ee787]</[color=#79c0ff]DropdownMenuLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">96</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">97</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"people-team"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">98</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Acme Inc[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">99</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">100</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">101</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"building"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">102</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Acme Corp[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">103</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">104</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">105</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"shield"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">106</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Evil Corp[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">107</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">108</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuSeparator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">109</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">110</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"add"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">111</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Add team[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">112</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">113</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]DropdownMenuContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">114</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]DropdownMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">115</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]SidebarMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">116</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]SidebarMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">117</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]SidebarHeader[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">118</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]SidebarContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">119</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]ScrollArea[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"sidebar-scroll"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">120</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]ScrollAreaContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">121</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]SidebarGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">122</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]SidebarGroupLabel[/color]>[/color]Platform[color=#7ee787]</[color=#79c0ff]SidebarGroupLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">123</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]SidebarGroupContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">124</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]SidebarMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">125</Text>
                  <Text className="code-line">{`                    {nav.map((item) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">126</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]Collapsible[/color] [color=#79c0ff]key[/color]=[color=#a5d6ff]{item.title}[/color] [color=#79c0ff]defaultOpen[/color]=[color=#a5d6ff]{item.open}[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">127</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]SidebarMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">128</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">129</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]SidebarMenuButton[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">130</Text>
                  <Text className="code-line">{`[color=#7ee787]                              [color=#79c0ff]data-sidebar-tooltip[/color]=[color=#a5d6ff]{item.title}[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">131</Text>
                  <Text className="code-line">{`[color=#7ee787]                            >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">132</Text>
                  <Text className="code-line">{`                              [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]{item.icon}[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">133</Text>
                  <Text className="code-line">{`                              [color=#7ee787]<[color=#79c0ff]Text[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">134</Text>
                  <Text className="code-line">{`                                {item.title}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">135</Text>
                  <Text className="code-line">{`                              [color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">136</Text>
                  <Text className="code-line">{`                              [color=#7ee787]<[color=#79c0ff]Icon[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">137</Text>
                  <Text className="code-line">{`[color=#7ee787]                                [color=#79c0ff]className[/color]=[color=#a5d6ff]"sidebar-menu-chevron"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">138</Text>
                  <Text className="code-line">{`[color=#7ee787]                                [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-right"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">139</Text>
                  <Text className="code-line">{`[color=#7ee787]                              />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">140</Text>
                  <Text className="code-line">{`                            [color=#7ee787]</[color=#79c0ff]SidebarMenuButton[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">141</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">142</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">143</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]SidebarMenuSub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">144</Text>
                  <Text className="code-line">{`                              {item.items.map((sub) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">145</Text>
                  <Text className="code-line">{`                                [color=#7ee787]<[color=#79c0ff]SidebarMenuSubItem[/color] [color=#79c0ff]key[/color]=[color=#a5d6ff]{sub}[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">146</Text>
                  <Text className="code-line">{`                                  [color=#7ee787]<[color=#79c0ff]SidebarMenuSubButton[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">147</Text>
                  <Text className="code-line">{`                                    [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]{sub}[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">148</Text>
                  <Text className="code-line">{`                                  [color=#7ee787]</[color=#79c0ff]SidebarMenuSubButton[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">149</Text>
                  <Text className="code-line">{`                                [color=#7ee787]</[color=#79c0ff]SidebarMenuSubItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">150</Text>
                  <Text className="code-line">{`                              ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">151</Text>
                  <Text className="code-line">{`                            [color=#7ee787]</[color=#79c0ff]SidebarMenuSub[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">152</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">153</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]SidebarMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">154</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]Collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">155</Text>
                  <Text className="code-line">{`                    ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">156</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]SidebarMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">157</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]SidebarGroupContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">158</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]SidebarGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">159</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]SidebarGroup[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"sidebar-projects"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">160</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]SidebarGroupLabel[/color]>[/color]Projects[color=#7ee787]</[color=#79c0ff]SidebarGroupLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">161</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]SidebarGroupContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">162</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]SidebarMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">163</Text>
                  <Text className="code-line">{`                    {projects.map((project) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">164</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]SidebarMenuItem[/color] [color=#79c0ff]key[/color]=[color=#a5d6ff]{project.title}[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">165</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]SidebarMenuButton[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">166</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]{project.icon}[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">167</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]Text[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">168</Text>
                  <Text className="code-line">{`                            {project.title}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">169</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">170</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]SidebarMenuButton[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">171</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]DropdownMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">172</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]DropdownMenuTrigger[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"sidebar-menu-action-trigger"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">173</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]SidebarMenuAction[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">174</Text>
                  <Text className="code-line">{`                              [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"more-horizontal"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">175</Text>
                  <Text className="code-line">{`                            [color=#7ee787]</[color=#79c0ff]SidebarMenuAction[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">176</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">177</Text>
                  <Text className="code-line">{`                          [color=#7ee787]<[color=#79c0ff]DropdownMenuContent[/color] [color=#79c0ff]side[/color]=[color=#a5d6ff]"right"[/color] [color=#79c0ff]align[/color]=[color=#a5d6ff]"start"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">178</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">179</Text>
                  <Text className="code-line">{`                              [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"folder"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">180</Text>
                  <Text className="code-line">{`                              [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]View Project[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">181</Text>
                  <Text className="code-line">{`                            [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">182</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">183</Text>
                  <Text className="code-line">{`                              [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"arrow-right"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">184</Text>
                  <Text className="code-line">{`                              [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Share Project[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">185</Text>
                  <Text className="code-line">{`                            [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">186</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]DropdownMenuSeparator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">187</Text>
                  <Text className="code-line">{`                            [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"destructive"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">188</Text>
                  <Text className="code-line">{`                              [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"delete"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">189</Text>
                  <Text className="code-line">{`                              [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Delete Project[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">190</Text>
                  <Text className="code-line">{`                            [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">191</Text>
                  <Text className="code-line">{`                          [color=#7ee787]</[color=#79c0ff]DropdownMenuContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">192</Text>
                  <Text className="code-line">{`                        [color=#7ee787]</[color=#79c0ff]DropdownMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">193</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]SidebarMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">194</Text>
                  <Text className="code-line">{`                    ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">195</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]SidebarMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">196</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]SidebarMenuButton[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"sidebar-more"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">197</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"more-horizontal"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">198</Text>
                  <Text className="code-line">{`                        [color=#7ee787]<[color=#79c0ff]Text[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"sidebar-menu-label"[/color]>[/color]More[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">199</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]SidebarMenuButton[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">200</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]SidebarMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">201</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]SidebarMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">202</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]SidebarGroupContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">203</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]SidebarGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">204</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]ScrollAreaContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">205</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]Scrollbar[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">206</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]ScrollArea[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">207</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]SidebarContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">208</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]SidebarFooter[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">209</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]SidebarMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">210</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]SidebarMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">211</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]DropdownMenu[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"sidebar-user-menu"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">212</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">213</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]SidebarMenuButton[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">214</Text>
                  <Text className="code-line">{`[color=#7ee787]                    [color=#79c0ff]className[/color]=[color=#a5d6ff]"sidebar-user h-11 p-2"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">215</Text>
                  <Text className="code-line">{`[color=#7ee787]                    [color=#79c0ff]data-sidebar-tooltip[/color]=[color=#a5d6ff]"shadcn"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">216</Text>
                  <Text className="code-line">{`[color=#7ee787]                  >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">217</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Widget[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"sidebar-avatar"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">218</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"person-circle"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">219</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">220</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Widget[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"flex flex-1 flex-col gap-0.5 text-left"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">221</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]Text[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"text-sm font-medium"[/color]>[/color]shadcn[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">222</Text>
                  <Text className="code-line">{`                      [color=#7ee787]<[color=#79c0ff]Text[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"text-xs text-muted-foreground"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">223</Text>
                  <Text className="code-line">{`                        m@example.com`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">224</Text>
                  <Text className="code-line">{`                      [color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">225</Text>
                  <Text className="code-line">{`                    [color=#7ee787]</[color=#79c0ff]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">226</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Icon[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">227</Text>
                  <Text className="code-line">{`[color=#7ee787]                      [color=#79c0ff]className[/color]=[color=#a5d6ff]"sidebar-menu-chevron"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">228</Text>
                  <Text className="code-line">{`[color=#7ee787]                      [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-up-down"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">229</Text>
                  <Text className="code-line">{`[color=#7ee787]                    />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">230</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]SidebarMenuButton[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">231</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]DropdownMenuTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">232</Text>
                  <Text className="code-line">{`                [color=#7ee787]<[color=#79c0ff]DropdownMenuContent[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">233</Text>
                  <Text className="code-line">{`[color=#7ee787]                  [color=#79c0ff]className[/color]=[color=#a5d6ff]"w-56"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">234</Text>
                  <Text className="code-line">{`[color=#7ee787]                  [color=#79c0ff]side[/color]=[color=#a5d6ff]"right"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">235</Text>
                  <Text className="code-line">{`[color=#7ee787]                  [color=#79c0ff]align[/color]=[color=#a5d6ff]"end"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">236</Text>
                  <Text className="code-line">{`[color=#7ee787]                >[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">237</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuLabel[/color]>[/color]shadcn[color=#7ee787]</[color=#79c0ff]DropdownMenuLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">238</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]Text[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"px-2 pb-1.5 text-xs text-muted-foreground"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">239</Text>
                  <Text className="code-line">{`                    m@example.com`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">240</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">241</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuSeparator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">242</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">243</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"premium"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">244</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Upgrade to Pro[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">245</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">246</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuSeparator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">247</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">248</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"person"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">249</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Account[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">250</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">251</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">252</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"payment"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">253</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Billing[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">254</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">255</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">256</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"alert"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">257</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Notifications[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">258</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">259</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuSeparator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">260</Text>
                  <Text className="code-line">{`                  [color=#7ee787]<[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">261</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"sign-out"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">262</Text>
                  <Text className="code-line">{`                    [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Sign Out[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">263</Text>
                  <Text className="code-line">{`                  [color=#7ee787]</[color=#79c0ff]DropdownMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">264</Text>
                  <Text className="code-line">{`                [color=#7ee787]</[color=#79c0ff]DropdownMenuContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">265</Text>
                  <Text className="code-line">{`              [color=#7ee787]</[color=#79c0ff]DropdownMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">266</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]SidebarMenuItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">267</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]SidebarMenu[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">268</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]SidebarFooter[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">269</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]SidebarRail[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">270</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]Sidebar[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">271</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]Widget[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"flex flex-1 flex-col gap-2 p-3"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">272</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]SidebarTrigger[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">273</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">274</Text>
                  <Text className="code-line">{`    [color=#7ee787]</[color=#79c0ff]SidebarProvider[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">275</Text>
                  <Text className="code-line">{`  );`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">276</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/sidebar-basic/4" style={{ display: "none" }}>
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
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]sidebar_basic_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
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
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]sidebar_basic_tsx_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
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
