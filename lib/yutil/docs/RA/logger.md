# logger

实现日志输出等辅助调试工具。

## 函数概要

```c
 void  Logger_SetLevel()   //设置级别                   
 int   Logger_Log()    //日志输出接口                     
 int   Logger_LogW()    //日志输出接口的宽字符版          
 void  Logger_SetHandler()    //设置日志处理器
 void  Logger_SetHandlerW()    //设置日志处理器的宽字符版
```



## Logger_SetLevel

```c
void Logger_SetLevel(LoggerLevel level);

```

设置日志级别，级别分类如下

```c
typedef enum LoggerLevel {
	LOGGER_LEVEL_ALL,
	LOGGER_LEVEL_DEBUG,
	LOGGER_LEVEL_INFO,
	LOGGER_LEVEL_WARNING,
	LOGGER_LEVEL_ERROR,
	LOGGER_LEVEL_OFF
} LoggerLevel;
```



**参数说明：**

- `level`：日志类型

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无


## Logger_Log

```c
int Logger_Log(LoggerLevel level, const char* fmt, ...);
```

日志输出接口。

**参数说明：**

- `level`：设置日志类型
- `fmt`：日志内容
- `...`： 同上

**返回说明：**

- 执行成功返回日志长度，失败返回0

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/main.c
- include/LCUI_Build.h

## Logger_LogW

```c
int Logger_LogW(LoggerLevel level, const wchar_t* fmt, ...);
```

宽字符版的日志输出接口。

**参数说明：**

- `level`：设置日志类型
- `fmt`：宽字符类型的日志内容字符串
- `...`： 同上

**返回说明：**

- 执行成功返回日志长度，失败返回0

**存在问题：**

1. 命名规范

**改进方案：**

- 根据命名规范修改

**依赖它的文件：**

- src/main.c

## Logger_SetHandler

```c
void Logger_SetHandler(void (*handler)(const char*));
```

设置日志处理器

宽字符版的日志输出接口。

**参数说明：**

- `handler`：目标处理器（只读）

**返回说明：**

- 无

**存在问题：**

1. 命名规范

**改进方案：**

- 根据命名规范修改

**依赖它的文件：**

- src/main.c

## Logger_SetHandlerW

```c
void Logger_SetHandlerW(void (*handler)(const wchar_t*));
```

设置宽字符版的日志处理器。

**参数说明：**

- `handler`：，目标处理器（只读）

**返回说明：**

- 无

**存在问题：**

1. 命名规范

**改进方案：**

- 根据命名规范修改

**依赖它的文件：**

- src/main.c