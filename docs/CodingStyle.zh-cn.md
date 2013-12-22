#LCUI 代码风格

本文档只是描述一些附加的代码风格，主要代码风格可以参考Linux内核的CodingStyle文档：
https://github.com/torvalds/linux/blob/master/Documentation/zh_CN/CodingStyle

##缩进 & 每行字符数限制

代码缩进宽度应设置为8个字符，每一行的代码长度应该限制在80列以内，8个字符的缩进可
以让代码更容易阅读，再加上每行字符数的限制，当你的代码缩进层次太深的时候可以给你
警告，留心这个警告。

建议缩进使用空格填充，这样就能使代码格式在其它默认缩进宽度不同的编辑器中不会有太
大变化。

##标识符的定义

如果你定义的全局变量和函数仅在当前源文件中使用，那么在定义时需在前面加上static修
饰符，以表示其只用在当前源文件内。

对于宏(#define)、类型定义(typedef)、结构体定义(struct)、枚举(enum)等，如果仅在当
前源文件中使用，那么就不应当写在头文件中。

##函数

一个函数的最大长度是和该函数的复杂度和缩进级数成反比的，如果你的函数中的代码很简
单、而且缩进层次较少，那么，这样的函数尽管很长，也是可以的。否则，你应该先重新考
虑你的函数是否干了太多的事，然后把它拆分成若干个小函数。

函数中的局部变量的数量不应超过5－10个，否则你的函数就有问题了，你需要做的和上述
的一样，对函数代码进行拆分。

在源文件里，使用空行隔开不同的函数。

## 集中的函数退出途径

虽然被某些人声称已经过时，但是goto语句的等价物还是经常被编译器所使用，具体形式是
无条件跳转指令。

当一个函数从多个位置退出并且需要做一些通用的清理工作的时候，goto的好处就显现出来
了。

理由是：

- 无条件语句容易理解和跟踪
- 嵌套程度减小
- 可以避免由于修改时忘记更新某个单独的退出点而导致的错误
- 减轻了编译器的工作，无需删除冗余代码;)

```c
int fun(int a)
{
        int result = 0;
        char *buffer;
        
        buffer = malloc(SIZE);
        ...
        if( ... ) {
            result = -1;
            goto error_out;
        }
        ...
        if( ... ) {
            result = -2;
            goto error_out;
        }
        ...
        return 0;
        
error_out:
        ...
        free(buffer);
        return result;
}

```


##函数命名规范

如果你的函数向外提供的公用函数，那么，函数的命名方式应该为：

函数类/对象/模块名_操作+对象属性

函数类/对象/模块名_子函数类/子对象/子模块+操作+对象属性

每个单词首字母应该大写，这样更容易区分出单词，因为个人感觉多（M）个单词缩写的混
合体会让人误以为是几（N，N<M）个单词。

函数命名示例：

```c
    GraphLayer_SetPos();
```

该函数属于GraphLayer模块，操作对象是图层（GraphLayer），实现的操作是设定（Set），
被操作的对象属性是位置（Pos），也就是设定图层的位置。

```c
        Queue_Add();
```

该函数的操作对象是队列（Queue），实现的操作是添加（Add），也就是往队列里添加数据。

```c
        TextLayer_Text_SetDefaultStyle();
```
        
该函数操作的主对象是文本图层(TextLayer)中的文本(Text)，实现的操作是设定(Set)，被
操作的对象属性是默认文本样式(DefaultStyle)，即：为文本图层中的文本设定默认的文本
样式。

如果你的函数仅在当前源文件中使用，那么可以不必遵循以上规则，但必须确保你的函数命
名便于阅读和理解。

如果函数名较长，应该考虑进行分行，例如：

```c
        static MyType* ObjectName_OperateAttribute( XXXX *object, XXXX arg1, XXXX arg2, XXXX arg3 )
```

应该改成：

```c
        static MyType* ObjectName_OperateAttribute(     XXXX *object,
                                                        XXXX arg1,
                                                        XXXX arg2,
                                                        XXXX arg3 )
```

如果函数名前的修饰符较多，可以将它们放到单独行里，例如：

```c
        static struct MyObjectStruct* 
        ObjectName_OperateAttribute(    XXXX *object,
                                        XXXX arg1,
                                        XXXX arg2,
                                        XXXX arg3 )
```

使用制表符缩进，调整参数列表中的参数的位置，保持对齐。

如果还是比较长的话，可以将函数名中的单词改成缩写，或者使用其它意思相同但比较短
的单词，像这样：

```c
        static struct MyObjectStruct* 
        ObjName_OptAttr( XXXX *object, XXXX arg1, XXXX arg2, XXXX arg3 )
```

调用函数时，每个参数的逗号后面需要加个空格。

```c
        func( arg1, arg2, arg3, arg4 );
```
