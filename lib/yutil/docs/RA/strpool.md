# strpool

strpool 用于解决大量相同字符串的内存占用问题。

## 函数概要

```c
strpool_t * strpool_create()    //创建字符串池        
strpool_t * strpool_free_str()    //分配字符串给字符池 
strpool_t * strpool_size()    //返回字符串池的大小     
void        strpool_destroy()    //销毁字符串池       
```

## strpool_create 

```c
 strpool_t *strpool_create(void);
```

创建字符串池。

**参数说明：**

- 无

**返回说明：**

- 返回新字符串池的指针

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/util/strlist.c
- src/gui/css_library.c

## strpool_alloc_str 

```c
 char *strpool_alloc_str(strpool_t *pool, const char *str);
```

分配字符串给字符池

**参数说明：**

- `pool`：目标字符串池
- `str`：目标字符串

**返回说明：**

- 返回指向存入字符池的字符串的指针

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/util/strlist.c
- src/gui/css_library.c



## strpool_size

```c
 size_t strpool_size(strpool_t *pool);
```

返回字符串池的大小

**参数说明：**

- `pool`：目标字符串池

**返回说明：**

- 返回字符串池占用内存空间的大小

**存在问题：**

1. 命名风格

**改进方案：**

- 修改命名风格。

**依赖它的文件：**

- 无

## strpool_destroy

```c
void strpool_destroy(strpool_t *pool);
```

销毁字符串池。

**参数说明：**

- `pool`：目标字符串池。

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/css_library.c
