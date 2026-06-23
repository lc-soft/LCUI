import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function AboutDocPage() {
  return (
    <Widget className="doc-page">
      <h1>关于 LCUI</h1>
      <p className="doc-page-desc">{`LCUI 是一个用 C 语言编写的开源桌面图形界面库，旨在为 C 开发者提供简洁易用的 GUI 开发体验，同时融入 CSS 样式、声明式界面描述等 Web 开发技术降低学习门槛。`}</p>
      <Widget className="doc-section">
        <h2>主要特性</h2>
        <ul>
          <li><Text>{`[b]跨平台[/b] — 支持 Windows 和 Linux。`}</Text></li>
          <li><Text>{`[b]全自绘组件[/b] — 组件在多个平台中都能保持一致的外观和行为。`}</Text></li>
          <li><Text>{`[b]DPI 自适应[/b] — 自动在高分辨率屏幕上缩放 UI，保持清晰显示。`}</Text></li>
          <li><Text>{`[b]自带 CSS 引擎[/b] — 支持使用 CSS 来定义用户界面的样式和布局，对于有网页开发经验的人比较容易上手。`}</Text></li>
          <li><Text>{`[b]提供现代化的开发工具[/b] — 通过 [bgcolor=#eee] @lcui/cli [/bgcolor] 工具，允许你使用 TypeScript 语言搭配 JSX 语法来编写用户界面。`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>适合谁使用</h2>
        <p>{`LCUI 适合这些开发者：`}</p>
        <ul>
          <li><Text>{`想在桌面端继续使用 C 语言，但希望摆脱繁琐的 Win32 / X11 原生开发体验。`}</Text></li>
          <li><Text>{`已经熟悉 Web 前端（HTML、CSS），想把这套经验迁移到桌面应用。`}</Text></li>
          <li><Text>{`需要构建单窗口、界面内容简单的桌面小工具。`}</Text></li>
        </ul>
        <p>{`如果场景是大型商业桌面产品、游戏引擎或与操作系统深度集成的工具，LCUI 可能不是最合适的选择；这种场景下建议参考 Qt、GTK 或原生 API。`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>架构</h2>
        <p>{`LCUI 从上到下分为四层：`}</p>
        <h3>应用层</h3>
        <p>{`你的业务代码：自定义组件、CSS 样式、TSX/JSX 代码、事件处理。这是唯一与你的项目领域相关的部分。`}</p>
        <h3>LCUI 运行时</h3>
        <p>{`初始化、事件循环、应用生命周期管理。负责把应用跑起、派发事件、驱动渲染。`}</p>
        <h3>UI 辅助层</h3>
        <p>{`UI XML 解析、UI Router 路由、光标管理、国际化（i18n）等辅助模块。把常用 UI 行为封装成可插拔的子系统。`}</p>
        <h3>基础设施层</h3>
        <p>{`YUtil（通用工具库）、PandaGL（2D 渲染引擎）、CSS 引擎、UI 部件系统、Thread / Worker 抽象。这些模块也可以单独使用。`}</p>
        <p>{`平台层（Windows / Linux）负责窗口管理与输入事件。`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>许可证</h2>
        <p>{`LCUI 基于 MIT License 发布。详情请查看仓库根目录的 LICENSE.TXT。`}</p>
      </Widget>
      <Widget className="doc-section">
        <h2>如何参与贡献</h2>
        <p>{`欢迎贡献！在提交 Pull Request 之前，请先阅读仓库根目录的 CONTRIBUTING.md。`}</p>
        <ul>
          <li><Text>{`[b]Bug 反馈[/b] — 在 GitHub Issues 提交问题。`}</Text></li>
          <li><Text>{`[b]功能建议[/b] — 在 GitHub Discussions 发起讨论。`}</Text></li>
          <li><Text>{`[b]代码贡献[/b] — Fork 仓库，创建分支，然后提交 PR。`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>社区</h2>
        <p>{`遇到使用问题时，可以通过 GitHub Discussions 提问（建议使用 [bgcolor=#eee] Q&A [/bgcolor] 标签）。同时我们也鼓励资深用户给新人提供帮助。`}</p>
      </Widget>
    </Widget>
  );
}
