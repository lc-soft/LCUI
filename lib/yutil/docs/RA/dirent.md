# dirent

提供目录操作相关函数。

## 函数概要

```c
int            LCUI_OpenDirA()    //用于打开目录

LCUI_DirEntry* LCUI_OpenDirW()    //用于打开目录
LCUI_DirEntry* LCUI_ReadDirA()    //读取目录，获取目录的详细信息。      
int            LCUI_ReadDirW()    //读取目录，获取目录的详细信息。      
char *         LCUI_CloseDir()    //关闭目录    
wchar_t *      LCUI_GetFileNameA()    //获取文件名                      
int            LCUI_GetFileNameW()    //获取文件名的宽字符版本          
int            LCUI_FileIsDirectory()    //判断所找到的项目是不是目录   
int            LCUI_FileIsRegular()    //来判断所找到的项目是不是常规文件夹 
```



## LCUI_OpenDirA

```c
int LCUI_OpenDirA(const char *path, LCUI_Dir *dir);

```

打开目录

**参数说明：**

- `path`：目标目录，字符数组
- `dir`：用LCUI_Dir结构体存放打开的结果，

**返回说明：**

- 成功返回 0，失败返回-1

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 



## LCUI_OpenDirW

```c
int LCUI_OpenDirW(const wchar_t *path, LCUI_Dir *dir);
```

打开目录,是LCUI_OpenDirA的宽字符版本

**参数说明：**

- `path`：目标目录，宽字符类型字符数组
- `dir`：用LCUI_Dir结构体存放打开的结果，

**返回说明：**

- 成功返回 0，失败返回-1

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## LCUI_ReadDirA

```c
LCUI_DirEntry *LCUI_ReadDirA(LCUI_Dir *dir);
```

读取目录，获取目录的详细信息。

**参数说明：**

- `dir`：指向目标目录的LCUI_Dir指针

**返回说明：**

- 成功返回指向目录下文件的开始，`LCUI_DirEntry`指针，失败返回 NULL

**存在问题：**

1. 命名风格

**改进方案：**

- 修改命名风格。

**依赖它的文件：**

- 无



## LCUI_ReadDirW

```c
LCUI_DirEntry *LCUI_ReadDirW(LCUI_Dir *dir);
```

读取目录，获取目录的详细信息。Windows支持版。

**参数说明：**

- `dir`：指向目标目录的LCUI_Dir指针

**返回说明：**

- 成功返回指向目录下文件的开始，LCUI_DirEntry指针，失败返回 NULL

**存在问题：**

1. 考虑与LCUI_ReadDirA是否可以合并

**改进方案：**

- 调研其他库函数，考虑是否可以合并。

**依赖它的文件：**

- 无

## LCUI_CloseDir

```c
int LCUI_CloseDir(LCUI_Dir *dir);
```

关闭目录

**参数说明：**

- `dir`：指向目标目录的`LCUI_Dir`指针

**返回说明：**

- 成功返回 0，失败返回-1

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## LCUI_GetFileNameA

```c
LCUI_API char *LCUI_GetFileNameA(LCUI_DirEntry *entry);
```

获取文件名

**参数说明：**

- `entry`：通过指向目录下文件的开始，`LCUI_DirEntry`指针输入，获取文件名

**返回说明：**

- 成功返回指向目录下文件名，失败返回 NULL

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## LCUI_GetFileNameW

```c
wchar_t *LCUI_GetFileNameW(LCUI_DirEntry *entry);
```

获取文件名的宽字符版本

**参数说明：**

- `entry`：通过`LCUI_DirEntry`指针输入，获取文件名

**返回说明：**

- 成功返回指向目录下宽字符类型的文件名，失败返回 NULL

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## LCUI_FileIsDirectory

```c
int LCUI_FileIsDirectory(LCUI_DirEntry *entry);
```

将`entry->dataW.dwFileAttributes`和`FILE_ATTRIBUTE_DIRECTORY` 做与运算，

来判断所找到的项目是不是目录

**参数说明：**

- `entry`：LCUI_DirEntry指针

**返回说明：**

- 成功1，失败返回0；

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## LCUI_FileIsRegular

```c
int LCUI_FileIsRegular(LCUI_DirEntry *entry);
```

将`entry->dataW.dwFileAttributes`和`FILE_ATTRIBUTE_DIRECTORY`做位的与运算，并作非运算。
来判断所找到的项目是不是常规文件

**参数说明：**

- `entry`：LCUI_DirEntry指针

**返回说明：**

- 成功1，失败返回0；

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无
