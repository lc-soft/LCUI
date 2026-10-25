import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import CheckboxBasicDemo from "../../../examples/checkbox-basic/index";
import CheckboxDescriptionDemo from "../../../examples/checkbox-description/index";
import CheckboxGroupDemo from "../../../examples/checkbox-group/index";
import CheckboxIndeterminateDemo from "../../../examples/checkbox-indeterminate/index";
import CheckboxDisabledDemo from "../../../examples/checkbox-disabled/index";
import FieldTable from "../../../components/field-table";
import { checkboxFields } from "../../../widget-fields/checkbox";

export default function CheckboxDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Checkbox</h1>
      <p className="doc-page-desc">{`允许用户在已选和未选之间切换的控件。`}</p>
      <CheckboxBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：单条二元选择（同意条款、订阅开关）`}</Text></li>
          <li><Text>{`[b]适用[/b]：表单中的勾选项`}</Text></li>
          <li><Text>{`[b]不适用[/b]：互斥的多选一场景（用 [bgcolor=#eee] radio-group [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/checkbox/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Checkbox[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/checkbox/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]Checkbox[/color] />`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`使用 [bgcolor=#eee] checked [/bgcolor] 控制选中状态，状态变化时触发 [bgcolor=#eee] change [/bgcolor] 事件。`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>示例</h2>
        <h3>说明</h3>
        <p>{`配合 [bgcolor=#eee] FieldContent [/bgcolor] 与 [bgcolor=#eee] FieldDescription [/bgcolor] 为选项补充说明文字。`}</p>
        <CheckboxDescriptionDemo />
        <h3>分组</h3>
        <p>{`用一组 [bgcolor=#eee] Field [/bgcolor] 组合出复选框列表。`}</p>
        <CheckboxGroupDemo />
        <h3>半选</h3>
        <p>{`[bgcolor=#eee] indeterminate [/bgcolor] 用于表达“子项部分被选中”的视觉状态，此时显示减号图标。`}</p>
        <CheckboxIndeterminateDemo />
        <h3>禁用</h3>
        <CheckboxDisabledDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>注意事项</h2>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]indeterminate 是状态，不是默认值[/b]。半选态（[bgcolor=#eee] indeterminate="true" [/bgcolor]）用于表达“子项部分被选中”的视觉状态（例如父级 checkbox 在部分子项被选时显示半选）。点击 indeterminate 状态的 checkbox 会转为 checked 状态。`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <FieldTable fields={checkboxFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
