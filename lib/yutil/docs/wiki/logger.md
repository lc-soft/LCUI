# logger

实现日志输出等辅助调试工具。

## 函数概要

```c
 void  logger_set_level()   //设置级别                   
 int   logger_log()    //日志输出接口                     
 int   logger_log_w()    //日志输出接口的宽字符版          
 void  logger_set_handler()    //设置日志处理器
 void  logger_set_handler_w()    //设置日志处理器的宽字符版
```


下面看下例子：
```c
#ifdef _WIN32
#include <windows.h>
static void win32_logger_log(const char *str)
{
	OutputDebugStringA(str);
}

static void win32_logger_log_w(const wchar_t *wcs)
{
	OutputDebugStringW(wcs);
}

#endif

static void test_logger_set_level(void)
{
	logger_set_level(LOGGER_LEVEL_OFF);
	logger_set_level(LOGGER_LEVEL_ERROR);
	logger_set_level(LOGGER_LEVEL_WARNING);
	logger_set_level(LOGGER_LEVEL_INFO);
	logger_set_level(LOGGER_LEVEL_DEBUG);
	logger_set_level(LOGGER_LEVEL_ALL);
}

static void test_logger_log(void)
{
	int len = 0;
	len = logger_log(LOGGER_LEVEL_ALL, "test\n");
	// logger_info("test i\n");
	// logger_debug("test d\n");
	// logger_warning("test w\n");
	// logger_error("test e\n");
	it_b("logger_log() should work", len == 5, TRUE);
}
static void test_logger_log_w(void)
{
	int len = 0;
	len = logger_log_w(LOGGER_LEVEL_ALL, L"test\n");
	logger_info_w(L"test i\n");
	logger_debug_w(L"test d\n");
	logger_warning_w(L"test w\n");
	logger_error_w(L"test e\n");
}

void test_logger(void)
{
	test_logger_set_level();
	test_logger_log();
#ifdef _WIN32
	logger_set_handler(win32_logger_log);
	logger_set_handler_w(win32_logger_log_w);
#endif
	test_logger_log_w();
}
```

## logger_set_level

```c
void logger_set_level(logger_level_e level);

```

设置日志级别，级别分类如下

```c
enum logger_level_e {
	LOGGER_LEVEL_ALL,
	LOGGER_LEVEL_DEBUG,
	LOGGER_LEVEL_INFO,
	LOGGER_LEVEL_WARNING,
	LOGGER_LEVEL_ERROR,
	LOGGER_LEVEL_OFF
};
```
## logger_log

```c
int logger_log(logger_level_e level, const char* fmt, ...);
```

日志输出接口。

**参数说明：**

- `level`：设置日志类型
- `fmt`：日志内容
- `...`： 同上

**返回说明：**

- 执行成功返回日志长度，失败返回0

## logger_log_w

```c
int logger_log_w(logger_level_e level, const wchar_t* fmt, ...);
```

宽字符版的日志输出接口。

**参数说明：**

- `level`：设置日志类型
- `fmt`：宽字符类型的日志内容字符串
- `...`： 同上

**返回说明：**

- 执行成功返回日志长度，失败返回0

```c
void logger_set_handler(void (*handler)(const char*));
```

设置日志处理器

宽字符版的日志输出接口。

**参数说明：**

- `handler`：目标处理器（只读）

## logger_set_handler_w

```c
void logger_set_handler_w(void (*handler)(const wchar_t*));
```

设置宽字符版的日志处理器。

**参数说明：**

- `handler`：，目标处理器（只读）

**返回说明：**

- 无

