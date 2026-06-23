import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function QuickStartDocPage() {
  return (
    <Widget className="doc-page">
      <h1>快速开始</h1>
      <p className="doc-page-desc">{`几分钟内上手 LCUI。`}</p>
      <Widget className="doc-section">
        <h2>环境要求</h2>
        <ul>
          <li><Text>{`[b]操作系统[/b] — Windows（推荐）或 Linux`}</Text></li>
          <li><Text>{`[b]Node.js[/b] — 运行 [bgcolor=#eee] @lcui/cli [/bgcolor] 工具`}</Text></li>
          <li><Text>{`[b]xmake[/b] — C/C++ 构建工具`}</Text></li>
          <li><Text>{`[b]Git[/b] — 下载和管理源码`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>安装</h2>
        <p>{`全局安装 LCUI CLI：`}</p>
        <Widget className="code-block" data-language="sh" data-source="zh-CN/overview/quick-start/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">Shell</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`npm install -g @lcui/cli`}</Text>
          </Widget>
        </Widget>
        <p>{`创建并运行你的第一个应用：`}</p>
        <Widget className="code-block" data-language="sh" data-source="zh-CN/overview/quick-start/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">Shell</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`lcui create my-app`}</Text>
            <Text className="code-line">{`[color=#953800]cd[/color] my-app`}</Text>
            <Text className="code-line">{`lcui build`}</Text>
            <Text className="code-line">{`xmake run app`}</Text>
          </Widget>
        </Widget>
        <p>{`[bgcolor=#eee] lcui create [/bgcolor] 会从 lcui-quick-start 模板仓库克隆一个最小项目，包含 xmake.lua 配置、示例代码和依赖。完成后运行 [bgcolor=#eee] lcui build [/bgcolor] 编译 TSX 资源，再运行 [bgcolor=#eee] xmake run app [/bgcolor] 启动应用。`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>下一步</h2>
        <ul>
          <li><Text>{`查看样板项目（[bgcolor=#eee] lcui create [/bgcolor] 生成的项目）中的代码和文件结构，了解 LCUI 应用的基本用法。`}</Text></li>
          <li><Text>{`阅读组件参考（Button、Text、TextInput、Anchor、ScrollArea），了解内置部件。`}</Text></li>
          <li><Text>{`阅读样式章节，学习如何用 CSS 为部件添加外观。`}</Text></li>
          <li><Text>{`阅读自定义章节，学习如何创建自定义部件。`}</Text></li>
          <li><Text>{`阅读 TSX 与开发工具章节，深入理解 [bgcolor=#eee] @lcui/react [/bgcolor] 的用法和限制。`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>遇到问题？</h2>
        <p>{`如果你在学习过程中遇到问题，可以通过以下途径获取帮助：`}</p>
        <ul>
          <li><Text>{`在 GitHub Discussions 提问（建议使用 [bgcolor=#eee] Q&A [/bgcolor] 标签）。`}</Text></li>
          <li><Text>{`在仓库根目录查看 CONTRIBUTING.md，了解如何参与贡献。`}</Text></li>
        </ul>
      </Widget>
    </Widget>
  );
}
