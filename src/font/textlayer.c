/* ***************************************************************************
 * textlayer.c -- text bitmap layer processing module.
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
 * textlayer.c -- 文本图层处理模块
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
#include <LCUI/graph.h>
#include <LCUI/font.h>

#define TextLayer_GetRow(layer, n) (n >= layer->row_list.length) ? NULL:layer->row_list.rows[n]

/* 根据对齐方式，计算文本行的起始X轴位置 */
#define TextLayer_GetRowStartX( layer, p_row, x ) \
	switch( layer->text_align ) { \
	case SV_CENTER: x = (layer->max_width - p_row->width)/2; break; \
	case SV_RIGHT: x = layer->max_width - p_row->width; break; \
	case SV_LEFT: \
	default: x = 0; break; \
	}

/** 获取文本行总数 */
int TextLayer_GetRowTotal( LCUI_TextLayer layer )
{
	return layer->row_list.length;
}

/** 获取指定文本行的高度 */
int TextLayer_GetRowHeight( LCUI_TextLayer layer, int row )
{
        if( row >= layer->row_list.length ) {
                return 0;
        }
        return layer->row_list.rows[row]->height;
}

/** 获取指定文本行的文本长度 */
int TextLayer_GetRowTextLength( LCUI_TextLayer layer, int row )
{
        if( row >= layer->row_list.length ) {
                return -1;
        }
        return layer->row_list.rows[row]->length;
}

static void TaskData_Init( TaskData *task )
{
	task->redraw_all = 0;
	task->update_bitmap = 0;
	task->update_typeset = 0;
	task->typeset_start_row = 0;
}

/** 添加 更新文本排版 的任务 */
static void TaskData_AddUpdateTypeset( TaskData *task, int start_row )
{
	if( start_row < task->typeset_start_row ) {
		task->typeset_start_row = start_row;
	}
	task->update_typeset = TRUE;
}

static void TextRow_Init( TextRowData* p_row )
{
	p_row->text_height = 0;
	p_row->width = 0;
	p_row->height = 0;
	p_row->string = NULL;
	p_row->length = 0;
	p_row->max_length = 0;
	p_row->eol = EOL_NONE;
}

static void TextRow_Destroy( TextRowData *p_row )
{
	int i;
	for( i=0; i<p_row->max_length; ++i ) {
		if( p_row->string[i] ) {
			free( p_row->string[i] );
		}
	}
	p_row->width = 0;
	p_row->height = 0;
	p_row->length = 0;
	p_row->text_height = 0;
	p_row->max_length = 0;
	free( p_row->string );
	p_row->string = NULL;
}

TextRowData* TextRowList_AddNewRow( TextRowList *rowlist )
{
	TextRowData *row_ptr, **new_rowlist;
	
	++rowlist->length;
	if( rowlist->max_length <= rowlist->length ) {
		new_rowlist = (TextRowData**)realloc( rowlist->rows, 
			sizeof(TextRowData*)*(rowlist->length+1) );
		if( !new_rowlist ) {
			--rowlist->length;
			return NULL;
		}
		rowlist->rows = new_rowlist;
		rowlist->max_length = rowlist->length;
	}
	
	row_ptr = (TextRowData*)malloc( sizeof(TextRowData) );
	if( !row_ptr ) {
		return NULL;
	}
	TextRow_Init( row_ptr );
	rowlist->rows[rowlist->length-1] = row_ptr;
	return row_ptr;
}

static TextRowData* 
TextRowList_InsertNewRow( TextRowList *rowlist, int i_row )
{
	int i;
	TextRowData *row_ptr, **new_rowlist;
	
	if( i_row > rowlist->length ) {
		return NULL;
	}
	++rowlist->length;
	if( rowlist->max_length <= rowlist->length ) {
		new_rowlist = (TextRowData**)realloc( rowlist->rows, 
			sizeof(TextRowData*)*(rowlist->length+1) );
		if( !new_rowlist ) {
			--rowlist->length;
			return NULL;
		}
		rowlist->rows = new_rowlist;
		rowlist->max_length = rowlist->length;
	}

	row_ptr = (TextRowData*)malloc( sizeof(TextRowData) );
	if( !row_ptr ) {
		return NULL;
	}
	TextRow_Init( row_ptr );
	for( i=rowlist->length-1; i>i_row; --i ) {
		rowlist->rows[i] = rowlist->rows[i-1];
	}
	rowlist->rows[i_row] = row_ptr;
	return row_ptr;
}

/** 从文本行列表中删除指定文本行 */
static int TextRowList_RemoveRow( TextRowList *p_rowlist, int row )
{
	if( row < 0 || row >= p_rowlist->length ) {
		return -1;
	}
	for( ; row < p_rowlist->length-1; ++row ) {
		p_rowlist->rows[row] = p_rowlist->rows[row+1];
	}
	TextRow_Destroy( p_rowlist->rows[row] );
	p_rowlist->rows[row] = NULL;
	--p_rowlist->rows;
	return 0;
}

/** 更新文本行的尺寸 */
static void TextLayer_UpdateRowSize( LCUI_TextLayer layer, TextRowData *p_row )
{
	int i;
	TextCharData *p_char;

	p_row->width = 0;
	p_row->text_height = layer->text_style.pixel_size;
	for( i=0; i<p_row->length; ++i ) {
		p_char = p_row->string[i];
		if( !p_char->bitmap ) {
			continue;
		}
		p_row->width += p_char->bitmap->advance.x;
		if( p_row->text_height < p_char->bitmap->advance.y ) {
			p_row->text_height = p_char->bitmap->advance.y;
		}
	}
	switch( layer->line_height.type ) {
	case SVT_SCALE:
		p_row->height = p_row->text_height * layer->line_height.scale;
		break;
	case SVT_PX:
		p_row->height = p_row->text_height * layer->line_height.scale;
		break;
	default:
		p_row->height = p_row->text_height*11/10;
		break;
	}
}

/** 设置文本行的字符串长度 */
static int TextRow_SetLength( TextRowData *p_row, int new_len )
{
	TextCharData **p_new_str;

	if( new_len < 0 ) {
		new_len = 0;
	}
	p_row->length = new_len;
	if( new_len <= p_row->max_length ) {
		return 0;
	}

	p_new_str = (TextCharData**)
	realloc( p_row->string, sizeof(TextCharData*)*new_len );
	if( !p_new_str ) {
		--p_row->length;
		return -1;
	}
	p_new_str[new_len] = NULL;
	p_row->string = p_new_str;
	p_row->max_length = p_row->length;
	return 0;
}

/** 将字符数据直接插入至文本行 */
static int TextRow_Insert( TextRowData *p_row, int ins_pos, TextCharData *p_char )
{
	int i;
	TextCharData **p_new_str;
	
	if( ins_pos > p_row->length ) {
		ins_pos = p_row->length;
	}
	++p_row->length;
	if( p_row->max_length <= p_row->length ) {
		p_new_str = (TextCharData**)realloc( p_row->string, 
			sizeof(TextCharData*)*p_row->length );
		if( !p_new_str ) {
			--p_row->length;
			return -1;
		}
		p_row->string = p_new_str;
		p_row->max_length = p_row->length;
	}
	for( i=p_row->length-1; i>=ins_pos; --i ) {
		p_row->string[i] = p_row->string[i-1];
	}
	p_row->string[ins_pos] = p_char;
	return 0;
}

/** 向文本行插入一个字符数据副本 */
static int TextRow_InsertCopy( TextRowData *p_row, int ins_pos, TextCharData *p_char )
{
	TextCharData *p_copy_char;
	p_copy_char = (TextCharData*)malloc( sizeof(TextCharData) );
	*p_copy_char = *p_char;
	return TextRow_Insert( p_row, ins_pos, p_copy_char );
}

/* 将文本行中的内容向左移动 */
static void TextRow_LeftMove( TextRowData *p_row, int n )
{
	int i, j;

	if( n <= 0 ) {
		return;
	}
	if( n > p_row->length ) {
		n = p_row->length;
	}
	p_row->length -= n;
	for( i=0,j=n; i<p_row->length; ++i,++j ) {
		p_row->string[i] = p_row->string[j];
	}
}

/** 更新字体位图 */
static void 
TextChar_UpdateBitmap( TextCharData *ch, LCUI_TextStyle *style )
{
	int pixel_size, font_id;

	font_id = style->font_id;
	pixel_size = style->pixel_size;
	if( ch->style ) {
		if( ch->style->has_family ) {
			font_id = ch->style->font_id;
		}
		if( ch->style->has_pixel_size ) {
			pixel_size = ch->style->pixel_size;
		}
	}
	ch->bitmap = FontLIB_GeFontBMP( ch->char_code, font_id, pixel_size );
	//printf("char_code: %c, pixel_size: %d, font_id: %d, bitmap: %p\n", 
	//ch->char_code, style->pixel_size, style->font_id, ch->bitmap);
}

/** 新建文本图层 */
LCUI_TextLayer TextLayer_New(void)
{
	LCUI_TextLayer layer;
	layer = (LCUI_TextLayer)malloc(sizeof(struct LCUI_TextLayerRec_));
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
	layer->is_using_buffer = FALSE;
	layer->text_align = SV_LEFT;
	layer->row_list.max_length = 0;
	layer->row_list.length = 0;
	layer->row_list.rows = NULL;
	layer->line_height.type = SVT_SCALE;
	layer->line_height.scale = 1.428;
	LinkedList_Init( &layer->style_cache, sizeof(LCUI_TextStyle) );
	TextStyle_Init( &layer->text_style );
	TaskData_Init( &layer->task );
	DirtyRectList_Init( &layer->dirty_rect );
	Graph_Init( &layer->graph );
	TextRowList_InsertNewRow( &layer->row_list, 0 );
	layer->graph.color_type = COLOR_TYPE_ARGB;
	return layer;
}

static void TextRowList_Destroy( TextRowList *list )
{
	int row;
	for( row=0; row<list->length; ++row ) {
		TextRow_Destroy( list->rows[row] );
		list->rows[row] = NULL;
	}
	list->max_length = 0;
	list->length = 0;
	if( list->rows ) {
		free( list->rows );
	}
	list->rows = NULL;
}

/** 销毁TextLayer */
void TextLayer_Destroy( LCUI_TextLayer *layer_ptr )
{
	LCUI_TextLayer layer = *layer_ptr;
	DirtyRectList_Destroy( &layer->dirty_rect );
	Graph_Free( &layer->graph );
	TextRowList_Destroy( &layer->row_list );
	free( layer );
	*layer_ptr = NULL;
}

/** 获取指定文本行中的文本段的矩形区域 */
static int TextLayer_GetRowRect( LCUI_TextLayer layer, int row, int start_col, 
				int end_col, LCUI_Rect *rect )
{
	int i, x, y;
	TextRowData *p_row;

	if( row >= layer->row_list.length ) {
		return -1;
	}
	/* 先计算在有效区域内的起始行的Y轴坐标 */
	y = layer->offset_y;
	for( i=0; i<row; ++i ) {
		y += layer->row_list.rows[i]->height;
	}
	p_row = layer->row_list.rows[row];
	if( end_col < 0 || end_col >= p_row->length ) {
		end_col = p_row->length-1;
	}
	TextLayer_GetRowStartX(layer, p_row, x);
	rect->x = layer->offset_x + x;
	rect->y = y + (int)((p_row->height - p_row->text_height)/2.0);
	rect->height = p_row->text_height;
	if( start_col == 0 && end_col == p_row->length-1 ) {
		rect->width = p_row->width;
	} else {
		for( i = 0; i < start_col; ++i ) {
			if( !p_row->string[i]->bitmap ) {
				continue;
			}
			rect->x += p_row->string[i]->bitmap->advance.x;
		}
		rect->width = 0;
		for( i=start_col; i<=end_col && i<p_row->length; ++i ) {
			if( !p_row->string[i]->bitmap ) {
				continue;
			}
			rect->width += p_row->string[i]->bitmap->advance.x;
		}
	}
	LCUIRect_ValidateArea( rect, Size(layer->max_width, layer->max_height) );
	if( rect->width <= 0 || rect->height <= 0) {
		return 1;
	}
	return 0;
}

/** 标记指定文本行的矩形区域为无效 */
static void TextLayer_InvalidateRowRect( LCUI_TextLayer layer, int row,
					 int start, int end )
{
	LCUI_Rect rect;	
	if( TextLayer_GetRowRect( layer, row, start, end, &rect ) == 0 ) {
		DirtyRectList_Add( &layer->dirty_rect, &rect );
	}
}

/** 标记指定范围内容的文本行的矩形为无效 */
void TextLayer_InvalidateRowsRect( LCUI_TextLayer layer, 
				   int start_row, int end_row )
{
	int i, y;
	LCUI_Rect rect;

	if( end_row < 0 || end_row >= layer->row_list.length ) {
		end_row = layer->row_list.length-1;
	}
	
	y = layer->offset_y;
	for( i=0; i<layer->row_list.length; ++i ) {
		y += layer->row_list.rows[i]->height;
		if( y > 0 && i > start_row) {
			break;
		}
	}
	for( ; i<=end_row; ++i ) {
		TextLayer_GetRowRect( layer, i, 0, -1, &rect);
		DirtyRectList_Add( &layer->dirty_rect, &rect );
		y += layer->row_list.rows[i]->height;
		if( y >= layer->max_height ) {
			break;
		}
	}
}

/** 设置插入点的行列坐标 */
void TextLayer_SetCaretPos( LCUI_TextLayer layer, int row, int col )
{
	if( row < 0 ) {
		row = 0;
	}
	else if( row >= layer->row_list.length ) {
		if( layer->row_list.length < 0 ) {
			row = 0;
		} else	{
			row = layer->row_list.length-1;
		}
	}

	if( col < 0 ) {
		col = 0;
	}
	else if( layer->row_list.length > 0 ) {
		if( col >= layer->row_list.rows[row]->length ) {
			col = layer->row_list.rows[row]->length;
		}
	} else {
		col = 0;
	}
	layer->insert_x = col;
	layer->insert_y = row;
}

/** 根据像素坐标设置文本光标的行列坐标 */
int TextLayer_SetCaretPosByPixelPos( LCUI_TextLayer layer, int x, int y )
{
	TextRowData* p_row;
	int i, pixel_pos, ins_x, ins_y;

	for( pixel_pos=0, i=0; i<layer->row_list.length; ++i ) {
		pixel_pos += layer->row_list.rows[i]->height;;
		if( pixel_pos >= y ) {
			ins_y = i;
			break;
		}
	}
	if( i >= layer->row_list.length ) {
		if( layer->row_list.length > 0 ) {
			ins_y = layer->row_list.length-1;
		} else {
			layer->insert_x = 0;
			layer->insert_y = 0;
			return -1;
		}
	}
	p_row = layer->row_list.rows[ins_y];
	ins_x = p_row->length;
	TextLayer_GetRowStartX(layer, p_row, pixel_pos);
	for( i=0; i<p_row->length; ++i ) {
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
int TextLayer_GetCharPixelPos( LCUI_TextLayer layer, int row,
			       int col, LCUI_Pos *pixel_pos )
{
	TextRowData* p_row;
	int i, pixel_x = 0, pixel_y = 0;

	if( row < 0 || row >= layer->row_list.length ) {
		return -1;
	}
	if( col < 0 ) {
		return -2;
	}
	else if( col > layer->row_list.rows[row]->length ) {
		return -3;
	}
	/* 累加前几行的高度 */
	for( i=0; i<row; ++i ) {
		pixel_y += layer->row_list.rows[i]->height;
	}
	p_row = layer->row_list.rows[row];
	TextLayer_GetRowStartX( layer, p_row, pixel_x );
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
int TextLayer_GetCaretPixelPos( LCUI_TextLayer layer, LCUI_Pos *pixel_pos )
{
	return TextLayer_GetCharPixelPos( layer, layer->insert_y, 
					  layer->insert_x, pixel_pos );
}

/** 清空文本 */
void TextLayer_ClearText( LCUI_TextLayer layer )
{
	layer->insert_x = 0;
	layer->insert_y = 0;
	TextLayer_InvalidateRowsRect( layer, 0, -1 );
	TextRowList_Destroy( &layer->row_list );
	LinkedList_Destroy( &layer->style_cache );
	layer->task.redraw_all = TRUE;
}

/** 对文本行进行断行 */
static void TextLayer_BreakTextRow( LCUI_TextLayer layer, int i_row, 
				    int col, EOLChar eol )
{
	int n;
	TextRowData *p_row, *p_next_row;

	p_row = layer->row_list.rows[i_row];
	p_next_row = TextRowList_InsertNewRow( &layer->row_list, i_row+1 );
	/* 将本行原有的行尾符转移至下一行 */
	p_next_row->eol = p_row->eol;
	p_row->eol = eol;
	for( n=p_row->length-1; n>=col; --n ) {
		TextRow_Insert( p_next_row, 0, p_row->string[n] );
		p_row->string[n] = NULL;
	}
	p_row->length = col;
	TextLayer_UpdateRowSize( layer, p_row );
	TextLayer_UpdateRowSize( layer, p_next_row );
}

/** 对指定行的文本进行排版 */
static void TextLayer_TextRowTypeset( LCUI_TextLayer layer, int row )
{
	int col, row_width;
	TextRowData *p_row, *p_next_row;
	TextCharData *p_char;

	p_row = layer->row_list.rows[row];
	row_width = 0;
	for( col=0; col<p_row->length; ++col ) {
		p_char = p_row->string[col];
		if( !p_char->bitmap ) {
			continue;
		}
		/* 累加行宽度 */
		row_width += p_char->bitmap->advance.x;
		/* 如果是当前行的第一个字符，或者行宽度没有超过宽度限制 */
		if( layer->max_width <= 0 || !layer->is_autowrap_mode 
		 || (layer->is_autowrap_mode && !layer->is_mulitiline_mode)
		 || col < 1 || p_row->width <= layer->max_width ) {
			continue;
		}
		TextLayer_BreakTextRow( layer, row, col, EOL_NONE );
		return;
	}
	TextLayer_UpdateRowSize( layer, p_row );
	/* 如果本行有换行符，或者是最后一行 */
	if( p_row->eol != EOL_NONE || row == layer->row_list.length-1 ) {
		return;
	}
	
	row_width = p_row->width;
	/* 本行的文本宽度未达到限制宽度，需要将下行的文本转移至本行 */
	while( p_row->eol != EOL_NONE ) {
		/* 获取下一行的指针 */
		p_next_row = TextLayer_GetRow( layer, row+1 );
		if( !p_next_row ) {
			return;
		}
		for( col=0; col<p_next_row->length; ++col ) {
			p_char = p_next_row->string[col];
			/* 忽略无字体位图的文字 */
			if( !p_char->bitmap ) {
				TextRow_Insert( p_row, p_row->length, p_char );
				p_next_row->string[col] = NULL;
				continue;
			}
			row_width += p_char->bitmap->advance.x;
			/* 如果没有超过宽度限制 */
			if( !layer->is_autowrap_mode || layer->max_width <= 0
			 || (layer->is_autowrap_mode && !layer->is_mulitiline_mode)
			 || p_row->width <= layer->max_width ) {
				TextRow_Insert( p_row, p_row->length, p_char );
				p_next_row->string[col] = NULL;
				continue;
			}
			/* 如果插入点在下一行 */
			if( layer->insert_y == row+1 ) {
				/* 如果插入点处于被转移的几个文字中 */
				if( layer->insert_x < col ) {
					layer->insert_y = row;
					layer->insert_x += p_row->length;
				} else {
					/* 否则，减去被转移的文字数 */
					layer->insert_x -= col;
				}
			}
			/* 将这一行剩余的文字向前移 */
			TextRow_LeftMove( p_next_row, col );
			TextLayer_UpdateRowSize( layer, p_row );
			return;
		}
		TextLayer_UpdateRowSize( layer, p_row );
		TextLayer_InvalidateRowRect( layer, row, 0, -1 );
		TextLayer_InvalidateRowRect( layer, row+1, 0, -1 );
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
static void TextLayer_TextTypeset( LCUI_TextLayer layer, int start_row )
{
	int row;
	/* 记录排版前各个文本行的矩形区域 */
	TextLayer_InvalidateRowsRect( layer, start_row, -1 );
	for( row=start_row; row<layer->row_list.length; ++row ) {
		TextLayer_TextRowTypeset( layer, row );
	}
	/* 记录排版后各个文本行的矩形区域 */
	TextLayer_InvalidateRowsRect( layer, start_row, -1 );
}

/** 对文本进行预处理 */ 
static int TextLayer_ProcessText( LCUI_TextLayer layer, 
				  const wchar_t *wstr,
				  int add_type, 
				  LinkedList *tags )
{
	EOLChar eol;
	const wchar_t *p_end, *p, *pp;
	int cur_col, cur_row, start_row, ins_x, ins_y;
	TextRowData *p_row;
	TextCharData char_data;
	LinkedList tmp_tags;
	LCUI_TextStyle *style = NULL;
	LCUI_BOOL is_tmp_tag_stack, need_typeset, rect_has_added;

	if( !wstr ) {
		return -1;
	}
	is_tmp_tag_stack = FALSE;
	need_typeset = FALSE;
	rect_has_added = FALSE;
	/* 如果是将文本追加至文本末尾 */
	if( add_type == TEXT_ADD_TYPE_APPEND ) {
		if( layer->row_list.length > 0 ) {
			cur_row = layer->row_list.length - 1;
		} else {
			cur_row = 0;
		}
		p_row =  TextLayer_GetRow( layer, cur_row );
		if( !p_row ) {
			p_row = TextRowList_AddNewRow( &layer->row_list );
		}
		cur_col = p_row->length;
	} else { /* 否则，是将文本插入至当前插入点 */
		cur_row = layer->insert_y;
		cur_col = layer->insert_x;
		p_row = TextLayer_GetRow( layer, cur_row );
		if( !p_row ) {
			p_row = TextRowList_AddNewRow( &layer->row_list );
		}
	}
	start_row = cur_row;
	ins_x = cur_col;
	ins_y = cur_row;
	/* 如果没有可用的标签栈，则使用临时的标签栈 */
	if( !tags ) {
		is_tmp_tag_stack = TRUE;
		StyleTags_Init( &tmp_tags );
		tags = &tmp_tags;
	}
	p_end = wstr + wcslen(wstr);
	for( p=wstr; p<p_end; ++p ) {
		/* 如果启用的样式标签支持，则处理样式的结束标签 */ 
		if( layer->is_using_style_tags ) {
			pp = StyleTags_ScanEndingTag( tags, p );
			if( pp ) {
				/* 抵消本次循环后的++p，以在下次循环时还能够在当前位置 */
				p = pp - 1;
				style = StyleTags_GetTextStyle( tags );
				LinkedList_Append( &layer->style_cache, style );
				continue;
			}
			pp = StyleTags_ScanBeginTag( tags, p );
			if( pp ) {
				p = pp - 1;
				style = StyleTags_GetTextStyle( tags );
				LinkedList_Append( &layer->style_cache, style );
				continue;
			}
		}
		
		if( *p == '\r' || *p == '\n' ) {
			/* 如果后面还有 \n，则说明是CR/LF换行模式 */
			if( *p == '\r' ) {
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
			p_row = TextLayer_GetRow( layer, ins_y );
			continue;
		}

		char_data.char_code = *p;
		char_data.style = style;
		TextChar_UpdateBitmap( &char_data, &layer->text_style );
		TextRow_InsertCopy( p_row, ins_x, &char_data );
		++ins_x;
	}
	/* 更新当前行的尺寸 */
	TextLayer_UpdateRowSize( layer, p_row );
	if( add_type == TEXT_ADD_TYPE_INSERT ) {
		layer->insert_x = ins_x;
		layer->insert_y = ins_y;
	}
	/* 若启用了自动换行模式，则标记需要重新对文本进行排版 */
	if( layer->is_autowrap_mode || need_typeset ) {
		TaskData_AddUpdateTypeset( &layer->task, cur_row );
	} else {
		TextLayer_InvalidateRowRect( layer, cur_row, 0, -1 );
	}
	/* 如果已经记录过文本行矩形区域 */
	if( rect_has_added ) {
		TextLayer_InvalidateRowsRect( layer, start_row, -1 );
		rect_has_added = TRUE;
	}
	/* 如果使用的是临时标签栈，则销毁它 */
	if( is_tmp_tag_stack ) {
		StyleTags_Destroy( tags );
	}
	return 0;
}

/** 插入文本内容（宽字符版） */
int TextLayer_InsertTextW( LCUI_TextLayer layer, const wchar_t *wstr,
			   LinkedList *tag_stack )
{
	return TextLayer_ProcessText( layer, wstr, TEXT_ADD_TYPE_INSERT, tag_stack );
}

/** 插入文本内容 */
int TextLayer_InsertTextA( LCUI_TextLayer layer, const char *str )
{
	return 0;
}

/** 插入文本内容（UTF-8版） */
int TextLayer_InsertText( LCUI_TextLayer layer, const char *utf8_str )
{
	return 0;
}

/** 追加文本内容（宽字符版） */
int TextLayer_AppendTextW( LCUI_TextLayer layer, const wchar_t *wstr, 
			   LinkedList *tag_stack )
{
	return TextLayer_ProcessText( layer, wstr, TEXT_ADD_TYPE_APPEND, tag_stack );
}

/** 追加文本内容 */
int TextLayer_AppendTextA( LCUI_TextLayer layer, const char *ascii_text )
{
	return 0;
}

/** 追加文本内容（UTF-8版） */
int TextLayer_AppendText( LCUI_TextLayer layer, const char *utf8_text )
{
	return 0;
}

/** 设置文本内容（宽字符版） */
int TextLayer_SetTextW( LCUI_TextLayer layer, const wchar_t *wstr,
			LinkedList *tag_stack )
{
	TextLayer_ClearText( layer );
	return TextLayer_AppendTextW( layer, wstr, tag_stack );
}

/** 设置文本内容 */
int TextLayer_SetTextA( LCUI_TextLayer layer, const char *ascii_text )
{
	return 0;
}

/** 设置文本内容（UTF-8版） */
int TextLayer_SetText( LCUI_TextLayer layer, const char *utf8_text )
{
	return 0;
}

/** 获取文本图层中的文本（宽字符版） */
int TextLayer_GetTextW( LCUI_TextLayer layer, int start_pos,
			int max_len, wchar_t *wstr_buff )
{
	int row, col = 0, i;
	TextRowData *row_ptr;

	if( start_pos<0 ) {
		return -1;
	}
	if( max_len <= 0 ) {
		return 0;
	}
	/* 先根据一维坐标计算行列坐标 */
	for( i=0,row=0; row<layer->row_list.max_length; ++row ) {
		if( i >= start_pos ) {
			col = start_pos - i;
			break;
		}
		i += layer->row_list.rows[row]->length;
	}
	for( i=0; row < layer->row_list.max_length && i < max_len; ++row ) {
		row_ptr = layer->row_list.rows[row];
		for( ; col < row_ptr->length && i < max_len; ++col,++i ) {
			wstr_buff[i] = row_ptr->string[col]->char_code;
		}
	}
	wstr_buff[i] = L'\0';
	return i;
}

/** 获取文本位图缓存 */
LCUI_Graph* TextLayer_GetGraphBuffer( LCUI_TextLayer layer )
{
	if( layer->is_using_buffer ) {
		return &layer->graph;
	}
	return NULL;
}

/** 设置最大尺寸 */
int TextLayer_SetMaxSize( LCUI_TextLayer layer, LCUI_Size new_size )
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
void TextLayer_SetMultiline( LCUI_TextLayer layer, int is_true )
{
	if( (layer->is_mulitiline_mode && !is_true)
	 || (!layer->is_mulitiline_mode && is_true) ) {
		layer->is_mulitiline_mode = is_true;
		TaskData_AddUpdateTypeset( &layer->task, 0 );;
	}
}

/** 删除指定行列的文字及其右边的文本 */
static int TextLayer_DeleteText( LCUI_TextLayer layer, int char_y, 
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
	if( char_y >= layer->row_list.length ) {
		return -2;
	}
	p_row = layer->row_list.rows[char_y];
	if( char_x > p_row->length ) {
		char_x = p_row->length;
	}
	end_x = char_x;
	end_y = char_y;
	--n_char;
	/* 计算结束点的位置 */
	for( end_y=char_y; end_y<layer->row_list.length && n_char>0; ++end_y ) {
		p_row = layer->row_list.rows[end_y];
		if( p_row->eol == EOL_NONE ) {
			if( end_x + n_char < p_row->length ) {
				end_x += n_char;
				n_char = 0;
				break;
			}
		} else {
			if( end_x + n_char <= p_row->length ) {
				end_x += n_char;
				n_char = 0;
				break;
			}
		}
		n_char -= (p_row->length - end_x);
		end_x = 0;
	}
	if( end_y >= layer->row_list.length ) {
		end_y = layer->row_list.length-1;
	}
	p_end_row = layer->row_list.rows[end_y];
	if( end_x > p_end_row->length ) {
		end_x = p_end_row->length;
	}
	/* 获取上一行文本 */
	p_prev_row = layer->row_list.rows[char_y-1];
	// 计算起始行与结束行拼接后的长度
	// 起始行：0 1 2 3 4 5，起点位置：2
	// 结束行：0 1 2 3 4 5，终点位置：4
	// 拼接后的长度：2 + 6 - 4 - 1 = 3
	len = char_x + p_end_row->length - end_x - 1;
	if( len < 0 ) {
		return -3;
	}
	/* 如果是同一行 */
	if( p_row == p_end_row ) {
		if( end_x >= p_end_row->length ) {
			return -4;
		}
		TextLayer_InvalidateRowRect( layer, char_y, char_x, -1 );
		TaskData_AddUpdateTypeset( &layer->task, char_y );
		for( i=char_x, j=end_x+1; j<p_row->length; ++i,++j ) {
			p_row->string[i] = p_row->string[j];
		}
		/* 如果当前行为空，也不是第一行，并且上一行没有结束符 */
		if( len <= 0 && end_y > 0 && p_prev_row->eol != EOL_NONE ) {
			TextRowList_RemoveRow( &layer->row_list, end_y );
		} 
		/* 调整起始行的容量 */
		TextRow_SetLength( p_row, len );
		/* 更新文本行的尺寸 */
		TextLayer_UpdateRowSize( layer, p_row );
		return 0;
	}
	/* 如果结束点在行尾，并且该行不是最后一行 */
	if( end_x == p_end_row->length && end_y < layer->row_list.length-1 ) {
		++end_y;
		p_end_row = TextLayer_GetRow( layer, end_y );
		end_x = -1;
		len = char_x + p_end_row->length;
	}
	TextRow_SetLength( p_row, len );
	/* 标记当前行后面的所有行的矩形需区域需要刷新 */
	TextLayer_InvalidateRowsRect( layer, char_y+1, -1 );
	/* 移除起始行与结束行之间的文本行 */
	for( i=char_y+1, j=i; j<end_y; ++j ) {
		TextLayer_InvalidateRowRect( layer, i, 0, -1 );
		TextRowList_RemoveRow( &layer->row_list, i );
	}
	end_y = char_y + 1;
	/* 将结束行的内容拼接至起始行 */
	for( i=char_x, j=end_x+1; i<len && j<p_end_row->length; ++i,++j ) {
		p_row->string[i] = p_end_row->string[j];
	}
	TextLayer_UpdateRowSize( layer, p_row );
	TextLayer_InvalidateRowRect( layer, end_y, 0, -1 );
	/* 移除结束行 */
	TextRowList_RemoveRow( &layer->row_list, end_y );
	/* 如果起始行无内容，并且上一行没有结束符（换行符），则
	 * 说明需要删除起始行 */
	if( len <= 0 && char_y > 0 && p_prev_row->eol != EOL_NONE ) {
		TextLayer_InvalidateRowRect( layer, char_y, 0, -1 );
		TextRowList_RemoveRow( &layer->row_list, char_y );
	}
	TaskData_AddUpdateTypeset( &layer->task, char_y );;
	return 0;
}

/** 删除文本光标的当前坐标右边的文本 */
int TextLayer_Delete( LCUI_TextLayer layer, int n_char )
{
	return TextLayer_DeleteText(	layer, layer->insert_y, 
					layer->insert_x, n_char );
}

/** 退格删除文本，即删除文本光标的当前坐标左边的文本 */
int TextLayer_Backspace( LCUI_TextLayer layer, int n_char )
{
	int n_del;
	int char_x, char_y;
	TextRowData* p_row;

	/* 先获取当前字的位置 */
	char_x = layer->insert_x;
	char_y = layer->insert_y;
	/* 再计算删除 n_char 个字后的位置 */
	for( n_del=n_char; char_y>=0; --char_y ) {
		p_row = layer->row_list.rows[char_y];
		/* 如果不是当前行，则重定位至行尾 */
		if( char_y < layer->insert_y ) {
			char_x = p_row->length;
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
	if( char_x == 0 && layer->row_list.length > 0 
	 && char_y >= layer->row_list.length ) {
		char_y = layer->row_list.length-1;
		char_x = layer->row_list.rows[char_y]->length;
	}
	/* 更新文本光标的位置 */
	TextLayer_SetCaretPos( layer, char_y, char_x );
	return 0;
}

/** 设置是否启用自动换行模式 */
void TextLayer_SetAutoWrap( LCUI_TextLayer layer, int is_true )
{
	if( (!layer->is_autowrap_mode && is_true)
	 || (layer->is_autowrap_mode && !is_true) ) {
		layer->is_autowrap_mode = is_true;
		TaskData_AddUpdateTypeset( &layer->task, 0 );
	}
}

/** 设置是否使用样式标签 */
void TextLayer_SetUsingStyleTags( LCUI_TextLayer layer, LCUI_BOOL is_true )
{
	layer->is_using_style_tags = is_true;
}

/** 计算并获取文本的宽度 */
int TextLayer_GetWidth( LCUI_TextLayer layer )
{
	int i, row, w, max_w;
	TextRowData* p_row;

	for( row=0,max_w=0; row<layer->row_list.length; ++row ) {
		p_row = layer->row_list.rows[row];
		for( i=0, w=0; i<p_row->length; ++i ) {
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
int TextLayer_GetHeight( LCUI_TextLayer layer )
{
	int i, h;
	for( i=0,h=0; i<layer->row_list.length; ++i ) {
		h += layer->row_list.rows[i]->height;
	}
	return h;
}

/** 重新载入各个文字的字体位图 */
void TextLayer_ReloadCharBitmap( LCUI_TextLayer layer )
{
	int row, col;
	TextCharData* p_char;
	TextRowData* p_row;

	for( row=0; row<layer->row_list.length; ++row ) {
		p_row = layer->row_list.rows[row];
		for( col=0; col<p_row->length; ++col ) {
			p_char = p_row->string[col];
			TextChar_UpdateBitmap( p_char, &layer->text_style );
			if( !p_char->bitmap ) {
				continue;
			}
		}
		TextLayer_UpdateRowSize( layer, p_row );
	}
}

/** 更新数据 */
void TextLayer_Update( LCUI_TextLayer layer, LinkedList *rect_list )
{
	if( layer->task.update_bitmap ) {
		TextLayer_InvalidateRowsRect( layer, 0, -1 );
		TextLayer_ReloadCharBitmap( layer );
		TextLayer_InvalidateRowsRect( layer, 0, -1 );
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
		TextLayer_InvalidateRowsRect( layer, 0, -1 );
		layer->offset_x = layer->new_offset_x;
		layer->offset_y = layer->new_offset_y;
		TextLayer_InvalidateRowsRect( layer, 0, -1 );
		layer->task.redraw_all = TRUE;
	}
	
	if( rect_list ) {
		void *data_ptr;
		/* 转移脏矩形记录，供利用 */
		LinkedList_ForEach( data_ptr, 0, &layer->dirty_rect ) {
			LinkedList_AppendCopy( rect_list, data_ptr );
		}
	 } 
}

/** 
 * 将文本图层中的指定区域的内容绘制至目标图像中
 * @param layer 要使用的文本图层
 * @param area 文本图层中需要绘制的区域
 * @param layer_pos 文本图层在目标图像中的位置
 * @param graph 目标图像
 */
int TextLayer_DrawToGraph( LCUI_TextLayer layer, LCUI_Rect area,
			   LCUI_Pos layer_pos, LCUI_Graph *graph )
{
	int x, y, row, col;
	LCUI_Pos char_pos;
	TextRowData *p_row;
	TextCharData *p_char;
	LCUI_Size box_size;

	box_size.w = layer->max_width;
	box_size.h = layer->max_height;
	/* 调整区域范围，使之有效 */
	LCUIRect_ValidateArea( &area, box_size );
	/* 加上Y轴坐标偏移量 */
	y = layer->offset_y;
	/* 先确定从哪一行开始绘制 */
	for( row=0; row<layer->row_list.length; ++row ) {
		p_row = TextLayer_GetRow( layer, row );
		y += p_row->height;
		if( y > area.y ) {
			y -= p_row->height;
			break;
		}
	}
	/* 如果没有可绘制的文本行 */
	if( row >= layer->row_list.length ) {
		return -1;
	}
	for( ; row < layer->row_list.length; ++row ) {
		p_row = TextLayer_GetRow( layer, row );
		TextLayer_GetRowStartX( layer, p_row, x );
		x += layer_pos.x;
		x += layer->offset_x;
		/* 确定从哪个文字开始绘制 */
		for( col=0; col<p_row->length; ++col ) {
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
		if( col >= p_row->length ) {
			y += p_row->height;
			continue;
		}
		/* 遍历该行的文字 */
		for( ; col<p_row->length; ++col ) {
			p_char = p_row->string[col];
			if( !p_char->bitmap ) {
				continue;
			}
			/* 计算字体位图的绘制坐标 */
			char_pos.x = x + p_char->bitmap->left;
			char_pos.y = y + p_row->height*4/5;
			char_pos.y -= p_char->bitmap->top;
			x += p_char->bitmap->advance.x;
			/* 判断文字使用的前景颜色，再进行绘制 */
			if( p_char->style && p_char->style->has_fore_color ) {
				FontBMP_Mix( graph, char_pos, p_char->bitmap,
					     p_char->style->fore_color );
			} else {
				FontBMP_Mix( graph, char_pos, p_char->bitmap,
					     layer->text_style.fore_color );
			}
			/* 如果超过绘制区域则不继续绘制该行文本 */
			if( x > area.x + area.width ) {
				break;
			}
		}
		y += p_row->height;
		/* 超出绘制区域范围就不绘制了 */
		if( y > area.y + area.height ) {
			break;
		}
	}
	return 0;
}

/** 绘制文本 */
int TextLayer_Draw( LCUI_TextLayer layer )
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
	return TextLayer_DrawToGraph( layer, rect, Pos(0,0), &layer->graph );
}

/** 清除已记录的无效矩形 */
void TextLayer_ClearInvalidRect( LCUI_TextLayer layer )
{
	LCUI_Rect *rect_ptr;
	LCUI_Graph invalid_graph;

	if( !layer->is_using_buffer ) {
		DirtyRectList_Destroy( &layer->dirty_rect );
		DirtyRectList_Init( &layer->dirty_rect );
		return;
	}
	LinkedList_ForEach( rect_ptr, 0, &layer->dirty_rect ) {
		Graph_Quote( &invalid_graph, &layer->graph, rect_ptr );
		Graph_FillAlpha( &invalid_graph, 0 );
	}
	DirtyRectList_Destroy( &layer->dirty_rect );
	DirtyRectList_Init( &layer->dirty_rect );
}

/** 设置全局文本样式 */
void TextLayer_SetTextStyle( LCUI_TextLayer layer, LCUI_TextStyle *style )
{
	layer->text_style = *style;
	layer->task.update_bitmap = TRUE;
}

/** 设置文本对齐方式 */
void TextLayer_SetTextAlign( LCUI_TextLayer layer, int align )
{
	layer->text_align = align;
	layer->task.update_typeset = TRUE;
}

/** 设置文本行的高度 */
void TextLayer_SetLineHeight( LCUI_TextLayer layer, LCUI_Style *val )
{
	layer->line_height = *val;
	layer->task.update_typeset = TRUE;
}
