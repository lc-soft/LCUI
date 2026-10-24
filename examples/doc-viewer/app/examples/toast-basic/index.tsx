import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function ToastBasicDemo() {
  return (
    <DemoProvider>
      <Widget $ref="preview" className="demo-preview" />
      <Widget className="code-block">
        <Widget className="code-block-header">
          <Widget className="demo-files">
            <Text className="demo-tab active" data-language="c" data-value="main.c">main.c</Text>
          </Widget>
          <Widget className="demo-languages">
            <Text className="demo-tab active" data-value="c">C</Text>
          </Widget>
        </Widget>
        <Widget className="code-block-body active" data-language="c" data-file="main.c" data-source="examples/toast-basic/1">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]show_toast[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *button, [color=#953800]ui_event_t[/color] *event, [color=#953800]void[/color] *arg)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_toast_options_t[/color] options = { [color=#0550ae]0[/color] };`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        options.title = [color=#0a3069]"Event created"[/color];`}</Text>
          <Text className="code-line">{`        options.description = [color=#0a3069]"Sunday, December 3 at 9:00 AM"[/color];`}</Text>
          <Text className="code-line">{`        ui_toast_show(&options);`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]toast_basic_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *row = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button = ui_create_button();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_add_class(row, [color=#0a3069]"toast-demo-row"[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);`}</Text>
          <Text className="code-line">{`        ui_button_set_text(button, [color=#0a3069]"Show toast"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_on(button, [color=#0a3069]"click"[/color], show_toast, [color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(row, button);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, row);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
