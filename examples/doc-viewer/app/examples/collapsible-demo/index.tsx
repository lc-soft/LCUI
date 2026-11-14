import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function CollapsibleDemoDemo() {
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
        <Widget className="code-block-body active" data-language="c" data-file="main.c" data-source="examples/collapsible-demo/0">
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
                  <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]const[/color] [color=#953800]char[/color] *collapsible_demo_css = [color=#0a3069]"\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`[color=#0a3069].collapsible-demo {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`[color=#0a3069]  display: flex;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`[color=#0a3069]  flex-direction: column;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`[color=#0a3069]  gap: 8px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`[color=#0a3069]  width: 350px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`[color=#0a3069].collapsible-demo .order-header {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`[color=#0a3069]  display: flex;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`[color=#0a3069]  align-items: center;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`[color=#0a3069]  justify-content: flex-start;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`[color=#0a3069]  width: 100%;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`[color=#0a3069]  box-sizing: border-box;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`[color=#0a3069]  gap: 16px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`[color=#0a3069]  padding: 0 16px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`[color=#0a3069].collapsible-demo collapsible-trigger {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`[color=#0a3069]  display: inline-flex;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`[color=#0a3069]  flex-shrink: 0;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`[color=#0a3069].collapsible-demo .order-title {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`[color=#0a3069]  display: block;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`[color=#0a3069]  font-size: 14px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`[color=#0a3069]  font-weight: 600;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`[color=#0a3069].collapsible-demo .order-row {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`[color=#0a3069]  display: flex;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`[color=#0a3069]  align-items: center;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`[color=#0a3069]  justify-content: space-between;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`[color=#0a3069]  box-sizing: border-box;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`[color=#0a3069]  padding: 8px 16px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`[color=#0a3069]  border: 1px solid #e5e5e5;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`[color=#0a3069]  border-radius: 6px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`[color=#0a3069]  font-size: 14px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`[color=#0a3069].collapsible-demo .order-box {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`[color=#0a3069]  display: flex;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`[color=#0a3069]  flex-direction: column;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`[color=#0a3069]  box-sizing: border-box;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`[color=#0a3069]  gap: 4px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`[color=#0a3069]  padding: 8px 16px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`[color=#0a3069]  border: 1px solid #e5e5e5;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`[color=#0a3069]  border-radius: 6px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`[color=#0a3069]  font-size: 14px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`[color=#0a3069].collapsible-demo .order-label {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`[color=#0a3069]  display: block;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`[color=#0a3069]  font-weight: 600;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`[color=#0a3069].collapsible-demo .order-copy {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`[color=#0a3069]  display: block;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`[color=#0a3069]  color: #666666;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`[color=#0a3069]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`[color=#0a3069]"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]collapsible_demo_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *box = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *collapsible = ui_create_collapsible();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *header = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *title = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *trigger = ui_create_collapsible_trigger();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *icon = ui_create_fluent_icon([color=#0a3069]"chevron-down"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *status = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *status_label = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *status_value = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *content = ui_create_collapsible_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *address = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *address_label = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">73</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *address_copy = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">74</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *items = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">75</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *items_label = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">76</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *items_copy = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">77</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">78</Text>
                  <Text className="code-line">{`        ui_load_css_string(collapsible_demo_css, [color=#0a3069]"collapsible-demo"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">79</Text>
                  <Text className="code-line">{`        ui_widget_add_class(box, [color=#0a3069]"collapsible-demo"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">80</Text>
                  <Text className="code-line">{`        ui_widget_add_class(header, [color=#0a3069]"order-header"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">81</Text>
                  <Text className="code-line">{`        ui_widget_add_class(title, [color=#0a3069]"order-title"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">82</Text>
                  <Text className="code-line">{`        ui_text_set_content(title, [color=#0a3069]"Order #4189"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">83</Text>
                  <Text className="code-line">{`        ui_button_set_variant(button, UI_BUTTON_VARIANT_GHOST);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">84</Text>
                  <Text className="code-line">{`        ui_button_set_size(button, UI_BUTTON_SIZE_ICON);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">85</Text>
                  <Text className="code-line">{`        ui_widget_append(button, icon);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">86</Text>
                  <Text className="code-line">{`        ui_widget_append(trigger, button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">87</Text>
                  <Text className="code-line">{`        ui_widget_append(header, title);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">88</Text>
                  <Text className="code-line">{`        ui_widget_append(header, trigger);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">89</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">90</Text>
                  <Text className="code-line">{`        ui_widget_add_class(status, [color=#0a3069]"order-row"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">91</Text>
                  <Text className="code-line">{`        ui_text_set_content(status_label, [color=#0a3069]"Status"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">92</Text>
                  <Text className="code-line">{`        ui_text_set_content(status_value, [color=#0a3069]"Shipped"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">93</Text>
                  <Text className="code-line">{`        ui_widget_add_class(status_label, [color=#0a3069]"order-copy"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">94</Text>
                  <Text className="code-line">{`        ui_widget_append(status, status_label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">95</Text>
                  <Text className="code-line">{`        ui_widget_append(status, status_value);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">96</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">97</Text>
                  <Text className="code-line">{`        ui_widget_add_class(address, [color=#0a3069]"order-box"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">98</Text>
                  <Text className="code-line">{`        ui_text_set_content(address_label, [color=#0a3069]"Shipping address"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">99</Text>
                  <Text className="code-line">{`        ui_widget_add_class(address_label, [color=#0a3069]"order-label"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">100</Text>
                  <Text className="code-line">{`        ui_text_set_content(address_copy, [color=#0a3069]"100 Market St, San Francisco"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">101</Text>
                  <Text className="code-line">{`        ui_widget_add_class(address_copy, [color=#0a3069]"order-copy"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">102</Text>
                  <Text className="code-line">{`        ui_widget_append(address, address_label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">103</Text>
                  <Text className="code-line">{`        ui_widget_append(address, address_copy);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">104</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">105</Text>
                  <Text className="code-line">{`        ui_widget_add_class(items, [color=#0a3069]"order-box"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">106</Text>
                  <Text className="code-line">{`        ui_text_set_content(items_label, [color=#0a3069]"Items"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">107</Text>
                  <Text className="code-line">{`        ui_widget_add_class(items_label, [color=#0a3069]"order-label"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">108</Text>
                  <Text className="code-line">{`        ui_text_set_content(items_copy, [color=#0a3069]"2x Studio Headphones"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">109</Text>
                  <Text className="code-line">{`        ui_widget_add_class(items_copy, [color=#0a3069]"order-copy"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">110</Text>
                  <Text className="code-line">{`        ui_widget_append(items, items_label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">111</Text>
                  <Text className="code-line">{`        ui_widget_append(items, items_copy);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">112</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">113</Text>
                  <Text className="code-line">{`        ui_widget_append(content, address);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">114</Text>
                  <Text className="code-line">{`        ui_widget_append(content, items);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">115</Text>
                  <Text className="code-line">{`        ui_widget_append(collapsible, header);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">116</Text>
                  <Text className="code-line">{`        ui_widget_append(collapsible, status);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">117</Text>
                  <Text className="code-line">{`        ui_widget_append(collapsible, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">118</Text>
                  <Text className="code-line">{`        ui_widget_append(box, collapsible);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">119</Text>
                  <Text className="code-line">{`        ui_widget_append(parent, box);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">120</Text>
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
                  <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]const[/color] [color=#ffa657]char[/color] *collapsible_demo_css = [color=#a5d6ff]"\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`[color=#a5d6ff].collapsible-demo {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  display: flex;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  flex-direction: column;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  gap: 8px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  width: 350px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`[color=#a5d6ff]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`[color=#a5d6ff].collapsible-demo .order-header {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  display: flex;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  align-items: center;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  justify-content: flex-start;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  width: 100%;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  box-sizing: border-box;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  gap: 16px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  padding: 0 16px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`[color=#a5d6ff]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`[color=#a5d6ff].collapsible-demo collapsible-trigger {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  display: inline-flex;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  flex-shrink: 0;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`[color=#a5d6ff]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`[color=#a5d6ff].collapsible-demo .order-title {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  display: block;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  font-size: 14px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  font-weight: 600;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`[color=#a5d6ff]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`[color=#a5d6ff].collapsible-demo .order-row {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  display: flex;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  align-items: center;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  justify-content: space-between;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  box-sizing: border-box;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  padding: 8px 16px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  border: 1px solid #e5e5e5;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  border-radius: 6px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  font-size: 14px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`[color=#a5d6ff]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`[color=#a5d6ff].collapsible-demo .order-box {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  display: flex;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  flex-direction: column;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  box-sizing: border-box;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  gap: 4px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  padding: 8px 16px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  border: 1px solid #e5e5e5;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  border-radius: 6px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  font-size: 14px;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`[color=#a5d6ff]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`[color=#a5d6ff].collapsible-demo .order-label {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  display: block;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  font-weight: 600;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`[color=#a5d6ff]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`[color=#a5d6ff].collapsible-demo .order-copy {\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  display: block;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`[color=#a5d6ff]  color: #666666;\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`[color=#a5d6ff]}\\[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`[color=#a5d6ff]"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]collapsible_demo_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *box = ui_create_widget([color=#79c0ff]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *collapsible = ui_create_collapsible();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *header = ui_create_widget([color=#79c0ff]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *title = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *trigger = ui_create_collapsible_trigger();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *button = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *icon = ui_create_fluent_icon([color=#a5d6ff]"chevron-down"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *status = ui_create_widget([color=#79c0ff]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *status_label = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *status_value = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *content = ui_create_collapsible_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *address = ui_create_widget([color=#79c0ff]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *address_label = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">73</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *address_copy = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">74</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *items = ui_create_widget([color=#79c0ff]NULL[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">75</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *items_label = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">76</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *items_copy = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">77</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">78</Text>
                  <Text className="code-line">{`        ui_load_css_string(collapsible_demo_css, [color=#a5d6ff]"collapsible-demo"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">79</Text>
                  <Text className="code-line">{`        ui_widget_add_class(box, [color=#a5d6ff]"collapsible-demo"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">80</Text>
                  <Text className="code-line">{`        ui_widget_add_class(header, [color=#a5d6ff]"order-header"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">81</Text>
                  <Text className="code-line">{`        ui_widget_add_class(title, [color=#a5d6ff]"order-title"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">82</Text>
                  <Text className="code-line">{`        ui_text_set_content(title, [color=#a5d6ff]"Order #4189"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">83</Text>
                  <Text className="code-line">{`        ui_button_set_variant(button, UI_BUTTON_VARIANT_GHOST);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">84</Text>
                  <Text className="code-line">{`        ui_button_set_size(button, UI_BUTTON_SIZE_ICON);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">85</Text>
                  <Text className="code-line">{`        ui_widget_append(button, icon);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">86</Text>
                  <Text className="code-line">{`        ui_widget_append(trigger, button);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">87</Text>
                  <Text className="code-line">{`        ui_widget_append(header, title);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">88</Text>
                  <Text className="code-line">{`        ui_widget_append(header, trigger);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">89</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">90</Text>
                  <Text className="code-line">{`        ui_widget_add_class(status, [color=#a5d6ff]"order-row"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">91</Text>
                  <Text className="code-line">{`        ui_text_set_content(status_label, [color=#a5d6ff]"Status"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">92</Text>
                  <Text className="code-line">{`        ui_text_set_content(status_value, [color=#a5d6ff]"Shipped"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">93</Text>
                  <Text className="code-line">{`        ui_widget_add_class(status_label, [color=#a5d6ff]"order-copy"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">94</Text>
                  <Text className="code-line">{`        ui_widget_append(status, status_label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">95</Text>
                  <Text className="code-line">{`        ui_widget_append(status, status_value);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">96</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">97</Text>
                  <Text className="code-line">{`        ui_widget_add_class(address, [color=#a5d6ff]"order-box"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">98</Text>
                  <Text className="code-line">{`        ui_text_set_content(address_label, [color=#a5d6ff]"Shipping address"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">99</Text>
                  <Text className="code-line">{`        ui_widget_add_class(address_label, [color=#a5d6ff]"order-label"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">100</Text>
                  <Text className="code-line">{`        ui_text_set_content(address_copy, [color=#a5d6ff]"100 Market St, San Francisco"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">101</Text>
                  <Text className="code-line">{`        ui_widget_add_class(address_copy, [color=#a5d6ff]"order-copy"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">102</Text>
                  <Text className="code-line">{`        ui_widget_append(address, address_label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">103</Text>
                  <Text className="code-line">{`        ui_widget_append(address, address_copy);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">104</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">105</Text>
                  <Text className="code-line">{`        ui_widget_add_class(items, [color=#a5d6ff]"order-box"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">106</Text>
                  <Text className="code-line">{`        ui_text_set_content(items_label, [color=#a5d6ff]"Items"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">107</Text>
                  <Text className="code-line">{`        ui_widget_add_class(items_label, [color=#a5d6ff]"order-label"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">108</Text>
                  <Text className="code-line">{`        ui_text_set_content(items_copy, [color=#a5d6ff]"2x Studio Headphones"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">109</Text>
                  <Text className="code-line">{`        ui_widget_add_class(items_copy, [color=#a5d6ff]"order-copy"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">110</Text>
                  <Text className="code-line">{`        ui_widget_append(items, items_label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">111</Text>
                  <Text className="code-line">{`        ui_widget_append(items, items_copy);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">112</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">113</Text>
                  <Text className="code-line">{`        ui_widget_append(content, address);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">114</Text>
                  <Text className="code-line">{`        ui_widget_append(content, items);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">115</Text>
                  <Text className="code-line">{`        ui_widget_append(collapsible, header);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">116</Text>
                  <Text className="code-line">{`        ui_widget_append(collapsible, status);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">117</Text>
                  <Text className="code-line">{`        ui_widget_append(collapsible, content);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">118</Text>
                  <Text className="code-line">{`        ui_widget_append(box, collapsible);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">119</Text>
                  <Text className="code-line">{`        ui_widget_append(parent, box);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">120</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/collapsible-demo/1" style={{ display: "none" }}>
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
                  <Text className="code-line">{`    .collapsible-demo {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`      display: flex;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`      flex-direction: column;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`      gap: 8px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`      width: 350px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`    .collapsible-demo .order-header {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`      display: flex;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`      align-items: center;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`      justify-content: flex-start;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`      width: 100%;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`      box-sizing: border-box;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`      gap: 16px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`      padding: 0 16px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`    .collapsible-demo collapsible-trigger {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`      display: inline-flex;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`      flex-shrink: 0;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`    .collapsible-demo .order-title {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`      display: block;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`      font-size: 14px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`      font-weight: 600;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`    .collapsible-demo .order-row {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`      display: flex;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`      align-items: center;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`      justify-content: space-between;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`      box-sizing: border-box;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`      padding: 8px 16px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`      border: 1px solid #e5e5e5;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`      border-radius: 6px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`      font-size: 14px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`    .collapsible-demo .order-box {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`      display: flex;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`      flex-direction: column;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`      box-sizing: border-box;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`      gap: 4px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`      padding: 8px 16px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`      border: 1px solid #e5e5e5;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`      border-radius: 6px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`      font-size: 14px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`    .collapsible-demo .order-label {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`      display: block;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`      font-weight: 600;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`    .collapsible-demo .order-copy {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`      display: block;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`      color: #666666;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]resource[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]widget[/color] [color=#0550ae]class[/color]=[color=#0a3069]"collapsible-demo"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"order-header"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"order-title"[/color]>[/color]Order #4189[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"icon"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`              [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-up-down"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"order-row"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"order-copy"[/color]>[/color]Status[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]text[/color]>[/color]Shipped[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">73</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"order-box"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">74</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"order-label"[/color]>[/color]Shipping address[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">75</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"order-copy"[/color]>[/color]100 Market St, San Francisco[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">76</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">77</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"order-box"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">78</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"order-label"[/color]>[/color]Items[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">79</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"order-copy"[/color]>[/color]2x Studio Headphones[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">80</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">81</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">82</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">83</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">84</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">85</Text>
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
                  <Text className="code-line">{`    .collapsible-demo {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`      display: flex;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`      flex-direction: column;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`      gap: 8px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`      width: 350px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`    .collapsible-demo .order-header {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`      display: flex;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`      align-items: center;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`      justify-content: flex-start;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`      width: 100%;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`      box-sizing: border-box;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`      gap: 16px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`      padding: 0 16px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`    .collapsible-demo collapsible-trigger {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`      display: inline-flex;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`      flex-shrink: 0;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`    .collapsible-demo .order-title {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`      display: block;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`      font-size: 14px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`      font-weight: 600;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`    .collapsible-demo .order-row {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`      display: flex;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`      align-items: center;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`      justify-content: space-between;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`      box-sizing: border-box;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`      padding: 8px 16px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`      border: 1px solid #e5e5e5;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`      border-radius: 6px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`      font-size: 14px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`    .collapsible-demo .order-box {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`      display: flex;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`      flex-direction: column;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`      box-sizing: border-box;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`      gap: 4px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`      padding: 8px 16px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`      border: 1px solid #e5e5e5;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`      border-radius: 6px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`      font-size: 14px;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`    .collapsible-demo .order-label {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`      display: block;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`      font-weight: 600;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`    .collapsible-demo .order-copy {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`      display: block;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`      color: #666666;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`    }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]resource[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]ui[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]widget[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"collapsible-demo"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]div[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"order-header"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"order-title"[/color]>[/color]Order #4189[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]button[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"ghost"[/color] [color=#79c0ff]size[/color]=[color=#a5d6ff]"icon"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`              [color=#7ee787]<[color=#79c0ff]icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-up-down"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]collapsible-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]div[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"order-row"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"order-copy"[/color]>[/color]Status[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]text[/color]>[/color]Shipped[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">73</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]div[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"order-box"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">74</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"order-label"[/color]>[/color]Shipping address[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">75</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"order-copy"[/color]>[/color]100 Market St, San Francisco[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">76</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">77</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]div[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"order-box"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">78</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"order-label"[/color]>[/color]Items[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">79</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]text[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"order-copy"[/color]>[/color]2x Studio Headphones[color=#7ee787]</[color=#79c0ff]text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">80</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]div[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">81</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]collapsible-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">82</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">83</Text>
                  <Text className="code-line">{`    [color=#7ee787]</[color=#79c0ff]widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">84</Text>
                  <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]ui[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">85</Text>
                  <Text className="code-line">{`[color=#7ee787]</[color=#79c0ff]lcui-app[/color]>[/color]`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/collapsible-demo/2" style={{ display: "none" }}>
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
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]collapsible_demo_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
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
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]collapsible_demo_xml_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
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
        <Widget className="code-block-body" data-language="tsx" data-file="example.tsx" data-source="examples/collapsible-demo/3" style={{ display: "none" }}>
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
                  <Text className="code-line">{`  [color=#8250df]Collapsible[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#8250df]CollapsibleContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#8250df]CollapsibleTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#8250df]Icon[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#8250df]Text[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Collapsible[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex w-\\[350px] flex-col gap-2"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex items-center justify-between gap-4 px-4"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"text-sm font-semibold"[/color]>[/color]Order #4189[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"ghost"[/color] [color=#0550ae]size[/color]=[color=#0a3069]"icon"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"size-8"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"chevron-up-down"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex items-center justify-between rounded-md border px-4 py-2 text-sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"text-muted-foreground"[/color]>[/color]Status[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"font-medium"[/color]>[/color]Shipped[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]CollapsibleContent[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-col gap-2"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-col gap-1 rounded-md border px-4 py-2 text-sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"font-medium"[/color]>[/color]Shipping address[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"text-muted-foreground"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`            100 Market St, San Francisco`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-col gap-1 rounded-md border px-4 py-2 text-sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"font-medium"[/color]>[/color]Items[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Text[/color] [color=#0550ae]className[/color]=[color=#0a3069]"text-muted-foreground"[/color]>[/color]2x Studio Headphones[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`  );`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
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
                  <Text className="code-line">{`  [color=#d2a8ff]Collapsible[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]CollapsibleContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]CollapsibleTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]Icon[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]Text[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]Widget[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`} [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`[color=#ff7b72]export[/color] [color=#ff7b72]default[/color] [color=#ff7b72]function[/color] [color=#d2a8ff]App[/color]([color=#79c0ff][/color]) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`  [color=#ff7b72]return[/color] (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]Collapsible[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"flex w-\\[350px] flex-col gap-2"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]Widget[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"flex items-center justify-between gap-4 px-4"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]Text[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"text-sm font-semibold"[/color]>[/color]Order #4189[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]Button[/color] [color=#79c0ff]variant[/color]=[color=#a5d6ff]"ghost"[/color] [color=#79c0ff]size[/color]=[color=#a5d6ff]"icon"[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"size-8"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]Icon[/color] [color=#79c0ff]name[/color]=[color=#a5d6ff]"chevron-up-down"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]Button[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]CollapsibleTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]Widget[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"flex items-center justify-between rounded-md border px-4 py-2 text-sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]Text[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"text-muted-foreground"[/color]>[/color]Status[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]Text[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"font-medium"[/color]>[/color]Shipped[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]CollapsibleContent[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"flex flex-col gap-2"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]Widget[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"flex flex-col gap-1 rounded-md border px-4 py-2 text-sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]Text[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"font-medium"[/color]>[/color]Shipping address[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]Text[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"text-muted-foreground"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`            100 Market St, San Francisco`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]Widget[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"flex flex-col gap-1 rounded-md border px-4 py-2 text-sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]Text[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"font-medium"[/color]>[/color]Items[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]Text[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"text-muted-foreground"[/color]>[/color]2x Studio Headphones[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]Widget[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]CollapsibleContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`    [color=#7ee787]</[color=#79c0ff]Collapsible[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`  );`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/collapsible-demo/4" style={{ display: "none" }}>
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
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]collapsible_demo_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
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
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]collapsible_demo_tsx_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
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
