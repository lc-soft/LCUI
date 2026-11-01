import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import AccordionDemoDemo from "../../../examples/accordion-demo/index";
import AccordionBasicDemoDemo from "../../../examples/accordion-basic-demo/index";
import AccordionMultipleDemo from "../../../examples/accordion-multiple/index";
import AccordionDisabledDemo from "../../../examples/accordion-disabled/index";
import AccordionBordersDemo from "../../../examples/accordion-borders/index";
import AccordionCardDemo from "../../../examples/accordion-card/index";
import AccordionCustomTriggerDemo from "../../../examples/accordion-custom-trigger/index";
import FieldTable from "../../../components/field-table";
import { accordionFields } from "../../../widget-fields/accordion";
import { accordionItemFields } from "../../../widget-fields/accordion-item";
import { accordionTriggerFields } from "../../../widget-fields/accordion-trigger";
import { accordionContentFields } from "../../../widget-fields/accordion-content";

export default function AccordionDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Accordion</h1>
      <p className="doc-page-desc">{`垂直堆叠的多组标题与内容，用于一次展开一个或多个分区。`}</p>
      <AccordionDemoDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：FAQ、设置分组、账单说明等需要按标题展开内容的页面`}</Text></li>
          <li><Text>{`[b]适用[/b]：内容较长但同一时间只需阅读其中一部分`}</Text></li>
          <li><Text>{`[b]不适用[/b]：需要并排切换的选项卡（使用 [bgcolor=#eee] select [/bgcolor] 或后续 tabs 部件）`}</Text></li>
          <li><Text>{`[b]不适用[/b]：需要模态确认或阻断操作的场景（使用 [bgcolor=#eee] dialog [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/accordion/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "144px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#8250df]Accordion[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`  [color=#8250df]AccordionContent[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`  [color=#8250df]AccordionItem[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`  [color=#8250df]AccordionTrigger[/color],`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/accordion/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "184px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]Accordion[/color] defaultValue=[color=#0a3069]"shipping"[/color]>`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">2</Text>
                  <Text className="code-line">{`  [color=#116329]<[color=#0550ae]AccordionItem[/color] [color=#0550ae]value[/color]=[color=#0a3069]"shipping"[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">3</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]AccordionTrigger[/color]>[/color]What are your shipping options?[color=#116329]</[color=#0550ae]AccordionTrigger[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">4</Text>
                  <Text className="code-line">{`    [color=#116329]<[color=#0550ae]AccordionContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">5</Text>
                  <Text className="code-line">{`      We offer standard, express, and overnight shipping.`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">6</Text>
                  <Text className="code-line">{`    [color=#116329]</[color=#0550ae]AccordionContent[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">7</Text>
                  <Text className="code-line">{`  [color=#116329]</[color=#0550ae]AccordionItem[/color]>[/color]`}</Text>
                </Widget>
                <Widget className="code-line-row">
                  <Text className="code-line-number">8</Text>
                  <Text className="code-line">{`</[color=#8250df]Accordion[/color]>`}</Text>
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
        <p>{`使用以下组合来构建 [bgcolor=#eee] Accordion [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/accordion/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "164px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`Accordion`}</Text>
                <Text className="code-line">{`├── AccordionItem`}</Text>
                <Text className="code-line">{`│   ├── AccordionTrigger`}</Text>
                <Text className="code-line">{`│   └── AccordionContent`}</Text>
                <Text className="code-line">{`└── AccordionItem`}</Text>
                <Text className="code-line">{`    ├── AccordionTrigger`}</Text>
                <Text className="code-line">{`    └── AccordionContent`}</Text>
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
        <p>{`单开模式，默认展开第一项；再次点击已展开项会收起。`}</p>
        <AccordionBasicDemoDemo />
        <h3>多开</h3>
        <p>{`设置 [bgcolor=#eee] multiple [/bgcolor] 后，各项独立展开，可同时打开多个分区。`}</p>
        <AccordionMultipleDemo />
        <h3>禁用</h3>
        <p>{`在 [bgcolor=#eee] AccordionItem [/bgcolor] 上设置 [bgcolor=#eee] disabled [/bgcolor]，该项的点击和 Enter/Space 都不会切换。`}</p>
        <AccordionDisabledDemo />
        <h3>边框</h3>
        <p>{`主题已为 item 提供底部分隔线；示例额外给根节点加外边框和圆角，并给 item 加水平内边距。`}</p>
        <AccordionBordersDemo />
        <h3>卡片</h3>
        <p>{`将 Accordion 放进 Card，用于账单、订阅等设置区块。`}</p>
        <AccordionCardDemo />
        <h3>自定义触发器</h3>
        <p>{`需要完全自定义标题栏时，可以不用 [bgcolor=#eee] AccordionTrigger [/bgcolor]，在任意 item 后代上声明 [bgcolor=#eee] data-accordion-trigger="true" [/bgcolor]。此时不会自动创建箭头，焦点与展开/收起视觉由调用方负责。`}</p>
        <AccordionCustomTriggerDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>Accordion</h3>
        <p>{`手风琴根节点，管理展开集合、单开/多开和 [bgcolor=#eee] change [/bgcolor] 事件。`}</p>
        <FieldTable fields={accordionFields} locale="zh-CN" />
        <h3>AccordionItem</h3>
        <p>{`单个分区，持有 [bgcolor=#eee] open [/bgcolor] 状态和节点 [bgcolor=#eee] value [/bgcolor]。`}</p>
        <FieldTable fields={accordionItemFields} locale="zh-CN" />
        <h3>AccordionTrigger</h3>
        <p>{`标题行，默认带有右侧箭头；[bgcolor=#eee] chevron="false" [/bgcolor] 可关闭。`}</p>
        <FieldTable fields={accordionTriggerFields} locale="zh-CN" />
        <h3>AccordionContent</h3>
        <p>{`分区内容，item 为 closed 时不参与显示。`}</p>
        <FieldTable fields={accordionContentFields} locale="zh-CN" />
      </Widget>
      <Widget className="doc-section">
        <h2>注意事项</h2>
        <Widget className="admonition admonition-note">
          <Text className="admonition-title">提示</Text>
          <Widget className="admonition-content">
            <p>{`[b]没有展开/收起动画[/b]。LCUI 的 CSS 引擎不支持 transition/animation，状态切换是瞬时的。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-info">
          <Text className="admonition-title">信息</Text>
          <Widget className="admonition-content">
            <p>{`[b]初始化不触发事件[/b]。[bgcolor=#eee] defaultValue [/bgcolor] / [bgcolor=#eee] value [/bgcolor] 以及解析阶段的 item [bgcolor=#eee] open [/bgcolor] 只同步状态；用户点击、Enter/Space 或 [bgcolor=#eee] ui_accordion_item_set_open() [/bgcolor] 才会发出 [bgcolor=#eee] change [/bgcolor]。[bgcolor=#eee] ui_accordion_set_value() [/bgcolor] 是静默的受控同步。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-info">
          <Text className="admonition-title">信息</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#eee] change [/bgcolor] 载荷是本次切换项的 value[/b]。root 的 [bgcolor=#eee] value [/bgcolor] 属性保存完整集合（逗号分隔），监听器中可读取 [bgcolor=#eee] ui_accordion_get_value(root) [/bgcolor]；一次设置多个值时不会逐个发事件。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-tip">
          <Text className="admonition-title">技巧</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#eee] data-accordion-trigger [/bgcolor] 是 LCUI 扩展[/b]。它只作为行为标记，不参与 CSS 选择器；自定义触发器需要自行设置焦点，并用 [bgcolor=#eee] accordion-item:open [/bgcolor] / [bgcolor=#eee] accordion-item:closed [/bgcolor] 写展开状态样式。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-caution">
          <Text className="admonition-title">警告</Text>
          <Widget className="admonition-content">
            <p>{`[b]键盘只支持 Enter/Space[/b]。trigger 可通过 Tab 聚焦，未实现方向键、Home/End 的 roving tabindex。单开模式自动关闭其它项时，被关闭项不会额外发出 [bgcolor=#eee] open [/bgcolor]/[bgcolor=#eee] close [/bgcolor]。`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
