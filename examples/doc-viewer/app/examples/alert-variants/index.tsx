import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function AlertVariantsDemo() {
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
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/alert-variants/0">
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-line-row">
                <Text className="code-line-number">1</Text>
                <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Alert[/color], [color=#8250df]AlertContent[/color], [color=#8250df]AlertDescription[/color], [color=#8250df]AlertTitle[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">2</Text>
                <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">3</Text>
                <Text className="code-line">{`  [color=#8250df]CheckmarkCircle[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`  [color=#8250df]ErrorCircle[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`  [color=#8250df]Info[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`  [color=#8250df]Warning[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/fluent-icons"[/color];`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]Example[/color]([color=#0550ae][/color]) {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`    [color=#116329]<[color=#0550ae]widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"block w-full max-w-md"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Alert[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"info"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"mb-6"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Info[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AlertContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AlertTitle[/color]>[/color]New feature available[color=#116329]</[color=#0550ae]AlertTitle[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AlertDescription[/color]>[/color]Try the updated dashboard.[color=#116329]</[color=#0550ae]AlertDescription[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AlertContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Alert[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Alert[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"success"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"mb-6"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]CheckmarkCircle[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AlertContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AlertTitle[/color]>[/color]Payment successful[color=#116329]</[color=#0550ae]AlertTitle[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AlertDescription[/color]>[/color]Your payment has been processed.[color=#116329]</[color=#0550ae]AlertDescription[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AlertContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Alert[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Alert[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"warning"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"mb-6"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Warning[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AlertContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AlertTitle[/color]>[/color]Storage almost full[color=#116329]</[color=#0550ae]AlertTitle[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AlertDescription[/color]>[/color]Free up space to continue.[color=#116329]</[color=#0550ae]AlertDescription[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AlertContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Alert[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Alert[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"destructive"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"mb-6"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]ErrorCircle[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">35</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AlertContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">36</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AlertTitle[/color]>[/color]Unable to save changes[color=#116329]</[color=#0550ae]AlertTitle[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">37</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AlertDescription[/color]>[/color]Please try again.[color=#116329]</[color=#0550ae]AlertDescription[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">38</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AlertContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">39</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Alert[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">40</Text>
                <Text className="code-line">{`    [color=#116329]</[color=#0550ae]widget[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">41</Text>
                <Text className="code-line">{`  );`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">42</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/alert-variants/1" style={{ display: "none" }}>
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
                <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]alert_variants_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
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
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/alert-variants/2" style={{ display: "none" }}>
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
                <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_alert[/color][color=#0550ae]([color=#953800]ui_alert_variant_t[/color] variant,[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`[color=#0550ae]                                 [color=#953800]const[/color] [color=#953800]char[/color] *icon_name, [color=#953800]const[/color] [color=#953800]char[/color] *title,[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`[color=#0550ae]                                 [color=#953800]const[/color] [color=#953800]char[/color] *description)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *alert = ui_create_alert();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`        ui_alert_set_variant(alert, variant);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`        ui_alert_set_icon(alert, icon_name);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`        ui_alert_set_title(alert, title);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`        ui_alert_set_description(alert, description);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`        ui_widget_add_class(alert, [color=#0a3069]"mb-6"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`        [color=#cf222e]return[/color] alert;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]alert_variants_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *box = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`        ui_widget_add_class(box, [color=#0a3069]"alert-variants-demo"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`        ui_widget_append(box, create_alert(UI_ALERT_VARIANT_INFO, [color=#0a3069]"info"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`                                           [color=#0a3069]"New feature available"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`                                           [color=#0a3069]"Try the updated dashboard."[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{`        ui_widget_append(`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`            box, create_alert(UI_ALERT_VARIANT_SUCCESS, [color=#0a3069]"checkmark-circle"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`                              [color=#0a3069]"Payment successful"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{`                              [color=#0a3069]"Your payment has been processed."[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`        ui_widget_append(box, create_alert(UI_ALERT_VARIANT_WARNING, [color=#0a3069]"warning"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{`                                           [color=#0a3069]"Storage almost full"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{`                                           [color=#0a3069]"Free up space to continue."[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`        ui_widget_append(`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`            box, create_alert(UI_ALERT_VARIANT_DESTRUCTIVE, [color=#0a3069]"error-circle"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`                              [color=#0a3069]"Unable to save changes"[/color], [color=#0a3069]"Please try again."[/color]));`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">35</Text>
                <Text className="code-line">{`        ui_widget_append(parent, box);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">36</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/alert-variants/3" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
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
                <Text className="code-line">{`    [color=#116329]<[color=#0550ae]widget[/color] [color=#0550ae]class[/color]=[color=#0a3069]"alert-variants-demo"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]alert[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"info"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"mb-6"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"info"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]alert-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]alert-title[/color]>[/color]New feature available[color=#116329]</[color=#0550ae]alert-title[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]alert-description[/color]>[/color]Try the updated dashboard.[color=#116329]</[color=#0550ae]alert-description[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]alert-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]alert[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]alert[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"success"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"mb-6"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"checkmark-circle"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]alert-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]alert-title[/color]>[/color]Payment successful[color=#116329]</[color=#0550ae]alert-title[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]alert-description[/color]>[/color]Your payment has been processed.[color=#116329]</[color=#0550ae]alert-description[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]alert-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]alert[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]alert[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"warning"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"mb-6"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"warning"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]alert-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]alert-title[/color]>[/color]Storage almost full[color=#116329]</[color=#0550ae]alert-title[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]alert-description[/color]>[/color]Free up space to continue.[color=#116329]</[color=#0550ae]alert-description[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]alert-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]alert[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]alert[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"destructive"[/color] [color=#0550ae]class[/color]=[color=#0a3069]"mb-6"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]icon[/color] [color=#0550ae]name[/color]=[color=#0a3069]"error-circle"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]alert-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]alert-title[/color]>[/color]Unable to save changes[color=#116329]</[color=#0550ae]alert-title[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]alert-description[/color]>[/color]Please try again.[color=#116329]</[color=#0550ae]alert-description[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]alert-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]alert[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`    [color=#116329]</[color=#0550ae]widget[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/alert-variants/4" style={{ display: "none" }}>
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
                <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]alert_variants_xml_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
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
