# 构建工具

- 开始日期：2023-03-28
- 目标主要版本：3.x
- 参考问题：无
- 实现 PR：无

## 概括

改用 xmake 来构建 LCUI，降低构建和使用成本。

## 基本示例

构建：

```sh
xmake
```

打包：

```sh
xmake package
```

安装到指定目录：

```sh
xmake install -o path/to/your/dir
```

生成 CMakelists.txt：

```sh
xmake project -k cmakelists
```

生成 compile_commands：

```sh
xmake project -k compile_commands
```

子库都有自己的构建配置，可单独构建它们：

```sh
xmake build pandagl
xmake build yutil
xmake build libcss
xmake build libui
xmake build libplatform
```

在 xmake.lua 中引入 LCUI 和子库：

```lua
add_repositories("lcui-repo git@github.com:lcui-dev/xmake-repo.git")
add_requires("lcui", "pandagl", "libcss")

target("lcuiapp")
    add_packages("lcui")

target("imageeditor")
    add_packages("pandagl")

target("cssparser")
    add_packages("libcss")
```

## 动机

2.x 版本采用的构建工具是 AutoMake 和 VisualStudio，分别用于 Linux、Windows 平台上的构建，而包管理工具是 lcpkg，依赖 Node.js 运行环境和 vcpkg 工具。

这些工具不仅增加了编译环境的搭建成本，还增加了项目维护人员的维护成本。因此，应该改用一款跨平台且自带包管理功能的构建工具代替它们。

## 详细设计

在源码根目录下添加 xmake.lua，然后参考 [XMake 文档](https://xmake.io/)完善配置。

## 缺点

需要考虑调整 debian 包的打包流程。

## 备选方案

CMake。不建议采用，无论是中文文档、包管理、配置文件编写体验都不如 XMake，遇到一些使用上的问题还需要花时间搜索资料和验证。
