import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import CollapsibleDemoDemo from "../../../../examples/collapsible-demo/index";
import CollapsibleBasicDemo from "../../../../examples/collapsible-basic/index";
import CollapsibleSettingsDemo from "../../../../examples/collapsible-settings/index";
import CollapsibleFileTreeDemo from "../../../../examples/collapsible-file-tree/index";
import FieldTable from "../../../../components/field-table";
import { collapsibleFields } from "../../../../widget-fields/collapsible";
import { collapsibleTriggerFields } from "../../../../widget-fields/collapsible-trigger";
import { collapsibleContentFields } from "../../../../widget-fields/collapsible-content";

export default function CollapsibleDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Collapsible</h1>
      <p className="doc-page-desc">{`用于展开和收起一段内容的交互组件。`}</p>
      <CollapsibleDemoDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：产品详情、设置面板、文件树等需要按需显示的内容`}</Text></li>
          <li><Text>{`[b]适用[/b]：内容较多但默认只需展示摘要的场景`}</Text></li>
          <li><Text>{`[b]不适用[/b]：需要模态交互或阻断页面操作的场景（使用 [bgcolor=#7f7f7f29] dialog [/bgcolor]）`}</Text></li>
          <li><Text>{`[b]不适用[/b]：仅用于切换少量标签或选项（使用 [bgcolor=#7f7f7f29] select [/bgcolor] 或 [bgcolor=#7f7f7f29] radio-group [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/collapsible/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Collapsible[/color], [color=#8250df]CollapsibleContent[/color], [color=#8250df]CollapsibleTrigger[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] { [color=#d2a8ff]Collapsible[/color], [color=#d2a8ff]CollapsibleContent[/color], [color=#d2a8ff]CollapsibleTrigger[/color] } [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/collapsible/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "144px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#8250df]Collapsible[/color] defaultOpen>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]CollapsibleTrigger[/color]>[/color]Product details[color=#116329]</[color=#0550ae]CollapsibleTrigger[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]CollapsibleContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`    This panel can be expanded or collapsed.`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#116329]</[color=#0550ae]CollapsibleContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`</[color=#8250df]Collapsible[/color]>`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]Collapsible[/color] defaultOpen>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]CollapsibleTrigger[/color]>[/color]Product details[color=#7ee787]</[color=#79c0ff]CollapsibleTrigger[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]CollapsibleContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`    This panel can be expanded or collapsed.`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]CollapsibleContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">6</Text>
                    <Text className="code-line">{`</[color=#d2a8ff]Collapsible[/color]>`}</Text>
                  </Widget>
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
        <p>{`使用以下组合来构建 [bgcolor=#7f7f7f29] Collapsible [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/collapsible/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "84px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Text className="code-line">{`Collapsible`}</Text>
                  <Text className="code-line">{`├── CollapsibleTrigger`}</Text>
                  <Text className="code-line">{`└── CollapsibleContent`}</Text>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Text className="code-line">{`Collapsible`}</Text>
                  <Text className="code-line">{`├── CollapsibleTrigger`}</Text>
                  <Text className="code-line">{`└── CollapsibleContent`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>示例</h2>
        <h3>基础</h3>
        <p>{`使用带边框的摘要行和详情面板展示订单信息。`}</p>
        <CollapsibleBasicDemo />
        <h3>设置面板</h3>
        <p>{`使用触发器展开额外的输入项；[bgcolor=#7f7f7f29] open [/bgcolor] 可用于受控状态。`}</p>
        <CollapsibleSettingsDemo />
        <h3>文件树</h3>
        <p>{`将 Collapsible 嵌套在文件夹节点中，可以实现独立的树形展开。`}</p>
        <CollapsibleFileTreeDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>Collapsible</h3>
        <p>{`折叠面板根节点，持有 open/closed 状态。`}</p>
        <FieldTable fields={collapsibleFields} locale="zh-CN" />
        <h3>CollapsibleTrigger</h3>
        <p>{`触发面板切换的节点；也可以给任意后代设置 [bgcolor=#7f7f7f29] data-collapsible-trigger [/bgcolor]。`}</p>
        <FieldTable fields={collapsibleTriggerFields} locale="zh-CN" />
        <h3>CollapsibleContent</h3>
        <p>{`面板内容区域，closed 时不参与显示。`}</p>
        <FieldTable fields={collapsibleContentFields} locale="zh-CN" />
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[bgcolor=#7f7f7f29] data-collapsible-trigger [/bgcolor] 只用于事件绑定，不参与 CSS 选择器。需要样式时请使用 class 或 [bgcolor=#7f7f7f29] collapsible:open [/bgcolor] / [bgcolor=#7f7f7f29] collapsible:closed [/bgcolor] 状态。`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
