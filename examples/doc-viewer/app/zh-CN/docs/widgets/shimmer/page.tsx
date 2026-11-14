import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";
import ShimmerBasicDemo from "../../../../examples/shimmer-basic/index";
import ShimmerColorDemo from "../../../../examples/shimmer-color/index";
import ShimmerDurationDemo from "../../../../examples/shimmer-duration/index";
import ShimmerSpreadDemo from "../../../../examples/shimmer-spread/index";
import ShimmerAngleDemo from "../../../../examples/shimmer-angle/index";
import ShimmerRunningDemo from "../../../../examples/shimmer-running/index";
import FieldTable from "../../../../components/field-table";
import { shimmerFields } from "../../../../widget-fields/shimmer";
import { shimmerContentFields } from "../../../../widget-fields/shimmer-content";

export default function ShimmerDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Shimmer</h1>
      <p className="doc-page-desc">{`给文本、图标等内容叠加一道左右柔化、可倾斜的高光带，从左往右扫过；高光只落在内容的不透明像素上，内容本身保持原色。`}</p>
      <ShimmerBasicDemo />
      <Widget className="doc-section">
        <h2>适用场景</h2>
        <ul>
          <li><Text>{`[b]适用[/b]：让一段状态文本（图标或纯文字）带着柔和高光扫过，表示“进行中/加载”`}</Text></li>
          <li><Text>{`[b]适用[/b]：流式输出时给“正在生成”的提示文字加一层滑动高光`}</Text></li>
          <li><Text>{`[b]不适用[/b]：纯灰块的骨架占位（Shimmer 叠加在真实内容上，不生成占位骨架）`}</Text></li>
          <li><Text>{`[b]不适用[/b]：有明确百分比或已知时长的任务进度（用 [bgcolor=#7f7f7f29] progress [/bgcolor]）`}</Text></li>
          <li><Text>{`[b]不适用[/b]：无固定形状的全局忙碌指示（用 [bgcolor=#7f7f7f29] spinner [/bgcolor]）`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/shimmer/0">
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
                    <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Shimmer[/color], [color=#8250df]ShimmerContent[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] { [color=#d2a8ff]Shimmer[/color], [color=#d2a8ff]ShimmerContent[/color] } [color=#ff7b72]from[/color] [color=#a5d6ff]"@lcui/react"[/color]`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`Shimmer 是 C 代码自绘部件：先按原色完整合成内容，再把一道带倾斜的高光带裁剪到内容的不透明像素上叠加。高光本身可配置颜色、周期、宽度与倾斜角，通过部件属性或对应的 [bgcolor=#7f7f7f29] ui_shimmer_set_* [/bgcolor] 接口设置：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/widgets/shimmer/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "124px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#8250df]Shimmer[/color] running={[color=#0550ae]true[/color]} duration={[color=#0550ae]2000[/color]} spread={[color=#0550ae]120[/color]} angle={[color=#0550ae]20[/color]} color=[color=#0a3069]"rgba(255,255,255,0.86)"[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ShimmerContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Text[/color]>[/color]Generating response…[color=#116329]</[color=#0550ae]Text[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ShimmerContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`</[color=#8250df]Shimmer[/color]>`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`<[color=#d2a8ff]Shimmer[/color] running={[color=#79c0ff]true[/color]} duration={[color=#79c0ff]2000[/color]} spread={[color=#79c0ff]120[/color]} angle={[color=#79c0ff]20[/color]} color=[color=#a5d6ff]"rgba(255,255,255,0.86)"[/color]>`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  [color=#7ee787]<[color=#79c0ff]ShimmerContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`    [color=#7ee787]<[color=#79c0ff]Text[/color]>[/color]Generating response…[color=#7ee787]</[color=#79c0ff]Text[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`  [color=#7ee787]</[color=#79c0ff]ShimmerContent[/color]>[/color]`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">5</Text>
                    <Text className="code-line">{`</[color=#d2a8ff]Shimmer[/color]>`}</Text>
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
        <p>{`使用以下组合来构建 [bgcolor=#7f7f7f29] Shimmer [/bgcolor]：`}</p>
        <Widget className="code-block" data-language="text" data-source="zh-CN/widgets/shimmer/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "64px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Text className="code-line">{`Shimmer`}</Text>
                  <Text className="code-line">{`└── ShimmerContent`}</Text>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Text className="code-line">{`Shimmer`}</Text>
                  <Text className="code-line">{`└── ShimmerContent`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>颜色</h2>
        <p>{`高光带叠加的颜色取自 [bgcolor=#7f7f7f29] color [/bgcolor]，其 alpha 就是扫过处的峰值强度。同一段灰字上叠不同颜色，扫过时会被“染”向各自的色相。`}</p>
        <ShimmerColorDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>时长</h2>
        <p>{`[bgcolor=#7f7f7f29] duration [/bgcolor] 决定高光扫过一次所需的时间，单位为毫秒，默认 [bgcolor=#7f7f7f29] 2000 [/bgcolor]。值越小扫得越快。`}</p>
        <ShimmerDurationDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>宽度</h2>
        <p>{`[bgcolor=#7f7f7f29] spread [/bgcolor] 决定高光带的宽度（逻辑像素）。窄带像一道锐利反光，宽带则一次漫过整行，峰值强度不变、只是覆盖范围不同。`}</p>
        <ShimmerSpreadDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>倾斜</h2>
        <p>{`[bgcolor=#7f7f7f29] angle [/bgcolor] 让高光带相对竖直方向倾斜（度数）。0° 时各行在同一列被扫到，倾斜后反光沿高度错切，先从顶部掠过再到下方。`}</p>
        <ShimmerAngleDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>关闭高亮</h2>
        <p>{`把 [bgcolor=#7f7f7f29] running [/bgcolor] 设为 [bgcolor=#7f7f7f29] false [/bgcolor] 即可关闭动画：此时不再叠加高光，内容按原色正常显示。`}</p>
        <ShimmerRunningDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API 参考</h2>
        <h3>Shimmer</h3>
        <p>{`扫光容器，捕获 [bgcolor=#7f7f7f29] ShimmerContent [/bgcolor] 子树；先按原色完整合成内容，再叠加一道可倾斜、左右渐隐的高光带并裁剪到内容不透明处。高光从左往右扫过，只在内容上显现。`}</p>
        <FieldTable fields={shimmerFields} locale="zh-CN" />
        <h3>ShimmerContent</h3>
        <p>{`被捕获的内容子树（如文本或图标加文字的状态行），作为 Shimmer 的直属子部件使用；省略时使用一个填满整行的默认占位块。Shimmer 默认块级、高度 20px，可用 [bgcolor=#7f7f7f29] width [/bgcolor]/[bgcolor=#7f7f7f29] height [/bgcolor] 覆盖。`}</p>
        <FieldTable fields={shimmerContentFields} locale="zh-CN" />
      </Widget>
    </Widget>
  );
}
