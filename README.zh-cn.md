# The LCUI Project

[![Gitter](https://badges.gitter.im/lc-soft/LCUI.svg)](https://gitter.im/lc-soft/LCUI?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)
[![Build Status](https://travis-ci.org/lc-soft/LCUI.png?branch=master)](https://travis-ci.org/lc-soft/LCUI)

![LCUI Logo](http://lcui.org/static/images/lcui-project-logo.png)

## 说明

LCUI 是一个简单的跨平台图形界面库，适用于开发一些简单的小程序，其诞生目的主要是方便作者开发GUI程序。

LCUI 是一个自由软件项目，基于 GNU通用公共许可协议 发布，该许可协议要求 LCUI 及 LCUI 衍生软件无论以何种形式发布，都必须确保收受者能收到或得到源码。

此项目主要由作者利用闲余时间进行开发和维护，项目的活跃度通常会受到各种因素的影响，大多数时间内都处于低活跃度状态。

请阅读 `docs/CHANGES.zh-cn.md` 文件了解新版本的更新内容。

阅读 `docs/FAQ.zh-cn.md` 文件可以找到常见问题的解决方法。

阅读 `docs/LICENSE.TXT`  文件了解 GPLv2 许可协议的具体内容。

项目主页：http://www.lcui.org

联系邮箱：lc-soft@live.cn

项目托管：https://github.com/lc-soft/LCUI

LCUI 的相关信息与资料可以在项目主页中找到，如果遇到问题、发现BUG或者有好的建议和
想法，可以在项目主页上留言或发送邮件告诉我们。

我们已经使用 GitHub 托管 LCUI 项目，网址在上面，欢迎您参与 LCUI 项目的优化和改进。

## 文件目录结构

以下是各个目录内容的相关说明：
```

/                项目文件目录
/bin/            在编译生成后库文件的存放目录
/build/          参与项目编译、构建和生成的相关文件
/build/VS2010/   VisualStudio 2010 工程文件
/build/VS2012/   VisualStudio 2012 工程文件
/docs/           相关文档
/include/        头文件
/src/            源代码
/src/gui/        图形界面
/src/gui/widget/ 预置的图形界面组件
/src/font/       文字和字体
/src/draw/       图形绘制
/src/bmp/        位图文件读写
/src/input/      输入处理，例如：响应键盘和鼠标的输入
/src/output/     输出处理，例如：输出图形至屏幕
/src/thread/     各平台下的线程操作接口的封装
/src/misc/       其它未分类的模块
/test/           测试（示例）程序

```

## 文档

你可以访问LCUI的项目主页浏览开发文档，开发文档也托管在GitHub上，地址：https://github.com/lc-soft/LCUI-Documentation

如果你通过LCUI的源代码和示例程序领悟到了一些些有趣的东西，可以帮助我们完善开发文档，与其他人分享你的成果。

## 构建

你需要运行 `./configure` (在源码根目录中) 以引导项目的构建。

在最简单的情况下，你可以运行：

	git clone https://github.com/lc-soft/LCUI.git
	cd LCUI
	git submodule update --init
	./configure

如果未找到 ./configure，请运行 ./autogen.sh 脚本生成它。

在 LCUI 的项目主页上发布的源码包，一般是比较稳定的版本，你只需要这样做：

1. 解压LCUI的源码包。
2. 进入解压后得到的新文件夹。
3. 运行源码根目录中的configure脚本，以对系统环境进行检测。
4. 使用make命令编译 LCUI 的源代码。
5. 等待编译成功完成，如果在编译过程中遇到错误，请检查您的系统中是否有完整的编译环境。
6. 使用make install命令安装 LCUI 的函数库和头文件。

windows系统上构建本项目方法，有两种：

1. 使用 Visual Studio 2010 或更高版本，你只需要使用它打开 `build/VisualStudio/` 目录下的
 `LCUI.sln` 文件，然后生成解决方案即可，生成成功后，在 `bin/` 目录下可找到库文件。
2. 使用Cygwin或MSYS，可按照上述1至6的步骤进行构建，在完成步骤3后，请修改include/LCUI/
目录下的 `config.h` 文件，定义 `LCUI_BUILD_IN_WIN32` 宏，若有 `LCUI_BUILD_IN_LINUX` 宏，
请删除它。该方法并未实际测试，如有其它方法请告知。

### 系统需求

* 字体处理： freetype2
* 触屏支持： tslib
* 线程支持： pthread
* 图片处理： libpng 和 libjpeg

为了你能正常使用LCUI，请先确保您的计算机已经安装了上述的函数库，并保持较新的版本。
理论上，LCUI 能在 GNU/Linux 系统环境中正常工作，LCUI 的图形输出，需要内核支持 FrameBuffer。

## 贡献

你可以通过GitHub向本项目提交合并请求，补丁应符合以下要求：

1. 遵循现有代码风格。
2. 一次提交应该做完整的一件事。
3. 提交信息的第一行为摘要行，长度应低于80个字符，如果需要描述修改原因，请在摘要行后留空行，然后再补充。
4. 修复之前的提交（已经合并）中存在的bug，提交信息的摘要行应该以`fixup!`开头，
   表示修复了它。如果你修复的是issues列表中未解决的bug，提交信息的摘要行应该以
   `fix->`开头，并加上issues列表中对应bug信息的标题。
5. 衍合你的分支至源项目的master分支。我们不希望抓取到冗余的合并请求。
6. **要清楚什么许可证适用于你的补丁：** 该代码库中的文件基于GPLv2（或更高版本），但（原
   作者）我们仍然可以创建非自由的衍生工具。然而，如果给我们的补丁是基于GPL的，我们希望它
   以后不会进入任何非自由的衍生工具，因此，如果补丁发布于公共领域，会为我们带来方便（以及
   避免任何法律问题）。


### GitHub流程

开发补丁应遵循以下流程：

#### 初始设置

1. 在GitHub上进行Fork (点击 Fork 按钮)
2. Clone到计算机： `git clone git@github.com:你的用户名/LCUI.git`
3. cd 到代码库中： `cd LCUI`
4. 设置远程记录，假设该记录名为upstream，那么命令为： `git remote add -f upstream git://github.com/lc-soft/LCUI.git`

#### 添加Feature

1. 为新的 feature 创建一个分支(branch)，假设分支名为 my_new_feature，那么命令为：`git checkout -b my_new_feature`
2. 在你的分支上工作, 像往常一样添加和提交修改。

创建一个分支并非必须的，但是，当已经合并至源项目时可以方便删除你的分支，也可以在提交合并
请求前比较你的分支和源项目的最终版本，然后提交合并请求。

#### 推送到GitHub

1. 推送分支至GitHub： `git push origin my_new_feature`
2. 发出合并请求： 在Github上点击 Pull Request 按钮

#### 有用的命令

如果源项目发生了很大的变化，最重要的是，你可以重现你的本地修改，这叫做`rebase`，例如：

	git fetch upstream
	git rebase upstream/master

这样会获取更改并重新应用你的commits。

这一般比合并更好，它会给出一个清晰的视图，以表示哪些提交是你本地代码库的分支中，如果同
样的变更已经在源项目中应用，它也会“修剪”你的本地的提交记录。

你可以将`-i`和`rebase`一起使用，以选择“交互式”衍合，这允许你移除、重排、合并以及修改
提交信息，例如：

	git rebase -i upstream/master


--- end of README.zh-cn.md ---
