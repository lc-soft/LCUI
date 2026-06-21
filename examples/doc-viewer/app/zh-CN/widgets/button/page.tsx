import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import ButtonBasicDemo from "../../../examples/button-basic/index";
import FieldTable from "../../../components/field-table";
import { buttonFields } from "../../../widget-fields/button";

export default function ButtonDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Button</h1>
      <p className="doc-page-desc">{`可点击的按钮部件，通常用于触发某个操作。`}</p>
      <ButtonBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：用户点击执行动作（提交表单、打开页面、切换状态）`}</Text></li>
          <li><Text>{`[b]适用[/b]：工具栏、对话框、导航栏中的交互入口`}</Text></li>
          <li><Text>{`[b]不适用[/b]：不需要交互的纯文本展示（用 [bgcolor=#eee] text [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>结构</h2>
        <p>{`导入部件：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/button/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Button[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]Button[/color] />[/color]`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <FieldTable fields={buttonFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
