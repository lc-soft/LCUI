import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import ToastBasicDemo from "../../../examples/toast-basic/index";
import ToastTypesDemo from "../../../examples/toast-types/index";
import ToastActionDemo from "../../../examples/toast-action/index";
import ToastPromiseDemo from "../../../examples/toast-promise/index";
import FieldTable from "../../../components/field-table";
import { toastFields } from "../../../widget-fields/toast";

export default function ToastDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Toast</h1>
      <p className="doc-page-desc">{`A succinct message displayed temporarily in a page corner for feedback, status notifications, and lightweight confirmations.`}</p>
      <ToastBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Use[/b]: transient feedback such as saved, failed, or a background task in progress`}</Text></li>
          <li><Text>{`[b]Use[/b]: lightweight actions such as undo, confirm, or cancel`}</Text></li>
          <li><Text>{`[b]Don't use[/b]: blocking confirmation flows (use [bgcolor=#eee] alert-dialog [/bgcolor])`}</Text></li>
          <li><Text>{`[b]Don't use[/b]: persistent, draggable, or complex editable content`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="c" data-source="en/widgets/toast/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
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
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]notify_saved[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`        [color=#953800]ui_toast_options_t[/color] options = { [color=#0550ae]0[/color] };`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`        options.type = UI_TOAST_TYPE_SUCCESS;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`        options.title = [color=#0a3069]"Saved"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`        options.description = [color=#0a3069]"Your changes have been saved."[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`        ui_toast_show(&options);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`The first [bgcolor=#eee] ui_toast_show() [/bgcolor] call creates a singleton [bgcolor=#eee] toast-viewport [/bgcolor] under [bgcolor=#eee] ui_root() [/bgcolor]. Later toasts stack bottom-right, with newer toasts below older ones.`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>Types</h2>
        <p>{`[bgcolor=#eee] type [/bgcolor] selects the status icon:`}</p>
        <ul>
          <li><Text>{`[bgcolor=#eee] UI_TOAST_TYPE_DEFAULT [/bgcolor]: no icon`}</Text></li>
          <li><Text>{`[bgcolor=#eee] UI_TOAST_TYPE_SUCCESS [/bgcolor]`}</Text></li>
          <li><Text>{`[bgcolor=#eee] UI_TOAST_TYPE_INFO [/bgcolor]`}</Text></li>
          <li><Text>{`[bgcolor=#eee] UI_TOAST_TYPE_WARNING [/bgcolor]`}</Text></li>
          <li><Text>{`[bgcolor=#eee] UI_TOAST_TYPE_ERROR [/bgcolor]`}</Text></li>
          <li><Text>{`[bgcolor=#eee] UI_TOAST_TYPE_LOADING [/bgcolor]: uses the [bgcolor=#eee] spinner [/bgcolor] widget`}</Text></li>
        </ul>
        <Widget className="code-block" data-language="c" data-source="en/widgets/toast/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "144px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#953800]ui_toast_options_t[/color] options = { [color=#0550ae]0[/color] };`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`options.type = UI_TOAST_TYPE_LOADING;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`options.title = [color=#0a3069]"Creating event..."[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`options.duration = UI_TOAST_DURATION_PERSISTENT;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`ui_toast_show(&options);`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
            </ScrollArea>
          </Widget>
        </Widget>
        <ToastTypesDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Action</h2>
        <p>{`A single action button can be configured through options:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/widgets/toast/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]on_undo[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *toast, [color=#953800]void[/color] *arg)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`        ui_toast_close(toast);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]notify_with_action[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`        [color=#953800]ui_toast_options_t[/color] options = { [color=#0550ae]0[/color] };`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`        options.title = [color=#0a3069]"Event created"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`        options.description = [color=#0a3069]"You can undo this action."[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`        options.action_text = [color=#0a3069]"Undo"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`        options.on_action = on_undo;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`        ui_toast_show(&options);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
            </ScrollArea>
          </Widget>
        </Widget>
        <ToastActionDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Custom actions</h2>
        <p>{`For multiple buttons such as confirm and cancel, get the action row with [bgcolor=#eee] ui_toast_get_actions() [/bgcolor] and append regular [bgcolor=#eee] button [/bgcolor] widgets:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/widgets/toast/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]on_confirm[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *button, [color=#953800]ui_event_t[/color] *event, [color=#953800]void[/color] *arg)[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`        ui_toast_close(event->data);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]confirm_delete[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`{`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`        [color=#953800]ui_toast_options_t[/color] options = { [color=#0550ae]0[/color] };`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *toast;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *actions;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *confirm;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`        options.title = [color=#0a3069]"Delete event?"[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`        options.description = [color=#0a3069]"This action cannot be undone."[/color];`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`        options.duration = UI_TOAST_DURATION_PERSISTENT;`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        toast = ui_toast_show(&options);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`        actions = ui_toast_get_actions(toast);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`        confirm = ui_create_button();`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`        ui_button_set_variant(confirm, UI_BUTTON_VARIANT_PRIMARY);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">21</Text>
                  <Text className="code-line">{`        ui_button_set_size(confirm, UI_BUTTON_SIZE_SM);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">22</Text>
                  <Text className="code-line">{`        ui_button_set_text(confirm, [color=#0a3069]"Confirm"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">23</Text>
                  <Text className="code-line">{`        ui_widget_on(confirm, [color=#0a3069]"click"[/color], on_confirm, toast);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">24</Text>
                  <Text className="code-line">{`        ui_widget_append(actions, confirm);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">25</Text>
                  <Text className="code-line">{`}`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`[bgcolor=#eee] ui_toast_get_actions() [/bgcolor] returns the internal [bgcolor=#eee] toast-actions [/bgcolor] row and makes it visible. A cancel button can be created and appended the same way.`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>Promise</h2>
        <p>{`Start with [bgcolor=#eee] UI_TOAST_TYPE_LOADING [/bgcolor], then update the type, title, and duration when the task finishes:`}</p>
        <Widget className="code-block" data-language="c" data-source="en/widgets/toast/4">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "164px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#953800]ui_widget_t[/color] *toast = ui_toast_show(&loading_options);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{` `}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`[color=#6e7781]/* After the task completes */[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`ui_toast_set_type(toast, UI_TOAST_TYPE_SUCCESS);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`ui_toast_set_title(toast, [color=#0a3069]"Event created"[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`ui_toast_set_description(toast, [color=#0a3069]"Your event has been created."[/color]);`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`ui_toast_set_duration(toast, [color=#0550ae]5000[/color]);`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
            </ScrollArea>
          </Widget>
        </Widget>
        <ToastPromiseDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Duration</h2>
        <ul>
          <li><Text>{`Default: auto-close after [bgcolor=#eee] 5000ms [/bgcolor]`}</Text></li>
          <li><Text>{`[bgcolor=#eee] UI_TOAST_DURATION_PERSISTENT [/bgcolor]: keep until [bgcolor=#eee] ui_toast_close() [/bgcolor] or the close button is clicked`}</Text></li>
          <li><Text>{`Loading toasts are persistent by default so they do not disappear before loading finishes`}</Text></li>
        </ul>
        <p>{`Interactive toasts should use [bgcolor=#eee] UI_TOAST_DURATION_PERSISTENT [/bgcolor].`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>Caveats</h2>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`Toast includes a close button. Clicking it, invoking an action callback, or timing out removes the toast. The associated timer is cancelled when the toast is closed.`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]This implementation does not include shadcn's slide/scale transitions or swipe gestures.[/b] The visual style (bottom-right stacking, rounded corners, border, shadow, icons, and typography) follows shadcn Toast, while motion and gestures remain static to suit current LCUI capabilities.`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>Toast</h3>
        <p>{`Toast message container and its options.`}</p>
        <FieldTable fields={toastFields} locale="en" />
      </Widget>
    </Widget>
  );
}
