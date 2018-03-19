AUTOMAKE_OPTIONS=foreign
##设定在编译时头文件的查找位置
AM_CFLAGS = -I$(top_builddir)/include $(CODE_COVERAGE_CFLAGS)
##需要编译的测试程序, noinst指的是不安装
noinst_PROGRAMS = helloworld test test_touch test_char_render \
test_string_render test_widget_render test_widget_layout test_widget_flex_layout \
test_widget_inline_block_layout test_scaling_support

##指定测试程序的源码文件
helloworld_SOURCES = helloworld.c
##指定测试程序编译时需要链接的库
helloworld_LDADD = $(top_builddir)/src/libLCUI.la

test_SOURCES = test.c test_css_parser.c test_xml_parser.c test_string.c \
test_font_load.c test_image_reader.c test_widget_rect.c test_widget_layout.c \
test_widget_flex_layout.c test_widget_inline_block_layout.c test_thread.c

test_LDADD   = $(top_builddir)/src/libLCUI.la -lm $(CODE_COVERAGE_LIBS)

test_touch_SOURCES = test_touch.c
test_touch_LDADD = $(top_builddir)/src/libLCUI.la

test_char_render_SOURCES = test_char_render.c
test_char_render_LDADD = $(top_builddir)/src/libLCUI.la

test_string_render_SOURCES = test_string_render.c
test_string_render_LDADD = $(top_builddir)/src/libLCUI.la

test_widget_render_SOURCES = test_widget_render.c
test_widget_render_LDADD = $(top_builddir)/src/libLCUI.la

test_widget_layout_SOURCES = test_widget_layout.c
test_widget_layout_LDADD = $(top_builddir)/src/libLCUI.la
test_widget_layout_CFLAGS = $(AM_CFLAGS) -D PREVIEW_MODE

test_widget_flex_layout_SOURCES = test_widget_flex_layout.c
test_widget_flex_layout_LDADD = $(top_builddir)/src/libLCUI.la
test_widget_flex_layout_CFLAGS = $(AM_CFLAGS) -D PREVIEW_MODE

test_widget_inline_block_layout_SOURCES = test_widget_inline_block_layout.c
test_widget_inline_block_layout_LDADD = $(top_builddir)/src/libLCUI.la
test_widget_inline_block_layout_CFLAGS = $(AM_CFLAGS) -D PREVIEW_MODE

test_scaling_support_SOURCES = test_scaling_support.c
test_scaling_support_LDADD = $(top_builddir)/src/libLCUI.la

@CODE_COVERAGE_RULES@

