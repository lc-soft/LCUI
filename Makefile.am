AUTOMAKE_OPTIONS=foreign
SUBDIRS=src build include test
ACLOCAL_AMFLAGS=-I m4
pkgconfig_DATA = $(PACKAGE).pc

EXTRA_DIST=autogen.sh README README.zh-cn.md README.md LICENSE.TXT \
CHANGELOG.md \
CHANGELOG.zh-cn.md \
debian/compat \
debian/liblcui1-dev.install \
debian/control \
debian/copyright \
debian/source \
debian/source/format \
debian/rules \
debian/changelog \
debian/liblcui1-dev.dirs \
debian/lcui-docs.docs \
debian/liblcui1.install \
debian/liblcui1.dirs \
docs/CodingStyle.zh-cn.md \
test/valgrind-suppressions.txt \
test/README.zh-cn \
test/Makefile.in \
test/Makefile.am \
test/helloworld.c \
test/helloworld.css \
test/helloworld.xml \
test/test.c \
test/test.h \
test/libtest.c \
test/libtest.h \
test/test_render.c \
test/test_touch.c \
test/test_string.c \
test/test_object.c \
test/test_thread.c \
test/test_linkedlist.c \
test/test_string_render.c \
test/test_widget_render.c \
test/test_char_render.c \
test/test_css_parser.css \
test/test_css_parser.xml \
test/test_css_parser.c \
test/test_xml_parser.xml \
test/test_xml_parser.nested.xml \
test/test_xml_parser.c \
test/test_font_load.c \
test/test_font_load.css \
test/test_font_load.ttf \
test/test_image_reader.c \
test/test_image_reader.bmp \
test/test_image_reader.jpg \
test/test_image_reader.png \
test/test_scrollbar.c \
test/test_scrollbar.xml \
test/test_widget.c \
test/test_widget_opacity.c \
test/test_widget_opacity.css \
test/test_widget_opacity.xml \
test/test_block_layout.c \
test/test_block_layout.css \
test/test_block_layout.xml \
test/test_block_layout.html \
test/test_widget_rect.c \
test/test_widget_event.c \
test/test_textview_resize.c \
test/test_textedit.c

.PHONY: test

test:
	cd test && make && ../libtool --mode=execute valgrind --suppressions="valgrind-suppressions.txt" --leak-check=full --error-exitcode=42 ./test

@CODE_COVERAGE_RULES@
