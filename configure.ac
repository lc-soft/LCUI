#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#
# 具体用法，可参考Automake的中文文档：
# http://www.linuxforum.net/books/autoconf.html
# 也可以到这看英文版的：
# http://www.gnu.org/savannah-checkouts/gnu/autoconf/manual/autoconf-2.69/html_node/index.html

AC_PREREQ([2.68])
AC_INIT(LCUI, 0.12.6, lc-soft@live.cn)

# Check system type
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([include/LCUI_Build.h])
AC_CONFIG_HEADERS([config.h])

# Make --enable-silent-rules the default.
# To get verbose build output you may configure
# with --disable-silent-rules or use "make V=1".
# 默认情况下，是 Make --enable-silent-rules， 也就是启用无声规则，make时输出的是精简的信息。
# 要获得详细的编译输出，你可以在运行configurue时加上--disable-silent-rules参数或使用"make V=1".
AM_SILENT_RULES([yes])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
# 检测LCUI所依赖的库是否存在并且能正常使用
want_iconv=no
AC_ARG_WITH(iconv, [  --with-iconv             use libiconv		(default)])
if test "x$with_iconv" != "xno"; then
    AC_CHECK_LIB([iconv], [iconv_open], want_iconv=yes, want_iconv=no)
    if test "x$want_iconv" != "xno"; then
	ICONV_LIBS=-liconv
	AC_SUBST(ICONV_LIBS)
    fi
fi

want_thread=no
thread_name=pthread
AC_ARG_WITH(pthread, [  --with-pthread             use pthread		(default)])
if test "x$with_pthread" != "xno"; then
    AC_CHECK_LIB([pthread], [pthread_create], want_thread=yes, want_thread=no)
    if test "x$want_thread" != "xno"; then
	THREAD_LIBS=-lpthread
	AC_CHECK_HEADERS([pthread.h],want_thread=yes,want_thread=no)
	AC_SUBST(THREAD_LIBS)
    fi
fi

want_freetype=no
AC_ARG_WITH(freetype, [  --with-freetype             use freetype		(default)])
if test "x$with_freetype" != "xno"; then
    AC_CHECK_LIB([freetype], [FT_Init_FreeType], want_freetype=yes, want_freetype=no)
    if test "x$want_freetype" != "xno"; then
	FREETYPE_LIBS=-lfreetype
	AC_CHECK_HEADERS([ft2build.h],want_freetype=yes,want_freetype=no)
	AC_SUBST(FREETYPE_LIBS)
    fi
fi

want_ts=no
AC_ARG_WITH(ts, [  --with-ts             use tslib		(default)])
if test "x$with_ts" != "xno"; then
    AC_CHECK_LIB([ts], [ts_open], want_ts=yes, want_ts=no)
    if test "x$want_ts" != "xno"; then
	TS_LIBS=-lts
	AC_SUBST(TS_LIBS)
    fi
fi

want_png=no
AC_ARG_WITH(png, [  --with-png              use libpng		(default)])
if test "x$with_png" != "xno"; then
    AC_CHECK_LIB([png], [png_sig_cmp],want_png=yes,want_png=no)
    if test "x$want_png" != "xno"; then
	PNG_LIBS=-lpng
	AC_CHECK_HEADERS([png.h],want_png=yes,want_png=no)
	AC_SUBST(PNG_LIBS)
    fi
fi

want_jpeg=no
AC_ARG_WITH(jpeg, [  --with-jpeg             use libjpeg		(default)])
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
AC_MSG_CHECKING(whether to enable debugging)
debug_default="no"
AC_ARG_ENABLE(debug, [  --enable-debug=[no/yes] turn on debugging
[default=$debug_default]],, enable_debug=$debug_default)
# 看到CFLAGS，你应该懂的！是给gcc编译器的参数。
if test "x$enable_debug" = "xyes"; then
CFLAGS="-O2 -g -Wall -D _GNU_SOURCE"
CXXFLAG="-O2 -g -Wall -D _GNU_SOURCE"
AC_MSG_RESULT(yes)
else
CFLAGS="-O2 -Wall -D _GNU_SOURCE"
CXXFLAGS="-O2 -Wall -D _GNU_SOURCE"
AC_MSG_RESULT(no)
fi

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h limits.h malloc.h memory.h stddef.h stdlib.h string.h strings.h sys/ioctl.h termio.h unistd.h wchar.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_HEADER_STDBOOL
AC_C_RESTRICT
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset munmap select strcasecmp sysinfo])

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

if test "$want_freetype" = "yes"; then
    AC_DEFINE_UNQUOTED(USE_FREETYPE, 1,  Define to 1 if you have the FreeType font engine)
fi

if test "$want_iconv" = "yes"; then
    AC_DEFINE_UNQUOTED(USE_LIBICONV, 1, Define to 1 if you have the libiconv)
fi

if test "$want_thread" = "yes"; then
    if test "$thread_name" = "pthread"; then
        AC_DEFINE_UNQUOTED(USE_PTHREAD, 1, Define to 1 if you select pthread for thread support)
    else 
        thread_name=none
    fi
else 
    thread_name=none
fi
##输出文件
AC_OUTPUT([Makefile
                 lcui.pc
                 include/Makefile
                 include/LCUI/Makefile
                 src/Makefile
                 src/font/Makefile
                 src/font/in-core/Makefile
                 src/draw/Makefile
                 src/draw/in-core/Makefile
                 src/thread/Makefile
                 src/bmp/Makefile
                 src/misc/Makefile
                 src/input/Makefile
                 src/output/Makefile
                 src/widget/Makefile
                 test/Makefile
                 test/mora/Makefile
                 test/photoviewer/Makefile
                 test/clock/Makefile
                 test/screenlock/Makefile])
echo
echo
echo -e "Build with tslib support ........ : $want_ts"
echo -e "Build with libpng support ....... : $want_png"
echo -e "Build with libjpeg support ...... : $want_jpeg"
echo -e "Build with libiconv support ..... : $want_iconv"
echo -e "Build with freetype support ..... : $want_freetype"
echo -e "Build with thread support ....... : $thread_name"
echo 

