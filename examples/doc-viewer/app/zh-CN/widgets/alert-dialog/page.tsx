import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import AlertDialogBasicDemo from "../../../examples/alert-dialog-basic/index";
import AlertDialogSmallDemo from "../../../examples/alert-dialog-small/index";
import AlertDialogDestructiveDemo from "../../../examples/alert-dialog-destructive/index";
import AlertDialogCallbackDemo from "../../../examples/alert-dialog-callback/index";
import FieldTable from "../../../components/field-table";
import { alertDialogFields } from "../../../widget-fields/alert-dialog";
import { alertDialogTriggerFields } from "../../../widget-fields/alert-dialog-trigger";
import { alertDialogOverlayFields } from "../../../widget-fields/alert-dialog-overlay";
import { alertDialogContentFields } from "../../../widget-fields/alert-dialog-content";
import { alertDialogHeaderFields } from "../../../widget-fields/alert-dialog-header";
import { alertDialogMediaFields } from "../../../widget-fields/alert-dialog-media";
import { alertDialogTitleFields } from "../../../widget-fields/alert-dialog-title";
import { alertDialogDescriptionFields } from "../../../widget-fields/alert-dialog-description";
import { alertDialogFooterFields } from "../../../widget-fields/alert-dialog-footer";
import { alertDialogCancelFields } from "../../../widget-fields/alert-dialog-cancel";
import { alertDialogActionFields } from "../../../widget-fields/alert-dialog-action";

export default function AlertDialogDocPage() {
  return (
    <Widget className="doc-page">
      <h1>AlertDialog</h1>
      <p className="doc-page-desc">{`打断用户、呈现重要内容并要求明确响应的模态对话框。`}</p>
      <AlertDialogBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：删除等破坏性或不可逆操作前的确认`}</Text></li>
          <li><Text>{`[b]适用[/b]：必须得到用户"确认 / 取消"明确回应才能继续的流程`}</Text></li>
          <li><Text>{`[b]不适用[/b]：允许被 Esc 或遮罩点击关闭的普通面板（用 [bgcolor=#eee] dialog [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/alert-dialog/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]AlertDialog[/color], [color=#8250df]AlertDialogTrigger[/color], [color=#8250df]AlertDialogOverlay[/color], [color=#8250df]AlertDialogContent[/color], [color=#8250df]AlertDialogHeader[/color], [color=#8250df]AlertDialogMedia[/color], [color=#8250df]AlertDialogTitle[/color], [color=#8250df]AlertDialogDescription[/color], [color=#8250df]AlertDialogFooter[/color], [color=#8250df]AlertDialogCancel[/color], [color=#8250df]AlertDialogAction[/color], [color=#8250df]Portal[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/alert-dialog/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]AlertDialog[/color]>`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]AlertDialogTrigger[/color]>[/color]Show Dialog[color=#116329]</[color=#0550ae]AlertDialogTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]Portal[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]AlertDialogOverlay[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`      [color=#116329]<[color=#0550ae]AlertDialogContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AlertDialogHeader[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AlertDialogTitle[/color]>[/color]Are you absolutely sure?[color=#116329]</[color=#0550ae]AlertDialogTitle[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AlertDialogDescription[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">9</Text>
                  <Text className="code-line">{`            This action cannot be undone. This will permanently delete your`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">10</Text>
                  <Text className="code-line">{`            account and remove your data from our servers.`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">11</Text>
                  <Text className="code-line">{`          [color=#116329]</[color=#0550ae]AlertDialogDescription[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">12</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AlertDialogHeader[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">13</Text>
                  <Text className="code-line">{`        [color=#116329]<[color=#0550ae]AlertDialogFooter[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">14</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AlertDialogCancel[/color]>[/color]Cancel[color=#116329]</[color=#0550ae]AlertDialogCancel[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">15</Text>
                  <Text className="code-line">{`          [color=#116329]<[color=#0550ae]AlertDialogAction[/color]>[/color]Continue[color=#116329]</[color=#0550ae]AlertDialogAction[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">16</Text>
                  <Text className="code-line">{`        [color=#116329]</[color=#0550ae]AlertDialogFooter[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">17</Text>
                  <Text className="code-line">{`      [color=#116329]</[color=#0550ae]AlertDialogContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">18</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]AlertDialogOverlay[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">19</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]Portal[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">20</Text>
                  <Text className="code-line">{`</[color=#8250df]AlertDialog[/color]>`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>组合</h2>
        <p>{`使用以下组合来构建 [bgcolor=#eee] AlertDialog [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/alert-dialog/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`AlertDialog`}</Text>
                <Text className="code-line">{`├── AlertDialogTrigger`}</Text>
                <Text className="code-line">{`└── Portal`}</Text>
                <Text className="code-line">{`    └── AlertDialogOverlay`}</Text>
                <Text className="code-line">{`        └── AlertDialogContent`}</Text>
                <Text className="code-line">{`            ├── AlertDialogHeader`}</Text>
                <Text className="code-line">{`            │   ├── AlertDialogMedia`}</Text>
                <Text className="code-line">{`            │   ├── AlertDialogTitle`}</Text>
                <Text className="code-line">{`            │   └── AlertDialogDescription`}</Text>
                <Text className="code-line">{`            └── AlertDialogFooter`}</Text>
                <Text className="code-line">{`                ├── AlertDialogCancel`}</Text>
                <Text className="code-line">{`                └── AlertDialogAction`}</Text>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>小号尺寸</h2>
        <p>{`在 [bgcolor=#eee] AlertDialogContent [/bgcolor] 上使用 [bgcolor=#eee] size="sm" [/bgcolor] 让对话框更窄，此时 Footer 的两个按钮等宽撑满内容区。`}</p>
        <AlertDialogSmallDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>破坏性操作</h2>
        <p>{`使用 [bgcolor=#eee] variant="destructive" [/bgcolor] 的 [bgcolor=#eee] AlertDialogAction [/bgcolor] 表达删除等破坏性动作。`}</p>
        <AlertDialogDestructiveDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>取消与确认回调</h2>
        <p>{`[bgcolor=#eee] AlertDialogCancel [/bgcolor] / [bgcolor=#eee] AlertDialogAction [/bgcolor] 关闭对话框后分别发出 [bgcolor=#eee] cancel [/bgcolor] / [bgcolor=#eee] action [/bgcolor] 事件，业务结果由调用方处理；也可以给脱离子树的普通按钮设置 [bgcolor=#eee] data-dialog-cancel [/bgcolor] / [bgcolor=#eee] data-dialog-action [/bgcolor] 关联键参与确认流程。`}</p>
        <AlertDialogCallbackDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>注意事项</h2>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]按 Esc 或点击遮罩不会关闭确认对话框[/b]，用户只能通过取消或确认作出选择；显式关闭仍需 [bgcolor=#eee] ui_dialog_close() [/bgcolor]。当前实现不包含打开/关闭动画与焦点陷阱。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`确认动作是同步事件，不支持异步等待与二次确认流程；需要时可监听 [bgcolor=#eee] action [/bgcolor] 事件在应用层实现。shadcn 的 Media 小节依赖 CSS grid 双列布局，LCUI 的 [bgcolor=#eee] AlertDialogMedia [/bgcolor] 采用纵向居中堆叠（见"破坏性操作"示例）；RTL 排版 LCUI 暂不支持。`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>AlertDialog</h3>
        <p>{`确认对话框状态容器，复用 Dialog 的开关 API，默认拒绝隐式关闭。`}</p>
        <FieldTable fields={alertDialogFields} locale="zh-CN" />
        <h3>AlertDialogTrigger</h3>
        <p>{`打开确认对话框的触发器，内部默认创建一个按钮。`}</p>
        <FieldTable fields={alertDialogTriggerFields} locale="zh-CN" />
        <h3>AlertDialogOverlay</h3>
        <p>{`通过 Portal 挂载到 root 层级的全屏遮罩，不会响应点击关闭。`}</p>
        <FieldTable fields={alertDialogOverlayFields} locale="zh-CN" />
        <h3>AlertDialogContent</h3>
        <p>{`居中显示在遮罩上的确认内容容器。`}</p>
        <FieldTable fields={alertDialogContentFields} locale="zh-CN" />
        <h3>AlertDialogHeader</h3>
        <p>{`媒体、标题与描述的组合头部，默认左对齐，仅 [bgcolor=#eee] size="sm" [/bgcolor] 时居中。`}</p>
        <FieldTable fields={alertDialogHeaderFields} locale="zh-CN" />
        <h3>AlertDialogMedia</h3>
        <p>{`头部媒体区域，可放置图标或图片。`}</p>
        <FieldTable fields={alertDialogMediaFields} locale="zh-CN" />
        <h3>AlertDialogTitle</h3>
        <p>{`确认对话框标题。`}</p>
        <FieldTable fields={alertDialogTitleFields} locale="zh-CN" />
        <h3>AlertDialogDescription</h3>
        <p>{`确认对话框说明文本。`}</p>
        <FieldTable fields={alertDialogDescriptionFields} locale="zh-CN" />
        <h3>AlertDialogFooter</h3>
        <p>{`取消与确认按钮区域，默认右对齐；[bgcolor=#eee] size="sm" [/bgcolor] 时按钮等宽撑满。`}</p>
        <FieldTable fields={alertDialogFooterFields} locale="zh-CN" />
        <h3>AlertDialogCancel</h3>
        <p>{`取消按钮，点击后关闭对话框并发出 [bgcolor=#eee] cancel [/bgcolor] 事件。`}</p>
        <FieldTable fields={alertDialogCancelFields} locale="zh-CN" />
        <h3>AlertDialogAction</h3>
        <p>{`确认按钮，点击后关闭对话框并发出 [bgcolor=#eee] action [/bgcolor] 事件。`}</p>
        <FieldTable fields={alertDialogActionFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
