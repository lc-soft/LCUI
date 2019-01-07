#include <wchar.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/util/logger.h>
#include <LCUI/util/charset.h>
#include "test.h"

int test_charset(void)
{
	int ret = 0;
	size_t len;
	char str[64];
	wchar_t wcs[64];

	len = LCUI_DecodeUTF8String(wcs, "hello", 64);
	CHECK_WITH_TEXT("test decode ascii string",
			len == 5 && wcscmp(wcs, L"hello") == 0);

	len = LCUI_DecodeUTF8String(wcs, "简体中文", 64);
	CHECK_WITH_TEXT("test decode utf-8 string",
			len == 4 && wcscmp(wcs, L"简体中文") == 0);

	len = LCUI_EncodeUTF8String(str, L"hello", 64);
	CHECK_WITH_TEXT("test encode ascii string to utf-8",
			len == 5 && strcmp(str, "hello") == 0);

	len = LCUI_EncodeUTF8String(str, L"简体中文", 64);
	CHECK_WITH_TEXT(
	    "test encode unicode string to utf-8",
	    len == strlen("简体中文") && strcmp(str, "简体中文") == 0);

	return ret;
}

#ifdef PREVIEW_MODE

int tests_count = 0;

int main(void)
{
	test_charset();
}

#endif
