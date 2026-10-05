import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import EmptyBasicDemo from "../../../examples/empty-basic/index";
import EmptyBackgroundDemo from "../../../examples/empty-background/index";
import FieldTable from "../../../components/field-table";
import { emptyFields } from "../../../widget-fields/empty";
import { emptyMediaFields } from "../../../widget-fields/empty-media";
import { emptyTitleFields } from "../../../widget-fields/empty-title";
import { emptyDescriptionFields } from "../../../widget-fields/empty-description";
import { emptyActionFields } from "../../../widget-fields/empty-action";

export default function EmptyDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Empty</h1>
      <p className="doc-page-desc">{`为无数据、无搜索结果等状态提供居中且可组合的占位内容。`}</p>
      <EmptyBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：列表、搜索结果或内容区域为空时`}</Text></li>
          <li><Text>{`[b]适用[/b]：需要引导用户创建第一项内容`}</Text></li>
          <li><Text>{`[b]不适用[/b]：数据仍在加载中的状态（用 loading 指示器）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/empty/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
            <Text className="code-line">{`  [color=#8250df]Button[/color],`}</Text>
            <Text className="code-line">{`  [color=#8250df]Empty[/color],`}</Text>
            <Text className="code-line">{`  [color=#8250df]EmptyAction[/color],`}</Text>
            <Text className="code-line">{`  [color=#8250df]EmptyDescription[/color],`}</Text>
            <Text className="code-line">{`  [color=#8250df]EmptyMedia[/color],`}</Text>
            <Text className="code-line">{`  [color=#8250df]EmptyTitle[/color],`}</Text>
            <Text className="code-line">{`  [color=#8250df]Text[/color],`}</Text>
            <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Folder[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/fluent-icons"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/empty/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Empty[/color]>`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]EmptyMedia[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Folder[/color] />[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]</[color=#0550ae]EmptyMedia[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]EmptyTitle[/color]>[/color]No data[color=#116329]</[color=#0550ae]EmptyTitle[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]EmptyDescription[/color]>[/color]No data found[color=#116329]</[color=#0550ae]EmptyDescription[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]<[color=#0550ae]EmptyAction[/color]>[/color]`}</Text>
            <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Button[/color]>[/color][color=#116329]<[color=#0550ae]Text[/color]>[/color]Add data[color=#116329]</[color=#0550ae]Text[/color]>[/color][color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
            <Text className="code-line">{`  [color=#116329]</[color=#0550ae]EmptyAction[/color]>[/color]`}</Text>
            <Text className="code-line">{`</[color=#8250df]Empty[/color]>`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>组合</h2>
        <p>{`使用以下组合来构建 [bgcolor=#eee] Empty [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/empty/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`Empty`}</Text>
            <Text className="code-line">{`├── EmptyMedia`}</Text>
            <Text className="code-line">{`├── EmptyTitle`}</Text>
            <Text className="code-line">{`├── EmptyDescription`}</Text>
            <Text className="code-line">{`└── EmptyAction`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>背景</h2>
        <p>{`添加浅色背景可以弱化空状态与周围内容的边界。`}</p>
        <EmptyBackgroundDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>Empty</h3>
        <p>{`空状态容器。`}</p>
        <FieldTable fields={emptyFields} locale="zh-CN" />
        <h3>EmptyMedia</h3>
        <p>{`图标、插画或其他媒体槽位。`}</p>
        <FieldTable fields={emptyMediaFields} locale="zh-CN" />
        <h3>EmptyTitle</h3>
        <p>{`空状态标题。`}</p>
        <FieldTable fields={emptyTitleFields} locale="zh-CN" />
        <h3>EmptyDescription</h3>
        <p>{`空状态说明。`}</p>
        <FieldTable fields={emptyDescriptionFields} locale="zh-CN" />
        <h3>EmptyAction</h3>
        <p>{`后续操作的布局槽位。`}</p>
        <FieldTable fields={emptyActionFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
