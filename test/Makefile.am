AUTOMAKE_OPTIONS=foreign
SUBDIRS=mora screenlock clock login
##设定在编译时头文件的查找位置
INCLUDES = -I$(top_builddir)/include
##需要编译的测试程序, noinst指的是不安装
noinst_PROGRAMS = helloworld test_font test_radiobtn test_checkbox test_progressbar \
test_button test_label test_loading ts_calibrate test_viewimage test_lcui_graph_mix \
test_window test_rect_cut test_gaussian_blur test_widget_dock  test_widget_style\
test_timer test_textbox test_scrollbar graph2array test_graphlayer \
test_messagebox

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

test_font_SOURCES = test_font.c
test_font_LDADD   = $(top_builddir)/src/libLCUI.la

test_viewimage_SOURCES = test_viewimage.c
test_viewimage_LDADD   = $(top_builddir)/src/libLCUI.la

test_lcui_graph_mix_SOURCES = test_lcui_graph_mix.c
test_lcui_graph_mix_LDADD   = $(top_builddir)/src/libLCUI.la

test_window_SOURCES	= test_window.c
test_window_LDADD	= $(top_builddir)/src/libLCUI.la

test_rect_cut_SOURCES	= test_rect_cut.c
test_rect_cut_LDADD	= $(top_builddir)/src/libLCUI.la

test_gaussian_blur_SOURCES	= test_gaussian_blur.c
test_gaussian_blur_LDADD	= $(top_builddir)/src/libLCUI.la

test_widget_dock_SOURCES	= test_widget_dock.c
test_widget_dock_LDADD		= $(top_builddir)/src/libLCUI.la

test_timer_SOURCES	= test_timer.c
test_timer_LDADD	= $(top_builddir)/src/libLCUI.la

test_textbox_SOURCES	= test_textbox.c
test_textbox_LDADD	= $(top_builddir)/src/libLCUI.la

test_scrollbar_SOURCES	= test_scrollbar.c
test_scrollbar_LDADD	= $(top_builddir)/src/libLCUI.la

graph2array_SOURCES	= graph2array.c
graph2array_LDADD	= $(top_builddir)/src/libLCUI.la

test_graphlayer_SOURCES	= test_graphlayer.c
test_graphlayer_LDADD	= $(top_builddir)/src/libLCUI.la

test_messagebox_SOURCES	= test_messagebox.c
test_messagebox_LDADD	= $(top_builddir)/src/libLCUI.la

test_widget_style_SOURCES = test_widget_style.c
test_widget_style_LDADD = $(top_builddir)/src/libLCUI.la
