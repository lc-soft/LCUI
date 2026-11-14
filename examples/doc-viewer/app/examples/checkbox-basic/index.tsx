import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function CheckboxBasicDemo() {
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
        <Widget className="code-block-body active" data-language="c" data-file="main.c" data-source="examples/checkbox-basic/0">
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
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]checkbox_basic_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *group = ui_create_field_group();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *card;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *fields\\[[color=#0550ae]4[/color]];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *boxes\\[[color=#0550ae]4[/color]];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *contents\\[[color=#0550ae]2[/color]];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *labels\\[[color=#0550ae]3[/color]];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`        [color=#953800]const[/color] [color=#953800]char[/color] *ids\\[[color=#0550ae]4[/color]] = { [color=#0a3069]"terms-checkbox"[/color], [color=#0a3069]"terms-checkbox-2"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`                               [color=#0a3069]"toggle-checkbox"[/color], [color=#0a3069]"toggle-checkbox-2"[/color] };`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`        [color=#953800]int[/color] i;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`        ui_widget_add_class(group, [color=#0a3069]"checkbox-basic-demo"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        [color=#cf222e]for[/color] (i = [color=#0550ae]0[/color]; i < [color=#0550ae]4[/color]; ++i) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`                fields\\[i] = ui_create_field();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`                ui_field_set_orientation(fields\\[i], [color=#0a3069]"horizontal"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`                boxes\\[i] = ui_create_checkbox();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`                ui_widget_set_id(boxes\\[i], ids\\[i]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`                ui_widget_append(fields\\[i], boxes\\[i]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`        }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(boxes\\[[color=#0550ae]1[/color]], [color=#0a3069]"checked"[/color], [color=#0a3069]"true"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`        ui_widget_set_disabled(boxes\\[[color=#0550ae]2[/color]], [color=#0550ae]true[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`        ui_widget_set_disabled(fields\\[[color=#0550ae]2[/color]], [color=#0550ae]true[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`        labels\\[[color=#0550ae]0[/color]] = ui_create_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`        ui_label_set_for(labels\\[[color=#0550ae]0[/color]], ids\\[[color=#0550ae]0[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`        ui_text_set_content(labels\\[[color=#0550ae]0[/color]], [color=#0a3069]"Accept terms and conditions"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`        ui_widget_append(fields\\[[color=#0550ae]0[/color]], labels\\[[color=#0550ae]0[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`        ui_widget_append(group, fields\\[[color=#0550ae]0[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`        contents\\[[color=#0550ae]0[/color]] = ui_create_field_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`        labels\\[[color=#0550ae]1[/color]] = ui_create_field_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`        ui_label_set_for(labels\\[[color=#0550ae]1[/color]], ids\\[[color=#0550ae]1[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`        ui_text_set_content(labels\\[[color=#0550ae]1[/color]], [color=#0a3069]"Accept terms and conditions"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`        ui_widget_append(contents\\[[color=#0550ae]0[/color]], labels\\[[color=#0550ae]1[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`        ui_widget_append(contents\\[[color=#0550ae]0[/color]], ui_create_field_description());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`        ui_text_set_content(`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`            ui_widget_get_child(contents\\[[color=#0550ae]0[/color]], [color=#0550ae]1[/color]),`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`            [color=#0a3069]"By clicking this checkbox, you agree to the terms."[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`        ui_widget_append(fields\\[[color=#0550ae]1[/color]], contents\\[[color=#0550ae]0[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`        ui_widget_append(group, fields\\[[color=#0550ae]1[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`        labels\\[[color=#0550ae]2[/color]] = ui_create_field_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`        ui_label_set_for(labels\\[[color=#0550ae]2[/color]], ids\\[[color=#0550ae]2[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`        ui_text_set_content(labels\\[[color=#0550ae]2[/color]], [color=#0a3069]"Enable notifications"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`        ui_widget_append(fields\\[[color=#0550ae]2[/color]], labels\\[[color=#0550ae]2[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`        ui_widget_append(group, fields\\[[color=#0550ae]2[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`        contents\\[[color=#0550ae]1[/color]] = ui_create_field_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`        ui_widget_append(contents\\[[color=#0550ae]1[/color]], ui_create_field_title());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`        ui_text_set_content(ui_widget_get_child(contents\\[[color=#0550ae]1[/color]], [color=#0550ae]0[/color]),`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`                            [color=#0a3069]"Enable notifications"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`        ui_widget_append(contents\\[[color=#0550ae]1[/color]], ui_create_field_description());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`        ui_text_set_content(`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`            ui_widget_get_child(contents\\[[color=#0550ae]1[/color]], [color=#0550ae]1[/color]),`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`            [color=#0a3069]"You can enable and disable notifications at any time."[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`        ui_widget_append(fields\\[[color=#0550ae]3[/color]], contents\\[[color=#0550ae]1[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`        [color=#6e7781]/* The whole card is clickable: FieldLabel wraps the Field and targets[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`[color=#6e7781]         * the checkbox through \`for\`. */[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`        card = ui_create_field_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`        ui_label_set_for(card, ids\\[[color=#0550ae]3[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`        ui_widget_append(card, fields\\[[color=#0550ae]3[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`        ui_widget_append(group, card);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`        ui_widget_append(parent, group);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
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
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]checkbox_basic_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *group = ui_create_field_group();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *card;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *fields\\[[color=#79c0ff]4[/color]];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *boxes\\[[color=#79c0ff]4[/color]];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *contents\\[[color=#79c0ff]2[/color]];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *labels\\[[color=#79c0ff]3[/color]];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`        [color=#ffa657]const[/color] [color=#ffa657]char[/color] *ids\\[[color=#79c0ff]4[/color]] = { [color=#a5d6ff]"terms-checkbox"[/color], [color=#a5d6ff]"terms-checkbox-2"[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`                               [color=#a5d6ff]"toggle-checkbox"[/color], [color=#a5d6ff]"toggle-checkbox-2"[/color] };`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`        [color=#ffa657]int[/color] i;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`        ui_widget_add_class(group, [color=#a5d6ff]"checkbox-basic-demo"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        [color=#ff7b72]for[/color] (i = [color=#79c0ff]0[/color]; i < [color=#79c0ff]4[/color]; ++i) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`                fields\\[i] = ui_create_field();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`                ui_field_set_orientation(fields\\[i], [color=#a5d6ff]"horizontal"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`                boxes\\[i] = ui_create_checkbox();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`                ui_widget_set_id(boxes\\[i], ids\\[i]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`                ui_widget_append(fields\\[i], boxes\\[i]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`        }`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`        ui_widget_set_attr(boxes\\[[color=#79c0ff]1[/color]], [color=#a5d6ff]"checked"[/color], [color=#a5d6ff]"true"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`        ui_widget_set_disabled(boxes\\[[color=#79c0ff]2[/color]], [color=#79c0ff]true[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`        ui_widget_set_disabled(fields\\[[color=#79c0ff]2[/color]], [color=#79c0ff]true[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`        labels\\[[color=#79c0ff]0[/color]] = ui_create_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`        ui_label_set_for(labels\\[[color=#79c0ff]0[/color]], ids\\[[color=#79c0ff]0[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`        ui_text_set_content(labels\\[[color=#79c0ff]0[/color]], [color=#a5d6ff]"Accept terms and conditions"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`        ui_widget_append(fields\\[[color=#79c0ff]0[/color]], labels\\[[color=#79c0ff]0[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`        ui_widget_append(group, fields\\[[color=#79c0ff]0[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`        contents\\[[color=#79c0ff]0[/color]] = ui_create_field_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`        labels\\[[color=#79c0ff]1[/color]] = ui_create_field_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`        ui_label_set_for(labels\\[[color=#79c0ff]1[/color]], ids\\[[color=#79c0ff]1[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`        ui_text_set_content(labels\\[[color=#79c0ff]1[/color]], [color=#a5d6ff]"Accept terms and conditions"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`        ui_widget_append(contents\\[[color=#79c0ff]0[/color]], labels\\[[color=#79c0ff]1[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`        ui_widget_append(contents\\[[color=#79c0ff]0[/color]], ui_create_field_description());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`        ui_text_set_content(`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`            ui_widget_get_child(contents\\[[color=#79c0ff]0[/color]], [color=#79c0ff]1[/color]),`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`            [color=#a5d6ff]"By clicking this checkbox, you agree to the terms."[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`        ui_widget_append(fields\\[[color=#79c0ff]1[/color]], contents\\[[color=#79c0ff]0[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`        ui_widget_append(group, fields\\[[color=#79c0ff]1[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`        labels\\[[color=#79c0ff]2[/color]] = ui_create_field_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`        ui_label_set_for(labels\\[[color=#79c0ff]2[/color]], ids\\[[color=#79c0ff]2[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`        ui_text_set_content(labels\\[[color=#79c0ff]2[/color]], [color=#a5d6ff]"Enable notifications"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`        ui_widget_append(fields\\[[color=#79c0ff]2[/color]], labels\\[[color=#79c0ff]2[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`        ui_widget_append(group, fields\\[[color=#79c0ff]2[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`        contents\\[[color=#79c0ff]1[/color]] = ui_create_field_content();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`        ui_widget_append(contents\\[[color=#79c0ff]1[/color]], ui_create_field_title());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`        ui_text_set_content(ui_widget_get_child(contents\\[[color=#79c0ff]1[/color]], [color=#79c0ff]0[/color]),`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`                            [color=#a5d6ff]"Enable notifications"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`        ui_widget_append(contents\\[[color=#79c0ff]1[/color]], ui_create_field_description());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`        ui_text_set_content(`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`            ui_widget_get_child(contents\\[[color=#79c0ff]1[/color]], [color=#79c0ff]1[/color]),`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`            [color=#a5d6ff]"You can enable and disable notifications at any time."[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`        ui_widget_append(fields\\[[color=#79c0ff]3[/color]], contents\\[[color=#79c0ff]1[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`        [color=#8b949e]/* The whole card is clickable: FieldLabel wraps the Field and targets[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`[color=#8b949e]         * the checkbox through \`for\`. */[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`        card = ui_create_field_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`        ui_label_set_for(card, ids\\[[color=#79c0ff]3[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`        ui_widget_append(card, fields\\[[color=#79c0ff]3[/color]]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`        ui_widget_append(group, card);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`        ui_widget_append(parent, group);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/checkbox-basic/1" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-block-lines code-block-lines-light">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]field-group[/color] [color=#0550ae]class[/color]=[color=#0a3069]"checkbox-basic-demo"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"terms-checkbox"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"terms-checkbox"[/color]>[/color]Accept terms and conditions[color=#116329]</[color=#0550ae]label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"terms-checkbox-2"[/color] [color=#0550ae]checked[/color]=[color=#0a3069]"true"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field-label[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`[color=#116329]            [color=#0550ae]for[/color]=[color=#0a3069]"terms-checkbox-2"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`[color=#116329]          >[/color]Accept terms and conditions[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field-description[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`[color=#116329]          >[/color]By clicking this checkbox, you agree to the terms.[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color] [color=#0550ae]disabled[/color]=[color=#0a3069]"true"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"toggle-checkbox"[/color] [color=#0550ae]disabled[/color]=[color=#0a3069]"true"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"toggle-checkbox"[/color]>[/color]Enable notifications[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"toggle-checkbox-2"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"toggle-checkbox-2"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-title[/color]>[/color]Enable notifications[color=#116329]</[color=#0550ae]field-title[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-description[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`[color=#116329]            >[/color]You can enable and disable notifications at any time.[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]field-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
                </Widget>
              </Widget>
              <Widget className="code-block-lines code-block-lines-dark">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#7ee787]<[color=#79c0ff]lcui-app[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]ui[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]field-group[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"checkbox-basic-demo"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]field[/color] [color=#79c0ff]orientation[/color]=[color=#a5d6ff]"horizontal"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]checkbox[/color] [color=#79c0ff]id[/color]=[color=#a5d6ff]"terms-checkbox"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]label[/color] [color=#79c0ff]for[/color]=[color=#a5d6ff]"terms-checkbox"[/color]>[/color]Accept terms and conditions[color=#7ee787]</[color=#79c0ff]label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]field[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]field[/color] [color=#79c0ff]orientation[/color]=[color=#a5d6ff]"horizontal"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]checkbox[/color] [color=#79c0ff]id[/color]=[color=#a5d6ff]"terms-checkbox-2"[/color] [color=#79c0ff]checked[/color]=[color=#a5d6ff]"true"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]field-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]field-label[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`[color=#7ee787]            [color=#79c0ff]for[/color]=[color=#a5d6ff]"terms-checkbox-2"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`[color=#7ee787]          >[/color]Accept terms and conditions[color=#7ee787]</[color=#79c0ff]field-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]field-description[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`[color=#7ee787]          >[/color]By clicking this checkbox, you agree to the terms.[color=#7ee787]</[color=#79c0ff]field-description[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]field-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]field[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]field[/color] [color=#79c0ff]orientation[/color]=[color=#a5d6ff]"horizontal"[/color] [color=#79c0ff]disabled[/color]=[color=#a5d6ff]"true"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]checkbox[/color] [color=#79c0ff]id[/color]=[color=#a5d6ff]"toggle-checkbox"[/color] [color=#79c0ff]disabled[/color]=[color=#a5d6ff]"true"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]field-label[/color] [color=#79c0ff]for[/color]=[color=#a5d6ff]"toggle-checkbox"[/color]>[/color]Enable notifications[color=#7ee787]</[color=#79c0ff]field-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]field[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]field-label[/color] [color=#79c0ff]for[/color]=[color=#a5d6ff]"toggle-checkbox-2"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]field[/color] [color=#79c0ff]orientation[/color]=[color=#a5d6ff]"horizontal"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]checkbox[/color] [color=#79c0ff]id[/color]=[color=#a5d6ff]"toggle-checkbox-2"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]field-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]field-title[/color]>[/color]Enable notifications[color=#7ee787]</[color=#79c0ff]field-title[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]field-description[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`[color=#7ee787]            >[/color]You can enable and disable notifications at any time.[color=#7ee787]</[color=#79c0ff]field-description[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]field-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]field[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]field-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`    [color=#7ee787]</[color=#79c0ff]field-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]ui[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`[color=#7ee787]</[color=#79c0ff]lcui-app[/color]>[/color]`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/checkbox-basic/2" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "184px" }}>
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
                  <Text className="code-line">{`[color=#ffa657]int[/color] [color=#d2a8ff]main[/color][color=#79c0ff]([color=#ffa657]void[/color])[/color]`}</Text>
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
                  <Text className="code-line">{`        ui_load_xml_file([color=#a5d6ff]"ui.xml"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`        [color=#ff7b72]return[/color] lcui_main();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="example.tsx" data-source="examples/checkbox-basic/3" style={{ display: "none" }}>
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
                  <Text className="code-line">{`  [color=#8250df]Checkbox[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#8250df]Field[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#8250df]FieldContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#8250df]FieldDescription[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#8250df]FieldGroup[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#8250df]FieldLabel[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#8250df]FieldTitle[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`  [color=#8250df]Label[/color],`}</Text>
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
                  <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]FieldGroup[/color] [color=#0550ae]className[/color]=[color=#0a3069]"max-w-sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"terms-checkbox"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"terms-checkbox"[/color]>[/color]Accept terms and conditions[color=#116329]</[color=#0550ae]Label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"terms-checkbox-2"[/color] [color=#0550ae]checked[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]for[/color]=[color=#0a3069]"terms-checkbox-2"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`            Accept terms and conditions`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`            By clicking this checkbox, you agree to the terms.`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color] [color=#0550ae]disabled[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"toggle-checkbox"[/color] [color=#0550ae]disabled[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]for[/color]=[color=#0a3069]"toggle-checkbox"[/color]>[/color]Enable notifications[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]for[/color]=[color=#0a3069]"toggle-checkbox-2"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"toggle-checkbox-2"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]FieldContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldTitle[/color]>[/color]Enable notifications[color=#116329]</[color=#0550ae]FieldTitle[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`              You can enable and disable notifications at any time.`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]FieldContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`  );`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
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
                  <Text className="code-line">{`  [color=#d2a8ff]Checkbox[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]Field[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]FieldContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]FieldDescription[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]FieldGroup[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]FieldLabel[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]FieldTitle[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]Label[/color],`}</Text>
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
                  <Text className="code-line">{`[color=#ff7b72]export[/color] [color=#ff7b72]default[/color] [color=#ff7b72]function[/color] [color=#d2a8ff]App[/color]([color=#79c0ff][/color]) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`  [color=#ff7b72]return[/color] (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]FieldGroup[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"max-w-sm"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]Field[/color] [color=#79c0ff]orientation[/color]=[color=#a5d6ff]"horizontal"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]Checkbox[/color] [color=#79c0ff]id[/color]=[color=#a5d6ff]"terms-checkbox"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]Label[/color] [color=#79c0ff]for[/color]=[color=#a5d6ff]"terms-checkbox"[/color]>[/color]Accept terms and conditions[color=#7ee787]</[color=#79c0ff]Label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]Field[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]Field[/color] [color=#79c0ff]orientation[/color]=[color=#a5d6ff]"horizontal"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]Checkbox[/color] [color=#79c0ff]id[/color]=[color=#a5d6ff]"terms-checkbox-2"[/color] [color=#79c0ff]checked[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]FieldContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]FieldLabel[/color] [color=#79c0ff]for[/color]=[color=#a5d6ff]"terms-checkbox-2"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`            Accept terms and conditions`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]FieldLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]FieldDescription[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`            By clicking this checkbox, you agree to the terms.`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]FieldDescription[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]FieldContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]Field[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]Field[/color] [color=#79c0ff]orientation[/color]=[color=#a5d6ff]"horizontal"[/color] [color=#79c0ff]disabled[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]Checkbox[/color] [color=#79c0ff]id[/color]=[color=#a5d6ff]"toggle-checkbox"[/color] [color=#79c0ff]disabled[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]FieldLabel[/color] [color=#79c0ff]for[/color]=[color=#a5d6ff]"toggle-checkbox"[/color]>[/color]Enable notifications[color=#7ee787]</[color=#79c0ff]FieldLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]Field[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]FieldLabel[/color] [color=#79c0ff]for[/color]=[color=#a5d6ff]"toggle-checkbox-2"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]Field[/color] [color=#79c0ff]orientation[/color]=[color=#a5d6ff]"horizontal"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]Checkbox[/color] [color=#79c0ff]id[/color]=[color=#a5d6ff]"toggle-checkbox-2"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]FieldContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]FieldTitle[/color]>[/color]Enable notifications[color=#7ee787]</[color=#79c0ff]FieldTitle[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]FieldDescription[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`              You can enable and disable notifications at any time.`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]FieldDescription[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]FieldContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]Field[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]FieldLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`    [color=#7ee787]</[color=#79c0ff]FieldGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`  );`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/checkbox-basic/4" style={{ display: "none" }}>
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
                  <Text className="code-line">{`[color=#8b949e]/* example.tsx is compiled to example.h by @lcui/cli */[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`[color=#8b949e]#[color=#ff7b72]include[/color] [color=#a5d6ff]"example.h"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`[color=#ffa657]int[/color] [color=#d2a8ff]main[/color][color=#79c0ff]([color=#ffa657]void[/color])[/color]`}</Text>
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
                  <Text className="code-line">{`        [color=#ff7b72]return[/color] lcui_main();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
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
