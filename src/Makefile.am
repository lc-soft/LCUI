AUTOMAKE_OPTIONS=foreign
##设定在编译时头文件的查找位置
AM_CFLAGS = -I$(abs_top_srcdir)/include
##以下是给Libtool的参数
LCUI_LDFLAGS = -version-info 3:0:0
LCUI_SOURCES = graph.c ime.c cursor.c
LCUI_LIBADD = \
	misc/libmisc.la \
	thread/libthread.la \
	kernel/libkernel.la \
	font/libfont.la \
	font/in-core/libfont_incore.la \
	input/libinput.la \
	output/liboutput.la \
	bmp/libbmp.la	\
	draw/libdraw.la \
	gui/libgui.la \
	$(LCUI_LIBS)

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
