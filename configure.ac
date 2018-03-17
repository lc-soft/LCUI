#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(LCUI, 1.0.0-beta.2, lc-soft@live.cn)

# Silence warning: ar: 'u' modifier ignored since 'D' is the default
AC_SUBST(AR_FLAGS, [cr])

# Check system type
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([include/LCUI_Build.h])
AC_CONFIG_HEADERS([include/LCUI/config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])

# Checks for header files.
AC_PATH_X
AC_CHECK_HEADERS([limits.h locale.h stdint.h stdlib.h string.h sys/time.h unistd.h wchar.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gettimeofday memset pow select sqrt strcasecmp strstr])

# Make --enable-silent-rules the default.
# To get verbose build output you may configure
# with --disable-silent-rules or use "make V=1".
AM_SILENT_RULES([yes])

AX_CODE_COVERAGE

LCUI_LIBS=""
CFLAGS="-Wall -D _GNU_SOURCE"
CXXFLAG="-Wall -D _GNU_SOURCE"
export C_INCLUDE_PATH=$C_INCLUDE_PATH:/usr/include/libxml2:/usr/include/freetype2/

# 检测是否启用线程支持
want_thread=no
thread_name=pthread
AC_ARG_WITH(pthread, AC_HELP_STRING([--with-pthread], [use pthread (default)]))
if test "x$with_pthread" != "xno"; then
	AX_PTHREAD([
		want_thread=yes
		CC="$PTHREAD_CC"
		CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
		LCUI_LIBS="$LCUI_LIBS $PTHREAD_LIBS"
		AC_DEFINE_UNQUOTED([LCUI_THREAD_PTHREAD], 1, [Define to 1 if you are using pthread support.])
	], [AC_MSG_ERROR([The support could not be configured for the POSIX thread programming interface.])])
fi

# 检测是否有libxml2库，必须有 libxml2 库才能启用 LCUIBuilder 功能模块
enable_builder=yes
AC_ARG_ENABLE(lcui-builder, AC_HELP_STRING([--enable-lcui-builder],
[enable lcui-builder function module [[default=yes]]]), [enable_builder=$enableval])
if test "$enable_builder" = "yes"; then
	AC_CHECK_HEADERS([libxml/parser.h],[
		AC_CHECK_LIB([xml2], [xmlParseFile], [
			enable_builder=yes
			LCUI_LIBS="$LCUI_LIBS -lxml2"
			CFLAGS="$CFLAGS -I/usr/include/libxml2"
			AC_DEFINE_UNQUOTED([USE_LCUI_BUILDER], 1, [Define to 1 if you enabled LCUIBuilder function module.])
		], [enable_builder=no])
	], [enable_builder=no])
fi

# 检测是否有freetype库
want_font_engine=yes
font_engine_name=none
AC_ARG_ENABLE(font-engine, AC_HELP_STRING([--enable-font-engine],
[turn on font-engine [[default=yes]]]), [want_font_engine=$enableval])
if test "$want_font_engine" = yes; then
	AC_CHECK_HEADERS([ft2build.h], [
		AC_CHECK_LIB([freetype], [FT_Init_FreeType], [
			want_font_engine=yes
			font_engine_name=freetype2
			LCUI_LIBS="$LCUI_LIBS `pkg-config --libs freetype2`"
			CFLAGS="$CFLAGS `pkg-config --cflags-only-I freetype2`"
			AC_DEFINE_UNQUOTED([LCUI_FONT_ENGINE_FREETYPE], 1, [Define to 1 if you have the FreeType font engine.])
		], [want_font_engine=no])
	], [want_font_engine=no])
else
	want_font_engine=no
fi

# 检测图形输出设备
want_video_output=yes
video_driver_name=framebuffer
AC_ARG_ENABLE(video-output, AC_HELP_STRING([--enable-video-output],
[turn on video-output [[default=yes]]]), [want_video_output=$enableval])
if test "$want_video_output" = yes; then
	AC_CHECK_HEADERS([linux/fb.h],[
		video_driver_name=framebuffer
		AC_DEFINE_UNQUOTED([LCUI_VIDEO_DRIVER_FRAMEBUFFER], 1, [Define to 1 if you select FrameBuffer for video support.])
	], [video_driver_name=none])
	AC_CHECK_HEADERS([X11/Xlib.h],[
		AC_CHECK_LIB([X11], [XOpenDisplay], [
			video_driver_name=x11
			LCUI_LIBS="$LCUI_LIBS `pkg-config --libs x11`"
			CFLAGS="$CFLAGS `pkg-config --cflags-only-I x11`"
			AC_DEFINE_UNQUOTED([LCUI_VIDEO_DRIVER_X11], 1, [Define to 1 if you select XWindow for video support.])
		], [])
	], [])
else
	want_video_output=no
	video_driver_name=none
fi

want_png=no
AC_ARG_WITH(png, AC_HELP_STRING([--with-png], [use libpng (default)]))
if test "x$with_png" != "xno"; then
	AC_CHECK_HEADERS([png.h],[
		AC_CHECK_LIB([png], [png_sig_cmp],[
			want_png=yes
			LCUI_LIBS="$LCUI_LIBS `pkg-config --libs libpng`"
			CFLAGS="$CFLAGS `pkg-config --cflags-only-I libpng`"
			AC_DEFINE_UNQUOTED([USE_LIBPNG], 1, [Define to 1 if you have the libpng.])
		], [want_png=no])
	], [want_png=no])
fi

want_jpeg=no
AC_ARG_WITH(jpeg, AC_HELP_STRING([--with-jpeg], [use libjpeg (default)]))
if test "x$with_jpeg" != "xno"; then
	AC_CHECK_HEADERS([jpeglib.h setjmp.h],[
		AC_CHECK_LIB([jpeg], [jpeg_start_decompress],[
			want_jpeg=yes
			LCUI_LIBS="$LCUI_LIBS -ljpeg"
			AC_DEFINE_UNQUOTED([USE_LIBJPEG], 1, [Define to 1 if you have the libjpeg])
		], [want_jpeg=no])
	], [want_jpeg=no])
fi

# 调试选项，如果需要对项目进行调试，可启用调试选项
want_debug=no
AC_MSG_CHECKING(whether to enable debugging)
AC_ARG_ENABLE(debug, AC_HELP_STRING([--enable-debug],
[turn on debugging [[default=no]]]), want_debug=$enableval)
if test "$want_debug" = "yes"; then
	CFLAGS="$CFLAGS -g"
	CXXFLAG="$CXXFLAGS -g"
	AC_MSG_RESULT(yes)
else
	CFLAGS="$CFLAGS -O2"
	CXXFLAG="$CXXFLAGS -O2"
	AC_MSG_RESULT(no)
fi

# 输出变量到每个Makefile中
AC_SUBST(LCUI_LIBS)
AC_SUBST(CFLAGS)

# 输出文件
AC_OUTPUT([Makefile
	 lcui.pc
	 build/Makefile
	 include/Makefile
	 include/LCUI/Makefile
	 include/LCUI/font/Makefile
	 include/LCUI/draw/Makefile
	 include/LCUI/gui/widget/Makefile
	 include/LCUI/gui/Makefile
	 include/LCUI/util/Makefile
	 src/Makefile
	 src/font/Makefile
	 src/font/in-core/Makefile
	 src/draw/Makefile
	 src/thread/Makefile
	 src/image/Makefile
	 src/util/Makefile
	 src/gui/Makefile
	 src/platform/Makefile
	 test/Makefile])
echo
echo
echo -e "Build with lcui-builder support .... : $enable_builder"
echo -e "Build with libpng support .......... : $want_png"
echo -e "Build with libjpeg support ......... : $want_jpeg"
echo -e "Build with font-engine support ..... : $font_engine_name"
echo -e "Build with thread support .......... : $thread_name"
echo -e "Build with video support ........... : $video_driver_name"
echo

