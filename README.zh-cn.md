<p align="center">
  <a href="http://lcui.org/">
    <img src="https://lcui.lc-soft.io/static/images/lcui-logo-lg.png" alt="" width=72 height=72>
  </a>
  <h3 align="center">LCUI</h3>
  <p align="center">
    C 的图形界面开发库，可借助 XML 和 CSS 构建简单的跨平台桌面应用
  </p>
  <p align="center">
    <a href="https://github.com/lc-soft/LCUI/actions"><img alt="GitHub Actions" src="https://github.com/lc-soft/LCUI/workflows/C%2FC%2B%2B%20CI/badge.svg"></a>
    <a href="https://codecov.io/gh/lc-soft/LCUI"><img src="https://codecov.io/gh/lc-soft/LCUI/branch/develop/graph/badge.svg" /></a>
    <a href="https://opensource.org/licenses/MIT"><img src="https://img.shields.io/github/license/lc-soft/LCUI.svg" alt="License"></a>
    <a href="https://github.com/lc-soft/LCUI/releases"><img src="https://img.shields.io/github/release/lc-soft/LCUI/all.svg" alt="Github Release"></a>
    <a href="https://github.com/lc-soft/LCUI/releases"><img src="https://img.shields.io/github/downloads/lc-soft/LCUI/total.svg" alt="Github All Releases"></a>
    <img src="https://img.shields.io/github/repo-size/lc-soft/LCUI.svg" alt="Repo size">
    <img src="https://img.shields.io/github/languages/code-size/lc-soft/LCUI.svg" alt="Code size">
  </p>
</p>

## 目录

<!-- TOC -->

- [目录](#%E7%9B%AE%E5%BD%95)
- [介绍](#%E4%BB%8B%E7%BB%8D)
    - [主要特性](#%E4%B8%BB%E8%A6%81%E7%89%B9%E6%80%A7)
    - [效果图](#%E6%95%88%E6%9E%9C%E5%9B%BE)
    - [相关项目](#%E7%9B%B8%E5%85%B3%E9%A1%B9%E7%9B%AE)
    - [设计参考](#%E8%AE%BE%E8%AE%A1%E5%8F%82%E8%80%83)
- [快速上手](#%E5%BF%AB%E9%80%9F%E4%B8%8A%E6%89%8B)
    - [使用 LCUI CLI](#%E4%BD%BF%E7%94%A8-lcui-cli)
    - [手动编译安装](#%E6%89%8B%E5%8A%A8%E7%BC%96%E8%AF%91%E5%AE%89%E8%A3%85)
        - [Windows](#windows)
        - [Ubuntu](#ubuntu)
- [文档](#%E6%96%87%E6%A1%A3)
- [路线图](#%E8%B7%AF%E7%BA%BF%E5%9B%BE)
- [贡献](#%E8%B4%A1%E7%8C%AE)
- [经济支持](#%E7%BB%8F%E6%B5%8E%E6%94%AF%E6%8C%81)
- [常见问题](#%E5%B8%B8%E8%A7%81%E9%97%AE%E9%A2%98)
- [许可](#%E8%AE%B8%E5%8F%AF)

<!-- /TOC -->

## 介绍

LCUI 是一个用 C 语言编写的桌面端图形界面开发库。

### 主要特性

- **C 语言编写：** 适用于体积较小且主要使用 C 语言实现的应用程序，以及偏向使用 C 语言编写简单应用的开发者。
- **跨平台：** 支持 Windows 和 GNU/Linux 系统，可开发简单的 Windows 桌面应用和通用应用，以及 Linux 桌面应用。
- **XML + CSS：** 预置 XML 和 CSS 解析器，你可以使用 XML 和 CSS 来描述界面结构和样式。
- **与网页类似的开发体验：** 由于 LCUI 的布局、样式和渲染器等相关功能的设计和实现大都参考了 MDN 文档和一些 Web 前端流行的开发库，因此开发体验和界面效果会与网页有一些相似之处，如果你已经有用 HTML 和 CSS 编过网页的经验，那么会比较容易上手。
- **可缩放：** 支持全局缩放，支持使用基于屏幕密度的 sp 和 dp 单位表示界面元素的位置和大小。

### 效果图

<table>
  <tbody>
    <tr>
      <td>
        <a class="thumbnail" href="https://github.com/lc-soft/LCUI/blob/develop/test/helloworld.c">
          <img src="https://lcui.org/static/images/showcase/screenshot-lcui-hello.png" alt="Hello App"/>
        </a>
      </td>
      <td>
        <a class="thumbnail" href="https://github.com/lc-soft/LC-Finder">
          <img src="https://gitee.com/lc-soft/LC-Finder/raw/develop/screenshots/2.jpg" alt="LC Finder"/>
        </a>
      </td>
    </tr>
    <tr>
      <td>
        <a class="thumbnail" href="https://github.com/lc-ui/lcui-router-app">
          <img src="https://gitee.com/lc-ui/lcui-router-app/raw/master/screenshot.gif" alt="LCUI Router App"/>
        </a>
      </td>
      <td>
        <a class="thumbnail" href="https://github.com/lc-ui/lc-design">
          <img src="https://lcui.lc-soft.io/static/images/showcase/lc-design-example-preview.png" alt="LC Design"/>
        </a>
      </td>
    </tr>
  </tbody>
</table>

### 相关项目

想要了解 LCUI 具体能做什么？你可以查看以下项目：

- [LCUI CLI](https://github.com/lc-ui/lcui-cli) - 用于开发 LCUI 应用程序的命令行工具。
- [LCUI Router](https://github.com/lc-soft/lcui-router) - LCUI 的路由管理器，用于解决 LCUI 应用内多视图的切换和状态管理问题，代码设计参考自 [Vue Router](https://github.com/vuejs/vue-router)。
- [LC Design](https://github.com/lc-ui/lc-design) — 专为 LCUI 开发的组件库，包含了一些通用组件和 CSS 样式，组件设计参考自 Bootstrap、ElementUI、AntDesign。
- [LC Finder](https://github.com/lc-soft/LC-Finder) — 图片管理器，LCUI 的旗舰级应用程序，你可以将它作为参考对象，以此评估 LCUI 的性能、界面效果和开发复杂度是否符合你的需求。
- [Trad](https://github.com/lc-soft/trad) — 基于 JavaScript 语法且可编译为 C 的语言，预置 LCUI 绑定，提供类似于 [React](https://reactjs.org/) 的声明式 UI 开发体验。
- [LCUI Quick Start](https://github.com/lc-ui/lcui-quick-start) - LCUI 应用程序模板。
- [LCUI Router App](https://blog.lc-soft.io/posts/build-a-browser-like-app.html) - LCUI Router 的应用程序模板，实现了和浏览器一样的图形界面、多标签页、路由导航等功能。
- [LC Design App](https://github.com/lc-ui/lc-design-app) - LC Design 组件库的应用程序模板，展示了 LC Design 的一些组件的简单用法和效果。

### 设计参考

- [SDL](https://github.com/SDL-mirror/SDL/tree/master/src/video/x11) — x11 的驱动代码参考
- [FreeType](https://www.freetype.org/freetype2/docs/design/design-3.html#section-1) — 数据结构的命名风格参考
- [LevelDB](https://github.com/google/leveldb/blob/master/include/leveldb/c.h) — 函数命名风格参考
- [jQuery](https://jquery.com/) — 部件操作接口的命名风格参考
- [MDN](https://developer.mozilla.org/zh-CN/docs/Web/CSS) — CSS 标准参考

## 快速上手

### 使用 LCUI CLI

LCUI CLI 是一个命令行工具，在使用它之前需要安装 [Node.js](https://nodejs.org/) ，之后再运行以下命令即可快速体验：

```bash
# 安装 lcui-cli 和 lcpkg
npm install -g @lcui/cli lcpkg

# 创建一个名为 myapp 的 LCUI 项目
lcui create myapp

# 进入项目目录
cd myapp

# 设置开发环境
lcui setup

# 构建项目
lcui build

# 运行项目
lcui run
```

### 手动编译安装

#### Windows

1. 打开命令行窗口，在 LCUI 的源码目录中使用以下命令安装依赖库：

    ```bash
    lcpkg install
    # 如果你需要编译 x64 版本的话
    lcpkg install --arch x64
    # 如果你需要编译适用于 Windows 通用应用平台 (UWP) 版本的话
    lcpkg install --platform uwp
    lcpkg install --arch x64 --platform uwp
    ```

1. 重命名 include 目录中的 `config.win32.h.in` 文件为 `config.h`。
1. 使用 [Visual Studio](https://visualstudio.microsoft.com/) 打开 `build/windows/LCUI.sln` 文件，然后编译生成 LCUI。

#### Ubuntu

```bash
# 安装依赖库
sudo apt-get install libpng-dev libjpeg-dev libxml2-dev libfreetype6-dev libx11-dev

# 克隆代码库
git clone https://github.com/lc-soft/LCUI.git

# 进入源码目录
cd LCUI

# 生成配置脚本
./autogen.sh

# 配置环境及构建工具
./configure

# 构建
make

# 如果需要安装的话
sudo make install

# 进入测试程序目录
cd test

#  运行 helloworld 程序
./helloworld
```

> **提示：** 如果需要自定义编译器、编译参数、安装位置等配置，请查阅 [INSTALL](INSTALL) 文件。

## 文档

- 在线教程：[https://docs.lcui.lc-soft.io](https://docs.lcui.lc-soft.io)
- 更新日志：[CHANGELOG.zh-cn.md](CHANGELOG.zh-cn.md)

## 路线图

此项目的开发计划分为主线和支线，主线中的开发任务主要由项目维护者负责推进，优先级较高，主要包括：

- LCUI 3.0 开发 ([#239](https://github.com/lc-soft/LCUI/issues/239))
- 完善文档

其它的任务则归为支线，交给开源社区贡献者以及像你这样的开发者来推进。如需了解更多细节可查看 [issues](https://github.com/lc-soft/LCUI/issues) 和[项目](https://github.com/lc-soft/LCUI/projects)页面。

## 贡献

有很多方式可以为此项目的发展做贡献：

- [反馈问题](https://github.com/lc-soft/LCUI/issues)并在问题关闭时帮助我们验证它们是否已经修复
- 在源码中[搜索 FIXME 注释](https://github.com/lc-soft/LCUI/search?l=C&q=FIXME)，然后尝试解决它们
- 在 [IssueHunt](https://issuehunt.io/r/lc-soft/LCUI) 上为感兴趣的 issue 设置悬赏，吸引其他开发者参与开发
- 审查[源代码的改动](https://github.com/lc-soft/LCUI/pulls)
- 修复已知问题

本项目采用了参与者公约定义的行为准则，该文档应用于许多开源社区，有关更多信息，请参阅[《行为准则》](CODE_OF_CONDUCT.zh-cn.md)。

## 经济支持

经济支持是最简单有效的支持方式，它能直接影响此项目的活跃度和贡献者数量，如果你看好此项目的发展前景，可以考虑在以下平台提供资金赞助：

- **爱发电:** https://afdian.net/@lc-soft
- **OpenCollective:** https://opencollective.com/LCUI

## 常见问题

1. **为什么开发它？**

   - 探索和实验新的 GUI 开发方式
   - 给职业生涯赚点加分项
   - 找点存在感
   - 打发时间

1. **这是一个浏览器内核吗？或者是像 Electron 这样的集成了浏览器环境的开发库？**

    不是，你可以当成是一个应用了部分 Web 技术的传统 GUI 开发库。

1. **我为什么要用 LCUI，而不是 Electron？**

    除了技术研究与交流，以及为开源社区发展做贡献外，你没有理由用 LCUI。相较于功能完备的 Electron 而言，文件体积小和内存占用低并没有什么用，毕竟现在机器配置都很高，即便 APP 的界面卡到爆，占用上百 MB 的内存和近 1 GB 的存储空间，只要能正常运作就够了。

1. **假如我要用它的话，需要注意什么？**

    建议你的项目满足以下条件：

    - 界面结构简单，以静态内容为主，不需要动画、表格、渐变背景色等高级特性。
    - 代码设计合理，核心功能代码与界面逻辑分离，即便在使用 LCUI 后发现不符合需求，也能轻松迁移至其它 GUI 库。

1. **适合哪些人使用？**

    适合有 GUI 应用开发经验、熟悉 Web 前端开发技术、有意向参与开源项目的 C 开发者使用，最好是具备两年 C 开发经验和一年 web 前端开发经验。以时间来衡量上手门槛可能有点不妥，以下按照技术方向分别列出了一些主要条件，你可自行判断自己是否能够快速上手。

    C：

    - 熟悉 C 语言及编译相关工具链的配置，能解决编译问题
    - 熟悉至少一种图形库和 GUI 库/框架，了解 GUI 应用程序的工作原理
    - 熟悉多线程编程，能够解决线程安全、线程同步等问题
    - 能熟练使用调试工具定位问题
    - 有较多的开源项目源码阅读经验

    Web 前端：

    - 熟练掌握常见布局
    - 熟悉 CSS 盒子模型和常用属性
    - 有良好的 CSS 编码风格
    - 了解前端工程化和相关开发工具

1. **我想要 ???? 功能，就像 ???? 里的那样。**

    请先[新建 issue](https://github.com/lc-soft/LCUI/issues/new/choose)，按照已有的模板补全内容，我们建议你尽量将你的需求描述清楚，如果能提供开发思路、相关参考文档等内容那是最好的，例如：

    ```markdown
    # 建议添加 xxx

    （简介）xxx 是......
    （特性）它能够......
    （理由）对于 LCUI 它能解决 ...... 等问题
    （实现思路）大致的实现方法是先这样......然后......再......最后......
    （参考资料）具体可参考这些 ......
    ```

    不推荐的写法：

    ```markdown
    # 建议添加 xxx

    如题，我觉得很有必要，请添加，谢谢。
    ```

1. **求添加 JavaScript/Python/Go/Rust/PHP/C#/Java 语言绑定**

    已有官方支持的 [Trad](https://github.com/lc-soft/trad) 语言绑定，不考虑添加其它语言绑定。如果你实在需要的话可以自己动手设计，毕竟你比作者更懂这些语言的编程思想和设计哲学，也算是一个展现技术实力的好机会。

## 许可

LCUI 采用的开源许可证是 [MIT](http://opensource.org/licenses/MIT)。
