/* ***************************************************************************
* charset.c -- The charset opreation set.
*
* Copyright (C) 2015-2016 by Liu Chao <lc-soft@live.cn>
*
* This file is part of the LCUI project, and may only be used, modified, and
* distributed under the terms of the GPLv2.
*
* (GPLv2 is abbreviation of GNU General Public License Version 2)
*
* By continuing to use, modify, or distribute this file you indicate that you
* have read the license and understand and accept it fully.
*
* The LCUI project is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
*
* You should have received a copy of the GPLv2 along with this file. It is
* usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
* ****************************************************************************/

/* ****************************************************************************
* charset.c -- 字符集的相关操作函数
*
* 版权所有 (C) 2015-2016 归属于 刘超 <lc-soft@live.cn>
*
* 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
*
* (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
*
* 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
*
* LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
* 定用途的隐含担保，详情请参照GPLv2许可协议。
*
* 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
* 没有，请查看：<http://www.gnu.org/licenses/>.
* ****************************************************************************/

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font/charset.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_SAVE_NUM   20

#ifdef LCUI_BUILD_IN_LINUX
static int SetLocaleByEncoding( int encoding )
{
	const char *locale;
	switch( encoding ) {
	case ENCODING_ANSI: 
		locale = NULL;
		break;
	case ENCODING_UTF8:
		locale = "zh_CN.UTF-8";
		break;
	default: return -1;
	}
	if( !setlocale( LC_CTYPE, locale ) ) {
		return -1;
	}
	return 0;
}
#else 
/** 将UTF-8字符串解码成 Unicode 字符串 */
static int DecodeFromUTF8( wchar_t *wstr, int max_len, const char *str )
{
	wchar_t unicode;
	const char *inptr;
	int i, len = 0, count = 0;
	unsigned char *p, byte, ch[MAX_SAVE_NUM];

	if( max_len == -1 ) {
		max_len = strlen( str );
	}
	for( inptr = str; *inptr && len < max_len; ++inptr ) {
		byte = *inptr;
		if( (byte >> 7) == 0 ) { // 0xxxxxxx
			wstr[len] = byte;
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
		wstr[len] = unicode;
		++len;
	}
	if( len < max_len ) {
		wstr[len] = 0;
	}
	return len;
}

#endif

int LCUI_DecodeString( wchar_t *wstr, const char *str, 
		       int max_len, int encoding )
{
#ifdef LCUI_BUILD_IN_WIN32
	// 暂时不处理其它编码方式
	switch( encoding ) {
	case ENCODING_ANSI:
	case ENCODING_UTF8:
	default: break;
	}
	return DecodeFromUTF8( wstr, max_len, str );
#else
	if( SetLocaleByEncoding( encoding ) != 0 ) {
		return -1;
	}
	return mbstowcs( wstr, str, max_len );
#endif
}

#ifdef LCUI_BUILD_IN_WIN32
#define encode(CP, WSTR, STR, LEN) \
WideCharToMultiByte(CP, 0, WSTR, -1, STR, LEN, NULL, NULL)
#endif

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
	if( SetLocaleByEncoding( encoding ) != 0 ) {
		return -1;
	}
	return wcstombs( str, wstr, max_len );
#endif
}
