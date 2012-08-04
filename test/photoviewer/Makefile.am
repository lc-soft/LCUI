AUTOMAKE_OPTIONS=foreign
##设定在编译时头文件的查找位置
INCLUDES = -I$(top_builddir)/include
##需要编译的测试程序, noinst指的是不安装
noinst_PROGRAMS = photoviewer
EXTRA_DIST= drawable/btn_prev_down_pressed.png  \
drawable/btn_zoom_up_pressed.png \
drawable/btn_next_up_disabled.png \
drawable/btn_zoom_down_disabled.png  \
drawable/icon.png \
drawable/btn_next_up_normal.png  \
drawable/btn_zoom_down_normal.png \
drawable/pic_fail.png \
drawable/btn_next_up_pressed.png \
drawable/btn_zoom_down_pressed.png \
drawable/pic_loading.png \
drawable/btn_prev_down_disabled.png \
drawable/btn_zoom_up_disabled.png \
drawable/btn_prev_down_normal.png \
drawable/btn_zoom_up_normal.png

photoviewer_SOURCES = photoviewer.c 
photoviewer_LDADD   = $(top_builddir)/src/libLCUI.la 
