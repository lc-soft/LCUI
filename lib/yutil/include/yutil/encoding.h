/*
 * encoding.h -- String encoding/decoding utilities.
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
#ifndef YUTIL_ENCODING_H
#define YUTIL_ENCODING_H

#include <wchar.h>

Y_BEGIN_DECLS

/**
 * Decode an UTF-8 encoded string to a wide character string.
 *
 * When \p wstr is NULL, returns the number of wchar_t needed (excluding the
 * null terminator), similar to strlen().  When \p wstr is not NULL, writes up
 * to \p max_len characters and always null-terminates at wstr[count], so the
 * buffer must have room for at least max_len+1 elements.  If the decoded
 * length exceeds max_len, only max_len characters are written.
 */
YUTIL_API size_t decode_utf8(wchar_t *wstr, const char *str, size_t max_len);

/**
 * Encode a wide character string to UTF-8.
 *
 * When \p str is NULL, returns the number of bytes needed (excluding the null
 * terminator), similar to strlen().  When \p str is not NULL, writes up to
 * \p max_len bytes and always null-terminates at str[count], so the buffer
 * must have room for at least max_len+1 elements.  If the encoded length
 * exceeds max_len, only max_len bytes are written.
 */
YUTIL_API size_t encode_utf8(char *str, const wchar_t *wstr, size_t max_len);

#ifdef _WIN32
YUTIL_API size_t decode_ansi(wchar_t *wstr, const char *str, size_t max_len);

YUTIL_API size_t encode_ansi(char *str, const wchar_t *wstr, size_t max_len);
#endif

Y_END_DECLS

#endif
