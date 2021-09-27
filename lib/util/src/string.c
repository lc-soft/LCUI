﻿/* string.c -- The string operation set.
 *
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
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
#include <errno.h>
#include <ctype.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>

#define STRTRIM_CODE(TYPE, OUTSTR, INSTR, CHARLIST, DCHARLIST)     \
	LCUI_BOOL clear, clear_left = TRUE;                        \
	TYPE *op = OUTSTR, *last_blank = NULL;                     \
	const TYPE *default_charlist = DCHARLIST, *ip = INSTR, *c; \
                                                                   \
	if (!CHARLIST) {                                           \
		CHARLIST = default_charlist;                       \
	}                                                          \
	for (; *ip; ip++) {                                        \
		for (clear = FALSE, c = CHARLIST; *c; ++c) {       \
			if (*ip == *c) {                           \
				clear = TRUE;                      \
				break;                             \
			}                                          \
		}                                                  \
		if (clear) {                                       \
			if (!clear_left) {                         \
				*op = *ip;                         \
				if (!last_blank) {                 \
					last_blank = op;           \
				}                                  \
				++op;                              \
			}                                          \
			continue;                                  \
		}                                                  \
		if (clear_left) {                                  \
			clear_left = FALSE;                        \
		}                                                  \
		last_blank = NULL;                                 \
		*op = *ip;                                         \
		++op;                                              \
	}                                                          \
	if (last_blank) {                                          \
		*last_blank = 0;                                   \
	}                                                          \
	*op = 0;                                                   \
	return op - outstr;

size_t strsize(const char *str)
{
	if (!str) {
		return sizeof(char);
	}
	return (strlen(str) + 1) * sizeof(char);
}

size_t wcssize(const wchar_t *str)
{
	if (!str) {
		return sizeof(wchar_t);
	}
	return (wcslen(str) + 1) * sizeof(wchar_t);
}

size_t strtolower(char *outstr, const char *instr)
{
	char *op = outstr;
	const char *ip = instr;
	for (; *ip; ++ip, ++op) {
		*op = tolower(*ip);
	}
	*op = 0;
	return ip - instr;
}

size_t strntolower(char *outstr, size_t max_len, const char *instr)
{
	char *op = outstr;
	const char *ip = instr;
	for (; *ip && max_len > 1; ++ip, ++op, --max_len) {
		*op = tolower(*ip);
	}
	*op = 0;
	return ip - instr;
}

char *strdup2(const char *str)
{
	size_t len = strlen(str) + 1;
	char *out = malloc(sizeof(char) * len);
	if (!out) {
		return NULL;
	}
	strcpy(out, str);
	return out;
}

wchar_t *wcsdup2(const wchar_t *str)
{
	size_t len = wcslen(str) + 1;
	wchar_t *out = malloc(sizeof(wchar_t) * len);
	if (!out) {
		return NULL;
	}
	wcscpy(out, str);
	return out;
}

size_t strtrim(char *outstr, const char *instr, const char *charlist)
{
	STRTRIM_CODE(char, outstr, instr, charlist, "\t\n\r ");
}

size_t wcstrim(wchar_t *outstr, const wchar_t *instr, const wchar_t *charlist)
{
	STRTRIM_CODE(wchar_t, outstr, instr, charlist, L"\t\n\r ");
}

unsigned strhash(unsigned hash, const char *str)
{
	const unsigned char *p = (unsigned char *)str;

	while (*p) {
		hash = ((hash << 5) + hash) + (*p++);
	}
	return hash;
}

size_t strreplace(char *str, size_t max_len, const char *substr,
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
	buf = malloc(buf_len * sizeof(wchar_t));
	strcpy(buf, str);
	pout = buf + (pin - str);
	strcpy(pout, newstr);
	pin += strlen(substr);
	pout += len;
	strcpy(pout, pin);
	buf[buf_len - 1] = 0;
	strncpy(str, buf, max_len);
	free(buf);
	return min(buf_len, max_len);
}

size_t wcsreplace(wchar_t *str, size_t max_len, const wchar_t *substr,
		  const wchar_t *newstr)
{
	size_t len, buf_len;
	wchar_t *buf, *pout, *pin;

	pin = wcsstr(str, substr);
	if (!pin) {
		return 0;
	}
	len = wcslen(newstr);
	buf_len = wcslen(str) + len + 1;
	buf = malloc(buf_len * sizeof(wchar_t));
	wcscpy(buf, str);
	pout = buf + (pin - str);
	wcscpy(pout, newstr);
	pin += wcslen(substr);
	pout += len;
	wcscpy(pout, pin);
	buf[buf_len - 1] = 0;
	wcsncpy(str, buf, max_len);
	free(buf);
	return min(buf_len, max_len);
}

int cmdsplit(const char *cmd, char ***outargv)
{
	size_t size;
	char **argv = NULL, **tmp;
	const char *cur = cmd, *p = NULL;
	int argc = 0, spaces = 0, qoutes = 0, len = 0;

	if (outargv) {
		argv = malloc(sizeof(char *));
		if (!argv) {
			return -ENOMEM;
		}
		argv[0] = NULL;
	}
	while (1) {
		switch (*cur) {
		case 0:
		case '\n':
		case '\r':
		case '\t':
		case ' ':
			if (*cur != 0) {
				if (spaces != 0) {
					break;
				}
				++spaces;
				if (qoutes > 0 && qoutes % 2 != 0) {
					++len;
					break;
				}
			}
			if (argv && len > 0) {
				size = sizeof(char *) * (argc + 2);
				tmp = realloc(argv, size);
				if (!tmp) {
					goto faild;
				}
				len += 1;
				argv = tmp;
				argv[argc + 1] = NULL;
				size = sizeof(char) * len;
				argv[argc] = malloc(size);
				if (!argv[argc]) {
					goto faild;
				}
				len -= 1;
				strncpy(argv[argc], p, len);
				argv[argc][len] = 0;
			}
			p = NULL;
			if (len > 0) {
				++argc;
			}
			qoutes = 0;
			len = 0;
			if (*cur == 0) {
				goto done;
			}
			break;
		case '"':
			++qoutes;
		default:
			if (!p) {
				p = cur;
			}
			spaces = 0;
			++len;
			break;
		}
		++cur;
	}

done:
	if (argv) {
		*outargv = argv;
	}
	return argc;

faild:
	argc += 1;
	while (argc-- > 0) {
		if (argv[argc]) {
			free(argv[argc]);
			argv[argc] = NULL;
		}
	}
	free(argv);
	return -ENOMEM;
}

int strsplit(const char *instr, const char *sep, char ***outstrs)
{
	int i = 0;
	const char *prev = instr;
	size_t len, sep_len = strlen(sep);
	char *next = strstr(prev, sep);
	char **newstrs = NULL;

	while (1) {
		char **tmp, *str;
		if (next) {
			len = next - prev + 1;
		} else {
			len = strlen(prev) + 1;
		}
		str = malloc(sizeof(char) * len);
		tmp = realloc(newstrs, sizeof(char *) * (i + 2));
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
		next = strstr(prev, sep);
		i += 1;
	}
	*outstrs = newstrs;
	return i + 1;
}
