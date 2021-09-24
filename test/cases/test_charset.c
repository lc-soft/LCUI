#include <wchar.h>
#include <string.h>
#include <LCUI_Build.h>
#include <yutil.h>
#include "ctest.h"

void test_charset(void)
{
	size_t len;
	char str[64];
	wchar_t wcs[64];

	len = decode_utf8(wcs, "hello", 64);
	it_b("test decode ascii string", len == 5 && wcscmp(wcs, L"hello") == 0,
	     TRUE);
#ifdef _WIN32
	len = decode_string(wcs, "简体中文", 64, ENCODING_ANSI);
	test_msg("test decode ansi string ... (%s)\n",
		 len == 4 && wcscmp(wcs, L"简体中文") == 0 ? "true" : "false");

	len = encode_string(str, L"简体中文", 64, ENCODING_ANSI);
	test_msg("test encode unicode string to ansi ... (%s)\n",
		 len == strlen("简体中文") && strcmp(str, "简体中文") == 0
		     ? "true"
		     : "false");
#else
	len = decode_utf8(wcs, "简体中文", 64);
	it_b("test decode utf-8 string",
	     len == 4 && wcscmp(wcs, L"简体中文") == 0, TRUE);

	len = encode_utf8(str, L"简体中文", 64);
	it_b("test encode unicode string to utf-8",
	     len == strlen("简体中文") && strcmp(str, "简体中文") == 0, TRUE);
#endif
	len = encode_utf8(str, L"hello", 64);
	it_b("test encode ascii string to utf-8",
	     len == 5 && strcmp(str, "hello") == 0, TRUE);
}
