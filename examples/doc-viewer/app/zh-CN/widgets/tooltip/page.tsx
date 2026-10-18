import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import TooltipBasicDemo from "../../../examples/tooltip-basic/index";
import TooltipSidesDemo from "../../../examples/tooltip-sides/index";
import TooltipDisabledDemo from "../../../examples/tooltip-disabled/index";
import FieldTable from "../../../components/field-table";
import { tooltipFields } from "../../../widget-fields/tooltip";
import { tooltipTriggerFields } from "../../../widget-fields/tooltip-trigger";
import { tooltipContentFields } from "../../../widget-fields/tooltip-content";

export default function TooltipDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Tooltip</h1>
      <p className="doc-page-desc">{`悬停或聚焦时显示简短说明的浮层。`}</p>
      <TooltipBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：图标按钮、截断文本、工具栏和表单标签等需要补充说明的场景`}</Text></li>
          <li><Text>{`[b]适用[/b]：短暂、非交互的提示信息`}</Text></li>
          <li><Text>{`[b]不适用[/b]：需要用户确认或操作的内容（使用 [bgcolor=#eee] dialog [/bgcolor] / [bgcolor=#eee] alert-dialog [/bgcolor]）`}</Text></li>
          <li><Text>{`[b]不适用[/b]：长篇说明或富文本内容（使用正文或 [bgcolor=#eee] field-description [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/tooltip/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Tooltip[/color], [color=#8250df]TooltipTrigger[/color], [color=#8250df]TooltipContent[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/tooltip/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Tooltip[/color]>`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]TooltipTrigger[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"outline"[/color]>[/color]Hover[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]</[color=#0550ae]TooltipTrigger[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]TooltipContent[/color]>[/color]Add to library[color=#116329]</[color=#0550ae]TooltipContent[/color]>[/color]`}</Text>
            <Text className="code-line">{`</[color=#8250df]Tooltip[/color]>`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>纯属性用法</h2>
        <p>{`任意部件都可以只加 [bgcolor=#eee] data-tooltip [/bgcolor] 属性，无需 [bgcolor=#eee] Tooltip [/bgcolor] 结构：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/tooltip/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Button[/color] data-tooltip=[color=#0a3069]"Add to library"[/color]>[color=#8250df]Hover[/color]</[color=#8250df]Button[/color]>`}</Text>
          </Widget>
        </Widget>
        <p>{`可用 [bgcolor=#eee] data-tooltip-* [/bgcolor] 覆盖默认参数：`}</p>
        <Widget className="mdx-table">
          <Widget className="mdx-table-row mdx-table-head">
            <Widget className="mdx-table-cell mdx-table-head-cell" style={{ width: "240px" }}>
              <Text>{`属性`}</Text>
            </Widget>
            <Widget className="mdx-table-cell mdx-table-head-cell" style={{ width: "240px" }}>
              <Text>{`对应参数`}</Text>
            </Widget>
            <Widget className="mdx-table-cell mdx-table-head-cell" style={{ width: "88px" }}>
              <Text>{`默认值`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`[bgcolor=#eee] data-tooltip [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`提示文本（必填）`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "88px" }}>
              <Text>{`-`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`[bgcolor=#eee] data-tooltip-side [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`top / right / bottom / left`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "88px" }}>
              <Text>{`[bgcolor=#eee] top [/bgcolor]`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`[bgcolor=#eee] data-tooltip-align [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`start / center / end`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "88px" }}>
              <Text>{`[bgcolor=#eee] center [/bgcolor]`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`[bgcolor=#eee] data-tooltip-side-offset [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`侧边偏移（px）`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "88px" }}>
              <Text>{`[bgcolor=#eee] 4 [/bgcolor]`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`[bgcolor=#eee] data-tooltip-align-offset [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`对齐偏移（px）`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "88px" }}>
              <Text>{`[bgcolor=#eee] 0 [/bgcolor]`}</Text>
            </Widget>
          </Widget>
          <Widget className="mdx-table-row">
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`[bgcolor=#eee] data-tooltip-delay [/bgcolor]`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "240px" }}>
              <Text>{`显示延迟（毫秒）`}</Text>
            </Widget>
            <Widget className="mdx-table-cell" style={{ width: "88px" }}>
              <Text>{`[bgcolor=#eee] 0 [/bgcolor]`}</Text>
            </Widget>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/tooltip/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Button[/color]`}</Text>
            <Text className="code-line">{`  data-tooltip=[color=#0a3069]"Add to library"[/color]`}</Text>
            <Text className="code-line">{`  data-tooltip-side=[color=#0a3069]"left"[/color]`}</Text>
            <Text className="code-line">{`  data-tooltip-side-offset=[color=#0a3069]"8"[/color]`}</Text>
            <Text className="code-line">{`  data-tooltip-delay=[color=#0a3069]"200"[/color]`}</Text>
            <Text className="code-line">{`>`}</Text>
            <Text className="code-line">{`  [color=#8250df]Hover[/color]`}</Text>
            <Text className="code-line">{`</[color=#8250df]Button[/color]>`}</Text>
          </Widget>
        </Widget>
        <ul>
          <li><Text>{`文本、方位、偏移和延迟在每次显示时从属性重新读取。`}</Text></li>
          <li><Text>{`[bgcolor=#eee] data-tooltip [/bgcolor] 为空字符串时不显示。`}</Text></li>
          <li><Text>{`显式 [bgcolor=#eee] Tooltip [/bgcolor] 结构内部的部件不会重复触发属性模式。`}</Text></li>
          <li><Text>{`需要把 trigger 和 content 放在不同子树时，可用 [bgcolor=#eee] data-tooltip-trigger="key" [/bgcolor] 与 [bgcolor=#eee] data-tooltip-target="key" [/bgcolor] 关联。`}</Text></li>
        </ul>
        <h3>C 便捷调用</h3>
        <p>{`C 端无需手写属性：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/widgets/tooltip/4">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`ui_tooltip_attach(button, [color=#0a3069]"Add to library"[/color]);`}</Text>
          </Widget>
        </Widget>
        <p>{`需要调整方位、偏移或延迟时：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/widgets/tooltip/5">
          <Widget className="code-block-header">
            <Text className="code-block-title">C</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]ui_tooltip_options_t[/color] options;`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`ui_tooltip_options_init(&options);`}</Text>
            <Text className="code-line">{`options.content = [color=#0a3069]"Add to library"[/color];`}</Text>
            <Text className="code-line">{`options.side = UI_TOOLTIP_SIDE_RIGHT;`}</Text>
            <Text className="code-line">{`options.side_offset = [color=#0550ae]8[/color];`}</Text>
            <Text className="code-line">{`options.delay = [color=#0550ae]200[/color];`}</Text>
            <Text className="code-line">{`ui_tooltip_attach_ex(button, &options);`}</Text>
          </Widget>
        </Widget>
        <p>{`[bgcolor=#eee] ui_tooltip_options_init() [/bgcolor] 会填入默认值（top、center、side offset 4、align offset 0、 delay 0），因此未修改的字段保持默认行为。`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>组合</h2>
        <p>{`使用以下组合来构建 [bgcolor=#eee] Tooltip [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/tooltip/6">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`Tooltip`}</Text>
            <Text className="code-line">{`├── TooltipTrigger`}</Text>
            <Text className="code-line">{`└── TooltipContent（Portal）`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>示例</h2>
        <h3>方向</h3>
        <p>{`使用 [bgcolor=#eee] side [/bgcolor] 属性控制提示出现的方位。`}</p>
        <TooltipSidesDemo />
        <h3>禁用控件</h3>
        <p>{`禁用的控件本身不接收鼠标事件，用 [bgcolor=#eee] TooltipTrigger [/bgcolor] 包住它即可正常显示提示。`}</p>
        <TooltipDisabledDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>Tooltip</h3>
        <p>{`提示的根节点，管理显示延迟和 Portal。`}</p>
        <FieldTable fields={tooltipFields} locale="zh-CN" />
        <h3>TooltipTrigger</h3>
        <p>{`触发提示的包装层，按钮、图标或其他可 hover 的部件放在其中。`}</p>
        <FieldTable fields={tooltipTriggerFields} locale="zh-CN" />
        <h3>TooltipContent</h3>
        <p>{`提示气泡，负责方位、对齐和内容。`}</p>
        <FieldTable fields={tooltipContentFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
