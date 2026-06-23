import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function TailwindCssDocPage() {
  return (
    <Widget className="doc-page">
      <h1>Tailwind CSS</h1>
      <p className="doc-page-desc">{`Tailwind CSS 是一个功能类优先（Utility-First）的 CSS 框架，通过预定义的 CSS 类帮助开发者快速设置样式。相比传统 CSS 编写方式，无需新建 CSS 文件、编写规则和思考类名。`}</p>
      <Widget className="doc-section">
        <h2>安装</h2>
        <Widget className="code-block" data-language="sh" data-source="zh-CN/devtools/tailwind-css/0">
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
        <h2>配置</h2>
        <p>{`从 lcui-quick-start 模板项目复制以下文件到项目根目录：`}</p>
        <ul>
          <li><Text>{`[bgcolor=#eee] postcss.config.js [/bgcolor]`}</Text></li>
          <li><Text>{`[bgcolor=#eee] tailwind.config.js [/bgcolor]`}</Text></li>
          <li><Text>{`[bgcolor=#eee] app/global.css [/bgcolor]`}</Text></li>
        </ul>
        <p>{`[bgcolor=#eee] global.css [/bgcolor] 中包含 Tailwind 的指令：`}</p>
        <Widget className="code-block" data-language="css" data-source="zh-CN/devtools/tailwind-css/1">
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
        <p>{`[bgcolor=#eee] @lcui/cli [/bgcolor] 编译时会通过 PostCSS 链（postcss + tailwindcss + postcss-rem-to-px）处理 Tailwind 指令，生成最终的 CSS 输出。`}</p>
        <Widget className="admonition admonition-tip">
          <Text className="admonition-title">技巧</Text>
          <Widget className="admonition-content">
            <p>{`如果你不想将 global.css 放到 app 目录内，请更改 [bgcolor=#eee] tailwind.config.js [/bgcolor] 中 [bgcolor=#eee] content [/bgcolor] 配置项的路径匹配规则。`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
