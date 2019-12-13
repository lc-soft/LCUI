AUTOMAKE_OPTIONS=foreign
AM_CFLAGS = -I$(abs_top_srcdir)/include $(CODE_COVERAGE_CFLAGS)

LCUI_LDFLAGS = -version-info 1:1:1
LCUI_SOURCES = graph.c ime.c cursor.c worker.c main.c timer.c painter.c display.c keyboard.c
LCUI_LIBADD = thread/libthread.la util/libutil.la platform/libplatform.la \
image/libimage.la draw/libdraw.la gui/libgui.la font/libfont.la \
font/in-core/libfont_incore.la $(PACKAGE_LIBS)

SUBDIRS = image util draw font thread gui platform

lib_LTLIBRARIES = libLCUI.la
libLCUIdir = $(prefix)/lib
libLCUI_la_LIBADD = $(LCUI_LIBADD) $(CODE_COVERAGE_LIBS)
libLCUI_la_SOURCES = $(LCUI_SOURCES)
libLCUI_la_LDFLAGS = $(LCUI_LDFLAGS)

