AUTOMAKE_OPTIONS=foreign
##设定在编译时头文件的查找位置
INCLUDES = -I$(top_builddir)/include
##需要编译的测试程序, noinst指的是不安装
noinst_PROGRAMS = helloworld test_radiobtn test_checkbox test_progressbar \
test_button test_label test_loading ts_calibrate test_viewimage
EXTRA_DIST=README image.jpg logo_128x128.png border.png cross.png ring.png bg.png

##指定测试程序的源码文件
helloworld_SOURCES = helloworld.c
##指定测试程序编译时需要链接的库
helloworld_LDADD   = $(top_builddir)/src/libLCUI.la

##同上
test_progressbar_SOURCES = test_progressbar.c
test_progressbar_LDADD   = $(top_builddir)/src/libLCUI.la


test_label_SOURCES = test_label.c
test_label_LDADD   = $(top_builddir)/src/libLCUI.la


test_loading_SOURCES = test_loading.c
test_loading_LDADD   = $(top_builddir)/src/libLCUI.la


test_button_SOURCES = test_button.c
test_button_LDADD   = $(top_builddir)/src/libLCUI.la

test_radiobtn_SOURCES = test_radiobtn.c
test_radiobtn_LDADD   = $(top_builddir)/src/libLCUI.la

test_checkbox_SOURCES = test_checkbox.c
test_checkbox_LDADD   = $(top_builddir)/src/libLCUI.la

ts_calibrate_SOURCES = ts_calibrate.c
ts_calibrate_LDADD   = $(top_builddir)/src/libLCUI.la


test_viewimage_SOURCES = test_viewimage.c
test_viewimage_LDADD   = $(top_builddir)/src/libLCUI.la

