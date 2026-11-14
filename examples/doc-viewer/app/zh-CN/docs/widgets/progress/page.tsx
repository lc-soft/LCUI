import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import ProgressBasicDemo from "../../../../examples/progress-basic/index";
import ProgressLabelDemo from "../../../../examples/progress-label/index";
import FieldTable from "../../../../components/field-table";
import { progressFields } from "../../../../widget-fields/progress";

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
          <li><Text>{`[b]不适用[/b]：需要精确数值或不确定进度的场景（用 [bgcolor=#7f7f7f29] text [/bgcolor] 自行绘制）`}</Text></li>
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
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Progress[/color], [color=#8250df]ProgressLabel[/color], [color=#8250df]ProgressValue[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] { [color=#d2a8ff]Progress[/color], [color=#d2a8ff]ProgressLabel[/color], [color=#d2a8ff]ProgressValue[/color] } [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/progress/1">
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
                    <Text className="code-line">{`<[color=#8250df]Progress[/color] value={[color=#0550ae]20[/color]} />`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]Progress[/color] value={[color=#79c0ff]20[/color]} />`}</Text>
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
        <h2>示例</h2>
        <h3>带标签</h3>
        <p>{`[bgcolor=#7f7f7f29] Progress [/bgcolor] 可接受 [bgcolor=#7f7f7f29] ProgressLabel [/bgcolor] 与 [bgcolor=#7f7f7f29] ProgressValue [/bgcolor] 子部件，标签行会显示在进度条上方。`}</p>
        <ProgressLabelDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <FieldTable fields={progressFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
