import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import ProgressBasicDemo from "../../../examples/progress-basic/index";
import FieldTable from "../../../components/field-table";
import { progressFields } from "../../../widget-fields/progress";

export default function ProgressDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Progress</h1>
      <p className="doc-page-desc">{`用于展示任务进度（如文件上传、加载状态）的线性进度条部件。`}</p>
      <ProgressBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：文件上传/下载、安装、加载等长时间任务的进度展示`}</Text></li>
          <li><Text>{`[b]适用[/b]：表单提交、异步操作的等待反馈`}</Text></li>
          <li><Text>{`[b]不适用[/b]：需要精确数值或不确定进度的场景（用 [bgcolor=#eee] text [/bgcolor] 自行绘制）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/progress/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Progress[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/progress/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]Progress[/color] value={[color=#0550ae]20[/color]} />`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>组合</h2>
        <p>{`使用以下组合来构建 [bgcolor=#eee] Progress [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/progress/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`Progress`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <FieldTable fields={progressFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
