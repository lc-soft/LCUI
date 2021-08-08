# 设计文档

LCUI 2.0 存在以下问题：

- 同时存在多种 API 设计风格。
- 头文件依赖关系复杂。
- 有些模块的耦合度较高。
- 编译有点慢。

我们计划在 3.0 版本中解决它们，主要方法是将 LCUI 项目中的大部分代码拆分为多个基础库，使得 LCUI 具备以下特性：

- **统一的 API 设计风格。** 免受混乱的 API 折磨，有助于提升开发体验和效率。
- **更快的编译速度。** 头文件变得更简单，这使得编译速度有所提升。
- **更好的模块化。** 对于用户，他们可以根据自己的需要对 LCUI 中的基础库进行修改和定制，甚至将之替换成更好的同类开源库。对于我们，则可以将一些基础库交给其他贡献者开发和维护。

## 参考资料

设计参考了以下开源项目：

- libgit2: https://libgit2.org/libgit2/
- LevelDB: https://github.com/google/leveldb/blob/master/include/leveldb/c.h

## 基础库

- **libutil:** 实用工具库，提供常用数据结构定义和操作函数、以及一些工具函数。
- **libpaint:** 图形绘制库。
- **libimage:** 图片处理库，提供常见格式图片文件读写功能。
- **libcss:** CSS 解析库，提供 CSS 解析和查询功能。
- **libfont:** 字体库，提供字体文件加载和渲染。
- **liblayout:** 布局引擎库，为图形界面布局提供支持。
- **libtext:** 文本排版库，提供文本排版和渲染功能。
- **libui:** 图形界面库。
- **libbuilder:** 界面构建库，支持实用 xml 代码描述界面内容。
- **libworker:** 工作线程库。

## 架构

（待完善）

```text
|------------------------------------------|
|                Application               |
|------------------------------------------|
| libui | libbuilder                       |
| -----------------------------------------|
| libtext  | libimage |           |        |
| --------------------| liblayout | libcss |
| libpaint | libfont  |           |        |
| -----------------------------------------|
|                  libutil                 |
|------------------------------------------|
```

## 基础库的设计

### 特性

- **不依赖构建工具：** 采用 xmake 作为默认的构建工具，并尽可能的简化构建配置和源码目录结构，使得其他人可以轻松改用自己喜欢的构建工具进行编译，甚至可以直接使用 `gcc -fpic -shared src/*.c -o libexample.so` 这种简单的命令进行编译。
- **易于安装：** 除了用 xmake 方式安装该库外，你也可以直接下载项目然后复制它的源码到你的项目目录中即可使用。

### 目录结构

```text
include/
    example/
        file1.h
        file2.h
        ...
    example.h
src/
    file1.c
    file2.c
    ...
test/
    test.c
    test_???.c
    test_???.c
    ...
example.h
example.c
LICENSE
README.md
```

### API 设计

**全局命名风格：**

采用小写+下划线命名风格，不使用库名作为接口的前缀名。

对于 LCUI 及其基础库而言，它们提供的大部分公共 API 是 UI 相关的，通常情况下一个应用程序不会用到两个 UI 库，很少会有命名冲突的情况，所以我们无需为了避免与其它库的 API 冲突而将库的名称作为公共 API 的前缀名。

**数据类型：**

数据类型名称以 `_t` 结尾，示例：

```c
struct foo_t {
    int bar;
};
```

如果是结构体类型，则应该使用 `typedef` 为该类型定义别名：

```c
typedef struct foo_t foo_t;
```

不使用 `typedef` 为数据类型的指针定义别名，例如以下写法是不推荐的。

```c
typedef struct foo* foo_t;
```

我们推荐将复杂数据结构定义在源文件中，在头文件中只公开由 `typedef` 定义的别名，操作接口只接受该数据结构的指针，示例：

```c
// example.h

typedef struct foo_t foo_t;

foo_t *foo_create(int bar);

void foo_destroy(foo_t *foo);

// example.c

#include "example.h"

struct foo_t {
    int bar;
};
```

**函数命名：**

推荐采用面向对象命名风格，以对象类型名称为前缀，后面接动词或动宾词组，相当于将 `object.doSomething()` 写成 `object_do_something()`。
函数命名中所使用的动词必须是常见的单词，例如：

- get/set
- add/remove
- new/free
- create/destroy
- init/shutdown
- open/close
- start/stop
- begin/end

示例：

```c
const char *value;
foo_t *foo;

foo_create(&foo);
foo_set_property(foo, "key", "value");
foo_get_property(foo, "key", &value);
foo_destroy(foo);
```

## 目录结构

```text
include/
    LCUI/
        util.h
        thread.h
        platform.h
        canvas.h
        image.h
        css.h
        font.h
        layout.h
        ui.h
        ...
    LCUI.h
lib/
    util/
    thread/
    canvas/
    image/
    css/
    font/
    layout/
    ...
src/
    main.c
    ...
```

**编码风格：**

沿用现在的编码风格。

**API 设计风格：**

与基础库的 API 设计风格相同，为了减少 LCUI 应用程序升级到 LCUI 3.0 时的改动量，我们会提供驼峰命名风格的 API。
