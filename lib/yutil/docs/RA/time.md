## time

提供一些高精度时间函数。

## 函数概要

| 限定符和类型 | 函数和说明                                                   |
| :----------- | :----------------------------------------------------------- |
| void         | `LCUITime_Init` <br />设置高精度时间可用                     |
| int64_t      | `LCUI_GetTime`<br />返回时间戳（精确到微秒）                 |
| int64_t      | `LCUI_GetTimeDelta`<br />返回时间差                          |
| void         | `LCUI_Sleep`<br />进程挂起一段时间， 单位是秒                |
| void         | `LCUI_MSleep`<br />进程挂起一段时间， 单位是微秒（百万分之一秒） |


## LCUITime_Init

```c
int64_t LCUI_GetTime(void)；
void LCUITime_Init(void)
```

初始化

**参数说明：**

- 无

**返回说明：**

- 用int64_t类型来存储时间，时间精度为毫秒

**存在问题：**

1. 命名

**改进方案：**

- 规范命名

**依赖它的文件：**

- **src/timer.c**

## LCUI_GetTime

```c
int64_t LCUI_GetTime(void)
```

获取当前时间戳

**参数说明：**

- 

**返回说明：**

- 用int64_t类型来存储时间，时间精度为毫秒

**存在问题：**

1. 无



## LCUI_GetTimeDelta

```c
int64_t LCUI_GetTimeDelta(int64_t start)
```

获取起始时间与当前时间的时间差

**参数说明：**

- `start` -起始时间

**返回说明：**

- 用int64_t类型来存储时间，时间精度为毫秒

**存在问题：**

1. 无



## LCUI_Sleep

```c
void LCUI_Sleep(unsigned int s);
```

将进程挂起一段时间， 单位是秒。

**参数说明：**

- `s` -挂起的时间

**返回说明：**

- 

## LCUI_MSleep

```c
void LCUI_MSleep(unsigned int ms);
```

将进程挂起一段时间， 单位是毫秒。

**参数说明：**

- `s` -挂起的时间

**返回说明：**

- 无

**存在问题：**

1. LCUI_Sleep和LCUI_MSleep 功能和实现类似，可以合并

**改进方案：**

- 添加参数，将两个函数合并为一个

**依赖它的文件：**

- 无
