import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../../components/code-block-copy";

export default function TailwindCssDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Tailwind CSS</h1>
      <p className="doc-page-desc">{`Tailwind CSS is a utility-first CSS framework that helps developers quickly apply styles through predefined CSS classes. Compared to traditional CSS, you don't need to create new CSS files, write rules, or think up class names.`}</p>
      <Widget className="doc-section">
        <h2>Installation</h2>
        <Widget className="code-block" data-language="sh" data-source="en/devtools/tailwind-css/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">Shell</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "44px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Text className="code-line">{`npm install -D tailwindcss postcss @thedutchcoder/postcss-rem-to-px`}</Text>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Text className="code-line">{`npm install -D tailwindcss postcss @thedutchcoder/postcss-rem-to-px`}</Text>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Configuration</h2>
        <p>{`Copy the following files from the lcui-quick-start template project to your project root:`}</p>
        <ul>
          <li><Text>{`[bgcolor=#7f7f7f29] postcss.config.js [/bgcolor]`}</Text></li>
          <li><Text>{`[bgcolor=#7f7f7f29] tailwind.config.js [/bgcolor]`}</Text></li>
          <li><Text>{`[bgcolor=#7f7f7f29] app/global.css [/bgcolor]`}</Text></li>
        </ul>
        <p>{`[bgcolor=#7f7f7f29] global.css [/bgcolor] contains the Tailwind directives:`}</p>
        <Widget className="code-block" data-language="css" data-source="en/devtools/tailwind-css/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">app/global.css</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <ScrollArea className="code-block-scroll" style={{ height: "84px" }}>
              <ScrollAreaContent className="code-block-scroll-content">
                <Widget className="code-block-lines code-block-lines-light">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#cf222e]@tailwind[/color] base;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`[color=#cf222e]@tailwind[/color] components;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`[color=#cf222e]@tailwind[/color] utilities;`}</Text>
                  </Widget>
                </Widget>
                <Widget className="code-block-lines code-block-lines-dark">
                  <Widget className="code-line-row">
                    <Text className="code-line-number">1</Text>
                    <Text className="code-line">{`[color=#ff7b72]@tailwind[/color] base;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">2</Text>
                    <Text className="code-line">{`[color=#ff7b72]@tailwind[/color] components;`}</Text>
                  </Widget>
                  <Widget className="code-line-row">
                    <Text className="code-line-number">3</Text>
                    <Text className="code-line">{`[color=#ff7b72]@tailwind[/color] utilities;`}</Text>
                  </Widget>
                </Widget>
              </ScrollAreaContent>
              <Scrollbar />
              <Scrollbar orientation="horizontal" />
            </ScrollArea>
          </Widget>
        </Widget>
        <p>{`When compiling, [bgcolor=#7f7f7f29] @lcui/cli [/bgcolor] processes Tailwind directives through the PostCSS chain (postcss + tailwindcss + postcss-rem-to-px) to generate the final CSS output.`}</p>
        <Widget className="admonition admonition-tip">
          <Text className="admonition-title">技巧</Text>
          <Widget className="admonition-content">
            <p>{`If you don't want to place global.css inside the app directory, change the [bgcolor=#7f7f7f29] content [/bgcolor] path matching rules in [bgcolor=#7f7f7f29] tailwind.config.js [/bgcolor].`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
