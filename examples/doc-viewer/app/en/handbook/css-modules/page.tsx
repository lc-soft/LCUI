import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function CssModulesDocPage() {
  return (
    <Widget className="doc-page">
      <h1>CSS Modules</h1>
      <p className="doc-page-desc">{`CSS Modules create locally scoped CSS classes for TSX components, avoiding naming conflicts and improving maintainability.`}</p>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <p>{`Create a file with the [bgcolor=#eee] .module.css [/bgcolor] suffix:`}</p>
        <Widget className="code-block" data-language="css" data-source="en/handbook/css-modules/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">MyComponent.module.css</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800].card[/color] {`}</Text>
            <Text className="code-line">{`  border: [color=#0550ae]1px[/color] solid [color=#0550ae]#eee[/color];`}</Text>
            <Text className="code-line">{`  border-radius: [color=#0550ae]4px[/color];`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
        <p>{`Import it in a [bgcolor=#eee] .tsx [/bgcolor] file:`}</p>
        <Widget className="code-block" data-language="tsx" data-source="en/handbook/css-modules/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">MyComponent.tsx</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] styles [color=#cf222e]from[/color] [color=#0a3069]"./MyComponent.module.css"[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`Use the [bgcolor=#eee] styles [/bgcolor] object instead of string class names in JSX:`}</p>
        <Widget className="code-block" data-language="diff" data-source="en/handbook/css-modules/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">DIFF</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`- <Widget className="card" />`}</Text>
            <Text className="code-line">{`+ <Widget className={styles.card} />`}</Text>
          </Widget>
        </Widget>
        <p>{`During compilation, [bgcolor=#eee] @lcui/cli [/bgcolor] converts [bgcolor=#eee] .module.css [/bgcolor] into C identifier bindings, automatically generating unique C constant names for class names to avoid global conflicts.`}</p>
      </Widget>
    </Widget>
  );
}
