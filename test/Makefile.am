AUTOMAKE_OPTIONS=foreign
##设定在编译时头文件的查找位置
AM_CFLAGS = -I$(top_builddir)/include
##需要编译的测试程序, noinst指的是不安装
noinst_PROGRAMS = helloworld test

##指定测试程序的源码文件
helloworld_SOURCES = helloworld.c
##指定测试程序编译时需要链接的库
helloworld_LDADD   = $(top_builddir)/src/libLCUI.la -lm

test_SOURCES = test.c test_css_parser.c test_string.c test_char_render.c test_string_render.c test_widget_render.c test_image_reader.c test_widget_layout.c
test_LDADD   = $(top_builddir)/src/libLCUI.la -lm
