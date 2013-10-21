/* ***************************************************************************
 * textstyle.c -- text style processing module.
 * 
 * Copyright (C) 2012-2013 by
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
 * textstyle.c -- 文本样式处理模块
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
#include LC_GRAPH_H
#include LC_FONT_H

/* 初始化字体样式数据 */
LCUI_API void
TextStyle_Init ( LCUI_TextStyle *data )
{
	data->_style = FALSE;
	data->_weight = FALSE;
	data->_decoration = FALSE;
	data->_family = FALSE;
	data->_back_color = FALSE;
	data->_fore_color = FALSE;
	data->font_id = FontLIB_GetDefaultFontID();
	data->style = FONT_STYLE_NORMAL;
	data->weight = FONT_WEIGHT_NORMAL;
	data->decoration = FONT_DECORATION_NONE;
	data->fore_color = RGB(0,0,0);
	data->back_color = RGB(255,255,255);
	data->pixel_size = 12;
}

/* 设置字体族 */
LCUI_API void
TextStyle_FontFamily( LCUI_TextStyle *style, const char *fontfamily )
{
	style->font_id = FontLIB_GetFontIDByFamilyName( fontfamily );
	style->_family = TRUE;
}

/* 设置字体族ID */
LCUI_API void
TextStyle_FontFamilyID( LCUI_TextStyle *style, int id )
{
	style->font_id = id;
	style->_family = TRUE;
}

/* 设置字体大小 */
LCUI_API void
TextStyle_FontSize( LCUI_TextStyle *style, int fontsize )
{
	style->pixel_size = fontsize;
	style->_pixel_size = TRUE;
}

/* 设置字体颜色 */
LCUI_API void
TextStyle_FontColor( LCUI_TextStyle *style, LCUI_RGB color )
{
	style->fore_color = color;
	style->_fore_color = TRUE;
}

/* 设置字体背景颜色 */
LCUI_API void
TextStyle_FontBackColor( LCUI_TextStyle *style, LCUI_RGB color )
{
	style->back_color = color;
	style->_back_color = TRUE;
}

/* 设置字体样式 */
LCUI_API void
TextStyle_FontStyle( LCUI_TextStyle *style, enum_font_style fontstyle )
{
	style->style = fontstyle;
	style->_style = TRUE;
}

LCUI_API void
TextStyle_FontWeight( LCUI_TextStyle *style, enum_font_weight fontweight ) 
{
	style->weight = fontweight;
	style->_weight = TRUE;
}

LCUI_API void
TextStyle_FontDecoration( LCUI_TextStyle *style, enum_font_decoration decoration )
/* 设置字体下划线 */
{
	style->decoration = decoration;
}

LCUI_API int
TextStyle_Cmp( LCUI_TextStyle *a, LCUI_TextStyle *b )
{
	return 0;
}

/*-------------------------- StyleTag --------------------------------*/
#define MAX_TAG_NUM 2

static void 
Destroy_StyleTag_Data( void *arg )
{ 
	//free( data->style );
}

/* 初始化样式标签库 */
LCUI_API void
StyleTag_Init( LCUI_Queue *tags )
{
	Queue_Init( tags, sizeof(StyleTag_Data), Destroy_StyleTag_Data );
}

/* 添加样式标签 */
LCUI_API int
StyleTag_Add( LCUI_Queue *tags, StyleTag_Data *data )
{
	return Queue_Add( tags, data );
}

/* 获取当前的样式数据 */
LCUI_API LCUI_TextStyle*
StyleTag_GetCurrentStyle ( LCUI_Queue *tags )
{
	PX_PT_t pxpt;
	StyleTag_Data *tag_data;
	LCUI_TextStyle *style_data;
	int i, total, equal = 0, flags[MAX_TAG_NUM];
	
	style_data = malloc (sizeof(LCUI_TextStyle));
	TextStyle_Init( style_data );
	memset( flags, 0, sizeof(flags) );
	
	total = Queue_GetTotal( tags );
	if(total <= 0) {
		free( style_data );
		return NULL;
	}
	
	/* 从样式数据队列中获取字体样式数据 */
	for(equal=0,i=total-1; i>=0; --i) {
		tag_data = Queue_Get( tags, i );
		DEBUG_MSG("tag id: %d\n", tag_data->tag);
		switch( tag_data->tag ) {
		    case TAG_ID_COLOR: 
			if( flags[0] != 0 ) {
				break;
			}
			style_data->_fore_color = TRUE;
			style_data->fore_color = *((LCUI_RGB*)tag_data->style);
			DEBUG_MSG("color: %d,%d,%d\n", data->fore_color.red,
			 data->fore_color.green, data->fore_color.blue);
			flags[0] = 1;
			++equal;
			break;
		    case TAG_ID_SIZE:
			if( flags[1] != 0 ) {
				break;
			}
			pxpt = *((PX_PT_t*)tag_data->style);
			style_data->_pixel_size = TRUE;
			style_data->pixel_size = pxpt.px;
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

static void 
StyleTag_Delete( LCUI_Queue *tags, StyleTag_ID tag )
/* 将指定标签的样式数据从队列中删除，只删除队列尾部第一个匹配的标签 */
{
	int i, total;
	StyleTag_Data *p; 
	 
	total = Queue_GetTotal( tags );
	DEBUG_MSG("delete start, total tag: %d\n", Queue_GetTotal( tags ));
	if(total <= 0) {
		return;
	}
	for(i=total-1; i>=0; --i) {
		p = Queue_Get( tags, i );
		if( p->tag == tag ) {
			Queue_Delete( tags, i );
			break;
		}
	} 
	DEBUG_MSG("delete end, total tag: %d\n", Queue_GetTotal( tags ));
}

LCUI_API void
clear_space(char *in, char *out)
/* 清除字符串中的空格 */
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

static wchar_t *
get_style_endtag ( wchar_t *str, char *out_tag_name )
/* 在字符串中获取样式的结束标签，输出的是标签名 */
{
	int i, j, len, tag_found = 0;
	
	len = wcslen ( str );
	//printf("string: %S\n", str);
	if(str[0] != '<' || str[1] != '/') { 
		return NULL;
	} 
	/* 匹配标签,获取标签名 */
	for(j=0,i=2; i<len; ++i) {
		switch(str[i]) {
		    case ' ': 
			if(  tag_found == 0 ) {
				break;
			} 
			return NULL;
		    case '>': goto end_tag_search;
		    default: out_tag_name[j] = str[i]; ++j; break;
		}
	}
	
end_tag_search:;

	out_tag_name[j] = 0;
	if( j < 1 ) { 
		return NULL;
	}
	return str+i;
}

static wchar_t *
get_style_tag( wchar_t *str, const char *tag, char *out_tag_data )
/* 在字符串中获取指定样式标签中的数据 */
{
	wchar_t *p;
	int i, j, len, tag_len; 
	
	len = wcslen ( str );
	DEBUG_MSG2("len = %d\n", len);
	tag_len = strlen ( tag );
	if(str[0] != '<') {
		DEBUG_MSG2("str[0] != '<'\n");
		return NULL;
	} 
	/* 匹配标签前半部分 */
	for(j=0,i=1; i<len; ++i) {
		if( str[i] == ' ' ) { 
			if( j == 0 || j >= tag_len ) { 
				continue;
			}
			return NULL;
		}
		else if( str[i] == tag[j] ) { 
			++j;
		} else { 
			/* 如果标签名部分已经匹配完 */
			if( j>= tag_len ) { 
				if( str[i] == '=' ) {
					++i; 
					break;
				}
			}
			/* 否则，有误 */
			return NULL;
		}
	}
	DEBUG_MSG2("tag is: %s\n", tag);
	/* 获取标签后半部分 */
	for(j=0; i<len; ++i) {
		DEBUG_MSG2("str[%d]: %c\n", i, str[i]);
		if( str[i] == ' ' ) {
			continue; 
		} else {
			/* 标签结束，退出 */
			if( str[i] == '>' ) {
				break;
			}
			/* 保存标签内的数据 */
			out_tag_data[j] = str[i];
			++j;
		}
	}
	out_tag_data[j] = 0;
	DEBUG_MSG2("out_tag_data: %s\n", out_tag_data);
	if(i >= len ) {
		return NULL;
	}
	p = &str[i];
	return p;
}

/* 根据字符串中的标签得到相应的样式数据，并返回指向标签后面字符的指针 */
static wchar_t *
covernt_tag_to_style_data (wchar_t *str, StyleTag_Data *out_data)
{
	wchar_t *p, *q; 
	char tag_data[256];
	
	p = str; 
	DEBUG_MSG("covernt_tag_to_style_data(): enter\n");
	if( (q = get_style_tag ( p, "color", tag_data)) ) {
		int r,g,b, len, i, j;
		LCUI_RGB rgb;
		
		p = q;
		DEBUG_MSG("is color style tag, data: %s\n", tag_data);
		len = strlen(tag_data); 
		for(j=0,i=0; i<len; ++i) {
			if(tag_data[i] == ',') {
				++j;
			}
		}
		if(j == 2) {
			sscanf( tag_data, "%d,%d,%d", &r, &g, &b ); 
		}
		else if(tag_data[0] == '#') {
			switch(len) {
			    case 4: 
				sscanf( tag_data, "#%1X%1X%1X", &r, &g, &b );
				r<<=4; g<<=4; b<<=4;
				break;
			    case 7:
				sscanf( tag_data, "#%2X%2X%2X", &r, &g, &b ); 
				break;
			    default:
				r=0; g=0; b=0;
				break;
			}
		} else {
			r=0; g=0; b=0;
		}
		DEBUG_MSG("color: %d,%d,%d\n", r,g,b);
		rgb = RGB(r, g, b);
		out_data->tag = TAG_ID_COLOR;
		out_data->style = malloc( sizeof(LCUI_RGB) );
		memcpy( out_data->style, &rgb, sizeof(LCUI_RGB) );
	}
	else if( (q = get_style_tag ( p, "size", tag_data)) ) {
		PX_PT_t pxpt;
		p = q;
		if( get_PX_PT_t( tag_data, &pxpt ) != 0) {
			return NULL;
		}
		out_data->tag = TAG_ID_SIZE;
		out_data->style = malloc( sizeof(PX_PT_t) );
		memcpy( out_data->style, &pxpt, sizeof(PX_PT_t) );
	} else {
		p = NULL;
	}
	DEBUG_MSG("covernt_tag_to_style_data(): quit\n");
	return p;
}

/* 处理样式标签 */
LCUI_API wchar_t*
StyleTag_ProcessTag( LCUI_Queue *tags, wchar_t *str )
{
	wchar_t *q;
	StyleTag_Data data;
	
	/* 开始处理样式标签 */
	q = covernt_tag_to_style_data ( str, &data );
	DEBUG_MSG2("handle_style_tag():%p\n", q);
	if( q ) {
		DEBUG_MSG2("add style data\n");
		/* 将标签样式数据加入队列 */
		StyleTag_Add( tags, &data ); 
	}
	return q;
}

/* 处理样式结束标签 */
LCUI_API wchar_t*
StyleTag_ProcessEndingTag( LCUI_Queue *tags, wchar_t *str )
{
	wchar_t *p;
	char tag_name[256];
	/* 获取标签名 */
	p = get_style_endtag( str, tag_name );
	if( p == NULL ) {
		return NULL;
	}
	if( LCUI_strcasecmpA(tag_name, "color") == 0 ) {
		/* 消除该标签添加的字体样式 */
		StyleTag_Delete ( tags, TAG_ID_COLOR );
	} 
	else if( LCUI_strcasecmpA(tag_name, "size") == 0 ) {
		/* 消除该标签添加的字体样式 */
		StyleTag_Delete ( tags, TAG_ID_SIZE );
	} else {
		return NULL;
	}
	return p;
}

/*------------------------- End StyleTag -----------------------------*/
