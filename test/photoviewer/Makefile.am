AUTOMAKE_OPTIONS=foreign
##设定在编译时头文件的查找位置
INCLUDES = -I$(top_builddir)/include
##需要编译的测试程序, noinst指的是不安装
noinst_PROGRAMS = photoviewer

photoviewer_SOURCES = photoviewer.c 
photoviewer_LDADD   = $(top_builddir)/src/libLCUI.la 
