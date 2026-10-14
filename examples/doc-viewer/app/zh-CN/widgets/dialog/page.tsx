import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import DialogBasicDemo from "../../../examples/dialog-basic/index";
import DialogCloseButtonDemo from "../../../examples/dialog-close-button/index";
import DialogNoCloseButtonDemo from "../../../examples/dialog-no-close-button/index";
import DialogStickyFooterDemo from "../../../examples/dialog-sticky-footer/index";
import DialogScrollableContentDemo from "../../../examples/dialog-scrollable-content/index";
import FieldTable from "../../../components/field-table";
import { dialogFields } from "../../../widget-fields/dialog";
import { dialogTriggerFields } from "../../../widget-fields/dialog-trigger";
import { dialogOverlayFields } from "../../../widget-fields/dialog-overlay";
import { dialogContentFields } from "../../../widget-fields/dialog-content";
import { dialogHeaderFields } from "../../../widget-fields/dialog-header";
import { dialogTitleFields } from "../../../widget-fields/dialog-title";
import { dialogDescriptionFields } from "../../../widget-fields/dialog-description";
import { dialogFooterFields } from "../../../widget-fields/dialog-footer";
import { dialogCloseFields } from "../../../widget-fields/dialog-close";

export default function DialogDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Dialog</h1>
      <p className="doc-page-desc">{`叠加在主窗口或其他对话框之上的模态窗口，其下方的内容进入不可交互状态。`}</p>
      <DialogBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：表单编辑、资料修改等需要在当前页面上完成的任务`}</Text></li>
          <li><Text>{`[b]适用[/b]：展示详情、引导流程等临时打断当前操作的辅助内容`}</Text></li>
          <li><Text>{`[b]不适用[/b]：需要用户明确选择、不允许隐式关闭的确认流程（用 [bgcolor=#eee] alert-dialog [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/dialog/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Dialog[/color], [color=#8250df]DialogTrigger[/color], [color=#8250df]DialogOverlay[/color], [color=#8250df]DialogContent[/color], [color=#8250df]DialogHeader[/color], [color=#8250df]DialogTitle[/color], [color=#8250df]DialogDescription[/color], [color=#8250df]DialogFooter[/color], [color=#8250df]DialogClose[/color], [color=#8250df]Portal[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/dialog/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Dialog[/color]>`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]DialogTrigger[/color]>[/color]Open[color=#116329]</[color=#0550ae]DialogTrigger[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]Portal[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]DialogOverlay[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]DialogContent[/color]>[/color]`}</Text>
            <Text className="code-line">{`        [color=#116329]<[color=#0550ae]DialogHeader[/color]>[/color]`}</Text>
            <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DialogTitle[/color]>[/color]Are you absolutely sure?[color=#116329]</[color=#0550ae]DialogTitle[/color]>[/color]`}</Text>
            <Text className="code-line">{`          [color=#116329]<[color=#0550ae]DialogDescription[/color]>[/color]`}</Text>
            <Text className="code-line">{`            This action cannot be undone. This will permanently delete your`}</Text>
            <Text className="code-line">{`            account and remove your data from our servers.`}</Text>
            <Text className="code-line">{`          [color=#116329]</[color=#0550ae]DialogDescription[/color]>[/color]`}</Text>
            <Text className="code-line">{`        [color=#116329]</[color=#0550ae]DialogHeader[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]</[color=#0550ae]DialogContent[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]</[color=#0550ae]DialogOverlay[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]</[color=#0550ae]Portal[/color]>[/color]`}</Text>
            <Text className="code-line">{`</[color=#8250df]Dialog[/color]>`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>组合</h2>
        <p>{`使用以下组合来构建 [bgcolor=#eee] Dialog [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/dialog/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`Dialog`}</Text>
            <Text className="code-line">{`├── DialogTrigger`}</Text>
            <Text className="code-line">{`└── Portal`}</Text>
            <Text className="code-line">{`    └── DialogOverlay`}</Text>
            <Text className="code-line">{`        └── DialogContent`}</Text>
            <Text className="code-line">{`            ├── DialogHeader`}</Text>
            <Text className="code-line">{`            │   ├── DialogTitle`}</Text>
            <Text className="code-line">{`            │   └── DialogDescription`}</Text>
            <Text className="code-line">{`            └── DialogFooter`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>自定义关闭按钮</h2>
        <p>{`在 Footer 中放置自己的关闭按钮（[bgcolor=#eee] DialogClose [/bgcolor] 或带 [bgcolor=#eee] data-dialog-close [/bgcolor] 的任意部件）。`}</p>
        <DialogCloseButtonDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>无关闭按钮</h2>
        <p>{`不放置 [bgcolor=#eee] DialogClose [/bgcolor] 节点即可；使用默认子树构建时可调用 [bgcolor=#eee] ui_dialog_show_close_button() [/bgcolor] 关闭右上角按钮。`}</p>
        <DialogNoCloseButtonDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>固定 Footer</h2>
        <p>{`内容区滚动时，Footer 保持可见。`}</p>
        <DialogStickyFooterDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>可滚动内容</h2>
        <p>{`长内容可以在 [bgcolor=#eee] DialogContent [/bgcolor] 内使用 [bgcolor=#eee] ScrollArea [/bgcolor] 滚动，标题保持可见。`}</p>
        <DialogScrollableContentDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>声明式关联</h2>
        <p>{`脱离 [bgcolor=#eee] Dialog [/bgcolor] 子树的普通部件可以用关联键松散绑定，无需嵌套在统一容器中：`}</p>
        <Widget className="code-block" data-language="xml" data-source="zh-CN/widgets/dialog/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">dialog-xml</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]data-dialog-trigger[/color]=[color=#0a3069]"profile"[/color]>[/color]Edit Profile[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]dialog-overlay[/color] [color=#0550ae]data-dialog-target[/color]=[color=#0a3069]"profile"[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]dialog-content[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]text[/color] [color=#0550ae]class[/color]=[color=#0a3069]"text-sm"[/color]>[/color]Clicking X or Esc closes the dialog.[color=#116329]</[color=#0550ae]text[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]data-dialog-close[/color]=[color=#0a3069]"profile"[/color]>[/color]Dismiss[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]</[color=#0550ae]dialog-content[/color]>[/color]`}</Text>
            <Text className="code-line">{`[color=#116329]</[color=#0550ae]dialog-overlay[/color]>[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[bgcolor=#eee] data-dialog-trigger [/bgcolor] 点击切换对应对话框，[bgcolor=#eee] data-dialog-target [/bgcolor] 指定浮层节点，[bgcolor=#eee] data-dialog-close [/bgcolor] 点击关闭。同一键可以有多个 trigger 与 close，但只允许一个 target。`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>注意事项</h2>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]当前实现不包含打开/关闭动画与焦点陷阱。[/b] 遮罩出现后背景内容不可点击，但 Tab 键仍可移动焦点到背景部件。需要严格模态行为时，请在应用层控制。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`按 Esc、点击遮罩空白处或任意 [bgcolor=#eee] DialogClose [/bgcolor] / [bgcolor=#eee] data-dialog-close [/bgcolor] 节点都会关闭对话框；点击内容内部不会。Esc 生效要求焦点位于对话框内容子树内（打开时焦点会自动落在首个可聚焦元素；若内容中没有任何可聚焦部件且未注入关闭按钮，Esc 无法关闭）。shadcn 的 RTL 排版依赖 [bgcolor=#eee] direction: rtl [/bgcolor]，LCUI 暂不支持。`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>Dialog</h3>
        <p>{`对话框状态容器，管理开关、遮罩挂载与焦点恢复。`}</p>
        <FieldTable fields={dialogFields} locale="zh-CN" />
        <h3>DialogTrigger</h3>
        <p>{`打开或关闭对话框的触发器，内部默认创建一个按钮。`}</p>
        <FieldTable fields={dialogTriggerFields} locale="zh-CN" />
        <h3>DialogOverlay</h3>
        <p>{`通过 Portal 挂载到 root 层级的全屏遮罩，负责拦截背景点击。`}</p>
        <FieldTable fields={dialogOverlayFields} locale="zh-CN" />
        <h3>DialogContent</h3>
        <p>{`居中显示在遮罩上的对话框内容容器。`}</p>
        <FieldTable fields={dialogContentFields} locale="zh-CN" />
        <h3>DialogHeader</h3>
        <p>{`标题与描述的组合头部。`}</p>
        <FieldTable fields={dialogHeaderFields} locale="zh-CN" />
        <h3>DialogTitle</h3>
        <p>{`对话框标题。`}</p>
        <FieldTable fields={dialogTitleFields} locale="zh-CN" />
        <h3>DialogDescription</h3>
        <p>{`对话框说明文本。`}</p>
        <FieldTable fields={dialogDescriptionFields} locale="zh-CN" />
        <h3>DialogFooter</h3>
        <p>{`操作按钮区域，默认右对齐。`}</p>
        <FieldTable fields={dialogFooterFields} locale="zh-CN" />
        <h3>DialogClose</h3>
        <p>{`关闭对话框的按钮。默认注入的是 ghost + icon 变体组合的标准按钮（绝对定位在右上角）； 任何带 [bgcolor=#eee] data-dialog-close [/bgcolor] 的节点都有同样的关闭行为。`}</p>
        <FieldTable fields={dialogCloseFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
