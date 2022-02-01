﻿/*
 * charset.h -- The charset opreation set.
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
#ifndef UTIL_CHARSET_H
#define UTIL_CHARSET_H

#include <wchar.h>

Y_BEGIN_DECLS

enum encoding_e { ENCODING_ANSI, ENCODING_UTF8 };

typedef enum encoding_e encoding_e;

#define decode_utf8(WSTR, STR, MAX_LEN) \
	decode_string(WSTR, STR, MAX_LEN, ENCODING_UTF8)

#define encode_utf8(STR, WSTR, MAX_LEN) \
	encode_string(STR, WSTR, MAX_LEN, ENCODING_UTF8)

YUTIL_API size_t decode_string(wchar_t *wstr, const char *str, size_t max_len,
			       encoding_e encoding);

YUTIL_API size_t encode_string(char *str, const wchar_t *wstr, size_t max_len,
			       encoding_e encoding);
Y_END_DECLS

#endif
