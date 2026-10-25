import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function FieldCheckboxDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/field-checkbox/5">
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
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
                <Text className="code-line">{`  [color=#8250df]FieldLegend[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`  [color=#8250df]FieldSeparator[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`  [color=#8250df]FieldSet[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-col gap-2 max-w-\\[400px]"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldLegend[/color]>[/color]Show these items on the desktop[color=#116329]</[color=#0550ae]FieldLegend[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{`          Select the items you want to show on the desktop.`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldGroup[/color] [color=#0550ae]className[/color]=[color=#0a3069]"gap-3"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-hard-disks-ljj"[/color] [color=#0550ae]defaultChecked[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`[color=#116329]              [color=#0550ae]htmlFor[/color]=[color=#0a3069]"finder-pref-9k2-hard-disks-ljj"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`[color=#116329]              [color=#0550ae]className[/color]=[color=#0a3069]"font-normal"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{`[color=#116329]            >[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`              Hard disks`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-external-disks-1yg"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">35</Text>
                <Text className="code-line">{`[color=#116329]              [color=#0550ae]htmlFor[/color]=[color=#0a3069]"finder-pref-9k2-external-disks-1yg"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">36</Text>
                <Text className="code-line">{`[color=#116329]              [color=#0550ae]className[/color]=[color=#0a3069]"font-normal"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">37</Text>
                <Text className="code-line">{`[color=#116329]            >[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">38</Text>
                <Text className="code-line">{`              External disks`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">39</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">40</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">41</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">42</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-cds-dvds-fzt"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">43</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">44</Text>
                <Text className="code-line">{`[color=#116329]              [color=#0550ae]htmlFor[/color]=[color=#0a3069]"finder-pref-9k2-cds-dvds-fzt"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">45</Text>
                <Text className="code-line">{`[color=#116329]              [color=#0550ae]className[/color]=[color=#0a3069]"font-normal"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">46</Text>
                <Text className="code-line">{`[color=#116329]            >[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">47</Text>
                <Text className="code-line">{`              CDs, DVDs, and iPods`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">48</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">49</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">50</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">51</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-connected-servers-6l2"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">52</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">53</Text>
                <Text className="code-line">{`[color=#116329]              [color=#0550ae]htmlFor[/color]=[color=#0a3069]"finder-pref-9k2-connected-servers-6l2"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">54</Text>
                <Text className="code-line">{`[color=#116329]              [color=#0550ae]className[/color]=[color=#0a3069]"font-normal"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">55</Text>
                <Text className="code-line">{`[color=#116329]            >[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">56</Text>
                <Text className="code-line">{`              Connected servers`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">57</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">58</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">59</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">60</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">61</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldSeparator[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">62</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">63</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-sync-folders-nep"[/color] [color=#0550ae]defaultChecked[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">64</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">65</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"finder-pref-9k2-sync-folders-nep"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">66</Text>
                <Text className="code-line">{`            Sync Desktop [color=#0550ae]&amp;[/color] Documents folders`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">67</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">68</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">69</Text>
                <Text className="code-line">{`            Your Desktop [color=#0550ae]&amp;[/color] Documents folders are being synced with iCloud`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">70</Text>
                <Text className="code-line">{`            Drive. You can access them from other devices.`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">71</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">72</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">73</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">74</Text>
                <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">75</Text>
                <Text className="code-line">{`  );`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">76</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/field-checkbox/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-line-row">
                <Text className="code-line-number">1</Text>
                <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">2</Text>
                <Text className="code-line">{`[color=#6e7781] * docs/examples/field-checkbox-tsx/main.c: -- Field widget demo (TSX variant)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">3</Text>
                <Text className="code-line">{`[color=#6e7781] *[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`[color=#6e7781] * SPDX-License-Identifier: MIT[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`[color=#6e7781] */[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]field_checkbox_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`        example_load(parent);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/field-checkbox/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-line-row">
                <Text className="code-line-number">1</Text>
                <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">2</Text>
                <Text className="code-line">{`[color=#6e7781] * docs/examples/field-checkbox/main.c: -- Field widget demo (C variant)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">3</Text>
                <Text className="code-line">{`[color=#6e7781] *[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`[color=#6e7781] * SPDX-License-Identifier: MIT[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`[color=#6e7781] */[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]make_check_row[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent, [color=#953800]const[/color] [color=#953800]char[/color] *id,[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`[color=#0550ae]                           [color=#953800]const[/color] [color=#953800]char[/color] *text, [color=#953800]int[/color] checked)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *row = ui_create_field();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *cb = ui_create_checkbox();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *lbl = ui_create_field_label();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`        ui_widget_add_class(lbl, [color=#0a3069]"font-normal"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`        ui_field_set_orientation(row, [color=#0a3069]"horizontal"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`        ui_widget_set_id(cb, id);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`        [color=#cf222e]if[/color] (checked) {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{`                ui_widget_set_attr(cb, [color=#0a3069]"checked"[/color], [color=#0a3069]"true"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`        }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`        ui_widget_append(row, cb);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`        ui_label_set_for(lbl, id);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`        ui_text_set_content(lbl, text);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{`        ui_widget_append(row, lbl);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`        ui_widget_append(parent, row);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]field_checkbox_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *outer, *[color=#953800]set[/color], *legend, *desc, *group;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *sep;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *sync_field, *sync_cb, *sync_content;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *sync_lbl, *sync_desc;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">35</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">36</Text>
                <Text className="code-line">{`        outer = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">37</Text>
                <Text className="code-line">{`        [color=#953800]set[/color] = ui_create_field_set();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">38</Text>
                <Text className="code-line">{`        legend = ui_create_field_legend();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">39</Text>
                <Text className="code-line">{`        desc = ui_create_field_description();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">40</Text>
                <Text className="code-line">{`        group = ui_create_field_group();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">41</Text>
                <Text className="code-line">{`        sep = ui_create_field_separator();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">42</Text>
                <Text className="code-line">{`        sync_field = ui_create_field();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">43</Text>
                <Text className="code-line">{`        sync_cb = ui_create_checkbox();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">44</Text>
                <Text className="code-line">{`        sync_content = ui_create_field_content();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">45</Text>
                <Text className="code-line">{`        sync_lbl = ui_create_field_label();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">46</Text>
                <Text className="code-line">{`        sync_desc = ui_create_field_description();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">47</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">48</Text>
                <Text className="code-line">{`        ui_widget_add_class(outer, [color=#0a3069]"field-demo"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">49</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">50</Text>
                <Text className="code-line">{`        ui_text_set_content(legend, [color=#0a3069]"Show these items on the desktop"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">51</Text>
                <Text className="code-line">{`        ui_widget_append([color=#953800]set[/color], legend);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">52</Text>
                <Text className="code-line">{`        ui_text_set_content(`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">53</Text>
                <Text className="code-line">{`            desc, [color=#0a3069]"Select the items you want to show on the desktop."[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">54</Text>
                <Text className="code-line">{`        ui_widget_append([color=#953800]set[/color], desc);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">55</Text>
                <Text className="code-line">{`        ui_widget_add_class(group, [color=#0a3069]"field-checkbox-demo-group"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">56</Text>
                <Text className="code-line">{`        ui_widget_append([color=#953800]set[/color], group);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">57</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">58</Text>
                <Text className="code-line">{`        make_check_row(group, [color=#0a3069]"finder-pref-9k2-hard-disks-ljj"[/color], [color=#0a3069]"Hard disks"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">59</Text>
                <Text className="code-line">{`                       [color=#0550ae]1[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">60</Text>
                <Text className="code-line">{`        make_check_row(group, [color=#0a3069]"finder-pref-9k2-external-disks-1yg"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">61</Text>
                <Text className="code-line">{`                       [color=#0a3069]"External disks"[/color], [color=#0550ae]0[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">62</Text>
                <Text className="code-line">{`        make_check_row(group, [color=#0a3069]"finder-pref-9k2-cds-dvds-fzt"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">63</Text>
                <Text className="code-line">{`                       [color=#0a3069]"CDs, DVDs, and iPods"[/color], [color=#0550ae]0[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">64</Text>
                <Text className="code-line">{`        make_check_row(group, [color=#0a3069]"finder-pref-9k2-connected-servers-6l2"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">65</Text>
                <Text className="code-line">{`                       [color=#0a3069]"Connected servers"[/color], [color=#0550ae]0[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">66</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">67</Text>
                <Text className="code-line">{`        ui_widget_append(outer, [color=#953800]set[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">68</Text>
                <Text className="code-line">{`        ui_widget_append(outer, sep);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">69</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">70</Text>
                <Text className="code-line">{`        ui_field_set_orientation(sync_field, [color=#0a3069]"horizontal"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">71</Text>
                <Text className="code-line">{`        ui_widget_set_id(sync_cb, [color=#0a3069]"finder-pref-9k2-sync-folders-nep"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">72</Text>
                <Text className="code-line">{`        ui_widget_set_attr(sync_cb, [color=#0a3069]"checked"[/color], [color=#0a3069]"true"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">73</Text>
                <Text className="code-line">{`        ui_widget_append(sync_field, sync_cb);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">74</Text>
                <Text className="code-line">{`        ui_widget_append(sync_field, sync_content);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">75</Text>
                <Text className="code-line">{`        ui_label_set_for(sync_lbl, [color=#0a3069]"finder-pref-9k2-sync-folders-nep"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">76</Text>
                <Text className="code-line">{`        ui_text_set_content(sync_lbl, [color=#0a3069]"Sync Desktop & Documents folders"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">77</Text>
                <Text className="code-line">{`        ui_widget_append(sync_content, sync_lbl);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">78</Text>
                <Text className="code-line">{`        ui_text_set_content(`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">79</Text>
                <Text className="code-line">{`            sync_desc, [color=#0a3069]"Your Desktop & Documents folders are being synced with "[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">80</Text>
                <Text className="code-line">{`                       [color=#0a3069]"iCloud Drive. You can access them from other devices."[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">81</Text>
                <Text className="code-line">{`        ui_widget_append(sync_content, sync_desc);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">82</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">83</Text>
                <Text className="code-line">{`        ui_widget_append(outer, sync_field);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">84</Text>
                <Text className="code-line">{`        ui_widget_append(parent, outer);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">85</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/field-checkbox/8" style={{ display: "none" }}>
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
                <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`    [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"field-demo"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-set[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-legend[/color]>[/color]Show these items on the desktop[color=#116329]</[color=#0550ae]field-legend[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-description[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{`[color=#116329]        >[/color]Select the items you want to show on the desktop.[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-group[/color] [color=#0550ae]class[/color]=[color=#0a3069]"field-checkbox-demo-group"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-hard-disks-ljj"[/color] [color=#0550ae]checked[/color]=[color=#0a3069]"true"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`[color=#116329]              [color=#0550ae]class[/color]=[color=#0a3069]"font-normal"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`[color=#116329]              [color=#0550ae]for[/color]=[color=#0a3069]"finder-pref-9k2-hard-disks-ljj"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`[color=#116329]            >[/color]Hard disks[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-external-disks-1yg"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{`[color=#116329]              [color=#0550ae]class[/color]=[color=#0a3069]"font-normal"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`[color=#116329]              [color=#0550ae]for[/color]=[color=#0a3069]"finder-pref-9k2-external-disks-1yg"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`[color=#116329]            >[/color]External disks[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-cds-dvds-fzt"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`[color=#116329]              [color=#0550ae]class[/color]=[color=#0a3069]"font-normal"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{`[color=#116329]              [color=#0550ae]for[/color]=[color=#0a3069]"finder-pref-9k2-cds-dvds-fzt"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`[color=#116329]            >[/color]CDs, DVDs, and iPods[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-connected-servers-6l2"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`[color=#116329]              [color=#0550ae]class[/color]=[color=#0a3069]"font-normal"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">35</Text>
                <Text className="code-line">{`[color=#116329]              [color=#0550ae]for[/color]=[color=#0a3069]"finder-pref-9k2-connected-servers-6l2"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">36</Text>
                <Text className="code-line">{`[color=#116329]            >[/color]Connected servers[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">37</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">38</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field-group[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">39</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field-set[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">40</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-separator[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">41</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">42</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"finder-pref-9k2-sync-folders-nep"[/color] [color=#0550ae]checked[/color]=[color=#0a3069]"true"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">43</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">44</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field-label[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">45</Text>
                <Text className="code-line">{`[color=#116329]            [color=#0550ae]for[/color]=[color=#0a3069]"finder-pref-9k2-sync-folders-nep"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">46</Text>
                <Text className="code-line">{`[color=#116329]          >[/color]Sync Desktop [color=#0550ae]&amp;[/color] Documents folders[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">47</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field-description[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">48</Text>
                <Text className="code-line">{`[color=#116329]          >[/color]Your Desktop [color=#0550ae]&amp;[/color] Documents folders are being synced with iCloud Drive. You can access them from other devices.[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">49</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">50</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">51</Text>
                <Text className="code-line">{`    [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
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
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/field-checkbox/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-line-row">
                <Text className="code-line-number">1</Text>
                <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">2</Text>
                <Text className="code-line">{`[color=#6e7781] * docs/examples/field-checkbox-xml/main.c: -- Field widget demo (XML variant)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">3</Text>
                <Text className="code-line">{`[color=#6e7781] *[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`[color=#6e7781] * SPDX-License-Identifier: MIT[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`[color=#6e7781] */[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]main[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`        lcui_init();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`        ui_load_xml_file([color=#0a3069]"ui.xml"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
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
