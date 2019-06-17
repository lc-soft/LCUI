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
- [许可](#许可)
- [文档](#文档)
- [构建](#构建)
- [社区](#社区)
- [贡献](#贡献)

## 介绍

LCUI 是一个图形界面开发库，主要使用 C 语言编写，支持使用 CSS 和 XML 描述界面结构和样式，可用于构建简单的桌面应用程序。

### 主要特性

- **C 语言编写：** 适用于体积较小且主要使用 C 语言实现的应用程序，以及偏向使用 C 语言编写简单应用的开发者。
- **Trad 语言绑定：** [Trad](https://github.com/lc-soft/trad) 是一个专为 LCUI 而设计的语言，它能使创建交互式 UI 变得轻而易举。
- **跨平台：** 支持 Windows 和 GNU/Linux 系统，可开发简单的 Windows 桌面应用和通用应用，以及 Linux 桌面应用。
- **XML 解析：** 支持使用 XML 标记语言描述界面的结构，更少的代码量，更直观的表达方式，易于开发和维护。
- **CSS 解析：** 支持使用 CSS 代码描述界面元素的位置、大小、背景、字体、边框等样式，与应用程序的逻辑分离，易于使用和修改。
- **类 HTML 布局：** 与浏览器处理的网页布局类似，但目前只支持块级（block）、内联块级（inline-block）和简单的弹性（flex）布局，如果你有网页开发经验的话会比较容易上手。
- **界面缩放：** 支持通过设置全局缩放比例来让界面适应不同像素密度的屏幕，也支持使用基于屏幕密度的 sp 和 dp 单位表示界面元素的位置和大小。
- **文本绘制：** 支持为文本设置全局字体、行高、水平对齐方式，支持对局部文本块单独设置颜色、背景色、加粗、斜体。
- **字体管理：** 支持加载多个字体文件，支持自定义界面中的标题、正文、引用、强调等类型的文本所使用的字体字族和风格。
- **图片处理：** 提供图片读取接口，支持读取 jpg、png 和 bmp 格式的图片。支持逐行读取，可在读取图片的过程反馈读取进度。
- **触控：** 支持多点触控，但目前只支持 Windows 系统。

### 缺少的特性

LCUI 是一个个人项目，其主要用途是方便作者能够开发简单的图形界面应用，简单也就意味着功能很少，比如：

- 没有硬件加速，图形渲染效率低下。
- 不支持剪切板，你不能选中和复制界面中的文本，也不能从其它程序复制文本到 LCUI 程序中。
- 输入法支持差，在 Linux 中仅支持输入英文字母和符号。
- 当前版本并不支持渲染圆角边框。
- 布局系统简单，不支持网格、表格等布局。

如果你有上述问题的相关解决经验，可以通过提交问题（Issue）或拉取请求（Pull Request）来向我们提供技术支持。

### 相关项目

想要了解 LCUI 具体能做什么？你可以查看以下项目：

- [Trad](https://github.com/lc-soft/trad) - 一个专为简化 C 开发而设计的语言。凭借 LCUI 与其的语言绑定，能使创建交互式 UI 变得更简单。
- [LCUI.css](https://github.com/lc-ui/lcui.css) - LCUI 组件库，包含了一些通用组件和 css 样式。
- [LCFinder](https://github.com/lc-soft/LC-Finder) - 图片管理器，LCUI 的旗舰级应用程序，你可以将它作为参考对象，以此评估 LCUI 的性能、界面效果和开发复杂度是否符合你的需求。

## 许可

LCUI 采用的开源许可证是 [MIT](http://opensource.org/licenses/MIT)。

## 文档

- 在线教程：[https://lcui.lc-soft.io/guide/](https://lcui.lc-soft.io/guide/)
- 更新日志：[CHANGELOG.zh-cn.md](CHANGELOG.zh-cn.md)

目前还没有 API 参考文档，你可以参考头文件、源代码、示例程序以及作者开发过的相关程序来了解基本用法。

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

## 社区

相信很多人已经习惯于看到那些活跃的开源项目，也习惯于向开源社区索取着各种资源，这些项目有着完善的文档、丰富的示例、活跃的社区还有成群的贡献者一起参与项目，即使自己什么都不用做，他们也能积极的发展下去。然而 LCUI 只是个个人项目，并未拥有这些资源，LCUI 的大多数功能都是按作者的需求而开发的，因此，如果你有什么需求，请先阅读以下内容。

- 遇到问题时请尽量花时间自己独立解决，实在无法解决的话，请再花些时间准备好详细的问题描述，然后按照下面给出的方法提交问题。
- 普通的使用问题，请在[开源中国社区（问答板块）](https://www.oschina.net/question/tag/LCUI)、[SegmentFault](https://segmentfault.com/search?q=LCUI) 或 [Stackoverflow](https://stackoverflow.com/search?q=LCUI) 上提交问题并 @ 作者，这样作者在帮助你解决问题后至少能赚点积分/声望值，还能让搜索引擎多收录一条 LCUI 相关的内容，帮助其他遇到类似问题的人。
- BUG、新功能建议、代码改进建议等核心开发相关的问题请提交至 [Issues](https://github.com/lc-soft/LCUI/issues) 页面中。
- 如果你想提交自己的代码改进方案，请先向作者确认这个方案是否符合要求，确认之后再开始编写代码，务必遵循[现有代码风格](docs/CodingStyle.zh-cn.md)。在代码完成后，请提交拉取请求(Pull Request)，让作者决定是否拉取你的分支里的代码并合并至主分支，必要的话请补充单元测试。
- 文档相关问题、需求及改进建议，请提交至 [LCUI-Guide](https://github.com/lc-ui/LCUI-Guide/issues) 项目里。

## 贡献

LCUI 还有很多需要完善的地方，例如：

- 部分头文件设计不合理，依赖太强。
- 部分模块代码耦合度高。
- 标识符命名不够规范。
- 目录规划不够合理。
- 不支持输入法。
- 文档太少。
- 示例太少。
- 组件太少。

由于 LCUI 是一个与作者职业无关的项目，在此项目上投入过多时间对于作者的意义并不大，因此受限于时间成本问题，像图形渲染优化、输入法支持、文档撰写、琐碎小功能等这类耗时长、收益低的工作，现阶段都不会安排在主线分支上。除去这里简单说明的一些待改进的地方外，在 LCUI 的源代码中也会有一些被 FIXME 注释标记的待改进的代码，通常这些注释还会说明为什么改进它，以及怎么改进它，如果你感兴趣，可以留意源代码中的 FIXME 注释内容，或者[直接搜索 FIXME 注释](https://github.com/lc-soft/LCUI/search?q=FIXME&unscoped_q=FIXME)。

本项目是以技术交流为主要目的而开放源代码的，如果你有相关问题解决经验，可以向此项目提供支持，与其他人分享你的经验，但在参与贡献之前，请先阅读[这篇贡献指南](CONTRIBUTING.md)。
