/* ***************************************************************************
 * charset.c -- The charset operation set.
 * 
 * Copyright (C) 2013 by
 * Liu Chao
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
 * charset.c -- 字符集的操作。
 *
 * 版权所有 (C) 2013 归属于
 * 刘超
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
#include LC_LCUI_H
#include LC_ERROR_H
#include LC_FONT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef LCUI_BUILD_IN_LINUX
#include <iconv.h>

/* 编码转换，从一种编码转为另一种编码，主要是调用iconv的API实现字符编码转换 */
static int 
code_convert(	char *src_charset,	char *des_charset, 
		const char *inbuf,	unsigned int inlen,
		unsigned char *outbuf,	unsigned int outlen )
{
	iconv_t cd;
	const char **pin = &inbuf;
	unsigned char **pout = &outbuf;
	
	cd = iconv_open(des_charset, src_charset);
	if (cd == 0) {
		return -1;
	}
	
	memset(outbuf, 0, outlen);
	
	if (iconv(cd,(char**)pin, &inlen, (char**)pout, &outlen)==-1) {
		return -1;
	}
	
	iconv_close(cd);
	return 0;
}
#endif

#define MAX_SAVE_NUM   20
static wchar_t 
covernt_code(unsigned char in[MAX_SAVE_NUM])
{
 	wchar_t unicode;
 	unicode = in[0];
	if (unicode >= 0xF0) {
		unicode = (wchar_t) (in[0] & 0x07) << 18;
		unicode |= (wchar_t) (in[1] & 0x3F) << 12;
		unicode |= (wchar_t) (in[2] & 0x3F) << 6;
		unicode |= (wchar_t) (in[3] & 0x3F);
	} else if (unicode >= 0xE0) {
		unicode = (wchar_t) (in[0] & 0x0F) << 12;
		unicode |= (wchar_t) (in[1] & 0x3F) << 6;
		unicode |= (wchar_t) (in[2] & 0x3F);
	} else if (unicode >= 0xC0) {
		unicode = (wchar_t) (in[0] & 0x1F) << 6;
		unicode |= (wchar_t) (in[1] & 0x3F);
	}
	return unicode;
}

/* UTF-8转Unicode */
LCUI_EXPORT(int)
LCUICharset_UTF8ToUnicode( const char *src_utf8, wchar_t **des_unicode )
{
	wchar_t *buff;
	unsigned char *p, t, save[MAX_SAVE_NUM];
	unsigned int len, i, j, n, count;
 	
	len = strlen(src_utf8)+1;  
	buff = (wchar_t *)calloc(sizeof(wchar_t), len); 
	if( !buff ) {
		return -1;
	}
	
	for(count=0,i=0,j=0; i<len; ++i) {
		t = src_utf8[i];
		/* 结束符的判断 */
		if(t == 0) {
			break;
		}
			
		if((t>>7) == 0) {// 0xxxxxxx
			buff[j] = t; 
			++j;
		} else if((t>>5) == 6) {// 110xxxxx 
			count = 2; 
		} else if((t>>4) == 14) {// 1110xxxx 
			count = 3; 
		} else if((t>>3) == 30) {// 11110xxx 
			count = 4; 
		} else if((t>>2) == 62) {// 111110xx 
			count = 5; 
		} else if((t>>1) == 126) {// 1111110x 
			count = 6; 
		}
		if(count > 0) {
			p = (unsigned char*)&src_utf8[i];
			for(n=0; n<count; ++n) {
				save[n] = *p++;
			}
			count = 0; 
			buff[j] = covernt_code(save);
			memset(save, 0, sizeof(save));
			++j;
		}
	}
	/****
	printf("result code:");
	for(i=0; i<j; ++i)
		printf("%02x ", buff[i]);
	printf("\n");
	******/
	*des_unicode = buff;
	return j;
}

/* ASCII转Unicode */
LCUI_EXPORT(int)
LCUICharset_ASCIIToUnicode( const char *src_ascii, wchar_t **des_unicode )
{
	wchar_t *buff;
	unsigned int len, i;
 	
	len = strlen(src_ascii);
	buff = (wchar_t *)malloc( sizeof(wchar_t)*(len+1) );
	for(i=0; i<len; ++i) {
		buff[i] = src_ascii[i];
	}
	buff[i] = 0;
	*des_unicode = buff;
	return len;
}

/* GB2312转Unicode */
LCUI_EXPORT(int)
LCUICharset_GB2312ToUnicode( const char *src_gb2312, wchar_t **des_unicode )
{
#ifdef LCUI_BUILD_IN_LINUX
	char *buff;
	unsigned char *p;
	unsigned int len, new_len;
 
	len = strlen( src_gb2312 );
	new_len = len*3;
	buff = (char *) calloc ( sizeof(char), new_len );
	p = (unsigned char*) buff;
	if(code_convert("gb2312", "utf8", src_gb2312, len, p, new_len)) {
		printf("%s (): error: %s", __FUNCTION__, ERROR_CONVERT_ERROR);
		return -1;
	}
	len = LCUICharset_UTF8ToUnicode( buff, des_unicode );
	free(buff);
	return len;
#else
	return -1;
#endif
}

