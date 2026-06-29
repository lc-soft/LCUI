import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import RadioGroupBasicDemo from "../../../examples/radio-group-basic/index";
import RadioGroupDisabledDemo from "../../../examples/radio-group-disabled/index";
import FieldTable from "../../../components/field-table";
import { radioGroupFields } from "../../../widget-fields/radio-group";
import { radioGroupItemFields } from "../../../widget-fields/radio-group-item";

export default function RadioGroupDocPage() {
  return (
    <Widget className="doc-page">
      <h1>RadioGroup</h1>
      <p className="doc-page-desc">{`一组互斥的单选按钮，同一时刻最多只有一项被选中。`}</p>
      <RadioGroupBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：从一组互斥的选项中选出唯一一个（视图密度、排序方式、单选枚举等）`}</Text></li>
          <li><Text>{`[b]适用[/b]：选项数量固定为 2~7 个的简单场景`}</Text></li>
          <li><Text>{`[b]不适用[/b]：需要并选多个选项的场景（用 [bgcolor=#eee] checkbox [/bgcolor]）`}</Text></li>
          <li><Text>{`[b]不适用[/b]：选项数量大到需要搜索或筛选（暂未提供 [bgcolor=#eee] select [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/radio-group/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]RadioGroup[/color], [color=#8250df]RadioGroupItem[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/radio-group/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]RadioGroup[/color] value=[color=#0a3069]"comfortable"[/color]>`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"default"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"r1"[/color] />[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"comfortable"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"r2"[/color] />[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]RadioGroupItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"compact"[/color] [color=#0550ae]id[/color]=[color=#0a3069]"r3"[/color] />[/color]`}</Text>
            <Text className="code-line">{`</[color=#8250df]RadioGroup[/color]>`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>组合</h2>
        <p>{`使用以下组合来构建 [bgcolor=#eee] RadioGroup [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/radio-group/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`RadioGroup`}</Text>
            <Text className="code-line">{`├── RadioGroupItem`}</Text>
            <Text className="code-line">{`└── RadioGroupItem`}</Text>
          </Widget>
        </Widget>
        <p>{`通常每项会与一个 [bgcolor=#eee] Label [/bgcolor] 搭配放置在同一行 flex 容器里，Label 的 [bgcolor=#eee] for [/bgcolor] 指向 RadioGroupItem 的 [bgcolor=#eee] id [/bgcolor]，点击 Label 会把 click 事件转发到对应的 item。`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>示例</h2>
        <h3>禁用</h3>
        <RadioGroupDisabledDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>RadioGroup</h3>
        <p>{`单选组容器，管理组内各项的互斥选中状态。`}</p>
        <FieldTable fields={radioGroupFields} locale="zh-CN" />
        <h3>RadioGroupItem</h3>
        <p>{`单选项，每一项代表一个可选值。同一 RadioGroup 内同一时刻最多只有一项处于 checked 状态。`}</p>
        <FieldTable fields={radioGroupItemFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
