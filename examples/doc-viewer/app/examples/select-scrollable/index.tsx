import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function SelectScrollableDemo() {
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
        <Widget className="code-block-body active" data-language="c" data-file="main.c" data-source="examples/select-scrollable/0">
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
                  <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]add_item[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *group, [color=#953800]const[/color] [color=#953800]char[/color] *label, [color=#953800]const[/color] [color=#953800]char[/color] *value)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *item = ui_create_select_item(value);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`        ui_text_set_content(item, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`        ui_widget_append(group, item);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]add_label[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *group, [color=#953800]const[/color] [color=#953800]char[/color] *text)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *label = ui_create_select_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`        ui_text_set_content(label, text);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        ui_widget_append(group, label);`}</Text>
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
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]select_scrollable_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *select = ui_create_select();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *group;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`        ui_widget_add_class(select, [color=#0a3069]"select-scrollable-demo"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`        ui_select_set_placeholder(select, [color=#0a3069]"Select a timezone"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`        ui_widget_append(select, ui_create_select_trigger());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`        ui_widget_append(select, ui_create_select_content());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`        group = ui_create_select_group();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`        add_label(group, [color=#0a3069]"North America"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`        add_item(group, [color=#0a3069]"Eastern Standard Time"[/color], [color=#0a3069]"est"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`        add_item(group, [color=#0a3069]"Central Standard Time"[/color], [color=#0a3069]"cst"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`        add_item(group, [color=#0a3069]"Mountain Standard Time"[/color], [color=#0a3069]"mst"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`        add_item(group, [color=#0a3069]"Pacific Standard Time"[/color], [color=#0a3069]"pst"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`        add_item(group, [color=#0a3069]"Alaska Standard Time"[/color], [color=#0a3069]"akst"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`        add_item(group, [color=#0a3069]"Hawaii Standard Time"[/color], [color=#0a3069]"hst"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`        ui_select_append_item(select, group);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`        group = ui_create_select_group();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`        add_label(group, [color=#0a3069]"Europe & Africa"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`        add_item(group, [color=#0a3069]"Greenwich Mean Time"[/color], [color=#0a3069]"gmt"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`        add_item(group, [color=#0a3069]"Central European Time"[/color], [color=#0a3069]"cet"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`        add_item(group, [color=#0a3069]"Eastern European Time"[/color], [color=#0a3069]"eet"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`        add_item(group, [color=#0a3069]"Western European Summer Time"[/color], [color=#0a3069]"west"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`        add_item(group, [color=#0a3069]"Central Africa Time"[/color], [color=#0a3069]"cat"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`        add_item(group, [color=#0a3069]"East Africa Time"[/color], [color=#0a3069]"eat"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`        ui_select_append_item(select, group);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`        ui_select_append_item(select, ui_create_select_separator());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`        group = ui_create_select_group();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`        add_label(group, [color=#0a3069]"Asia"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`        add_item(group, [color=#0a3069]"Moscow Time"[/color], [color=#0a3069]"msk"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`        add_item(group, [color=#0a3069]"India Standard Time"[/color], [color=#0a3069]"ist"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`        add_item(group, [color=#0a3069]"China Standard Time"[/color], [color=#0a3069]"cst_china"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`        add_item(group, [color=#0a3069]"Japan Standard Time"[/color], [color=#0a3069]"jst"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`        add_item(group, [color=#0a3069]"Korea Standard Time"[/color], [color=#0a3069]"kst"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`        add_item(group, [color=#0a3069]"Indonesia Central Standard Time"[/color], [color=#0a3069]"ist_indonesia"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`        ui_select_append_item(select, group);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`        ui_widget_append(parent, select);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
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
                  <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]void[/color] [color=#d2a8ff]add_item[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *group, [color=#ffa657]const[/color] [color=#ffa657]char[/color] *label, [color=#ffa657]const[/color] [color=#ffa657]char[/color] *value)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *item = ui_create_select_item(value);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`        ui_text_set_content(item, label);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`        ui_widget_append(group, item);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]void[/color] [color=#d2a8ff]add_label[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *group, [color=#ffa657]const[/color] [color=#ffa657]char[/color] *text)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *label = ui_create_select_label();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`        ui_text_set_content(label, text);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        ui_widget_append(group, label);`}</Text>
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
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]select_scrollable_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *select = ui_create_select();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *group;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`        ui_widget_add_class(select, [color=#a5d6ff]"select-scrollable-demo"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`        ui_select_set_placeholder(select, [color=#a5d6ff]"Select a timezone"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`        ui_widget_append(select, ui_create_select_trigger());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`        ui_widget_append(select, ui_create_select_content());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`        group = ui_create_select_group();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`        add_label(group, [color=#a5d6ff]"North America"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`        add_item(group, [color=#a5d6ff]"Eastern Standard Time"[/color], [color=#a5d6ff]"est"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`        add_item(group, [color=#a5d6ff]"Central Standard Time"[/color], [color=#a5d6ff]"cst"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`        add_item(group, [color=#a5d6ff]"Mountain Standard Time"[/color], [color=#a5d6ff]"mst"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`        add_item(group, [color=#a5d6ff]"Pacific Standard Time"[/color], [color=#a5d6ff]"pst"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`        add_item(group, [color=#a5d6ff]"Alaska Standard Time"[/color], [color=#a5d6ff]"akst"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`        add_item(group, [color=#a5d6ff]"Hawaii Standard Time"[/color], [color=#a5d6ff]"hst"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`        ui_select_append_item(select, group);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`        group = ui_create_select_group();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`        add_label(group, [color=#a5d6ff]"Europe & Africa"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`        add_item(group, [color=#a5d6ff]"Greenwich Mean Time"[/color], [color=#a5d6ff]"gmt"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`        add_item(group, [color=#a5d6ff]"Central European Time"[/color], [color=#a5d6ff]"cet"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`        add_item(group, [color=#a5d6ff]"Eastern European Time"[/color], [color=#a5d6ff]"eet"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`        add_item(group, [color=#a5d6ff]"Western European Summer Time"[/color], [color=#a5d6ff]"west"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`        add_item(group, [color=#a5d6ff]"Central Africa Time"[/color], [color=#a5d6ff]"cat"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`        add_item(group, [color=#a5d6ff]"East Africa Time"[/color], [color=#a5d6ff]"eat"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`        ui_select_append_item(select, group);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`        ui_select_append_item(select, ui_create_select_separator());`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`        group = ui_create_select_group();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`        add_label(group, [color=#a5d6ff]"Asia"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`        add_item(group, [color=#a5d6ff]"Moscow Time"[/color], [color=#a5d6ff]"msk"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`        add_item(group, [color=#a5d6ff]"India Standard Time"[/color], [color=#a5d6ff]"ist"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`        add_item(group, [color=#a5d6ff]"China Standard Time"[/color], [color=#a5d6ff]"cst_china"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`        add_item(group, [color=#a5d6ff]"Japan Standard Time"[/color], [color=#a5d6ff]"jst"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`        add_item(group, [color=#a5d6ff]"Korea Standard Time"[/color], [color=#a5d6ff]"kst"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`        add_item(group, [color=#a5d6ff]"Indonesia Central Standard Time"[/color], [color=#a5d6ff]"ist_indonesia"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`        ui_select_append_item(select, group);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`        ui_widget_append(parent, select);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/select-scrollable/1" style={{ display: "none" }}>
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
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]select[/color] [color=#0550ae]class[/color]=[color=#0a3069]"select-scrollable-demo"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]select-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]select-value[/color] [color=#0550ae]data-select-placeholder[/color]=[color=#0a3069]"Select a timezone"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]select-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]portal[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]select-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]select-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-label[/color]>[/color]North America[color=#116329]</[color=#0550ae]select-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"est"[/color]>[/color]Eastern Standard Time[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"cst"[/color]>[/color]Central Standard Time[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"mst"[/color]>[/color]Mountain Standard Time[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"pst"[/color]>[/color]Pacific Standard Time[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"akst"[/color]>[/color]Alaska Standard Time[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"hst"[/color]>[/color]Hawaii Standard Time[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]select-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]select-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-label[/color]>[/color]Europe [color=#0550ae]&amp;[/color] Africa[color=#116329]</[color=#0550ae]select-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"gmt"[/color]>[/color]Greenwich Mean Time[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"cet"[/color]>[/color]Central European Time[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"eet"[/color]>[/color]Eastern European Time[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"west"[/color]>[/color]Western European Summer Time[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"cat"[/color]>[/color]Central Africa Time[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"eat"[/color]>[/color]East Africa Time[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]select-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]select-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]select-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-label[/color]>[/color]Asia[color=#116329]</[color=#0550ae]select-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"msk"[/color]>[/color]Moscow Time[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"ist"[/color]>[/color]India Standard Time[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"cst_china"[/color]>[/color]China Standard Time[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"jst"[/color]>[/color]Japan Standard Time[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"kst"[/color]>[/color]Korea Standard Time[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]select-item[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`[color=#116329]              [color=#0550ae]value[/color]=[color=#0a3069]"ist_indonesia"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`[color=#116329]            >[/color]Indonesia Central Standard Time[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]select-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]select-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]portal[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]select[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
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
                  <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]select[/color] [color=#79c0ff]class[/color]=[color=#a5d6ff]"select-scrollable-demo"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]select-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]select-value[/color] [color=#79c0ff]data-select-placeholder[/color]=[color=#a5d6ff]"Select a timezone"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]select-trigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]portal[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]select-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]select-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-label[/color]>[/color]North America[color=#7ee787]</[color=#79c0ff]select-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"est"[/color]>[/color]Eastern Standard Time[color=#7ee787]</[color=#79c0ff]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"cst"[/color]>[/color]Central Standard Time[color=#7ee787]</[color=#79c0ff]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"mst"[/color]>[/color]Mountain Standard Time[color=#7ee787]</[color=#79c0ff]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"pst"[/color]>[/color]Pacific Standard Time[color=#7ee787]</[color=#79c0ff]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"akst"[/color]>[/color]Alaska Standard Time[color=#7ee787]</[color=#79c0ff]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"hst"[/color]>[/color]Hawaii Standard Time[color=#7ee787]</[color=#79c0ff]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]select-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]select-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-label[/color]>[/color]Europe [color=#79c0ff]&amp;[/color] Africa[color=#7ee787]</[color=#79c0ff]select-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"gmt"[/color]>[/color]Greenwich Mean Time[color=#7ee787]</[color=#79c0ff]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"cet"[/color]>[/color]Central European Time[color=#7ee787]</[color=#79c0ff]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"eet"[/color]>[/color]Eastern European Time[color=#7ee787]</[color=#79c0ff]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"west"[/color]>[/color]Western European Summer Time[color=#7ee787]</[color=#79c0ff]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"cat"[/color]>[/color]Central Africa Time[color=#7ee787]</[color=#79c0ff]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"eat"[/color]>[/color]East Africa Time[color=#7ee787]</[color=#79c0ff]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]select-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]select-separator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]select-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-label[/color]>[/color]Asia[color=#7ee787]</[color=#79c0ff]select-label[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"msk"[/color]>[/color]Moscow Time[color=#7ee787]</[color=#79c0ff]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"ist"[/color]>[/color]India Standard Time[color=#7ee787]</[color=#79c0ff]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"cst_china"[/color]>[/color]China Standard Time[color=#7ee787]</[color=#79c0ff]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"jst"[/color]>[/color]Japan Standard Time[color=#7ee787]</[color=#79c0ff]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-item[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]"kst"[/color]>[/color]Korea Standard Time[color=#7ee787]</[color=#79c0ff]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]select-item[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`[color=#7ee787]              [color=#79c0ff]value[/color]=[color=#a5d6ff]"ist_indonesia"[/color][/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`[color=#7ee787]            >[/color]Indonesia Central Standard Time[color=#7ee787]</[color=#79c0ff]select-item[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{`          [color=#7ee787]</[color=#79c0ff]select-group[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]select-content[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]portal[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`    [color=#7ee787]</[color=#79c0ff]select[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]ui[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`[color=#7ee787]</[color=#79c0ff]lcui-app[/color]>[/color]`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/select-scrollable/2" style={{ display: "none" }}>
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
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]select_scrollable_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
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
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]select_scrollable_xml_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
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
        <Widget className="code-block-body" data-language="tsx" data-file="example.tsx" data-source="examples/select-scrollable/3" style={{ display: "none" }}>
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
                  <Text className="code-line">{`  [color=#8250df]Select[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#8250df]SelectContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#8250df]SelectGroup[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#8250df]SelectItem[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#8250df]SelectLabel[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#8250df]SelectSeparator[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#8250df]SelectTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`  [color=#8250df]SelectValue[/color],`}</Text>
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
                  <Text className="code-line">{`[color=#cf222e]const[/color] northAmerica = \\[`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Eastern Standard Time"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"est"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Central Standard Time"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"cst"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Mountain Standard Time"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"mst"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Pacific Standard Time"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"pst"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Alaska Standard Time"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"akst"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Hawaii Standard Time"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"hst"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`[color=#cf222e]const[/color] europeAfrica = \\[`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Greenwich Mean Time"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"gmt"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Central European Time"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"cet"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Eastern European Time"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"eet"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Western European Summer Time"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"west"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Central Africa Time"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"cat"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"East Africa Time"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"eat"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`[color=#cf222e]const[/color] asia = \\[`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Moscow Time"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"msk"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"India Standard Time"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"ist"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"China Standard Time"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"cst_china"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Japan Standard Time"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"jst"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Korea Standard Time"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"kst"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`  { [color=#0550ae]label[/color]: [color=#0a3069]"Indonesia Central Standard Time"[/color], [color=#0550ae]value[/color]: [color=#0a3069]"ist_indonesia"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Example[/color]([color=#0550ae][/color]) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Select[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]SelectTrigger[/color] [color=#0550ae]className[/color]=[color=#0a3069]"w-full max-w-64"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]SelectValue[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"Select a timezone"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]SelectTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]SelectLabel[/color]>[/color]North America[color=#116329]</[color=#0550ae]SelectLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`          {northAmerica.map((item) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]key[/color]=[color=#0a3069]{item.value}[/color] [color=#0550ae]value[/color]=[color=#0a3069]{item.value}[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`              {item.label}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`          ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]SelectLabel[/color]>[/color]Europe [color=#0550ae]&amp;[/color] Africa[color=#116329]</[color=#0550ae]SelectLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`          {europeAfrica.map((item) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]key[/color]=[color=#0a3069]{item.value}[/color] [color=#0550ae]value[/color]=[color=#0a3069]{item.value}[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`              {item.label}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`          ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]SelectSeparator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]SelectLabel[/color]>[/color]Asia[color=#116329]</[color=#0550ae]SelectLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`          {asia.map((item) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`            [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]key[/color]=[color=#0a3069]{item.value}[/color] [color=#0550ae]value[/color]=[color=#0a3069]{item.value}[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`              {item.label}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`            [color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`          ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Select[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">73</Text>
                  <Text className="code-line">{`  );`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">74</Text>
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
                  <Text className="code-line">{`  [color=#d2a8ff]Select[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SelectContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SelectGroup[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SelectItem[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SelectLabel[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SelectSeparator[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SelectTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`  [color=#d2a8ff]SelectValue[/color],`}</Text>
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
                  <Text className="code-line">{`[color=#ff7b72]const[/color] northAmerica = \\[`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]label[/color]: [color=#a5d6ff]"Eastern Standard Time"[/color], [color=#79c0ff]value[/color]: [color=#a5d6ff]"est"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]label[/color]: [color=#a5d6ff]"Central Standard Time"[/color], [color=#79c0ff]value[/color]: [color=#a5d6ff]"cst"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]label[/color]: [color=#a5d6ff]"Mountain Standard Time"[/color], [color=#79c0ff]value[/color]: [color=#a5d6ff]"mst"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]label[/color]: [color=#a5d6ff]"Pacific Standard Time"[/color], [color=#79c0ff]value[/color]: [color=#a5d6ff]"pst"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]label[/color]: [color=#a5d6ff]"Alaska Standard Time"[/color], [color=#79c0ff]value[/color]: [color=#a5d6ff]"akst"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]label[/color]: [color=#a5d6ff]"Hawaii Standard Time"[/color], [color=#79c0ff]value[/color]: [color=#a5d6ff]"hst"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`[color=#ff7b72]const[/color] europeAfrica = \\[`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]label[/color]: [color=#a5d6ff]"Greenwich Mean Time"[/color], [color=#79c0ff]value[/color]: [color=#a5d6ff]"gmt"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]label[/color]: [color=#a5d6ff]"Central European Time"[/color], [color=#79c0ff]value[/color]: [color=#a5d6ff]"cet"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]label[/color]: [color=#a5d6ff]"Eastern European Time"[/color], [color=#79c0ff]value[/color]: [color=#a5d6ff]"eet"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]label[/color]: [color=#a5d6ff]"Western European Summer Time"[/color], [color=#79c0ff]value[/color]: [color=#a5d6ff]"west"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">26</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]label[/color]: [color=#a5d6ff]"Central Africa Time"[/color], [color=#79c0ff]value[/color]: [color=#a5d6ff]"cat"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">27</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]label[/color]: [color=#a5d6ff]"East Africa Time"[/color], [color=#79c0ff]value[/color]: [color=#a5d6ff]"eat"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">28</Text>
                  <Text className="code-line">{`];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">29</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">30</Text>
                  <Text className="code-line">{`[color=#ff7b72]const[/color] asia = \\[`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">31</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]label[/color]: [color=#a5d6ff]"Moscow Time"[/color], [color=#79c0ff]value[/color]: [color=#a5d6ff]"msk"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">32</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]label[/color]: [color=#a5d6ff]"India Standard Time"[/color], [color=#79c0ff]value[/color]: [color=#a5d6ff]"ist"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">33</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]label[/color]: [color=#a5d6ff]"China Standard Time"[/color], [color=#79c0ff]value[/color]: [color=#a5d6ff]"cst_china"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">34</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]label[/color]: [color=#a5d6ff]"Japan Standard Time"[/color], [color=#79c0ff]value[/color]: [color=#a5d6ff]"jst"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">35</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]label[/color]: [color=#a5d6ff]"Korea Standard Time"[/color], [color=#79c0ff]value[/color]: [color=#a5d6ff]"kst"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">36</Text>
                  <Text className="code-line">{`  { [color=#79c0ff]label[/color]: [color=#a5d6ff]"Indonesia Central Standard Time"[/color], [color=#79c0ff]value[/color]: [color=#a5d6ff]"ist_indonesia"[/color] },`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">37</Text>
                  <Text className="code-line">{`];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">38</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">39</Text>
                  <Text className="code-line">{`[color=#ff7b72]export[/color] [color=#ff7b72]default[/color] [color=#ff7b72]function[/color] [color=#d2a8ff]Example[/color]([color=#79c0ff][/color]) {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">40</Text>
                  <Text className="code-line">{`  [color=#ff7b72]return[/color] (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">41</Text>
                  <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]Select[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">42</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]SelectTrigger[/color] [color=#79c0ff]className[/color]=[color=#a5d6ff]"w-full max-w-64"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">43</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]SelectValue[/color] [color=#79c0ff]placeholder[/color]=[color=#a5d6ff]"Select a timezone"[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">44</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]SelectTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">45</Text>
                  <Text className="code-line">{`      [color=#7ee787]<[color=#79c0ff]SelectContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">46</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]SelectGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">47</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]SelectLabel[/color]>[/color]North America[color=#7ee787]</[color=#79c0ff]SelectLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">48</Text>
                  <Text className="code-line">{`          {northAmerica.map((item) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">49</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]SelectItem[/color] [color=#79c0ff]key[/color]=[color=#a5d6ff]{item.value}[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]{item.value}[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">50</Text>
                  <Text className="code-line">{`              {item.label}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">51</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]SelectItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">52</Text>
                  <Text className="code-line">{`          ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">53</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]SelectGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">54</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]SelectGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">55</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]SelectLabel[/color]>[/color]Europe [color=#79c0ff]&amp;[/color] Africa[color=#7ee787]</[color=#79c0ff]SelectLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">56</Text>
                  <Text className="code-line">{`          {europeAfrica.map((item) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">57</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]SelectItem[/color] [color=#79c0ff]key[/color]=[color=#a5d6ff]{item.value}[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]{item.value}[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">58</Text>
                  <Text className="code-line">{`              {item.label}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">59</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]SelectItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">60</Text>
                  <Text className="code-line">{`          ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">61</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]SelectGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">62</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]SelectSeparator[/color] />[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">63</Text>
                  <Text className="code-line">{`        [color=#7ee787]<[color=#79c0ff]SelectGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">64</Text>
                  <Text className="code-line">{`          [color=#7ee787]<[color=#79c0ff]SelectLabel[/color]>[/color]Asia[color=#7ee787]</[color=#79c0ff]SelectLabel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">65</Text>
                  <Text className="code-line">{`          {asia.map((item) => (`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">66</Text>
                  <Text className="code-line">{`            [color=#7ee787]<[color=#79c0ff]SelectItem[/color] [color=#79c0ff]key[/color]=[color=#a5d6ff]{item.value}[/color] [color=#79c0ff]value[/color]=[color=#a5d6ff]{item.value}[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">67</Text>
                  <Text className="code-line">{`              {item.label}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">68</Text>
                  <Text className="code-line">{`            [color=#7ee787]</[color=#79c0ff]SelectItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">69</Text>
                  <Text className="code-line">{`          ))}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">70</Text>
                  <Text className="code-line">{`        [color=#7ee787]</[color=#79c0ff]SelectGroup[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">71</Text>
                  <Text className="code-line">{`      [color=#7ee787]</[color=#79c0ff]SelectContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">72</Text>
                  <Text className="code-line">{`    [color=#7ee787]</[color=#79c0ff]Select[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">73</Text>
                  <Text className="code-line">{`  );`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">74</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/select-scrollable/4" style={{ display: "none" }}>
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
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]select_scrollable_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
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
                  <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]select_scrollable_tsx_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *parent)[/color]`}</Text>
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
