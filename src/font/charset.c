/*
 * charset.c -- The charset opreation set.
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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font/charset.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_SAVE_NUM   20

#ifdef LCUI_BUILD_IN_WIN32
#define encode(CP, WSTR, STR, LEN) \
WideCharToMultiByte(CP, 0, WSTR, -1, STR, LEN, NULL, NULL)
#define decode(CP, STR, WSTR, LEN) \
MultiByteToWideChar(CP, 0, STR, -1, WSTR, LEN)
#endif

/** 将UTF-8字符串解码成 Unicode 字符串 */
static int DecodeFromUTF8( wchar_t *wstr, int max_len, const char *str )
{
	wchar_t unicode;
	const char *inptr;
	int i, len = 0, count = 0;
	unsigned char *p, byte, ch[MAX_SAVE_NUM];

	for( inptr = str; *inptr; ++inptr ) {
		if( max_len > 0 && len >= max_len ) {
			break;
		}
		byte = *inptr;
		if( (byte >> 7) == 0 ) { // 0xxxxxxx
			if( wstr ) {
				wstr[len] = byte;
			}
			++len;
			continue;
		}
		if( (byte >> 5) == 6 ) { // 110xxxxx 
			count = 2;
		} else if( (byte >> 4) == 14 ) { // 1110xxxx 
			count = 3;
		} else if( (byte >> 3) == 30 ) { // 11110xxx 
			count = 4;
		} else if( (byte >> 2) == 62 ) { // 111110xx 
			count = 5;
		} else if( (byte >> 1) == 126 ) { // 1111110x 
			count = 6;
		} else {
			continue;
		}
		p = (unsigned char*)inptr;
		for( i = 0; i < count; ++i ) {
			ch[i] = *p++;
		}
		count = 0;
		unicode = ch[0];
		if( unicode >= 0xF0 ) {
			unicode = (wchar_t)(ch[0] & 0x07) << 18;
			unicode |= (wchar_t)(ch[1] & 0x3F) << 12;
			unicode |= (wchar_t)(ch[2] & 0x3F) << 6;
			unicode |= (wchar_t)(ch[3] & 0x3F);
		} else if( unicode >= 0xE0 ) {
			unicode = (wchar_t)(ch[0] & 0x0F) << 12;
			unicode |= (wchar_t)(ch[1] & 0x3F) << 6;
			unicode |= (wchar_t)(ch[2] & 0x3F);
		} else if( unicode >= 0xC0 ) {
			unicode = (wchar_t)(ch[0] & 0x1F) << 6;
			unicode |= (wchar_t)(ch[1] & 0x3F);
		}
		if( wstr ) {
			wstr[len] = unicode;
		}
		++len;
	}
	if( len < max_len && wstr ) {
		wstr[len] = 0;
	}
	return len;
}

int LCUI_DecodeString( wchar_t *wstr, const char *str, 
		       int max_len, int encoding )
{
#ifdef LCUI_BUILD_IN_WIN32
	// 暂时不处理其它编码方式
	switch( encoding ) {
	case ENCODING_ANSI:
		return decode( CP_ACP, str, wstr, max_len );
	case ENCODING_UTF8:
		return DecodeFromUTF8( wstr, max_len, str );
	default: break;
	}
	return 0;
#else
	return DecodeFromUTF8( wstr, max_len, str );
#endif
}

int LCUI_EncodeString( char *str, const wchar_t *wstr, 
		       int max_len, int encoding )
{
#ifdef LCUI_BUILD_IN_WIN32
	int cp;
	// 暂时不处理其它编码方式
	switch( encoding ) {
	case ENCODING_ANSI: cp = CP_ACP; break;
	case ENCODING_UTF8: cp = CP_UTF8; break;
	default: return -1;
	}
	return encode( cp, wstr, str, max_len );
#else
	return wcstombs( str, wstr, max_len );
#endif
}
