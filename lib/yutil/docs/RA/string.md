# String

扩展额外的字符串操作。

## 函数概要

```c
char *    strdup2()    //strdup() 的再实现版本，将串拷贝到新建的位置，用于清除编译错误
wchar_t * wcsdup2()    //strdup2() 的宽字符版本                       
size_t    strsize()    //计算字符串占用的内存空间大小。               
size_t    wcssize()    //strzie()的宽字符版本                         
size_t    strtolower()    //将字符串中的字母转成小写字母。            
size_t    strntolower()    //将字符串中前面的指定长度的字母转成小写字母。
size_t    strtrim()    //清除字符串首尾的字符                         
size_t    wcstrim()    //strtrim()的宽字符版本。                      
unsigned  strhash()    //返回输入的字符串的哈希值                     
size_t    strreplace()    //字符串替换                                
size_t    wcsreplace()    //宽字符类型字符串替换                      
int       cmdsplit()    //分割命令行字符串                            
int       strsplit()    //分割字符串                                  
```



## strdup2 

```c
char *strdup2(const char *str);

```

`strdup()` 的再实现版本，将串拷贝到新建的位置，用于清除编译错误 

**参数说明：**

- `str`：指向要复制的（以  NULL  结束）字符串的指针

**返回说明：**

- 返回新字符串的指针

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/font/fontlibrary.c
- src/gui/css_fontstyle.c
- src/gui/css_library.c
- src/gui/css_parser.c
- src/gui/widget_attribute.c
- src/gui/widget_background.c
- src/gui/widget_base.c
- src/gui/widget_event.c
- src/gui/widget_helper.c
- src/gui/widget_id.c
- src/gui/widget_prototype.c
- src/gui/widget_style.c
- src/gui/widget/anchor.c
- src/gui/widget/textview.c

## wcsdup2 

```c
wchar_t *wcsdup2(const wchar_t *str);

```

`strdup2()` 的宽字符版本，将串拷贝到新建的位置，用于清除编译错误 

**参数说明：**

- `str`：指向要复制的（以  NULL  结束）宽字符类型字符串的指针

**返回说明：**

- 返回新字符串的指针

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/css_library.c



## strsize

```c
size_t strsize(const char *str);
```

计算字符串占用的内存空间大小。

**参数说明：**

- `str`：指向目标（以  NULL  结束）字符串的指针

**返回说明：**

- 返回占用内存空间的大小

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/font/fontlibrary.c
- src/gui/builder.c
- src/gui/css_rule_font_face.c
- src/gui/widget/anchor.c

## wcssize

```c
size_t wcssize(const wchar_t *str);
```

计算字符串占用的内存空间大小。

**参数说明：**

- `str`：指向（以  NULL  结束）宽字符类型字符串指针

**返回说明：**

- 返回占用内存空间的大小

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/widget/textview.c



## strtolower

```c
size_t strtolower(char *outstr, const char *instr);
```

将字符串中的字母转成小写字母。

**参数说明：**

- `outstr`：目标缓冲，存放转换后的字符串。
- `instr`：指向要转换的（以  NULL  结束）宽字符类型字符串指针

**返回说明：**

- 返回字符串的长度。

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/widget/textview.c

## strntolower

```c
size_t strntolower(char *outstr, size_t max_len, const char *instr);
```

将字符串中前面的指定长度的字母转成小写字母。

**参数说明：**

- `outstr`：目标缓冲，存放转换后的字符串。
- `instr`：指向要转换的（以  NULL  结束）宽字符类型字符串指针

**返回说明：**

- 返回输入字符串的长度。

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## strtrim

```c
size_t strtrim(char *outstr, const char *instr, const char *charlist);
```

清除字符串首尾的字符


**参数说明：**

- `outstr` – 目标缓存，存放处理后的字符串
- `instr` – 需要处理的字符串
- `charlist` – 需要清除的字符列表，当为  NULL  时，默认清除空白符

**返回说明：**

- 返回处理后的字符串的长度

**改进方案：**

- 无

**依赖它的文件：**

- src/font/fontlibrary.c
- src/gui/css_rule_font_face.c
- src/util/parse.c

## wcstrim

```c
size_t wcstrim(wchar_t *outstr, const wchar_t *instr,

              const wchar_t *charlist);



unsigned strhash(unsigned hash, const char *str);


```

清除宽字符类型字符串首尾的字符。

**参数说明：**

- `outstr` – 目标缓存，存放处理后的字符串
- `instr` – 需要处理的字符串
- `charlist` – 需要清除的字符列表，当为 NULL 时，默认清除空白符

**返回说明：**

- 返回处理后的字符串的长度

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/widget/textview.c

## strhash

```c
unsigned strhash(unsigned hash, const char *str);
```

返回输入的字符串的哈希值

**参数说明：**

- `hash` – 无符号型的哈希值
- `instr` – 目标字符串

**返回说明：**

- 返回哈希值

**存在问题：**

1. 可以使用更好的hash生成函数

**改进方案：**

- 调研其他hash生成函数，选择一种较好的方式实现。

**依赖它的文件：**

- src/gui/widget_hash.c
- src/util/object.c

## strreplace

```c
size_t strreplace(char *str, size_t max_len, const char *substr, const char *newstr);
```

字符串替换


**参数说明：**

- `str` – 需要处理的字符串，替换成功后字符串内容也会被修改
- `max_len` – 输出字符串的最大长度
- `substr` – 字符串中需要被替换的子字符串
- `newstr` – 替换的新字符串

**返回说明：**

- 返回替换后的字符串长度，若未替换，则返回0

**改进方案：**

- 无

**依赖它的文件：**

- 无

## wcsreplace

```c
size_t strreplace(char *str, size_t max_len, const char *substr, const char *newstr);
```

宽字符类型字符串替换

**参数说明：**

- `str` – 需要处理的字符串，替换成功后字符串内容也会被修改
- `max_len` – 输出字符串的最大长度
- `substr` – 字符串中需要被替换的子字符串
- `newstr` – 替换的新字符串

**返回说明：**

- 返回替换后的字符串长度，若未替换，则返回0

**改进方案：**

- 无

**依赖它的文件：**

- 无

## cmdsplit

```c
int cmdsplit(const char *cmd, char ***outargv);
```

分割命令行字符串

**参数说明：**

- `cmd` – 需分割的命令行字符串
- `outargv` – 分割后的命令行参数列表

**返回说明：**

- 返回参数数量

**改进方案：**

- 无

**依赖它的文件：**

- build/windows/LCUIMain/win32_main.c

## strsplit

```c
int strsplit(const char *instr, const char *sep, char ***outstrs);
```

分割字符串

**参数说明：**

- `instr` – 需分割的字符串
  `sep` – 分割标记字符串
  `outstrs` – 分割后的字符串列表

**返回说明：**

- 返回替换后的字符串长度

**改进方案：**

- 无

**依赖它的文件：**

- 无