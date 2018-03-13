/*
 * textlayer.c -- Text layout and rendering module.
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

#include <stdlib.h>
#include <ctype.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/font.h>

 /** 文本添加类型 */
enum TextAddType {
	TAT_INSERT,	/**< 插入至插入点处 */
	TAT_APPEND	/**< 追加至文本末尾 */
};

#define TextRowList_AddNewRow(ROWLIST) \
	TextRowList_InsertNewRow(ROWLIST, (ROWLIST)->length)
#define TextLayer_GetRow(layer, n) \
	(n >= layer->text_rows.length) ? NULL:layer->text_rows.rows[n]
#define GetDefaultLineHeight(H) iround( H * 1.42857143 )

/* 根据对齐方式，计算文本行的起始X轴位置 */
static int TextLayer_GetRowStartX( LCUI_TextLayer layer, LCUI_TextRow txtrow )
{
	int width;
	if( layer->fixed_width > 0 ) {
		width = layer->fixed_width;
	} else {
		width = layer->width;
	}
	switch( layer->text_align ) {
	case SV_CENTER: return (width - txtrow->width) / 2;
	case SV_RIGHT: return width - txtrow->width;
	case SV_LEFT:
	default: break;
	}
	return 0;
}

/** 获取文本行总数 */
int TextLayer_GetRowTotal( LCUI_TextLayer layer )
{
	return layer->text_rows.length;
}

/** 获取指定文本行的高度 */
int TextLayer_GetRowHeight( LCUI_TextLayer layer, int row )
{
        if( row >= layer->text_rows.length ) {
                return 0;
        }
        return layer->text_rows.rows[row]->height;
}

/** 获取指定文本行的文本长度 */
int TextLayer_GetRowTextLength( LCUI_TextLayer layer, int row )
{
        if( row >= layer->text_rows.length ) {
                return -1;
        }
        return layer->text_rows.rows[row]->length;
}

/** 添加 更新文本排版 的任务 */
void TextLayer_AddUpdateTypeset( LCUI_TextLayer layer, int start_row )
{
	if( start_row < layer->task.typeset_start_row ) {
		layer->task.typeset_start_row = start_row;
	}
	layer->task.update_typeset = TRUE;
}

static void TextRow_Init( LCUI_TextRow txtrow )
{
	txtrow->width = 0;
	txtrow->height = 0;
	txtrow->length = 0;
	txtrow->string = NULL;
	txtrow->eol = LCUI_EOL_NONE;
	txtrow->text_height = 0;
}

static void TextRow_Destroy( LCUI_TextRow txtrow )
{
	int i;
	for( i = 0; i < txtrow->length; ++i ) {
		if( txtrow->string[i] ) {
			free( txtrow->string[i] );
		}
	}
	txtrow->width = 0;
	txtrow->height = 0;
	txtrow->length = 0;
	txtrow->text_height = 0;
	if( txtrow->string ) {
		free( txtrow->string );
	}
	txtrow->string = NULL;
}

/** 向文本行列表中插入新的文本行 */
static LCUI_TextRow TextRowList_InsertNewRow( LCUI_TextRowList rowlist,
					      int i_row )
{
	int i, size;
	LCUI_TextRow txtrow, *txtrows;
	if( i_row > rowlist->length ) {
		i_row = rowlist->length;
	}
	++rowlist->length;
	size = sizeof( LCUI_TextRow )*(rowlist->length + 1);
	txtrows = realloc( rowlist->rows, size );
	if( !txtrows ) {
		--rowlist->length;
		return NULL;
	}
	txtrows[rowlist->length] = NULL;
	txtrow = malloc( sizeof( LCUI_TextRowRec ) );
	if( !txtrow ) {
		--rowlist->length;
		return NULL;
	}
	TextRow_Init( txtrow );
	for( i = rowlist->length - 1; i > i_row; --i ) {
		txtrows[i] = txtrows[i - 1];
	}
	txtrows[i_row] = txtrow;
	rowlist->rows = txtrows;
	return txtrow;
}

/** 从文本行列表中删除指定文本行 */
static int TextRowList_RemoveRow( LCUI_TextRowList rowlist, int i_row )
{
	if( i_row < 0 || i_row >= rowlist->length ) {
		return -1;
	}
	TextRow_Destroy( rowlist->rows[i_row] );
	free( rowlist->rows[i_row] );
	for( ; i_row < rowlist->length - 1; ++i_row ) {
		rowlist->rows[i_row] = rowlist->rows[i_row + 1];
	}
	rowlist->rows[i_row] = NULL;
	--rowlist->length;
	return 0;
}

/** 更新文本行的尺寸 */
static void TextLayer_UpdateRowSize( LCUI_TextLayer layer,
				     LCUI_TextRow txtrow )
{
	int i;
	LCUI_TextChar txtchar;
	txtrow->width = 0;
	txtrow->text_height = layer->text_default_style.pixel_size;
	for( i = 0; i < txtrow->length; ++i ) {
		txtchar = txtrow->string[i];
		if( !txtchar->bitmap ) {
			continue;
		}
		txtrow->width += txtchar->bitmap->advance.x;
		if( txtrow->text_height < txtchar->bitmap->advance.y ) {
			txtrow->text_height = txtchar->bitmap->advance.y;
		}
	}
	if( layer->line_height > -1 ) {
		txtrow->height = layer->line_height;
	} else {
		txtrow->height = GetDefaultLineHeight( txtrow->height );
	}
}

/** 设置文本行的字符串长度 */
static int TextRow_SetLength( LCUI_TextRow txtrow, int len )
{
	LCUI_TextChar *txtstr;
	if( len < 0 ) {
		len = 0;
	}
	txtstr = realloc( txtrow->string, sizeof( LCUI_TextChar )*(len + 1) );
	if( !txtstr ) {
		return -1;
	}
	txtstr[len] = NULL;
	txtrow->string = txtstr;
	txtrow->length = len;
	return 0;
}

/** 将字符数据直接插入至文本行 */
static int TextRow_Insert( LCUI_TextRow txtrow, int ins_pos,
			   LCUI_TextChar txtchar )
{
	int i;
	if( ins_pos < 0 ) {
		ins_pos = txtrow->length + 1 + ins_pos;
		if( ins_pos < 0 ) {
			ins_pos = 0;
		}
	} else if( ins_pos > txtrow->length ) {
		ins_pos = txtrow->length;
	}
	TextRow_SetLength( txtrow, txtrow->length + 1 );
	for( i = txtrow->length - 1; i > ins_pos; --i ) {
		txtrow->string[i] = txtrow->string[i - 1];
	}
	txtrow->string[ins_pos] = txtchar;
	return 0;
}

/** 向文本行插入一个字符数据副本 */
static int TextRow_InsertCopy( LCUI_TextRow txtrow, int ins_pos,
			       LCUI_TextChar txtchar )
{
	LCUI_TextChar txtchar2;
	txtchar2 = malloc( sizeof(LCUI_TextCharRec) );
	*txtchar2 = *txtchar;
	return TextRow_Insert( txtrow, ins_pos, txtchar2 );
}

/** 更新字体位图 */
static void TextChar_UpdateBitmap( LCUI_TextChar ch, LCUI_TextStyle style )
{
	int i = 0;
	int size = style->pixel_size;
	int *font_ids = style->font_ids;
	if( ch->style ) {
		if( ch->style->has_family ) {
			font_ids = ch->style->font_ids;
		}
		if( ch->style->has_pixel_size ) {
			size = ch->style->pixel_size;
		}
	}
	while( font_ids && font_ids[i] >= 0 ) {
		int ret = LCUIFont_GetBitmap( ch->code, font_ids[i],
					      size, &ch->bitmap );
		if( ret == 0 ) {
			return;
		}
		++i;
	}
	LCUIFont_GetBitmap( ch->code, -1, size, &ch->bitmap );
}

/** 新建文本图层 */
LCUI_TextLayer TextLayer_New(void)
{
	LCUI_TextLayer layer;
	layer = malloc( sizeof( LCUI_TextLayerRec ) );
	layer->width = 0;
	layer->length = 0;
	layer->offset_x = 0;
	layer->offset_y = 0;
	layer->insert_x = 0;
	layer->insert_y = 0;
	layer->max_width = 0;
	layer->max_height = 0;
	layer->fixed_width = 0;
	layer->fixed_height = 0;
	layer->new_offset_x = 0;
	layer->new_offset_y = 0;
	layer->line_height = -1;
	layer->text_rows.length = 0;
	layer->text_rows.rows = NULL;
	layer->text_align = SV_LEFT;
	layer->enable_canvas = FALSE;
	layer->enable_autowrap = FALSE;
	layer->enable_mulitiline = FALSE;
	layer->enable_style_tags = FALSE;
	layer->word_break = LCUI_WORD_BREAK_NORMAL;
	TextStyle_Init( &layer->text_default_style );
	LinkedList_Init( &layer->text_styles );
	layer->task.typeset_start_row = 0;
	layer->task.update_typeset = 0;
	layer->task.update_bitmap = 0;
	layer->task.redraw_all = 0;
	Graph_Init( &layer->canvas );
	LinkedList_Init( &layer->dirty_rects );
	layer->canvas.color_type = COLOR_TYPE_ARGB;
	TextRowList_InsertNewRow( &layer->text_rows, 0 );
	return layer;
}

static void TextRowList_Destroy( LCUI_TextRowList list )
{
	int row;
	for( row=0; row<list->length; ++row ) {
		TextRow_Destroy( list->rows[row] );
		free( list->rows[row] );
		list->rows[row] = NULL;
	}
	list->length = 0;
	if( list->rows ) {
		free( list->rows );
	}
	list->rows = NULL;
}

static void OnDestroyTextStyle( void *data )
{
	TextStyle_Destroy( data );
	free( data );
}

static void TextLayer_DestroyStyleCache( LCUI_TextLayer layer )
{
	LinkedList_Clear( &layer->text_styles, OnDestroyTextStyle );
}

/** 销毁TextLayer */
void TextLayer_Destroy( LCUI_TextLayer layer )
{

	RectList_Clear( &layer->dirty_rects );
	TextStyle_Destroy( &layer->text_default_style );
	TextRowList_Destroy( &layer->text_rows );
	TextLayer_DestroyStyleCache( layer );
	Graph_Free( &layer->canvas );
	free( layer );
}

/** 获取指定文本行中的文本段的矩形区域 */
static int TextLayer_GetRowRect( LCUI_TextLayer layer, int i_row,
				 int start_col, int end_col, LCUI_Rect *rect )
{
	int i;
	LCUI_TextRow txtrow;
	if( i_row >= layer->text_rows.length ) {
		return -1;
	}
	/* 先计算在有效区域内的起始行的Y轴坐标 */
	rect->y = layer->offset_y;
	rect->x = layer->offset_x;
	for( i = 0; i < i_row; ++i ) {
		rect->y += layer->text_rows.rows[i]->height;
	}
	txtrow = layer->text_rows.rows[i_row];
	if( end_col < 0 || end_col >= txtrow->length ) {
		end_col = txtrow->length - 1;
	}
	rect->height = txtrow->height;
	rect->x += TextLayer_GetRowStartX( layer, txtrow );
	if( start_col == 0 && end_col == txtrow->length - 1 ) {
		rect->width = txtrow->width;
	} else {
		for( i = 0; i < start_col; ++i ) {
			if( !txtrow->string[i]->bitmap ) {
				continue;
			}
			rect->x += txtrow->string[i]->bitmap->advance.x;
		}
		rect->width = 0;
		for( i = start_col; i <= end_col &&
		     i < txtrow->length; ++i ) {
			if( !txtrow->string[i]->bitmap ) {
				continue;
			}
			rect->width += txtrow->string[i]->bitmap->advance.x;
		}
	}
	if( rect->width <= 0 || rect->height <= 0 ) {
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
		RectList_Add( &layer->dirty_rects, &rect );
	}
}

/** 标记指定范围内容的文本行的矩形为无效 */
void TextLayer_InvalidateRowsRect( LCUI_TextLayer layer,
				   int start_row, int end_row )
{
	int i, y;
	LCUI_Rect rect;

	if( end_row < 0 || end_row >= layer->text_rows.length ) {
		end_row = layer->text_rows.length - 1;
	}

	y = layer->offset_y;
	for( i = 0; i < layer->text_rows.length; ++i ) {
		y += layer->text_rows.rows[i]->height;
		if( i >= start_row && y >= 0 ) {
			y -= layer->text_rows.rows[i]->height;
			break;
		}
	}
	for( ; i <= end_row; ++i ) {
		TextLayer_GetRowRect( layer, i, 0, -1, &rect );
		RectList_Add( &layer->dirty_rects, &rect );
		y += layer->text_rows.rows[i]->height;
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
	else if( row >= layer->text_rows.length ) {
		if( layer->text_rows.length < 0 ) {
			row = 0;
		} else	{
			row = layer->text_rows.length-1;
		}
	}
	if( col < 0 ) {
		col = 0;
	}
	else if( layer->text_rows.length > 0 ) {
		if( col >= layer->text_rows.rows[row]->length ) {
			col = layer->text_rows.rows[row]->length;
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
	LCUI_TextRow txtrow;
	int i, pixel_pos, ins_x, ins_y;
	pixel_pos = layer->offset_y;
	for( i = 0; i < layer->text_rows.length; ++i ) {
		pixel_pos += layer->text_rows.rows[i]->height;
		if( pixel_pos >= y ) {
			ins_y = i;
			break;
		}
	}
	if( i >= layer->text_rows.length ) {
		if( layer->text_rows.length > 0 ) {
			ins_y = layer->text_rows.length - 1;
		} else {
			layer->insert_x = 0;
			layer->insert_y = 0;
			return -1;
		}
	}
	txtrow = layer->text_rows.rows[ins_y];
	ins_x = txtrow->length;
	pixel_pos = layer->offset_x;
	pixel_pos += TextLayer_GetRowStartX( layer, txtrow );
	for( i = 0; i < txtrow->length; ++i ) {
		LCUI_TextChar txtchar;
		txtchar = txtrow->string[i];
		if( !txtchar->bitmap ) {
			continue;
		}
		pixel_pos += txtchar->bitmap->advance.x;
		/* 如果在当前字中心点的前面 */
		if( x <= pixel_pos - txtchar->bitmap->advance.x / 2 ) {
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
	LCUI_TextRow txtrow;
	int i, pixel_x = 0, pixel_y = 0;
	if( row < 0 || row >= layer->text_rows.length ) {
		return -1;
	}
	if( col < 0 ) {
		return -2;
	} else if( col > layer->text_rows.rows[row]->length ) {
		return -3;
	}
	/* 累加前几行的高度 */
	for( i = 0; i < row; ++i ) {
		pixel_y += layer->text_rows.rows[i]->height;
	}
	txtrow = layer->text_rows.rows[row];
	pixel_x = TextLayer_GetRowStartX( layer, txtrow );
	for( i = 0; i < col; ++i ) {
		LCUI_TextChar txtchar = txtrow->string[i];
		if( !txtchar || !txtchar->bitmap ) {
			continue;
		}
		pixel_x += txtchar->bitmap->advance.x;
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
	TextLayer_InvalidateRowsRect( layer, 0, -1 );
	layer->width = 0;
	layer->length = 0;
	layer->insert_x = 0;
	layer->insert_y = 0;
	TextLayer_DestroyStyleCache( layer );
	TextRowList_Destroy( &layer->text_rows );
	TextRowList_InsertNewRow( &layer->text_rows, 0 );
	layer->task.redraw_all = TRUE;
}

/** 对文本行进行断行 */
static void TextLayer_BreakTextRow( LCUI_TextLayer layer, int row,
				    int col, LCUI_EOLChar eol )
{
	int n;
	LCUI_TextRow txtrow, next;
	txtrow = TextLayer_GetRow( layer, row );
	next = TextRowList_InsertNewRow( &layer->text_rows, row + 1 );
	/* 将本行原有的行尾符转移至下一行 */
	next->eol = txtrow->eol;
	txtrow->eol = eol;
	for( n = txtrow->length - 1; n >= col; --n ) {
		TextRow_Insert( next, 0, txtrow->string[n] );
		txtrow->string[n] = NULL;
	}
	txtrow->length = col;
	TextLayer_UpdateRowSize( layer, txtrow );
	TextLayer_UpdateRowSize( layer, next );
}

/** 将指定行与下一行合并 */
static void TextLayer_MergeRow( LCUI_TextLayer layer, int row )
{
	int i, j;
	LCUI_TextRow txtrow = TextLayer_GetRow( layer, row );
	LCUI_TextRow next = TextLayer_GetRow( layer, row + 1 );

	if( !txtrow || !next ) {
		return;
	}
	if( layer->insert_y > row ) {
		--layer->insert_y;
		if( layer->insert_y == row ) {
			layer->insert_x += txtrow->length;
		}
	}
	i = txtrow->length;
	TextRow_SetLength( txtrow, txtrow->length + next->length );
	for( j = 0; j < next->length; ++i, ++j ) {
		txtrow->string[i] = next->string[j];
		next->string[j] = NULL;
	}
	txtrow->eol = next->eol;
	TextLayer_UpdateRowSize( layer, txtrow );
	TextRowList_RemoveRow( &layer->text_rows, row + 1 );
}

/** 对指定行的文本进行排版 */
static void TextLayer_TextRowTypeset( LCUI_TextLayer layer, int row )
{
	LCUI_TextRow txtrow;
	LCUI_TextChar txtchar;
	LCUI_BOOL not_autowrap;
	int col, max_width, row_width = 0, word_col = 0;

	if( layer->fixed_width > 0 ) {
		max_width = layer->fixed_width;
	} else {
		max_width = layer->max_width;
	}
	if( max_width <= 0 || !layer->enable_autowrap ||
	    (layer->enable_autowrap && !layer->enable_mulitiline) ) {
		not_autowrap = TRUE;
	} else {
		not_autowrap = FALSE;
	}
	txtrow = layer->text_rows.rows[row];
	for( col = 0; col < txtrow->length; ++col ) {
		txtchar = txtrow->string[col];
		if( !txtchar->bitmap ) {
			continue;
		}
		if( !isalpha( txtchar->code ) ) {
			word_col = col + 1;
		}
		/* 累加行宽度 */
		row_width += txtchar->bitmap->advance.x;
		/* 如果是当前行的第一个字符，或者行宽度没有超过宽度限制 */
		if( not_autowrap || col < 1 || row_width <= max_width ) {
			continue;
		}
		if( layer->word_break == LCUI_WORD_BREAK_NORMAL ) {
			if( word_col < 1 ) {
				continue;
			}
			col = word_col;
		}
		TextLayer_BreakTextRow( layer, row, col, LCUI_EOL_NONE );
		return;
	}
	TextLayer_UpdateRowSize( layer, txtrow );
	/* 如果本行有换行符，或者是最后一行 */
	if( txtrow->eol != LCUI_EOL_NONE ||
	    row == layer->text_rows.length - 1 ) {
		return;
	}
	row_width = txtrow->width;
	/* 本行的文本宽度未达到限制宽度，需要将下行的文本转移至本行 */
	if( txtrow->eol == LCUI_EOL_NONE ) {
		TextLayer_InvalidateRowRect( layer, row, 0, -1 );
		TextLayer_InvalidateRowRect( layer, row + 1, 0, -1 );
		TextLayer_MergeRow( layer, row );
		TextLayer_TextRowTypeset( layer, row );
	}
}

/** 从指定行开始，对文本进行排版 */
static void TextLayer_TextTypeset( LCUI_TextLayer layer, int start_row )
{
	int row;
	/* 记录排版前各个文本行的矩形区域 */
	TextLayer_InvalidateRowsRect( layer, start_row, -1 );
	for( row = start_row; row < layer->text_rows.length; ++row ) {
		TextLayer_TextRowTypeset( layer, row );
	}
	/* 记录排版后各个文本行的矩形区域 */
	TextLayer_InvalidateRowsRect( layer, start_row, -1 );
}

static const wchar_t *TextLayer_ProcessStyleTag( LCUI_TextLayer layer,
						 const wchar_t *p,
						 LinkedList *tags,
						 LCUI_TextStyle *style )
{
	LCUI_TextStyle s;
	const wchar_t *pp;
	pp = StyleTags_GetEnd( tags, p );
	if( pp ) {
		s = StyleTags_GetTextStyle( tags );
		if( s ) {
			TextStyle_Merge( s, &layer->text_default_style );
			LinkedList_Append( &layer->text_styles, s );
		}
		*style = s;
		return pp;
	}
	pp = StyleTags_GetStart( tags, p );
	if( pp ) {
		s = StyleTags_GetTextStyle( tags );
		if( s ) {
			TextStyle_Merge( s, &layer->text_default_style );
			LinkedList_Append( &layer->text_styles, s );
		}
		*style = s;
		return pp;
	}
	return NULL;
}

/** 对文本进行预处理 */
static int TextLayer_ProcessText( LCUI_TextLayer layer, const wchar_t *wstr,
				  int add_type, LinkedList *tags )
{
	LCUI_EOLChar eol;
	LCUI_TextRow txtrow;
	LCUI_TextCharRec txtchar;
	LinkedList tmp_tags;
	const wchar_t *p;
	int cur_col, cur_row, start_row, ins_x, ins_y;
	LCUI_BOOL need_typeset, rect_has_added;
	LCUI_TextStyle style = NULL;

	if( !wstr ) {
		return -1;
	}
	need_typeset = FALSE;
	rect_has_added = FALSE;
	StyleTags_Init( &tmp_tags );
	if( !tags ) {
		tags = &tmp_tags;
	}
	/* 如果是将文本追加至文本末尾 */
	if( add_type == TAT_APPEND ) {
		if( layer->text_rows.length > 0 ) {
			cur_row = layer->text_rows.length - 1;
		} else {
			cur_row = 0;
		}
		txtrow = TextLayer_GetRow( layer, cur_row );
		if( !txtrow ) {
			txtrow = TextRowList_AddNewRow( &layer->text_rows );
		}
		cur_col = txtrow->length;
	} else { /* 否则，是将文本插入至当前插入点 */
		cur_row = layer->insert_y;
		cur_col = layer->insert_x;
		txtrow = TextLayer_GetRow( layer, cur_row );
		if( !txtrow ) {
			txtrow = TextRowList_AddNewRow( &layer->text_rows );
		}
	}
	start_row = cur_row;
	ins_x = cur_col;
	ins_y = cur_row;
	for( p = wstr; *p; ++p ) {
		if( layer->enable_style_tags ) {
			const wchar_t *pp;
			pp = TextLayer_ProcessStyleTag( layer, p,
							tags, &style );
			if( pp ) {
				p = pp - 1;
				continue;
			}
		}
		if( *p == '\r' || *p == '\n' ) {
			/* 判断是哪一种换行模式 */
			if( *p == '\r' ) {
				if( *(p + 1) == '\n' ) {
					eol = LCUI_EOL_CR_LF;
				} else {
					eol = LCUI_EOL_CR;
				}
			} else {
				eol = LCUI_EOL_LF;
			}
			/* 如果没有记录过文本行的矩形区域 */
			if( !rect_has_added ) {
				TextLayer_InvalidateRowsRect( layer,
							      ins_y, -1 );
				rect_has_added = TRUE;
				start_row = ins_y;
			}
			/* 将当前行中的插入点为截点，进行断行 */
			TextLayer_BreakTextRow( layer, ins_y, ins_x, eol );
			layer->width = max( layer->width, txtrow->width );
			need_typeset = TRUE;
			++layer->length;
			ins_x = 0;
			++ins_y;
			txtrow = TextLayer_GetRow( layer, ins_y );
			continue;
		}
		txtchar.style = style;
		txtchar.code = *p;
		TextChar_UpdateBitmap( &txtchar, &layer->text_default_style );
		TextRow_InsertCopy( txtrow, ins_x, &txtchar );
		++layer->length;
		++ins_x;
	}
	/* 更新当前行的尺寸 */
	TextLayer_UpdateRowSize( layer, txtrow );
	layer->width = max( layer->width, txtrow->width );
	if( add_type == TAT_INSERT ) {
		layer->insert_x = ins_x;
		layer->insert_y = ins_y;
	}
	/* 若启用了自动换行模式，则标记需要重新对文本进行排版 */
	if( layer->enable_autowrap || need_typeset ) {
		TextLayer_AddUpdateTypeset( layer, cur_row );
	} else {
		TextLayer_InvalidateRowRect( layer, cur_row, 0, -1 );
	}
	/* 如果已经记录过文本行矩形区域 */
	if( rect_has_added ) {
		TextLayer_InvalidateRowsRect( layer, start_row, -1 );
		rect_has_added = TRUE;
	}
	StyleTags_Clear( &tmp_tags );
	return 0;
}

/** 插入文本内容（宽字符版） */
int TextLayer_InsertTextW( LCUI_TextLayer layer, const wchar_t *wstr,
			   LinkedList *tag_stack )
{
	return TextLayer_ProcessText( layer, wstr, TAT_INSERT,
				      tag_stack );
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
	return TextLayer_ProcessText( layer, wstr, TAT_APPEND, 
				      tag_stack );
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
	LCUI_TextRow row_ptr;

	if( start_pos < 0 ) {
		return -1;
	}
	if( max_len <= 0 ) {
		return 0;
	}
	/* 先根据一维坐标计算行列坐标 */
	for( i = 0, row = 0; row < layer->text_rows.length; ++row ) {
		if( i >= start_pos ) {
			col = start_pos - i;
			break;
		}
		i += layer->text_rows.rows[row]->length;
	}
	for( i = 0; row < layer->text_rows.length && i < max_len; ++row ) {
		row_ptr = layer->text_rows.rows[row];
		for( ; col < row_ptr->length && i < max_len; ++col, ++i ) {
			wstr_buff[i] = row_ptr->string[col]->code;
		}
	}
	wstr_buff[i] = L'\0';
	return i;
}

LCUI_Graph* TextLayer_GetGraphBuffer( LCUI_TextLayer layer )
{
	if( layer->enable_canvas ) {
		return &layer->canvas;
	}
	return NULL;
}

int TextLayer_GetWidth( LCUI_TextLayer layer )
{
	int i, row, w, max_w;
	LCUI_TextRow txtrow;

	DEBUG_MSG( "rows: %d, font-size: %d\n",
		   layer->text_rows.length,
		   layer->text_default_style.pixel_size );
	for( row = 0, max_w = 0; row < layer->text_rows.length; ++row ) {
		txtrow = layer->text_rows.rows[row];
		for( i = 0, w = 0; i < txtrow->length; ++i ) {
			if( !txtrow->string[i]->bitmap ||
			    !txtrow->string[i]->bitmap->buffer ) {
				continue;
			}
			w += txtrow->string[i]->bitmap->advance.x;
			DEBUG_MSG("[%d/%d] %d %c, width: %d/%d\n", 
				    i, txtrow->length,
				    txtrow->string[i]->code,
				    txtrow->string[i]->code,
				    txtrow->string[i]->bitmap->advance.x,
				    w);
		}
		if( w > max_w ) {
			max_w = w;
		}
	}
	return max_w;
}

int TextLayer_GetHeight( LCUI_TextLayer layer )
{
	int i, h;
	for( i = 0, h = 0; i < layer->text_rows.length; ++i ) {
		h += layer->text_rows.rows[i]->height;
	}
	return h;
}

int TextLayer_SetFixedSize( LCUI_TextLayer layer, int width, int height )
{
	layer->fixed_width = width;
	layer->fixed_height = height;
	if( layer->enable_canvas ) {
		Graph_Create( &layer->canvas, width, height );
	}
	layer->task.redraw_all = TRUE;
	if( layer->enable_autowrap ) {
		layer->task.typeset_start_row = 0;
		layer->task.update_typeset = TRUE;
	}
	return 0;
}

int TextLayer_SetMaxSize( LCUI_TextLayer layer, int width, int height )
{
	layer->max_width = width;
	layer->max_height = height;
	if( layer->enable_canvas ) {
		Graph_Create( &layer->canvas, width, height );
	}
	layer->task.redraw_all = TRUE;
	if( layer->enable_autowrap ) {
		layer->task.typeset_start_row = 0;
		layer->task.update_typeset = TRUE;
	}
	return 0;
}

/** 设置是否启用多行文本模式 */
void TextLayer_SetMultiline( LCUI_TextLayer layer, int is_true )
{
	if( (layer->enable_mulitiline && !is_true)
	 || (!layer->enable_mulitiline && is_true) ) {
		layer->enable_mulitiline = is_true;
		TextLayer_AddUpdateTypeset( layer, 0 );
	}
}

/** 删除指定行列的文字及其右边的文本 */
static int TextLayer_TextDeleteEx( LCUI_TextLayer layer, int char_y,
				   int char_x, int n_char )
{
	int end_x, end_y, i, j, len;
	LCUI_TextRow txtrow, end_txtrow, prev_txtrow;

	if( char_x < 0 ) {
		char_x = 0;
	}
	if( char_y < 0 ) {
		char_y = 0;
	}
	if( n_char <= 0 ) {
		return -1;
	}
	if( char_y >= layer->text_rows.length ) {
		return -2;
	}
	txtrow = layer->text_rows.rows[char_y];
	if( char_x > txtrow->length ) {
		char_x = txtrow->length;
	}
	i = n_char;
	end_x = char_x;
	end_y = char_y;
	/* 计算结束点的位置 */
	for( ; end_y < layer->text_rows.length && n_char > 0; ++end_y ) {
		txtrow = layer->text_rows.rows[end_y];
		if( end_x + n_char <= txtrow->length ) {
			end_x += n_char;
			n_char = 0;
			break;
		}
		n_char -= (txtrow->length - end_x);
		if( txtrow->eol == LCUI_EOL_NONE ) {
			end_x = 0;
		} else {
			n_char -= 1;
			end_x = 0;
		}
	}
	if( n_char >= 0 ) {
		layer->length -= i - n_char;
	} else {
		layer->length -= n_char;
	}
	if( end_y >= layer->text_rows.length ) {
		end_y = layer->text_rows.length - 1;
		end_txtrow = layer->text_rows.rows[end_y];
		end_x = end_txtrow->length;
	} else {
		end_txtrow = layer->text_rows.rows[end_y];
	}
	if( end_x > end_txtrow->length ) {
		end_x = end_txtrow->length;
	}
	if( end_x == char_x && end_y == char_y ) {
		return 0;
	}
	/* 获取上一行文本 */
	prev_txtrow = layer->text_rows.rows[char_y - 1];
	// 计算起始行与结束行拼接后的长度
	// 起始行：0 1 2 3 4 5，起点位置：2
	// 结束行：0 1 2 3 4 5，终点位置：4
	// 拼接后的长度：2 + 6 - 4 = 4
	len = char_x + end_txtrow->length - end_x;
	if( len < 0 ) {
		return -3;
	}
	/* 如果是同一行 */
	if( txtrow == end_txtrow ) {
		if( end_x > end_txtrow->length ) {
			return -4;
		}
		TextLayer_InvalidateRowRect( layer, char_y, char_x, -1 );
		TextLayer_AddUpdateTypeset( layer, char_y );
		for( i = char_x, j = end_x; j < txtrow->length; ++i, ++j ) {
			txtrow->string[i] = txtrow->string[j];
		}
		/* 如果当前行为空，也不是第一行，并且上一行没有结束符 */
		if( len <= 0 && end_y > 0 &&
		    prev_txtrow->eol != LCUI_EOL_NONE ) {
			TextRowList_RemoveRow( &layer->text_rows, end_y );
		}
		/* 调整起始行的容量 */
		TextRow_SetLength( txtrow, len );
		/* 更新文本行的尺寸 */
		TextLayer_UpdateRowSize( layer, txtrow );
		return 0;
	}
	/* 如果结束点在行尾，并且该行不是最后一行 */
	if( end_x == end_txtrow->length &&
	    end_y < layer->text_rows.length - 1 ) {
		++end_y;
		end_txtrow = TextLayer_GetRow( layer, end_y );
		end_x = -1;
		len = char_x + end_txtrow->length;
	}
	TextRow_SetLength( txtrow, len );
	/* 标记当前行后面的所有行的矩形需区域需要刷新 */
	TextLayer_InvalidateRowsRect( layer, char_y + 1, -1 );
	/* 移除起始行与结束行之间的文本行 */
	for( i = char_y + 1, j = i; j < end_y; ++j ) {
		TextLayer_InvalidateRowRect( layer, i, 0, -1 );
		TextRowList_RemoveRow( &layer->text_rows, i );
	}
	i = char_x;
	j = end_x + 1;
	end_y = char_y + 1;
	/* 将结束行的内容拼接至起始行 */
	for( ; i < len && j < end_txtrow->length; ++i, ++j ) {
		txtrow->string[i] = end_txtrow->string[j];
	}
	TextLayer_UpdateRowSize( layer, txtrow );
	TextLayer_InvalidateRowRect( layer, end_y, 0, -1 );
	/* 移除结束行 */
	TextRowList_RemoveRow( &layer->text_rows, end_y );
	/* 如果起始行无内容，并且上一行没有结束符（换行符），则
	 * 说明需要删除起始行 */
	if( len <= 0 && char_y > 0 &&
	    prev_txtrow->eol != LCUI_EOL_NONE ) {
		TextLayer_InvalidateRowRect( layer, char_y, 0, -1 );
		TextRowList_RemoveRow( &layer->text_rows, char_y );
	}
	TextLayer_AddUpdateTypeset( layer, char_y );
	return 0;
}

/** 删除文本光标的当前坐标右边的文本 */
int TextLayer_TextDelete( LCUI_TextLayer layer, int n_char )
{
	return TextLayer_TextDeleteEx(	layer, layer->insert_y,
					layer->insert_x, n_char );
}

/** 退格删除文本，即删除文本光标的当前坐标左边的文本 */
int TextLayer_TextBackspace( LCUI_TextLayer layer, int n_char )
{
	int n_del;
	int char_x, char_y;
	LCUI_TextRow txtrow;

	/* 先获取当前字的位置 */
	char_x = layer->insert_x;
	char_y = layer->insert_y;
	/* 再计算删除 n_char 个字后的位置 */
	for( n_del = n_char; char_y >= 0; --char_y ) {
		txtrow = layer->text_rows.rows[char_y];
		/* 如果不是当前行，则重定位至行尾 */
		if( char_y < layer->insert_y ) {
			char_x = txtrow->length;
			if( txtrow->eol == LCUI_EOL_NONE ) {
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
	if( char_y < 0 || n_del == n_char ) {
		return -1;
	}
	/* 若能够被删除的字不够 n_char 个，则调整需删除的字数 */
	if( n_del > 0 ) {
		n_char -= n_del;
	}
	/* 开始删除文本 */
	TextLayer_TextDeleteEx( layer, char_y, char_x, n_char );
	/* 若最后一行被完全移除，则移动输入点至上一行的行尾处 */
	if( char_x == 0 && layer->text_rows.length > 0
	    && char_y >= layer->text_rows.length ) {
		char_y = layer->text_rows.length - 1;
		char_x = layer->text_rows.rows[char_y]->length;
	}
	/* 更新文本光标的位置 */
	TextLayer_SetCaretPos( layer, char_y, char_x );
	return 0;
}

void TextLayer_SetAutoWrap( LCUI_TextLayer layer, LCUI_BOOL autowrap )
{
	if( layer->enable_autowrap != autowrap ) {
		layer->enable_autowrap = autowrap;
		TextLayer_AddUpdateTypeset( layer, 0 );
	}
}

void TextLayer_SetWordBreak( LCUI_TextLayer layer, LCUI_WordBreakMode mode )
{
	if( layer->word_break != mode ) {
		layer->word_break = mode;
		TextLayer_AddUpdateTypeset( layer, 0 );
	}
}

/** 设置是否使用样式标签 */
void TextLayer_SetUsingStyleTags( LCUI_TextLayer layer, LCUI_BOOL enable )
{
	layer->enable_style_tags = enable;
}

static void TextLayer_UpdateTextStyleCache( LCUI_TextLayer layer )
{
	LinkedListNode *node;
	if( !layer->text_default_style.has_family ) {
		TextStyle_SetDefaultFont( &layer->text_default_style );
	}
	/* 替换缺省字体，确保能够正确应用字体设置 */
	for( LinkedList_Each( node, &layer->text_styles ) ) {
		TextStyle_Merge( node->data, &layer->text_default_style );
	}
}

/** 重新载入各个文字的字体位图 */
void TextLayer_ReloadCharBitmap( LCUI_TextLayer layer )
{
	int row, col;
	TextLayer_UpdateTextStyleCache( layer );
	for( row = 0; row < layer->text_rows.length; ++row ) {
		LCUI_TextRow txtrow = layer->text_rows.rows[row];
		for( col = 0; col < txtrow->length; ++col ) {
			LCUI_TextChar txtchar = txtrow->string[col];
			TextChar_UpdateBitmap( txtchar, &layer->text_default_style );
		}
		TextLayer_UpdateRowSize( layer, txtrow );
	}
}

void TextLayer_Update( LCUI_TextLayer layer, LinkedList *rects )
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
	layer->width = TextLayer_GetWidth( layer );
	/* 如果坐标偏移量有变化，记录各个文本行区域 */
	if( layer->new_offset_x != layer->offset_x
	 || layer->new_offset_y != layer->offset_y ) {
		TextLayer_InvalidateRowsRect( layer, 0, -1 );
		layer->offset_x = layer->new_offset_x;
		layer->offset_y = layer->new_offset_y;
		TextLayer_InvalidateRowsRect( layer, 0, -1 );
		layer->task.redraw_all = TRUE;
	}
	if( rects ) {
		LinkedList_Concat( rects, &layer->dirty_rects );
	 }
}

static void TextLayer_ValidateArea( LCUI_TextLayer layer, LCUI_Rect *area )
{
	int width, height;
	if( layer->fixed_width > 0 ) {
		width = layer->fixed_width;
	} else if( layer->max_width > 0 ) {
		width = layer->max_width;
	} else {
		width = layer->width;
	}
	if( layer->fixed_height > 0 ) {
		height = layer->fixed_height;
	} else {
		height = TextLayer_GetHeight( layer );
	}
	LCUIRect_ValidateArea( area, width, height );
}

static void TextLayer_DrawChar( LCUI_TextLayer layer, LCUI_TextChar ch,
				LCUI_Graph *graph, LCUI_Pos ch_pos )
{
	/* 判断文字使用的前景颜色，再进行绘制 */
	if( ch->style && ch->style->has_fore_color ) {
		FontBitmap_Mix( graph, ch_pos, ch->bitmap,
				ch->style->fore_color );
	} else {
		FontBitmap_Mix( graph, ch_pos, ch->bitmap,
				layer->text_default_style.fore_color );
	}
}

static void TextLayer_DrawTextRow( LCUI_TextLayer layer, LCUI_Rect *area,
				   LCUI_Graph *graph, LCUI_Pos layer_pos,
				   LCUI_TextRow txtrow, int y )
{
	LCUI_TextChar txtchar;
	LCUI_Pos ch_pos;
	int baseline, col, x;
	baseline = txtrow->text_height * 4 / 5;
	x = TextLayer_GetRowStartX( layer, txtrow ) + layer->offset_x;
	/* 确定从哪个文字开始绘制 */
	for( col = 0; col < txtrow->length; ++col ) {
		txtchar = txtrow->string[col];
		/* 忽略无字体位图的文字 */
		if( !txtchar->bitmap ) {
			continue;
		}
		x += txtchar->bitmap->advance.x;
		if( x > area->x ) {
			x -= txtchar->bitmap->advance.x;
			break;
		}
	}
	/* 若一整行的文本都不在可绘制区域内 */
	if( col >= txtrow->length ) {
		y += txtrow->height;
		return;
	}
	/* 遍历该行的文字 */
	for( ; col < txtrow->length; ++col ) {
		txtchar = txtrow->string[col];
		if( !txtchar->bitmap ) {
			continue;
		}
		/* 计算字体位图的绘制坐标 */
		ch_pos.x = layer_pos.x + x;
		ch_pos.y = layer_pos.y + y;
		if( txtchar->style &&  txtchar->style->has_back_color ) {
			LCUI_Rect rect;
			rect.x = ch_pos.x;
			rect.y = ch_pos.y;
			rect.height = txtrow->height;
			rect.width = txtchar->bitmap->advance.x;
			Graph_FillRect( graph, txtchar->style->back_color,
					&rect, TRUE );
		}
		ch_pos.x += txtchar->bitmap->left;
		ch_pos.y += baseline;
		ch_pos.y += (txtrow->height - baseline) / 2;
		ch_pos.y -= txtchar->bitmap->top;
		TextLayer_DrawChar( layer, txtchar, graph, ch_pos );
		x += txtchar->bitmap->advance.x;
		/* 如果超过绘制区域则不继续绘制该行文本 */
		if( x > area->x + area->width ) {
			break;
		}
	}
}

int TextLayer_RenderTo( LCUI_TextLayer layer, LCUI_Rect area,
			   LCUI_Pos layer_pos, LCUI_Graph *canvas )
{
	int y, row;
	LCUI_TextRow txtrow;

	y = layer->offset_y;
	/* 确定可绘制的最大区域范围 */
	TextLayer_ValidateArea( layer, &area );
	for( row = 0; row < layer->text_rows.length; ++row ) {
		txtrow = TextLayer_GetRow( layer, row );
		y += txtrow->height;
		if( y > area.y ) {
			y -= txtrow->height;
			break;
		}
	}
	/* 如果没有可绘制的文本行 */
	if( row >= layer->text_rows.length ) {
		return -1;
	}
	for( ; row < layer->text_rows.length; ++row ) {
		txtrow = TextLayer_GetRow( layer, row );
		TextLayer_DrawTextRow( layer, &area, canvas,
				       layer_pos, txtrow, y );
		y += txtrow->height;
		/* 超出绘制区域范围就不绘制了 */
		if( y > area.y + area.height ) {
			break;
		}
	}
	return 0;
}

/** 绘制文本 */
int TextLayer_RenderAll( LCUI_TextLayer layer )
{
	LCUI_Rect rect;
	LCUI_Pos pos = {0,0};

	/* 如果文本位图缓存无效 */
	if( layer->enable_canvas && !Graph_IsValid( &layer->canvas ) ) {
		return -1;
	}
	rect.x = 0;
	rect.y = 0;
	rect.width = layer->max_width;
	rect.height = layer->max_height;
	return TextLayer_RenderTo( layer, rect, pos, &layer->canvas );
}

/** 清除已记录的无效矩形 */
void TextLayer_ClearInvalidRect( LCUI_TextLayer layer )
{
	LinkedListNode *node;
	LCUI_Graph invalid_graph;

	if( !layer->enable_canvas ) {
		RectList_Clear( &layer->dirty_rects );
		return;
	}
	for( LinkedList_Each( node, &layer->dirty_rects ) ) {
		Graph_Quote( &invalid_graph, &layer->canvas, node->data );
		Graph_FillAlpha( &invalid_graph, 0 );
	}
	RectList_Clear( &layer->dirty_rects );
}

/** 设置全局文本样式 */
void TextLayer_SetTextStyle( LCUI_TextLayer layer, LCUI_TextStyle style )
{
	TextStyle_Destroy( &layer->text_default_style );
	TextStyle_Copy( &layer->text_default_style, style );
	layer->task.update_bitmap = TRUE;
}

/** 设置文本对齐方式 */
void TextLayer_SetTextAlign( LCUI_TextLayer layer, int align )
{
	layer->text_align = align;
	layer->task.update_typeset = TRUE;
	layer->task.typeset_start_row = 0;
}

/** 设置文本行的高度 */
void TextLayer_SetLineHeight( LCUI_TextLayer layer, int height )
{
	layer->line_height = height;
	layer->task.update_typeset = TRUE;
	layer->task.typeset_start_row = 0;
}

void TextLayer_SetOffset( LCUI_TextLayer layer, int offset_x, int offset_y )
{
	layer->new_offset_x = offset_x;
	layer->new_offset_y = offset_y;
}
