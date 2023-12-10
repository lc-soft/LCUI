# 编码规范

- 开始日期：2023-03-26
- 目标主要版本：3.x
- 参考问题：无
- 实现 PR：无

## 概要

参考其它 C 开源项目，重新制定统一的编码规范。

## 基本示例

```c
typedef enum parser_state_t {
        PARSER_STATE_START,
        PARSER_STATE_DATA_BEGIN,
        PARSER_STATE_DATA_END,
        PARSER_STATE_ERROR
} parser_state_t;

typedef struct parser_t {
        /** 解析器状态 */
        parser_state_t state;

        /** 当前字符 */
        char *cur;
} parser_t;

parser_t *parser_create(void)
{
        parser_t *parser;

        parser = malloc(sizeof(parser_t));
        parser->state = PARSER_STATE_START;
        parser->cur = NULL;
        return parser;
}
```

## 动机

2.x 版本的代码风格不一致，包含了驼峰、小写下划线风格以及各种命名规则，导致源码阅读体验和接口使用体验较差。

## 详细设计

使用 clang-format 工具对源码进行格式化，其配置文件 `.clang-format` 存放于源码根目录中。

由于预设的格式化规则已经满足此编码规范的大部分要求，以下仅对需要在编码时注意的规范进行简单说明，不再详细说明规范的全部内容。

### 命名

常见的 C 语言开源库的编码风格大都是蛇形命名法(`snake_case`)，例如：[libgit2](https://github.com/libgit2/libgit2)、[cairo](https://www.cairographics.org/)、[leveldb](https://github.com/google/leveldb/blob/main/include/leveldb/c.h)，因此，LCUI 项目也采用该编码风格。

由于 LCUI 已被拆分为多个子库，这些库内的标识符不再需要添加 `LCUI_` 前缀。

类型名以 `_t` 结尾，例如：

```c
typedef struct foo_t {
        int bar;
} foo_t;
```

宏定义和枚举采用大写下划线命名法，例如：

```c
typedef enum pd_color_type_t {
	    PD_COLOR_TYPE_INDEX8,
	    PD_COLOR_TYPE_GRAY8,
	    PD_COLOR_TYPE_RGB323,
	    PD_COLOR_TYPE_ARGB2222,
	    PD_COLOR_TYPE_RGB555,
	    PD_COLOR_TYPE_RGB565,
	    PD_COLOR_TYPE_RGB888,
	    PD_COLOR_TYPE_ARGB8888
} pd_color_type_t;

#define PANDAGL_VERSION "0.1.0"
```

### 注释

推荐写在代码行上面：

```ts
struct foo {
        /** comment */
        int bar;

        /** string */
        char *str;
}
```

不推荐写在右侧：

```ts
struct foo {
        int bar;    /**< comment */
        char *str;  /**< string */
}
```

### 缩进

为确保代码在网页端和编辑器内的显示效果一致，以八个空格为一个缩进层级，不使用制表符缩进。

## 缺点

改动很大，涉及几乎所有代码，从 2.x 版本迁移到 3.x 版本需要参照头文件内容对旧的标识符进行全局替换。

## 备选方案

全部采用驼峰命名。
