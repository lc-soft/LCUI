import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import ToastBasicDemo from "../../../../examples/toast-basic/index";
import ToastTypesDemo from "../../../../examples/toast-types/index";
import ToastActionDemo from "../../../../examples/toast-action/index";
import ToastPromiseDemo from "../../../../examples/toast-promise/index";
import FieldTable from "../../../../components/field-table";
import { toastFields } from "../../../../widget-fields/toast";

export default function ToastDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Toast</h1>
      <p className="doc-page-desc">{`Toast（轻提示）会在页面角落短暂显示一条消息，可用于操作反馈、状态通知和需要用户确认的轻量交互。`}</p>
      <ToastBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：保存成功、发送失败、后台任务进行中等短暂状态反馈`}</Text></li>
          <li><Text>{`[b]适用[/b]：需要提供撤销、确认或取消等轻量操作的场景`}</Text></li>
          <li><Text>{`[b]不适用[/b]：必须打断用户并等待处理的确认流程（使用 [bgcolor=#eee] alert-dialog [/bgcolor]）`}</Text></li>
          <li><Text>{`[b]不适用[/b]：需要常驻页面、可拖动或复杂编辑的内容`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="c" data-source="zh-CN/widgets/toast/0">
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
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`第一次调用 [bgcolor=#eee] ui_toast_show() [/bgcolor] 时会在 [bgcolor=#eee] ui_root() [/bgcolor] 下创建单例 [bgcolor=#eee] toast-viewport [/bgcolor]。后续 Toast 会以右下角、从下往上的顺序堆叠。`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>类型</h2>
        <p>{`[bgcolor=#eee] type [/bgcolor] 控制状态图标：`}</p>
        <ul>
          <li><Text>{`[bgcolor=#eee] UI_TOAST_TYPE_DEFAULT [/bgcolor]：不显示图标`}</Text></li>
          <li><Text>{`[bgcolor=#eee] UI_TOAST_TYPE_SUCCESS [/bgcolor]`}</Text></li>
          <li><Text>{`[bgcolor=#eee] UI_TOAST_TYPE_INFO [/bgcolor]`}</Text></li>
          <li><Text>{`[bgcolor=#eee] UI_TOAST_TYPE_WARNING [/bgcolor]`}</Text></li>
          <li><Text>{`[bgcolor=#eee] UI_TOAST_TYPE_ERROR [/bgcolor]`}</Text></li>
          <li><Text>{`[bgcolor=#eee] UI_TOAST_TYPE_LOADING [/bgcolor]：使用 [bgcolor=#eee] spinner [/bgcolor] 部件显示加载图标`}</Text></li>
        </ul>
        <Widget className="code-block" data-language="c" data-source="zh-CN/widgets/toast/1">
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
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <ToastTypesDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>操作</h2>
        <p>{`单个操作按钮可以通过 options 直接配置：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/widgets/toast/2">
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
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <ToastActionDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>自定义操作</h2>
        <p>{`需要确认/取消等多个按钮时，通过 [bgcolor=#eee] ui_toast_get_actions() [/bgcolor] 取得操作区域，然后追加普通 [bgcolor=#eee] button [/bgcolor] 部件：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/widgets/toast/3">
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
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`[bgcolor=#eee] ui_toast_get_actions() [/bgcolor] 返回的是 Toast 内部的 [bgcolor=#eee] toast-actions [/bgcolor] 容器；调用后该容器会显示。取消按钮可以用同样的方式创建并追加。`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>Promise</h2>
        <p>{`可以先用 [bgcolor=#eee] UI_TOAST_TYPE_LOADING [/bgcolor] 显示进行中状态，任务完成后再更新类型、标题和持续时间：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/widgets/toast/4">
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
                  <Text className="code-line">{`[color=#6e7781]/* 任务完成后 */[/color]`}</Text>
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
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <ToastPromiseDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>持续时间</h2>
        <ul>
          <li><Text>{`默认：[bgcolor=#eee] 5000ms [/bgcolor] 后自动关闭`}</Text></li>
          <li><Text>{`[bgcolor=#eee] UI_TOAST_DURATION_PERSISTENT [/bgcolor]：常驻，直到调用 [bgcolor=#eee] ui_toast_close() [/bgcolor] 或点击关闭按钮`}</Text></li>
          <li><Text>{`Loading 类型默认常驻，避免在加载完成前消失`}</Text></li>
        </ul>
        <p>{`交互型 Toast（确认/取消）应使用 [bgcolor=#eee] UI_TOAST_DURATION_PERSISTENT [/bgcolor]。`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>注意事项</h2>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`Toast 默认带有关闭按钮；点击关闭、执行操作回调或超时后都会移除当前 Toast。关闭时会取消关联的定时器。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]当前实现不包含 shadcn 的滑入/滑出、缩放和 swipe 手势。[/b] 视觉样式（右下角堆叠、圆角、边框、阴影、图标和排版）与 shadcn Toast 对齐，动画和手势保持静态以适配当前 LCUI 能力。`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>Toast</h3>
        <p>{`Toast 消息容器及其选项。`}</p>
        <FieldTable fields={toastFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
