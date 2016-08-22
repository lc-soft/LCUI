# The LCUI Project

![LCUI Logo](http://lcui.org/static/images/lcui-project-logo.png)

[![Join the chat at https://gitter.im/lc-soft/LCUI](https://badges.gitter.im/lc-soft/LCUI.svg)](https://gitter.im/lc-soft/LCUI?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Build Status](https://travis-ci.org/lc-soft/LCUI.png?branch=master)](https://travis-ci.org/lc-soft/LCUI)
[![Downloads](https://img.shields.io/sourceforge/dt/lc-ui.svg)](https://sourceforge.net/projects/lc-ui)
[![License](https://img.shields.io/badge/license-GPLv2-blue.svg)](http://www.gnu.org/licenses/old-licenses/gpl-2.0.html)

## 说明

LCUI 是一个简单的跨平台图形界面库，其诞生目的主要是方便作者开发图形界面程序，
源代码主要由 C 语言编写，支持使用 XML 和 CSS 代码来描述图像界面，适用于开发一
些简单的小程序，可以实现一些简单的效果，类似于这样：

![example screenshot](https://lcui.lc-soft.io/static/images/feature-screenshot-02.png)

LCUI 是一个自由软件项目，基于 [GNU通用公共许可协议](http://www.gnu.org/licenses/gpl-2.0.html) 
发布，该许可协议要求 LCUI 及 LCUI 衍生软件无论以何种形式发布，都必须确保收受者
能收到或得到该版本的源代码。此项目主要由作者利用闲余时间进行开发和维护，项目的
活跃度通常会受到各种因素的影响，大多数时间内都处于低活跃度状态。

请阅读 `docs/CHANGES.zh-cn.md` 文件了解新版本的更新内容。

阅读 `docs/FAQ.zh-cn.md` 文件可以找到常见问题的解决方法。

阅读 `docs/LICENSE.TXT`  文件了解 GPLv2 许可协议的具体内容。

项目主页：http://lcui.lc-soft.io

联系邮箱：lc-soft@live.cn

Github：https://github.com/lc-soft/LCUI

## 文档

教程：https://lcui.lc-soft.io/guide/

目前还没有 API 参考文档，你可以参考头文件、源代码、示例程序以及作者开发过的相
关程序来了解基本用法。

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
 * [libx11-dev](https://www.x.org/) — X11 客户端库
 * [freetype](https://www.freetype.org/) — 字体渲染引擎

如果你的系统是 Ubuntu，可运行以下命令来安装依赖：

	apt-get install libpng-dev libjpeg-dev libxml2-dev libfreetype6-dev libx11-dev


### 在 Windows 中构建

LCUI 主要是在 Windows 系统环境下开发的，你可以使用 VisualStudio 打开 
`/build/VS2012/LCUI.sln` 文件，然后编译生成 LCUI。如果你用的是其它 IDE，请尝
试按该 IDE 的方式创建项目并将源文件添加至项目内，然后编译。

上述的依赖库除 libx11 外，都可以在 Windows 下编译生成，如果觉得手动编译它们很
麻烦，想要现成可用的依赖库和头文件，可以联系作者。

## 贡献

**你可以通过 GitHub 向本项目提交合并请求**

补丁应符合以下要求：

1. 遵循现有代码风格，请参考 `docs/CodingStyle.zh-cn.md` 文件。
2. 一次提交应该做完整的一件事。
3. 提交信息的第一行为摘要行，长度应低于80个字符，如果需要描述修改原因，请在摘要行后留空行，然后再补充。
4. 修复之前的提交（已经合并）中存在的 bug，提交信息的摘要行应该以 `修复` 或含义类似的词语开头，表示修复
   了它。如果你修复的是 issues 列表中未解决的问题，需在摘要行中加上 issues 列表中对应问题的编号。
5. 衍合你的分支至源项目的 master 分支。我们不希望落拉取到冗余的合并请求。
6. **要清楚什么许可证适用于你的补丁：** 该代码库中的文件基于GPLv2（或更高版本），但（原
   作者）我们仍然可以创建非自由的衍生工具。然而，如果给我们的补丁是基于GPL的，我们希望它
   以后不会进入任何非自由的衍生工具，因此，如果补丁发布于公共领域，会为我们带来方便（以及
   避免任何法律问题）。

**你可以通过捐赠来支持 LCUI 的开发**

作者目前是一名独立开发者，你的贡献是非常有帮助的，如果你想通过捐赠来让作者能够花更多的时间来改进该项目，
可以访问作者在 [Patreon](https://www.patreon.com/lcsoft) 上设立的页面。

### GitHub 流程

开发补丁应遵循以下流程：

#### 初始设置

1. 在 GitHub 上进行Fork (点击 Fork 按钮)
2. Clone 到计算机： `git clone git@github.com:你的用户名/LCUI.git`
3. cd 到代码库中： `cd LCUI`
4. 设置远程仓库记录，假设该记录名为 upstream，那么命令为： `git remote add -f upstream git://github.com/lc-soft/LCUI.git`

#### 添加Feature

1. 为新的 feature 创建一个分支(branch)，假设分支名为 my_new_feature，那么命令为：`git checkout -b my_new_feature`
2. 在你的分支上工作, 像往常一样添加和提交修改。

创建一个分支并非必须的，但是，当已经合并至源项目时可以方便删除你的分支，也可以在提交合并
请求前比较你的分支和源项目的最终版本，然后提交合并请求。

#### 推送到GitHub

1. 推送分支至GitHub： `git push origin my_new_feature`
2. 发出合并请求： 在Github上点击 `Pull Request` 按钮

#### 有用的命令

如果源项目发生了很大的变化，最重要的是，你可以重现你的本地修改，这叫做`rebase`，例如：

	git fetch upstream
	git rebase upstream/master

这样会获取更改并重新应用你的代码提交记录。

这一般比合并更好，它会给出一个清晰的视图，以表示哪些提交是你本地代码库的分支中，如果同
样的变更已经在源项目中应用，它也会“修剪”你的本地的提交记录。

你可以将`-i`和`rebase`一起使用，以选择“交互式”衍合，这允许你移除、重排、合并以及修改
提交信息，例如：

	git rebase -i upstream/master


--- end of README.zh-cn.md ---
