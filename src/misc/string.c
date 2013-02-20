/* ***************************************************************************
 * string.c -- The string operation set.
 * 
 * Copyright (C) 2012 by
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
 * string.c -- 字符串操作集
 *
 * 版权所有 (C) 2012 归属于 
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
#include LC_FONT_H

/* 将字母转换成大写 */
static char uppercase( char ch )
{
	if( 'a' <= ch && ch <= 'z' ) {
		return ch - 32;
	}
	return ch;
}

/* 不区分大小写，对比两个字符串 */
int lcui_strcasecmp( const char *str1, const char *str2 )
{
	const char *p1, *p2;
	for(p1=str1, p2=str2; *p1!=0 || *p2!=0; ++p1, ++p2) {
		if(uppercase(*p1) == uppercase(*p2)) {
			continue;
		}
		return *p1-*p2;
	}
	if( *p1 != 0 ) {
		return 1;
	} else if( *p2 != 0 ) {
		return -1;
	}
	return 0;
}

void String_Init(LCUI_String *in)
/* 功能：初始化String结构体中的数据 */
{
	in->size = 0;
	in->string = NULL;
}

void Strcpy (LCUI_String * des, const char *src)
/* 功能：拷贝字符串至String结构体数据中 */
{
	if(des == NULL) {
		return;
	}
	if (src != NULL) {
		if (des->size != 0) {
			free (des->string);
		}
		des->size = sizeof (char) * (strlen (src) + 1);
		des->string = calloc (1, des->size);
		strcpy (des->string, src);
	} else {
		des->size = sizeof (char) * (2);
		des->string = calloc (1, des->size);
	}
}

int Strcmp(LCUI_String *str1, const char *str2)
/* 功能：对比str1与str2 */
{
	if (str1 != NULL && str1->size > 0 && str2 != NULL) 
		return strcmp(str1->string, str2); 
	else return -1;
}

int LCUI_Strcmp(LCUI_String *str1, LCUI_String *str2)
/* LCUI_String 字符串对比 */
{
	if( str1->size > 0 && str2->size > 0 ) {
		return strcmp(str1->string, str2->string);
	}
	return 0;
}

int LCUI_Strcpy(LCUI_String *str1, LCUI_String *str2)
/* LCUI_String 字符串拷贝 */
{
	if( str2->size <= 0 ) {
		return -1;
	}
	if(str1->size > 0) {
		free( str1->string );
	}
	str1->string = (char*)calloc(str2->size+1, sizeof(char));
	strcpy( str1->string, str2->string );
	str1->size = str2->size;
	return 0;
}

void String_Free(LCUI_String *in) 
{
	if(in->size > 0) {
		free(in->string); 
	}
	in->string = NULL;
}

void WChar_T_Free(LCUI_WChar_T *ch) 
{
	ch->bitmap = NULL;
}

void WString_Free(LCUI_WString *str) 
{
	int i;
	if(str != NULL) {
		if(str->size > 0 && str->string != NULL) {
			for(i = 0; i < str->size; ++i) {
				WChar_T_Free(&str->string[i]); 
			}
			free(str->string);
			str->string = NULL;
		}
	}
}
