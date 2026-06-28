#include <wchar.h>
#include <string.h>
#include "test.h"
#include "libtest.h"
#include "yutil/keywords.h"
#include "yutil/encoding.h"

static void test_decode_utf8_ascii(void)
{
        wchar_t wcs[64];
        size_t len;

        len = decode_utf8(wcs, "hello", 64);
        it_i("should return 5 for ascii string", (int)len, 5);
        it_b("should null-terminate at len", wcs[len] == 0, true);
        it_b("should match expected string", wcscmp(wcs, L"hello") == 0, true);
}

static void test_decode_utf8_multibyte(void)
{
        wchar_t wcs[64];
        size_t len;

        len = decode_utf8(wcs, u8"简体中文", 64);
        it_i("should return 4 for 4 CJK characters", (int)len, 4);
        it_b("should null-terminate at len", wcs[len] == 0, true);
        it_b("should match expected string", wcscmp(wcs, L"简体中文") == 0,
             true);
}

static void test_decode_utf8_mixed(void)
{
        wchar_t wcs[64];
        size_t len;

        len = decode_utf8(wcs, u8"a简b", 64);
        it_i("should return 3 for mixed ascii+CJK", (int)len, 3);
        it_b("should null-terminate at len", wcs[len] == 0, true);
        it_b("should match expected string", wcscmp(wcs, L"a简b") == 0, true);
}

static void test_decode_utf8_truncate(void)
{
        wchar_t wcs[8];
        size_t len;

        len = decode_utf8(wcs, "hello", 3);
        it_i("should return 3 when truncated by max_len 3", (int)len, 3);
        it_b("should always null-terminate", wcs[len] == 0, true);
        it_b("should contain truncated content",
             wcs[0] == 'h' && wcs[1] == 'e' && wcs[2] == 'l', true);

        len = decode_utf8(wcs, "hello", 4);
        it_i("should return 4 when truncated by max_len 4", (int)len, 4);
        it_b("should always null-terminate", wcs[len] == 0, true);
        it_b("should contain truncated content",
             wcs[0] == 'h' && wcs[1] == 'e' && wcs[2] == 'l' && wcs[3] == 'l',
             true);
}

static void test_decode_utf8_exactly_fills(void)
{
        wchar_t wcs[8];
        size_t len;

        memset(wcs, 0xFF, sizeof(wcs));
        len = decode_utf8(wcs, "hello", 5);
        it_i("should return 5 when exactly fills max_len", (int)len, 5);
        it_b("should write null at wstr[5]", wcs[5] == 0, true);
        it_b("should not overwrite wstr[6]", wcs[6] == 0xFFFF, true);
}

static void test_decode_utf8_null_termination(void)
{
        wchar_t wcs[8];
        size_t len;

        memset(wcs, 0xFF, sizeof(wcs));
        len = decode_utf8(wcs, "hi", 2);
        it_b("should always null-terminate when fits", wcs[len] == 0, true);

        memset(wcs, 0xFF, sizeof(wcs));
        len = decode_utf8(wcs, "hello", 3);
        it_b("should always null-terminate when truncated", wcs[len] == 0,
             true);

        memset(wcs, 0xFF, sizeof(wcs));
        len = decode_utf8(wcs, "", 4);
        it_b("should always null-terminate for empty string", wcs[len] == 0,
             true);
}

static void test_decode_utf8_null_buffer(void)
{
        size_t len;

        len = decode_utf8(NULL, "hello", 0);
        it_i("should pre-calculate 5 for ascii", (int)len, 5);

        len = decode_utf8(NULL, u8"简体中文", 0);
        it_i("should pre-calculate 4 for CJK", (int)len, 4);

        len = decode_utf8(NULL, u8"a简b", 0);
        it_i("should pre-calculate 3 for mixed", (int)len, 3);
}

static void test_decode_utf8_empty(void)
{
        wchar_t wcs[8];
        size_t len;

        len = decode_utf8(wcs, "", 8);
        it_i("should return 0 for empty string", (int)len, 0);
        it_b("should null-terminate at len", wcs[0] == 0, true);
}

static void test_encode_utf8_ascii(void)
{
        char str[64];
        size_t len;

        len = encode_utf8(str, L"hello", 64);
        it_i("should return 5 for ascii string", (int)len, 5);
        it_b("should null-terminate at len", str[len] == 0, true);
        it_b("should match expected string", strcmp(str, "hello") == 0, true);
}

static void test_encode_utf8_multibyte(void)
{
        char str[64];
        size_t len;

        len = encode_utf8(str, L"简体中文", 64);
        it_i("should return 12 for 4 CJK characters (3 bytes each)", (int)len,
             12);
        it_b("should null-terminate at len", str[len] == 0, true);
        it_b("should match expected string", strcmp(str, u8"简体中文") == 0,
             true);
}

static void test_encode_utf8_truncate(void)
{
        char str[8];
        size_t len;

        len = encode_utf8(str, L"hello world", 7);
        it_i("should return 7 when truncated by max_len 7", (int)len, 7);
        it_b("should always null-terminate", str[len] == 0, true);
        it_b("should contain truncated content",
             strncmp(str, "hello w", 7) == 0, true);
}

static void test_encode_utf8_exactly_fills(void)
{
        unsigned char str[8];
        size_t len;

        memset(str, 0xAA, sizeof(str));
        len = encode_utf8((char *)str, L"hello", 5);
        it_i("should return 5 when exactly fills max_len", (int)len, 5);
        it_b("should write null at str[5]", str[5] == 0, true);
        it_b("should not overwrite str[6]", str[6] == 0xAA, true);
}

static void test_encode_utf8_null_termination(void)
{
        char str[8];
        size_t len;

        memset(str, 0xFF, sizeof(str));
        len = encode_utf8(str, L"hi", 2);
        it_b("should always null-terminate when fits", str[len] == 0, true);

        memset(str, 0xFF, sizeof(str));
        len = encode_utf8(str, L"hello", 3);
        it_b("should always null-terminate when truncated", str[len] == 0,
             true);

        memset(str, 0xFF, sizeof(str));
        len = encode_utf8(str, L"", 4);
        it_b("should always null-terminate for empty string", str[len] == 0,
             true);
}

static void test_encode_utf8_null_buffer(void)
{
        size_t len;

        len = encode_utf8(NULL, L"hello", 0);
        it_i("should pre-calculate 5 for ascii", (int)len, 5);

        len = encode_utf8(NULL, L"简体中文", 0);
        it_i("should pre-calculate 12 for CJK", (int)len, 12);
}

static void test_encode_utf8_empty(void)
{
        char str[8];
        size_t len;

        len = encode_utf8(str, L"", 8);
        it_i("should return 0 for empty string", (int)len, 0);
        it_b("should null-terminate at len", str[0] == 0, true);
}

static void test_decode_encode_ansi(void)
{
#ifdef _WIN32
#ifndef CI_ENV
        wchar_t wcs[64];
        char str[64];
        size_t len;

        len = decode_ansi(wcs, "简体中文", 64);
        it_b("should decode ansi string",
             (int)len == 4 && wcscmp(wcs, L"简体中文") == 0, true);
        it_b("should null-terminate at len", wcs[len] == 0, true);

        len = encode_ansi(str, L"简体中文", 64);
        it_b(
            "should encode unicode string to ansi",
            (int)len == (int)strlen("简体中文") && strcmp(str, "简体中文") == 0,
            true);
        it_b("should null-terminate at len", str[len] == 0, true);
#endif
#endif
}

void test_encoding(void)
{
        describe("decode utf8 ascii", test_decode_utf8_ascii);
        describe("decode utf8 multibyte", test_decode_utf8_multibyte);
        describe("decode utf8 mixed", test_decode_utf8_mixed);
        describe("decode utf8 truncate", test_decode_utf8_truncate);
        describe("decode utf8 exactly fills", test_decode_utf8_exactly_fills);
        describe("decode utf8 null termination",
                 test_decode_utf8_null_termination);
        describe("decode utf8 null buffer", test_decode_utf8_null_buffer);
        describe("decode utf8 empty", test_decode_utf8_empty);
        describe("encode utf8 ascii", test_encode_utf8_ascii);
        describe("encode utf8 multibyte", test_encode_utf8_multibyte);
        describe("encode utf8 truncate", test_encode_utf8_truncate);
        describe("encode utf8 exactly fills", test_encode_utf8_exactly_fills);
        describe("encode utf8 null termination",
                 test_encode_utf8_null_termination);
        describe("encode utf8 null buffer", test_encode_utf8_null_buffer);
        describe("encode utf8 empty", test_encode_utf8_empty);
        describe("decode encode ansi", test_decode_encode_ansi);
}
