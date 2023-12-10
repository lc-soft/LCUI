# 架构

- 开始日期：2023-03-26
- 目标主要版本：3.x
- 参考问题：无
- 实现 PR：无

## 概要

重新设计源码目录结构，将 LCUI 拆分为多个子库，重构部分模块以减少不必要的耦合。

## 基本示例

新的目录结构：

```text
docs/
examples/
include/
lib/
    css/
    pandagl/
    platform/
    thread/
    timer/
    ui/
    ui-cursor/
    ui-server/
    ui-widgets/
    ui-xml/
    worker/
    yutil/
src/
tests/
```

更精简的头文件路径：

```c
// 之前
#include <LCUI/util.h>
#include <LCUI/display.h>
#include <LCUI/graph.h>
#include <LCUI/widget.h>
#include <LCUI/css.h>

// 之后
#include <yutil.h>
#include <platform.h>
#include <pandagl.h>
#include <ui.h>
#include <css.h>
```

## 动机

图形处理、系统窗口管理、输入输出管理等功能已经有更好的开源实现，继续维护和改进它们只是在浪费时间，而且 LCUI 项目维护人员的兴趣并不在底层功能模块上面。因此，LCUI 应该被拆分成多个较为独立、通用的子库，以便于他人根据自身的需要，对 LCUI 进行定制、裁剪，又或是只使用部分子库。典型的适用场景包括：

- 单独构建 PandaGL 图形库，在应用程序中调用它的接口实现简单的图像编辑和文字渲染。
- 基于 SDL 实现一系列与平台库 libplatform 相同的接口，使 LCUI 应用程序能够将 SDL 作为后端来为图形界面的各项能力提供更好的支持。
- 在其它 UI 库中使用 LCUI 的自绘组件。

简而言之，这是为了扩大 LCUI 的受众范围，使其从面向单一 UI 开发需求的 UI 库项目，转变为面向多种需求的项目集。

## 详细设计

新增 lib 目录用于存放子库，子库的目录命名和结构如下：

```
[libname]/
    include/
        [libname]/
            config.h
            foo.h
            bar.h
        [libname].h
    src/
        config.h.in
        foo.c
        foo.h
        bar.c
        bar.h
    xmake.lua
```

源文件的私有接口声明在同目录同名头文件中，而公开接口则声明在 `include/[libname]` 目录中的同名头文件内。

`src` 目录中存放这些子库共同运作所需的代码，包括初始化应用程序资源、字体、主循环、窗口管理等。

为明确依赖关系，子库的依赖库都由它的 xmake.lua 配置引入。

## 缺点

无。

## 备选方案

无。

## 采用策略

这是个破坏性改动，从 2.x 版本迁移到 3.x 版本需要更新头文件包含代码。
