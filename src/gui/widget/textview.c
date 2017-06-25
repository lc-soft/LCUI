/* ***************************************************************************
 * textview.c -- LCUI's TextView Widget
 *
 * Copyright (C) 2015-2017 by Liu Chao <lc-soft@live.cn>
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
 * textview.c -- LCUI 的文本显示部件
 *
 * 版权所有 (C) 2015-2017 归属于 刘超 <lc-soft@live.cn>
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <LCUI/gui/metrics.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/css_fontstyle.h>
#include <LCUI/gui/widget/textview.h>

enum TaskType {
	TASK_SET_TEXT,
	TASK_SET_AUTOWRAP,
	TASK_SET_TEXT_ALIGN,
	TASK_UPDATE,
	TASK_TOTAL
};

typedef struct LCUI_TextViewRec_ {
	LCUI_Widget widget;		/**< 所属的部件 */
	LCUI_BOOL has_content;		/**< 是否有设置 content 属性 */
	LCUI_Mutex mutex;		/**< 互斥锁 */
	LCUI_TextLayer layer;		/**< 文本图层 */
	LCUI_FontStyleRec style;	/**< 文字样式 */
	struct {
		LCUI_BOOL is_valid;
		union {
			wchar_t *text;
			LCUI_BOOL autowrap;
			int align;
		};
	} tasks[TASK_TOTAL];
} LCUI_TextViewRec, *LCUI_TextView;

/*---------------------------- Private -------------------------------*/

static struct LCUI_TextViewModule {
	LCUI_WidgetPrototype prototype;
} self;
static void TextView_OnParseText( LCUI_Widget w, const char *text )
{
	TextView_SetText( w, text );
}

static void TextView_SetTaskForLineHeight( LCUI_Widget w, int height )
{
	LCUI_TextView txt = Widget_GetData( w, self.prototype );
	TextLayer_SetLineHeight( txt->layer, height );
	txt->tasks[TASK_UPDATE].is_valid = TRUE;
	Widget_AddTask( w, WTT_USER );
}

static void TextView_SetTextStyle( LCUI_Widget w, LCUI_TextStyle *style )
{
	LCUI_TextView txt = Widget_GetData( w, self.prototype );
	TextLayer_SetTextStyle( txt->layer, style );
	txt->tasks[TASK_UPDATE].is_valid = TRUE;
	Widget_AddTask( w, WTT_USER );
}

static void TextView_SetTaskForTextAlign( LCUI_Widget w, int align )
{
	LCUI_TextView txt = Widget_GetData( w, self.prototype );
	txt->tasks[TASK_SET_TEXT_ALIGN].align = align;
	txt->tasks[TASK_SET_TEXT_ALIGN].is_valid = TRUE;
	Widget_AddTask( w, WTT_USER );
}

static void TextView_SetTaskForAutoWrap( LCUI_Widget w, LCUI_BOOL autowrap )
{
	LCUI_TextView txt = Widget_GetData( w, self.prototype );
	txt->tasks[TASK_SET_AUTOWRAP].autowrap = autowrap;
	txt->tasks[TASK_SET_AUTOWRAP].is_valid = TRUE;
	Widget_AddTask( w, WTT_USER );
}

static void TextView_UpdateStyle( LCUI_Widget w )
{
	LCUI_TextStyle ts;
	LCUI_TextView txt = Widget_GetData( w, self.prototype );
	LCUI_FontStyle fs = &txt->style;
	const wchar_t *content = fs->content;
	LCUIFontStyle_Compute( fs, w->style );
	LCUIFontStyle_GetTextStyle( fs, &ts );
	TextView_SetTaskForTextAlign( w, fs->text_align );
	TextView_SetTaskForLineHeight( w, fs->line_height );
	TextView_SetTaskForAutoWrap( w, fs->white_space != SV_NOWRAP );
	TextView_SetTextStyle( w, &ts );
	if( content != fs->content ) {
		TextView_SetTextW( w, fs->content );
	}
	txt->tasks[TASK_UPDATE].is_valid = TRUE;
	Widget_AddTask( w, WTT_USER );
	TextStyle_Destroy( &ts );
}

static void TextView_OnResize( LCUI_Widget w, LCUI_WidgetEvent e, void *arg )
{
	LinkedList rects;
	LinkedListNode *node;
	LCUI_TextView txt;
	int width = 0, height = 0;
	float scale, max_width = 0, max_height = 0;

	LinkedList_Init( &rects );
	scale = LCUIMetrics_GetScale();
	txt = Widget_GetData( w, self.prototype );
	if( Widget_HasAutoWidth( w ) ) {
		max_width = Widget_ComputeMaxWidth( w );
		max_width -= w->computed_style.border.left.width;
		max_width -= w->computed_style.border.right.width;
		max_width -= w->padding.left + w->padding.right;
	} else {
		max_width = w->box.content.width;
	}
	if( Widget_HasAutoStyle( w, key_height ) ) {
		max_height = w->box.content.height;
	}
	/* 将当前部件宽高作为文本层的固定宽高 */
	width = LCUIMetrics_ComputeActual( w->box.content.width, SVT_PX );
	height = LCUIMetrics_ComputeActual( w->box.content.height, SVT_PX );
	TextLayer_SetFixedSize( txt->layer, width, height );
	width = LCUIMetrics_ComputeActual( max_width, SVT_PX );
	height = LCUIMetrics_ComputeActual( max_height, SVT_PX );
	TextLayer_SetMaxSize( txt->layer, width, height );
	TextLayer_Update( txt->layer, &rects );
	for( LinkedList_Each( node, &rects ) ) {
		LCUIRect_Scale( node->data, node->data, 1.0f / scale );
		Widget_InvalidateArea( w, node->data, SV_CONTENT_BOX );
	}
	RectList_Clear( &rects );
	TextLayer_ClearInvalidRect( txt->layer );
}

/** 初始化 TextView 部件数据 */
static void TextView_OnInit( LCUI_Widget w )
{
	int i;
	LCUI_TextView txt;
	txt = Widget_AddData( w, self.prototype,
			      sizeof( LCUI_TextViewRec ) );
	for( i = 0; i < TASK_TOTAL; ++i ) {
		txt->tasks[i].is_valid = FALSE;
	}
	txt->widget = w;
	txt->has_content = FALSE;
	/* 初始化文本图层 */
	txt->layer = TextLayer_New();
	/* 启用多行文本显示 */
	TextLayer_SetAutoWrap( txt->layer, TRUE );
	TextLayer_SetMultiline( txt->layer, TRUE );
	/* 启用样式标签的支持 */
	TextLayer_SetUsingStyleTags( txt->layer, TRUE );
	Widget_BindEvent( w, "resize", TextView_OnResize, NULL, NULL );
	LCUIFontStyle_Init( &txt->style );
	LCUIMutex_Init( &txt->mutex );
}

static void TextView_ClearTasks( LCUI_Widget w )
{
	int i = TASK_SET_TEXT;
	LCUI_TextView txt = Widget_GetData( w, self.prototype );
	if( txt->tasks[i].is_valid ) {
		txt->tasks[i].is_valid = FALSE;
		free( txt->tasks[i].text );
		txt->tasks[i].text = NULL;
	}
}

/** 释放 TextView 部件占用的资源 */
static void TextView_OnDestroy( LCUI_Widget w )
{
	LCUI_TextView txt = Widget_GetData( w, self.prototype );
	LCUIFontStyle_Destroy( &txt->style );
	TextLayer_Destroy( txt->layer );
	LCUIMutex_Unlock( &txt->mutex );
	TextView_ClearTasks( w );
}

static void TextView_AutoSize( LCUI_Widget w, float *width, float *height )
{
	float scale = LCUIMetrics_GetScale();
	LCUI_TextView txt = Widget_GetData( w, self.prototype );
	if( Widget_HasAutoWidth( w ) ) {
		int fixed_w = txt->layer->fixed_width;
		int fixed_h = txt->layer->fixed_height;
		/* 解除固定宽高设置，以计算最大宽高 */
		TextLayer_SetFixedSize( txt->layer, (int)*width, 0 );
		TextLayer_Update( txt->layer, NULL );
		if( *width <= 0 ) {
			*width = TextLayer_GetWidth( txt->layer ) / scale;
		}
		if( *height <= 0 ) {
			*height = TextLayer_GetHeight( txt->layer ) / scale;
		}
		/* 还原固定宽高设置 */
		TextLayer_SetFixedSize( txt->layer, fixed_w, fixed_h );
		TextLayer_Update( txt->layer, NULL );
		return;
	}
	if( *width <= 0 ) {
		*width = TextLayer_GetWidth( txt->layer ) / scale;
	}
	if( *height <= 0 ) {
		*height = TextLayer_GetHeight( txt->layer ) / scale;
	}
}

/** 私有的任务处理接口 */
static void TextView_OnTask( LCUI_Widget w )
{
	int i;
	float scale;
	LinkedList rects;
	LinkedListNode *node;
	LCUI_TextView txt = Widget_GetData( w, self.prototype );

	LinkedList_Init( &rects );
	i = TASK_SET_TEXT;
	if( txt->tasks[i].is_valid ) {
		LCUIMutex_Lock( &txt->mutex );
		txt->tasks[i].is_valid = FALSE;
		TextLayer_SetTextW( txt->layer, txt->tasks[i].text, NULL );
		txt->tasks[TASK_UPDATE].is_valid = TRUE;
		free( txt->tasks[i].text );
		txt->tasks[i].text = NULL;
		LCUIMutex_Unlock( &txt->mutex );
	}
	i = TASK_SET_AUTOWRAP;
	if( txt->tasks[i].is_valid ) {
		txt->tasks[i].is_valid = FALSE;
		TextLayer_SetAutoWrap( txt->layer, txt->tasks[i].autowrap );
		txt->tasks[TASK_UPDATE].is_valid = TRUE;
	}
	i = TASK_SET_TEXT_ALIGN;
	if( txt->tasks[i].is_valid ) {
		txt->tasks[i].is_valid = FALSE;
		TextLayer_SetTextAlign( txt->layer, txt->tasks[i].align );
		txt->tasks[TASK_UPDATE].is_valid = TRUE;
	}
	i = TASK_UPDATE;
	if( !txt->tasks[i].is_valid ) {
		return;
	}
	txt->tasks[i].is_valid = FALSE;
	LinkedList_Init( &rects );
	scale = LCUIMetrics_GetScale();
	TextLayer_Update( txt->layer, &rects );
	for( LinkedList_Each( node, &rects ) ) {
		LCUIRect_Scale( node->data, node->data, 1.0f / scale );
		Widget_InvalidateArea( w, node->data, SV_CONTENT_BOX );
	}
	RectList_Clear( &rects );
	TextLayer_ClearInvalidRect( txt->layer );
	if( w->style->sheet[key_width].type == SVT_AUTO
	 || w->style->sheet[key_height].type == SVT_AUTO ) {
		Widget_AddTask( w, WTT_RESIZE );
	}
}

/** 绘制 TextView 部件 */
static void TextView_OnPaint( LCUI_Widget w, LCUI_PaintContext paint )
{
	LCUI_Pos pos;
	LCUI_RectF rectf;
	LCUI_TextView txt;
	LCUI_Rect content_rect, rect;

	rectf = w->box.content;
	rectf.x -= w->box.graph.x;
	rectf.y -= w->box.graph.y;
	LCUIMetrics_ComputeRectActual( &content_rect, &rectf );
	LCUIRect_GetOverlayRect( &content_rect, &paint->rect, &rect );
	pos.x = content_rect.x - paint->rect.x;
	pos.y = content_rect.y - paint->rect.y;
	rect.x -= content_rect.x;
	rect.y -= content_rect.y;
	txt = Widget_GetData( w, self.prototype );
	TextLayer_DrawToGraph( txt->layer, rect, pos, &paint->canvas );
}

/*-------------------------- End Private -----------------------------*/

/*---------------------------- Public --------------------------------*/

int TextView_SetTextW( LCUI_Widget w, const wchar_t *text )
{
	size_t len;
	wchar_t *newtext;
	LCUI_TextView txt;

	len = text ? wcslen( text ) + 1 : 1;
	newtext = NEW( wchar_t, len );
	if( !newtext ) {
		return -1;
	}
	if( !text ) {
		newtext[0] = 0;
	} else {
		wcscpy( newtext, text );
	}
	txt = Widget_GetData( w, self.prototype );
	LCUIMutex_Lock( &txt->mutex );
	if( txt->tasks[TASK_SET_TEXT].is_valid
	 && txt->tasks[TASK_SET_TEXT].text ) {
		free( txt->tasks[TASK_SET_TEXT].text );
	}
	txt->tasks[TASK_SET_TEXT].is_valid = TRUE;
	txt->tasks[TASK_SET_TEXT].text = newtext;
	Widget_AddTask( w, WTT_USER );
	LCUIMutex_Unlock( &txt->mutex );
	return 0;
}

int TextView_SetText( LCUI_Widget w, const char *utf8_text )
{
	int ret;
	wchar_t *wstr;
	size_t len = strlen( utf8_text ) + 1;
	wstr = malloc( sizeof( wchar_t )*len );
	LCUI_DecodeString( wstr, utf8_text, len, ENCODING_UTF8 );
	ret = TextView_SetTextW( w, wstr );
	if( wstr ) {
		free( wstr );
	}
	return ret;
}

void TextView_SetLineHeight( LCUI_Widget w, int height )
{
	Widget_SetFontStyle( w, key_line_height, height, px );
}

void TextView_SetTextAlign( LCUI_Widget w, int align )
{
	Widget_SetFontStyle( w, key_text_align, align, style );
}

void TextView_SetAutoWrap( LCUI_Widget w, LCUI_BOOL autowrap )
{
	if( autowrap ) {
		Widget_SetFontStyle( w, key_white_space, SV_AUTO, style );
	} else {
		Widget_SetFontStyle( w, key_white_space, SV_NOWRAP, style );
	}
}

/*-------------------------- End Public ------------------------------*/

void LCUIWidget_AddTextView( void )
{
	self.prototype = LCUIWidget_NewPrototype( "textview", NULL );
	self.prototype->init = TextView_OnInit;
	self.prototype->paint = TextView_OnPaint;
	self.prototype->destroy = TextView_OnDestroy;
	self.prototype->autosize = TextView_AutoSize;
	self.prototype->update = TextView_UpdateStyle;
	self.prototype->settext = TextView_OnParseText;
	self.prototype->runtask = TextView_OnTask;
}
