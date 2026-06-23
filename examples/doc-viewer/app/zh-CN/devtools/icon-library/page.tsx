import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function IconLibraryDocPage() {
  return (
    <Widget className="doc-page">
      <h1>图标库</h1>
      <p className="doc-page-desc">{`[bgcolor=#eee] @lcui/fluent-icons [/bgcolor] 是专为 LCUI 适配的图标库，图标都来自 Microsoft 的 fluentui-system-icons 项目。`}</p>
      <Widget className="doc-section">
        <h2>安装</h2>
        <Widget className="code-block" data-language="sh" data-source="zh-CN/devtools/icon-library/0">
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
        <h2>选取图标</h2>
        <p>{`在 flicon.io 网站中搜索和选取图标。以放大图标为例，英文名通常是 Zoom In。`}</p>
        <p>{`fluentui-system-icons 的图标有 16、20、24 等几种尺寸可选，命名方式是"图标名+尺寸+风格"。[bgcolor=#eee] @lcui/fluent-icons [/bgcolor] 的命名方式是"图标名+风格"，当风格为 Regular 时可以省略它。`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>用法</h2>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/devtools/icon-library/1">
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
        <p>{`默认尺寸是 20。如果图标尺寸固定且希望更好的渲染效果，可以指定 size 参数：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/devtools/icon-library/2">
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
