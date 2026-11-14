import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";

export default function CustomizationDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Customization</h1>
      <p className="doc-page-desc">{`You can create entirely new widget types or extend existing ones.`}</p>
      <Widget className="doc-section">
        <h2>Creating a custom widget</h2>
        <p>{`Use [bgcolor=#7f7f7f29] ui_create_widget_prototype() [/bgcolor] to register a new widget type:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/customization/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">src/counter.c</Text>
          </Widget>
          <Widget className="code-block-body">
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
                    <Text className="code-line">{`[color=#cf222e]typedef[/color] [color=#cf222e]struct[/color] {`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`        [color=#953800]int[/color] value;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`} [color=#953800]counter_data_t[/color];`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_prototype_t[/color] *counter_proto;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]counter_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">10</Text>
                    <Text className="code-line">{`{`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">11</Text>
                    <Text className="code-line">{`        [color=#953800]counter_data_t[/color] *data;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">12</Text>
                    <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *text;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">13</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">14</Text>
                    <Text className="code-line">{`        data = ui_widget_add_data(w, counter_proto, [color=#cf222e]sizeof[/color]([color=#953800]counter_data_t[/color]));`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">15</Text>
                    <Text className="code-line">{`        data->value = [color=#0550ae]0[/color];`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">16</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">17</Text>
                    <Text className="code-line">{`        text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">18</Text>
                    <Text className="code-line">{`        ui_text_set_content(text, [color=#0a3069]"0"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">19</Text>
                    <Text className="code-line">{`        ui_widget_append(w, text);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">20</Text>
                    <Text className="code-line">{`}`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">21</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">22</Text>
                    <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]counter_destroy[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">23</Text>
                    <Text className="code-line">{`{`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">24</Text>
                    <Text className="code-line">{`        [color=#6e7781]/* Release owned resources (if any) */[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">25</Text>
                    <Text className="code-line">{`}`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">26</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">27</Text>
                    <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]register_counter_widget[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">28</Text>
                    <Text className="code-line">{`{`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">29</Text>
                    <Text className="code-line">{`        counter_proto = ui_create_widget_prototype([color=#0a3069]"counter"[/color], [color=#0550ae]NULL[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">30</Text>
                    <Text className="code-line">{`        counter_proto->init = counter_init;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">31</Text>
                    <Text className="code-line">{`        counter_proto->destroy = counter_destroy;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">32</Text>
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
                    <Text className="code-line">{`[color=#ff7b72]typedef[/color] [color=#ff7b72]struct[/color] {`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`        [color=#ffa657]int[/color] value;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`} [color=#ffa657]counter_data_t[/color];`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]ui_widget_prototype_t[/color] *counter_proto;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]void[/color] [color=#d2a8ff]counter_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *w)[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">10</Text>
                    <Text className="code-line">{`{`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">11</Text>
                    <Text className="code-line">{`        [color=#ffa657]counter_data_t[/color] *data;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">12</Text>
                    <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *text;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">13</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">14</Text>
                    <Text className="code-line">{`        data = ui_widget_add_data(w, counter_proto, [color=#ff7b72]sizeof[/color]([color=#ffa657]counter_data_t[/color]));`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">15</Text>
                    <Text className="code-line">{`        data->value = [color=#79c0ff]0[/color];`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">16</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">17</Text>
                    <Text className="code-line">{`        text = ui_create_widget([color=#a5d6ff]"text"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">18</Text>
                    <Text className="code-line">{`        ui_text_set_content(text, [color=#a5d6ff]"0"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">19</Text>
                    <Text className="code-line">{`        ui_widget_append(w, text);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">20</Text>
                    <Text className="code-line">{`}`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">21</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">22</Text>
                    <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]void[/color] [color=#d2a8ff]counter_destroy[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *w)[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">23</Text>
                    <Text className="code-line">{`{`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">24</Text>
                    <Text className="code-line">{`        [color=#8b949e]/* Release owned resources (if any) */[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">25</Text>
                    <Text className="code-line">{`}`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">26</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">27</Text>
                    <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]register_counter_widget[/color][color=#79c0ff]([color=#ffa657]void[/color])[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">28</Text>
                    <Text className="code-line">{`{`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">29</Text>
                    <Text className="code-line">{`        counter_proto = ui_create_widget_prototype([color=#a5d6ff]"counter"[/color], [color=#79c0ff]NULL[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">30</Text>
                    <Text className="code-line">{`        counter_proto->init = counter_init;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">31</Text>
                    <Text className="code-line">{`        counter_proto->destroy = counter_destroy;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">32</Text>
                    <Text className="code-line">{`}`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`Use the custom widget:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/customization/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">src/main.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#cf222e]extern[/color] [color=#953800]void[/color] [color=#8250df]register_counter_widget[/color][color=#0550ae]([color=#953800]void[/color])[/color];`}</Text>
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
                    <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *counter;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`        lcui_init();`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`        register_counter_widget();`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">10</Text>
                    <Text className="code-line">{`        counter = ui_create_widget([color=#0a3069]"counter"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">11</Text>
                    <Text className="code-line">{`        ui_widget_append(ui_root(), counter);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">12</Text>
                    <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">13</Text>
                    <Text className="code-line">{`}`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]extern[/color] [color=#ffa657]void[/color] [color=#d2a8ff]register_counter_widget[/color][color=#79c0ff]([color=#ffa657]void[/color])[/color];`}</Text>
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
                    <Text className="code-line">{`        [color=#ffa657]ui_widget_t[/color] *counter;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`        lcui_init();`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`        register_counter_widget();`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">10</Text>
                    <Text className="code-line">{`        counter = ui_create_widget([color=#a5d6ff]"counter"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">11</Text>
                    <Text className="code-line">{`        ui_widget_append(ui_root(), counter);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">12</Text>
                    <Text className="code-line">{`        [color=#ff7b72]return[/color] lcui_main();`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">13</Text>
                    <Text className="code-line">{`}`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Extending an existing widget</h2>
        <p>{`Pass a parent type name as the second argument to [bgcolor=#7f7f7f29] ui_create_widget_prototype() [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/customization/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">src/icon_button.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_prototype_t[/color] *icon_button_proto;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]icon_button_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`{`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`        [color=#6e7781]/* Call parent (button) init logic */[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`        icon_button_proto->proto->init(w);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`        ui_widget_add_class(w, [color=#0a3069]"icon-button"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`}`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">10</Text>
                    <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]register_icon_button[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">11</Text>
                    <Text className="code-line">{`{`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">12</Text>
                    <Text className="code-line">{`        icon_button_proto =`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">13</Text>
                    <Text className="code-line">{`            ui_create_widget_prototype([color=#0a3069]"icon-button"[/color], [color=#0a3069]"button"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">14</Text>
                    <Text className="code-line">{`        icon_button_proto->init = icon_button_init;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">15</Text>
                    <Text className="code-line">{`}`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]ui_widget_prototype_t[/color] *icon_button_proto;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]void[/color] [color=#d2a8ff]icon_button_init[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *w)[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`{`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`        [color=#8b949e]/* Call parent (button) init logic */[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`        icon_button_proto->proto->init(w);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{`        ui_widget_add_class(w, [color=#a5d6ff]"icon-button"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`}`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">9</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">10</Text>
                    <Text className="code-line">{`[color=#ffa657]void[/color] [color=#d2a8ff]register_icon_button[/color][color=#79c0ff]([color=#ffa657]void[/color])[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">11</Text>
                    <Text className="code-line">{`{`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">12</Text>
                    <Text className="code-line">{`        icon_button_proto =`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">13</Text>
                    <Text className="code-line">{`            ui_create_widget_prototype([color=#a5d6ff]"icon-button"[/color], [color=#a5d6ff]"button"[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">14</Text>
                    <Text className="code-line">{`        icon_button_proto->init = icon_button_init;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">15</Text>
                    <Text className="code-line">{`}`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b]Parent [bgcolor=#7f7f7f29] init [/bgcolor] must be called manually.[/b] LCUI does not automatically call the parent type's [bgcolor=#7f7f7f29] init [/bgcolor] when creating a subtype widget. If you forget to call [bgcolor=#7f7f7f29] proto->proto->init(w) [/bgcolor], the widget will lack the parent type's base behaviors (e.g. [bgcolor=#7f7f7f29] button [/bgcolor] won't register click events or set default styles).`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Widget lifecycle</h2>
        <p>{`[bgcolor=#7f7f7f29] ui_widget_prototype_t [/bgcolor] has several callback hooks covering each lifecycle stage:`}</p>
        <ul>
          <li><Text>{`[b][bgcolor=#7f7f7f29] init [/bgcolor][/b] — When the widget is created. Responsible for initializing owned data, child widgets, and event bindings.`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] destroy [/bgcolor][/b] — When the widget is destroyed. Responsible for releasing owned memory and unbinding events.`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] update [/bgcolor][/b] — When the widget needs updating, receives a [bgcolor=#7f7f7f29] ui_task_type_t [/bgcolor] parameter indicating the update type (style update, property update, etc.). Called by LCUI during each frame's update loop.`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] setattr [/bgcolor][/b] — When an XML attribute is set (for responding to attribute changes).`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] settext [/bgcolor][/b] — When text content is set.`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] sizehint [/bgcolor][/b] — Estimates the widget's natural size constraints (min/max content size) without external constraints. The layout engine uses this when computing child widget sizes.`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] resize [/bgcolor][/b] — When the widget size changes. Receives the new content area width and height as parameters.`}</Text></li>
          <li><Text>{`[b][bgcolor=#7f7f7f29] paint [/bgcolor][/b] — When the widget needs to be redrawn. Used for custom drawing logic.`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Custom painting</h2>
        <p>{`Override the [bgcolor=#7f7f7f29] paint [/bgcolor] callback to draw custom graphics on a widget:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/customization/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">src/my_widget.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]my_widget_paint[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w, [color=#953800]pd_context_t[/color] *paint_ctx,[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`[color=#0550ae]                              [color=#953800]ui_widget_actual_style_t[/color] *style)[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`{`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`        [color=#953800]pd_canvas_t[/color] *canvas = paint_ctx->canvas;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`        [color=#953800]pd_color_t[/color] color = pd_color_from_rgb([color=#0550ae]255[/color], [color=#0550ae]0[/color], [color=#0550ae]0[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`        [color=#953800]pd_rect_t[/color] rect = { style->left, style->top, w->width, w->height };`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`        pd_canvas_fill_rect(canvas, color, rect);`}</Text>
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
                    <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]register_my_widget[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">12</Text>
                    <Text className="code-line">{`{`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">13</Text>
                    <Text className="code-line">{`        [color=#953800]ui_widget_prototype_t[/color] *proto =`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">14</Text>
                    <Text className="code-line">{`            ui_create_widget_prototype([color=#0a3069]"my-widget"[/color], [color=#0550ae]NULL[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">15</Text>
                    <Text className="code-line">{`        proto->paint = my_widget_paint;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">16</Text>
                    <Text className="code-line">{`}`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]void[/color] [color=#d2a8ff]my_widget_paint[/color][color=#79c0ff]([color=#ffa657]ui_widget_t[/color] *w, [color=#ffa657]pd_context_t[/color] *paint_ctx,[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`[color=#79c0ff]                              [color=#ffa657]ui_widget_actual_style_t[/color] *style)[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`{`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`        [color=#ffa657]pd_canvas_t[/color] *canvas = paint_ctx->canvas;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`        [color=#ffa657]pd_color_t[/color] color = pd_color_from_rgb([color=#79c0ff]255[/color], [color=#79c0ff]0[/color], [color=#79c0ff]0[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`        [color=#ffa657]pd_rect_t[/color] rect = { style->left, style->top, w->width, w->height };`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">7</Text>
                    <Text className="code-line">{` `}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">8</Text>
                    <Text className="code-line">{`        pd_canvas_fill_rect(canvas, color, rect);`}</Text>
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
                    <Text className="code-line">{`[color=#ffa657]static[/color] [color=#ffa657]void[/color] [color=#d2a8ff]register_my_widget[/color][color=#79c0ff]([color=#ffa657]void[/color])[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">12</Text>
                    <Text className="code-line">{`{`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">13</Text>
                    <Text className="code-line">{`        [color=#ffa657]ui_widget_prototype_t[/color] *proto =`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">14</Text>
                    <Text className="code-line">{`            ui_create_widget_prototype([color=#a5d6ff]"my-widget"[/color], [color=#79c0ff]NULL[/color]);`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">15</Text>
                    <Text className="code-line">{`        proto->paint = my_widget_paint;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">16</Text>
                    <Text className="code-line">{`}`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#7f7f7f29] paint [/bgcolor] is only for drawing.[/b] Widget size estimation is handled by [bgcolor=#7f7f7f29] sizehint [/bgcolor]; do not modify widget size or the child widget tree inside [bgcolor=#7f7f7f29] paint [/bgcolor], as it may cause infinite loops.`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Caveats</h2>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#7f7f7f29] destroy [/bgcolor] must release owned resources.[/b] If a custom widget allocated memory, opened files, or subscribed to system events (e.g. [bgcolor=#7f7f7f29] lcui_settings_on_deserialize [/bgcolor]) in [bgcolor=#7f7f7f29] init [/bgcolor], it must perform symmetric cleanup in [bgcolor=#7f7f7f29] destroy [/bgcolor].`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b]Custom widgets must be registered after [bgcolor=#7f7f7f29] lcui_init() [/bgcolor] and before first instantiation.[/b] Calling your [bgcolor=#7f7f7f29] register_xxx() [/bgcolor] function right at the start of [bgcolor=#7f7f7f29] main() [/bgcolor] is the safest place, ensuring all consumers (XML loading, router instantiation, TSX compilation results) can see the prototype.`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
