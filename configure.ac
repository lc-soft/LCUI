#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#
# 具体用法，可参考Automake的中文文档：
# http://www.linuxforum.net/books/autoconf.html
# 也可以到这看英文版的：
# http://www.gnu.org/savannah-checkouts/gnu/autoconf/manual/autoconf-2.69/html_node/index.html

AC_PREREQ([2.68])
AC_INIT(LCUI, 1.0.0, lc-soft@live.cn)

# Check system type
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([include/LCUI_Build.h])
AC_CONFIG_HEADERS([include/LCUI/config.h])

# Make --enable-silent-rules the default.
# To get verbose build output you may configure
# with --disable-silent-rules or use "make V=1".
# 默认情况下，是 Make --enable-silent-rules， 也就是启用无声规则，make时输出的是精简的信息。
# 要获得详细的编译输出，你可以在运行configurue时加上--disable-silent-rules参数或使用"make V=1".
AM_SILENT_RULES([yes])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h limits.h malloc.h memory.h stddef.h stdlib.h string.h strings.h sys/ioctl.h termio.h unistd.h wchar.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_HEADER_STDBOOL
AC_C_RESTRICT
AC_TYPE_UINT32_T

# Checks for library functions.
AC_CHECK_FUNCS([memset munmap select strcasecmp sysinfo])

# Checks for libraries.
# 检测LCUI所依赖的库是否存在并且能正常使用
want_iconv=no
AC_ARG_WITH(iconv, [  --with-iconv            use libiconv	(default)])
if test "x$with_iconv" != "xno"; then
    AC_CHECK_LIB([iconv], [iconv_open], want_iconv=yes, want_iconv=no)
    if test "x$want_iconv" != "xno"; then
	ICONV_LIBS=-liconv
	AC_SUBST(ICONV_LIBS)
    fi
fi
##检测是否启用线程支持
want_thread=no
thread_name=pthread
AC_ARG_WITH(pthread, [  --with-pthread          use pthread	(default)])
if test "x$with_pthread" != "xno"; then
    ##检测是否有pthread函数库，测试调用pthread_create函数，若成功则want_thread=yes，否则want_thread=no
    AC_CHECK_LIB([pthread], [pthread_create], want_thread=yes, want_thread=no)
    if test "x$want_thread" != "xno"; then
	THREAD_LIBS=-lpthread
	##检测pthread.h头文件
	AC_CHECK_HEADERS([pthread.h],want_thread=yes,want_thread=no)
	##让THREAD_LIBS变量能够在每个Makefile中能用到
	AC_SUBST(THREAD_LIBS)
    fi
fi

##检测是否有freetype库
want_font_engine="yes"
font_engine_name="freetype"
AC_ARG_ENABLE(font-engine, AC_HELP_STRING([--enable-font-engine],
[turn on font-engine [[default=yes]]]), want_font_engine=$enableval)
if test "$want_font_engine" = "yes"; then
	AC_CHECK_LIB([freetype], [FT_Init_FreeType], want_font_engine="yes", want_font_engine="no")
	if test "$want_font_engine" = "yes"; then
		AC_CHECK_HEADERS([ft2build.h],
		[want_font_engine="yes"], [want_font_engine="no"])
	fi
	if test "$want_font_engine" = "yes"; then
		FONT_LIBS=-lfreetype
		AC_SUBST(FONT_LIBS)
	else
		want_font_engine="no"
		font_engine_name="none"
	fi
else
	want_font_engine="no"
	font_engine_name="none"
fi

## 检测图形输出设备
want_video_output=yes
video_driver_name="framebuffer"
AC_ARG_ENABLE(video-output, AC_HELP_STRING([--enable-video-output],
[turn on video-output [[default=yes]]]), want_video_output=$enableval)
if test "$want_video_output" = "yes"; then
	AC_CHECK_HEADERS([linux/fb.h],
	[video_driver_name="framebuffer"], [video_driver_name="none"])
else
	want_video_output=no
	video_driver_name=none
fi

want_ts=no
AC_ARG_WITH(ts, [  --with-ts               use tslib	(default)])
if test "x$with_ts" != "xno"; then
    AC_CHECK_LIB([ts], [ts_open], want_ts=yes, want_ts=no)
    if test "x$want_ts" != "xno"; then
	TS_LIBS=-lts
	AC_SUBST(TS_LIBS)
    fi
fi

want_png=no
AC_ARG_WITH(png, [  --with-png              use libpng	(default)])
if test "x$with_png" != "xno"; then
    AC_CHECK_LIB([png], [png_sig_cmp],want_png=yes,want_png=no)
    if test "x$want_png" != "xno"; then
	PNG_LIBS=-lpng
	AC_CHECK_HEADERS([png.h],want_png=yes,want_png=no)
	AC_SUBST(PNG_LIBS)
    fi
fi

want_jpeg=no
AC_ARG_WITH(jpeg, [  --with-jpeg             use libjpeg	(default)])
if test "x$with_jpeg" != "xno"; then
    AC_CHECK_LIB([jpeg], [jpeg_start_decompress],want_jpeg=yes,want_jpeg=no)
    if test "x$want_jpeg" != "xno"; then
	JPEG_LIBS=-ljpeg
	AC_CHECK_HEADERS([jpeglib.h setjmp.h],want_jpeg=yes,want_jpeg=no)
	AC_SUBST(JPEG_LIBS)
    fi
fi


# DEBUG option
# 调试选项，如果需要对项目进行调试，可启用调试选项
want_debug=no
AC_MSG_CHECKING(whether to enable debugging)
AC_ARG_ENABLE(debug, AC_HELP_STRING([--enable-debug],
[turn on debugging [[default=no]]]), want_debug=$enableval)
if test "$want_debug" = "yes"; then
	CFLAGS="-O2 -g -Wall -D _GNU_SOURCE -finput-charset=UTF-8"
	CXXFLAG="-O2 -g -Wall -D _GNU_SOURCE -finput-charset=UTF-8"
	AC_MSG_RESULT(yes)
else
	CFLAGS="-O2 -Wall -D _GNU_SOURCE -finput-charset=UTF-8"
	CXXFLAGS="-O2 -Wall -D _GNU_SOURCE -finput-charset=UTF-8"
	AC_MSG_RESULT(no)
fi

#根据之前的检测结果，定义相应宏
if test "$want_png" = "yes"; then
    AC_DEFINE_UNQUOTED(USE_LIBPNG, 1, Define to 1 if you have the libpng)
fi

if test "$want_jpeg" = "yes"; then
    AC_DEFINE_UNQUOTED(USE_LIBJPEG, 1, Define to 1 if you have the libjpeg)
fi

if test "$want_ts" = "yes"; then
    AC_DEFINE_UNQUOTED(USE_TSLIB, 1, Define to 1 if you have the tslib)
fi

font_engine_name="none"
if test "$want_font_engine" = "yes"; then
    font_engine_name="freetype"
    AC_DEFINE_UNQUOTED(LCUI_FONT_ENGINE_FREETYPE, 1,  Define to 1 if you have the FreeType font engine)

fi

if test "$want_iconv" = "yes"; then
    AC_DEFINE_UNQUOTED(USE_LIBICONV, 1, Define to 1 if you have the libiconv)
fi

if test "$want_thread" = "yes"; then
    if test "$thread_name" = "pthread"; then
	AC_DEFINE_UNQUOTED(LCUI_THREAD_PTHREAD, 1, Define to 1 if you using pthread support)
    else
        AC_DEFINE_UNQUOTED(LCUI_THREAD_NONE, 1, Define to 1 if not have thread support)
    fi
else
    AC_DEFINE_UNQUOTED(LCUI_THREAD_NONE, 1, Define to 1 if not have thread support)
fi

if test "$want_video_output" = "yes"; then
    if test "$video_driver_name" = "framebuffer"; then
        AC_DEFINE_UNQUOTED(LCUI_VIDEO_DRIVER_FRAMEBUFFER, 1, Define to 1 if you select FrameBuffer for video support)
    else
        AC_DEFINE_UNQUOTED(LCUI_VIDEO_DRIVER_NONE, 1, Define to 1 if not have video output support)
    fi
else
    AC_DEFINE_UNQUOTED(LCUI_VIDEO_DRIVER_NONE, 1, Define to 1 if not have video output support)
fi

##输出文件
AC_OUTPUT([Makefile
                 lcui.pc
                 build/Makefile
                 include/Makefile
                 include/LCUI/Makefile
                 include/LCUI/font/Makefile
                 include/LCUI/draw/Makefile
                 include/LCUI/gui/widget/Makefile
                 include/LCUI/kernel/Makefile
                 include/LCUI/gui/Makefile
                 include/LCUI/misc/Makefile
                 src/Makefile
                 src/font/Makefile
                 src/font/in-core/Makefile
                 src/draw/Makefile
                 src/thread/Makefile
                 src/thread/win32/Makefile
                 src/thread/pthread/Makefile
                 src/bmp/Makefile
                 src/misc/Makefile
                 src/kernel/Makefile
                 src/input/Makefile
                 src/output/Makefile
                 src/gui/Makefile
                 test/Makefile
                 test/mora/Makefile
                 test/clock/Makefile
                 test/login/Makefile
                 test/screenlock/Makefile])
echo
echo
echo -e "Build with tslib support ........... : $want_ts"
echo -e "Build with libpng support .......... : $want_png"
echo -e "Build with libjpeg support ......... : $want_jpeg"
echo -e "Build with libiconv support ........ : $want_iconv"
echo -e "Build with font-engine support ..... : $font_engine_name"
echo -e "Build with thread support .......... : $thread_name"
echo -e "Build with video support ........... : $video_driver_name"
echo

