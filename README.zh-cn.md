# The LCUI Project

![LCUI Logo](https://lcui.lc-soft.io/static/images/lcui-logo-lg.png)

[![Join the chat at https://gitter.im/lc-soft/LCUI](https://img.shields.io/badge/%E8%AE%A8%E8%AE%BA%E7%BB%84-bearychat-2dbe60.svg)](https://img.shields.io/badge/%E8%AE%A8%E8%AE%BA%E7%BB%84-bearychat-2dbe60.svg)
[![加入讨论组 https://lcsoft.bearychat.com/apply](https://img.shields.io/badge/%E8%AE%A8%E8%AE%BA%E7%BB%84-bearychat-2dbe60.svg)](https://lcsoft.bearychat.com/apply)
[![Build Status](https://travis-ci.org/lc-soft/LCUI.png?branch=master)](https://travis-ci.org/lc-soft/LCUI)
[![Coverage Status](https://coveralls.io/repos/github/lc-soft/LCUI/badge.svg?branch=develop)](https://coveralls.io/github/lc-soft/LCUI?branch=develop)
[![Github All Releases](https://img.shields.io/github/downloads/lc-soft/LCUI/total.svg)](https://github.com/lc-soft/LCUI/releases)
[![License](https://img.shields.io/badge/license-GPLv2-blue.svg)](http://www.gnu.org/licenses/old-licenses/gpl-2.0.html)
[![OpenCollective](https://opencollective.com/lcui/backers/badge.svg)](#backers) 
[![OpenCollective](https://opencollective.com/lcui/sponsors/badge.svg)](#sponsors)

## 说明

LCUI 是一个简单的跨平台图形界面库，其诞生目的主要是方便作者开发图形界面程序，源代码主要由 C 语言编写，支持使用 XML 和 CSS 代码来描述图像界面，适用于开发一些简单的小程序，可以实现一些简单的效果，类似于这样：

![example screenshot](https://lcui.lc-soft.io/static/images/feature-screenshot-02.png)

LCUI 是一个自由软件项目，任何人都可以参与改进它，但由于它并不是由一个专业的团队在负责全职开发工作，开发进度非常慢，大多数的功能都是按作者的需求而开发的，因此，如果你遇到 BUG，或者想添加一些新功能，请尽量尝试自己手动处理，作者也非常乐意接受你在 GitHub 上通过 Pull Request 功能贡献的代码。如果你需要一个成熟稳定的图形界面解决方案，请使用主流的图形界面开发库。

请阅读 `docs/CHANGES.zh-cn.md` 文件了解新版本的更新内容。

阅读 `docs/FAQ.zh-cn.md` 文件可以找到常见问题的解决方法。

阅读 `docs/LICENSE.TXT`  文件了解 GPLv2 许可协议的具体内容。

项目主页：http://lcui.lc-soft.io

联系邮箱：root@lc-soft.io

Github：https://github.com/lc-soft/LCUI

## 许可

LCUI 采用的开源许可证是 [GNU通用公共许可证第二版](http://www.gnu.org/licenses/gpl-2.0.html)，简称 GPLv2。简而言之，你有权利使用、修改、传播 LCUI 的源代码，当你传播给其他人时，你也必须让他们对你传播的 LCUI 副本拥有同样的权利。另外需要注意的是，如果你的应用程序用到了 LCUI 的代码，并且想传播给其他人，那么除 Windows API、MFC、DirectX 等这类系统级的函数库外，该应用程序及其它依赖库的源代码都必须向他们公开。

或许该许可协议在国内没有多大作用，如果你不想遵循该许可协议，那么使用 LCUI 也就意味着你需要独自解决它带来的各种问题，例如：功能缺陷、偶尔崩溃、性能和内存占用等问题，作者不承担任何责任，也不要试图联系作者协助解决问题，因为作者没有义务为违反协议的人提供技术支持。

## 文档

教程：https://lcui.lc-soft.io/guide/

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

如果需要运行示例程序，可运行命令来编译生成示例程序：

	cd test
	make

### 依赖项

如果你想构建全特性的 LCUI，建议安装以下依赖库：

 * [libpng](http://www.libpng.org/pub/png/libpng.html) — PNG 图像压缩库
 * [libjpeg](http://www.ijg.org/) — JPEG 图像压缩库
 * [libxml2](http://xmlsoft.org/) — XML 解析器及相关工具集
 * [libx11](https://www.x.org/) — X11 客户端库
 * [freetype](https://www.freetype.org/) — 字体渲染引擎

如果你的系统是 Ubuntu，可运行以下命令来安装依赖：

	apt-get install libpng-dev libjpeg-dev libxml2-dev libfreetype6-dev libx11-dev


### 在 Windows 中构建

LCUI 主要是在 Windows 系统环境下开发的，你可以使用 VisualStudio 打开 `/build/windows/LCUI.sln` 文件，然后编译生成 LCUI。如果你用的是其它 IDE，请尝试按该 IDE 的方式创建项目并将源文件添加至项目内，然后编译。

上述的依赖库中除 Windows 系统用不到的 libx11 库外，都可以在 Windows 系统环境下编译生成，如果觉得手动编译它们很 麻烦，想要现成可用的依赖库和头文件，可以在网上搜索，或者联系作者。

## 贡献

**你可以通过 GitHub 向本项目提交合并请求**

补丁应符合以下要求：

1. 遵循现有代码风格，请参考 `docs/CodingStyle.zh-cn.md` 文件。
2. 一次提交应该做完整的一件事。
3. 提交信息的第一行为摘要行，长度应低于80个字符，如果需要描述修改原因，请在摘要行后留空行，然后再补充。
4. 修复之前的提交（已经合并）中存在的 bug，提交信息的摘要行应该以 `修复` 或含义类似的词语开头，表示修复了它。如果你修复的是 issues 列表中未解决的问题，需在摘要行中加上 issues 列表中对应问题的编号。
5. 衍合你的分支至源项目的 develop 分支。我们不希望落拉取到冗余的合并请求。
6. **要清楚什么许可证适用于你的补丁：** 该代码库中的文件基于GPLv2（或更高版本），但（原作者）我们仍然可以创建非自由的衍生工具。然而，如果给我们的补丁是基于GPL的，我们希望它以后不会进入任何非自由的衍生工具，因此，如果补丁发布于公共领域，会为我们带来方便（以及避免任何法律问题）。

**你可以通过捐赠来支持 LCUI 的开发**

作者目前是一名独立开发者，你的贡献是非常有帮助的，如果你想通过捐赠来让作者能够花更多的时间来改进该项目，可以访问作者在 [Patreon](https://www.patreon.com/lcsoft) 和 [OpenCollective](https://opencollective.com/lcui) 上设立的页面。
### GitHub 流程

开发补丁应遵循以下流程：

#### 初始设置

1. 在 GitHub 上进行Fork (点击 Fork 按钮)
2. Clone 到计算机：`git clone git@github.com:你的用户名/LCUI.git`
3. cd 到代码库中：`cd LCUI`
4. 设置远程仓库记录，假设该记录名为 upstream，那么命令为：`git remote add -f upstream git://github.com/lc-soft/LCUI.git`

#### 添加特性

1. 为新的特性创建一个分支，假设分支名为 my_new_feature，那么命令为：`git checkout -b my_new_feature`
2. 在你的分支上工作, 像往常一样添加和提交修改。

创建一个分支并非必须的，但是，当已经合并至源项目时可以方便删除你的分支，也可以在提交合并请求前比较你的分支和源项目的最终版本，然后提交合并请求。

#### 推送到 GitHub

1. 推送分支至 GitHub：`git push origin my_new_feature`
2. 发起合并请求：在Github上点击 `Pull Request` 按钮

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

--- end of README.zh-cn.md ---
