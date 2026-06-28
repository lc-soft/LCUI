# glib

- [GLib Utilities: GLib Reference Manual (gnome.org)](https://developer.gnome.org/glib/stable/glib-utilities.html)

暂只对 GLib Utilities 以中与 LCUI 实用工具库关系较近的**部分库的部分函数**进行调研。

## 概要

| 类型                                                         | 说明                                  |
| :----------------------------------------------------------- | :------------------------------------ |
| [字符串实用函数](https://developer.gnome.org/glib/stable/glib-String-Utility-Functions.html) | 各种与字符串相关的函数                |
| [字符集转换](https://developer.gnome.org/glib/stable/glib-Character-Set-Conversion.html) | 在不同字符集之间转换字符串            |
| **[Unicode](https://developer.gnome.org/glib/stable/glib-Unicode-Manipulation.html)** | 操作 Unicode 字符和 UTF-8字符串的函数 |
| [GDateTime](https://developer.gnome.org/glib/stable/glib-GDateTime.html) | 表示日期和时间的结构                  |
| [日期及时间功能](https://developer.gnome.org/glib/stable/glib-Date-and-Time-Functions.html) | 日历计算和其他时间的东西              |
| [计时器](https://developer.gnome.org/glib/stable/glib-Timers.html) | keep track of elapsed time            |
| [Windows 兼容功能](https://developer.gnome.org/glib/stable/glib-Windows-Compatibility-Functions.html) | Windows 环境下仿 UNIX                 |
| [GLib Data Types](https://developer.gnome.org/glib/stable/glib-data-types.html) | 数据类型                              |



## **[字符串实用函数](https://developer.gnome.org/glib/stable/glib-String-Utility-Functions.html)** 

**使用场景：**

- **用于创建、复制和操作字符串的许多实用函数**

由于包含函数过多，故结合分析报告，暂只调研以下内容

### g_strdup ()

```C
gchar *
g_strdup (const gchar *str);
```

复制字符串。如果 str 为 NULL，则返回 NULL。当不再需要时，返回的字符串应该用 `g_free ()`释放。

### g_strndup ()

```C
gchar *
g_strndup (const gchar *str,
           gsize n);
```

复制字符串的前 n 个字节，返回一个新分配的长度为 n + 1字节的缓冲区，该缓冲区总是以 nul 结尾。如果 str 的长度小于 n 个字节，则用 null 填充缓冲区。如果 str 为 NULL，则返回 NULL。当不再需要时，应释放返回的值。

若要从 UTF-8编码的字符串中复制一定数量的字符，请使用 `g_utf8_strncpy ()`。

### g_strdupv ()

```C
gchar **
g_strdupv (gchar **str_array);
```

复制以 null 结尾的字符串数组。复制是深度复制; 应该首先释放每个字符串，然后释放数组本身，以释放新数组。如果调用 NULL 值，则 `g_strdupv ()`只返回 NULL。

### g_str_to_ascii ()

```c
gchar *
g_str_to_ascii (const gchar *str,
                const gchar *from_locale);
```

将 UTF-8 str 译为普通 ASCII 码。

### g_ascii_strup ()

```c
gchar *
g_ascii_strup (const gchar *str,
               gssize len);
```

将所有小写的 ASCII 字母转换为大写的 ASCII 字母。

### g_ascii_strdown ()

```c
gchar *
g_ascii_strdown (const gchar *str,
                 gssize len);
```

将所有大写的 ASCII 字母转换为小写的 ASCII 字母。

### g_ascii_tolower ()

```c
gchar
g_ascii_tolower (gchar c);
```

将字符转换为 ASCII 小写。

与标准的 c 库 tolower ()`函数不同，这只能识别标准的 ASCII 字母，并忽略语言环境，返回所有未修改的非 ASCII 字符，即使它们是特定字符集中的小写字母。与标准库函数不同的是，这个函数接受并返回一个 char，而不是 int，所以不要在 EOF 上调用它，但是在传入一个可能非 ascii 字符之前，不需要担心将它转换为 guchar。

### g_ascii_toupper ()

```c
gchar
g_ascii_toupper (gchar c);
```

将字符转换为 ASCII 大写字母。

与标准的 c 库 toupper ()`函数不同，这只能识别标准的 ASCII 字母，并忽略语言环境，返回所有未修改的非 ASCII 字符，即使它们是特定字符集中的大写字母。与标准库函数不同的是，这个函数接受并返回一个 char，而不是 int，所以不要在 EOF 上调用它，但是在传入一个可能非 ascii 字符之前，不需要担心将它转换为 guchar。

### g_strin_ascii_up ()

```c
GString *
g_string_ascii_up (GString *string);
```

将所有小写的 ASCII 字母转换为大写的 ASCII 字母。

### g_string_ascii_down ()

```c
GString *
g_string_ascii_down (GString *string);
```

将所有大写 ASCII 字母转换为小写 ASCII 字母。



### g_strstrip()

```c
#define             g_strstrip( string )
```

从字符串中移除前和尾空格。

### g_strsplit ()

```c
gchar **
g_strsplit (const gchar *string,
            const gchar *delimiter,
            gint max_tokens);
```

使用给定的分隔符将字符串拆分为最多个max_tokens部分。如果达到 max_token，字符串的其余部分将追加到最后一个后。

### g_strsplit_set ()

```c
gchar **
g_strsplit_set (const gchar *string,
                const gchar *delimiters,
                gint max_tokens);
```

将字符串拆分为若干不包含分隔符中任何字符的标记。标记是不包含任何分隔符的最长字符串(可能是空的)。如果达到 `max_token`，则将剩余字符追加到最后一个`token`后。



## [字符集转换](https://developer.gnome.org/glib/stable/glib-Character-Set-Conversion.html)

字符集转换ー在不同字符集之间转换字符串

GLib 提供了函数 `g_filename_to_utf8()`和 `g_filename_from_utf8()`来执行必要的转换。这些函数将文件名从 `g_filename_encoding` 中指定的编码转换为 UTF-8，反之亦然。下图说明了如何使用这些函数在 UTF-8和文件系统中的文件名编码之间进行转换。

由于包含函数过多，故结合分析报告，暂只调研以下内容

### g_locale_to_utf8 ()

```
gchar *
g_locale_to_utf8 (const gchar *opsysstring,
                  gssize len,
                  gsize *bytes_read,
                  gsize *bytes_written,
                  GError **error);
```

将当前语言环境中 c 运行时用于字符串的编码中的字符串(通常与操作系统使用的字符串相同)转换为 UTF-8字符串。

### g_filename_to_utf8 ()

```
gchar *
g_filename_to_utf8 (const gchar *opsysstring,
                    gssize len,
                    gsize *bytes_read,
                    gsize *bytes_written,
                    GError **error);
```

将 GLib 对文件名使用的编码中的字符串转换为 UTF-8字符串。注意，在 Windows GLib 中，文件名使用 UTF-8; 在其他平台上，这个函数间接取决于当前语言环境。

### g_filename_from_utf8 ()

```
gchar *
g_filename_from_utf8 (const gchar *utf8string,
                      gssize len,
                      gsize *bytes_read,
                      gsize *bytes_written,
                      GError **error);
```

将一个字符串从 UTF-8转换为文件名使用的 GLib 编码。注意，在 Windows GLib 中，文件名使用 UTF-8; 在其他平台上，这个函数间接取决于当前语言环境。

### g_get_filename_charsets ()

```
gboolean
g_get_filename_charsets (const gchar ***filename_charsets);
```

确定用于文件名的首选字符集。来自字符集的第一个字符集是文件名编码，后续的字符集在试图生成一个文件名的可显示表示时使用，参见 `g_filename_display_name ()`。

### g_filename_display_name ()

```
gchar *
`g_filename_display_name (const gchar *filename);
```

将文件名转换为有效的 UTF-8字符串。这个转换不一定是可逆的，所以您应该保留原来的函数，并且只为了显示的目的使用这个函数的返回值。与 `g_filename_to_utf8()`不同，即使文件名实际上不在 GLib 文件名编码中，结果也肯定是非 null 的。

### g_filename_display_basename ()

```
gchar *
g_filename_display_basename (const gchar *filename);
```

返回特定文件名的显示基名，保证是有效的 UTF-8。显示名称可能与文件名不同，例如，将其转换为 UTF-8可能会出现问题，而且某些文件可以在显示中进行翻译。

### g_locale_from_utf8 ()

```
gchar *
g_locale_from_utf8 (const gchar *utf8string,
                    gssize len,
                    gsize *bytes_read,
                    gsize *bytes_written,
                    GError **error);
```

将字符串从 UTF-8转换为当前语言环境中 c 运行时使用的字符串编码(通常与操作系统使用的编码相同)。在 Windows 上，这意味着系统代码页。

### g_get_charset ()

```
gboolean
g_get_charset (const char **charset);
```

获取当前区域设置的字符集; 您可以将此字符集用作 `g_convert ()`的参数，将当前区域设置的编码转换为其他编码。(通常 `g_locale_to_utf8()`和 `g_locale_from_utf8()`都是很好的快捷方式。)

### g_get_codeset ()

```
gchar *
g_get_codeset (void);
```

获取当前区域设置的字符集。



## **[Unicode](https://developer.gnome.org/glib/stable/glib-Unicode-Manipulation.html)** 

Unicode 的 UTF-8、 UTF-16和 UCS-4编码之间进行转换的函数。

由于包含函数过多，故结合分析报告，暂只调研以下内容。

### g_utf8_get_char ()

```c
gunichar
`g_utf8_get_char (const gchar *p);
```

将以 UTF-8编码的字节序列转换为 Unicode字符。

### g_utf8_get_char_validated ()

```c
gunichar
g_utf8_get_char_validated (const gchar *p,
                           gssize max_len);
```

将以 UTF-8编码的字节序列转换为 Unicode字符。此函数检查不完整字符、无效字符(如 Unicode 范围之外的字符)以及有效字符的编码过长。

### g_utf8_offset_to_pointer ()

```
gchar *
g_utf8_offset_to_pointer (const gchar *str,
                          glong offset);
```

将整数字符偏移量转换为指向字符串中某个位置的指针。

### g_utf8_pointer_to_offset ()

```
glong
g_utf8_pointer_to_offset (const gchar *str,
                          const gchar *pos);
```

将指针转换为字符串中的位置，转换为整数字符偏移量。

### g_utf8_prev_char ()

```
gchar *
g_utf8_prev_char (const gchar *p);
```

查找字符串中 p 前面的前一个 UTF-8字符。

## **GDateTime** 

存储日期和时间的结构体。

```c
struct _GDateTime
{
  /* Microsecond timekeeping within Day */
  guint64 usec;

  /* TimeZone information */
  GTimeZone *tz;
  gint interval;

  /* 1 is 0001-01-01 in Proleptic Gregorian */
  gint32 days;

  gint ref_count;  /* (atomic) */
};
```

由于包含函数过多，故结合分析报告，暂只调研以下内容。

### g_date_time_unref ()

```
void
g_date_time_unref (GDateTime *datetime);
```

自动地将日期时间的引用计数减少一个。

### g_date_time_ref ()

```
GDateTime *
g_date_time_ref (GDateTime *datetime);
```

自动地将日期时间的引用计数增加一个。

### g_date_time_new_now ()

```
GDateTime *
g_date_time_new_now (GTimeZone *tz);
```

在给定的时区 tz 中创建与此精确时刻对应的 GDateTime。时间在系统允许的范围内是精确的，最大精确度为1微秒。

### g_date_time_new_now_local ()

```
GDateTime *
g_date_time_new_now_local (void);
```

在本地时区中创建与此精确时刻对应的 GDateTime。

### g_date_time_new_now_utc ()

```
GDateTime *
g_date_time_new_now_utc (void);
```

创建与 UTC 中的此确切时刻对应的 GDateTime。

### g_date_time_new_from_unix_local ()

```
GDateTime *
g_date_time_new_from_unix_local (gint64 t);
```

在本地时区中创建与给定的 Unix 时间 t 对应的 GDateTime。

### g_date_time_new_from_unix_utc ()

```
GDateTime *
g_date_time_new_from_unix_utc (gint64 t);
```

创建与给定的 Unix 时间 t 对应的 GDateTime。

### g_date_time_new ()

```
GDateTime *
g_date_time_new (GTimeZone *tz,
                 gint year,
                 gint month,
                 gint day,
                 gint hour,
                 gint minute,
                 gdouble seconds);
```

在时区 tz 中创建与给定日期和时间相对应的新 GDateTime。

### g_date_time_new_local ()

```
GDateTime *
g_date_time_new_local (gint year,
                       gint month,
                       gint day,
                       gint hour,
                       gint minute,
                       gdouble seconds);
```

创建与本地时区中的给定日期和时间对应的新 GDateTime。

### g_date_time_new_utc ()

```
GDateTime *
g_date_time_new_utc (gint year,
                     gint month,
                     gint day,
                     gint hour,
                     gint minute,
                     gdouble seconds);
```

以 UTC 格式创建与给定日期和时间对应的新 GDateTime。

### g_date_time_add ()

```
GDateTime *
g_date_time_add (GDateTime *datetime,
                 GTimeSpan timespan);
```

创建日期时间的副本并将指定的时间盘添加到该副本。

### g_date_time_add_years ()

```
GDateTime *
g_date_time_add_years (GDateTime *datetime,
                       gint years);
```

创建日期时间的副本，并将指定的年数添加到副本中。加上负值，减去年份。

### g_date_time_add_months ()

```
GDateTime *
g_date_time_add_months (GDateTime *datetime,
                        gint months);
```

创建日期时间的副本，并将指定的月数添加到副本中。加上负值减去月份。

### g_date_time_add_weeks ()

```
GDateTime *
g_date_time_add_weeks (GDateTime *datetime,
                       gint weeks);
```

创建日期时间的副本，并将指定的周数添加到副本中。将负值加到减去周数。

### g_date_time_add_days ()

```
GDateTime *
g_date_time_add_days (GDateTime *datetime,
                      gint days);
```

创建日期时间的副本并将指定的天数添加到副本中。加上负值，减去天数。

### g_date_time_add_hours ()

```
GDateTime *
g_date_time_add_hours (GDateTime *datetime,
                       gint hours);
```

创建日期时间的副本并添加指定的小时数。添加负值以减去小时。

### g_date_time_add_minutes ()

```
GDateTime *
g_date_time_add_minutes (GDateTime *datetime,
                         gint minutes);
```

创建添加指定分钟数的日期时间的副本。添加负值以减去分钟。

### g_date_time_add_seconds ()

```
GDateTime *
g_date_time_add_seconds (GDateTime *datetime,
                         gdouble seconds);
```

创建日期时间的副本并添加指定的秒数。添加负值以减去秒数。

### g_date_time_add_full ()

```
GDateTime *
g_date_time_add_full (GDateTime *datetime,
                      gint years,
                      gint months,
                      gint days,
                      gint hours,
                      gint minutes,
                      gdouble seconds);
```

创建一个新的 GDateTime，将指定的值添加到日期时间中的当前日期和时间。增加负值以减去。

### g_date_time_format ()

```
gchar *
g_date_time_format (GDateTime *datetime,
                    const gchar *format);
```

创建一个新分配的字符串，表示format格式。 *``* .

## Date and Time Functions

计算日历和其他时间的函数。

GDate 数据结构表示从1月1日到未来几千年之间的某一天(现在是65535年左右，但是 `g_date_set_parse ()`只解析到8000年左右——只有“几千”)。GDate 表示的是每天的日期，而不是天文日期或历史日期或 ISO 时间戳之类的。它在时间上向前和向后推断当前的公历; 没有尝试改变日历以匹配时间周期或地点。GDate 不存储时间信息; 它表示一天。

**GDate 实现有几个不错的特性; 它只是一个64位的结构，因此存储大量日期是非常有效的。**

```c
struct GTimeVal {
  glong tv_sec;
  glong tv_usec;
};
struct GDate {
  guint julian_days : 32; /* julian days representation - we use a
                           *  bitfield hoping that 64 bit platforms
                           *  will pack this whole struct in one big
                           *  int
                           */

  guint julian : 1;    /* julian is valid */
  guint dmy    : 1;    /* dmy is valid */

  /* DMY representation */
  guint day    : 6;
  guint month  : 4;
  guint year   : 16;
};

typedef gint32  GTime GLIB_DEPRECATED_TYPE_IN_2_62_FOR(GDateTime);
```

由于包含函数过多，故结合分析报告，暂只调研以下内容。

### g_usleep ()

```
void
g_usleep (gulong microseconds);
```

将当前线程暂停为给定的微秒数。

### g_get_monotonic_time ()

```
gint64
g_get_monotonic_time (void);
```

查询系统单调时间。

### g_get_real_time ()

```
gint64
g_get_real_time (void);
```

查询系统时间。.

### g_date_new ()

```
GDate *
g_date_new (void);
```

分配 GDate 并将其初始化为安全状态。新的日期将被清除(就像您调用了 `g_date_clear ()`一样) ，但是无效(它不代表现有的日期)。使用 `g_date_Free ()`释放返回值。

### g_date_new_dmy ()

```
GDate *
g_date_new_dmy (GDateDay day,
                GDateMonth month,
                GDateYear year);
```

类似于 `g_date_new ()` ，但也设置日期的值。假设传入的天-月-年三元组表示现有的天，返回的日期将有效。

### g_date_new_julian ()

```
GDate *
g_date_new_julian (guint32 julian_day);
```

类似于 `g_date_new ()` ，但也设置日期的值。假设传入的儒略日号有效(大于0，小于一个不合理的大数) ，返回的日期将有效。.

### g_date_clear ()

```
void
g_date_clear (GDate *date,
              guint n_dates);
```

将一个或多个 GDate 结构初始化为安全但无效的状态。已清除的日期将不表示现有日期，但不包含垃圾。用于初始化在堆栈上声明的日期。有效性可以使用 `g_date_valid ()`进行测试

### g_date_free ()

```
void
g_date_free (GDate *date);
```

释放从 `g_date_new ()`返回的 GDate。

### g_date_copy ()

```
GDate *
g_date_copy (const GDate *date);
```

将 GDate 复制到新分配的 GDate。如果输入无效(由 `g_ date_valid ()`确定) ，则无效状态将被复制到新对象中。



## Timer

GTimer 记录一个开始时间，并计算从那时起经过的微秒数。这在不同的平台上做得有些不同，而且可能很难做到完全正确，因此 GTimer 提供了一个可移植/方便的界面。

```c
/**
 * GTimer:
 *
 * Opaque datatype that records a start time.
 **/
struct _GTimer
{
  guint64 start;
  guint64 end;

  guint active : 1;
};
```

由于包含函数过多，故结合分析报告，暂只调研以下内容。

### g_timer_new ()

```
GTimer *
g_timer_new (void);
```

创建一个新的计时器，并开始计时(即隐式地为您调用 `g_timer_start ()`)。

### g_timer_start ()

```
void
g_timer_start (GTimer *timer);
```

标记一个开始时间，这样以后对 `g_timer_elapsed ()`的调用将报告自调用 `g_timer_start ()`以来的时间。`g_ timer_new ()`自动标记开始时间，因此不需要在创建计时器后立即调用 `g_ timer_start ()`。

### g_timer_stop ()

```
void
g_timer_stop (GTimer *timer);
```

标记结束时间，因此调用 `g_timer_elapsed ()`将返回结束时间和开始时间之间的差值。

### g_timer_continue ()

```
void
g_timer_continue (GTimer *timer);
```

恢复以前用 gtimer_stop ()`停止的计时器。在使用此函数之前必须调用 gtimer_stop ()`。

### g_timer_elapsed ()

```
gdouble
g_timer_elapsed (GTimer *timer,
                 gulong *microseconds);
```

如果计时器已启动但尚未停止，则获得计时器启动后的时间。如果计时器已经停止，则获取从启动计时器到停止计时器的时间间隔。返回值是经过的秒数，包括任何小数部分。微秒输出参数基本上是无用的。

### g_timer_reset ()

```
void
g_timer_reset (GTimer *timer);
```

这个函数是无用的; 可以在已经开始的计时器上调用 `g_timer_start ()`来重置开始时间，所以 `g_timer_reset ()`没有用处。

### g_timer_destroy ()

```
void
g_timer_destroy (GTimer *timer);
```

销毁计时器，释放相关资源。

### g_timer_is_active ()

```
gboolean
g_timer_is_active (GTimer *timer);
```

判断计时器当前是否处于活动状态

## Windows Compatibility Functions

由于包含函数过多，故结合分析报告，暂只调研以下内容。

### g_win32_get_command_line ()

```
gchar **
g_win32_get_command_line (void);
```

获取 Windows 上 GLib 文件名编码中的命令行参数(即: UTF-8)。

### g_win32_error_message ()

```
gchar *
g_win32_error_message (gint error);
```

将 Win32错误代码(由 GetLastError ()`或 WSAGetLastError ()`返回)转换为相应的消息

### g_win32_getlocale ()

```
gchar *
g_win32_getlocale (void);
```

Microsoft c 库中的 `setlocale ()`函数使用“ English_united States. 1252”等形式的 locale 名称。我们需要 UNIXish 标准格式“ en_us”、“ zh_tw”等。这个函数从 Windows 获取当前线程语言环境-没有任何编码信息-并返回它作为一个字符串的上述形式，用于形成文件名等。返回的字符串应该释放为 `g_free ()`。

### g_win32_get_package_installation_directory_of_module ()

```
gchar *
g_win32_get_package_installation_directory_of_module
                               (gpointer hmodule);
```

此函数尝试根据软件包的 DLL 位置确定软件包的安装目录。

### g_win32_locale_filename_from_utf8 ()

```
gchar *
 g_win32_locale_filename_from_utf8 (const gchar *utf8filename);
```

将文件名从 UTF-8转换为系统代码页。

### g_WIN32_DLLMAIN_FOR_DLL_NAME()

```
# define  g_WIN32_DLLMAIN_FOR_DLL_NAME(static, dll_name) GLIB_DEPRECATED_MACRO_IN_2_26
```

在 Windows 上，此宏定义一个 DllMain ()`函数，该函数存储要编译的代码将包含在其中的实际 DLL 名称。

### g_WIN32_HAVE_WIDECHAR_API

```
#define g_WIN32_HAVE_WIDECHAR_API() TRUE
```

在 Windows 上，如果代码运行在 Win32 API 函数的宽字符版本和 c 库函数的宽字符版本工作的 Windows 版本上，这个宏定义了一个表达式，计算结果为 TRUE。(它们总是出现在 dll 中，但在 Windows 9x 和 Me 上不起作用。)

### g_WIN32_IS_NT_BASED

```
#define  g_WIN32_IS_NT_BASED() TRUE
```

在 Windows 上，此宏定义了一个表达式，如果代码在基于 nt 的 Windows 操作系统上运行，则该表达式的计算结果为 TRUE。



## 编码风格

- [C Coding Style (gnome.org)](https://developer.gnome.org/programming-guidelines/stable/c-coding-style.html.en)

### 命名规则

glib的各种实用程序具有一致的接口。它的编码风格是半面向对象，标识符加了一个前缀“g”，这也是一种通行的命名约定。

`g_+小写+下划线`。

### 行宽

尽量使用80到120个字符之间。

### 缩进

GNU style.：每层缩进2个空格

### 类型问题

为了做到跨平台，所有类型，都是全部重定义过。

### 头文件

头文件的唯一主要规则是，函数定义应该垂直对齐三列:

```c
return_type          function_name           (type   argument,
                                              type   argument,
                                              type   argument);
```

>  如果您正在创建一个公共库，请尝试导出一个单独的公共头文件，该文件反过来包含所有较小的头文件。这样就不会直接包含公共标头，而是在应用程序中使用单个 include。

对于库，所有头文件都应该有包含保护(用于内部使用)和 c++ 保护。

###  **GObject 类**

GObject 类定义和实现需要一些额外的编码样式通知，并且应该始终正确地使用名称空间。

Typedef 声明应该放在文件的开头:

```c
typedef struct _GtkBoxedStruct       GtkBoxedStruct;
typedef struct _GtkMoreBoxedStruct   GtkMoreBoxedStruct;
//枚举类型:
typedef enum
{
  GTK_SIZE_REQUEST_WIDTH_FOR_HEIGHT,
  GTK_SIZE_REQUEST_HEIGHT_FOR_WIDTH
} GtkSizeRequestMode;
//回调类型:
typedef void (* GtkCallback) (GtkWidget *widget,
                              gpointer   user_data);
```

实例结构应该使用 `G_DECLARE_FINAL_TYPE` 或 `G_DECLARE_DERIVABLE_TYPE`:

```c
#define GTK_TYPE_FOO (gtk_foo_get_type ())
G_DECLARE_FINAL_TYPE (GtkFoo, gtk_foo, GTK, FOO, GtkWidget)
```

对于final类型，私有数据可以存储在对象结构中，这应该在 c 文件中定义:

```c
struct _GtkFoo
{
  GObject   parent_instance;

  guint     private_data;
  gpointer  more_private_data;
};
```

对于可派生类型，私有数据必须存储在 c 文件中的一个私有结构中，使用`G_DEFINE_TYPE_WITH_PRIVATE()`配置，并使用 `_get_instance_private()` 函数访问:

```c
#define GTK_TYPE_FOO gtk_foo_get_type ()
G_DECLARE_DERIVABLE_TYPE (GtkFoo, gtk_foo, GTK, FOO, GtkWidget)

struct _GtkFooClass
{
  GtkWidgetClass parent_class;

  void (* handle_frob)  (GtkFrobber *frobber,
                         guint       n_frobs);

  gpointer padding[12];
};
```

使用 `G_DEFINE_TYPE()`, `G_DEFINE_TYPE_WITH_PRIVATE()`, 和 `G_DEFINE_TYPE_WITH_CODE()` 宏, 或者它们的抽象变体 `G_DEFINE_ABSTRACT_TYPE()`, `G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(),` 和`G_DEFINE_ABSTRACT_TYPE_WITH_CODE() ；`还要使用类似的宏来定义接口和封装类型。.

接口类型应该始终使用用于强制转换的虚拟 typedef:

```c
typedef struct _GtkFooable          GtkFooable;
```

接口结构应该有到 dummy typedef的标准化接口

```c
typedef struct _GtkFooableInterface     GtkFooableInterface;
```

接口必须具有以下宏:

| Macro                      | Expands to                    |
| -------------------------- | ----------------------------- |
| GTK_TYPE_*iface_name*      | *iface_name*_get_type         |
| GTK_*iface_name*           | G_TYPE_CHECK_INSTANCE_CAST    |
| GTK_IS_*iface_name*        | G_TYPE_CHECK_INSTANCE_TYPE    |
| GTK_*iface_name*_GET_IFACE | G_TYPE_INSTANCE_GET_INTERFACE |

### 宏

除非绝对必要，尽量避免使用私有宏。记住在一个块或一系列需要它们的函数的末尾 # undef 它们。

内联函数通常优于私有宏。

除非计算为常数，否则不应使用公共宏。

### Public API

避免将变量导出为公共 API，因为这在某些平台上很麻烦。最好还是添加 getter 和 setter。另外，通常要注意全局变量。

### Private API

在多个源文件中需要的非导出函数应该以下划线(“ _”)作为前缀，并在私有头文件中声明。例如，_ mylib _ internal _ foo ()。

下划线前缀函数从不导出。

只在一个源文件中需要的非导出函数应声明为静态函数。
