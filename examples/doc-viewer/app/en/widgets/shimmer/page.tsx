import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";
import ShimmerBasicDemo from "../../../examples/shimmer-basic/index";
import ShimmerColorDemo from "../../../examples/shimmer-color/index";
import ShimmerDurationDemo from "../../../examples/shimmer-duration/index";
import ShimmerSpreadDemo from "../../../examples/shimmer-spread/index";
import ShimmerAngleDemo from "../../../examples/shimmer-angle/index";
import ShimmerRunningDemo from "../../../examples/shimmer-running/index";
import FieldTable from "../../../components/field-table";
import { shimmerFields } from "../../../widget-fields/shimmer";
import { shimmerContentFields } from "../../../widget-fields/shimmer-content";

export default function ShimmerDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Shimmer</h1>
      <p className="doc-page-desc">{`Overlays a soft-edged, tiltable highlight band on text, icons, or other content and sweeps it from left to right; the highlight only lands on the content's opaque pixels while the content keeps its own colors.`}</p>
      <ShimmerBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: letting a status line (an icon or plain text) sweep with a soft highlight to signal "in progress / loading"`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: adding a sliding highlight to a "generating" hint while text streams in`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: a plain gray skeleton placeholder (Shimmer overlays real content and does not generate a skeleton)`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: progress with a definite percentage or a known duration (use [bgcolor=#eee] progress [/bgcolor])`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: a global busy indicator without a fixed shape (use [bgcolor=#eee] spinner [/bgcolor])`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/shimmer/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-line-row">
                  <Text className="code-line-number">1</Text>
                  <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]Shimmer[/color], [color=#8250df]ShimmerContent[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color]`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`Shimmer is a self-drawn widget implemented in C: it first composites the content at full strength in its own colors, then overlays a tilted highlight band clipped to the content's opaque pixels. The highlight itself can be configured with a color, period, width, and tilt, either through widget properties or the matching [bgcolor=#eee] ui_shimmer_set_* [/bgcolor] APIs:`}</p>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/shimmer/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "124px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
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
              </ScrollAreaContent>
              <Scrollbar />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Composition</h2>
        <p>{`Use the following composition to build a [bgcolor=#eee] Shimmer [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/shimmer/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TEXT</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "64px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Text className="code-line">{`Shimmer`}</Text>
                <Text className="code-line">{`└── ShimmerContent`}</Text>
              </ScrollAreaContent>
              <Scrollbar />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Color</h2>
        <p>{`The highlight band takes its color from [bgcolor=#eee] color [/bgcolor], and its alpha is the peak strength where it sweeps. Overlaying different colors on the same gray text tints the sweep toward each hue as it passes.`}</p>
        <ShimmerColorDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Duration</h2>
        <p>{`[bgcolor=#eee] duration [/bgcolor] sets how long one sweep takes, in milliseconds; it defaults to [bgcolor=#eee] 2000 [/bgcolor]. A smaller value sweeps faster.`}</p>
        <ShimmerDurationDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Width</h2>
        <p>{`[bgcolor=#eee] spread [/bgcolor] sets the width of the highlight band in logical pixels. A narrow band reads as a sharp glint, while a wide one washes over the whole row at once, with the same peak strength and only a different span.`}</p>
        <ShimmerSpreadDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Angle</h2>
        <p>{`[bgcolor=#eee] angle [/bgcolor] tilts the band relative to vertical, in degrees. At 0° every row is swept at the same column; when tilted, the glint shears along the height, grazing the top before the lower part.`}</p>
        <ShimmerAngleDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>Disabling</h2>
        <p>{`Set [bgcolor=#eee] running [/bgcolor] to [bgcolor=#eee] false [/bgcolor] to stop the animation: no highlight is added and the content shows in its own colors.`}</p>
        <ShimmerRunningDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>Shimmer</h3>
        <p>{`The shimmer container; it captures the [bgcolor=#eee] ShimmerContent [/bgcolor] subtree, composites it at full strength in its own colors, then overlays a tiltable highlight band that fades out on both sides, clipped to the content's opaque pixels. The highlight sweeps from left to right and only shows on the content.`}</p>
        <FieldTable fields={shimmerFields} locale="en" />
        <h3>ShimmerContent</h3>
        <p>{`The captured content subtree (such as a status row of text, or an icon plus text), used as a direct child of Shimmer. When omitted, a default placeholder block filling the whole row is used. Shimmer is block-level with a height of 20px by default, which can be overridden with [bgcolor=#eee] width [/bgcolor]/[bgcolor=#eee] height [/bgcolor].`}</p>
        <FieldTable fields={shimmerContentFields} locale="en" />
      </Widget>
    </Widget>
  );
}
