# time



## LCUI

LCUI中 直接用 int64_t 存储时间的方式。

**实现方式：**

```c
int64_t 
```

## tbox

tbox 中，关于时间的存储方式：有使用`__int64`、`signed long (long) `存储时间，同时也提供了__tb_timeval_t 结构体来存储

**实现方式：**

```c
//关于时间的存储方式：tbox中使用`__int64`、`signed long (long) `存储时间
//同时也提供更加详细的结构体
typedef struct __tb_timeval_t
{
    tb_time_t                       tv_sec;
    tb_suseconds_t                  tv_usec;/*微秒us*/

}tb_timeval_t;
```

## glib

tbox 中，关于时间的存储方式：有使用gint32 存储时间，同时也提供了 GTimeVal 结构体来存储

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

GDate 数据结构表示从1月1日到未来几千年之间的某一天(现在是65535年左右，但是 `g_date_set_parse()`只解析到8000年左右)。GDate 表示的是每天的日期，而不是天文日期或历史日期或 ISO 时间戳之类的。它在时间上向前和向后推断当前的公历; 没有尝试改变日历以匹配时间周期或地点。

**GDate 实现有几个不错的特性; 它只是一个64位的结构，因此存储大量日期是非常有效的。**



## Linux

**Linux常用时间类型**
Linux下常用时间类型有四种：time_t、struct tm、struct timeval、struct timespec。

### time_t

time_t是一个长整型（即int64类型），其值表示为从UTC时间1970年1月1日00时00分00秒(也称为Linux系统的Epoch时间)到当前时刻的秒数。

由于time_t类型长度的限制，它所表示的时间不能晚于2038－1－19 03:14:07。

为了能够表示更久远的时间，可用64位或更长的整形数来保存日历时间，这里不作详述。

**优点**：

### struct tm

tm结构在time.h中定义：

```c
#ifndef _TM_DEFINED
struct tm{
    int tm_sec; /*秒 - 取值区间为[0, 59]*/
    int tm_min; /*分 - 取值区间为[0, 59]*/
    int tm_hour; /*时 - 取值区间为[0, 23]*/
    int tm_mday; /*日 - 取值区间为[1, 31]*/
    int tm_mon; /*月份 - 取值区间为[0, 11]*/
    int tm_year; /*年份 - 其值为1900年至今年数*/
    int tm_wday; /*星期 - 取值区间[0, 6]，0代表星期天，1代表星期1，以此类推*/
    int tm_yday; /*从每年的1月1日开始的天数-取值区间为[0, 365]，0代表1月1日*/
    int tm_isdst; /*夏令时标识符，使用夏令时，tm_isdst为正，不使用夏令时，tm_isdst为0，不了解情况时，tm_isdst为负*/
};
#define _TM_DEFINED
#endif
```

ANSI C 标准称使用 tm 结构的这种时间表示为分解时间(broken-down time)。
使用`gmtime( )`和`localtime( )`可将 time_t 时间类型转换为 tm 结构体；
使用`mktime( )`将 tm 结构体转换为`time_t`时间类型；
使用`asctime( )`将`struct tm`转换为字符串形式。

### **struct timeval**

timeval结构体在time.h中定义：

```c
Struct tmieval{
    time_t tv_sec; /*秒s*/
    suseconds_t tv_usec; /*微秒us*/
};
```


设置时间函数`settimeofday( )`与获取时间函数`gettimeofday( )`均使用该事件类型作为传参。

### **struct timespec**

```c
timespec结构体在time.h定义：
struct timespec{
    time_t tv_sec; /*秒s*/
    long tv_nsec; /*纳秒ns*/
};
```

## 比较和结论

`int64`适合保存精度不高的日历时间。

`timeval`结构体则更适合可以提供提高精度的时间。

综上所述：我们可以在LCUI中同时提供int64和timeval两种，以应对不同的需求。

## 参阅

**参考文章**：

- [Linux时间函数_Skyline的专栏-CSDN博客](https://blog.csdn.net/water_cow/article/details/7521567)
