#LCUI 代码风格


本文档只是描述一些附加的代码风格，主要代码风格可以参考Linux内核的CodingStyle文档：
https://github.com/torvalds/linux/blob/master/Documentation/zh_CN/CodingStyle

##限制每行代码长度

不管你的屏幕有多大，每一行的代码长度应该限制在80列以内，因为在阅读代码时，视线主
要从上往下移动，每行代码过长会使视线移动范围变大，不方便阅读。

##函数

函数名的命名方式为：

函数类/对象/模块名_操作+对象属性

函数类/对象/模块名_子函数类/子对象/子模块+操作+对象属性

每个单词首字母应该大写，这样更容易区分出单词，因为个人感觉多（M）个单词缩写的混合体会让
人误以为是几（N，N<M）个单词。

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

调用函数时，代码应该类似于这样：

```c
        func( arg1, arg2, arg3, arg4 );
```

参数列表前后都要空格，参数列表中的逗号后面也需要加个空格。还可以这样：

```c
        func (arg1, arg2, arg3, arg4);
```

函数名与()之间空一格。
