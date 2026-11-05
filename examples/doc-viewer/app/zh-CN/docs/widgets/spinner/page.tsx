import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import SpinnerBasicDemo from "../../../../examples/spinner-basic/index";
import SpinnerCustomContentDemo from "../../../../examples/spinner-custom-content/index";
import SpinnerSizeDemo from "../../../../examples/spinner-size/index";
import SpinnerRunningDemo from "../../../../examples/spinner-running/index";
import FieldTable from "../../../../components/field-table";
import { spinnerFields } from "../../../../widget-fields/spinner";
import { spinnerContentFields } from "../../../../widget-fields/spinner-content";

export default function SpinnerDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Spinner</h1>
      <p className="doc-page-desc">{`用于表达“加载中”的自绘旋转指示器部件。`}</p>
      <SpinnerBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：等待异步结果、无法预知进度的加载状态`}</Text></li>
          <li><Text>{`[b]适用[/b]：按钮、卡片等局部区域的忙碌指示`}</Text></li>
          <li><Text>{`[b]不适用[/b]：有明确百分比或已知时长的任务进度（用 [bgcolor=#eee] progress [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/spinner/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Spinner[/color], [color=#8250df]SpinnerContent[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/spinner/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`<[color=#8250df]Spinner[/color] running={[color=#0550ae]true[/color]} speed={[color=#0550ae]1[/color]} />`}</Text>
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
        <p>{`使用以下组合来构建 [bgcolor=#eee] Spinner [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/spinner/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "64px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`Spinner`}</Text>
                <Text className="code-line">{`└── SpinnerContent`}</Text>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>示例</h2>
        <h3>自定义内容</h3>
        <SpinnerCustomContentDemo />
        <h3>不同尺寸</h3>
        <SpinnerSizeDemo />
        <h3>运行状态控制</h3>
        <SpinnerRunningDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>Spinner</h3>
        <p>{`旋转指示器容器，捕获 [bgcolor=#eee] SpinnerContent [/bgcolor] 子树并绕其中心旋转绘制。默认自带 16×16 尺寸，可用 [bgcolor=#eee] width [/bgcolor]/[bgcolor=#eee] height [/bgcolor] 覆盖。`}</p>
        <FieldTable fields={spinnerFields} locale="zh-CN" />
        <h3>SpinnerContent</h3>
        <p>{`被旋转捕获的内容子树，作为 Spinner 的直接子部件使用；省略时使用默认的圆环，其中一边透明形成随旋转扫过的缺口。`}</p>
        <FieldTable fields={spinnerContentFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
