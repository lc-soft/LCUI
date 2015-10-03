/* ***************************************************************************
 * textstyle.c -- text style processing module.
 * 
 * Copyright (C) 2012-2015 by Liu Chao <lc-soft@live.cn>
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
 * textstyle.c -- 文本样式处理模块
 *
 * 版权所有 (C) 2012-2015 归属于 刘超 <lc-soft@live.cn>
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
#include <LCUI/font.h>

#define case_in_blank_char	\
	case '\t':		\
	case ' ':		\
	case '\n':		\
	case '\r'

enum LCUI_StyleTagID {
	TAG_ID_FAMILY,
	TAG_ID_STYLE,
	TAG_ID_WIEGHT,
	TAG_ID_DECORATION,
	TAG_ID_SIZE,
	TAG_ID_COLOR
};

typedef struct LCUI_StyleTag {
	int id;
	LCUI_Style style;
} LCUI_StyleTag;

/** 初始化字体样式数据 */
void TextStyle_Init( LCUI_TextStyle *data )
{
	data->has_style = FALSE;
	data->has_weight = FALSE;
	data->has_decoration = FALSE;
	data->has_family = FALSE;
	data->has_back_color = FALSE;
	data->has_fore_color = FALSE;
	data->font_ids = NULL;
	data->style = FONT_STYLE_NORMAL;
	data->weight = FONT_WEIGHT_NORMAL;
	data->decoration = FONT_DECORATION_NONE;
	data->fore_color.value = 0x33333333;
	data->back_color.value = 0xffffffff;
	data->pixel_size = 13;
}

static void trim( char *outstr, const char *instr )
{
	char *op = outstr;
	const char *ip = instr;

	while( *ip++ ) {
		switch( *ip ) {
		case_in_blank_char:
			continue;
		default:
			*op = *ip;
			++op;
		}
	}
	*op = 0;
	while( --op >= outstr ) {
		switch( *ip ) {
		case_in_blank_char:
			break;
		default:
			op = outstr;
			continue;
		}
		*op = 0;
	}
}

/**
 * 设置字体
 * @param[in][out] ts 字体样式数据
 * @param[in] str 字体名称，如果有多个名称则用逗号分隔
 */
int TextStyle_SetFont( LCUI_TextStyle *ts, const char *str )
{
	char name[256];
	const char *p, *style_name;
	int count, i, *ids;
	
	if( ts->has_family && ts->font_ids ) {
		free( ts->font_ids );
	}
	ts->font_ids = NULL;
	for( p = str, count = 1; *p; ++p ) {
		if( *p == ',' ) {
			++count;
		}
	}
	if( p - str == 0 ) {
		return -1;
	}
	ids = (int*)malloc(sizeof(int)*(count+1));
	if( !ids ) {
		return -2;
	}
	ids[count] = -1;
	switch( ts->style ) {
	case FONT_STYLE_OBLIQUE: style_name = "oblique"; break;
	case FONT_STYLE_ITALIC: style_name = "italic"; break;
	case FONT_STYLE_NORMAL:
	default: style_name = "regular"; break;
	}
	for( p = str, count = 0, i = 0; *p; ++p ) {
		if( *p != ',' ) {
			name[i++] = *p;
			continue;
		}
		name[i] = 0;
		trim( name, name );
		ids[count] = LCUIFont_GetId( name, style_name );
		if( ids[count] > 0 ) {
			++count;
		}
		i = 0;
	}
	ts->has_family = TRUE;
	ts->font_ids = ids;
	return 0;
}

/*-------------------------- StyleTag --------------------------------*/
#define MAX_TAG_NUM 2

/** 初始化样式标签栈 */
void StyleTags_Init( LinkedList *tags )
{
	LinkedList_Init( tags, sizeof(LCUI_StyleTag) );
}

/** 销毁样式标签栈 */
void StyleTags_Destroy( LinkedList *tags )
{
	LinkedList_Destroy( tags );
}

/** 获取当前的文本样式 */
LCUI_TextStyle* StyleTags_GetTextStyle( LinkedList *tags )
{
	LCUI_StyleTag *tag_data;
	LCUI_TextStyle *style_data;
	int i, total, equal = 0, flags[MAX_TAG_NUM] = {0};
	
	total = LinkedList_GetTotal( tags );
	if( total <= 0 ) {
		return NULL;
	}
	style_data = (LCUI_TextStyle*)malloc( sizeof(LCUI_TextStyle) );
	TextStyle_Init( style_data );
	/* 根据已经记录的各种样式，生成当前应有的文本样式 */
	for(equal=0,i=total-1; i>=0; --i) {
		LinkedList_Goto( tags, i );
		tag_data = (LCUI_StyleTag*)LinkedList_Get( tags );
		DEBUG_MSG("tag id: %d\n", tag_data->id);
		switch( tag_data->id ) {
		    case TAG_ID_COLOR: 
			if( flags[0] != 0 ) {
				break;
			}
			style_data->has_fore_color = TRUE;
			style_data->fore_color = tag_data->style.color;
			DEBUG_MSG("color: %d,%d,%d\n", style_data->fore_color.red,
			 style_data->fore_color.green, style_data->fore_color.blue);
			flags[0] = 1;
			++equal;
			break;
		case TAG_ID_SIZE:
			if( flags[1] != 0 ) {
				break;
			}
			style_data->has_pixel_size = TRUE;
			style_data->pixel_size = tag_data->style.px;
			flags[1] = 1;
			++equal;
			break;
		    default: break;
		}
		if(equal == MAX_TAG_NUM) {
			break;
		}
	}
	if( equal == 0 ) {
		free( style_data );
		return NULL;
	}
	return style_data;
}

/** 将指定标签的样式数据从队列中删除，只删除队列尾部第一个匹配的标签 */
static void StyleTags_Delete( LinkedList *tags, int id )
{
	int i, total;
	LCUI_StyleTag *p; 
	 
	total = LinkedList_GetTotal( tags );
	DEBUG_MSG("delete start, total tag: %d\n", total);
	if(total <= 0) {
		return;
	}
	for(i=total-1; i>=0; --i) {
		LinkedList_Goto( tags, i );
		p = (LCUI_StyleTag*)LinkedList_Get( tags );
		if( p->id == id ) {
			LinkedList_Delete( tags );
			break;
		}
	} 
	DEBUG_MSG("delete end, total tag: %d\n", LinkedList_GetTotal( tags ));
}

/** 清除字符串中的空格 */
void clear_space( char *in, char *out )
{
	int j, i, len = strlen(in);
	for(j=i=0; i<len; ++i) {
		if(in[i] == ' ') {
			continue;
		}
		out[j] = in[i];
		++j;
	}
	out[j] = 0;
}

/** 在字符串中获取样式的结束标签，输出的是标签名 */
const wchar_t* scan_style_ending_tag( const wchar_t *wstr, char *name )
{
	int i, j, len;
	
	len = wcslen ( wstr );
	//printf("string: %S\n", wstr);
	if( wstr[0] != '[' || wstr[1] != '/' ) { 
		return NULL;
	} 
	/* 匹配标签,获取标签名 */
	for(j=0,i=2; i<len; ++i) {
		switch( wstr[i] ) {
		case ' ': break;
		case ']': ++i; goto end_tag_search;
		default:
			if( name ) {
				name[j] = wstr[i];
			}
			++j; 
			break;
		}
	}
	
end_tag_search:;
	if( name ) {
		name[j] = 0;
	}
	if( j < 1 ) { 
		return NULL;
	}
	return wstr+i;
}

/** 从字符串中获取样式标签的名字及样式属性 */
const wchar_t* scan_style_tag( const wchar_t *wstr, char *name,
			       int max_name_len, char *data )
{
	int i, j, len; 
	LCUI_BOOL end_name = FALSE;

	len = wcslen( wstr );
	DEBUG_MSG2("len = %d\n", len);
	if( wstr[0] != '<' ) {
		DEBUG_MSG2("str[0] != '<'\n");
		return NULL;
	}
	/* 匹配标签前半部分 */
	for( j=0,i=1; i<len; ++i ) {
		if( wstr[i] == ' ' ) {
			/* 如果上个字符不是空格，说明标签名已经结束 */
			if( i > 0 && wstr[i-1] != ' ' ) {
				end_name = TRUE;
			}
			/* 标签名首部和尾部可包含空格 */
			if( j == 0 || max_name_len == 0
			 || (max_name_len > 0 && end_name) ) { 
				continue;
			}
			/* 标签名中间不能包含空格 */
			return NULL;
		}
		/* 如果标签名部分已经匹配完 */
		if( wstr[i] == '=' ) {
			++i;
			break;
		}
		/* 如果标签名已经结束了 */
		if( end_name ) {
			return NULL;
		}
		if( max_name_len > 0 && data ) {
			name[j] = wstr[i];
		}
		++j;
	}
	
	if( data ) {
		name[j] = 0;
	}
	DEBUG_MSG2("tag is: %s\n", tag);
	/* 获取标签后半部分 */
	for(j=0; i<len; ++i) {
		DEBUG_MSG2("str[%d]: %c\n", i, str[i]);
		if( wstr[i] == ' ' ) {
			continue; 
		}
		/* 标签结束，退出 */
		if( wstr[i] == '>' ) {
			++i;
			break;
		}
		if( data ) {
			/* 保存标签内的数据 */
			data[j] = wstr[i];
		}
		++j;
	}
	if( data ) {
		data[j] = 0;
	}
	DEBUG_MSG2("data: %s\n", data);
	if( i >= len ) {
		return NULL;
	}
	return wstr+i;
}

/** 在字符串中获取指定样式标签中的数据 */
static const wchar_t* 
scan_style_tag_by_name( const wchar_t *wstr, const char *name, char *data )
{
	int i, j, len, tag_len; 
	
	len = wcslen( wstr );
	DEBUG_MSG2("len = %d\n", len);
	tag_len = strlen( name );
	if( wstr[0] != '[' ) {
		DEBUG_MSG2("wstr[0] != '<'\n");
		return NULL;
	} 
	/* 匹配标签前半部分 */
	for( j=0,i=1; i<len; ++i ) {
		if( wstr[i] == ' ' ) { 
			if( j == 0 || j >= tag_len ) { 
				continue;
			}
			return NULL;
		}
		else if( wstr[i] == name[j] ) { 
			++j;
			continue;
		}
		/* 如果标签名部分已经匹配完 */
		if( j>= tag_len && wstr[i] == '=' ) {
			++i; 
			break;
		}
		/* 否则，有误 */
		return NULL;
	}
	DEBUG_MSG2("tag is: %s\n", tag);
	/* 获取标签后半部分 */
	for(j=0; i<len; ++i) {
		DEBUG_MSG2("wstr[%d]: %c\n", i, wstr[i]);
		if( wstr[i] == ' ' ) {
			continue; 
		}
		/* 标签结束，退出 */
		if( wstr[i] == ']' ) {
			++i;
			break;
		}
		/* 保存标签内的数据 */
		data[j] = wstr[i];
		++j;
	}
	data[j] = 0;
	DEBUG_MSG2("data: %s\n", data);
	if(i >= len ) {
		return NULL;
	}
	return &wstr[i];
}

/** 根据字符串中的标签得到相应的样式数据，并返回指向标签后面字符的指针 */
static const wchar_t* 
scan_style_tag_data( const wchar_t *wstr, LCUI_StyleTag *tag )
{
	const wchar_t *p, *q; 
	char tag_data[256];
	
	p = wstr; 
	if( (q = scan_style_tag_by_name( p, "color", tag_data)) ) {
		DEBUG_MSG("is color style tag, data: %s\n", tag_data);
		if( !ParseColor( &tag->style, tag_data ) ) {
			return NULL;
		}
		tag->id = TAG_ID_COLOR;
		return q;
	}
	if( (q = scan_style_tag_by_name( p, "size", tag_data)) ) {
		if( !ParseNumber( &tag->style, tag_data ) ) {
			return NULL;
		}
		tag->id = TAG_ID_SIZE;
		return q;
	}
	return NULL;
}

/** 处理样式标签 */
const wchar_t*
StyleTags_ScanBeginTag( LinkedList *tags, const wchar_t *str )
{
	const wchar_t *q;
	LCUI_StyleTag data;
	
	/* 开始处理样式标签 */
	q = scan_style_tag_data( str, &data );
	DEBUG_MSG2("handle_style_tag():%p\n", q);
	if( q ) {
		DEBUG_MSG2("add style data\n");
		/* 将标签样式数据加入队列 */
		LinkedList_AppendCopy( tags, &data );
	}
	return q;
}

/** 处理样式结束标签 */
const wchar_t* 
StyleTags_ScanEndingTag( LinkedList *tags, const wchar_t *str )
{
	const wchar_t *p;
	char tag_name[256];
	/* 获取标签名 */
	p = scan_style_ending_tag( str, tag_name );
	if( !p ) {
		return NULL;
	}
	/* 删除相应的样式标签 */
	if( strcmp(tag_name, "color") == 0 ) {
		StyleTags_Delete( tags, TAG_ID_COLOR );
	} 
	else if( strcmp(tag_name, "size") == 0 ) {
		StyleTags_Delete( tags, TAG_ID_SIZE );
	} else {
		return NULL;
	}
	return p;
}

/*------------------------- End StyleTag -----------------------------*/
