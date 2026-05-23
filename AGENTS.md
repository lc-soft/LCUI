## 代码格式

遵循 .clang-format 文件中定义的规则，修改文件后需格式化。

### 缩进

使用八个空格缩进。

### 代码结构

源文件中的代码结构应该为：

1. 预处理指令
2. 类型
3. 全局变量
4. 函数声明
5. 函数定义

其中“函数定义”应该按照功能类别分组、按依赖关系从基础到高级的顺序排列，例如：

```c
static int var1;
static int var1;

static void func1();
static void func2();

// object 1

void object1_func1()
{
  // ..
}

void object1_func2() {
    // ...
     object1_func1();
}


// object 2

void object2_func1()
{
  // ..
}

void object2_func2() {
    // ...
     object2_func1();
}
```

注意！优先通过调整函数定义代码块的顺序来解决声明问题，而不是前置声明函数。

## 测试用例

### 归属规则

按下面优先级判定一个测试归属：

1. case 实际 `#include` 的库头文件集合（含传递依赖）。
2. 调用的运行时入口（`lcui_init` / `ui_init` / `pd_*_init` 等）。
3. xmake target 上需要的 `add_deps`。

仅触达单 lib（外加 yutil/ctest/标准库）的测试放 `lib/<name>/tests/`；触达两个及以上同级 lib，或依赖 `src/widgets/` 注册的 widget 类型的测试放 `tests/integration/`。

### 文件与函数命名

- 文件名：`test_<topic>.c`
- 套件入口（lib 内）：`void test_<lib>_<topic>(void)`，如 `test_ui_xml_parser`、`test_pandagl_image_reader`
- 套件入口（顶层集成）：`void test_<topic>(void)`，无前缀，如 `test_settings`
- 套件入口必须在两个位置都注册：所属 lib 的 `tests/main.c` 与顶层 `tests/main.c` 中的 `suites[]` 表
- 内部分组用 `static void <动词>_<group>(void)`，由 `ctest_describe` 注册

### 描述文本风格

- `ctest_describe(name, fn)` 的 `name` 是名词性主题，全小写空格分词，无 `test` 前缀。例：`"widget opacity"`、`"flex layout"`、`"settings.fps_cap"`
- `ctest_equal_*(name, ...)` 的 `name` 用 `should ...` 行为陈述。例：`"should default fps_cap to 120"`、`"should match parent border color"`
- 当上下文清晰（例如 layout case 中描述某 selector 对应的 box）时，可保留 jQuery 选择器风格的描述，无需强行加 should

### 资源文件

- 跨 lib 共享的 fixture 放 `tests/fixtures/`
- 仅本 lib 用的 fixture 也建议复制到 `tests/fixtures/`（顶层 lcui-tests 与单 lib binary 共用同一 rundir）
- xmake target 的 `set_rundir` 指向 `tests/fixtures/`
- 测试代码加载资源时直接用文件名，不带目录前缀

### 三种文件职责

- `tests/integration/test_<topic>.c`：跨 lib 集成测试，自动断言。不调用 `lcui_main`，必要时由 `tests/previews/preview_<topic>.c` 提供可视诊断
- `tests/scenes/<topic>_scene.{c,h}`：可视化场景搭建模块，签名 `void <topic>_scene_build(...)`。只构造 widget 树和应用样式，不做断言、不调用 `ctest_*`、不调用 `lcui_main`/`lcui_quit`。给 cases 与未来的 examples demo 共用
- `lib/<name>/tests/test_<topic>.c`：纯 lib 测试，仅断言

### 编写示例

```c
#include <ctest-custom.h>

void test_my_case(void)
{
        ctest_equal_int("should add two numbers", 1 + 1, 2);
}
```

注册：

```c
/* lib/<name>/tests/main.c 或 tests/main.c */
extern void test_my_case(void);

static const ctest_suite_t suites[] = {
        { "my case", test_my_case },
        { NULL, NULL }
};

CTEST_MAIN(suites)
```

### 运行

- 全量：`xmake test`
- 按 pattern 过滤：`xmake test "*/widget*"`（匹配 target/test 名）
- 按 group 过滤：`xmake test -g tests`
- 单 binary 跑全部 suite：`xmake run <lib>-tests`，例如 `xmake run lcui-tests`
- 单 binary 内细粒度过滤：`xmake run lcui-tests --grep="<pattern>"`，子串匹配 suite 名
- 单 binary 列出 suite：`xmake run lcui-tests --list`
- 内存检查：`xmake f --memcheck=y && xmake test`，调用 drmemory（Windows）或 valgrind（Linux）；恢复正常运行：`xmake f --memcheck=n`

### 不要触碰

- `lib/yutil/tests/`：使用旧 libtest 框架自管理，不并入 ctest 体系，不被 lcui-tests 收集

## 重构约定

### 合并重复分支

当存在两个分支仅输入不同但后续处理相同（如选择 obs->root 或 ui_root），用局部变量合并公共逻辑，避免重复代码。

```c
/* before */
if (ctx->logger) {
    write_log(ctx->logger, msg);
} else {
    logger_t *logger = get_default_logger();
    write_log(logger, msg);
}

/* after */
logger_t *logger = ctx->logger ? ctx->logger : get_default_logger();
write_log(logger, msg);
```

### 提取公共逻辑

若公共逻辑较长，可提取为独立函数，并保持调用路径一致，减少分叉实现。

## 指令

### gen-commit

用于生成符合 Angular 规范的提交信息，scope 应为 lib 目录下的任意目录名（例如：ui、css），标题长度限制在 80 字符以内，应结合本次会话内容生成，无需读取实际改动文件内容。
