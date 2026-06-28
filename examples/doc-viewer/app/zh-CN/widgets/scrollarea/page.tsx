import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import ScrollareaBasicDemo from "../../../examples/scrollarea-basic/index";
import ScrollareaDualScrollbarsDemo from "../../../examples/scrollarea-dual-scrollbars/index";
import FieldTable from "../../../components/field-table";
import { scrollareaFields } from "../../../widget-fields/scrollarea";
import { scrollareaContentFields } from "../../../widget-fields/scrollarea-content";
import { scrollbarFields } from "../../../widget-fields/scrollbar";

export default function ScrollareaDocPage() {
  return (
    <Widget className="doc-page">
      <h1>ScrollArea</h1>
      <p className="doc-page-desc">{`当子内容超出容器范围时，提供滚动能力的容器部件。`}</p>
      <ScrollareaBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b] — 内容高度或宽度可能超过容器（长列表、文档、日志）。`}</Text></li>
          <li><Text>{`[b]适用[/b] — 需要滚动条可视化反馈的场景。`}</Text></li>
          <li><Text>{`[b]不适用[/b] — 内容总是小于容器（直接用普通 [bgcolor=#eee] widget [/bgcolor] 即可）。`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/scrollarea/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]ScrollArea[/color], [color=#8250df]ScrollAreaContent[/color], [color=#8250df]Scrollbar[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/scrollarea/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]ScrollArea[/color]>`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ScrollAreaContent[/color]>[/color]{/* 子内容 */}[color=#116329]</[color=#0550ae]ScrollAreaContent[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]Scrollbar[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color] />[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]Scrollbar[/color] />[/color]`}</Text>
            <Text className="code-line">{`</[color=#8250df]ScrollArea[/color]>`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>组合</h2>
        <p>{`使用以下组合来构建 [bgcolor=#eee] ScrollArea [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/scrollarea/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`ScrollArea`}</Text>
            <Text className="code-line">{`├── ScrollAreaContent`}</Text>
            <Text className="code-line">{`└── Scrollbar`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>示例</h2>
        <h3>双向滚动</h3>
        <ScrollareaDualScrollbarsDemo />
        <p>{`当内容需要在水平和垂直两个方向上滚动时，同时设置横向和纵向滚动条。`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>ScrollArea</h3>
        <p>{`滚动容器，负责裁剪超出范围的子内容并接收滚动事件。`}</p>
        <FieldTable fields={scrollareaFields} locale="zh-CN" />
        <h3>ScrollAreaContent</h3>
        <p>{`滚动内容容器，承载实际的子部件。必须作为 ScrollArea 的直接子部件使用。`}</p>
        <FieldTable fields={scrollareaContentFields} locale="zh-CN" />
        <h3>Scrollbar</h3>
        <p>{`滚动条部件，用于显示并操作滚动条。作为 ScrollArea 的直接子部件使用。`}</p>
        <FieldTable fields={scrollbarFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
