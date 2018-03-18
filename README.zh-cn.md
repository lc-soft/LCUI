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
- [支持者](#支持者)
- [赞助商](#赞助商)

## 介绍

LCUI 是一种自由和开放源代码的图形界面开发库，主要使用 C 语言编写，支持使用 CSS 和 XML 描述界面结构和样式，可用于构建简单的桌面应用程序。

### 主要特性

- **C 语言编写：** 适用于体积较小且主要使用 C 语言实现的应用程序，以及偏向使用 C 语言编写简单应用的开发者。
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
- 不支持选中和复制界面中的文本。
- 不支持实用 CTRL+C 复制内容，不支持使用 CTRL+V 粘贴内容到输入框。
- 不支持使用输入法输入非英文字符，例如：中文，日文，等。
- 当前版本并不支持渲染圆角边框。
- 布局系统简单，不支持网格、表格等布局。

如果你有上述问题的相关解决经验，可以通过提交问题（Issue）或拉取请求（Pull Request）来向作者提供技术支持。

### 效果图

![Hello, World!](https://lcui.lc-soft.io/static/images/screenshot-lcui-hello.png)

[![LCUI.css](https://lcui.lc-soft.io/static/images/screenshot-lcui-css.gif)](https://github.com/lc-ui/LCUI.css)

[![LCFinder](https://lcui.lc-soft.io/static/images/screenshot-lcfinder.png)](https://github.com/lc-soft/LC-Finder)

## 许可

LCUI 采用的开源许可证是 [MIT](http://opensource.org/licenses/MIT)。

## 文档

- 在线教程：[https://lcui.lc-soft.io/guide/](https://lcui.lc-soft.io/guide/)
- 更新日志：[docs/CHANGES.zh-cn.md](docs/CHANGES.zh-cn.md)
- 常见问题：[docs/FAQ.zh-cn.md](docs/FAQ.zh-cn.md)

目前还没有 API 参考文档，你可以参考头文件、源代码、示例程序以及作者开发过的相关程序来了解基本用法。

## 构建

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

### 依赖项

如果你想构建全特性的 LCUI，建议安装以下依赖库：

- [libpng](http://www.libpng.org/pub/png/libpng.html) — PNG 图像压缩库
- [libjpeg](http://www.ijg.org/) — JPEG 图像压缩库
- [libxml2](http://xmlsoft.org/) — XML 解析器及相关工具集
- [libx11](https://www.x.org/) — X11 客户端库
- [freetype](https://www.freetype.org/) — 字体渲染引擎

如果你的系统是 Ubuntu，可运行以下命令来安装依赖：

    apt-get install libpng-dev libjpeg-dev libxml2-dev libfreetype6-dev libx11-dev

### 在 Windows 中构建

LCUI 主要是在 Windows 系统环境下开发的，你可以使用 VisualStudio 打开 `/build/windows/LCUI.sln` 文件，然后编译生成 LCUI。如果你用的是其它 IDE，请尝试按该 IDE 的方式创建项目并将源文件添加至项目内，然后编译。

上述的依赖库中除 Windows 系统用不到的 libx11 库外，都可以在 Windows 系统环境下编译生成，具体的编译方法你可以在教程中找到。如果你觉得编译这些依赖库很麻烦，可以试试用 [vcpkg](https://github.com/Microsoft/vcpkg) 来安装它们，而[发行版](https://github.com/lc-soft/LCUI/releases)的附件列表中一般也会有一个包含所有依赖库的压缩包可供使用。

## 社区

相信很多人已经习惯于看到那些活跃的开源项目，也习惯于向开源社区索取着各种资源，这些项目有着完善的文档、丰富的示例、活跃的社区还有成群的贡献者一起参与项目，即使自己什么都不用做，他们也能积极的发展下去。然而 LCUI 只是个个人项目，并未拥有这些资源，LCUI 的大多数功能都是按作者的需求而开发的，因此，如果你有什么需求，请先阅读以下内容。

- 遇到问题时请尽量花时间自己独立解决，实在无法解决的话，请再花些时间准备好详细的问题描述，然后按照下面给出的方法提交问题。
- 普通的使用问题，请在[开源中国社区（问答板块）](https://www.oschina.net/question/tag/LCUI)、[SegmentFault](https://segmentfault.com/search?q=LCUI) 或 [Stackoverflow](https://stackoverflow.com/search?q=LCUI) 上提交问题并 @ 作者，这样作者在帮助你解决问题后至少能赚点积分/声望值，还能让搜索引擎多收录一条 LCUI 相关的内容，帮助其他遇到类似问题的人。
- BUG、新功能建议、代码改进建议等核心开发相关的问题请提交至 GitHub 的 [Issues](https://github.com/lc-soft/LCUI/issues) 页面中。建议你使用英语撰写内容，因为 LCUI 不仅仅面向国内程序员。
- 如果你想提交自己的代码改进方案，请先向作者确认这个方案是否符合要求，确认之后再开始编写代码，务必遵循[现有代码风格](docs/CodingStyle.zh-cn.md)。在代码完成后，请提交拉取请求(Pull Request)，让作者决定是否拉取你的分支里的代码并合并至主分支，必要的话请补充单元测试。
- 文档相关问题、需求及改进建议，请提交至 [LCUI-Guide](https://github.com/lc-ui/LCUI-Guide/issues) 项目里。

## 贡献

LCUI 还有很多需要完善的地方，相信有经验的人很容易就能发现，例如：

- 图形渲染效率低下，不支持硬件加速。
- 部分头文件设计不合理，依赖太强。
- 部分模块代码耦合度高。
- 标识符命名不够规范。
- 目录规划不够合理。
- 不支持输入法。
- 文档太少。
- 示例太少。
- 组件太少。

由于 LCUI 是一个与作者职业无关的项目，在此项目上投入过多时间对于作者的意义并不大，因此受限于时间成本问题，像图形渲染优化、输入法支持、文档撰写、琐碎小功能等这类耗时长、收益低的工作，现阶段都不会安排在主线分支上。本项目是以技术交流为主要目的而开放源代码的，如果你有相关问题解决经验，可以向此项目提供支持，与其他人分享你的经验。

在参与贡献之前，请先阅读[这篇贡献指南](CONTRIBUTING.md)。

## 支持者

每月捐赠支持该项目的持续发展。 [[成为支持者](https://opencollective.com/lcui#backer)]

<a href="https://opencollective.com/lcui/backer/0/website" target="_blank"><img src="https://opencollective.com/lcui/backer/0/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/1/website" target="_blank"><img src="https://opencollective.com/lcui/backer/1/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/2/website" target="_blank"><img src="https://opencollective.com/lcui/backer/2/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/3/website" target="_blank"><img src="https://opencollective.com/lcui/backer/3/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/4/website" target="_blank"><img src="https://opencollective.com/lcui/backer/4/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/5/website" target="_blank"><img src="https://opencollective.com/lcui/backer/5/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/6/website" target="_blank"><img src="https://opencollective.com/lcui/backer/6/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/7/website" target="_blank"><img src="https://opencollective.com/lcui/backer/7/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/8/website" target="_blank"><img src="https://opencollective.com/lcui/backer/8/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/9/website" target="_blank"><img src="https://opencollective.com/lcui/backer/9/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/10/website" target="_blank"><img src="https://opencollective.com/lcui/backer/10/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/11/website" target="_blank"><img src="https://opencollective.com/lcui/backer/11/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/12/website" target="_blank"><img src="https://opencollective.com/lcui/backer/12/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/13/website" target="_blank"><img src="https://opencollective.com/lcui/backer/13/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/14/website" target="_blank"><img src="https://opencollective.com/lcui/backer/14/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/15/website" target="_blank"><img src="https://opencollective.com/lcui/backer/15/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/16/website" target="_blank"><img src="https://opencollective.com/lcui/backer/16/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/17/website" target="_blank"><img src="https://opencollective.com/lcui/backer/17/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/18/website" target="_blank"><img src="https://opencollective.com/lcui/backer/18/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/19/website" target="_blank"><img src="https://opencollective.com/lcui/backer/19/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/20/website" target="_blank"><img src="https://opencollective.com/lcui/backer/20/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/21/website" target="_blank"><img src="https://opencollective.com/lcui/backer/21/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/22/website" target="_blank"><img src="https://opencollective.com/lcui/backer/22/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/23/website" target="_blank"><img src="https://opencollective.com/lcui/backer/23/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/24/website" target="_blank"><img src="https://opencollective.com/lcui/backer/24/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/25/website" target="_blank"><img src="https://opencollective.com/lcui/backer/25/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/26/website" target="_blank"><img src="https://opencollective.com/lcui/backer/26/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/27/website" target="_blank"><img src="https://opencollective.com/lcui/backer/27/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/28/website" target="_blank"><img src="https://opencollective.com/lcui/backer/28/avatar.svg"></a>
<a href="https://opencollective.com/lcui/backer/29/website" target="_blank"><img src="https://opencollective.com/lcui/backer/29/avatar.svg"></a>

## 赞助商

成为赞助商，将你的 LOGO 展示在 README 文档和项目主页里。 [[成为赞助商](https://opencollective.com/lcui#sponsor)]

<a href="https://opencollective.com/lcui/sponsor/0/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/0/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/1/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/1/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/2/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/2/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/3/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/3/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/4/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/4/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/5/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/5/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/6/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/6/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/7/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/7/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/8/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/8/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/9/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/9/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/10/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/10/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/11/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/11/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/12/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/12/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/13/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/13/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/14/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/14/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/15/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/15/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/16/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/16/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/17/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/17/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/18/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/18/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/19/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/19/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/20/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/20/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/21/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/21/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/22/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/22/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/23/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/23/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/24/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/24/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/25/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/25/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/26/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/26/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/27/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/27/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/28/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/28/avatar.svg"></a>
<a href="https://opencollective.com/lcui/sponsor/29/website" target="_blank"><img src="https://opencollective.com/lcui/sponsor/29/avatar.svg"></a>
