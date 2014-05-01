/* ***************************************************************************
 * textbox.c -- LCUI's TextBox widget
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
 * textbox.c -- LCUI 的文本框部件
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

//#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_GRAPH_H
#include LC_FONT_H
#include LC_TEXTBOX_H
#include LC_SCROLLBAR_H
#include LC_INPUT_H
#include LC_INPUT_METHOD_H
#include LC_ERROR_H 

#define MALLOC_ONE(type) (type*)malloc(sizeof(type))
#define REALLOC(obj,size,type) (type*)realloc(obj,size*sizeof(type))

#define CARET_HIDE 0
#define CARET_SHOW 1

#define WIDGET_MSG_TEXT_BLOCK		(WIDGET_USER+1)
#define WIDGET_MSG_CLEAR_TEXT		(WIDGET_USER+2)
#define WIDGET_MSG_TEXT_DELETE		(WIDGET_USER+3)
#define WIDGET_MSG_TEXT_BACKSPACE	(WIDGET_USER+4)

#define TEXTBOX_DEFAULT_TEXT_BLOCK_SIZE	512

typedef struct LCUI_CaretRec_ {
	int type;
	int w, h;
	int blink_time;
	int timer_id;
	int state;
	LCUI_BOOL need_show;
	LCUI_Widget *widget;
} LCUI_Caret;

typedef struct LCUI_TextBox_ {
	LCUI_TextLayer text;			/**< 实际文本 */
	LCUI_TextLayer mask_text;		/**< 屏蔽后的文本 */
	LCUI_Widget *scrollbar[2];		/**< 两个滚动条 */
	LCUI_Caret caret;			/**< 文本插入符 */
	LCUI_BOOL read_only;			/**< 是否只读 */
	int block_size;				/**< 块大小 */
	LCUI_Queue text_block_buffer;		/**< 文本块缓冲区 */
	LCUI_BOOL show_placeholder;		/**< 表示占位符是否已经显示 */
	LCUI_WString placeholder;		/**< 文本框的占位符 */
	LCUI_WString allow_input_char;		/**< 允许输入的字符 */
	wchar_t password_char;			/**< 屏蔽符的副本 */
	LCUI_TextStyle placeholder_style;	/**< 占位符的文本样式 */
	LCUI_TextStyle textstyle_bak;		/**< 文本框内文本样式的副本 */
} LCUI_TextBox;

typedef enum TextBlockType_ {
	TEXT_BLOCK_TYPE_TEXT_BEGIN,
	TEXT_BLOCK_TYPE_TEXT_BODY,
	TEXT_BLOCK_TYPE_TEXT_END
} TextBlockType;

/** 文本块数据结构 */
typedef struct LCUI_TextBlock_ {
	TextBlockType type;			/**< 文本块类型 */
	TextAddType add_type;			/**< 指定该文本块的添加方式 */
	LCUI_StyleTagStack *style;		/**< 整个文本所应用的样式栈 */
	wchar_t *text;				/**< 文本块(段) */
} LCUI_TextBlock;

/*---------------------------------- Caret ----------------------------------*/
/** 切换成现实状态 */
static void Caret_BlinkShow( LCUI_Caret *caret )
{
	if( !caret->need_show ) {
		return;
	}
	caret->state = CARET_SHOW;
	Widget_Show( caret->widget );
	LCUITimer_Reset( caret->timer_id, caret->blink_time );
}

/** 切换成现实隐藏 */
static void Caret_BlinkHide( LCUI_Caret *caret )
{
	caret->state = CARET_HIDE;
	Widget_Hide( caret->widget );
	LCUITimer_Reset( caret->timer_id, caret->blink_time );
}

static void Caret_Blink( void *arg )
{
	LCUI_Caret *caret = (LCUI_Caret*)arg;
	if( caret->need_show ) {
		if( caret->state == CARET_HIDE ) {
			caret->state = CARET_SHOW;
			Widget_Show( caret->widget );
		} else {
			caret->state = CARET_HIDE;
			Widget_Hide( caret->widget );
		}
	}
}

static void Caret_Init( LCUI_Caret *caret )
{
	caret->blink_time = 500;
	caret->need_show = FALSE;
	caret->state = CARET_HIDE;
	caret->type = 0;
	caret->w = 1;
	caret->h = 14;
	caret->timer_id = LCUITimer_Set( 
		caret->blink_time, Caret_Blink, (void*)caret, TRUE 
	);
	caret->widget = Widget_New(NULL);
	/* 设置不可被鼠标点击 */
	Widget_SetClickable( caret->widget, FALSE );
	Widget_SetBackgroundColor( caret->widget, RGB(0,0,0) );
	Widget_SetBackgroundTransparent( caret->widget, FALSE );
	Widget_Resize( caret->widget, Size(caret->w,caret->h) );
}

/** 设置插入符是否可见 */
static void Caret_SetVisible( LCUI_Caret *caret, LCUI_BOOL visible )
{
	caret->need_show = visible;
	if( visible ) {
		caret->state = CARET_SHOW;
		Widget_Show( caret->widget );
	} else {
		caret->state = CARET_HIDE;
		Widget_Hide( caret->widget );
	}
	LCUITimer_Reset( caret->timer_id, caret->blink_time );
}

/** 设置闪烁的时间间隔 */
static inline void Caret_SetBlinkTime( LCUI_Caret *caret, unsigned int n_ms )
{
	caret->blink_time = n_ms;
	LCUITimer_Reset( caret->timer_id, caret->blink_time );
}

/** 设置插入符的容器 */
static inline void Caret_AddTo( LCUI_Caret *caret, LCUI_Widget *container )
{
	Widget_Container_Add( container, caret->widget );
}

/** 移动插入符 */
static void Caret_Move( LCUI_Caret *caret, int x, int y )
{
	Widget_Move( caret->widget, Pos(x,y) );
}

/** 设置插入符的宽度 */
static void Caret_SetSize( LCUI_Caret *caret, int w, int h )
{
	if( w <= 0 ) {
		w = caret->w;
	}
	if( h <= 0 ) {
		h = caret->h;
	}
	Widget_Resize( caret->widget, Size(w,h) );
}

/** 销毁插入符 */
static void Caret_Destroy( LCUI_Caret *caret )
{
	LCUITimer_Free( caret->timer_id );
	Widget_Destroy( caret->widget );
	caret->timer_id = -1;
	caret->widget = NULL;
}

/*-------------------------------- End Caret --------------------------------*/

/** 移动文本框内的文本插入符的行列坐标 */
static void TextBox_MoveCaret( LCUI_Widget *widget, int row, int col )
{
	LCUI_Pos pixel_pos;
	LCUI_TextBox *tb;
	int h;
	
	tb = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	TextLayer_SetCaretPos( &tb->text, row, col );
	if( TextLayer_GetCaretPixelPos( &tb->text, &pixel_pos ) == 0 ) {
		h = TextLayer_GetRowHeight( &tb->text, row );
		Caret_SetSize( &tb->caret, -1, h );
		Caret_Move( &tb->caret, pixel_pos.x, pixel_pos.y );
	}
	Caret_BlinkShow( &tb->caret );
}

/*------------------------------- TextBlock ---------------------------------*/
static void TextBlock_Destroy( void *arg )
{
	LCUI_TextBlock *blk;
	blk = (LCUI_TextBlock*)arg;
	if( blk->type == TEXT_BLOCK_TYPE_TEXT_END ) {
		StyleTagStack_Destroy( blk->style );
		blk->style = NULL;
	}
	free( blk->text );
}

static int TextBox_AddTextToBuffer( LCUI_Widget *widget, const wchar_t *wtext,
							TextAddType add_type )
{
	LCUI_TextBox *textbox;
	LCUI_TextBlock txtblk;
	wchar_t *text_buff;
	const wchar_t *p;
	int i, j, len, tag_len, size;
	
	txtblk.add_type = add_type;
	txtblk.style = MALLOC_ONE( LCUI_StyleTagStack );
	StyleTagStack_Init( txtblk.style );

	textbox = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	len = wcslen( wtext );
	for( i=0; i<len; ++i ) {
		if( len-i > textbox->block_size ) {
			size = textbox->block_size;
			txtblk.type = TEXT_BLOCK_TYPE_TEXT_BODY;
		} else {
			size = len-i;
			txtblk.type = TEXT_BLOCK_TYPE_TEXT_END;
		}
		if( i == 0 ) {
			txtblk.type = TEXT_BLOCK_TYPE_TEXT_BEGIN;
		}

		size += 1;
		text_buff = (wchar_t*)malloc( sizeof(wchar_t)*size );
		if( !text_buff ) {
			return -2;
		}
		
		/* 如果未启用样式标签功能 */
		if( !textbox->text.is_using_style_tags ) {
			for( j=0; i<len && j<size-1; ++j,++i ) {
				text_buff[j] = wtext[i];
			}
			--i;
			text_buff[j] = 0;
			txtblk.text = text_buff;
			/* 添加文本块至缓冲区 */
			Queue_Add( &textbox->text_block_buffer, &txtblk );
			continue;
		}
		for( j=0; i<len&&j<size-1; ++j,++i ) {
			text_buff[j] = wtext[i];
			/* 检测是否有样式标签 */
			p = scan_style_tag( wtext+i, NULL, 0, NULL );
			if( !p ) {
				p = scan_style_ending_tag( wtext+i, NULL );
				if( !p ) {
					continue;
				}
			}
			/* 计算标签的长度 */
			tag_len = p-wtext-i;
			/* 若当前块大小能够容纳这个标签 */
			if( j+tag_len <= size-1 ) {
				continue;
			}
			/* 重新计算该文本块的大小，并重新分配内存空间 */
			size = j+tag_len+1;
			text_buff = REALLOC( text_buff, size, wchar_t );
		}
		--i;
		text_buff[j] = 0;
		txtblk.text = text_buff;
		/* 添加文本块至缓冲区 */
		Queue_Add( &textbox->text_block_buffer, &txtblk );
	}
	WidgetMsg_Post( widget, WIDGET_MSG_TEXT_BLOCK, NULL, TRUE, FALSE );
	return 0;
}

/** 更新文本框内的字体位图 */
static void TextBox_ProcTextBlock( LCUI_Widget *widget, void *arg )
{
	LCUI_TextBox *tb;
	LCUI_TextBlock *blk;
	
	tb = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	/* 如果缓冲区内没有文本块 */
	if( Queue_GetTotal( &tb->text_block_buffer ) <= 0 ) {
		return;
	}
	Widget_Lock( widget );
	/* 获取第一个文本块 */
	blk = (LCUI_TextBlock*)Queue_Get( &tb->text_block_buffer, 0 );
	if( !blk ) {
		Widget_Unlock( widget );
		return;
	}

	switch( blk->add_type ) {
	case TEXT_ADD_TYPE_APPEND:
		/* 将此文本块追加至文本末尾 */
		TextLayer_AppendTextW( &tb->text, blk->text, blk->style );
		break;
	case TEXT_ADD_TYPE_INSERT:
		/* 将此文本块插入至文本插入符所在处 */
		TextLayer_InsertTextW( &tb->text, blk->text, blk->style );
	default: break;
	}
	/* 删除该文本块 */
	Queue_Delete( &tb->text_block_buffer, 0 );
	/* 下次继续更新 */
	WidgetMsg_Post( widget, WIDGET_MSG_TEXT_BLOCK, NULL, TRUE, FALSE );
	Widget_Update( widget );
	Widget_Unlock( widget );
}

/*----------------------------- End TextBlock ---------------------------------*/

/** 指定文本框是否处理控制符 */
LCUI_API void TextBox_SetUsingStyleTags( LCUI_Widget *widget, LCUI_BOOL is_true )
{
	LCUI_TextBox *tb;
	tb = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	TextLayer_SetUsingStyleTags( &tb->text, is_true );
}

/** 指定文本框是否启用多行文本显示 */
LCUI_API void TextBox_SetMultiline( LCUI_Widget *widget, LCUI_BOOL is_true )
{
	LCUI_TextBox *tb;
	tb = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	TextLayer_SetMultiline( &tb->text, is_true );
}

static void TextBox_DoClearText( LCUI_Widget *widget, void* arg )
{
	LCUI_TextBox *tb;
	tb = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	TextLayer_ClearText( &tb->text );
	Widget_InvalidateArea( widget, NULL );
}

/** 清空文本内容 */
LCUI_API void TextBox_ClearText( LCUI_Widget *widget )
{
	WidgetMsg_Post( widget, WIDGET_MSG_CLEAR_TEXT, NULL, FALSE, FALSE );
}

/** 为文本框设置文本（宽字符版） */
LCUI_API int TextBox_SetTextW( LCUI_Widget *widget, const wchar_t *wstr )
{
	LCUI_TextBox *tb;
	tb = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	TextBox_ClearText( widget );
	return TextBox_AddTextToBuffer( widget, wstr, TEXT_ADD_TYPE_APPEND );
}

/** 为文本框追加文本（宽字符版） */
LCUI_API int TextBox_AppendTextW( LCUI_Widget *widget, const wchar_t *wstr )
{
	LCUI_TextBox *tb;
	tb = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	return TextBox_AddTextToBuffer( widget, wstr, TEXT_ADD_TYPE_APPEND );
}

/** 为文本框插入文本（宽字符版） */
LCUI_API int TextBox_InsertTextW( LCUI_Widget *widget, const wchar_t *wstr )
{
	LCUI_TextBox *tb;
	tb = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	return TextBox_AddTextToBuffer( widget, wstr, TEXT_ADD_TYPE_INSERT );
}

static void TextBox_OnFocusIn( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_TextBox *tb;
	tb = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	/* 设定输入法的目标 */
	LCUIIME_SetTarget( widget );
	Caret_SetVisible( &tb->caret, TRUE );
}

static void TextBox_OnFocusOut(	LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_TextBox *tb;
	tb = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	Caret_SetVisible( &tb->caret, FALSE );
}

static void TextBox_DoTextBackspace( LCUI_Widget *widget, void *arg )
{
	LCUI_TextBox *tb;
	tb = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	TextLayer_Backspace( &tb->text, (int)arg );
	Caret_BlinkShow( &tb->caret );
	Widget_Update( widget );
}

static void TextBox_DoTextDelete( LCUI_Widget *widget, void *arg )
{
	LCUI_TextBox *tb;
	tb = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	TextLayer_Delete( &tb->text, (int)arg );
	Caret_BlinkShow( &tb->caret );
	Widget_Update( widget );
}

static void TextBox_TextBackspace( LCUI_Widget *widget, int n_ch )
{
	WidgetMsg_Post( widget, WIDGET_MSG_TEXT_BACKSPACE, (void*)n_ch, FALSE, FALSE );
}

static void TextBox_TextDelete(LCUI_Widget *widget, int n_ch )
{
	WidgetMsg_Post( widget, WIDGET_MSG_TEXT_DELETE, (void*)n_ch, FALSE, FALSE );
}

/** 处理按键事件 */
static void TextBox_ProcessKey( LCUI_Widget *widget, LCUI_WidgetEvent *event )
{
	LCUI_TextBox *tb;
	int cols, rows, cur_col, cur_row;
	
	if( event->key.key_state == LCUIKEYSTATE_RELEASE ) {
		return;
	}

	//_DEBUG_MSG("you input: %d\n", event->key.key_code);
	tb = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	cur_row = tb->text.insert_y;
	cur_col = tb->text.insert_x;
	cols = TextLayer_GetRowTextLength( &tb->text, cur_row );
	rows = TextLayer_GetRowTotal( &tb->text ); 
	switch( event->key.key_code ) {
	case LCUIKEY_HOMEPAGE: //home键移动光标至行首
		cur_col = 0;
		break;
	case LCUIKEY_END: //end键移动光标至行尾
		cur_col = cols;
		break;
	case LCUIKEY_LEFT:
		if( cur_col > 0 ) {
			--cur_col;
		} else if( cur_row > 0 ) {
			--cur_row;
			cur_col = TextLayer_GetRowTextLength( &tb->text, cur_row );
		}
		break;
	case LCUIKEY_RIGHT:
		if( cur_col < cols ) {
			++cur_col;
		} else if( cur_row < rows-1 ) {
			++cur_row;
			cur_col = 0;
		}
		break;
	case LCUIKEY_UP:
		if( cur_row > 0 ) {
			--cur_row;
		}
		break;
	case LCUIKEY_DOWN:
		if( cur_row < rows-1 ) {
			++cur_row;
		}
		break;
	case LCUIKEY_BACKSPACE: //删除光标左边的字符
		   TextBox_TextBackspace( widget, 1 );
		break;
	case LCUIKEY_DELETE: //删除光标右边的字符
		    TextBox_TextDelete( widget, 1 );
		break;
	default:break;
	}
	TextBox_MoveCaret( widget, cur_row, cur_col );
}

/** 处理输入法对文本框输入的内容 */
static void TextBox_OnInput( LCUI_Widget *widget, LCUI_WidgetEvent *event )
{
	unsigned int i;
	wchar_t *ptr, *tmp_ptr, *ptr_last;
	LCUI_TextBox *textbox;

	ptr = event->input.text;
	ptr_last = ptr + MAX_INPUT_TEXT_LEN;
	textbox = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	/* 如果文本框是只读的 */
	if( textbox->read_only ) {
		return;
	}
	if( textbox->allow_input_char.length == 0 ) {
		TextBox_InsertTextW( widget, event->input.text );
		return;
	}
	for( ; ptr<ptr_last && *ptr!='\0'; ++ptr ) {
		/* 判断当前字符是否为限制范围内的字符 */
		for( i=0; i<textbox->allow_input_char.length; ++i ) {
			if( textbox->allow_input_char.string[i] == *ptr ) {
				break;
			}
		}
		/* 如果已提前结束循环，则表明当前字符是允许的 */
		if( i < textbox->allow_input_char.length ) {
			continue;
		}
		/* 否则不是允许的字符，需移除该字符 */
		tmp_ptr = ptr;
		while( tmp_ptr<ptr_last && *tmp_ptr!='\0' ) {
			*tmp_ptr = *(tmp_ptr+1);
			++tmp_ptr;
		}
	}
}

static void TextBox_OnInit( LCUI_Widget *widget )
{
	LCUI_TextBox *tb;
	
	widget->valid_state = WIDGET_STATE_ACTIVE | WIDGET_STATE_OVERLAY;
	widget->valid_state |= (WIDGET_STATE_NORMAL | WIDGET_STATE_DISABLE);
	widget->focus = TRUE;
	Widget_SetPadding( widget, Padding(3,3,3,3) );
	Widget_SetBackgroundTransparent( widget, FALSE );

	tb = Widget_NewPrivateData( widget, LCUI_TextBox );
	
	tb->read_only = FALSE;
	TextLayer_Init( &tb->text );
	Caret_Init( &tb->caret );
	Caret_AddTo( &tb->caret, widget );
	TextLayer_SetUsingStyleTags( &tb->text, FALSE );
	
	LCUIWString_Init( &tb->allow_input_char );

	tb->block_size = TEXTBOX_DEFAULT_TEXT_BLOCK_SIZE;
	Queue_Init( &tb->text_block_buffer, sizeof(LCUI_TextBlock), TextBlock_Destroy );
	
	TextBox_SetMultiline( widget, FALSE );
	WidgetMsg_Connect( widget, WIDGET_MSG_CLEAR_TEXT, TextBox_DoClearText );
	WidgetMsg_Connect( widget, WIDGET_MSG_TEXT_BLOCK, TextBox_ProcTextBlock );
	WidgetMsg_Connect( widget, WIDGET_MSG_TEXT_BACKSPACE, TextBox_DoTextBackspace );
	WidgetMsg_Connect( widget, WIDGET_MSG_TEXT_DELETE, TextBox_DoTextDelete );
	/* 关联按键输入事件 */
	Widget_ConnectEvent( widget, EVENT_KEYBOARD, TextBox_ProcessKey );
	Widget_ConnectEvent( widget, EVENT_INPUT, TextBox_OnInput );
	/* 关联 FOCUS_OUT 和 FOCUS_IN 事件 */
	Widget_ConnectEvent( widget, EVENT_FOCUSOUT, TextBox_OnFocusOut );
	Widget_ConnectEvent( widget, EVENT_FOCUSIN, TextBox_OnFocusIn );

}

static void TextBox_OnDestroy( LCUI_Widget *widget )
{

}

static void TextBox_OnResize( LCUI_Widget *widget )
{
	LCUI_Size size;
	LCUI_TextBox *tb;

	tb = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	size = Widget_GetContainerSize( widget );
	TextLayer_SetMaxSize( &tb->text, size );
}

/** 更新当前文本框的样式 */
static void TextBox_ExecUpdateStyle( LCUI_Widget *widget )
{
	LCUI_Border border;
	Widget_SetBackgroundColor( widget, RGB(255,255,255) );
	Widget_SetBackgroundTransparent( widget, FALSE );
	/* 如果该部件已经获得全局焦点 */
	if( Widget_GetGlobalFocus( widget ) ) {
		border = Border( 1, BORDER_STYLE_SOLID, RGB(55,123,203) );
		Widget_SetBorder( widget, border );
		return;
	}
	Border_Radius( &border, 0 );
	/* 根据不同的状态，设定不同的边框样式 */
	switch( widget->state ) {
	case WIDGET_STATE_NORMAL:
		border.left_width = 1;
		border.left_style = BORDER_STYLE_SOLID;
		border.left_color = RGB(204,204,204);
		border.top_width = 1;
		border.top_style = BORDER_STYLE_SOLID;
		border.top_color = RGB(204,204,204);
		border.right_width = 1;
		border.right_style = BORDER_STYLE_SOLID;
		border.right_color = RGB(221,221,221);
		border.bottom_width = 1;
		border.bottom_style = BORDER_STYLE_SOLID;
		border.bottom_color = RGB(221,221,221);
		break;
	case WIDGET_STATE_OVERLAY :
		border.left_width = 1;
		border.left_style = BORDER_STYLE_SOLID;
		border.left_color = RGB(171,171,171);
		border.top_width = 1;
		border.top_style = BORDER_STYLE_SOLID;
		border.top_color = RGB(171,171,171);
		border.right_width = 1;
		border.right_style = BORDER_STYLE_SOLID;
		border.right_color = RGB(204,204,204);
		border.bottom_width = 1;
		border.bottom_style = BORDER_STYLE_SOLID;
		border.bottom_color = RGB(204,204,204);
		break;
	case WIDGET_STATE_ACTIVE :
		border = Border( 1, BORDER_STYLE_SOLID, RGB(55,123,203) );
		break;
	case WIDGET_STATE_DISABLE :
		break;
		default : break;
	}
	Widget_SetBorder( widget, border );
}

/** 更新文本框的文本图层 */
static void TextBox_UpdateTextLayer( LCUI_Widget *widget )
{
	int n;
	LCUI_TextBox *tb;
	LCUI_Rect *p_rect;
	LCUI_TextLayer *layer;
	LinkedList rect_list;

	tb = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	DirtyRectList_Init( &rect_list );
	/* 根据是否启用屏蔽符，判断需更新哪个文本图层的数据 */
	if( !tb->password_char ) {
		layer = &tb->text;
	} else {
		layer = &tb->mask_text;
	}
	n = LinkedList_GetTotal( &rect_list );
	LinkedList_Goto( &rect_list, 0 );
	while(n--) {
		p_rect = (LCUI_Rect*)LinkedList_Get( &rect_list );
		Widget_InvalidateArea( widget, p_rect );
		LinkedList_ToNext( &rect_list );
	}
	DirtyRectList_Destroy( &rect_list );
	TextLayer_ClearInvalidRect( layer );
}

static void TextBox_OnUpdate( LCUI_Widget *widget )
{
	LCUI_TextBox *tb;
	tb = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	TextBox_ExecUpdateStyle( widget );
	TextBox_UpdateTextLayer( widget );
	TextBox_MoveCaret( widget, tb->text.insert_y, tb->text.insert_x );
}

static void TextBox_OnPaint( LCUI_Widget *widget )
{
	LCUI_TextBox *tb;
	LCUI_Pos pos;
	LCUI_Rect area;
	LCUI_Graph area_graph;

	tb = (LCUI_TextBox*)Widget_GetPrivateData( widget );
	TextBox_UpdateTextLayer( widget );
	TextLayer_Draw( &tb->text );
	if( !Widget_BeginPaint( widget, &area ) ) {
		return;
	}
	Widget_QuoteInnerGraph( widget, &area_graph, &area );
	pos.x = widget->padding.left;
	pos.y = widget->padding.top;
	TextLayer_DrawToGraph( &tb->text, area, pos, 
			widget->background.transparent, &area_graph );
	Widget_EndPaint( widget, &area );
}

void RegisterTextBox( void )
{
	WidgetType_Add( WIDGET_TEXT_BOX );
	WidgetFunc_Add( WIDGET_TEXT_BOX, TextBox_OnInit, FUNC_TYPE_INIT );
	WidgetFunc_Add( WIDGET_TEXT_BOX, TextBox_OnUpdate, FUNC_TYPE_UPDATE );
	WidgetFunc_Add( WIDGET_TEXT_BOX, TextBox_OnPaint, FUNC_TYPE_PAINT );
	WidgetFunc_Add( WIDGET_TEXT_BOX, TextBox_OnResize, FUNC_TYPE_RESIZE );
	WidgetFunc_Add( WIDGET_TEXT_BOX, TextBox_OnDestroy, FUNC_TYPE_DESTROY );
}
