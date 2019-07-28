<p align="center">
  <a href="http://lcui.org/">
    <img src="https://lcui.lc-soft.io/static/images/lcui-logo-lg.png" alt="" width=72 height=72>
  </a>
  <h3 align="center">LCUI</h3>
  <p align="center">
    面向 C 的图形界面开发库，可借助 XML 和 CSS 构建简单的跨平台桌面应用
  </p>
  <p align="center">
    <a href="https://travis-ci.org/lc-soft/LCUI"><img src="https://travis-ci.org/lc-soft/LCUI.png?branch=master" alt="Build Status"></a>
    <a href="https://coveralls.io/github/lc-soft/LCUI?branch=develop"><img src="https://coveralls.io/repos/github/lc-soft/LCUI/badge.svg?branch=develop" alt="Coverage Status"></a>
    <a href="http://opensource.org/licenses/MIT"><img src="https://img.shields.io/github/license/lc-soft/LCUI.svg" alt="License"></a>
    <a href="https://github.com/lc-soft/LCUI/releases"><img src="https://img.shields.io/github/release/lc-soft/LCUI/all.svg" alt="Github Release"></a>
    <a href="https://github.com/lc-soft/LCUI/releases"><img src="https://img.shields.io/github/downloads/lc-soft/LCUI/total.svg" alt="Github All Releases"></a>
    <img src="https://img.shields.io/github/repo-size/lc-soft/LCUI.svg" alt="Repo size">
    <img src="https://img.shields.io/github/languages/code-size/lc-soft/LCUI.svg" alt="Code size">
  </p>
</p>

## 目录

- [介绍](#介绍)
  - [主要特性](#主要特性)
  - [缺少的特性](#缺少的特性)
  - [相关项目](#相关项目)
  - [设计参考](#设计参考)
- [构建](#构建)
  - [依赖项](#依赖项)
  - [引导](#引导)
  - [在 Windows 中构建](#在-windows-中构建)
- [贡献](#贡献)
- [许可](#许可)
- [文档](#文档)
- [常见问题](#常见问题)

## 介绍

LCUI 是一个桌面端图形界面开发库，主要使用 C 语言编写，支持使用 CSS 和 XML 描述界面结构和样式，可用于构建简单的桌面应用程序。

### 主要特性

- **C 语言编写：** 适用于体积较小且主要使用 C 语言实现的应用程序，以及偏向使用 C 语言编写简单应用的开发者。
- **跨平台：** 支持 Windows 和 GNU/Linux 系统，可开发简单的 Windows 桌面应用和通用应用，以及 Linux 桌面应用。
- **XML 和 CSS 解析：** 使用 XML 和 CSS 来描述界面的结构和样式，易于开发和维护。
- **可缩放：** 支持设置全局缩放比例来让界面适应不同像素密度的屏幕，支持使用基于屏幕密度的 sp 和 dp 单位表示界面元素的位置和大小。
- **图片处理：** 支持读取 jpg、png 和 bmp 格式的图片。
- **触控：** 支持多点触控，但目前只支持 Windows 系统。

### 缺少的特性

LCUI 是一个个人项目，其主要用途是方便作者能够开发简单的图形界面应用，简单也就意味着功能很少，比如：

- 没有硬件加速，图形渲染效率低下。
- 不支持剪切板，你不能选中和复制界面中的文本，也不能从其它程序复制文本到 LCUI 程序中。
- 输入法支持差，在 Linux 中仅支持输入英文字母和符号。
- 布局系统简单，不支持网格、表格等布局。

如今可参考的同类开源项目有很多，例如：[SDL](https://github.com/SDL-mirror/SDL)、[imgui](https://github.com/ocornut/imgui)，LCUI 中大部分缺少的特性都能从这些项目中找到相关实现，开发成本和复杂度也因此而降低了很多，如果你有这些项目的研究经验和使用经验，可以考虑向我们分享改进方案。

### 相关项目

想要了解 LCUI 具体能做什么？你可以查看以下项目：

- [Trad](https://github.com/lc-soft/trad) — 一个基于 JavaScript 语法且可编译为 C 的语言，预置 LCUI 绑定，凭借其语言特性和 JSX 语法支持可轻松地基于 LCUI 来创建交互式的图形界面。
- [LC Design](https://github.com/lc-ui/lc-design) — 专为 LCUI 开发的组件库，包含了一些通用组件和 css 样式。
- [LC Finder](https://github.com/lc-soft/LC-Finder) — 图片管理器，LCUI 的旗舰级应用程序，你可以将它作为参考对象，以此评估 LCUI 的性能、界面效果和开发复杂度是否符合你的需求。

### 设计参考

- [SDL](https://github.com/SDL-mirror/SDL/tree/master/src/video/x11) — x11 的驱动代码参考
- [FreeType](https://www.freetype.org/freetype2/docs/design/design-3.html#section-1) — 数据结构的命名风格参考
- [jQuery](https://jquery.com/) — 部件操作接口的命名风格参考
- [MDN](https://developer.mozilla.org/zh-CN/docs/Web/CSS) — CSS 标准参考

## 构建

### 依赖项

如果你想构建全特性的 LCUI，建议安装以下依赖库：

- [libpng](http://www.libpng.org/pub/png/libpng.html) — PNG 图像压缩库
- [libjpeg](http://www.ijg.org/) — JPEG 图像压缩库
- [libxml2](http://xmlsoft.org/) — XML 解析器及相关工具集
- [libx11](https://www.x.org/) — X11 客户端库
- [freetype](https://www.freetype.org/) — 字体渲染引擎

如果你的系统是 Ubuntu，可运行以下命令来安装依赖：

    apt-get install libpng-dev libjpeg-dev libxml2-dev libfreetype6-dev libx11-dev

### 引导

你需要运行 `./configure` (在源码根目录中) 以引导项目的构建。

在最简单的情况下，你可以运行：

    git clone https://github.com/lc-soft/LCUI.git
    cd LCUI
    ./configure

如果未找到 ./configure，请运行 ./autogen.sh 脚本生成它。

在 `./configure` 执行完后，运行以下命令编译源代码并安装 LCUI 的函数库和头文件：

    make
    make install

如果需要自定义编译器、编译参数、安装位置等配置，请阅读 [INSTALL](INSTALL) 文件了解详情。

测试和示例程序都存放在 test 目录中，如果需要运行他们请运行以下命令：

    cd test
    make

运行 helloworld 示例：

    ./helloworld

### 在 Windows 中构建

LCUI 主要是在 Windows 系统环境下开发的，目前只提供 Visual Studio 的解决方案文件（.sln）,你可以使用 [Visual Studio](https://visualstudio.microsoft.com/) 打开 `/build/windows/LCUI.sln` 文件，然后编译生成 LCUI。如果你用的是其它 IDE，请尝试按该 IDE 的方式创建项目并将源文件添加至项目内，然后编译。

上述的依赖库中除 Windows 系统用不到的 libx11 库外，都可以在 Windows 系统环境下编译生成，具体的编译方法你可以在教程中找到。如果你觉得编译这些依赖库很麻烦，可以使用 [vcpkg](https://github.com/Microsoft/vcpkg) 来安装它们，只需在命令行里输入如下命令：

    ./vcpkg install freetype libxml2 libjpeg-turbo libpng

[发行版](https://github.com/lc-soft/LCUI/releases)的附件列表中一般也会有一个包含所有依赖库的压缩包可供使用。

## 贡献

本项目是以技术交流为主要目的而开放源代码的，如果你有相关问题解决经验，可以向此项目提供支持，贡献方式有如下几种：

- [反馈问题](https://github.com/lc-soft/LCUI/issues)并在问题关闭时帮助我们验证它们是否已经修复
- 审查[源代码的改动](https://github.com/lc-soft/LCUI/pulls)
- [开源问答](https://www.oschina.net/question/ask)、[思否](https://segmentfault.com/)上与其他 LCUI 用户和开发人员交流
- [修复已知问题](CONTRIBUTING.zh-cn.md)

本项目采用了参与者公约定义的行为准则，该文档应用于许多开源社区，有关更多信息，请参阅[《行为准则》](CODE_OF_CONDUCT.zh-cn.md)。

## 许可

LCUI 采用的开源许可证是 [MIT](http://opensource.org/licenses/MIT)。

## 文档

- 在线教程：[https://lcui.lc-soft.io/guide/](https://lcui.lc-soft.io/guide/)
- 更新日志：[CHANGELOG.zh-cn.md](CHANGELOG.zh-cn.md)

目前还没有 API 参考文档，你可以参考头文件、源代码、示例程序以及上述的相关项目来了解基本用法。

## 常见问题

1. 适合哪些人使用？

    适合有 GUI 开发经验、熟悉 Web 前端开发技术、有意向参与开源项目的 C 开发者使用，最好是具备两年 C 开发经验和一年 web 前端开发经验。以时间来衡量上手门槛可能有点模糊，以下按照技术方向分别列出了一些主要条件，你可自行判断自己是否能够快速上手。

    C：

    - 熟悉 C 语言及编译相关工具链的配置，能解决大部分的编译问题
    - 熟悉至少一种图形库和 GUI 库/框架，了解 GUI 应用程序的工作原理
    - 熟悉多线程编程，能够解决线程安全、线程同步等问题
    - 能熟练使用调试工具定位问题

    Web 前端：

    - 熟练掌握常见布局
    - 熟悉 CSS 盒子模型和常用属性
    - 能在 CSS 新特性不可用的情况下使用基础样式来实现大部分效果
    - 有良好的 CSS 编码风格

1. 和写网页一样吗？需要注意什么？

    主要有以下差异需要注意：

    - 部件是基本的界面元素，不是文字，不支持图文混排，不存在 `inline` 显示类型。
    - 所有文本由 TextView 部件渲染，它的显示类型为 `inline-block` 而不是 `inline`。
    - 部件不会溢出父级部件的边界框，效果类似于已应用样式：`overflow: hidden;`。
    - 绝对定位的部件始终相对于其父级部件，效果类似于父部件已应用样式：`position: relative;`。
    - 没有图形化的开发工具来调试图形界面，你需要凭借自己的想象力和开发经验来验证 BUG 是来自你的代码还是 LCUI 内部的。

1. CSS 支持度如何？

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
        - [x] justify-content
          - [x] flex-start
          - [x] center
          - [x] flex-end
        - [ ] float
        - [ ] transition
        - [ ] transform
        - [ ] flex
        - [ ] ...
    </details>
