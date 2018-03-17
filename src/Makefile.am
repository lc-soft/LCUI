AUTOMAKE_OPTIONS=foreign
##设定在编译时头文件的查找位置
AM_CFLAGS = -I$(abs_top_srcdir)/include $(CODE_COVERAGE_CFLAGS)

LCUI_LDFLAGS = -version-info 1:0:0
LCUI_SOURCES = graph.c ime.c cursor.c worker.c main.c timer.c display.c keyboard.c
LCUI_LIBADD = thread/libthread.la util/libutil.la platform/libplatform.la \
image/libimage.la draw/libdraw.la gui/libgui.la font/libfont.la \
font/in-core/libfont_incore.la  $(LCUI_LIBS)

SUBDIRS = image util draw font thread gui platform

##库的编译需要这样
lib_LTLIBRARIES = libLCUI.la
##库的安装位置
libLCUIdir = $(prefix)/lib
##库的其它子库
libLCUI_la_LIBADD = $(LCUI_LIBADD) $(CODE_COVERAGE_LIBS)
##库的源文件
libLCUI_la_SOURCES = $(LCUI_SOURCES)
##编译的附加选项
libLCUI_la_LDFLAGS = $(LCUI_LDFLAGS)

