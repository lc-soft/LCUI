AUTOMAKE_OPTIONS=subdir-objects foreign
##设定在编译时头文件的查找位置
AM_CFLAGS = -I$(top_srcdir)/include $(CODE_COVERAGE_CFLAGS)
##需要编译的测试程序, noinst指的是不安装
noinst_PROGRAMS = helloworld test test_charset test_touch test_char_render \
test_string_render test_widget_render test_render test_widget_opacity \
test_scaling_support test_widget test_scrollbar test_textview_resize \
test_image_scaling_bench test_block_layout test_flex_layout test_fill_rect \
test_fill_rect_with_rgba test_pixel_manipulation test_paint_background \
test_paint_border test_paint_boxshadow test_mix_rect_with_opacity

##指定测试程序的源码文件
helloworld_SOURCES = helloworld.c
##指定测试程序编译时需要链接的库
helloworld_LDADD = $(top_builddir)/src/libLCUI.la

test_SOURCES = test.c \
libtest.c \
test_mainloop.c \
test_charset.c \
test_string.c \
test_strpool.c \
test_linkedlist.c \
test_object.c \
test_thread.c \
test_font_load.c \
test_css_parser.c \
test_xml_parser.c \
test_image_reader.c \
test_block_layout.c \
test_flex_layout.c \
test_widget_rect.c \
test_widget_opacity.c \
test_widget_event.c \
test_textview_resize.c \
test_textedit.c \
test_settings.c \
test_scrollbar.c

test_LDADD = $(top_builddir)/src/libLCUI.la -lm $(CODE_COVERAGE_LIBS)

test_touch_SOURCES = test_touch.c
test_touch_LDADD = $(top_builddir)/src/libLCUI.la

test_char_render_SOURCES = test_char_render.c
test_char_render_LDADD = $(top_builddir)/src/libLCUI.la

test_string_render_SOURCES = test_string_render.c
test_string_render_LDADD = $(top_builddir)/src/libLCUI.la

test_widget_render_SOURCES = test_widget_render.c
test_widget_render_LDADD = $(top_builddir)/src/libLCUI.la

test_block_layout_SOURCES = test_block_layout.c libtest.c
test_block_layout_LDADD = $(top_builddir)/src/libLCUI.la
test_block_layout_CFLAGS = $(AM_CFLAGS) -D PREVIEW_MODE

test_flex_layout_SOURCES = test_flex_layout.c libtest.c
test_flex_layout_LDADD = $(top_builddir)/src/libLCUI.la
test_flex_layout_CFLAGS = $(AM_CFLAGS) -D PREVIEW_MODE

test_charset_SOURCES = test_charset.c libtest.c
test_charset_LDADD = $(top_builddir)/src/libLCUI.la
test_charset_CFLAGS = $(AM_CFLAGS) -D PREVIEW_MODE

test_textview_resize_SOURCES = test_textview_resize.c libtest.c
test_textview_resize_LDADD = $(top_builddir)/src/libLCUI.la
test_textview_resize_CFLAGS = $(AM_CFLAGS) -D PREVIEW_MODE

test_widget_opacity_SOURCES = test_widget_opacity.c libtest.c
test_widget_opacity_LDADD = $(top_builddir)/src/libLCUI.la
test_widget_opacity_CFLAGS = $(AM_CFLAGS) -D PREVIEW_MODE

test_scaling_support_SOURCES = test_scaling_support.c
test_scaling_support_LDADD = $(top_builddir)/src/libLCUI.la

test_scrollbar_SOURCES = test_scrollbar.c libtest.c
test_scrollbar_LDADD = $(top_builddir)/src/libLCUI.la
test_scrollbar_CFLAGS = $(AM_CFLAGS) -D PREVIEW_MODE

test_widget_SOURCES = test_widget.c
test_widget_LDADD = $(top_builddir)/src/libLCUI.la

test_render_SOURCES = test_render.c
test_render_LDADD = $(top_builddir)/src/libLCUI.la

test_image_scaling_bench_LDADD = $(top_builddir)/src/libLCUI.la

test_pixel_manipulation_SOURCES = test_pixel_manipulation.c
test_pixel_manipulation_LDADD = $(top_builddir)/src/libLCUI.la

test_paint_background_SOURCES = test_paint_background.c
test_paint_background_LDADD = $(top_builddir)/src/libLCUI.la

test_paint_border_SOURCES = test_paint_border.c
test_paint_border_LDADD = $(top_builddir)/src/libLCUI.la

test_paint_boxshadow_SOURCES = test_paint_boxshadow.c
test_paint_boxshadow_LDADD = $(top_builddir)/src/libLCUI.la

test_mix_rect_with_opacity_SOURCES = test_mix_rect_with_opacity.c
test_mix_rect_with_opacity_LDADD = $(top_builddir)/src/libLCUI.la

test_fill_rect_SOURCES = test_fill_rect.c
test_fill_rect_LDADD = $(top_builddir)/src/libLCUI.la

test_fill_rect_with_rgba_SOURCES = test_fill_rect_with_rgba.c
test_fill_rect_with_rgba_LDADD = $(top_builddir)/src/libLCUI.la

@CODE_COVERAGE_RULES@
