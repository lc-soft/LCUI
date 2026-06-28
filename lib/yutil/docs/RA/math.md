## 数学运算 math

提供一些额外的数学运算操作。

## 函数概要

```c
/** round double to interger */
#define iround(X) (X < 0 ? (int)(X - 0.5) : (int)(X + 0.5))  //返回传入参数最接近的整数 
#define max(a,b) (((a) > (b)) ? (a) : (b)) //返回给定参数表中的最小值     
#define max(a,b) (((a) > (b)) ? (a) : (b))  //返回给定参数表中的最大值     
```



## iround

```c
/** round double to interger */
#define iround(X) (X < 0 ? (int)(X - 0.5) : (int)(X + 0.5))

```

取整函数

**参数说明：**

- X：小数类型

**返回说明：**

- 返回离X最接近的整数。

**存在问题：**

1. 命名风格

**改进方案：**

- 根据命名规范修改

**依赖它的文件：**

- **src/cursor.c**
- **src/display.c**
- **src/draw/background.c**
- **src/font/textlayer.c**
- **src/font/textstyle.c**
- **src/gui/css_fontstyle.c**
- **src/gui/metrics.c**
- **src/gui/widget_event.c**
- **src/gui/widget_paint.c**
- **src/gui/widget/scrollbar.c**
- **src/gui/widget/textedit.c**
- **src/platform/windows/uwp_input.cpp**
- **src/util/parse.c**
- **src/util/rect.c**


## max

```c
#define max(a,b) (((a) > (b)) ? (a) : (b))
```

比较返回最大值。

**参数说明：**

- a：比较数1
- b：比较数2

**返回说明：**

- a > b，返回 a；
- 否则返回 b

**存在问题：**

1. 无

## min

```c
#define min(a,b) (((a) < (b)) ? (a) : (b))
```

比较返回最小值。

- **参数说明：**

  - a：比较数1
  - b：比较数2

  **返回说明：**

  - a < b，返回 a；
  - 否则返回 b

**存在问题：**

1. 无
