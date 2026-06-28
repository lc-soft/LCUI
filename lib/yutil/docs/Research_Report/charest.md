# mbstowcs() & wcstombs()

## mbstowcs()

**通过该函数可以将多字节字符转换成宽字符**

```c
size_t mbstowcs( wchar_t* dst, const char* src, std::size_t len);
```

把参数 **str** 所指向的多字节字符的字符串转换为参数 `wchar_t* pwcs `所指向的数组。

**参数说明**

- **pwds** -- 指向一个 wchar_t 元素的数组，数组长度足以存储一个最大字符长度的宽字符串。
- **str** -- 要被转换的多字节字符字符串。
- **n** -- 可被转换的最大字符数。

**返回说明**

- 该函数返回转换的字符数，不包括结尾的空字符。如果遇到一个无效的多字节字符，则返回 -1 值。

### 使用场景

 UTF-8、ANSI 转换Unicode。

## wcstombs()

把宽字符把转换成多字节字符串

```c
size_t wcstombs(char *mbstr, const wchar_t *wcstr, size_t count);
```

参数说明

- **mbstr** -- 指向一个 char 元素的数组，数组长度足以存储一个最大字符长度的字符串。
- **str** -- 要被转换的宽字符字符串。
- **n** -- 可被转换的最大字符数。

返回说明

- 返回转换为参数 **mbstr** 所指向的数组。

#### 使用场景

Unicode转换 UTF-8、ANSI 

## 结论

**标准库提供的函数满足现有使用需求**，

~~但由于直接调用`mbstowcs()` & `wcstombs()`函数进行转换前需要先执行`setlocal()`，较为繁琐且不宜阅读；~~

~~故仍可提供字符编码转换功能。但将实现改为对标准库的封装。~~

经过实际测试，直接调用标准库的函数并不麻烦，且由于提供的需要的编码不多，所以不提供字符编码转换功能。

## 参考资料

- [C 库函数 – mbstowcs() | 菜鸟教程 (runoob.com)](https://www.runoob.com/cprogramming/c-function-mbstowcs.html)
- [mbstowcs, _mbstowcs_l | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mbstowcs-mbstowcs-l?view=msvc-160)
- [字符串处理 - ANSI - Unicode - UTF8 转换_sandro_zhang的专栏-CSDN博客](https://blog.csdn.net/sandro_zhang/article/details/7514413)
- [mbstowcs - C++ Reference (cplusplus.com)](https://cplusplus.com/reference/cstdlib/mbstowcs/)
- [wcstombs - C++ Reference (cplusplus.com)](https://cplusplus.com/reference/cstdlib/wcstombs/)

