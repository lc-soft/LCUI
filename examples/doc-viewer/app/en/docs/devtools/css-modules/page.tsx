import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";

export default function CssModulesDocPage() {
  return (
    <Widget className="doc-page">
      <h1>CSS Modules</h1>
      <p className="doc-page-desc">{`CSS Modules create locally scoped CSS classes for TSX components, avoiding naming conflicts and improving maintainability.`}</p>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <p>{`Create a file with the [bgcolor=#7f7f7f29] .module.css [/bgcolor] suffix:`}</p>
        <Widget className="code-block" data-language="css" data-source="en/devtools/css-modules/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">MyComponent.module.css</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "104px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#953800].card[/color] {`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  border: [color=#0550ae]1px[/color] solid [color=#0550ae]#eee[/color];`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  border-radius: [color=#0550ae]4px[/color];`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`}`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#79c0ff].card[/color] {`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`  border: [color=#79c0ff]1px[/color] solid [color=#79c0ff]#eee[/color];`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`  border-radius: [color=#79c0ff]4px[/color];`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">4</Text>
                    <Text className="code-line">{`}`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`Import it in a [bgcolor=#7f7f7f29] .tsx [/bgcolor] file:`}</p>
        <Widget className="code-block" data-language="tsx" data-source="en/devtools/css-modules/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">MyComponent.tsx</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#cf222e]import[/color] styles [color=#cf222e]from[/color] [color=#0a3069]"./MyComponent.module.css"[/color];`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]import[/color] styles [color=#ff7b72]from[/color] [color=#a5d6ff]"./MyComponent.module.css"[/color];`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`Use the [bgcolor=#7f7f7f29] styles [/bgcolor] object instead of string class names in JSX:`}</p>
        <Widget className="code-block" data-language="diff" data-source="en/devtools/css-modules/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">DIFF</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "64px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`- <Widget className="card" />`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`+ <Widget className={styles.card} />`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`- <Widget className="card" />`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`+ <Widget className={styles.card} />`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`During compilation, [bgcolor=#7f7f7f29] @lcui/cli [/bgcolor] converts [bgcolor=#7f7f7f29] .module.css [/bgcolor] into C identifier bindings, automatically generating unique C constant names for class names to avoid global conflicts.`}</p>
      </Widget>
    </Widget>
  );
}
