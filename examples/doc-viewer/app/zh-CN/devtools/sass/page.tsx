import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function SassDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Sass</h1>
      <p className="doc-page-desc">{`Sass 是一个流行的 CSS 预处理器，通过变量、嵌套规则和混合元素等功能扩展 CSS。`}</p>
      <Widget className="doc-section">
        <h2>用法</h2>
        <p>{`[bgcolor=#eee] @lcui/cli [/bgcolor] 已内置 Sass 预处理器，在编译 [bgcolor=#eee] .sass [/bgcolor] 和 [bgcolor=#eee] .scss [/bgcolor] 后缀的文件时会自动调用，无需额外安装或配置。`}</p>
        <p>{`在 TSX 中直接导入 Sass 文件：`}</p>
        <Widget className="code-block" data-language="tsx" data-source="zh-CN/devtools/sass/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">MyComponent.tsx</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]import[/color] [color=#0a3069]"./MyComponent.scss"[/color];`}</Text>
          </Widget>
        </Widget>
        <p>{`CLI 会将 Sass 文件编译为 CSS，再转换为 C 代码在运行时加载。`}</p>
      </Widget>
    </Widget>
  );
}
