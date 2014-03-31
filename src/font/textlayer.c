/* ***************************************************************************
 * textlayer.c -- text bitmap layer processing module.
 * 
 * Copyright (C) 2012-2014 by
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
 * 版权所有 (C) 2012-2014 归属于
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

static void TaskData_Init( TaskData *task )
{
	task->redraw_all = 0;
	task->update_bitmap = 0;
	task->update_typeset = 0;
	task->typeset_start_row = 0;
}

/** 添加 更新文本排版 的任务 */
static inline void TaskData_AddUpdateTypeset( TaskData *task, int start_row )
{
	if( start_row < task->typeset_start_row ) {
		task->typeset_start_row = start_row;
	}
	task->update_typeset = TRUE;
}

static void TextRow_Init( TextRowData* p_row )
{
	p_row->bottom_spacing = 1;
	p_row->top_spacing = 1;
	p_row->max_height = 0;
	p_row->max_width = 0;
	p_row->string = NULL;
	p_row->string_len = 0;
	p_row->string_max_len = 0;
	p_row->eol = EOL_NONE;
}

static void TextRow_Destroy( TextRowData *p_row )
{
	int i;
	for( i=0; i<p_row->string_max_len; ++i ) {
		if( p_row->string[i] ) {
			free( p_row->string[i] );
		}
	}
	p_row->string_max_len = 0;
	p_row->string_len = 0;
	free( p_row->string );
	p_row->string = NULL;
}

static inline TextRowData* 
TextRowList_GetRow( TextRowList *p_row_list, int n_row )
{
	if( n_row >= p_row_list->rows ) {
		return NULL;
	}
	return p_row_list->rowdata[n_row];
}

static inline TextRowData* 
TextRowList_AddNewRow( TextRowList *rowlist )
{
	TextRowData *row_ptr, **new_rowlist;
	
	++rowlist->rows;
	if( rowlist->max_rows <= rowlist->rows ) {
		new_rowlist = (TextRowData**)realloc( rowlist->rowdata, 
			sizeof(TextRowData*)*(rowlist->rows+1) );
		if( !new_rowlist ) {
			--rowlist->rows;
			return NULL;
		}
		rowlist->rowdata = new_rowlist;
		rowlist->max_rows = rowlist->rows;
	}
	
	row_ptr = (TextRowData*)malloc( sizeof(TextRowData) );
	if( !row_ptr ) {
		return NULL;
	}
	TextRow_Init( row_ptr );
	rowlist->rowdata[rowlist->rows-1] = row_ptr;
	return row_ptr;
}

static TextRowData* 
TextRowList_InsertNewRow( TextRowList *rowlist, int i_row )
{
	int i;
	TextRowData *row_ptr, **new_rowlist;
	
	if( i_row > rowlist->rows ) {
		return NULL;
	}
	++rowlist->rows;
	if( rowlist->max_rows <= rowlist->rows ) {
		new_rowlist = (TextRowData**)realloc( rowlist->rowdata, 
			sizeof(TextRowData*)*(rowlist->rows+1) );
		if( !new_rowlist ) {
			--rowlist->rows;
			return NULL;
		}
		rowlist->rowdata = new_rowlist;
		rowlist->max_rows = rowlist->rows;
	}

	row_ptr = (TextRowData*)malloc( sizeof(TextRowData) );
	if( !row_ptr ) {
		return NULL;
	}
	TextRow_Init( row_ptr );
	for( i=rowlist->rows-1; i>i_row; --i ) {
		rowlist->rowdata[i] = rowlist->rowdata[i-1];
	}
	rowlist->rowdata[i_row] = row_ptr;
	return row_ptr;
}

/** 从文本行列表中删除指定文本行 */
static int TextRowList_RemoveRow( TextRowList *p_rowlist, int row )
{
	if( row < 0 || row >= p_rowlist->rows ) {
		return -1;
	}
	for( ; row < p_rowlist->rows-1; ++row ) {
		p_rowlist->rowdata[row] = p_rowlist->rowdata[row+1];
	}
	TextRow_Destroy( p_rowlist->rowdata[row] );
	p_rowlist->rowdata[row] = NULL;
	--p_rowlist->rows;
	return 0;
}

/** 更新文本行的尺寸 */
static void TextRow_UpdateSize( TextRowData *p_row, int default_size )
{
	int i;
	TextCharData* p_char;

	p_row->max_width = 0;
	p_row->max_height = default_size;
	for( i=0; i<p_row->string_len; ++i ) {
		p_char = p_row->string[i];
		if( !p_char->bitmap ) {
			continue;
		}
		p_row->max_width += p_char->bitmap->advance.x;
		if( p_row->max_height < p_char->bitmap->advance.y ) {
			p_row->max_height = p_char->bitmap->advance.y;
		}
	}
	p_row->max_height = p_row->max_height*11/10;
}

/** 设置文本行的字符串长度 */
static int TextRow_SetLength( TextRowData *p_row, int new_len )
{
	TextCharData **p_new_str;

	if( new_len < 0 ) {
		new_len = 0;
	}
	p_row->string_len = new_len;
	if( new_len <= p_row->string_max_len ) {
		return 0;
	}

	p_new_str = (TextCharData**)
	realloc( p_row->string, sizeof(TextCharData*)*new_len );
	if( !p_new_str ) {
		--p_row->string_len;
		return -1;
	}
	/* 用0填充新增的空间 */
	memset( &p_new_str[new_len], 0, new_len-p_row->string_max_len );
	p_row->string = p_new_str;
	p_row->string_max_len = p_row->string_len;
	return 0;
}

/** 将字符数据直接插入至文本行 */
static int TextRow_Insert( TextRowData *p_row, int ins_pos, TextCharData *p_char )
{
	int i;
	TextCharData **p_new_str;
	
	if( ins_pos > p_row->string_len ) {
		ins_pos = p_row->string_len;
	}
	++p_row->string_len;
	if( p_row->string_max_len <= p_row->string_len ) {
		p_new_str = (TextCharData**)realloc( p_row->string, 
			sizeof(TextCharData*)*p_row->string_len );
		if( !p_new_str ) {
			--p_row->string_len;
			return -1;
		}
		p_row->string = p_new_str;
		p_row->string_max_len = p_row->string_len;
	}
	for( i=p_row->string_len-1; i>=ins_pos; --i ) {
		p_row->string[i] = p_row->string[i-1];
	}
	p_row->string[ins_pos] = p_char;
	return 0;
}

/** 向文本行插入一个字符数据副本 */
static int TextRow_InsertCopy( TextRowData *p_row, int ins_pos, TextCharData *p_char )
{
	TextCharData* p_copy_char;
	p_copy_char = (TextCharData*)malloc( sizeof(TextCharData) );
	memcpy( p_copy_char, p_char, sizeof(TextCharData) );
	return TextRow_Insert( p_row, ins_pos, p_copy_char );
}

/* 将文本行中的内容向左移动 */
static void TextRow_LeftMove( TextRowData *p_row, int n )
{
	int i, j, m;

	if( n <= 0 ) {
		return;
	}
	if( n > p_row->string_len ) {
		n = p_row->string_len;
	}
	m = p_row->string_len - n;
	for( i=0,j=n; i<m; ++i,++j ) {
		p_row->string[i] = p_row->string[j];
	}
	p_row->string_len = m;
}

/** 更新字体位图 */
static void TextChar_UpdateBitmap( TextCharData* p_data, 
				LCUI_TextStyle *default_style )
{
	int pixel_size, font_id;
	if( p_data->style ) {
		if( p_data->style->_family ) {
			font_id = p_data->style->font_id;
		} else {
			font_id = default_style->font_id;
		}
		if( p_data->style->_pixel_size ) {
			pixel_size = p_data->style->pixel_size;
		} else {
			pixel_size = default_style->pixel_size;
		}
	} else {
		font_id = default_style->font_id;
		pixel_size = default_style->pixel_size;

	}
	p_data->bitmap = FontLIB_GetExistFontBMP( font_id, p_data->char_code,
								pixel_size );
	//printf("char_code: %c, pixel_size: %d, font_id: %d, bitmap: %p\n", 
	//p_data->char_code, default_style->pixel_size, default_style->font_id, p_data->bitmap);
}

LCUI_API void TextLayer_Init( LCUI_TextLayer *layer )
{
	layer->max_width = 0;
	layer->max_height = 0;
	layer->offset_x = 0;
	layer->offset_y = 0;
	layer->new_offset_x = 0;
	layer->new_offset_y = 0;
	layer->insert_x = 0;
	layer->insert_y = 0;
	layer->is_mulitiline_mode = FALSE;
	layer->is_autowrap_mode = FALSE;
	layer->is_using_style_tags = FALSE;
	layer->is_using_buffer = TRUE;
	layer->text_align = TEXT_ALIGN_LEFT;

	layer->row_list.max_rows = 0;
	layer->row_list.rows = 0;
	layer->row_list.rowdata = NULL;
	
	TextStyle_Init( &layer->text_style );
	TaskData_Init( &layer->task );
	DirtyRectList_Init( &layer->dirty_rect );
	Graph_Init( &layer->graph );
	TextRowList_InsertNewRow( &layer->row_list, 0 );
	layer->graph.color_type = COLOR_TYPE_RGBA;
}

static void TextRowList_Destroy( TextRowList *list )
{
	int row;
	for( row=0; row<list->rows; ++row ) {
		TextRow_Destroy( list->rowdata[row] );
		list->rowdata[row] = NULL;
	}
	list->max_rows = 0;
	list->rows = 0;
	if( list->rowdata ) {
		free( list->rowdata );
	}
	list->rowdata = NULL;
}

/** 销毁TextLayer */
LCUI_API void TextLayer_Destroy( LCUI_TextLayer *layer )
{
	DirtyRectList_Destroy( &layer->dirty_rect );
	Graph_Free( &layer->graph );
	TextRowList_Destroy( &layer->row_list );
}

/** 获取指定文本行中的文本段的矩形区域 */
static int TextLayer_GetRowRectEx( LCUI_TextLayer *layer, int i_row,
				int start_col, int end_col, LCUI_Rect *rect )
{
	int i, x, y;
	TextRowData *p_row;

	if( i_row >= layer->row_list.rows ) {
		return -1;
	}
	/* 先计算在有效区域内的起始行的Y轴坐标 */
	y = layer->offset_y;
	for( i=0; i<i_row; ++i ) {
		p_row = layer->row_list.rowdata[i];
		y += p_row->top_spacing;
		y += p_row->max_height;
		y += p_row->bottom_spacing;
	}
	p_row = layer->row_list.rowdata[i_row];
	if( end_col < 0 || end_col >= p_row->string_len ) {
		end_col = p_row->string_len-1;
	}

	y += p_row->top_spacing;
	/* 根据对齐方式，计算该行的位置 */
	switch( layer->text_align ) {
	case TEXT_ALIGN_CENTER:
		x = (layer->max_width - p_row->max_width)/2;
		break;
	case TEXT_ALIGN_RIGHT:
		x = layer->max_width - p_row->max_width;
		break;
	case TEXT_ALIGN_LEFT:
	default:
		x = 0;
		break;
	}
	rect->x = layer->offset_x + x;

	y += p_row->top_spacing;
	rect->y = y;
	y += p_row->max_height;
	y += p_row->bottom_spacing;
	rect->height = p_row->max_height;

	for( i=0; i<start_col; ++i ) {
		if( !p_row->string[i]->bitmap ) {
			continue;
		}
		rect->x += p_row->string[i]->bitmap->advance.x;
	}

	rect->width = 0;
	for( i=start_col; i<=end_col && i<p_row->string_len; ++i ) {
		if( !p_row->string[i]->bitmap ) {
			continue;
		}
		rect->width += p_row->string[i]->bitmap->advance.x;
	}
	if( rect->x < 0 ) {
		rect->width += x;
		rect->x = 0;
	}
	else if( rect->x >= layer->max_width ) {
		return 1;
	}
	if( rect->x + rect->width >= layer->max_width ) {
		rect->width = layer->max_width - rect->x;
	}
	if( rect->y < 0 ) {
		rect->height += y;
		rect->y = 0;
	}
	else if( rect->y >= layer->max_height ) {
		return 1;
	}
	if( rect->y + rect->height >= layer->max_height ) {
		rect->height = layer->max_height - rect->y;
	}
	return 0;
}

/** 获取指定文本行的矩形区域 */
static int TextLayer_GetRowRect( LCUI_TextLayer *layer, int i_row,
				 LCUI_Rect *rect )
{
	int i, x, y;
	TextRowData *p_row;

	if( i_row >= layer->row_list.rows ) {
		return -1;
	}
	/* 先计算在有效区域内的起始行的Y轴坐标 */
	y = layer->offset_y;
	for( i=0; i<i_row; ++i ) {
		p_row = layer->row_list.rowdata[i];
		y += p_row->top_spacing;
		y += p_row->max_height;
		y += p_row->bottom_spacing;
	}
	p_row = layer->row_list.rowdata[i_row];
	y += p_row->top_spacing;
	/* 根据对齐方式，计算该行的位置 */
	switch( layer->text_align ) {
	case TEXT_ALIGN_CENTER:
		x = (layer->max_width - p_row->max_width)/2;
		break;
	case TEXT_ALIGN_RIGHT:
		x = layer->max_width - p_row->max_width;
		break;
	case TEXT_ALIGN_LEFT:
	default:
		x = 0;
		break;
	}
	y += p_row->top_spacing;
	rect->x = layer->offset_x + x;
	rect->y = y;
	y += p_row->max_height;
	y += p_row->bottom_spacing;
	rect->width = p_row->max_width;
	rect->height = p_row->max_height;
	if( rect->x < 0 ) {
		rect->width += x;
		rect->x = 0;
	}
	else if( rect->x >= layer->max_width ) {
		return 1;
	}
	if( rect->x + rect->width >= layer->max_width ) {
		rect->width = layer->max_width - rect->x;
	}
	if( rect->y < 0 ) {
		rect->height += y;
		rect->y = 0;
	}
	else if( rect->y >= layer->max_height ) {
		return 1;
	}
	if( rect->y + rect->height >= layer->max_height ) {
		rect->height = layer->max_height - rect->y;
	}
	return 0;
}

/** 标记指定文本行的矩形区域为无效 */
static void TextLayer_InvalidateRowRectEx( LCUI_TextLayer *layer, int i_row,
						int start, int end )
{
	int ret;
	LCUI_Rect rect;

	ret = TextLayer_GetRowRectEx( layer, i_row, start, end, &rect );
	if( ret == 0 ) {
		DirtyRectList_Add( &layer->dirty_rect, &rect );
	}
}

/** 标记指定文本行的矩形区域为无效 */
static void TextLayer_InvalidateRowRect( LCUI_TextLayer *layer, int i_row )
{
	LCUI_Rect rect;
	if( TextLayer_GetRowRect( layer, i_row, &rect ) == 0 ) {
		DirtyRectList_Add( &layer->dirty_rect, &rect );
	}
}

/** 标记当前显示区域内的所有文本行的矩形为无效 */
LCUI_API void TextLayer_InvalidateAllRowRect( LCUI_TextLayer* layer )
{
	int i, x, y;
	LCUI_Rect rect;
	TextRowData *p_row;

	/* 先计算在有效区域内的起始行的Y轴坐标 */
	y = layer->offset_y;
	for( i=0; i<layer->row_list.rows; ++i ) {
		p_row = layer->row_list.rowdata[i];
		y += p_row->top_spacing;
		y += p_row->max_height;
		if( y > 0 ) {
			y -= p_row->max_height;
			break;
		}
		y += p_row->bottom_spacing;
	}
	for( ; i<layer->row_list.rows; ++i ) {
		p_row = layer->row_list.rowdata[i];
		/* 根据对齐方式，计算该行的位置 */
		switch( layer->text_align ) {
		case TEXT_ALIGN_CENTER:
			x = (layer->max_width - p_row->max_width)/2;
			break;
		case TEXT_ALIGN_RIGHT:
			x = layer->max_width - p_row->max_width;
			break;
		case TEXT_ALIGN_LEFT:
		default:
			x = 0;
			break;
		}
		y += p_row->top_spacing;
		rect.x = layer->offset_x + x;
		rect.y = y;
		y += p_row->max_height;
		y += p_row->bottom_spacing;
		rect.width = p_row->max_width;
		rect.height = p_row->max_height;
		if( rect.x < 0 ) {
			rect.width += x;
			rect.x = 0;
		}
		else if( rect.x >= layer->max_width ) {
			continue;
		}
		if( rect.x + rect.width >= layer->max_width ) {
			rect.width = layer->max_width - rect.x;
		}
		if( rect.y < 0 ) {
			rect.height += y;
			rect.y = 0;
		}
		else if( rect.y >= layer->max_height ) {
			continue;
		}
		if( rect.y + rect.height >= layer->max_height ) {
			rect.height = layer->max_height - rect.y;
		}

		DirtyRectList_Add( &layer->dirty_rect, &rect );
	}
}


/** 标记指定范围内容的文本行的矩形为无效 */
LCUI_API void TextLayer_InvalidateRowsRect( LCUI_TextLayer *layer,
					   int start_row, int end_row )
{
	int i, x, y;
	LCUI_Rect rect;
	TextRowData *p_row;

	/* 先计算在有效区域内的起始行的Y轴坐标 */
	y = layer->offset_y;
	for( i=0; i<start_row; ++i ) {
		p_row = layer->row_list.rowdata[i];
		y += p_row->top_spacing;
		y += p_row->max_height;
		y += p_row->bottom_spacing;
	}
	if( end_row < 0 ) {
		end_row = layer->row_list.rows-1;
	}
	for( i=start_row; i<=end_row; ++i ) {
		p_row = layer->row_list.rowdata[i];
		/* 根据对齐方式，计算该行的位置 */
		switch( layer->text_align ) {
		case TEXT_ALIGN_CENTER:
			x = (layer->max_width - p_row->max_width)/2;
			break;
		case TEXT_ALIGN_RIGHT:
			x = layer->max_width - p_row->max_width;
			break;
		case TEXT_ALIGN_LEFT:
		default:
			x = 0;
			break;
		}
		y += p_row->top_spacing;
		rect.x = layer->offset_x + x;
		rect.y = y;
		y += p_row->max_height;
		y += p_row->bottom_spacing;
		rect.width = p_row->max_width;
		rect.height = p_row->max_height;
		if( rect.x < 0 ) {
			rect.width += x;
			rect.x = 0;
		}
		else if( rect.x >= layer->max_width ) {
			continue;
		}
		if( rect.x + rect.width >= layer->max_width ) {
			rect.width = layer->max_width - rect.x;
		}
		if( rect.y < 0 ) {
			rect.height += y;
			rect.y = 0;
		}
		else if( rect.y >= layer->max_height ) {
			continue;
		}
		if( rect.y + rect.height >= layer->max_height ) {
			rect.height = layer->max_height - rect.y;
		}

		DirtyRectList_Add( &layer->dirty_rect, &rect );
	}
}

/** 设置插入点的行列坐标 */
LCUI_API void TextLayer_SetCaretPos( LCUI_TextLayer* layer, int row, int col )
{
	if( row < 0 ) {
		row = 0;
	}
	else if( row >= layer->row_list.rows ) {
		if( layer->row_list.rows < 0 ) {
			row = 0;
		} else	{
			row = layer->row_list.rows-1;
		}
	}

	if( col < 0 ) {
		col = 0;
	}
	else if( layer->row_list.rows > 0 ) {
		if( col >= layer->row_list.rowdata[row]->string_len ) {
			col = layer->row_list.rowdata[row]->string_len;
		}
	} else {
		col = 0;
	}
	layer->insert_x = col;
	layer->insert_y = row;
}

/** 根据像素坐标设置文本光标的行列坐标 */
LCUI_API int TextLayer_SetCaretPosByPixelPos( LCUI_TextLayer* layer, int x, int y )
{
	TextRowData* p_row;
	int i, pixel_pos, ins_x, ins_y;

	for( pixel_pos=0, i=0; i<layer->row_list.rows; ++i ) {
		p_row = layer->row_list.rowdata[i];
		pixel_pos += p_row->top_spacing;
		pixel_pos += p_row->max_height;
		pixel_pos += p_row->bottom_spacing;
		if( pixel_pos >= y ) {
			ins_y = i;
			break;
		}
	}
	if( i >= layer->row_list.rows ) {
		if( layer->row_list.rows > 0 ) {
			ins_y = layer->row_list.rows-1;
		} else {
			layer->insert_x = 0;
			layer->insert_y = 0;
			return -1;
		}
	}
	p_row = layer->row_list.rowdata[ins_y];
	ins_x = p_row->string_len;
	/* 根据文本对齐方式，确定当前行文本的起始坐标 */
	switch( layer->text_align ) {
	case TEXT_ALIGN_CENTER:
		pixel_pos = (layer->max_width - p_row->max_width)/2;
		break;
	case TEXT_ALIGN_RIGHT:
		pixel_pos = layer->max_width - p_row->max_width;
		break;
	case TEXT_ALIGN_LEFT:
	default:
		pixel_pos = 0;
		break;
	}
	for( i=0; i<p_row->string_len; ++i ) {
		if( !p_row->string[i]->bitmap ) {
			continue;
		}
		pixel_pos += p_row->string[i]->bitmap->advance.x;
		/* 如果在当前字中心点的前面 */
		if( x <= pixel_pos - p_row->string[i]->bitmap->advance.x/2 ) {
			ins_x = i;
			break;
		}
	}
	TextLayer_SetCaretPos( layer, ins_y, ins_x );
	return 0;
}

/** 获取指定行列的文字的像素坐标 */
LCUI_API int TextLayer_GetCharPixelPos( LCUI_TextLayer* layer, int row,
						int col, LCUI_Pos *pixel_pos )
{
	TextRowData* p_row;
	int i, pixel_x = 0, pixel_y = 0;

	if( row < 0 || row >= layer->row_list.rows ) {
		return -1;
	}
	if( col < 0 ) {
		return -2;
	}
	else if( col > layer->row_list.rowdata[row]->string_len ) {
		return -3;
	}
	/* 累加前几行的高度 */
	for( i=0; i<row; ++i ) {
		p_row = layer->row_list.rowdata[i];
		pixel_y += p_row->top_spacing;
		pixel_y += p_row->max_height;
		pixel_y += p_row->bottom_spacing;
	}
	p_row = layer->row_list.rowdata[i];
	pixel_y += p_row->top_spacing;
	p_row = layer->row_list.rowdata[row];
	switch( layer->text_align ) {
	case TEXT_ALIGN_CENTER:
		pixel_x = (layer->max_width - p_row->max_width)/2;
		break;
	case TEXT_ALIGN_RIGHT:
		pixel_x = layer->max_width - p_row->max_width;
		break;
	case TEXT_ALIGN_LEFT:
	default:
		pixel_x = 0;
		break;
	}
	for( i=0; i<col; ++i ) {
		if( !p_row->string[i] ) {
			break;
		}
		if( !p_row->string[i]->bitmap ) {
			continue;
		}
		pixel_x += p_row->string[i]->bitmap->advance.x;
	}
	pixel_pos->x = pixel_x;
	pixel_pos->y = pixel_y;
	return 0;
}

/** 获取文本光标的像素坐标 */
LCUI_API int TextLayer_GetCaretPixelPos( LCUI_TextLayer *layer, LCUI_Pos *pixel_pos )
{
	return TextLayer_GetCharPixelPos( layer, layer->insert_y, 
						layer->insert_x, pixel_pos );
}

/** 清空文本 */
LCUI_API void TextLayer_ClearText( LCUI_TextLayer* layer )
{
	layer->insert_x = 0;
	layer->insert_y = 0;
	TextLayer_InvalidateAllRowRect( layer );
	TextRowList_Destroy( &layer->row_list );
	layer->task.redraw_all = TRUE;
}

/** 对文本行进行断行 */
static void TextLayer_BreakTextRow( LCUI_TextLayer *layer, int i_row, 
						int col, EOLChar eol )
{
	int n;
	TextRowData *p_row, *p_next_row;

	p_row = layer->row_list.rowdata[i_row];
	p_next_row = TextRowList_InsertNewRow( &layer->row_list, i_row+1 );
	/* 将本行原有的行尾符转移至下一行 */
	p_next_row->eol = p_row->eol;
	p_row->eol = eol;
	for( n=p_row->string_len-1; n>=col; --n ) {
		TextRow_Insert( p_next_row, 0, p_row->string[n] );
		p_row->string[n] = NULL;
	}
	p_row->string_len = col;
	TextRow_UpdateSize( p_row, layer->text_style.pixel_size );
	TextRow_UpdateSize( p_next_row, layer->text_style.pixel_size );
}

/** 对指定行的文本进行排版 */
static void TextLayer_TextRowTypeset( LCUI_TextLayer* layer, int row )
{
	int col, row_width;
	TextRowData *p_row, *p_next_row;
	TextCharData *p_char;

	p_row = layer->row_list.rowdata[row];
	row_width = 0;
	for( col=0; col<p_row->string_len; ++col ) {
		p_char = p_row->string[col];
		if( !p_char->bitmap ) {
			continue;
		}
		/* 累加行宽度 */
		row_width += p_char->bitmap->advance.x;
		/* 如果是当前行的第一个字符，或者行宽度没有超过宽度限制 */
		if( layer->max_width <= 0 || !layer->is_autowrap_mode 
		 || (layer->is_autowrap_mode && !layer->is_mulitiline_mode)
		 || col < 1 || p_row->max_width <= layer->max_width ) {
			continue;
		}
		TextLayer_BreakTextRow( layer, row, col, EOL_NONE );
		return;
	}
	TextRow_UpdateSize( p_row, layer->text_style.pixel_size );
	/* 如果本行有换行符，或者是最后一行 */
	if( p_row->eol != EOL_NONE || row == layer->row_list.rows-1 ) {
		return;
	}
	
	row_width = p_row->max_width;
	/* 本行的文本宽度未达到限制宽度，需要将下行的文本转移至本行 */
	while( p_row->eol != EOL_NONE ) {
		/* 获取下一行的指针 */
		p_next_row = TextRowList_GetRow( &layer->row_list, row+1 );
		if( !p_next_row ) {
			return;
		}
		for( col=0; col<p_next_row->string_len; ++col ) {
			p_char = p_next_row->string[col];
			/* 忽略无字体位图的文字 */
			if( !p_char->bitmap ) {
				TextRow_Insert( p_row, p_row->string_len, p_char );
				p_next_row->string[col] = NULL;
				continue;
			}
			row_width += p_char->bitmap->advance.x;
			/* 如果没有超过宽度限制 */
			if( !layer->is_autowrap_mode || layer->max_width <= 0
			 || (layer->is_autowrap_mode && !layer->is_mulitiline_mode)
			 || p_row->max_width <= layer->max_width ) {
				TextRow_Insert( p_row, p_row->string_len, p_char );
				p_next_row->string[col] = NULL;
				continue;
			}
			/* 如果插入点在下一行 */
			if( layer->insert_y == row+1 ) {
				/* 如果插入点处于被转移的几个文字中 */
				if( layer->insert_x < col ) {
					layer->insert_y = row;
					layer->insert_x += p_row->string_len;
				} else {
					/* 否则，减去被转移的文字数 */
					layer->insert_x -= col;
				}
			}
			/* 将这一行剩余的文字向前移 */
			TextRow_LeftMove( p_next_row, col );
			TextRow_UpdateSize( p_row, layer->text_style.pixel_size );
			return;
		}
		TextRow_UpdateSize( p_row, layer->text_style.pixel_size );
		TextLayer_InvalidateRowRect( layer, row );
		TextLayer_InvalidateRowRect( layer, row+1 );
		_DEBUG_MSG("remove row %d\n", row+1);
		/* 删除这一行，因为这一行的内容已经转移至当前行 */
		TextRowList_RemoveRow( &layer->row_list, row+1 );
		/* 如果插入点当前行在后面 */
		if( layer->insert_y > row ) {
			--layer->insert_y;
		}
	}
}

/** 从指定行开始，对文本进行排版 */
static void TextLayer_TextTypeset( LCUI_TextLayer *layer, int start_row )
{
	int row;
	/* 记录排版前各个文本行的矩形区域 */
	TextLayer_InvalidateRowsRect( layer, start_row, -1 );
	for( row=start_row; row<layer->row_list.rows; ++row ) {
		TextLayer_TextRowTypeset( layer, row );
	}
	/* 记录排版后各个文本行的矩形区域 */
	TextLayer_InvalidateRowsRect( layer, start_row, -1 );
}

/** 对文本进行预处理 */ 
static int TextLayer_ProcessText( LCUI_TextLayer *layer, const wchar_t *wstr,
			TextAddType add_type, LCUI_StyleTagStack *tag_stack )
{
	EOLChar eol;
	int cur_col, cur_row, start_row, ins_x, ins_y;
	const wchar_t *p_end, *p, *pp;
	TextRowData *p_row;
	TextCharData char_data;
	LCUI_Queue tmp_tag_stack;
	LCUI_BOOL is_tmp_tag_stack, need_typeset, rect_has_added;

	if( !wstr ) {
		return -1;
	}
	is_tmp_tag_stack = FALSE;
	need_typeset = FALSE;
	rect_has_added = FALSE;
	/* 如果是将文本追加至文本末尾 */
	if( add_type == TEXT_ADD_TYPE_APPEND ) {
		if( layer->row_list.rows > 0 ) {
			cur_row = layer->row_list.rows - 1;
		} else {
			cur_row = 0;
		}
		p_row = TextRowList_GetRow( &layer->row_list, cur_row );
		if( !p_row ) {
			p_row = TextRowList_AddNewRow( &layer->row_list );
		}
		cur_col = p_row->string_len;
	} else { /* 否则，是将文本插入至当前插入点 */
		cur_row = layer->insert_y;
		cur_col = layer->insert_x;
		p_row = TextRowList_GetRow( &layer->row_list, cur_row );
		if( !p_row ) {
			p_row = TextRowList_AddNewRow( &layer->row_list );
		}
	}
	start_row = cur_row;
	ins_x = cur_col;
	ins_y = cur_row;
	/* 如果没有可用的标签栈，则使用临时的标签栈 */
	if( !tag_stack ) {
		is_tmp_tag_stack = TRUE;
		StyleTagStack_Init( &tmp_tag_stack );
		tag_stack = &tmp_tag_stack;
	}
	p_end = wstr + wcslen(wstr);
	for( p=wstr; p<p_end; ++p ) {
		if( layer->is_using_style_tags ) {
			/* 处理样式的结束标签 */ 
			pp = StyleTagStack_ScanEndingTag( tag_stack, p );
			if( pp ) {
				p = pp;
				/* 抵消本次循环后的++p，以在下次循环时还能够在当前位置 */
				--p; 
				continue;
			}
			/* 处理样式标签 */
			pp = StyleTagStack_ScanBeginTag( tag_stack, p );
			if( pp ) {
				p = pp - 1;
				continue;
			}
		}
		
		if( *p == '\r' || *p == '\n' ) {
			/* 如果后面还有 \n，则说明是CR/LF换行模式 */
			if( *p == 'r' ) {
				if( p+1 < p_end && *(p+1) == '\n' ) {
					eol = EOL_CR_LF;
				} else {
					/* 否则是CR换行模式 */
					eol = EOL_CR;
				}
			} else {
				eol = EOL_LF;
			}
			/* 如果没有记录过文本行的矩形区域 */
			if( !rect_has_added ) {
				TextLayer_InvalidateRowsRect( layer, ins_y, -1 );
				rect_has_added = TRUE;
				start_row = ins_y;
			}
			/* 将当前行中的插入点为截点，进行断行 */
			TextLayer_BreakTextRow( layer, ins_y, ins_x, eol );
			need_typeset = TRUE;
			ins_x = 0;
			++ins_y;
			p_row = TextRowList_GetRow( &layer->row_list, ins_y );
			continue;
		}

		char_data.char_code = *p;
		/* 获取当前文本样式数据 */
		char_data.style = StyleTagStack_GetTextStyle( tag_stack );
		/* 更新字体位图 */
		TextChar_UpdateBitmap( &char_data, &layer->text_style );
		TextRow_InsertCopy( p_row, ins_x, &char_data );
		++ins_x;
	}
	/* 更新当前行的尺寸 */
	TextRow_UpdateSize( p_row, layer->text_style.pixel_size );
	if( add_type == TEXT_ADD_TYPE_INSERT ) {
		layer->insert_x = ins_x;
		layer->insert_y = ins_y;
	}
	/* 若启用了自动换行模式，则标记需要重新对文本进行排版 */
	if( layer->is_autowrap_mode || need_typeset ) {
		TaskData_AddUpdateTypeset( &layer->task, cur_row );
	} else {
		TextLayer_InvalidateRowRect( layer, cur_row );
	}
	/* 如果已经记录过文本行矩形区域 */
	if( rect_has_added ) {
		TextLayer_InvalidateRowsRect( layer, start_row, -1 );
		rect_has_added = TRUE;
	}
	/* 如果使用的是临时标签栈，则销毁它 */
	if( is_tmp_tag_stack ) {
		StyleTagStack_Destroy( tag_stack );
	}
	return 0;
}

/** 插入文本内容（宽字符版） */
LCUI_API int TextLayer_InsertTextW( LCUI_TextLayer *layer, const wchar_t *wstr,
						LCUI_StyleTagStack *tag_stack )
{
	return TextLayer_ProcessText( layer, wstr, TEXT_ADD_TYPE_INSERT, tag_stack );
}

/** 插入文本内容 */
LCUI_API int TextLayer_InsertTextA( LCUI_TextLayer *layer, const char *str )
{
	return 0;
}

/** 插入文本内容（UTF-8版） */
LCUI_API int TextLayer_InsertText( LCUI_TextLayer *layer, const char *utf8_str )
{
	return 0;
}

/** 追加文本内容（宽字符版） */
LCUI_API int TextLayer_AppendTextW( LCUI_TextLayer *layer, const wchar_t *wstr, 
						LCUI_StyleTagStack *tag_stack )
{
	return TextLayer_ProcessText( layer, wstr, TEXT_ADD_TYPE_APPEND, tag_stack );
}

/** 追加文本内容 */
LCUI_API int TextLayer_AppendTextA( LCUI_TextLayer *layer, const char *ascii_text )
{
	return 0;
}

/** 追加文本内容（UTF-8版） */
LCUI_API int TextLayer_AppendText( LCUI_TextLayer *layer, const char *utf8_text )
{
	return 0;
}

/** 设置文本内容（宽字符版） */
LCUI_API int TextLayer_SetTextW( LCUI_TextLayer *layer, const wchar_t *wstr,
						LCUI_StyleTagStack *tag_stack )
{
	TextLayer_ClearText( layer );
	return TextLayer_AppendTextW( layer, wstr, tag_stack );
}

/** 设置文本内容 */
LCUI_API int TextLayer_SetTextA( LCUI_TextLayer* layer, const char *ascii_text )
{
	return 0;
}

/** 设置文本内容（UTF-8版） */
LCUI_API int TextLayer_SetText( LCUI_TextLayer* layer, const char *utf8_text )
{
	return 0;
}

/** 获取文本图层中的文本（宽字符版） */
LCUI_API int TextLayer_GetTextW( LCUI_TextLayer *layer, int start_pos,
					int max_len, wchar_t *wstr_buff )
{
	int row, col, i;
	TextRowData *row_ptr;

	if( start_pos<0 ) {
		return -1;
	}
	if( max_len <= 0 ) {
		return 0;
	}
	/* 先根据一维坐标计算行列坐标 */
	for( i=0,row=0; row<layer->row_list.max_rows; ++row ) {
		if( i >= start_pos ) {
			col = start_pos - i;
			break;
		}
		i += layer->row_list.rowdata[row]->string_len;
	}
	for( i=0; row < layer->row_list.max_rows, i < max_len; ++row ) {
		row_ptr = layer->row_list.rowdata[row];
		for( ; col < row_ptr->string_len, i < max_len; ++col,++i ) {
			wstr_buff[i] = row_ptr->string[col]->char_code;
		}
	}
	wstr_buff[i] = L'\0';
	return i;
}

/** 获取文本位图缓存 */
LCUI_API LCUI_Graph* TextLayer_GetGraphBuffer( LCUI_TextLayer *layer )
{
	if( layer->is_using_buffer ) {
		return &layer->graph;
	}
	return NULL;
}

/** 设置最大尺寸 */
LCUI_API int TextLayer_SetMaxSize( LCUI_TextLayer *layer, LCUI_Size new_size )
{
	if( new_size.w <= 0 || new_size.h <= 0 ) {
		return -1;
	}
	layer->max_width = new_size.w;
	layer->max_height = new_size.h;
	if( layer->is_using_buffer ) {
		Graph_Create( &layer->graph, new_size.w, new_size.h );
	}
	layer->task.redraw_all = TRUE;
	if( layer->is_autowrap_mode ) {
		layer->task.typeset_start_row = 0;
		layer->task.update_typeset = TRUE;
	}
	return 0;
}

/** 设置是否启用多行文本模式 */
LCUI_API void TextLayer_SetMultiline( LCUI_TextLayer* layer, int is_true )
{
	if( layer->is_mulitiline_mode && !is_true
	 || !layer->is_mulitiline_mode && is_true ) {
		layer->is_mulitiline_mode = is_true;
		TaskData_AddUpdateTypeset( &layer->task, 0 );;
	}
}

/** 删除指定行列的文字及其右边的文本 */
static int TextLayer_DeleteText( LCUI_TextLayer* layer, int char_y, 
						int char_x, int n_char )
{
	int end_x, end_y, i, j, len;
	TextRowData *p_row, *p_end_row, *p_prev_row;

	if( char_x < 0 ) {
		char_x = 0;
	}
	if( char_y < 0 ) {
		char_y = 0;
	}
	if( n_char <= 0 ) {
		return -1;
	}
	if( char_y >= layer->row_list.rows ) {
		return -2;
	}
	p_row = layer->row_list.rowdata[char_y];
	if( char_x > p_row->string_len ) {
		char_x = p_row->string_len;
	}
	end_x = char_x;
	end_y = char_y;
	--n_char;
	/* 计算结束点的位置 */
	for( end_y=char_y; end_y<layer->row_list.rows && n_char>0; ++end_y ) {
		p_row = layer->row_list.rowdata[end_y];
		if( p_row->eol == EOL_NONE ) {
			if( end_x + n_char < p_row->string_len ) {
				end_x += n_char;
				n_char = 0;
				break;
			}
		} else {
			if( end_x + n_char <= p_row->string_len ) {
				end_x += n_char;
				n_char = 0;
				break;
			}
		}
		n_char -= (p_row->string_len - end_x);
		end_x = 0;
	}
	if( end_y >= layer->row_list.rows ) {
		end_y = layer->row_list.rows-1;
	}
	p_end_row = layer->row_list.rowdata[end_y];
	if( end_x > p_end_row->string_len ) {
		end_x = p_end_row->string_len;
	}
	/* 获取上一行文本 */
	p_prev_row = layer->row_list.rowdata[char_y-1];
	// 计算起始行与结束行拼接后的长度
	// 起始行：0 1 2 3 4 5，起点位置：2
	// 结束行：0 1 2 3 4 5，终点位置：4
	// 拼接后的长度：2 + 6 - 4 - 1 = 3
	len = char_x + p_end_row->string_len - end_x - 1;
	if( len < 0 ) {
		return -3;
	}
	/* 如果是同一行 */
	if( p_row == p_end_row ) {
		if( end_x >= p_end_row->string_len ) {
			return -4;
		}
		TextLayer_InvalidateRowRectEx( layer, char_y, char_x, -1 );
		TaskData_AddUpdateTypeset( &layer->task, char_y );
		for( i=char_x, j=end_x+1; j<p_row->string_len; ++i,++j ) {
			p_row->string[i] = p_row->string[j];
		}
		/* 如果当前行为空，也不是第一行，并且上一行没有结束符 */
		if( len <= 0 && end_y > 0 && p_prev_row->eol != EOL_NONE ) {
			TextRowList_RemoveRow( &layer->row_list, end_y );
		} 
		/* 调整起始行的容量 */
		TextRow_SetLength( p_row, len );
		/* 更新文本行的尺寸 */
		TextRow_UpdateSize( p_row, layer->text_style.pixel_size );
		return 0;
	}
	/* 如果结束点在行尾，并且该行不是最后一行 */
	if( end_x == p_end_row->string_len && end_y < layer->row_list.rows-1 ) {
		++end_y;
		p_end_row = TextRowList_GetRow( &layer->row_list, end_y );
		end_x = -1;
		len = char_x + p_end_row->string_len;
	}
	TextRow_SetLength( p_row, len );
	/* 标记当前行后面的所有行的矩形需区域需要刷新 */
	TextLayer_InvalidateRowsRect( layer, char_y+1, -1 );
	/* 移除起始行与结束行之间的文本行 */
	for( i=char_y+1, j=i; j<end_y; ++j ) {
		TextLayer_InvalidateRowRect( layer, i );
		TextRowList_RemoveRow( &layer->row_list, i );
	}
	end_y = char_y + 1;
	/* 将结束行的内容拼接至起始行 */
	for( i=char_x, j=end_x+1; i<len && j<p_end_row->string_len; ++i,++j ) {
		p_row->string[i] = p_end_row->string[j];
	}
	TextRow_UpdateSize( p_row, layer->text_style.pixel_size );
	TextLayer_InvalidateRowRect( layer, end_y );
	/* 移除结束行 */
	TextRowList_RemoveRow( &layer->row_list, end_y );
	/* 如果起始行无内容，并且上一行没有结束符（换行符），则
	 * 说明需要删除起始行 */
	if( len <= 0 && char_y > 0 && p_prev_row->eol != EOL_NONE ) {
		TextLayer_InvalidateRowRect( layer, char_y );
		TextRowList_RemoveRow( &layer->row_list, char_y );
	}
	TaskData_AddUpdateTypeset( &layer->task, char_y );;
	return 0;
}

/** 删除文本光标的当前坐标右边的文本 */
LCUI_API int TextLayer_Delete( LCUI_TextLayer *layer, int n_char )
{
	return TextLayer_DeleteText(	layer, layer->insert_y, 
					layer->insert_x, n_char );
}

/** 退格删除文本，即删除文本光标的当前坐标左边的文本 */
LCUI_API int TextLayer_Backspace( LCUI_TextLayer* layer, int n_char )
{
	int n_del;
	int char_x, char_y;
	TextRowData* p_row;

	/* 先获取当前字的位置 */
	char_x = layer->insert_x;
	char_y = layer->insert_y;
	/* 再计算删除 n_char 个字后的位置 */
	for( n_del=n_char; char_y>=0; --char_y ) {
		p_row = layer->row_list.rowdata[char_y];
		/* 如果不是当前行，则重定位至行尾 */
		if( char_y < layer->insert_y ) {
			char_x = p_row->string_len;
			if( p_row->eol == EOL_NONE ) {
				--char_x;
			}
		}
		if( char_x >= n_del ) {
			char_x = char_x - n_del;
			n_del = 0;
			break;
		}
		n_del = n_del - char_x - 1;
	}
	if( char_y < 0 && n_del == n_char ) {
		return -1;
	}
	/* 若能够被删除的字不够 n_char 个，则调整需删除的字数 */
	if( n_del > 0 ) {
		n_char -= n_del;
	}
	/* 开始删除文本 */
	TextLayer_DeleteText( layer, char_y, char_x, n_char );
	/* 若最后一行被完全移除，则移动输入点至上一行的行尾处 */
	if( char_x == 0 && layer->row_list.rows > 0 
	 && char_y >= layer->row_list.rows ) {
		char_y = layer->row_list.rows-1;
		char_x = layer->row_list.rowdata[char_y]->string_len;
	}
	/* 更新文本光标的位置 */
	TextLayer_SetCaretPos( layer, char_y, char_x );
	return 0;
}

/** 设置是否启用自动换行模式 */
LCUI_API void TextLayer_SetAutoWrap( LCUI_TextLayer* layer, int is_true )
{
	if( !layer->is_autowrap_mode && is_true
	 || layer->is_autowrap_mode && !is_true ) {
		layer->is_autowrap_mode = is_true;
		TaskData_AddUpdateTypeset( &layer->task, 0 );
	}
}

/** 设置是否使用样式标签 */
LCUI_API void TextLayer_SetUsingStyleTags( LCUI_TextLayer *layer, LCUI_BOOL is_true )
{
	layer->is_using_style_tags = is_true;
}

/** 计算并获取文本的宽度 */
LCUI_API int TextLayer_GetWidth( LCUI_TextLayer* layer )
{
	int i, row, w, max_w;
	TextRowData* p_row;

	for( row=0,max_w=0; row<layer->row_list.rows; ++row ) {
		p_row = layer->row_list.rowdata[row];
		for( i=0, w=0; i<p_row->string_len; ++i ) {
			if( !p_row->string[i]->bitmap ) {
				continue;
			}
			w += p_row->string[i]->bitmap->advance.x;
		}
		if( w > max_w ) {
			max_w = w;
		}
	}
	return max_w;
}

/** 计算并获取文本的高度 */
LCUI_API int TextLayer_GetHeight( LCUI_TextLayer* layer )
{
	int i, h;
	TextRowData* p_row;

	for( i=0,h=0; i<layer->row_list.rows; ++i ) {
		p_row = layer->row_list.rowdata[i];
		h += p_row->top_spacing;
		h += p_row->max_height;
		h += p_row->bottom_spacing;
	}
	return h;
}

/** 重新载入各个文字的字体位图 */
LCUI_API void TextLayer_ReloadCharBitmap( LCUI_TextLayer* layer )
{
	int row, col;
	TextCharData* p_char;
	TextRowData* p_row;

	for( row=0; row<layer->row_list.rows; ++row ) {
		p_row = layer->row_list.rowdata[row];
		for( col=0; col<p_row->string_len; ++col ) {
			p_char = p_row->string[col];
			TextChar_UpdateBitmap( p_char, &layer->text_style );
			if( !p_char->bitmap ) {
				continue;
			}
		}
		TextRow_UpdateSize( p_row, layer->text_style.pixel_size );
	}
}

/** 更新数据 */
LCUI_API void TextLayer_Update( LCUI_TextLayer* layer, LinkedList *rect_list )
{
	if( layer->task.update_bitmap ) {
		TextLayer_InvalidateAllRowRect( layer );
		TextLayer_ReloadCharBitmap( layer );
		TextLayer_InvalidateAllRowRect( layer );
		layer->task.update_bitmap = FALSE;
		layer->task.redraw_all = TRUE;
	}

	if( layer->task.update_typeset ) {
		TextLayer_TextTypeset( layer, layer->task.typeset_start_row );
		layer->task.update_typeset = FALSE;
		layer->task.typeset_start_row = 0;
	}

	/* 如果坐标偏移量有变化，记录各个文本行区域 */
	if( layer->new_offset_x != layer->offset_x
	 || layer->new_offset_y != layer->offset_y ) {
		TextLayer_InvalidateAllRowRect( layer );
		layer->offset_x = layer->new_offset_x;
		layer->offset_y = layer->new_offset_y;
		TextLayer_InvalidateAllRowRect( layer );
		layer->task.redraw_all = TRUE;
	}
	
	if( rect_list ) {
		void *data_ptr;
		LinkedList_Goto( &layer->dirty_rect, 0 );
		/* 转移脏矩形记录，供利用 */
		while( data_ptr = LinkedList_Get( &layer->dirty_rect ) ) {
			LinkedList_AddDataCopy( rect_list, data_ptr );
			LinkedList_ToNext( &layer->dirty_rect );
		}
	 } 
}

/** 
 * 将文本图层中的指定区域的内容绘制至目标图像中
 * @param layer 要使用的文本图层
 * @param area 文本图层中需要绘制的区域
 * @param layer_pos 文本图层在目标图像中的位置
 * @param need_replace 绘制时是否需要覆盖像素
 * @param graph 目标图像
 */
LCUI_API int TextLayer_DrawToGraph( LCUI_TextLayer *layer, LCUI_Rect area,
		LCUI_Pos layer_pos, LCUI_BOOL need_replace, LCUI_Graph *graph )
{
	int x, y, row, col;
	LCUI_Pos char_pos;
	TextRowData *p_row;
	TextCharData *p_char;
	LCUI_Size box_size;

	box_size.w = layer->max_width;
	box_size.h = layer->max_height;
	/* 调整区域范围，使之有效 */
	area = LCUIRect_ValidateArea( box_size, area );
	/* 加上Y轴坐标偏移量 */
	y = layer->offset_y;
	/* 先确定从哪一行开始绘制 */
	for( row=0; row<layer->row_list.rows; ++row ) {
		p_row = TextRowList_GetRow( &layer->row_list, row );
		y += p_row->top_spacing;
		y += p_row->max_height;
		if( y > area.y ) {
			y -= p_row->top_spacing;
			y -= p_row->max_height;
			break;
		}
		y += p_row->bottom_spacing;
	}
	/* 如果没有可绘制的文本行 */
	if( row >= layer->row_list.rows ) {
		return -1;
	}
	
	for( ; row < layer->row_list.rows; ++row ) {
		p_row = TextRowList_GetRow( &layer->row_list, row );
		/* 根据对齐方式，计算该行的位置 */
		switch( layer->text_align ) {
		case TEXT_ALIGN_CENTER:
			x = (layer->max_width - p_row->max_width)/2;
			break;
		case TEXT_ALIGN_RIGHT:
			x = layer->max_width - p_row->max_width;
			break;
		case TEXT_ALIGN_LEFT:
		default:
			x = 0;
			break;
		}
		y += p_row->top_spacing;
		x += layer_pos.x;
		x += layer->offset_x;
		
		/* 确定从哪个文字开始绘制 */
		for( col=0; col<p_row->string_len; ++col ) {
			p_char = p_row->string[col];
			/* 忽略无字体位图的文字 */
			if( !p_char->bitmap ) {
				continue;
			}
			x += p_char->bitmap->advance.x;
			if( x > area.x ) {
				x -= p_char->bitmap->advance.x;
				break;
			}
		}
		/* 若一整行的文本都不在可绘制区域内 */
		if( col >= p_row->string_len ) {
			y += p_row->max_height;
			y += p_row->bottom_spacing;
			continue;
		}
		/* 遍历该行的文字 */
		for( ; col<p_row->string_len; ++col ) {
			p_char = p_row->string[col];
			if( !p_char->bitmap ) {
				continue;
			}
			/* 计算字体位图的绘制坐标 */
			char_pos.x = x + p_char->bitmap->left;
			char_pos.y = y + p_row->max_height*4/5;
			char_pos.y -= p_char->bitmap->top;
			x += p_char->bitmap->advance.x;
			/* 判断文字使用的前景颜色，再进行绘制 */
			if( p_char->style && p_char->style->_fore_color ) {
				FontBMP_Mix( 
					graph, char_pos,
					p_char->bitmap,
					p_char->style->fore_color,
					need_replace );
			} else {
				FontBMP_Mix( 
					graph, char_pos,
					p_char->bitmap, 
					layer->text_style.fore_color,
					need_replace 
				);
			}
			/* 如果超过绘制区域则不继续绘制该行文本 */
			if( x > area.x + area.width ) {
				break;
			}
		}
		y += p_row->max_height;
		y += p_row->bottom_spacing;
		/* 超出绘制区域范围就不绘制了 */
		if( y > area.y + area.height ) {
			break;
		}
	}
	return 0;
}

/** 绘制文本 */
LCUI_API int TextLayer_Draw( LCUI_TextLayer* layer )
{
	LCUI_Rect rect;
	/* 如果文本位图缓存无效 */
	if( layer->is_using_buffer && !Graph_IsValid( &layer->graph ) ) {
		return -1;
	}
	rect.x = 0;
	rect.y = 0;
	rect.w = layer->max_width;
	rect.h = layer->max_height;
	return TextLayer_DrawToGraph( layer, rect, Pos(0,0), TRUE, &layer->graph );
}

/** 清除已记录的无效矩形 */
LCUI_API void TextLayer_ClearInvalidRect( LCUI_TextLayer *layer )
{
	LCUI_Rect *rect_ptr;
	LCUI_Graph invalid_graph;

	if( !layer->is_using_buffer ) {
		DirtyRectList_Destroy( &layer->dirty_rect );
		DirtyRectList_Init( &layer->dirty_rect );
		return;
	}

	while( rect_ptr = (LCUI_Rect*)LinkedList_Get( &layer->dirty_rect ) ) {
		Graph_Quote( &invalid_graph, &layer->graph, *rect_ptr );
		Graph_FillAlpha( &invalid_graph, 0 );
		LinkedList_ToNext( &layer->dirty_rect );
	}
	DirtyRectList_Destroy( &layer->dirty_rect );
	DirtyRectList_Init( &layer->dirty_rect );
}

/** 设置全局文本样式 */
LCUI_API void TextLayer_SetTextStyle( LCUI_TextLayer *layer,
					LCUI_TextStyle *style )
{
	layer->text_style = *style;
	layer->task.update_bitmap = TRUE;
}
