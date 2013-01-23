#LCUI 代码风格

这是一个简短的文档，主要代码风格可以参考Linux内核的，这里只是描述一些附加的代码风格。

##限制每行代码长度

不管你的屏幕有多大，每一行的代码长度应该限制在80列以内，在阅读代码时，视线主要从上往下
移动，每行代码太长的话，视线移动范围会变大，容易引起视觉疲劳。
而在github等一些网站上，网页会限制每行能显示的字符数，如果网页是自动对超出范围的字符进
行换行，那么代码过长可能会使整个代码变的混乱；如果是显示滚动条，通过拖动滚动条浏览未显
示出来的代码，这很麻烦。

##函数

函数名的命名方式为：函数类/对象/模块名_操作+对象属性
每个单词首字母应该大写，这样更容易区分出单词，因为个人感觉多（M）个单词缩写的混合体会让
人误以为是几（N，N<M）个单词。

函数命名示例：

GraphLayer_SetPos();

该函数属于GraphLayer模块，操作对象是图层（GraphLayer），实现的操作是设定（Set），
被操作的对象属性是位置（Pos），也就是设定图层的位置。

Queue_Add();

该函数的操作对象是队列（Queue），实现的操作是添加（Add），也就是往队列里添加数据。

如果函数名较长，应该考虑进行分行，例如：

static MyType* ObjectName_OperateAttribute( XXXX *object, XXXX arg1, XXXX arg2, XXXX arg3 )

应该改成：

static MyType* 
ObjectName_OperateAttribute( XXXX *object, XXXX arg1, XXXX arg2, XXXX arg3 )

如果改了后还是比较长的话，可以对参数列表进行修改：

static MyType* 
ObjectName_OperateAttribute(	XXXX *object,	XXXX arg1,
				XXXX arg2,	XXXX arg3 )

使用制表符缩进，调整参数列表中的参数的位置。

如果还是比较长的话，可以将函数名中的单词改成缩写，或者使用其它意思相同但比较短的单词，
像这样：
static MyType* 
ObjName_OptAttr(	XXXX *object, XXXX arg1,
			XXXX arg2, XXXX arg3 )

调用函数时，代码应该类似于这样：

func( arg1, arg2, arg3, arg4 );

参数列表前后都要空格，参数列表中的逗号后面也需要加个空格。还可以这样：

func (arg1, arg2, arg3, arg4);

函数名与()之间空一格。




