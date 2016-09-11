AUTOMAKE_OPTIONS=foreign
SUBDIRS=font draw gui util
##一些需要安装的头文件
# Headers which are installed to support the library
INSTINCLUDES=LCUI.h config.h display.h graph.h draw.h font.h surface.h ime.h \
input.h thread.h util.h timer.h main.h cursor.h
EXTRA_DIST=platform.h platform/linux/linux_display.h \
platform/linux/linux_events.h platform/linux/linux_mouse.h \
platform/linux/linux_keyboard.h platform/linux/linux_fbdisplay.h \
platform/linux/linux_x11display.h platform/linux/linux_x11events.h \
platform/linux/linux_x11mouse.h platform/windows/windows_display.h \
platform/windows/windows_events.h platform/windows/windows_mouse.h \
platform/windows/windows_keyboard.h

# Headers to install
pkginclude_HEADERS = $(INSTINCLUDES)
pkgincludedir=$(prefix)/include/LCUI/
