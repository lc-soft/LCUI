AUTOMAKE_OPTIONS=foreign
##设定在编译时头文件的查找位置
AM_CFLAGS = -I$(abs_top_srcdir)/include
##以下是给Libtool的参数
LCUI_LDFLAGS = -version-info 3:0:0
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

LCUI_SOURCES = \
	LCUI_Queue.c		\
	LCUI_Graph.c		\
	LCUI_GraphLayer.c	\
	LCUI_StyleLibrary.c	\
	LCUI_InputMethod.c	\
	LCUI_Cursor.c

LCUI_LIBADD = \
	bmp/libbmp.la	\
	draw/libdraw.la \
	misc/libmisc.la \
	kernel/libkernel.la \
	font/libfont.la \
	font/in-core/libfont_incore.la \
	input/libinput.la \
	output/liboutput.la \
	thread/libthread.la \
	thread/win32/libthread.la \
	thread/pthread/libthread.la \
	gui/libgui.la \
	$(JPEG_LIBS) \
	$(PNG_LIBS) \
	$(THREAD_LIBS) \
	$(FONT_LIBS) \
	$(ICONV_LIBS) \
	$(TS_LIBS)

SUBDIRS = bmp misc draw kernel font input output thread gui

##库的编译需要这样
lib_LTLIBRARIES = libLCUI.la
##库的安装位置
libLCUIdir = $(prefix)/lib
##库的其它子库
libLCUI_la_LIBADD = $(LCUI_LIBADD)
##库的源文件
libLCUI_la_SOURCES = $(LCUI_SOURCES)
##编译的附加选项
libLCUI_la_LDFLAGS = $(LCUI_LDFLAGS)
