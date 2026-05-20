## 代码格式

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

在 tests/cases 中新建测试代码，源文件名称以 `test_` 开头，内部添加头文件包含：

```c
#include <ctest-custom.h>
``

然后使用 lib/ctest/include/ctest.h 中的函数输出测试信息、检测测试结果，例如:


```c
#include <ctest-custom.h>

int add(int a, int b)
{
    return a + b;
}

void test_my_case(void)
{
    ctest_equat_init("check add(1, 1)", 2, add(1, 1));
}
```

最后，在 tests/run_tests.c 中添加这个测试的调用代码。

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
