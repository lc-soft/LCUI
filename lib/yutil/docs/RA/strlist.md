# strlist

strlist 是针对部件和 CSS 选择器的多字符串存储需求而设计的，配合 strpool 使用。

## 函数概要

```c
int  strlist_add()    //向字符串组添加字符串              
int  strlist_add_one()    //向字符串组添加一个字符串       
int  strlist_remove_one()    //删除字符串组中的一个字符串  
int  sortedstrlist_add()    //向已排序的字符串组添加字符串 
int  strlist_has()    //判断字符串组中是否包含指定字符串  
int  strlist_remove()    //从字符串组中移除指定字符串      
void strlist_free()    //释放字符串组                      
```



## strlist_add 

```c
int strlist_add(strlist_t *strlist, const char *str);

```

向字符串组添加字符串

**参数说明：**

- `strlist` – 字符串组
- `str` – 字符串

**返回说明：**

- 返回添加字符串组的数量

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/widget_style.c
- src/gui/widget_status.c
- src/gui/widget_class.c
- src/gui/css_library.c

## strlist_add_one 

```c
 int strlist_add_one(strlist_t *strlist, const char *str);
```

向字符串组添加一个字符串

**参数说明：**

- `strlist` – 字符串组
- `str` – 字符串

**返回说明：**

- 添加成功返回1，失败返回0

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## strlist_remove_one

```c
 int strlist_remove_one(strlist_t *strlist, const char *str);
```

删除字符串组中的一个字符串

**参数说明：**

- `strlist` – 字符串组
- `str` – 目标字符串

**返回说明：**

- 成功返回1，失败返回0

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无



## sortedstrlist_add 

```c
int sortedstrlist_add(strlist_t *strlist, const char *str);
```

向已排序的字符串组添加字符串


**参数说明：**

- `strlist` – 字符串组
- `str` – 字符串

**返回说明：**

- 添加成功返回1

**存在问题：**

1. 头文件中重复定义

**改进方案：**

- 删除重复的定义

**依赖它的文件：**

- src/gui/widget_style.c
- src/gui/css_library.c

## strlist_has

```c
int strlist_has(strlist_t strlist, const char *str)
```

判断字符串组中是否包含指定字符串

**参数说明：**

- `strlist` – 字符串组
- `str` – 字符串

**返回说明：**

- 如果包含则返回 1， 否则返回 0

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/widget_class.c
- src/gui/widget_status.c

## strlist_remove 

```c
int strlist_remove(strlist_t *strlist, const char *str);

```

从字符串组中移除指定字符串

**参数说明：**

- `strlist` – 字符串组
- `str` – 字符串

**返回说明：**

- 如果删除成功则返回 1， 否则返回 0

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/widget_class.c

- src/gui/widget_status.cint sortedstrlist_add(strlist_t *strlist, const char *str)


## strlist_free

```c
 void strlist_free(strlist_t strs);
```

释放字符串组

**参数说明：**

- `strs` – 字符串组

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/css_library.c
- src/gui/widget_class.c
- src/gui/widget_status.c
- src/util/string.c



