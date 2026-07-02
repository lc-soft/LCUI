import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import FieldBasicDemo from "../../../examples/field-basic/index";
import FieldCheckboxDemo from "../../../examples/field-checkbox/index";
import FieldChoiceCardDemo from "../../../examples/field-choice-card/index";
import FieldFieldsetDemo from "../../../examples/field-fieldset/index";
import FieldGroupDemo from "../../../examples/field-group/index";
import FieldInputDemo from "../../../examples/field-input/index";
import FieldRadioDemo from "../../../examples/field-radio/index";
import FieldTable from "../../../components/field-table";
import { fieldFields } from "../../../widget-fields/field";
import { fieldSetFields } from "../../../widget-fields/field-set";
import { fieldLegendFields } from "../../../widget-fields/field-legend";
import { fieldGroupFields } from "../../../widget-fields/field-group";
import { fieldLabelFields } from "../../../widget-fields/field-label";
import { fieldContentFields } from "../../../widget-fields/field-content";
import { fieldTitleFields } from "../../../widget-fields/field-title";
import { fieldDescriptionFields } from "../../../widget-fields/field-description";
import { fieldSeparatorFields } from "../../../widget-fields/field-separator";

export default function FieldDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Field</h1>
      <p className="doc-page-desc">{`组合 label、控件和说明文字，构成可访问的表单字段。`}</p>
      <FieldBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：表单字段的统一外壳（label + 控件 + 描述 + 错误信息）`}</Text></li>
          <li><Text>{`[b]适用[/b]：需要在横向布局里把 label 和控件并排`}</Text></li>
          <li><Text>{`[b]适用[/b]：把多个字段组成语义分组（FieldSet + FieldLegend）`}</Text></li>
          <li><Text>{`[b]不适用[/b]：单行 inline 文本（用 text widget）`}</Text></li>
          <li><Text>{`[b]不适用[/b]：要求原生 HTML [bgcolor=#eee] <form> [/bgcolor] 提交语义的场景（LCUI 没复刻）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/field/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Field[/color], [color=#8250df]FieldLabel[/color], [color=#8250df]FieldDescription[/color], [color=#8250df]FieldSet[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/field/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Field[/color]>`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"username"[/color]>[/color]用户名[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]TextInput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"username"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"evil_rabbit"[/color] />[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]选择一个唯一的用户名。[color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
            <Text className="code-line">{`</[color=#8250df]Field[/color]>`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>组合</h2>
        <p>{`使用以下组合来构建 Field：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/field/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`Field`}</Text>
            <Text className="code-line">{`├── FieldLabel`}</Text>
            <Text className="code-line">{`├── TextInput / Checkbox / RadioGroup`}</Text>
            <Text className="code-line">{`└── FieldDescription`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`FieldSet`}</Text>
            <Text className="code-line">{`├── FieldLegend`}</Text>
            <Text className="code-line">{`├── FieldDescription`}</Text>
            <Text className="code-line">{`└── FieldGroup`}</Text>
            <Text className="code-line">{`    ├── Field`}</Text>
            <Text className="code-line">{`    ├── FieldSeparator`}</Text>
            <Text className="code-line">{`    └── Field`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>示例</h2>
        <h3>Checkbox</h3>
        <FieldCheckboxDemo />
        <h3>Choice Card</h3>
        <FieldChoiceCardDemo />
        <h3>Fieldset</h3>
        <FieldFieldsetDemo />
        <h3>Field Group</h3>
        <FieldGroupDemo />
        <h3>Input</h3>
        <FieldInputDemo />
        <h3>Radio</h3>
        <FieldRadioDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>Field</h3>
        <p>{`字段容器，提供方向控制。`}</p>
        <FieldTable fields={fieldFields} locale="zh-CN" />
        <h3>FieldSet</h3>
        <p>{`语义分组容器。`}</p>
        <FieldTable fields={fieldSetFields} locale="zh-CN" />
        <h3>FieldLegend</h3>
        <p>{`分组标题。`}</p>
        <FieldTable fields={fieldLegendFields} locale="zh-CN" />
        <h3>FieldGroup</h3>
        <p>{`视觉堆叠容器。`}</p>
        <FieldTable fields={fieldGroupFields} locale="zh-CN" />
        <h3>FieldLabel</h3>
        <p>{`字段标签，点击转发到目标 input。`}</p>
        <FieldTable fields={fieldLabelFields} locale="zh-CN" />
        <h3>FieldContent</h3>
        <p>{`横向字段中的文本内容块。`}</p>
        <FieldTable fields={fieldContentFields} locale="zh-CN" />
        <h3>FieldTitle</h3>
        <p>{`FieldContent 内的标题文本。`}</p>
        <FieldTable fields={fieldTitleFields} locale="zh-CN" />
        <h3>FieldDescription</h3>
        <p>{`字段辅助说明。`}</p>
        <FieldTable fields={fieldDescriptionFields} locale="zh-CN" />
        <h3>FieldSeparator</h3>
        <p>{`字段组之间的分隔线。`}</p>
        <FieldTable fields={fieldSeparatorFields} locale="zh-CN" />
      </Widget>
      <Widget className="doc-section">
        <h2>注意事项</h2>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[b]FieldLabel 的 click forwarding 依赖 [bgcolor=#eee] for [/bgcolor] 属性[/b]。必须给 FieldLabel 设 [bgcolor=#eee] for="input-id" [/bgcolor] 并给目标控件设相同 [bgcolor=#eee] id [/bgcolor]，点击才会转发。`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
