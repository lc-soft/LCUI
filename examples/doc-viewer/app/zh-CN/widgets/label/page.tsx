import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import LabelBasicDemo from "../../../examples/label-basic/index";
import FieldTable from "../../../components/field-table";
import { labelFields } from "../../../widget-fields/label";

export default function LabelDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Label</h1>
      <p className="doc-page-desc">{`渲染与控件关联的可访问标签，点击时将 click 事件转发到 [bgcolor=#eee] for [/bgcolor] 指向的目标部件。`}</p>
      <LabelBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：为 checkbox、text-input 等可交互控件提供点击区域扩展（点击文字也能触发控件）`}</Text></li>
          <li><Text>{`[b]适用[/b]：辅助功能（a11y）中将文本与控件关联`}</Text></li>
          <li><Text>{`[b]不适用[/b]：纯展示文本（用 [bgcolor=#eee] text [/bgcolor] 部件）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/label/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Label[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/label/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Label[/color] [color=#cf222e]for[/color]=[color=#0a3069]"agree"[/color]>I agree</[color=#8250df]Label[/color]>`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <FieldTable fields={labelFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
