## 字符编码 charset

支持 UTF-8、ANSI 与 Unicode 间的字符编码转换。

## 函数概要

```c
 size_t        LCUI_DecodeString //多字节字符编码转 Unicode  
 size_t        LCUI_EncodeString //Unicode 转多字节字符编码 
```


## LCUI_DecodeString

```c
size_t LCUI_DecodeString(wchar_t *wstr, const char *str, size_t max_len,
			 int encoding)
```

用于提供 将多字节字符编码转换宽字符( Unicode 编码类型)。 

提供 UTF-8 转 Unicode ，以及 本地字符编码 转 Unicode。

**参数说明：**

- wstr：目标缓存，用来存放转换之后的宽字节字符；当取值为NULL时，返回值为目标缓存所需 wchar_t 类型的个数
- str： 用来存放待转换的多字节字符串
- max_len：用来指定最大转换长度
- encoding：(对于 Windows 平台)通过枚举值 ENCODING_ANSI / ENCODING_UTF8 决定编码格式

**返回说明：**

- 转换成功，且 wcstr == NULL 时，返回目标缓存所需的大小（ wchar_t  类型个数，但不含终结符）；
- 转换成功，且 wcstr  != NULL 时，返回转换的字节数；
- 转换不成功，返回(size_t) (0)；

**存在问题：**

1. 重复：目前的使用场景与标准库提供的 mbstowcs() 重复

**改进方案：**

- 调研字符编码转换相关资料，验证标准库提供的函数是否满足现有使用需求，然后决定是否提供字符编码转换功能。

**依赖它的文件：**

- **LCUI/src/gui/widget/textedit.c** 
  - `TextEdit_SetText(LCUI_Widget widget, const char *utf8_str)`
  - `TextEdit_SetPlaceHolder(LCUI_Widget w, const char *str)`
- **src/gui/widget/textview.c** 
  - `TextView_SetText(LCUI_Widget w, const char *utf8_text)`
- **src/gui/css_fontstyle.c**
  - `OnComputeContent(LCUI_CSSFontStyle fs, LCUI_Style s)`

## LCUI_EncodeString

```c
size_t LCUI_EncodeString(char *str, const wchar_t *wstr, size_t max_len,
			 int encoding)
```

用于提供 宽字符( Unicode 编码类型)转换成多字节字符编码。 

提供 Unicode 转 UTF-8， 本地字符编码。

**参数说明：**

- str： 目标缓存，用来存放转换之后的多字节字符串
- wstr：用来存放待转换的宽字节字符
- max_len：用来指定最大转换长度
- encoding：(对于 Windows 平台)通过枚举值 ENCODING_ANSI / ENCODING_UTF8 决定编码格式

**返回说明：**

- 转换成功，且 str== NULL 时，返回目标缓存所需的大小（ char  类型个数，但不含终结符）；
- 转换成功，且 str!= NULL 时，返回转换的字节数；
- 转换不成功，返回(size_t) (0)；

**存在问题：**

1. 重复：目前的使用场景与标准库提供的 wcstombs() 重复；

**改进方案：**

- 调研字符编码转换相关资料，验证标准库提供的函数是否满足现有使用需求，然后决定是否提供字符编码转换功能。

**依赖它的文件：**

- **src/platform/linux/linux_x11display.c** 
  - `X11Surface_SetCaptionW(LCUI_Surface surface, const wchar_t *wstr)`
- **src/util/dirent.c** 
  - `LCUI_OpenDirW(const wchar_t *path, LCUI_Dir *dir)`
- **src/util/object.c**
  - `WString_ToString(LCUI_Object str, LCUI_Object newstr)`

