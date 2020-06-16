#include <wchar.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/util/logger.h>
#include <LCUI/util/charset.h>
#include "test.h"
#include "libtest.h"

void test_charset(void)
{
	size_t len;
	char str[64];
	wchar_t wcs[64];

	len = LCUI_DecodeUTF8String(wcs, "hello", 64);
	it_b("test decode ascii string", len == 5 && wcscmp(wcs, L"hello") == 0,
	     TRUE);
#ifdef WIN32
	len = LCUI_DecodeString(wcs, "简体中文", 64, ENCODING_ANSI);
	it_b("test decode ansi string",
	     len == 4 && wcscmp(wcs, L"简体中文") == 0, TRUE);

	len = LCUI_EncodeString(str, L"简体中文", 64, ENCODING_ANSI);
	it_b("test encode unicode string to ansi",
	     len == strlen("简体中文") && strcmp(str, "简体中文") == 0, TRUE);
#else
	len = LCUI_DecodeUTF8String(wcs, "简体中文", 64);
	it_b("test decode utf-8 string",
	     len == 4 && wcscmp(wcs, L"简体中文") == 0, TRUE);

	len = LCUI_EncodeUTF8String(str, L"简体中文", 64);
	it_b("test encode unicode string to utf-8",
	     len == strlen("简体中文") && strcmp(str, "简体中文") == 0, TRUE);
#endif
	len = LCUI_EncodeUTF8String(str, L"hello", 64);
	it_b("test encode ascii string to utf-8",
	     len == 5 && strcmp(str, "hello") == 0, TRUE);
}

#ifdef PREVIEW_MODE

int main(void)
{
	test_charset();
}

#endif
