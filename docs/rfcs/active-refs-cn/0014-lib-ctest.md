# 测试库

- 开始日期：2023-06-03
- 目标主要版本：3.x
- 参考问题：无
- 实现 PR：无

## 概括

一个简单的测试库，用于统计测试用例执行结果和耗时，以友好的格式输出测试结果。

## 基本示例

```c
#include <ctest.h>

void test_stdio(void)
{
    int num = 0;

    ctest_equal_int("the string \"100\" should be converted to 100",
                    sscanf("100", "%d", &num), 100);
}

int main(void)
{
    ctest_describe("stdio.h", test_stdio);
    return ctest_finish();
}
```

运行后输出：

```terminal
  stdio.h
    ✓ the string "100" should be converted to 100

  1 passing (0ms)
```

## 动机

- **命名规范化：** `describe()`、`print_test_result()`、`it_i()` 命名比较随意，不太容易看出它们同属一个库。
- **减少依赖：** ctest 的 `it_rectf()`、`it_rect()` 等函数依赖图形库和 UI 库的数据类型，其它用到 ctest 的库会因此依赖这些库。
- **适应更多的数据类型：** 内置的 `it_s()`、`it_b()`、`it_i()` 不够用，而且它们的大部分实现代码是一样的，应该优化。

## 详细设计

### 调整命名

命名都以 `ctest_` 开头。

`it_`开头的测试函数改为以 `ctest_equal_` 开头，并将 s、i、b 改为更完整的单词：str、int、bool。


### 提升扩展性

新增 `ctest_equal()` 函数，接收字符串转换函数 `to_str` 和自定义数据的指针。

```c
bool ctest_equal(const char *name, int (*to_str)(void *, char *, unsigned),
		 void *actual, void *expected);
```

原有的 `it_s()`、`it_int()` 等函数改为基于 `ctest_equal()` 实现，例如：

```c
int ctest_int_to_str(void *data, char *str, unsigned max_len)
{
	return snprintf(str, max_len, "%d", *(int *)data);
}

bool ctest_euqal_int(const char *name, int actual, int expected)
{
	return ctest_equal(name, ctest_int_to_str, &actual, &expected);
}

```

### 减少依赖

内部移除 `it_rect()`、`it_rectf()`，由 UI 库和图形库内部提供这些数据类型值的测试函数，例如：

```c
int ui_rect_to_str(ui_rect_t *rect, char *str, unsigned max_len)
{
        return snprintf(str, max_len, "(%g, %g, %g, %g)", rect->x, rect->y,
                        rect->width, rect->height);
}

static inline bool ctest_euqal_ui_rect(const char *name, ui_rect_t *actual,
				       ui_rect_t *expected)
{
	return ctest_equal(name, (ctest_to_str_func_t)ui_rect_to_str, actual, expected);
}
```

## 缺点

无。

## 备选方案

无。

## 采用策略

全局替换。
