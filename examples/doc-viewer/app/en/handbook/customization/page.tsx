import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function CustomizationDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Customization</h1>
      <p className="doc-page-desc">{`You can create entirely new widget types or extend existing ones.`}</p>
      <Widget className="doc-section">
        <h2>Creating a custom widget</h2>
        <p>{`Use [bgcolor=#eee] ui_create_widget_prototype() [/bgcolor] to register a new widget type:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/customization/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">src/counter.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#cf222e]typedef[/color] [color=#cf222e]struct[/color] {`}</Text>
            <Text className="code-line">{`        [color=#953800]int[/color] value;`}</Text>
            <Text className="code-line">{`} [color=#953800]counter_data_t[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_prototype_t[/color] *counter_proto;`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]counter_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        [color=#953800]counter_data_t[/color] *data;`}</Text>
            <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *text;`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`        data = ui_widget_add_data(w, counter_proto, [color=#cf222e]sizeof[/color]([color=#953800]counter_data_t[/color]));`}</Text>
            <Text className="code-line">{`        data->value = [color=#0550ae]0[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`        text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
            <Text className="code-line">{`        ui_text_set_content(text, [color=#0a3069]"0"[/color]);`}</Text>
            <Text className="code-line">{`        ui_widget_append(w, text);`}</Text>
            <Text className="code-line">{`}`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]counter_destroy[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        [color=#6e7781]/* Release owned resources (if any) */[/color]`}</Text>
            <Text className="code-line">{`}`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]register_counter_widget[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        counter_proto = ui_create_widget_prototype([color=#0a3069]"counter"[/color], [color=#0550ae]NULL[/color]);`}</Text>
            <Text className="code-line">{`        counter_proto->init = counter_init;`}</Text>
            <Text className="code-line">{`        counter_proto->destroy = counter_destroy;`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
        <p>{`Use the custom widget:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/customization/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">src/main.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]extern[/color] [color=#953800]void[/color] [color=#8250df]register_counter_widget[/color][color=#0550ae]([color=#953800]void[/color])[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]main[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *counter;`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`        lcui_init();`}</Text>
            <Text className="code-line">{`        register_counter_widget();`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`        counter = ui_create_widget([color=#0a3069]"counter"[/color]);`}</Text>
            <Text className="code-line">{`        ui_widget_append(ui_root(), counter);`}</Text>
            <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Extending an existing widget</h2>
        <p>{`Pass a parent type name as the second argument to [bgcolor=#eee] ui_create_widget_prototype() [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/customization/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">src/icon_button.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_prototype_t[/color] *icon_button_proto;`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]icon_button_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        [color=#6e7781]/* Call parent (button) init logic */[/color]`}</Text>
            <Text className="code-line">{`        icon_button_proto->proto->init(w);`}</Text>
            <Text className="code-line">{`        ui_widget_add_class(w, [color=#0a3069]"icon-button"[/color]);`}</Text>
            <Text className="code-line">{`}`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]register_icon_button[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        icon_button_proto =`}</Text>
            <Text className="code-line">{`            ui_create_widget_prototype([color=#0a3069]"icon-button"[/color], [color=#0a3069]"button"[/color]);`}</Text>
            <Text className="code-line">{`        icon_button_proto->init = icon_button_init;`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b]Parent [bgcolor=#eee] init [/bgcolor] must be called manually.[/b] LCUI does not automatically call the parent type's [bgcolor=#eee] init [/bgcolor] when creating a subtype widget. If you forget to call [bgcolor=#eee] proto->proto->init(w) [/bgcolor], the widget will lack the parent type's base behaviors (e.g. [bgcolor=#eee] button [/bgcolor] won't register click events or set default styles).`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Widget lifecycle</h2>
        <p>{`[bgcolor=#eee] ui_widget_prototype_t [/bgcolor] has several callback hooks covering each lifecycle stage:`}</p>
        <ul>
          <li><Text>{`[b][bgcolor=#eee] init [/bgcolor][/b] — When the widget is created. Responsible for initializing owned data, child widgets, and event bindings.`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] destroy [/bgcolor][/b] — When the widget is destroyed. Responsible for releasing owned memory and unbinding events.`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] update [/bgcolor][/b] — When the widget needs updating, receives a [bgcolor=#eee] ui_task_type_t [/bgcolor] parameter indicating the update type (style update, property update, etc.). Called by LCUI during each frame's update loop.`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] setattr [/bgcolor][/b] — When an XML attribute is set (for responding to attribute changes).`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] settext [/bgcolor][/b] — When text content is set.`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] sizehint [/bgcolor][/b] — Estimates the widget's natural size constraints (min/max content size) without external constraints. The layout engine uses this when computing child widget sizes.`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] resize [/bgcolor][/b] — When the widget size changes. Receives the new content area width and height as parameters.`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] paint [/bgcolor][/b] — When the widget needs to be redrawn. Used for custom drawing logic.`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Custom painting</h2>
        <p>{`Override the [bgcolor=#eee] paint [/bgcolor] callback to draw custom graphics on a widget:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/handbook/customization/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">src/my_widget.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]my_widget_paint[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w, [color=#953800]pd_context_t[/color] *paint_ctx,[/color]`}</Text>
            <Text className="code-line">{`[color=#0550ae]                              [color=#953800]ui_widget_actual_style_t[/color] *style)[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        [color=#953800]pd_canvas_t[/color] *canvas = paint_ctx->canvas;`}</Text>
            <Text className="code-line">{`        [color=#953800]pd_color_t[/color] color = pd_color_from_rgb([color=#0550ae]255[/color], [color=#0550ae]0[/color], [color=#0550ae]0[/color]);`}</Text>
            <Text className="code-line">{`        [color=#953800]pd_rect_t[/color] rect = { style->left, style->top, w->width, w->height };`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`        pd_canvas_fill_rect(canvas, color, rect);`}</Text>
            <Text className="code-line">{`}`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]register_my_widget[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        [color=#953800]ui_widget_prototype_t[/color] *proto =`}</Text>
            <Text className="code-line">{`            ui_create_widget_prototype([color=#0a3069]"my-widget"[/color], [color=#0550ae]NULL[/color]);`}</Text>
            <Text className="code-line">{`        proto->paint = my_widget_paint;`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#eee] paint [/bgcolor] is only for drawing.[/b] Widget size estimation is handled by [bgcolor=#eee] sizehint [/bgcolor]; do not modify widget size or the child widget tree inside [bgcolor=#eee] paint [/bgcolor], as it may cause infinite loops.`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Caveats</h2>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#eee] destroy [/bgcolor] must release owned resources.[/b] If a custom widget allocated memory, opened files, or subscribed to system events (e.g. [bgcolor=#eee] lcui_settings_on_deserialize [/bgcolor]) in [bgcolor=#eee] init [/bgcolor], it must perform symmetric cleanup in [bgcolor=#eee] destroy [/bgcolor].`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b]Custom widgets must be registered after [bgcolor=#eee] lcui_init() [/bgcolor] and before first instantiation.[/b] Calling your [bgcolor=#eee] register_xxx() [/bgcolor] function right at the start of [bgcolor=#eee] main() [/bgcolor] is the safest place, ensuring all consumers (XML loading, router instantiation, TSX compilation results) can see the prototype.`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
