/*
 * textstyle.c -- text style processing module.
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

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>

typedef enum LCUI_TextStyleTagType_ {
	TEXT_STYLE_STYLE,
	TEXT_STYLE_BOLD,
	TEXT_STYLE_ITALIC,
	TEXT_STYLE_SIZE,
	TEXT_STYLE_COLOR,
	TEXT_STYLE_BG_COLOR,
	TEXT_STYLE_TOTAL_NUM
} LCUI_TextStyleTagType;

typedef struct LCUI_StyleTag {
	LCUI_TextStyleTagType id;
	LCUI_StyleRec style;
} LCUI_TextStyleTag;

void TextStyle_Init( LCUI_TextStyle data )
{
	data->has_style = FALSE;
	data->has_weight = FALSE;
	data->has_family = FALSE;
	data->has_pixel_size = FALSE;
	data->has_back_color = FALSE;
	data->has_fore_color = FALSE;
	data->font_ids = NULL;
	data->style = FONT_STYLE_NORMAL;
	data->weight = FONT_WEIGHT_NORMAL;
	data->fore_color.value = 0xff333333;
	data->back_color.value = 0xffffffff;
	data->pixel_size = 13;
}

int TextStyle_CopyFamily( LCUI_TextStyle dst, LCUI_TextStyle src )
{
	size_t len;
	if( !src->has_family ) {
		return 0;
	}
	for( len = 0; src->font_ids[len]; ++len );
	len += 1;
	if( dst->font_ids ) {
		free( dst->font_ids );
	}
	dst->font_ids = malloc( len * sizeof( int ) );
	if( !dst->font_ids ) {
		return -ENOMEM;
	}
	dst->has_family = TRUE;
	memcpy( dst->font_ids, src->font_ids, len * sizeof( int ) );
	return 0;
}

int TextStyle_Copy( LCUI_TextStyle dst, LCUI_TextStyle src )
{
	*dst = *src;
	dst->font_ids = NULL;
	return TextStyle_CopyFamily( dst, src );
}

void TextStyle_Destroy( LCUI_TextStyle data )
{
	if( data->font_ids ) {
		free( data->font_ids );
	}
	data->font_ids = NULL;
}

void TextStyle_Merge( LCUI_TextStyle base, LCUI_TextStyle target )
{
	int *font_ids = NULL;
	base->has_family = TRUE;
	TextStyle_CopyFamily( base, target );
	if( target->has_style && !base->has_style &&
	    target->style != FONT_STYLE_NORMAL ) {
		base->has_style = TRUE;
		base->style = target->style;
	}
	if( LCUIFont_UpdateStyle( base->font_ids,
				  base->style,
				  &font_ids ) > 0 ) {
		free( base->font_ids );
		base->font_ids = font_ids;
	}
	if( target->has_weight && !base->has_weight &&
	    target->weight != FONT_WEIGHT_NORMAL ) {
		base->has_weight = TRUE;
		base->weight = target->weight;
	}
	if( LCUIFont_UpdateWeight( base->font_ids,
				   base->weight,
				   &font_ids ) > 0 ) {
		free( base->font_ids );
		base->font_ids = font_ids;
	}
}

int TextStyle_SetWeight( LCUI_TextStyle ts, LCUI_FontWeight weight )
{
	int *font_ids;
	ts->weight = weight;
	ts->has_weight = TRUE;
	if( LCUIFont_UpdateWeight( ts->font_ids, weight, &font_ids ) > 0 ) {
		free( ts->font_ids );
		ts->font_ids = font_ids;
		return 0;
	}
	return -1;
}

int TextStyle_SetStyle( LCUI_TextStyle ts, LCUI_FontStyle style )
{
	int *font_ids;
	ts->style = style;
	ts->has_style = TRUE;
	if( LCUIFont_UpdateStyle( ts->font_ids, style, &font_ids ) > 0 ) {
		free( ts->font_ids );
		ts->font_ids = font_ids;
		return 0;
	}
	return -1;
}

int TextStyle_SetFont( LCUI_TextStyle ts, const char *str )
{
	size_t count;
	if( ts->has_family && ts->font_ids ) {
		free( ts->font_ids );
	}
	ts->font_ids = NULL;
	ts->has_family = FALSE;
	count = LCUIFont_GetIdByNames( &ts->font_ids, ts->style,
				       ts->weight, str );
	if( count > 0 ) {
		ts->has_family = TRUE;
		return 0;
	}
	return -1;
}

int TextStyle_SetDefaultFont( LCUI_TextStyle ts )
{
	if( ts->has_family && ts->font_ids ) {
		free( ts->font_ids );
		ts->has_family = FALSE;
	}
	ts->font_ids = malloc( sizeof( int ) * 2 );
	if( !ts->font_ids ) {
		ts->font_ids = NULL;
		return -ENOMEM;
	}
	ts->has_family = TRUE;
	ts->font_ids[0] = LCUIFont_GetDefault();
	ts->font_ids[1] = 0;
	return 0;
}

/*-------------------------- StyleTag --------------------------------*/

void StyleTags_Clear( LinkedList *tags )
{
	LinkedList_Clear( tags, free );
}

/** 获取当前的文本样式 */
LCUI_TextStyle StyleTags_GetTextStyle( LinkedList *tags )
{
	int count = 0;
	LinkedListNode *node;
	LCUI_TextStyleTag *tag;
	LCUI_TextStyle style;
	LCUI_BOOL found_tags[TEXT_STYLE_TOTAL_NUM] = { 0 };

	if( tags->length <= 0 ) {
		return NULL;
	}
	style = malloc( sizeof( LCUI_TextStyleRec ) );
	TextStyle_Init( style );
	/* 根据已经记录的各种样式，生成当前应有的文本样式 */
	for( LinkedList_EachReverse( node, tags ) ) {
		tag = node->data;
		switch( tag->id ) {
		case TEXT_STYLE_COLOR:
			if( found_tags[tag->id] ) {
				break;
			}
			style->has_fore_color = TRUE;
			style->fore_color = tag->style.color;
			found_tags[tag->id] = TRUE;
			++count;
			break;
		case TEXT_STYLE_BG_COLOR:
			if( found_tags[tag->id] ) {
				break;
			}
			style->has_back_color = TRUE;
			style->back_color = tag->style.color;
			found_tags[tag->id] = TRUE;
			++count;
			break;
		case TEXT_STYLE_BOLD:
			if( found_tags[tag->id] ) {
				break;
			}
			found_tags[tag->id] = TRUE;
			TextStyle_SetWeight( style, FONT_WEIGHT_BOLD );
			++count;
			break;
		case TEXT_STYLE_ITALIC:
			if( found_tags[tag->id] ) {
				break;
			}
			found_tags[tag->id] = TRUE;
			TextStyle_SetStyle( style, FONT_STYLE_ITALIC );
			++count;
			break;
		case TEXT_STYLE_SIZE:
			if( found_tags[tag->id] ) {
				break;
			}
			style->has_pixel_size = TRUE;
			style->pixel_size = iround( tag->style.px );
			found_tags[tag->id] = TRUE;
			++count;
			break;
		default: break;
		}
		if( count == 4 ) {
			break;
		}
	}
	if( count == 0 ) {
		free( style );
		return NULL;
	}
	return style;
}

/** 将指定标签的样式数据从队列中删除，只删除队列尾部第一个匹配的标签 */
static void StyleTags_Delete( LinkedList *tags, int id )
{
	LCUI_TextStyleTag *tag;
	LinkedListNode *node;
	DEBUG_MSG( "delete start, total tag: %d\n", total );
	if( tags->length <= 0 ) {
		return;
	}
	for( LinkedList_Each( node, tags ) ) {
		tag = node->data;
		if( tag->id == id ) {
			free( tag );
			LinkedList_DeleteNode( tags, node );
			break;
		}
	}
	DEBUG_MSG( "delete end, total tag: %d\n", tags->length );
}

/** 清除字符串中的空格 */
void clear_space( char *in, char *out )
{
	size_t j, i, len = strlen( in );
	for( j = i = 0; i < len; ++i ) {
		if( in[i] == ' ' ) {
			continue;
		}
		out[j] = in[i];
		++j;
	}
	out[j] = 0;
}

/** 在字符串中获取样式的结束标签，输出的是标签名 */
const wchar_t *ScanStyleEndingTag( const wchar_t *wstr, wchar_t *name )
{
	size_t i, j, len;

	len = wcslen( wstr );
	//LOG("string: %S\n", wstr);
	if( wstr[0] != '[' || wstr[1] != '/' ) {
		return NULL;
	}
	/* 匹配标签,获取标签名 */
	for( j = 0, i = 2; i < len; ++i ) {
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
	return wstr + i;
}

/** 从字符串中获取样式标签的名字及样式属性 */
const wchar_t *ScanStyleTag( const wchar_t *wstr, wchar_t *name,
			       int max_name_len, wchar_t *data )
{
	size_t i, j, len;
	LCUI_BOOL end_name = FALSE;

	len = wcslen( wstr );
	if( wstr[0] != '<' ) {
		return NULL;
	}
	/* 匹配标签前半部分 */
	for( j = 0, i = 1; i < len; ++i ) {
		if( wstr[i] == ' ' ) {
			/* 如果上个字符不是空格，说明标签名已经结束 */
			if( i > 0 && wstr[i - 1] != ' ' ) {
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
	/* 获取标签后半部分 */
	for( j = 0; i < len; ++i ) {
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
	if( i >= len ) {
		return NULL;
	}
	return wstr + i;
}

/** 在字符串中获取指定样式标签中的数据 */
static const wchar_t *ScanStyleTagByName( const wchar_t *wstr,
					  const wchar_t *name,
					  char *data )
{
	size_t i, j, len, tag_len;

	len = wcslen( wstr );
	tag_len = wcslen( name );
	if( wstr[0] != '[' ) {
		return NULL;
	}
	/* 匹配标签前半部分 */
	for( j = 0, i = 1; i < len; ++i ) {
		if( wstr[i] == ' ' ) {
			if( j == 0 || j >= tag_len ) {
				continue;
			}
			return NULL;
		}
		if( j < tag_len ) {
			if( wstr[i] == name[j] ) {
				++j;
				continue;
			}
		} else if( wstr[i] == '=' ) {
			++i;
			break;
		} else if( wstr[i] == ']' ) {
			break;
		}
		return NULL;
	}
	/* 获取标签后半部分 */
	for( j = 0; i < len; ++i ) {
		if( wstr[i] == ' ' ) {
			continue;
		}
		/* 标签结束，退出 */
		if( wstr[i] == ']' ) {
			++i;
			break;
		}
		/* 保存标签内的数据 */
		data[j] = (char)wstr[i];
		++j;
	}
	data[j] = 0;
	if( i >= len ) {
		return NULL;
	}
	return &wstr[i];
}

/** 根据字符串中的标签得到相应的样式数据，并返回指向标签后面字符的指针 */
static const wchar_t *ScanStyleTagData( const wchar_t *wstr,
					LCUI_TextStyleTag *tag )
{
	const wchar_t *p, *q;
	char tag_data[256];

	p = wstr;
	if( (q = ScanStyleTagByName( p, L"color", tag_data )) ) {
		if( !ParseColor( &tag->style, tag_data ) ) {
			return NULL;
		}
		tag->id = TEXT_STYLE_COLOR;
		return q;
	}
	if( (q = ScanStyleTagByName( p, L"bgcolor", tag_data )) ) {
		if( !ParseColor( &tag->style, tag_data ) ) {
			return NULL;
		}
		tag->id = TEXT_STYLE_BG_COLOR;
		return q;
	}
	if( (q = ScanStyleTagByName( p, L"size", tag_data )) ) {
		if( !ParseNumber( &tag->style, tag_data ) ) {
			return NULL;
		}
		tag->id = TEXT_STYLE_SIZE;
		return q;
	}
	if( (q = ScanStyleTagByName( p, L"b", tag_data )) ) {
		tag->id = TEXT_STYLE_BOLD;
		return q;
	}
	if( (q = ScanStyleTagByName( p, L"i", tag_data )) ) {
		tag->id = TEXT_STYLE_ITALIC;
		return q;
	}
	return NULL;
}

/** 处理样式标签 */
const wchar_t *StyleTags_GetStart( LinkedList *tags, const wchar_t *str )
{
	const wchar_t *q;
	LCUI_TextStyleTag *tag = NEW( LCUI_TextStyleTag, 1 );
	q = ScanStyleTagData( str, tag );
	if( q ) {
		/* 将标签样式数据加入队列 */
		LinkedList_Insert( tags, 0, tag );
	} else {
		free( tag );
	}
	return q;
}

/** 处理样式结束标签 */
const wchar_t* StyleTags_GetEnd( LinkedList *tags, const wchar_t *str )
{
	const wchar_t *p;
	wchar_t tagname[256];
	/* 获取标签名 */
	p = ScanStyleEndingTag( str, tagname );
	if( !p ) {
		return NULL;
	}
	/* 删除相应的样式标签 */
	if( wcscmp( tagname, L"color" ) == 0 ) {
		StyleTags_Delete( tags, TEXT_STYLE_COLOR );
	} else if( wcscmp( tagname, L"bgcolor" ) == 0 ) {
		StyleTags_Delete( tags, TEXT_STYLE_BG_COLOR );
	} else if( wcscmp( tagname, L"size" ) == 0 ) {
		StyleTags_Delete( tags, TEXT_STYLE_SIZE );
	} else if( wcscmp( tagname, L"b" ) == 0 ) {
		StyleTags_Delete( tags, TEXT_STYLE_BOLD );
	} else if( wcscmp( tagname, L"i" ) == 0 ) {
		StyleTags_Delete( tags, TEXT_STYLE_ITALIC );
	} else {
		return NULL;
	}
	return p;
}

/*------------------------- End StyleTag -----------------------------*/
