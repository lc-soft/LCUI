import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function SassDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Sass</h1>
      <p className="doc-page-desc">{`Sass is a popular CSS preprocessor that extends CSS with variables, nesting rules, mixins, and more.`}</p>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <p>{`[bgcolor=#eee] @lcui/cli [/bgcolor] has a built-in Sass preprocessor that is automatically invoked when compiling [bgcolor=#eee] .sass [/bgcolor] and [bgcolor=#eee] .scss [/bgcolor] files — no additional installation or configuration needed.`}</p>
        <p>{`Import Sass files directly in TSX:`}</p>
        <Widget className="code-block" data-language="tsx" data-source="en/handbook/sass/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">MyComponent.tsx</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] [color=#0a3069]"./MyComponent.scss"[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`The CLI compiles Sass files into CSS, then converts them to C code for runtime loading.`}</p>
      </Widget>
    </Widget>
  );
}
