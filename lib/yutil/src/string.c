/* string.c -- The string operation set.
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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include "yutil/keywords.h"
#include "yutil/types.h"
#include "yutil/string.h"
#include "yutil/strlist.h"
#include "yutil/math.h"

size_t y_strlower(char *outstr, const char *instr)
{
        char *op = outstr;
        const char *ip = instr;
        for (; *ip; ++ip, ++op) {
                *op = (char)tolower(*ip);
        }
        *op = 0;
        return ip - instr;
}

char *y_strdup(const char *str)
{
        size_t len = strlen(str) + 1;
        char *out = (char *)malloc(sizeof(char) * len);
        if (!out) {
                return NULL;
        }
        strcpy(out, str);
        return out;
}

wchar_t *y_wcsdup(const wchar_t *str)
{
        size_t len = wcslen(str) + 1;
        wchar_t *out = (wchar_t *)malloc(sizeof(wchar_t) * len);
        if (!out) {
                return NULL;
        }
        wcscpy(out, str);
        return out;
}

size_t y_strtrim(char *outstr, const char *instr, const char *charlist)
{
        bool clear, clear_left = true;
        char *op = outstr, *last_blank = NULL;
        const char *default_charlist = "\t\n\r ", *ip = instr, *c;

        if (!charlist) {
                charlist = default_charlist;
        }
        for (; *ip; ip++) {
                for (clear = false, c = charlist; *c; ++c) {
                        if (*ip == *c) {
                                clear = true;
                                break;
                        }
                }
                if (clear) {
                        if (!clear_left) {
                                *op = *ip;
                                if (!last_blank) {
                                        last_blank = op;
                                }
                                ++op;
                        }
                        continue;
                }
                if (clear_left) {
                        clear_left = false;
                }
                last_blank = NULL;
                *op = *ip;
                ++op;
        }
        if (last_blank) {
                *last_blank = 0;
        }
        *op = 0;
        return op - outstr;
}

size_t y_wcstrim(wchar_t *outstr, const wchar_t *instr, const wchar_t *charlist)
{
        bool clear, clear_left = true;
        wchar_t *op = outstr, *last_blank = NULL;
        const wchar_t *default_charlist = L"\t\n\r ", *ip = instr, *c;

        if (!charlist) {
                charlist = default_charlist;
        }
        for (; *ip; ip++) {
                for (clear = false, c = charlist; *c; ++c) {
                        if (*ip == *c) {
                                clear = true;
                                break;
                        }
                }
                if (clear) {
                        if (!clear_left) {
                                *op = *ip;
                                if (!last_blank) {
                                        last_blank = op;
                                }
                                ++op;
                        }
                        continue;
                }
                if (clear_left) {
                        clear_left = false;
                }
                last_blank = NULL;
                *op = *ip;
                ++op;
        }
        if (last_blank) {
                *last_blank = 0;
        }
        *op = 0;
        return op - outstr;
}

unsigned y_strhash(unsigned hash, const char *str)
{
        const unsigned char *p = (unsigned char *)str;

        while (*p) {
                hash = ((hash << 5) + hash) + (*p++);
        }
        return hash;
}

size_t y_strreplace(char *str, size_t max_len, const char *substr,
                    const char *newstr)
{
        size_t len, buf_len;
        char *buf, *pout, *pin;

        pin = strstr(str, substr);
        if (!pin) {
                return 0;
        }
        len = strlen(newstr);
        buf_len = strlen(str) + len + 1;
        buf = (char *)malloc(buf_len * sizeof(wchar_t));
        strcpy(buf, str);
        pout = buf + (pin - str);
        strcpy(pout, newstr);
        pin += strlen(substr);
        pout += len;
        strcpy(pout, pin);
        buf[buf_len - 1] = 0;
        strncpy(str, buf, max_len);
        free(buf);
        return y_min(buf_len, max_len);
}

int y_strsplit(const char *instr, const char *sep, char ***outstrs)
{
        int i = 0;
        const char *prev = instr;
        size_t len, sep_len = strlen(sep);
        char *next = (char *)strstr(prev, sep);
        char **newstrs = NULL;

        while (1) {
                char **tmp, *str;
                if (next) {
                        len = next - prev + 1;
                } else {
                        len = strlen(prev) + 1;
                }
                str = (char *)malloc(sizeof(char) * len);
                tmp = (char **)realloc(newstrs, sizeof(char *) * (i + 2));
                if (!tmp) {
                        strlist_free(newstrs);
                        return 0;
                }
                newstrs = tmp;
                strncpy(str, prev, len - 1);
                str[len - 1] = 0;
                newstrs[i] = str;
                newstrs[i + 1] = NULL;
                if (next) {
                        prev = next + sep_len;
                } else {
                        break;
                }
                next = (char *)strstr(prev, sep);
                i += 1;
        }
        *outstrs = newstrs;
        return i + 1;
}
