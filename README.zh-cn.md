<p align="center">
  <a href="http://lcui.org/">
    <img src="https://lcui.lc-soft.io/static/images/lcui-logo-lg.png" alt="" width=72 height=72>
  </a>
  <h3 align="center">LCUI</h3>
  <p align="center">
    C 的图形界面开发库，可借助 XML 和 CSS 构建简单的跨平台桌面应用
  </p>
  <p align="center">
    <a href="https://codecov.io/gh/lc-soft/LCUI"><img src="https://codecov.io/gh/lc-soft/LCUI/branch/develop/graph/badge.svg" /></a>
    <a href="http://opensource.org/licenses/MIT"><img src="https://img.shields.io/github/license/lc-soft/LCUI.svg" alt="License"></a>
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
    - [状态](#%E7%8A%B6%E6%80%81)
    - [效果图](#%E6%95%88%E6%9E%9C%E5%9B%BE)
    - [相关项目](#%E7%9B%B8%E5%85%B3%E9%A1%B9%E7%9B%AE)
    - [设计参考](#%E8%AE%BE%E8%AE%A1%E5%8F%82%E8%80%83)
- [快速上手](#%E5%BF%AB%E9%80%9F%E4%B8%8A%E6%89%8B)
    - [Windows](#windows)
        - [创建新的 LCUI 应用项目](#%E5%88%9B%E5%BB%BA%E6%96%B0%E7%9A%84-lcui-%E5%BA%94%E7%94%A8%E9%A1%B9%E7%9B%AE)
        - [为已有的项目安装 LCUI](#%E4%B8%BA%E5%B7%B2%E6%9C%89%E7%9A%84%E9%A1%B9%E7%9B%AE%E5%AE%89%E8%A3%85-lcui)
        - [手动编译安装](#%E6%89%8B%E5%8A%A8%E7%BC%96%E8%AF%91%E5%AE%89%E8%A3%85)
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

### 状态

平台      | 构建状态
---------|:---------
Windows  | [![Windows Build status](https://ci.appveyor.com/api/projects/status/ps6mq5sy8jkbks0y?svg=true)](https://ci.appveyor.com/project/lc-soft/lcui)
Linux    | [![Linux Build Status](https://badges.herokuapp.com/travis/lc-soft/LCUI?env=BADGE=linux&label=build&branch=develop)](https://travis-ci.org/lc-soft/LCUI) [![Linux Build Status](https://badges.herokuapp.com/travis/lc-soft/LCUI?env=BADGE=arm64&label=build%20for%20ARM64&branch=develop)](https://travis-ci.org/lc-soft/LCUI) [![Linux Build Status](https://badges.herokuapp.com/travis/lc-soft/LCUI?env=BADGE=wasm&label=build%20for%20WebAssembly&branch=develop)](https://travis-ci.org/lc-soft/LCUI) [!["GitHub Actions](https://github.com/lc-ui/lc-design/workflows/C%2FC%2B%2B%20CI/badge.svg)](https://github.com/lc-soft/LCUI/actions)
OSX      | [![OSX Build Status](https://badges.herokuapp.com/travis/lc-soft/LCUI?env=BADGE=osx&label=build&branch=develop)](https://travis-ci.org/lc-soft/LCUI)

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

### Windows

#### 创建新的 LCUI 应用项目

使用 [lcui-cli](https://github.com/lc-ui/lcui-cli) 快速创建一个 LCUI 应用项目：

```bash
# 安装 lcui-cli 和 lcpkg
npm install -g @lcui/cli lcpkg

# 创建一个名为 myapp 的 LCUI 项目
lcui create myapp

# 进入项目目录
cd myapp

# 安装依赖库
lcpkg install

# 启动这个项目
lcpkg run start
```

#### 为已有的项目安装 LCUI

在你的项目目录里使用 [lcpkg](https://github.com/lc-soft/lcpkg) 来快速安装 LCUI：

```bash
# 初始化 lcpkg 配置文件，告诉 lcpkg 你的项目相关信息
lcpkg init

# 从 GitHub 下载安装已编译好的 LCUI 库
lcpkg install github.com/lc-soft/LCUI
```

#### 手动编译安装

如果你想手动从源码编译 LCUI：

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

### Ubuntu

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

教程只有一部分，并且已经很久没有更新了，你可以参考头文件、源代码、示例程序以及上述的相关项目来了解基本用法。如果你希望我们能够为此项目提供完善的文档，可以考虑花时间帮助我们制定文档目录、章节和内容范围，相当于撰写一个文档模板，这样我们就不用花费时间在参考各种技术文档、思考初学者需要知道些什么、以及组织内容结构等工作上面，只需根据已定好的章节标题补全剩余内容即可。

## 路线图

此项目的开发计划分为主线和支线，主线中的开发任务主要由项目维护者负责推进，优先级较高，按照时间范围分为以下三类：

- 近期计划
  - 添加安卓端适配
  - 开发示例移动端应用
  - 撰写移动端应用开发教程
- 中期计划
  - 开发移动端组件库
- 未来计划
  - 为移动端组件库添加 React 实现，使之能应用在 Web 前端项目
  - 重新设计 Trad 语言并重写其编译器，使之能应付基础的 UI 开发需求
  - 重新设计图形抽象接口，支持引入主流的图形库来丰富图形元素的渲染方式
  - 优化 CSS 样式库的内存占用
  - 优化字体缓存库的内存占用
  - 支持编译为 WebAssembly 运行在浏览器上
  - 为 CSS 解析器添加支持 `@media` 查询
  - 完善文档和教程

其它的任务则归为支线，交给开源社区贡献者以及像你这样的开发者来推进。如需了解更多细节可查看 [issues](https://github.com/lc-soft/LCUI/issues) 和[项目](https://github.com/lc-soft/LCUI/projects/2)页面。

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

1. **和写网页一样吗？需要注意什么？**

    不一样，主要有以下差异需要注意：

    - 界面描述文件格式是 XML，与 HTML 有一点区别。
    - 本质上是在写 C 代码，开发效率比 JavaScript 低很多。
    - 没有 `<script>` 标签，你不能像 HTML 那样内嵌 JavaScript 或 C 代码。
    - 部件 (Widget) 是基本的界面布局元素，不是文字，不支持图文混排，不存在 `inline` 显示类型。
    - 滚动条是一个独立的部件，使用 `overflow: scroll;` 样式不会出现滚动条，你需要主动创建它并指定容器和滚动层。
    - 所有文本由 TextView 部件渲染，它的显示类型为 `block` 而不是 `inline`。
    - 部件不会溢出父级部件的边界框，效果类似于已应用样式：`overflow: hidden;`。
    - 绝对定位的部件始终相对于其父级部件，而不是父级第一个非静态定位的部件。
    - 没有像 [Chrome Devtools](https://developers.google.com/web/tools/chrome-devtools) 这样的工具来调试图形界面，你需要凭借自己的想象力和开发经验来验证 BUG 是来自你的代码还是 LCUI 内部的。

1. **CSS 支持度如何？**

    以下是支持的 CSS 特性列表，已勾选的是表示已支持（至少支持基本功能），未列出的属性则默认不支持。

    <details>
      <summary>CSS 特性覆盖范围</summary>

      - at rules
        - [x] `@font-face`
        - [ ] `@keyframes`
        - [ ] `@media`
      - keywords
        - [ ] `!important`
      - selectors
        - [x] `*`
        - [x] `type`
        - [x] `#id`
        - [x] `.class`
        - [x] `:hover`
        - [x] `:focus`
        - [x] `:active`
        - [x] `:first-child`
        - [x] `:last-child`
        - [ ] `[attr="value"]`
        - [ ] `:not()`
        - [ ] `:nth-child()`
        - [ ] `parent > child`
        - [ ] `a ~ b`
        - [ ] `::after`
        - [ ] `::before`
        - [ ] ...
      - units
        - [x] px
        - [x] dp
        - [x] sp
        - [x] pt
        - [x] %
        - [ ] rem
        - [ ] vh
        - [ ] vw
      - properties
        - [x] top, right, bottom, left
        - [x] width, height
        - [x] visiblility
        - [x] display
          - [x] none
          - [x] inline-block
          - [x] block
          - [x] flex
          - [ ] inline-flex
          - [ ] inline
          - [ ] grid
          - [ ] table
          - [ ] table-cell
          - [ ] table-row
          - [ ] table-column
          - [ ] ...
        - [x] position
          - [x] static
          - [x] relative
          - [x] absolute
          - [ ] fixed
        - [x] box-sizing
          - [x] border-box
          - [x] content-box
        - [x] border
        - [x] border-radius
        - [x] background-color
        - [x] background-image
        - [x] background-position
        - [x] background-cover
        - [ ] background
        - [x] pointer-evnets
        - [x] font-face
        - [x] font-family
        - [x] font-size
        - [x] font-style
        - [x] flex
        - [x] flex-shrink
        - [x] flex-grow
        - [x] flex-basis
        - [x] flex-wrap
        - [x] flex-direction
        - [x] justify-content
          - [x] flex-start
          - [x] center
          - [x] flex-end
        - [x] align-items
          - [x] flex-start
          - [x] center
          - [x] flex-end
          - [x] stretch
        - [ ] float
        - [ ] transition
        - [ ] transform
        - [ ] ...
    </details>

1. **我想要 ???? 功能，就像 ???? 里的那样。**

    请先[新建 issue](https://github.com/lc-soft/LCUI/issues/new/choose)，按照已有的模板补全内容。

1. **求添加 JavaScript/Python/Go/Rust/PHP/C#/Java 语言绑定**

    已有官方支持的 [Trad](https://github.com/lc-soft/trad) 语言绑定，不考虑添加其它语言绑定。如果你实在需要的话可以自己动手设计，毕竟你比作者更懂这些语言的编程思想和设计哲学，也算是一个展现技术实力的好机会。

## 许可

LCUI 采用的开源许可证是 [MIT](http://opensource.org/licenses/MIT)。