AUTOMAKE_OPTIONS=foreign
##设定在编译时头文件的查找位置
INCLUDES = -I../include
##以下是给Libtool的参数
LDFLAGS=-version-info 1:1:0
##以下内容摘自相关文章。
##Libtool 的版本号分为 3 个部分 :
##current: 表示当前库输出的接口的数量。
##revision: 表示当前库输出接口的修改次数。
##age: 表示当前库支持先前的库接口的数量。
##Libtool 的库版本通过参数 -version-info current:revision:age指定。
##如果没有指定，默认版本是 0.0.0。
##注意，应该尽可能少的更新库版本号，尤其是不能强行将库版本号和软件发行号保持一致，下面是更新库版本号的几个策略 :
##如果修改了库的源代码，那么应该增加 revision。这是当前接口的新的修订版本。
##如果改变了接口，应该增加 current，将 revision重置为 0。这是接口的新版本。
##如果新接口是前面接口的超集 ( 前面的接口还是可用 )，那么应该增加 age。这是一个向后兼容的版本。
##如果新接口删除了前面接口的元素，那么应该将 age重置为 0。这是一个新的，但是不向后兼容的版本。

##库的编译需要这样
libLCUI_LTLIBRARIES=libLCUI.la
##库的安装位置
libLCUIdir=$(prefix)/lib
##库的源文件
libLCUI_la_SOURCES=LCUI_Main.c \
LCUI_Queue.c \
LCUI_Font.c \
LCUI_Memory.c \
LCUI_Thread.c \
LCUI_Input.c \
LCUI_Graphics.c \
LCUI_Work.c \
LCUI_Cursor.c \
LCUI_Widget.c \
LCUI_Misc.c \
LCUI_Button.c \
LCUI_Error.c \
LCUI_Label.c \
LCUI_PictureBox.c \
LCUI_ProgressBar.c \
LCUI_Resources.c \
LCUI_Window.c \
LCUI_Menu.c \
LCUI_CheckBox.c \
LCUI_RadioButton.c


