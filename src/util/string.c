/* ***************************************************************************
 * string.c -- The string operation set.
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
 * string.c -- 字符串相关操作函数。
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

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h> 

int strtrim( char *outstr, const char *instr, const char *charlist )
{
	LCUI_BOOL clear, clear_left = TRUE;
	char *op = outstr, *last_blank = NULL;
	const char *default_char_list = "\t\n\r ", *ip = instr, *c;

	if( !charlist ) {
		charlist = default_char_list;
	}
	for( ; *ip; ip++ ) {
		for( clear = FALSE, c = charlist; *c;  ++c ) {
			if( *ip == *c ) {
				clear = TRUE;
				break;
			}
		}
		if( clear ) {
			if( !clear_left ) {	
				*op = *ip;
				if( !last_blank ) {
					last_blank = op;
				}
				++op;
			}
			continue;
		}
		if( clear_left ) {
			clear_left = FALSE;
		}
		last_blank = NULL;
		*op = *ip;
		++op;
	}
	if( last_blank ) {
		*last_blank = 0;
	}
	*op = 0;
	return op - outstr;
}

void freestrs( char **strs )
{
	int i = 0;
	while( strs[i] ) {
		free( strs[i] );
		++i;
	}
	free( strs );
}

int strsplit( const char *instr, const char *sep, char ***outstrs )
{
	int len, i = 0;
	const char *prev = instr;
	int sep_len = strlen( sep );
	char *next = strstr( prev, sep );
	char **newstrs = NULL;

	while( 1 ) {
		char **tmp, *str;
		if( next ) {
			len = next - prev + 1;
		} else {
			len = strlen( prev ) + 1;
		}
		str = malloc( sizeof( char ) * len );
		tmp = realloc( newstrs, sizeof( char* ) * (i + 2) );
		if( !tmp ) {
			freestrs( newstrs );
			return 0;
		}
		newstrs = tmp;
		strncpy( str, prev, len - 1 );
		str[len - 1] = 0;
		newstrs[i] = str;
		newstrs[i + 1] = NULL;
		if( next ) {
			prev = next + sep_len;
		} else {
			break;
		}
		next = strstr( prev, sep );
		i += 1;
	}
	*outstrs = newstrs;
	return i + 1;
}

static int strsaddone( char ***strlist, const char *str )
{
	int i = 0;
	char **newlist;

	if( !*strlist ) {
		newlist = (char**)malloc( sizeof(char*) * 2 );
		goto check_done;
	}
	for( i = 0; (*strlist)[i]; ++i ) {
		if( strcmp((*strlist)[i], str) == 0 ) {
			return 0;
		}
	}
	newlist = (char**)realloc( *strlist, (i+2)*sizeof(char*) );
check_done:
	if( !newlist ) {
		return 0;
	}
	newlist[i] = strdup(str);
	newlist[i+1] = NULL;
	*strlist = newlist;
	return 1;
}

int strsadd( char ***strlist, const char *str )
{
	char buff[256];
	int count = 0, i, head;
	for( head = 0, i = 0; str[i]; ++i ) {
		if( str[i] != ' ' ) {
			continue;
		}
		if( i - 1 > head ) {
			strncpy( buff, &str[head], i - head );
			buff[i - head] = 0;
			count += strsaddone( strlist, buff );
		}
		head = i + 1;
	}
	if( i - 1 > head ) {
		strncpy( buff, &str[head], i - head );
		buff[i - head] = 0;
		count += strsaddone( strlist, buff );
	}
	return count;
}

int strshas( char **strlist, const char *str )
{
	int i;
	if( !strlist ) {
		return 0;
	}
	for( i = 0; strlist[i]; ++i ) {
		if( strcmp(strlist[i], str) == 0 ) {
			return 1;
		}
	}
	return 0;
}

static int strsdelone( char ***strlist, const char *str )
{
	int i, pos, len;
	char **newlist;

	if( !*strlist ) {
		return 0;
	}
	for( pos = -1, i = 0; (*strlist)[i]; ++i ) {
		if( strcmp( (*strlist)[i], str ) == 0 ) {
			pos = i;
			break;
		}
	}
	if( pos == -1 ) {
		return 0;
	}
	for( ; (*strlist)[i]; ++i );
	if( pos == 0 && i < 2 ) {
		free( *strlist );
		*strlist = NULL;
		return 1;
	}
	len = i - 1;
	newlist = (char**)malloc( i * sizeof(char*) );
	for( i = 0; i < pos; ++i ) {
		newlist[i] = (*strlist)[i];
	}
	for( i = pos; i < len; ++i ) {
		newlist[i] = (*strlist)[i+1];
	}
	newlist[i] = NULL;
	free( (*strlist)[pos] );
	free( *strlist );
	*strlist = newlist;
	return 1;
}

int strsdel( char ***strlist, const char *str )
{
	char buff[256];
	int count = 0, i, head;

	for( head = 0, i = 0; str[i]; ++i ) {
		if( str[i] != ' ' ) {
			continue;
		}
		if( i - 1 > head ) {
			strncpy( buff, &str[head], i - head );
			buff[i - head] = 0;
			count += strsdelone( strlist, buff );
		}
		head = i + 1;
	}
	if( i - 1 > head ) {
		strncpy( buff, &str[head], i - head );
		buff[i - head] = 0;
		count += strsdelone( strlist, buff );
	}
	return count;
}

int sortedstrsadd( char ***strlist, const char *str )
{
	int i, n, len, pos;
	char **newlist, *newstr;

	if( *strlist ) {
		for( i = 0; (*strlist)[i]; ++i );
		len = i + 2;
	} else {
		len = 2;
	}
	newlist = realloc( *strlist, sizeof( char* ) * len );
	if( !newlist ) {
		return -ENOMEM;
	}
	newlist[len - 2] = NULL;
	for( i = 0, pos = -1; newlist[i]; ++i ) {
		n = strcmp( newlist[i], str );
		if( n < 0 ) {
			continue;
		} else if( n == 0 ) {
			return 1;
		} else {
			pos = i;
			break;
		}
	}
	n = strlen( str ) + 1;
	newstr = malloc( sizeof(char) * n );
	if( !newstr ) {
		return -ENOMEM;
	}
	strncpy( newstr, str, n );
	if( pos >= 0 ) {
		for( i = len - 2; i > pos; --i ) {
			newlist[i] = newlist[i - 1];
		}
		newlist[pos] = newstr;
	} else {
		pos = len - 2;
	}
	newlist[pos] = newstr;
	newlist[len - 1] = NULL;
	*strlist = newlist;
	return 0;
}
