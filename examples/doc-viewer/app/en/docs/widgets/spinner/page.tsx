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
      <p className="doc-page-desc">{`A self-drawn spinning indicator used to express a "loading" state.`}</p>
      <SpinnerBasicDemo />
      <Widget className="doc-section">
        <h2>Use cases</h2>
        <ul>
          <li><Text>{`[b]Suitable[/b]: a loading state that waits for an async result whose progress cannot be predicted`}</Text></li>
          <li><Text>{`[b]Suitable[/b]: a busy indicator for a local area such as a button or a card`}</Text></li>
          <li><Text>{`[b]Not suitable[/b]: progress with a definite percentage or a known duration (use [bgcolor=#eee] progress [/bgcolor])`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/spinner/0">
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
        <Widget className="code-block" data-language="tsx" data-source="en/widgets/spinner/1">
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
        <h2>Composition</h2>
        <p>{`Use the following composition to build a [bgcolor=#eee] Spinner [/bgcolor]:`}</p>
        <Widget className="code-block" data-language="text" data-source="en/widgets/spinner/2">
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
        <h2>Examples</h2>
        <h3>Custom Content</h3>
        <SpinnerCustomContentDemo />
        <h3>Sizes</h3>
        <SpinnerSizeDemo />
        <h3>Running State</h3>
        <SpinnerRunningDemo />
      </Widget>
      <Widget className="doc-section">
        <h2>API Reference</h2>
        <h3>Spinner</h3>
        <p>{`The spinner container; it captures the [bgcolor=#eee] SpinnerContent [/bgcolor] subtree and draws it rotated about its center. It is 16×16 by default and can be overridden with [bgcolor=#eee] width [/bgcolor]/[bgcolor=#eee] height [/bgcolor].`}</p>
        <FieldTable fields={spinnerFields} locale="en" />
        <h3>SpinnerContent</h3>
        <p>{`The content subtree captured for rotation, used as a direct child of Spinner. When omitted, a default ring is used, with one side left transparent to form a gap that sweeps around as it rotates.`}</p>
        <FieldTable fields={spinnerContentFields} locale="en" />
      </Widget>
    </Widget>
  );
}
