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
AC_PROG_CC_STDC
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET
AC_PROG_LN_S
AC_PROG_RANLIB
AC_PROG_INSTALL
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_AWK 

# Checks for libraries.
# 检测LCUI所依赖的库是否存在并且能正常使用
# 在检测不到可用的依赖库时，会调用AC_MSG_ERROR宏来打印错误信息
AC_CHECK_LIB(iconv, iconv_open)
AC_CHECK_LIB(pthread, pthread_create,,AC_MSG_ERROR(pthread is not found!) )
AC_CHECK_LIB(png, png_create_read_struct,,AC_MSG_ERROR(libpng is not found!))
AC_CHECK_LIB(freetype, FT_Init_FreeType,,AC_MSG_ERROR(freetype is not found!))
AC_CHECK_LIB(jpeg, jpeg_read_header,,AC_MSG_ERROR(libjpeg is not found!))
AC_CHECK_LIB(ts, ts_open,,AC_MSG_ERROR(tslib is not found!))

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

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset munmap select strcasecmp sysinfo])
##输出文件
AC_OUTPUT([Makefile
                 include/Makefile
                 include/LCUI/Makefile
                 src/Makefile
                 test/Makefile
                 test/mora/Makefile
                 test/photoviewer/Makefile
                 test/clock/Makefile
                 test/screenlock/Makefile])
