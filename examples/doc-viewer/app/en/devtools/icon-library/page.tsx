import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function IconLibraryDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Icon Library</h1>
      <p className="doc-page-desc">{`[bgcolor=#eee] @lcui/fluent-icons [/bgcolor] is an icon library adapted for LCUI. All icons come from Microsoft's fluentui-system-icons project.`}</p>
      <Widget className="doc-section">
        <h2>Installation</h2>
        <Widget className="code-block" data-language="sh" data-source="en/devtools/icon-library/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">Shell</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`npm install @lcui/fluent-icons`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>Choosing icons</h2>
        <p>{`Search and select icons on flicon.io. For example, the zoom-in icon is typically named "Zoom In" in English.`}</p>
        <p>{`fluentui-system-icons icons are available in sizes like 16, 20, and 24, named in the format "icon name + size + style". [bgcolor=#eee] @lcui/fluent-icons [/bgcolor] uses the naming format "icon name + style", and when the style is Regular it can be omitted.`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>Usage</h2>
        <Widget className="code-block" data-language="tsx" data-source="en/devtools/icon-library/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] { [color=#8250df]ZoomIn[/color] } [color=#cf222e]from[/color] [color=#0a3069]"@lcui/fluent-icons"[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#116329]<[color=#0550ae]ZoomIn[/color] />[/color]`}</Text>
          </Widget>
        </Widget>
        <p>{`The default size is 20. If the icon size is fixed and you want better rendering, specify the size parameter:`}</p>
        <Widget className="code-block" data-language="tsx" data-source="en/devtools/icon-library/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">TypeScript</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`<[color=#8250df]ZoomIn[/color] size={[color=#0550ae]32[/color]} />`}</Text>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
