/*
 * encoding.c -- String encoding/decoding utilities.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn>
 * Copyright (c) 2021, Li Zihao <yidianyiko@foxmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <wchar.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "yutil/keywords.h"
#include "yutil/types.h"
#include "yutil/encoding.h"

/* clang-format off */

#define MAX_SAVE_NUM 20

#define UNI_SUR_HIGH_START  0xD800
#define UNI_SUR_HIGH_END    0xDBFF
#define UNI_SUR_LOW_START   0xDC00
#define UNI_SUR_LOW_END     0xDFFF

#define UNICODE_UTF8_4 0x1fffff

/* clang-format on */

#ifdef _WIN32
#include <windows.h>

static size_t win32_encode(char *str, const wchar_t *wcs, size_t maxlen,
                           int codepage)
{
        size_t bytes;

        bytes = WideCharToMultiByte(codepage, 0, wcs, -1, str, (int)maxlen,
                                    NULL, NULL);
        if (bytes > 0 && bytes < maxlen) {
                bytes -= 1;
        }
        return bytes;
}

static size_t win32_decode(wchar_t *wcs, const char *str, size_t maxlen,
                           int codepage)
{
        size_t len;

        len = MultiByteToWideChar(codepage, 0, str, -1, wcs, (int)maxlen);
        if (len > 0 && len < maxlen) {
                len -= 1;
        }
        return len;
}

size_t decode_ansi(wchar_t *wstr, const char *str, size_t max_len)
{
        return win32_decode(wstr, str, max_len, CP_ACP);
}

size_t encode_ansi(char *str, const wchar_t *wstr, size_t max_len)
{
        return win32_encode(str, wstr, max_len, CP_ACP);
}
#endif

static size_t utf8_to_ucs2(const char *utf8, wchar_t *ucs2)
{
        size_t i, count;
        wchar_t unicode;
        const unsigned char *byte;
        unsigned char buf[MAX_SAVE_NUM] = { 0 };

        byte = (unsigned char *)utf8;
        if ((*byte >> 7) == 0) {
                *ucs2 = *byte;
                return 1;
        }
        if ((*byte >> 5) == 6) {
                count = 2;
        } else if ((*byte >> 4) == 14) {
                count = 3;
        } else if ((*byte >> 3) == 30) {
                count = 4;
        } else if ((*byte >> 2) == 62) {
                count = 5;
        } else if ((*byte >> 1) == 126) {
                count = 6;
        } else {
                return 0;
        }
        for (i = 0; i < count; ++i) {
                buf[i] = *byte++;
        }
        unicode = buf[0];
        if (unicode >= 0xF0) {
                unicode = (wchar_t)(buf[0] & 0x07) << 18;
                unicode |= (wchar_t)(buf[1] & 0x3F) << 12;
                unicode |= (wchar_t)(buf[2] & 0x3F) << 6;
                unicode |= (wchar_t)(buf[3] & 0x3F);
        } else if (unicode >= 0xE0) {
                unicode = (wchar_t)(buf[0] & 0x0F) << 12;
                unicode |= (wchar_t)(buf[1] & 0x3F) << 6;
                unicode |= (wchar_t)(buf[2] & 0x3F);
        } else if (unicode >= 0xC0) {
                unicode = (wchar_t)(buf[0] & 0x1F) << 6;
                unicode |= (wchar_t)(buf[1] & 0x3F);
        }
        *ucs2 = unicode;
        return count;
}

static size_t ucs2_to_utf8(int32_t ucs2, unsigned char *utf8)
{
        utf8[0] = 0;
        if (ucs2 < 0x80) {
                utf8[0] = ucs2;
                utf8[1] = '\0';
                return 1;
        }
        if (ucs2 < 0x800) {
                utf8[0] = (ucs2 >> 6) | 0xC0;
                utf8[1] = (ucs2 & 0x3F) | 0x80;
                utf8[2] = '\0';
                return 2;
        }
        if (ucs2 < 0xFFFF) {
                utf8[0] = ((ucs2 >> 12)) | 0xE0;
                utf8[1] = ((ucs2 >> 6) & 0x3F) | 0x80;
                utf8[2] = ((ucs2) & 0x3F) | 0x80;
                utf8[3] = '\0';
                if (ucs2 >= UNI_SUR_HIGH_START && ucs2 <= UNI_SUR_LOW_END) {
                        return 0;
                }
                return 3;
        }
        if (ucs2 <= UNICODE_UTF8_4) {
                utf8[0] = 0xF0 | (ucs2 >> 18);
                utf8[1] = 0x80 | ((ucs2 >> 12) & 0x3F);
                utf8[2] = 0x80 | ((ucs2 >> 6) & 0x3F);
                utf8[3] = 0x80 | ((ucs2 & 0x3F));
                utf8[4] = '\0';
                return 4;
        }
        return 0;
}

size_t decode_utf8(wchar_t *wstr, const char *str, size_t max_len)
{
        size_t n;
        size_t count = 0;
        wchar_t buf;
        wchar_t *wp = wstr;
        const char *p = str;

        if (wstr) {
                while (*p) {
                        count += 1;
                        if (count > max_len) {
                                count -= 1;
                                break;
                        }
                        n = utf8_to_ucs2(p, wp);
                        if (n > 0) {
                                p += n;
                                ++wp;
                        } else {
                                ++p;
                        }
                }
                wstr[count] = 0;
        } else {
                while (*p) {
                        n = utf8_to_ucs2(p, &buf);
                        if (n > 0) {
                                p += n;
                        } else {
                                ++p;
                        }
                        count += 1;
                }
        }
        return count;
}

size_t encode_utf8(char *str, const wchar_t *wstr, size_t max_len)
{
        size_t n;
        size_t count = 0;
        char *p = str;
        unsigned char buf[MAX_SAVE_NUM] = { 0 };
        const wchar_t *wp = wstr;

        if (str) {
                while (*wp) {
                        n = ucs2_to_utf8(*wp, buf);
                        count += n;
                        if (count > max_len) {
                                count -= n;
                                break;
                        }
                        strcpy(p, (char *)buf);
                        p += n;
                        ++wp;
                }
                str[count] = 0;
        } else {
                while (*wp) {
                        count += ucs2_to_utf8(*wp, buf);
                        ++wp;
                }
        }
        return count;
}
