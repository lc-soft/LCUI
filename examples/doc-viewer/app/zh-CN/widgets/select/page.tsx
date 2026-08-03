import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import SelectBasicDemo from "../../../examples/select-basic/index";
import FieldTable from "../../../components/field-table";
import { selectFields } from "../../../widget-fields/select";
import { selectTriggerFields } from "../../../widget-fields/select-trigger";
import { selectContentFields } from "../../../widget-fields/select-content";
import { selectItemFields } from "../../../widget-fields/select-item";

export default function SelectDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Select</h1>
      <p className="doc-page-desc">{`Select 提供一个可展开的下拉选项列表，用于从多个候选值中选择一项。`}</p>
      <SelectBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：从多个选项中选择唯一值`}</Text></li>
          <li><Text>{`[b]适用[/b]：选项较多、需要节省页面空间的表单`}</Text></li>
          <li><Text>{`[b]不适用[/b]：需要同时选择多个值的场景（用 [bgcolor=#eee] checkbox [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/select/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Select[/color], [color=#8250df]SelectTrigger[/color], [color=#8250df]SelectValue[/color], [color=#8250df]SelectContent[/color], [color=#8250df]SelectItem[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/select/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Select[/color]>`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]SelectTrigger[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]SelectValue[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"选择主题"[/color] />[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]</[color=#0550ae]SelectTrigger[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"light"[/color]>[/color]浅色[color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"dark"[/color]>[/color]深色[color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"system"[/color]>[/color]跟随系统[color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]</[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
            <Text className="code-line">{`</[color=#8250df]Select[/color]>`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>组合</h2>
        <p>{`使用以下组合来构建 [bgcolor=#eee] Select [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/select/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`Select`}</Text>
            <Text className="code-line">{`├── SelectTrigger`}</Text>
            <Text className="code-line">{`│   └── SelectValue`}</Text>
            <Text className="code-line">{`└── SelectContent`}</Text>
            <Text className="code-line">{`    └── SelectItem`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>Select</h3>
        <p>{`Select 状态容器，管理当前选中值、placeholder 和下拉面板。`}</p>
        <FieldTable fields={selectFields} locale="zh-CN" />
        <h3>SelectTrigger</h3>
        <p>{`触发下拉面板打开或关闭的按钮。`}</p>
        <FieldTable fields={selectTriggerFields} locale="zh-CN" />
        <h3>SelectContent</h3>
        <p>{`通过 Portal 挂载到 root 层级的选项面板。`}</p>
        <FieldTable fields={selectContentFields} locale="zh-CN" />
        <h3>SelectItem</h3>
        <p>{`代表一个可选值的选项。`}</p>
        <FieldTable fields={selectItemFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
