import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function TailwindCssDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Tailwind CSS</h1>
      <p className="doc-page-desc">{`Tailwind CSS is a utility-first CSS framework that helps developers quickly apply styles through predefined CSS classes. Compared to traditional CSS, you don't need to create new CSS files, write rules, or think up class names.`}</p>
      <Widget className="doc-section">
        <h2>Installation</h2>
        <Widget className="code-block" data-language="sh" data-source="en/handbook/tailwind-css/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">Shell</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`npm install -D tailwindcss postcss @thedutchcoder/postcss-rem-to-px`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Configuration</h2>
        <p>{`Copy the following files from the lcui-quick-start template project to your project root:`}</p>
        <ul>
          <li><Text>{`[bgcolor=#eee] postcss.config.js [/bgcolor]`}</Text></li>
          <li><Text>{`[bgcolor=#eee] tailwind.config.js [/bgcolor]`}</Text></li>
          <li><Text>{`[bgcolor=#eee] app/global.css [/bgcolor]`}</Text></li>
        </ul>
        <p>{`[bgcolor=#eee] global.css [/bgcolor] contains the Tailwind directives:`}</p>
        <Widget className="code-block" data-language="css" data-source="en/handbook/tailwind-css/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">app/global.css</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]@tailwind[/color] base;`}</Text>
            <Text className="code-line">{`[color=#cf222e]@tailwind[/color] components;`}</Text>
            <Text className="code-line">{`[color=#cf222e]@tailwind[/color] utilities;`}</Text>
          </Widget>
        </Widget>
        <p>{`When compiling, [bgcolor=#eee] @lcui/cli [/bgcolor] processes Tailwind directives through the PostCSS chain (postcss + tailwindcss + postcss-rem-to-px) to generate the final CSS output.`}</p>
        <Widget className="admonition admonition-tip">
          <Text className="admonition-title">技巧</Text>
          <Widget className="admonition-content">
            <p>{`If you don't want to place global.css inside the app directory, change the [bgcolor=#eee] content [/bgcolor] path matching rules in [bgcolor=#eee] tailwind.config.js [/bgcolor].`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
