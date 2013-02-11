/* ***************************************************************************
 * textlayer.c -- text bitmap layer processing module.
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
 * textlayer.c -- 文本图层处理模块
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
//#define DEBUG
//#define DEBUG1
//#define DEBUG2
#include <LCUI_Build.h>

#include LC_LCUI_H 
#include LC_GRAPH_H
#include LC_WIDGET_H
#include LC_FONT_H

#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Special_KeyWord	Special_KeyWord;
typedef struct _tag_style_data	tag_style_data;

typedef enum _tag_id	enum_tag_id;

enum _tag_id
{
	TAG_ID_FAMILY = 0,
	TAG_ID_STYLE = 1,
	TAG_ID_WIEGHT = 2,
	TAG_ID_DECORATION = 3,
	TAG_ID_SIZE = 4,
	TAG_ID_COLOR = 5
};

/*************************** 特殊关键词 *****************************/
struct _Special_KeyWord
{
	LCUI_String keyword;		/* 关键词 */
	LCUI_TextStyle *data;		/* 该关键词使用的字体数据 */
	LCUI_Queue *text_source_data;	/* 关键词所属的源文本 */
};
/******************************************************************/

struct _tag_style_data 
{
	enum_tag_id tag;
	void *style;
};

/**************************** TextStyle *******************************/
void 
TextStyle_Init ( LCUI_TextStyle *data )
/* 初始化字体样式数据 */
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

void
TextStyle_FontFamily( LCUI_TextStyle *style, const char *fontfamily )
/* 设置字体族 */
{
	style->font_id = FontLIB_GetFontIDByFamilyName( fontfamily );
}

void
TextStyle_FontSize( LCUI_TextStyle *style, int fontsize )
/* 设置字体大小 */
{
	style->pixel_size = fontsize;
}

void
TextStyle_FontColor( LCUI_TextStyle *style, LCUI_RGB color )
/* 设置字体颜色 */
{
	style->fore_color = color;
}

void
TextStyle_FontBackColor( LCUI_TextStyle *style, LCUI_RGB color )
/* 设置字体背景颜色 */
{
	style->back_color = color;
}

void
TextStyle_FontStyle( LCUI_TextStyle *style, enum_font_style fontstyle )
/* 设置字体样式 */
{
	style->style = fontstyle;
}

void
TextStyle_FontWeight( LCUI_TextStyle *style, enum_font_weight fontweight ) 
{
	style->weight = fontweight;
}

void
TextStyle_FontDecoration( LCUI_TextStyle *style, enum_font_decoration decoration )
/* 设置字体下划线 */
{
	style->decoration = decoration;
}

int 
TextStyle_Cmp( LCUI_TextStyle *a, LCUI_TextStyle *b )
{
	return 0;
}

/************************** End TextStyle *****************************/

static void 
destroy_tag_style_data( tag_style_data *data )
{ 
	//free( data->style );
}

static void 
Destroy_Special_KeyWord(Special_KeyWord *key)
{
	/* 在该关键词所属的原文本中查找组成该关键词的字，并修改字所使用的字体数据 */
}

static void 
Destroy_CharData(LCUI_CharData *data)
{ 
	//FontBMP_Free( &data->bitmap );
	//free( data->data );
	//if( data->using_quote == IS_FALSE ) {
		//free( data->data );
	//}
}

static void 
Destroy_Text_RowData(Text_RowData *data)
{
	Destroy_Queue ( &data->string );
}


/*----------------------------- Tag Proc -----------------------------*/
static void 
TextLayer_TagStyle_Add( LCUI_TextLayer *layer, tag_style_data *data )
/* 将字体样式数据加入队列 */
{
	Queue_Add( &layer->tag_buff, data );
}

#define MAX_TAG_NUM 2

static LCUI_TextStyle *
TextLayer_Get_Current_TextStyle ( LCUI_TextLayer *layer )
/* 获取当前的字体样式数据 */
{
	int i, total, equal = 0,flags[MAX_TAG_NUM];
	LCUI_TextStyle *data;
	tag_style_data *p;
	
	data = (LCUI_TextStyle*) malloc (sizeof(LCUI_TextStyle));
	TextStyle_Init( data );
	memset( flags, 0, sizeof(flags) );
	total = Queue_Get_Total( &layer->tag_buff );
	if(total <= 0) {
		free( data );
		return NULL;
	}
	/* 从样式数据队列中获取字体样式数据 */
	for(equal=0,i=total-1; i>=0; --i) {
		p = Queue_Get( &layer->tag_buff, i );
		DEBUG_MSG("tag id: %d\n", p->tag);
		switch( p->tag ) {
		    case TAG_ID_COLOR: 
			if( flags[0] == 0 ) {
				data->_fore_color = TRUE;
				data->fore_color = *((LCUI_RGB*)p->style);
				DEBUG_MSG("color: %d,%d,%d\n", data->fore_color.red,
				 data->fore_color.green, data->fore_color.blue);
				flags[0] = 1;
				++equal;
			}
			break;
		    case TAG_ID_SIZE:
			if( flags[1] == 0 ) {
				PX_PT_t pxpt;
				pxpt = *((PX_PT_t*)p->style);
				data->_pixel_size = TRUE;
				data->pixel_size = pxpt.px;
				flags[1] = 1;
				++equal;
			}
			break;
		    default: break;
		}
		if(equal == MAX_TAG_NUM) {
			break;
		}
	}
	if( equal == 0 ) {
		free( data );
		return NULL;
	}
	return data;
}

static void 
TextLayer_TagStyle_Delete( LCUI_TextLayer *layer, enum_tag_id tag)
/* 将指定标签的样式数据从队列中删除，只删除队列尾部第一个匹配的标签 */
{
	int i, total;
	tag_style_data *p; 
	 
	total = Queue_Get_Total( &layer->tag_buff );
	DEBUG_MSG("delete start, total tag: %d\n", Queue_Get_Total( &layer->tag_buff ));
	if(total <= 0) {
		return;
	}
	for(i=total-1; i>=0; --i) {
		p = Queue_Get( &layer->tag_buff, i );
		if( p->tag == tag ) {
			Queue_Delete( &layer->tag_buff, i );
			break;
		}
	} 
	DEBUG_MSG("delete end, total tag: %d\n", Queue_Get_Total( &layer->tag_buff ));
}

void clear_space(char *in, char *out)
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
	wchar_t *p;
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
	p = &str[i];
	return p;
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

static wchar_t *
covernt_tag_to_style_data (wchar_t *str, tag_style_data *out_data)
/* 根据字符串中的标签得到相应的样式数据，并返回指向标签后面字符的指针 */
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

static wchar_t *
handle_style_tag( LCUI_TextLayer *layer, wchar_t *str )
{
	wchar_t *q;
	tag_style_data data;
	
	/* 开始处理样式标签 */
	q = covernt_tag_to_style_data ( str, &data );
	DEBUG_MSG2("handle_style_tag():%p\n", q);
	if( q ) {
		DEBUG_MSG2("add style data\n");
		/* 将标签样式数据加入队列 */
		TextLayer_TagStyle_Add( layer, &data ); 
	}
	return q;
}

static wchar_t *
handle_style_endtag( LCUI_TextLayer *layer, wchar_t *str )
/* 处理样式的结束标签 */
{
	wchar_t *p;
	char tag_name[256];
	/* 获取标签名 */
	p = get_style_endtag( str, tag_name );
	if( p == NULL ) {
		return NULL;
	}
	if( strcasecmp(tag_name, "color") == 0 ) {
		/* 消除该标签添加的字体样式 */
		TextLayer_TagStyle_Delete ( layer, TAG_ID_COLOR );
	} 
	else if( strcasecmp(tag_name, "size") == 0 ) {
		/* 消除该标签添加的字体样式 */
		TextLayer_TagStyle_Delete ( layer, TAG_ID_SIZE );
	} else {
		return NULL;
	}
	return p;
}

/*--------------------------- End Tag Proc ----------------------------*/

static void
TextLayer_Clear( 
	LCUI_TextLayer *layer,
	LCUI_Pos pos, 
	int max_h, 
	LCUI_CharData *char_ptr )
{
	static LCUI_Rect area;
	//printf("pos: %d,%d, max_h: %d\n", pos.x, pos.y, max_h);
	/* 计算区域范围 */
	area.x = pos.x + char_ptr->bitmap->left;
	area.y = pos.y + max_h-1;
	area.y -= char_ptr->bitmap->top;
	area.width = char_ptr->bitmap->width;
	area.height = char_ptr->bitmap->rows;
	/* 记录需刷新的区域 */
	RectQueue_Add( &layer->clear_area, area );
	//printf("record area: %d,%d,%d,%d\n", area.x, area.y, area.width, area.height);
}

static void 
TextLayer_Get_Char_BMP ( LCUI_TextStyle *default_style, LCUI_CharData *data )
/* 获取字体位图，字体的样式由文本图层中记录的字体样式决定 */
{
	int pixel_size;
	/* 获取字体尺寸 */
	if( !data->data ) {
		pixel_size = default_style->pixel_size;
	} else {
		/* 如果已经自定义字体尺寸 */
		if( data->data->_pixel_size ) {
			pixel_size = data->data->pixel_size; 
		} else {
			pixel_size = default_style->pixel_size;
		}
	}
	data->bitmap = Get_ExistFontBMP( default_style->font_id, 
			data->char_code, pixel_size );
}

static int 
TextLayer_Text_Add_NewRow ( LCUI_TextLayer *layer )
/* 添加新行 */
{
	Text_RowData data; 
	/* 单整行最大尺寸改变时，需要移动整行，目前还未支持此功能 */
	data.pos = Pos(0,0); 
	data.max_size = Size(0,0);
	data.last_char = NULL;
	Queue_Init( &data.string, sizeof(LCUI_CharData), NULL );
	/* 使用链表模式，方便数据的插入 */
	Queue_Set_DataMode( &data.string, QUEUE_DATA_MODE_LINKED_LIST );
	/* 队列成员使用指针，主要是引用text_source_data里面的数据 */
	Queue_Using_Pointer( &data.string );
	return Queue_Add( &layer->rows_data, &data );
}

static void 
TextLayer_Text_RowBreak ( 
	LCUI_TextLayer *layer, 
	Text_RowData *src, 
	int break_point, 
	Text_RowData *des )
/* 对目标行进行断行处理，也就是将目标行指定位置后面的全部文字转移到另一行 */
{
	static int i, total;
	static LCUI_CharData *char_ptr;
	
	total = Queue_Get_Total( &src->string );
	for(i=break_point; i<total; ++i ) {
		char_ptr = Queue_Get( &src->string, break_point );
		Queue_Add_Pointer( &des->string, char_ptr );
		char_ptr->need_update = TRUE;
		Queue_Delete_Pointer( &src->string, break_point );
	}
}

static int
TextLayer_Text_Insert_NewRow ( LCUI_TextLayer *layer, int row )
/* 在插入新行至指定位置 */
{
	Text_RowData data;
	
	data.pos = Pos(0,0); 
	data.max_size = Size(0,0);
	data.last_char = NULL;
	Queue_Init( &data.string, sizeof(LCUI_CharData), NULL ); 
	Queue_Set_DataMode( &data.string, QUEUE_DATA_MODE_LINKED_LIST ); 
	Queue_Using_Pointer( &data.string );
	return Queue_Insert( &layer->rows_data, row, &data );
}

static Text_RowData *
TextLayer_Get_Current_RowData ( LCUI_TextLayer *layer )
/* 获取指向当前行的指针 */
{
	return Queue_Get( &layer->rows_data, layer->current_des_pos.y );
}


static void
TextLayer_Update_RowSize (LCUI_TextLayer *layer, int row )
/* 更新指定行文本位图的尺寸 */
{
	int total, i;
	LCUI_Size size;
	LCUI_CharData *char_data;
	Text_RowData *row_data;
	LCUI_TextStyle *style;
	
	row_data = Queue_Get( &layer->rows_data, row );
	total = Queue_Get_Total( &row_data->string ); 
	style = TextLayer_Get_Current_TextStyle( layer ); 
	size = Size(0,14);
	if( !style ) {
		if(layer->default_data.pixel_size > 0) {
			size = Size(0, layer->default_data.pixel_size+2);
		}
	} else {
		if(style->pixel_size > 0) {
			size = Size(0, style->pixel_size+2);
		} 
	}
	free( style );
	for( i=0; i<total; ++i ) {
		/* 如果屏蔽字符有效，则使用该字符的数据 */
		if( layer->password_char.char_code > 0 ) {
			char_data = &layer->password_char;
		} else {
			char_data = Queue_Get( &row_data->string, i );
			if( !char_data ) {
				continue;
			}
		}
		size.w += char_data->bitmap->advance.x;
		if( char_data->data ) {
			if( char_data->data->_pixel_size ) {
				if( size.h < char_data->data->pixel_size + 2) {
					size.h = char_data->data->pixel_size + 2;
				}
			} else {
				if( size.h < 14) {
					size.h = 14;
				}
			}
		} else {
			if( size.h < 14) {
				size.h = 14;
			}
		}
	}
	row_data->max_size = size;
}

void 
TextLayer_Init( LCUI_TextLayer *layer )
/* 初始化文本图层相关数据 */
{
	layer->read_only = FALSE;
	layer->using_code_mode = FALSE; 
	layer->using_style_tags = FALSE; 
	layer->enable_word_wrap = FALSE; 
	layer->enable_multiline = FALSE;
	layer->need_proc_buff = FALSE;
	layer->need_scroll_layer = FALSE;
	layer->have_select = FALSE;
	layer->start = 0;
	layer->end = 0;
	layer->offset_pos = Pos(0,0);
	layer->old_offset_pos = Pos(0,0);
	
	Queue_Init( &layer->color_keyword, sizeof(Special_KeyWord), Destroy_Special_KeyWord );
	/* 队列中使用链表储存这些数据 */
	Queue_Init( &layer->text_source_data, sizeof(LCUI_CharData), Destroy_CharData );
	Queue_Set_DataMode( &layer->text_source_data, QUEUE_DATA_MODE_LINKED_LIST ); 
	Queue_Init( &layer->rows_data, sizeof(Text_RowData), Destroy_Text_RowData ); 
	Queue_Init( &layer->tag_buff, sizeof(tag_style_data), destroy_tag_style_data );
	Queue_Init( &layer->style_data, sizeof(LCUI_TextStyle), NULL );
	RectQueue_Init( &layer->clear_area );
	/* 初始化屏蔽符的数据 */
	layer->password_char.display = TRUE;
	layer->password_char.need_update = FALSE;
	layer->password_char.data = NULL;
	layer->password_char.char_code = 0;
	layer->password_char.bitmap = NULL;
	
	layer->default_data.pixel_size = 12;
	layer->current_src_pos = 0;
	layer->current_des_pos = Pos(0,0);
	layer->max_text_len = 5000; 
	TextStyle_Init ( &layer->default_data );
	
	String_Init( &layer->text_buff );
	//TextLayer_Text_Add_NewRow ( layer );/* 添加新行 */
}

void 
Destroy_TextLayer( LCUI_TextLayer *layer )
/* 销毁文本图层占用的资源 */
{
	Destroy_Queue( &layer->text_source_data );
	Destroy_Queue( &layer->rows_data );
	Destroy_Queue( &layer->tag_buff );
	Destroy_Queue( &layer->style_data );
	Destroy_Queue( &layer->clear_area );
	String_Free( &layer->text_buff );
}

static int
__TextLayer_Text( LCUI_TextLayer *layer )
/* 设定整个文本图层中需显示的文本，原有选中文本被删除 */
{
	if( !layer ) {
		return -1;
	}
	TextLayer_Text_Clear( layer );
	TextLayer_Text_Add( layer, layer->text_buff.string );
	return 0;
}

static void
__TextLayer_OldArea_Erase( LCUI_Widget *widget, LCUI_TextLayer *layer )
/* 
 * 功能：擦除文本图层的老区域
 * 说明：根据记录的旧偏移坐标，刷新部件区域内的文字所在区域。
 *  */
{
	static int i, j, x, y, rows, len;
	static LCUI_Graph *graph, slot;
	static Text_RowData *row_ptr;
	static LCUI_CharData *char_ptr;
	static LCUI_Rect area;
	
	Graph_Init( &slot );
	graph = Widget_GetSelfGraph( widget );
	rows = Queue_Get_Total( &layer->rows_data );
	for(y=layer->old_offset_pos.y,i=0; y<0 && i<rows; ++i) {
		row_ptr = Queue_Get( &layer->rows_data, i );
		if( !row_ptr ) {
			continue;
		}
		if( y + row_ptr->max_size.h >= 0 ) {
			break;
		}
		y += row_ptr->max_size.h;
	}
	for(; i<rows; ++i) {
		row_ptr = Queue_Get( &layer->rows_data, i );
		if( !row_ptr ) {
			continue;
		}
		len = Queue_Get_Total( &row_ptr->string );
		for(x=layer->old_offset_pos.x,j=0; x<0 && j<len; ++j) {
			char_ptr = Queue_Get( &row_ptr->string, j );
			if( !char_ptr ) {
				continue;
			}
			if( x+char_ptr->bitmap->advance.x >= 0 ) {
				break;
			}
			x += char_ptr->bitmap->advance.x;
		}
		
		area.height = layer->default_data.pixel_size+2;
		for( ; j<len; ++j ) {
			char_ptr = Queue_Get( &row_ptr->string, j );
			if( !char_ptr ) {
				continue;
			}
			area.y = row_ptr->max_size.h-1;
			area.y -= char_ptr->bitmap->top;
			if( area.y + char_ptr->bitmap->rows > area.height ) {
				area.height = area.y + char_ptr->bitmap->rows;
			}
			x += char_ptr->bitmap->advance.x;
			if( x > widget->size.w ) {
				break;
			}
		}
		area.x = 0;
		area.y = y;
		area.width = x;
		/* 引用部件图层中的区域 */
		Quote_Graph( &slot, graph, area );
		/* 将该区域的alpha通道填充为0 */
		Graph_Fill_Alpha( &slot, 0 );
		//printf("area: %d,%d,%d,%d\n", area.x, area.y, area.width, area.height);
		/* 添加刷新区域 */
		Widget_InvalidArea( widget, area );
		y += row_ptr->max_size.h;
		if( y > widget->size.h ) {
			break;
		}
	}
}

void 
TextLayer_Draw( LCUI_Widget *widget, LCUI_TextLayer *layer, int mode )
/* 将文本图层绘制到目标部件的图层上 */
{
	LCUI_Rect area;
	LCUI_Pos pos;
	BOOL draw_all = FALSE;
	int i, j, n, rows, size;
	LCUI_RGB color;
	LCUI_Graph slot, *graph;
	LCUI_CharData *p_data;
	Text_RowData *p_row;
	
	//clock_t start;
	//start = clock();
	//printf("TextLayer_Draw(): enter\n");
	
	graph = Widget_GetSelfGraph( widget );
	/* 如果文本缓存区内有数据 */
	if( layer->need_proc_buff ) {
		__TextLayer_Text( layer );
		layer->need_proc_buff = FALSE;
	}
	/* 如果需要滚动图层 */
	if( layer->need_scroll_layer ) {
		/* 根据之前记录的偏移坐标，刷新文本图层 */
		__TextLayer_OldArea_Erase( widget, layer );
		draw_all = TRUE;
		layer->need_scroll_layer = FALSE;
	}
	
	//nobuff_printf("1, use time: %ld\n", clock() - start );
	//start = clock();
	
	Graph_Init( &slot );
	/* 先处理需要清空的区域 */
	n = Queue_Get_Total( &layer->clear_area ); 
	for(i=0; i<n; ++i) { 
		RectQueue_Get( &area, 0 , &layer->clear_area ); 
		area.x += layer->offset_pos.x;
		area.y += layer->offset_pos.y;
		Queue_Delete( &layer->clear_area, 0 );
		Quote_Graph( &slot, graph, area );
		/* 将该区域的alpha通道填充为0 */
		Graph_Fill_Alpha( &slot, 0 );
		Widget_InvalidArea( widget, area ); 
		//printf("refresh area: %d,%d,%d,%d\n",
		//area.x, area.y, area.width, area.height);
	}
	//nobuff_printf("2, use time: %ld\n", clock() - start );
	//start = clock();
	/* 开始绘制文本位图至目标图层上 */
	rows = Queue_Get_Total( &layer->rows_data ); 
	for(pos.y=layer->offset_pos.y,i=0; i<rows; ++i) {
		p_row = Queue_Get( &layer->rows_data, i );
		if( !p_row ) {
			continue;
		}
		n = Queue_Get_Total( &p_row->string );
		/* 如果当前字的位图的Y轴跨距不在有效绘制区域内 */
		if( pos.y + p_row->max_size.h <= 0 ) {
			pos.y += p_row->max_size.h;
			continue;
		}
		for(pos.x=layer->offset_pos.x,j=0; j<n; ++j) {
			/* 如果设置了屏蔽符 */
			p_data = Queue_Get( &p_row->string, j ); 
			if( !p_data ) {
				continue;
			}
			
			if( layer->password_char.char_code > 0 ) {
				layer->password_char.need_update = p_data->need_update;
				p_data = &layer->password_char;
			}
			/* 如果当前字的位图的X轴跨距不在有效绘制区域内 */
			if( pos.x + p_data->bitmap->advance.x <= 0) {
				pos.x += p_data->bitmap->advance.x;
				continue;
			}
			/* 获取该字体位图的大致尺寸 */
			if( p_data->data ) {
				size = p_data->data->pixel_size;
				size += 2;
				color = p_data->data->fore_color;
			} else {
				size = layer->default_data.pixel_size + 2; 
				color = layer->default_data.fore_color;
			}
			/* 如果字体位图已标记更新，则绘制它 */
			if( p_data->need_update || draw_all ) { 
			//nobuff_printf("get, pos: %d, char_ptr: %p, char: %c, draw\n", 
			//	i, p_data, p_data->char_code ); 
				p_data->need_update = FALSE;
				/* 计算区域范围 */
				area.x = pos.x + p_data->bitmap->left;
				area.y = pos.y + p_row->max_size.h-1;
				area.y -= p_data->bitmap->top;
				area.height = p_data->bitmap->rows;
				area.width = p_data->bitmap->width;
				/* 贴上字体位图 */
				FontBMP_Mix( graph, Pos(area.x, area.y),
					p_data->bitmap, color, mode );
				/* 记录该区域，以刷新显示到屏幕上 */
				Widget_InvalidArea( widget, area );
			}
			pos.x += p_data->bitmap->advance.x;
			if( pos.x > widget->size.w ) {
				break;
			}
		}
		pos.y += p_row->max_size.h;
		if( pos.y > widget->size.h ) {
			break;
		}
	}
	//nobuff_printf("3, use time: %ld\n", clock() - start );
	//printf("TextLayer_Draw(): quit\n");
}

void
TextLayer_Refresh( LCUI_TextLayer *layer )
/* 标记文本图层中每个字的位图，等待绘制文本图层时进行更新 */
{
	int i, j;
	int rows, len;
	Text_RowData *row_ptr;
	LCUI_CharData *char_ptr;
	LCUI_Rect area;
	
	rows = Queue_Get_Total( &layer->rows_data );
	for(area.y=0,area.x=0,i=0; i<rows; ++i) {
		row_ptr = Queue_Get( &layer->rows_data, i );
		len = Queue_Get_Total( &row_ptr->string );
		for(j=0; j<len; ++j) {
			char_ptr = Queue_Get( &row_ptr->string, j );
			if( !char_ptr ) {
				continue;
			}
			char_ptr->need_update = TRUE; 
		}
		area.height = row_ptr->max_size.h;
		area.width = row_ptr->max_size.w;
		RectQueue_Add( &layer->clear_area, area );
		area.y += row_ptr->max_size.h;
	}
}

void
TextLayer_Set_Offset( LCUI_TextLayer *layer, LCUI_Pos offset_pos )
/* 设定文本图层的偏移位置 */
{
	/* 如果之前已经标记不需要滚动，那么就记录当前的偏移位置 */
	if( !layer->need_scroll_layer ) {
		layer->old_offset_pos = layer->offset_pos;
	}
	if( layer->offset_pos.x == offset_pos.x
	&& layer->offset_pos.y == offset_pos.y ) {
		return;
	}
	layer->offset_pos = offset_pos;
	layer->need_scroll_layer = TRUE;
}

/* 剪切板 */
//static LCUI_String clip_board;

LCUI_Size 
TextLayer_Get_Size ( LCUI_TextLayer *layer )
/* 获取文本图层的实际尺寸 */
{
	int i, rows;
	LCUI_Size size;
	Text_RowData *p_row; 
	
	rows = Queue_Get_Total( &layer->rows_data );
	for(size.w=0,size.h=0,i=0; i<rows; ++i) {
		p_row = Queue_Get( &layer->rows_data, i );
		if( !p_row ) {
			continue;
		}
		if( size.w < p_row->max_size.w ) {
			size.w = p_row->max_size.w;
		}
		size.h += p_row->max_size.h;
	}
	/* 尺寸稍微搞大一点，因为显示文本光标需要一定空间 */
	size.w += 2;
	size.h += 2;
	return size;
}

wchar_t *
TextLayer_Get_Text( LCUI_TextLayer *layer )
/* 获取文本图层中的文本内容 */
{
	int i, buff_size;
	wchar_t *text_buff;
	LCUI_CharData *char_p;
	buff_size = Queue_Get_Total( &layer->text_source_data );
	if( buff_size <= 0 ) {
		return NULL;
	}
	text_buff = (wchar_t*) malloc( sizeof(wchar_t)*(buff_size+1) );
	printf("print text:\n");
	for( i=0; i<buff_size; ++i ) {
		char_p = Queue_Get( &layer->text_source_data, i );
		printf("%c", char_p->char_code);
		text_buff[i] = char_p->char_code;
	}
	printf("\nend\n");
	text_buff[i] = 0;
	return text_buff;
}

void 
TextLayer_CharLater_Refresh( LCUI_TextLayer *layer, LCUI_Pos char_pos )
/* 刷新指定行中指定字以及后面的字的区域 */
{
	LCUI_Pos pos;
	int i, len;
	Text_RowData *row_ptr;
	LCUI_CharData *char_ptr;
	
	/* 获取该行文字的起点Y轴坐标 */
	for( pos.y=0,i=0; i<char_pos.y; ++i ) {
		row_ptr = Queue_Get( &layer->rows_data, i );
		if( !row_ptr ) {
			continue;
		}
		pos.y += row_ptr->max_size.h;
	}
	row_ptr = Queue_Get( &layer->rows_data, char_pos.y );
	len = Queue_Get_Total( &row_ptr->string );
	/* 获取该字的起点X轴坐标 */
	for( pos.x=0,i=0; i<char_pos.x; ++i ) {
		char_ptr = Queue_Get( &row_ptr->string, i ); 
		if( !char_ptr ) {
			continue;
		}
		pos.x += char_ptr->bitmap->advance.x;
	}
	
	for( i=char_pos.x; i<len; ++i ) {
		char_ptr = Queue_Get( &row_ptr->string, i );
		if( !char_ptr ) {
			continue;
		}
		
		TextLayer_Clear( layer, pos, row_ptr->max_size.h, char_ptr );
		/* 标记该字的位图需要重绘 */
		char_ptr->need_update = TRUE;
		pos.x += char_ptr->bitmap->advance.x;
	}
}

void
TextLayer_Text_Set_Default_Style( LCUI_TextLayer *layer, LCUI_TextStyle style )
/* 设定默认的文本样式，需要调用TextLayer_Draw函数进行文本位图更新 */
{
	LCUI_Pos pos;
	LCUI_CharData *char_ptr;
	LCUI_TextStyle *old_style;
	Text_RowData *row_ptr;
	int rows, len, i, j;
	
	layer->default_data = style; 
	rows = Queue_Get_Total( &layer->rows_data );
	for(pos.y=0,i=0; i<rows; ++i) {
		row_ptr = Queue_Get( &layer->rows_data, i ); 
		len = Queue_Get_Total( &row_ptr->string );
		for(pos.x=0,j=0; j<len; ++j) {
			char_ptr = Queue_Get( &row_ptr->string, j );
			if( !char_ptr ) {
				continue;
			}
			old_style = char_ptr->data;
			if( !old_style ) {
				char_ptr->need_update = TRUE; 
				goto skip_style_cmp;
			}
			/* 若有属性是缺省的 */
			if(!old_style->_pixel_size) {
				old_style->pixel_size = style.pixel_size;
				char_ptr->need_update = TRUE; 
			}
			if(!old_style->_style) {
				old_style->style = style.style;
				char_ptr->need_update = TRUE; 
			}
			if(!old_style->_family) {
				old_style->font_id = style.font_id;
				char_ptr->need_update = TRUE; 
			}
			if(!old_style->_weight) {
				old_style->weight = style.weight;
				char_ptr->need_update = TRUE; 
			}
			if(!old_style->_back_color) {
				old_style->back_color = style.back_color;
				char_ptr->need_update = TRUE; 
			}
			if(!old_style->_fore_color) {
				old_style->fore_color = style.fore_color;
				char_ptr->need_update = TRUE; 
			}
			if(!old_style->_decoration) {
				old_style->decoration = style.decoration;
				char_ptr->need_update = TRUE; 
			} 
skip_style_cmp:;
			if(char_ptr->need_update) {
				TextLayer_Clear( layer, pos, row_ptr->max_size.h, char_ptr );
				pos.x += char_ptr->bitmap->advance.x;
				TextLayer_Get_Char_BMP ( &layer->default_data, char_ptr );
			} else {
				pos.x += char_ptr->bitmap->advance.x;
			}
		}
		pos.y += row_ptr->max_size.h; 
		TextLayer_Update_RowSize( layer, i );
	}
}

void 
TextLayer_ReadOnly( LCUI_TextLayer *layer, BOOL flag )
/* 指定文本图层中的文本是否为只读 */
{
	layer->read_only = flag;
}

void
TextLayer_Text_Clear( LCUI_TextLayer *layer )
/* 清空文本内容 */
{
	TextLayer_Refresh( layer );
	Destroy_Queue( &layer->text_source_data );
	Destroy_Queue( &layer->rows_data );
	Destroy_Queue( &layer->style_data );
	layer->current_src_pos = 0;
	layer->current_des_pos = Pos(0,0);
}

void
TextLayer_Row_Set_End( LCUI_TextLayer *layer, uint_t row, uint_t start_cols )
/* 为指定行设定结束点，结束点及后面的数据将被删除，但不记录残余文本位图区域 */
{
	uint_t total, i;
	Text_RowData *row_data; 
	
	row_data = Queue_Get( &layer->rows_data, row );
	total = Queue_Get_Total( &row_data->string );
	/* 移除多余的数据 */
	for(i=start_cols; i<total; ++i) {
		Queue_Delete( &row_data->string, start_cols ); 
	}
}

int
TextLayer_Text_Get_Length( LCUI_TextLayer *layer )
/* 获取文本位图中的文本长度 */
{
	return Queue_Get_Total( &layer->text_source_data );
}

void
TextLayer_Text_Set_MaxLength( LCUI_TextLayer *layer, int max )
/* 设定文本位图中的文本长度 */
{
	if( max > 0 ) {
		layer->max_text_len = max;
	}
	// 对现有文本进行截断处理，暂不添加添加
}

void 
TextLayer_Text_Set_PasswordChar( LCUI_TextLayer *layer, wchar_t ch )
/* 
 * 设置屏蔽字符，设置后，文本框内的文本都会显示成该字符
 * 如果ch的值为0，则不对文本框里的文本进行屏蔽 
 * */
{
	layer->password_char.char_code = ch;
	TextLayer_Get_Char_BMP( &layer->default_data, &layer->password_char );
	//暂时不进行其它处理
}

void
TextLayer_Text_Process( LCUI_TextLayer *layer, int pos_type, char *new_text )
/* 对文本进行预处理，处理后的数据保存至layer里 */ 
{
	BOOL refresh = TRUE;
	LCUI_Pos cur_pos, des_pos;
	int total, cur_len, row, src_pos, total_row, n_ignore = 0;
	wchar_t *finish, *buff, *p, *q;
	
	LCUI_Pos tmp_pos;
	LCUI_CharData *char_ptr, char_data; 
	Text_RowData *cur_row_ptr, *tmp_row_ptr;
	
	/* 如果有选中的文本，那就删除 */
	//......  
	DEBUG_MSG1("enter\n");
	/* 如果是将文本追加至文本末尾 */
	if( pos_type == AT_TEXT_LAST ) {
		cur_pos.y = Queue_Get_Total( &layer->rows_data );
		if( cur_pos.y > 0 ) {
			--cur_pos.y;
		}
		cur_row_ptr = Queue_Get( &layer->rows_data, cur_pos.y );
		if( !cur_row_ptr ) {
			TextLayer_Text_Add_NewRow( layer );
			cur_row_ptr = Queue_Get( &layer->rows_data, cur_pos.y );
		}
		cur_pos.x = Queue_Get_Total( &cur_row_ptr->string );
		src_pos = Queue_Get_Total( &layer->text_source_data );
		des_pos = cur_pos;
	} else {/* 否则，是将文本插入至光标所在位置 */
		cur_pos = TextLayer_Cursor_GetPos( layer );
		DEBUG_MSG1( "cur_pos: %d,%d\n", cur_pos.x, cur_pos.y );
		cur_row_ptr = Queue_Get( &layer->rows_data, cur_pos.y );
		DEBUG_MSG1( "cur_row_ptr: %p\n", cur_row_ptr );
		if( !cur_row_ptr ) {
			TextLayer_Text_Add_NewRow( layer );
			cur_row_ptr = Queue_Get( &layer->rows_data, cur_pos.y );
		}
		src_pos = layer->current_src_pos;
		des_pos = layer->current_des_pos;
		DEBUG_MSG1( "src_pos: %d\n", src_pos );
		DEBUG_MSG1( "des_pos: %d,%d\n", des_pos.x, des_pos.y );
	}
	row = cur_pos.y;
	
	total = Char_To_Wchar_T( new_text, &buff ); 
	total_row = TextLayer_Get_Rows( layer );
	/* 判断当前要添加的字符的总数是否超出最大限制 */
	cur_len = Queue_Get_Total( &layer->text_source_data );
	if( total + cur_len > layer->max_text_len ) {
		total = layer->max_text_len - cur_len;
	}
	if( total < 0 ) {
		total = 0;
	}
	//_DEBUG_MSG( "layer: %p, cur total: %d, max_len: %d\n", 
	// layer, cur_len, layer->max_text_len );
	DEBUG_MSG1( "total char: %d\n", total );
	DEBUG_MSG1( "total row: %d\n", total_row );
	
	char_data.bitmap = NULL;
	/* 先记录这一行需要刷新的区域，起点为光标所在位置 */
	TextLayer_CharLater_Refresh( layer, cur_pos );
	for(p=buff, finish=buff+total; p<finish; ++p) { 
		DEBUG_MSG2( "1, char: %c\n", *p );
		if( layer->using_style_tags ) {
			/* 处理样式的结束标签 */ 
			q = handle_style_endtag ( layer, p );
			if( q ) {
				/* 计算需忽略的字符数 */
				n_ignore = q-p+1;
			} else {
				/* 处理样式标签 */
				q = handle_style_tag ( layer, p ); 
				if( q ) {
					n_ignore = q-p+1;
				}
			}
		}
		/* 针对换行符模式为Win(CR/LF)的文本，进行处理 */
		if(*p == '\n' || *p == '\r') { 
			/* 计算需要忽略的换行符的数量 */
			for( n_ignore=0,q=p; *q == '\n' || *q == '\r'; 
				++q,++n_ignore);
		} 
		if(n_ignore > 0) {
			/* 被忽略的字符的属性都一样，所以只需赋一次值 */
			char_data.data = NULL;
			char_data.display = FALSE; 
			char_data.need_update = FALSE;
			char_data.bitmap = NULL;
		}
		while(n_ignore > 0) {
			DEBUG_MSG2( "ignore = %d\n", n_ignore );
			char_data.char_code = *p++;
			Queue_Insert( &layer->text_source_data, src_pos, &char_data ); 
			char_ptr = Queue_Get( &layer->text_source_data, src_pos );
			/* 如果启用多行显示，并遇到换行符，那就增加新行 */
			if( layer->enable_multiline 
			 && char_data.char_code == '\n' ) {
				++row;
				if( refresh ) {
					tmp_pos.x = 0;
					tmp_pos.y = row;
					/* 刷新该行后面所有行的字符 */
					for( ; tmp_pos.y<total_row; ++tmp_pos.y ) {
						TextLayer_CharLater_Refresh( layer, tmp_pos );
					}
					refresh = FALSE;
				}
				/* 换行符的数据单独保存 */
				cur_row_ptr->last_char = char_ptr;
				/* 在该行插入新行 */
				TextLayer_Text_Insert_NewRow( layer, row );
				/* 获取新行的行指针 */
				tmp_row_ptr = Queue_Get( &layer->rows_data, row );
				/* 当前位置后面的字符转移到新行里 */
				TextLayer_Text_RowBreak( layer, cur_row_ptr, 
				 des_pos.x, tmp_row_ptr );
				/* 行指针切换到下一行 */
				cur_row_ptr = tmp_row_ptr;
				/* 更新当前字符坐标 */
				des_pos.x = 0;
				des_pos.y = row;
				/* 更新总行数 */
				total_row = TextLayer_Get_Rows( layer );
			}
			++src_pos; 
			--n_ignore;
			if(n_ignore == 0) {
				n_ignore = -1;
				break;
			}
		}
		if(n_ignore == -1) {
			--p; n_ignore = 0;
			continue;
		}
		DEBUG_MSG2( "2, char: %c\n", *p );
		char_data.char_code = *p;
		char_data.display = TRUE; 
		char_data.need_update = TRUE; 
		char_data.data = TextLayer_Get_Current_TextStyle( layer );
		/* 插入至源文本中 */
		Queue_Insert( &layer->text_source_data, src_pos, &char_data );
		/* 获取源文本中的字符数据的指针 */
		char_ptr = Queue_Get( &layer->text_source_data, src_pos );
		/* 将该指针添加至行数据队列中 */
		cur_row_ptr = Queue_Get( &layer->rows_data, des_pos.y );
		Queue_Insert_Pointer( &cur_row_ptr->string, des_pos.x, char_ptr );
		++src_pos; ++des_pos.x; 
	}
	/* 释放用于临时储存Wchar_T字符的空间 */
	free( buff );
	
	if( pos_type == AT_CURSOR_POS ) {
		layer->current_des_pos = des_pos;
		layer->current_src_pos = src_pos;
	}
	DEBUG_MSG1("quit\n");
}

void
TextLayer_Text_GenerateBMP( LCUI_TextLayer *layer )
/* 为文本图层中的文本生成位图，已存在位图的文字将不重新生成 */
{
	BOOL refresh = FALSE;
	LCUI_Pos pos;
	int i, j, len, rows;
	Text_RowData *row_ptr;
	LCUI_CharData *char_ptr;
	
	DEBUG_MSG1("enter\n");
	DEBUG_MSG1("thread: %lu\n", thread_self());
	rows = Queue_Get_Total( &layer->rows_data );
	for( pos.y=0,j=0; j<rows; ++j ) {
		row_ptr = Queue_Get( &layer->rows_data, j );
		len = Queue_Get_Total( &row_ptr->string );
		DEBUG_MSG1("row %d, len: %d\n", j, len);
		for( pos.x=0,i=0; i<len; ++i) {
			char_ptr = Queue_Get( &row_ptr->string, i );
			DEBUG_MSG1("generate FontBMP, get char_ptr: %p, char: %c\n", 
					char_ptr, char_ptr->char_code );
			if( !char_ptr || !char_ptr->display ) {
				DEBUG_MSG1("no display\n");
				continue;
			}
			if( FontBMP_Valid( char_ptr->bitmap ) ) {
				DEBUG_MSG1("have FontBMP\n");
				if( !refresh ) {
					pos.x += char_ptr->bitmap->advance.x;
					continue;
				}
			} else {
				refresh = TRUE;
				DEBUG_MSG1( "generate FontBMP, char code: %d\n", char_ptr->char_code );
				TextLayer_Get_Char_BMP ( &layer->default_data, char_ptr );
			}
			DEBUG_MSG1( "char_data->bitmap->advance.x: %d\n", char_ptr->bitmap->advance.x );
			TextLayer_Clear( layer, pos, row_ptr->max_size.h, char_ptr );
			char_ptr->need_update = TRUE;
			pos.x += char_ptr->bitmap->advance.x;
		}
		refresh = FALSE;
		/* 更新当前行的尺寸 */
		TextLayer_Update_RowSize( layer, j );
		DEBUG_MSG1("row size: %d,%d\n", row_ptr->max_size.w, row_ptr->max_size.h);
		pos.y += row_ptr->max_size.h;
	}
	DEBUG_MSG1("quit\n");
}

void 
TextLayer_Print_Info( LCUI_TextLayer *layer )
/* 打印文本图层信息 */
{
	int32_t i, j, len, rows;
	Text_RowData *row_ptr;
	LCUI_CharData *char_ptr;
	
	printf( "layer: %p\n", layer );
	rows = Queue_Get_Total( &layer->rows_data );
	for(j=0; j<rows; ++j) {
		row_ptr = Queue_Get( &layer->rows_data, j );
		len = Queue_Get_Total( &row_ptr->string );
		printf( "row[%d/%d], len: %d\n", j, rows, len );
		for(i=0; i<len; ++i) {
			char_ptr = Queue_Get( &row_ptr->string, i );
			printf( "char code: %d, display: %d\n", 
			char_ptr->char_code, char_ptr->display );
			Print_FontBMP_Info( char_ptr->bitmap );
		}
	}
	printf("\n\n");
}

void
TextLayer_Text( LCUI_TextLayer *layer, char *new_text )
/* 
 * 功能：设定整个文本图层中需显示的文本
 * 说明：文本将被储存至缓冲区，等待绘制文本位图时再处理缓冲区内的文本
 *  */
{
	/* 将文本存储至缓冲区 */
	Strcpy( &layer->text_buff, new_text );
	/* 标记，需要处理缓冲区 */
	layer->need_proc_buff = TRUE;
}

int 
TextLayer_Text_Append( LCUI_TextLayer *layer, char *new_text )
/* 在文本末尾追加文本，不移动光标，不删除原有选中文本 */
{
	TextLayer_Text_Process( layer, AT_TEXT_LAST, new_text );
	TextLayer_Text_GenerateBMP( layer );
	return 0;
}

int 
TextLayer_Text_Add( LCUI_TextLayer *layer, char *new_text )
/* 在光标处添加文本，如有选中文本，将被删除 */
{
	TextLayer_Text_Process( layer, AT_CURSOR_POS, new_text );
	TextLayer_Text_GenerateBMP( layer );
	return 0;
}

int 
TextLayer_Text_Paste( LCUI_TextLayer *layer )
/* 将剪切板的内容粘贴至文本图层 */
{
	return 0;
}

static int
TextLayer_Get_CharPos( 
	LCUI_TextLayer *layer, 
	LCUI_CharData *char_ptr, 
	int left_or_right )
/* 获取指定字符数据在源文本中的位置 */
{
	int n, src_pos, total;
	LCUI_CharData *tmp_ptr;
	
	if( !char_ptr ) {
		return -1;
	}
	src_pos = layer->current_src_pos;
	total = Queue_Get_Total( &layer->text_source_data );
	//printf( "source text len: %d\n", total );
	/* 确保起点位置有效 */
	if( src_pos >= total ) {
		src_pos = total-1;
	}
	if( src_pos < 0 ) {
		src_pos = 0;
	}
	/* 判断遍历方向 */
	if( left_or_right == 0 ) {
		goto left_search;
	} else {
		goto right_search;
	}
	/* 确定该字在源文本中的位置 */
left_search:
	//printf( "left, char_ptr: %p, char_code: %c\n", 
	//	char_ptr, char_ptr?char_ptr->char_code:'?' );
	for( n=src_pos; n>=0 ;--n ) {
		tmp_ptr = Queue_Get( &layer->text_source_data, n );
		//printf( "get, char_ptr: %p, char_code: %c\n", 
		//	tmp_ptr, tmp_ptr?tmp_ptr->char_code:'?' );
		if( tmp_ptr == char_ptr ) {
			break;
		}
	}
	goto end_search;
	
right_search:
	//printf( "right, char_ptr: %p, char_code: %c\n", 
	//	char_ptr, char_ptr?char_ptr->char_code:'?' );
	for( n=src_pos; n<total; ++n ) {
		tmp_ptr = Queue_Get( &layer->text_source_data, n );
		//printf( "get, char_ptr: %p, char_code: %c\n", 
		//	tmp_ptr, tmp_ptr?tmp_ptr->char_code:'?' );
		if( tmp_ptr == char_ptr ) {
			break;
		}
	}
	goto end_search;
	
end_search:
	/* 检测遍历结果 */
	if( left_or_right == 0 ) {
		if( n < 0 ) {
			goto right_search;
		}
		/* 未找到则返回-1 */
		if( n >= total ) {
			return -1;
		}
	} else {
		if( n < 0 ) {
			return -1;
		}
		if( n >= total ) {
			goto left_search;
		}
	}
	return n;
}


static LCUI_CharData *
TextLayer_Get_CurChar( LCUI_TextLayer *layer )
/* 获取光标附近的字符数据的指针 */
{
	int total;
	LCUI_Pos pos;
	LCUI_CharData *char_ptr;
	Text_RowData *row_ptr;
	
	pos = TextLayer_Cursor_GetPos( layer );
	row_ptr = Queue_Get( &layer->rows_data, pos.y );
	if( !row_ptr ) {
		return NULL;
	}
	char_ptr = Queue_Get( &row_ptr->string, pos.x );
	total = Queue_Get_Total( &row_ptr->string );
	if( !char_ptr ) {
		/* 如果当前光标在这行行尾 */
		if( pos.x == total ) {
			char_ptr = row_ptr->last_char;
		} else {
			return NULL;
		}
	}
	return char_ptr;
}


static int
TextLayer_Get_Cursor_CharPos( LCUI_TextLayer *layer, int left_or_right )
/* 根据光标当前所在位置，获取对于源字符串中的位置 */
{
	LCUI_CharData *char_ptr;
	
	char_ptr = TextLayer_Get_CurChar( layer );
	return TextLayer_Get_CharPos( layer, char_ptr, left_or_right );
}

static int
TextLayer_Update_CurSrcPos( LCUI_TextLayer *layer, int left_or_right )
/* 更新当前光标所在的字符 对应于源文本中的位置 */
{
	int pos, max;
	pos = TextLayer_Get_Cursor_CharPos( layer, left_or_right );
	max = Queue_Get_Total( &layer->text_source_data );
	//printf("pos: %d, max: %d\n", pos, max );
	if( pos == -1 || pos > max ) {
		pos = max;
	}
	if( pos >= 0 ) {
		layer->current_src_pos = pos;
	}
	//printf( "layer->current_src_pos: %d\n", layer->current_src_pos );
	return layer->current_src_pos;
}


LCUI_Pos 
TextLayer_Set_Cursor_PixelPos( LCUI_TextLayer *layer, LCUI_Pos pixel_pos )
/* 
 * 功能：根据传入的二维坐标，设定光标在的文本图层中的位置
 * 说明：该位置会根据当前位置中的字体位图来调整，确保光标显示在字体位图边上，而不
 * 会遮挡字体位图；光标在文本图层中的位置改变后，在字符串中的位置也要做相应改变，
 * 因为文本的添加，删减，都需要以光标当前所在位置对应的字符为基础。
 * 返回值：文本图层中对应字体位图的坐标，单位为像素
 *  */
{
	LCUI_Pos new_pos, pos;
	Text_RowData *row_ptr;
	LCUI_CharData *char_ptr;
	int i, n, rows, cols, tmp;
	
	pos.x = pos.y = 0;
	rows = Queue_Get_Total( &layer->rows_data );
	/* 减去偏移坐标 */
	pixel_pos = Pos_Sub( pixel_pos, layer->offset_pos );
	for( new_pos.y=0,i=0; i<rows; ++i ) {
		row_ptr = Queue_Get( &layer->rows_data, i );
		if( pixel_pos.y >= row_ptr->max_size.h ) {
			pixel_pos.y -= row_ptr->max_size.h; 
			if(i < rows-1 ) {
				new_pos.y += row_ptr->max_size.h; 
				continue;
			}
		}
		break;
	}
	pos.y = i;
	row_ptr = Queue_Get( &layer->rows_data, i );
	if( !row_ptr ) {
		cols = 0;
	} else {
		cols = Queue_Get_Total( &row_ptr->string );
	}
	for( new_pos.x=0,n=0; n<cols; ++n ) {
		char_ptr = Queue_Get( &row_ptr->string, n );
		if( !char_ptr ) {
			continue;
		}
		tmp = char_ptr->bitmap->advance.x;
		if( pixel_pos.x >= tmp/2 ) {
			pixel_pos.x -= tmp;
			new_pos.x += tmp;
			if(n < cols-1 || (n == cols-1 && pixel_pos.x >= 0)) {
				continue;
			}
		}
		break;
	}
	pos.x = n;
	/* 判断光标位置变化，以快速确定当前字符在源文本中的位置并更新 */
	if( layer->current_des_pos.y > pos.y 
	 || (layer->current_des_pos.y == pos.y 
	 && layer->current_des_pos.x > pos.x)) {
		layer->current_des_pos = pos;
		 /* 优先向左边遍历 */
		TextLayer_Update_CurSrcPos( layer, 0 );
	} 
	else if( layer->current_des_pos.y == pos.y 
		&& layer->current_des_pos.x == pos.x );
	else { 
		layer->current_des_pos = pos;
		/* 优先向右边遍历 */
		TextLayer_Update_CurSrcPos( layer, 1 );
	}
	/* 加上偏移坐标 */
	new_pos = Pos_Add( new_pos, layer->offset_pos );
	return new_pos;
}

LCUI_Pos
TextLayer_Cursor_SetPos( LCUI_TextLayer *layer, LCUI_Pos pos )
/* 设定光标在文本框中的位置，并返回该光标的坐标，单位为像素 */
{
	LCUI_Pos pixel_pos;
	Text_RowData *row_ptr;
	LCUI_CharData *char_ptr;
	int rows, cols, total;
	
	pixel_pos.x = pixel_pos.y = 0;
	total = Queue_Get_Total( &layer->rows_data );
	if( pos.y >= total ) {
		pos.y = total-1;
	}
	if( pos.y < 0 ) {
		pos.y = 0;
	}
	/* 累加pos.y行之前几行的高度 */
	for( pixel_pos.y=0,rows=0; rows<pos.y; ++rows ) {
		row_ptr = Queue_Get( &layer->rows_data, rows );
		if( !row_ptr ) {
			continue;
		}
		pixel_pos.y += row_ptr->max_size.h;
	}
	/* 获取当前行的指针 */
	row_ptr = Queue_Get( &layer->rows_data, rows );
	if( !row_ptr ) {
		return pixel_pos;
	}
	/* 获取当前行的文字数 */
	total = Queue_Get_Total( &row_ptr->string ); 
	if( pos.x > total ) {
		pos.x = total;
	}
	if( pos.x < 0 ) {
		pos.x = 0;
	}
	/* 累计宽度 */
	for( pixel_pos.x=0,cols=0; cols<pos.x; ++cols ) {
		char_ptr = Queue_Get( &row_ptr->string, cols );
		if( !char_ptr ) {
			continue;
		}
		pixel_pos.x += char_ptr->bitmap->advance.x;
		//printf("TextLayer_Cursor_SetPos(): pixel pos x: %d, total: %d, cols: %d, pos.x: %d\n", 
		//pixel_pos.x, total, cols, pos.x);
	}
	//printf("layer->current_des_pos: %d,%d,  pos: %d,%d\n",
	//layer->current_des_pos.x, layer->current_des_pos.y, pos.x, pos.y );
	if( layer->current_des_pos.y > pos.y 
	 || (layer->current_des_pos.y == pos.y 
	 && layer->current_des_pos.x > pos.x)) {
		layer->current_des_pos = pos;
		TextLayer_Update_CurSrcPos( layer, 0 );
	} 
	else if( layer->current_des_pos.y == pos.y 
		&& layer->current_des_pos.x == pos.x );
	else {
		layer->current_des_pos = pos;
		TextLayer_Update_CurSrcPos( layer, 1 );
	}
	layer->current_des_pos = pos;
	/* 加上偏移坐标 */
	pixel_pos = Pos_Add( pixel_pos, layer->offset_pos );
	/* 微调位置 */
	pixel_pos.y += 2;
	return pixel_pos;
}


int 
TextLayer_Text_Delete( LCUI_TextLayer *layer, int n )
/* 删除光标右边处n个字符 */
{
	return 0;
}

static int
TextLayer_Text_DeleteChar( 
		LCUI_TextLayer *layer, 
		LCUI_CharData *char_ptr, 
		int left_or_right )
/* 
 * 功能：将字符数据从源文本中移除
 * 参数：
 * layer         操作对象
 * char_ptr      指向所要删除的字符数据的指针
 * left_or_right 指定在源文本中遍历的方向，值为0时是向左遍历，其它值则像右遍历
 *  */
{
	int n;
	n = TextLayer_Get_CharPos( layer, char_ptr, left_or_right );
	/* 将该字从源文本中移除 */
	Queue_Delete( &layer->text_source_data, n );
	return 0;
}


static int 
_TextLayer_Text_Delete ( LCUI_TextLayer *layer, LCUI_Pos start_pos, int len )
/* 以start_pos为起点，删除n个文字 */
{
	BOOL refresh = TRUE;
	LCUI_CharData *char_ptr;
	LCUI_Pos tmp_pos, pixel_pos;
	int left_or_right, rows, cols;
	Text_RowData *row_ptr, *tmp_row;
	
	if( start_pos.x < 0 ) {
		len += start_pos.x;
		start_pos.x = 0;
	}
	if( start_pos.y < 0 ) {
		start_pos.y = 0;
	}
	if( len <= 0 ) {
		return -1;
	}
	/* 确定起点位置的XY轴坐标 */
	pixel_pos = TextLayer_Char_GetPixelPos( layer, start_pos );
	
	rows = Queue_Get_Total( &layer->rows_data );
	row_ptr = Queue_Get( &layer->rows_data, start_pos.y );
	if( !row_ptr ) {
		return -1;
	}
	cols = Queue_Get_Total( &row_ptr->string );
	
	/* 根据光标所在位置，确定遍历方向 */
	if( layer->current_des_pos.y > start_pos.y 
	 || (layer->current_des_pos.y == start_pos.y 
	 && layer->current_des_pos.x > start_pos.x)) {
		left_or_right = 0;
	}
	else {
		left_or_right = 1;
	}
	
	/* 如果需删除的字符只在当前行 */
	if( start_pos.x + len <= cols ) {
		/* 标记后面的文字位图需要刷新 */
		TextLayer_CharLater_Refresh( layer, start_pos );
	}
	for( ; start_pos.x<=cols && len>0; --len ) {
		/* 如果到了行尾 */
		if( start_pos.x == cols ) {
			/* 如果当前行是最后一行 */
			if( start_pos.y >= rows-1 ) {
				break;
			}
			if( refresh ) {
				tmp_pos.x = 0;
				tmp_pos.y=start_pos.y+1;
				/* 刷新该行后面所有行的字符 */
				for( ; tmp_pos.y<rows; ++tmp_pos.y ) {
					TextLayer_CharLater_Refresh( layer, tmp_pos );
				}
				refresh = FALSE;
			}
			
			/* 将当前行行尾的换行符'\n'从源文本中移除 */
			TextLayer_Text_DeleteChar( layer, row_ptr->last_char, left_or_right );
			/* 获取指向下一行文本的指针 */
			tmp_row = Queue_Get( &layer->rows_data, start_pos.y+1 );
			/* 将下一行的文本拼接至当前行行尾 */
			Queue_Cat( &row_ptr->string, &tmp_row->string );
			/* 将下一行的行尾字符数据转移至当前行 */
			row_ptr->last_char = tmp_row->last_char;
			/* 销毁下一行的文本 */
			Destroy_Queue( &tmp_row->string ); 
			Queue_Delete( &layer->rows_data, start_pos.y+1 );
			/* 更新当前行的总字符数 */
			cols = Queue_Get_Total( &row_ptr->string );
			/* 更新总行数 */
			rows = Queue_Get_Total( &layer->rows_data );
			/* 更新当前行的尺寸 */
			TextLayer_Update_RowSize( layer, start_pos.y );
			continue;
		}
		char_ptr = Queue_Get( &row_ptr->string, start_pos.x );
		if( !char_ptr ) {
			continue;
		}
		TextLayer_Clear( layer, pixel_pos, row_ptr->max_size.h, char_ptr );
		pixel_pos.x += char_ptr->bitmap->advance.x;
		/* 将该字从源文本中移除 */
		TextLayer_Text_DeleteChar( layer, char_ptr, left_or_right );
		/* 该字在这行的字体位图也需要删除 */
		cols = Queue_Get_Total( &row_ptr->string );
		Queue_Delete_Pointer( &row_ptr->string, start_pos.x );
		cols = Queue_Get_Total( &row_ptr->string );
		char_ptr = Queue_Get( &row_ptr->string, start_pos.x );
		cols = Queue_Get_Total( &row_ptr->string );
	}
	/* 更新当前行的尺寸 */
	TextLayer_Update_RowSize( layer, start_pos.y );
	return 0;
}

int 
TextLayer_Text_Backspace( LCUI_TextLayer *layer, int n )
/* 删除光标左边处n个字符 */
{
	int i, row_len;
	LCUI_Pos char_pos;
	Text_RowData *row_ptr;
	
	if( layer->read_only ) {
		return -1;
	}
	
	if( n <= 0 ) {
		return -2;
	}
	/* 计算当前光标所在字的位置 */
	char_pos = TextLayer_Cursor_GetPos( layer );
	DEBUG_MSG2( "before: %d,%d\n", char_pos.x, char_pos.y );
	for( i=n; char_pos.y>=0; --char_pos.y ) {
		row_ptr = Queue_Get( &layer->rows_data, char_pos.y );
		row_len = Queue_Get_Total( &row_ptr->string );
		
		if( char_pos.x == -1 ) {
			char_pos.x = row_len;
		}
		for( ; char_pos.x>=0 && i>0; --char_pos.x,--i );
		
		if( i<=0 && char_pos.x >= 0 ) {
			break;
		}
	}
	DEBUG_MSG2( "after: %d,%d\n", char_pos.x, char_pos.y );
	if( i>0 ) {
		n -= i;
	}
	DEBUG_MSG2("start_pos: %d,%d, len: %d\n", char_pos.x, char_pos.y, n);
	/* 开始删除文字 */
	_TextLayer_Text_Delete( layer, char_pos, n );
	/* 删除完后，需要将光标向左移动一个位置 */
	TextLayer_Cursor_SetPos( layer, char_pos );
	return 0;
}


LCUI_Pos 
TextLayer_Char_GetPixelPos( LCUI_TextLayer *layer, LCUI_Pos char_pos )
/* 获取显示出来的文字相对于文本图层的坐标，单位为像素 */
{
	LCUI_Pos pixel_pos;
	Text_RowData *row_ptr;
	LCUI_CharData *char_ptr;
	int rows, cols, total;
	
	pixel_pos.x = pixel_pos.y = 0;
	char_pos = TextLayer_Cursor_GetPos( layer );
	total = Queue_Get_Total( &layer->rows_data );
	if( char_pos.y >= total ) {
		char_pos.y = total-1;
	}
	if( char_pos.y < 0 ) {
		char_pos.y = 0;
	}
	/* 累加pos.y行之前几行的高度 */
	for( pixel_pos.y=0,rows=0; rows<char_pos.y; ++rows ) {
		row_ptr = Queue_Get( &layer->rows_data, rows );
		if( !row_ptr ) {
			continue;
		}
		pixel_pos.y += row_ptr->max_size.h;
	}
	/* 获取当前行的指针 */
	row_ptr = Queue_Get( &layer->rows_data, rows );
	if( !row_ptr ) {
		pixel_pos.y += 2;
		return pixel_pos;
	}
	/* 获取当前行的文字数 */
	total = Queue_Get_Total( &row_ptr->string ); 
	if( char_pos.x > total ) {
		char_pos.x = total;
	}
	if( char_pos.x < 0 ) {
		char_pos.x = 0;
	}
	/* 累计宽度 */
	for( pixel_pos.x=0,cols=0; cols<char_pos.x; ++cols ) {
		char_ptr = Queue_Get( &row_ptr->string, cols );
		if( !char_ptr ) {
			continue;
		}
		/* 如果设定了屏蔽字符 */
		if( layer->password_char.char_code > 0 ) {
			pixel_pos.x += layer->password_char.bitmap->advance.x;
		} else {
			pixel_pos.x += char_ptr->bitmap->advance.x;
		}
	}
	/* 微调位置 */
	pixel_pos.y += 2;
	return pixel_pos;
}

LCUI_Pos
TextLayer_Cursor_GetPos( LCUI_TextLayer *layer )
/* 获取光标在文本框中的位置，也就是光标在哪一行的哪个字后面 */
{
	return layer->current_des_pos;
}

LCUI_Pos
TextLayer_Cursor_GetFixedPixelPos( LCUI_TextLayer *layer )
/* 获取文本图层的光标位置，单位为像素 */
{
	LCUI_Pos pos;
	pos = TextLayer_Cursor_GetPos( layer );
	pos = TextLayer_Char_GetPixelPos( layer, pos );
	return pos;
}

LCUI_Pos
TextLayer_Cursor_GetPixelPos( LCUI_TextLayer *layer )
/* 获取文本图层的光标相对于容器位置，单位为像素 */
{
	LCUI_Pos pos;
	pos = TextLayer_Cursor_GetFixedPixelPos( layer );
	/* 加上偏移坐标 */
	pos = Pos_Add( pos, layer->offset_pos );
	return pos;
}

int
TextLayer_Get_RowLen( LCUI_TextLayer *layer, int row )
/* 获取指定行显式文字数 */
{
	int total;
	Text_RowData *row_ptr;
	
	total = Queue_Get_Total( &layer->rows_data );
	if( row > total ) {
		row = total;
	}
	/* 获取当前行的指针 */
	row_ptr = Queue_Get( &layer->rows_data, row );
	if( !row_ptr ) {
		return 0;
	}
	return Queue_Get_Total( &row_ptr->string ); 
}

int 
TextLayer_CurRow_GetMaxHeight( LCUI_TextLayer *layer )
/* 获取当前行的最大高度 */
{
	Text_RowData *row_ptr;
	row_ptr = TextLayer_Get_Current_RowData( layer );
	if( !row_ptr ) {
		return layer->default_data.pixel_size + 2;
	}
	return row_ptr->max_size.h;
}

int 
TextLayer_Get_Rows( LCUI_TextLayer *layer )
/* 获取文本行数 */
{
	return Queue_Get_Total( &layer->rows_data );
}


int 
TextLayer_Get_Select_Text( LCUI_TextLayer *layer, char *out_text )
/* 获取文本图层内被选中的文本 */
{ 
	/* 如果选择了文本 */
	if( layer->have_select ) {
		/* 获取选中的文本内容 */
		//......
		return 0;
	}
	return -1;
}

int 
TextLayer_Copy_Select_Text( LCUI_TextLayer *layer )
/* 复制文本图层内被选中的文本 */
{
	/* 如果选择了文本 */
	if( layer->have_select ) {
		/* 将选中的文本内容添加至剪切版 */
		//......
		return 0;
	}
	return -1;
}

int 
TextLayer_Cut_Select_Text( LCUI_TextLayer *layer )
/* 剪切文本图层内被选中的文本 */
{
	/* 如果选择了文本 */
	if( layer->have_select ) {
		/* 将选中的文本内容添加至剪切版，并删除被选中的文本 */
		//......
		return 0;
	}
	return -1;
}

void 
TextLayer_Using_StyleTags( LCUI_TextLayer *layer, BOOL flag )
/* 指定文本图层是否处理样式标签 */
{
	layer->using_style_tags = flag;
}

void 
TextLayer_Multiline( LCUI_TextLayer *layer, BOOL flag )
/* 指定文本图层是否启用多行文本显示 */
{
	layer->enable_multiline = flag;
}
