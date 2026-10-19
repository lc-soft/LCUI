import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import PopoverBasicDemo from "../../../examples/popover-basic/index";
import PopoverSimpleDemo from "../../../examples/popover-simple/index";
import PopoverAlignDemo from "../../../examples/popover-align/index";
import FieldTable from "../../../components/field-table";
import { popoverFields } from "../../../widget-fields/popover";
import { popoverTriggerFields } from "../../../widget-fields/popover-trigger";
import { popoverContentFields } from "../../../widget-fields/popover-content";
import { popoverHeaderFields } from "../../../widget-fields/popover-header";
import { popoverTitleFields } from "../../../widget-fields/popover-title";
import { popoverDescriptionFields } from "../../../widget-fields/popover-description";

export default function PopoverDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Popover</h1>
      <p className="doc-page-desc">{`点击触发器后在 Portal 中显示可交互内容的非模态浮层。`}</p>
      <PopoverBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：表单、设置面板、筛选器等需要承载可交互内容的场景`}</Text></li>
          <li><Text>{`[b]适用[/b]：希望点击触发器后展示一段结构化内容，同时保留页面其余部分可操作`}</Text></li>
          <li><Text>{`[b]不适用[/b]：纯文本、短提示（使用 [bgcolor=#eee] tooltip [/bgcolor]）`}</Text></li>
          <li><Text>{`[b]不适用[/b]：需要遮罩和焦点陷阱的确认流程（使用 [bgcolor=#eee] dialog [/bgcolor] / [bgcolor=#eee] alert-dialog [/bgcolor]）`}</Text></li>
          <li><Text>{`[b]不适用[/b]：长篇内容或需要持续滚动的复杂面板（使用页面、[bgcolor=#eee] scrollarea [/bgcolor] 或抽屉）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/popover/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Popover[/color], [color=#8250df]PopoverTrigger[/color], [color=#8250df]PopoverContent[/color], [color=#8250df]PopoverHeader[/color], [color=#8250df]PopoverTitle[/color], [color=#8250df]PopoverDescription[/color], [color=#8250df]Button[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/popover/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Popover[/color]>`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]PopoverTrigger[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]Open popover[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]</[color=#0550ae]PopoverTrigger[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]PopoverContent[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]PopoverHeader[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]PopoverTitle[/color]>[/color]Dimensions[color=#116329]</[color=#0550ae]PopoverTitle[/color]>[/color]`}</Text>
            <Text className="code-line">{`      [color=#116329]<[color=#0550ae]PopoverDescription[/color]>[/color]Set the dimensions for the layer.[color=#116329]</[color=#0550ae]PopoverDescription[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]</[color=#0550ae]PopoverHeader[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]</[color=#0550ae]PopoverContent[/color]>[/color]`}</Text>
            <Text className="code-line">{`</[color=#8250df]Popover[/color]>`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>组合</h2>
        <p>{`使用以下组合来构建 [bgcolor=#eee] Popover [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/popover/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`Popover`}</Text>
            <Text className="code-line">{`├── PopoverTrigger`}</Text>
            <Text className="code-line">{`└── PopoverContent`}</Text>
            <Text className="code-line">{`    ├── PopoverHeader`}</Text>
            <Text className="code-line">{`    │   ├── PopoverTitle`}</Text>
            <Text className="code-line">{`    │   └── PopoverDescription`}</Text>
            <Text className="code-line">{`    └── （任意可交互内容）`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>示例</h2>
        <h3>基础</h3>
        <p>{`一个包含标题和描述的基础 Popover。`}</p>
        <PopoverSimpleDemo />
        <h3>对齐</h3>
        <p>{`使用 [bgcolor=#eee] align [/bgcolor] 控制弹出层在方位轴上的对齐方式。`}</p>
        <PopoverAlignDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>注意事项</h2>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[b]Popover 是非模态浮层。[/b] 没有遮罩和焦点陷阱；点击外部控件时，点击会穿透并关闭 Popover，外部控件本身仍然生效。页面滚动时气泡会跟随触发器移动。按 Esc 关闭当前 Popover。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`同一时间只保留一个 active Popover；打开另一个 Popover 会关闭上一个。内部内容的点击不会关闭 Popover，需要关闭时调用 [bgcolor=#eee] ui_popover_close() [/bgcolor] 或点击触发器/外部。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]当前实现不包含边缘碰撞翻转和 resize 重定位。[/b] 也不提供 [bgcolor=#eee] PopoverAnchor [/bgcolor]；需要改变锚点时请把 [bgcolor=#eee] PopoverTrigger [/bgcolor] 放到目标位置。`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>Popover</h3>
        <p>{`弹出层状态容器，管理开关、Portal 和焦点恢复。`}</p>
        <FieldTable fields={popoverFields} locale="zh-CN" />
        <h3>PopoverTrigger</h3>
        <p>{`触发弹出层的包装层，按钮、图标或其他可点击部件放在其中。`}</p>
        <FieldTable fields={popoverTriggerFields} locale="zh-CN" />
        <h3>PopoverContent</h3>
        <p>{`承载可交互内容的 Portal 气泡，负责方位、对齐和偏移。`}</p>
        <FieldTable fields={popoverContentFields} locale="zh-CN" />
        <h3>PopoverHeader</h3>
        <p>{`标题与描述的组合头部。`}</p>
        <FieldTable fields={popoverHeaderFields} locale="zh-CN" />
        <h3>PopoverTitle</h3>
        <p>{`弹出层标题。`}</p>
        <FieldTable fields={popoverTitleFields} locale="zh-CN" />
        <h3>PopoverDescription</h3>
        <p>{`弹出层说明文本。`}</p>
        <FieldTable fields={popoverDescriptionFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
