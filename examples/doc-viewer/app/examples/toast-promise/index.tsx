import { Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function ToastPromiseDemo() {
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
        <Widget className="code-block-body active" data-language="c" data-file="main.c" data-source="examples/toast-promise/1">
          <CodeBlockCopy />
          <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *pending_toast;`}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]int[/color] pending_timer;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]on_pending_destroy[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *toast, [color=#953800]ui_event_t[/color] *event,[/color]`}</Text>
          <Text className="code-line">{`[color=#0550ae]                               [color=#953800]void[/color] *arg)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#cf222e]if[/color] (toast != pending_toast) {`}</Text>
          <Text className="code-line">{`                [color=#cf222e]return[/color];`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`        pending_toast = [color=#0550ae]NULL[/color];`}</Text>
          <Text className="code-line">{`        [color=#cf222e]if[/color] (pending_timer) {`}</Text>
          <Text className="code-line">{`                ptk_clear_timer(pending_timer);`}</Text>
          <Text className="code-line">{`                pending_timer = [color=#0550ae]0[/color];`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]finish_promise[/color][color=#0550ae]([color=#953800]void[/color] *arg)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *toast = pending_toast;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        pending_timer = [color=#0550ae]0[/color];`}</Text>
          <Text className="code-line">{`        pending_toast = [color=#0550ae]NULL[/color];`}</Text>
          <Text className="code-line">{`        [color=#cf222e]if[/color] (!toast) {`}</Text>
          <Text className="code-line">{`                [color=#cf222e]return[/color];`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`        ui_toast_set_type(toast, UI_TOAST_TYPE_SUCCESS);`}</Text>
          <Text className="code-line">{`        ui_toast_set_title(toast, [color=#0a3069]"Event created"[/color]);`}</Text>
          <Text className="code-line">{`        ui_toast_set_description(toast, [color=#0a3069]"Your event has been created."[/color]);`}</Text>
          <Text className="code-line">{`        ui_toast_set_duration(toast, [color=#0550ae]5000[/color]);`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]show_promise[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *button, [color=#953800]ui_event_t[/color] *event, [color=#953800]void[/color] *arg)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_toast_options_t[/color] options = { [color=#0550ae]0[/color] };`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *toast;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        [color=#cf222e]if[/color] (pending_toast) {`}</Text>
          <Text className="code-line">{`                [color=#953800]ui_widget_t[/color] *old = pending_toast;`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`                pending_toast = [color=#0550ae]NULL[/color];`}</Text>
          <Text className="code-line">{`                ui_toast_close(old);`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`        [color=#cf222e]if[/color] (pending_timer) {`}</Text>
          <Text className="code-line">{`                ptk_clear_timer(pending_timer);`}</Text>
          <Text className="code-line">{`                pending_timer = [color=#0550ae]0[/color];`}</Text>
          <Text className="code-line">{`        }`}</Text>
          <Text className="code-line">{`        options.type = UI_TOAST_TYPE_LOADING;`}</Text>
          <Text className="code-line">{`        options.title = [color=#0a3069]"Creating event..."[/color];`}</Text>
          <Text className="code-line">{`        options.description = [color=#0a3069]"This may take a moment."[/color];`}</Text>
          <Text className="code-line">{`        options.duration = UI_TOAST_DURATION_PERSISTENT;`}</Text>
          <Text className="code-line">{`        toast = ui_toast_show(&options);`}</Text>
          <Text className="code-line">{`        pending_toast = toast;`}</Text>
          <Text className="code-line">{`        ui_widget_on(toast, [color=#0a3069]"destroy"[/color], on_pending_destroy, [color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        pending_timer = ptk_set_timeout([color=#0550ae]2000[/color], finish_promise, [color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`}`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]toast_promise_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
          <Text className="code-line">{`{`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *row = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button = ui_create_button();`}</Text>
          <Text className="code-line">{` `}</Text>
          <Text className="code-line">{`        ui_widget_add_class(row, [color=#0a3069]"toast-demo-row"[/color]);`}</Text>
          <Text className="code-line">{`        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);`}</Text>
          <Text className="code-line">{`        ui_button_set_text(button, [color=#0a3069]"Create event"[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_on(button, [color=#0a3069]"click"[/color], show_promise, [color=#0550ae]NULL[/color]);`}</Text>
          <Text className="code-line">{`        ui_widget_append(row, button);`}</Text>
          <Text className="code-line">{`        ui_widget_append(parent, row);`}</Text>
          <Text className="code-line">{`}`}</Text>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
