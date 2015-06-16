/* ***************************************************************************
 * textview.c -- LCUI's TextView Widget
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
 * textview.c -- LCUI 的文本显示部件
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
#include <LCUI/widget_build.h>
#include <LCUI/gui/widget/textview.h>

enum TaskType {
	TASK_SET_TEXT,
	TASK_SET_AUTOWRAP,
	TASK_SET_TEXT_ALIGN,
	TASK_UPDATE,
	TASK_TOTAL
};

typedef struct LCUI_TextView_ {
	LCUI_TextLayer layer;	/* 文本图层 */
	struct {
		LCUI_BOOL is_valid;
		union {
			wchar_t *text;
			LCUI_BOOL autowrap;
		};
	} tasks[4];
} LCUI_TextView;

/*---------------------------- Private -------------------------------*/

/** 初始化 TextView 部件数据 */
static void TextView_OnInit( LCUI_Widget w )
{
	int i;
	LCUI_TextView *txt;

	/* txt部件不需要焦点 */
	w->focus = FALSE;
	txt = Widget_NewPrivateData( w, LCUI_TextView );
	for( i = 0; i < TASK_TOTAL; ++i ) {
		txt->tasks[i].is_valid = FALSE;
	}
	/* 初始化文本图层 */
	txt->layer = TextLayer_New();
	/* 启用多行文本显示 */
	TextLayer_SetMultiline( txt->layer, TRUE );
	/* 启用样式标签的支持 */
	TextLayer_SetUsingStyleTags( txt->layer, TRUE );
}

/** 释放 TextView 部件占用的资源 */
static void TextView_OnDestroy( LCUI_Widget w )
{
	LCUI_TextView *txt;
	txt = (LCUI_TextView*)w->private_data;
	TextLayer_Destroy( &txt->layer );
}

static void TextView_OnUpdate( LCUI_Widget w )
{
	int n;
	LCUI_TextView *txt;
	LCUI_Rect *p_rect;
	LinkedList rect_list;
	LCUI_Size new_size;

	txt = (LCUI_TextView*)w->private_data;
	DirtyRectList_Init( &rect_list );
	/* 先更新文本图层的数据 */
	TextLayer_Update( txt->layer, &rect_list );
	new_size.w = TextLayer_GetWidth( txt->layer );
	new_size.h = TextLayer_GetHeight( txt->layer );
	/* 如果部件尺寸不是由LCUI自动调整的 */
	if( w->style.w.type == SVT_PX ) {
		new_size.w = w->base.width;
		new_size.h = w->base.height;
		/* 最小尺寸为20x20 */
		if( new_size.w < 20 ) {
			new_size.w = 20;
		}
		if( new_size.h < 20 ) {
			new_size.h = 20;
		}
		TextLayer_SetMaxSize( txt->layer, new_size );
		TextLayer_Update( txt->layer, &rect_list );
	}
	else if( !txt->layer->is_autowrap_mode ) {
		/* 既然未启用自动换行，那么自动调整部件尺寸 */
		if( new_size.w != w->base.width
		 || new_size.h != w->base.height ) {
			TextLayer_SetMaxSize( txt->layer, new_size );
			Widget_Resize( w, new_size.w, new_size.h );
			TextLayer_Update( txt->layer, &rect_list );
		}
	} else {
		/* 能到这里，则说明部件启用了文本自动换行和自动尺寸调整 */
		new_size.w = w->base.width;
		new_size.h = w->base.height;
		/* 将部件所在容器的宽度作为图像宽度 */
		new_size.w = w->parent->base.box.content.width;
		/* 高度必须有效，最小高度为20 */
		if( new_size.h < 20 ) {
			new_size.h = 20;
		}
		if( txt->layer->graph.w != new_size.w
		 || txt->layer->graph.h != new_size.h ) {
			/* 重新设置最大尺寸 */
			TextLayer_SetMaxSize( txt->layer, new_size );
			TextLayer_Update( txt->layer, &rect_list );
			/* 重新计算文本图层的尺寸 */
			new_size.w = TextLayer_GetWidth( txt->layer );
			new_size.h = TextLayer_GetHeight( txt->layer );
			TextLayer_SetMaxSize( txt->layer, new_size );
			Widget_Resize( w, new_size.w, new_size.h );
		}
	}

	n = LinkedList_GetTotal( &rect_list );
	LinkedList_Goto( &rect_list, 0 );
	/* 将得到的无效区域导入至部件的无效区域列表 */
	while(n--) {
		p_rect = (LCUI_Rect*)LinkedList_Get( &rect_list );
		Widget_InvalidateArea( w, p_rect, CONTENT_BOX );
		LinkedList_ToNext( &rect_list );
	}
	DirtyRectList_Destroy( &rect_list );
	TextLayer_ClearInvalidRect( txt->layer );
}

/** 私有的任务处理接口 */
static void TextView_OnTask( LCUI_Widget w, LCUI_WidgetTask *t )
{
	int i;
	LinkedList rects;
	LCUI_TextView *txt = (LCUI_TextView*)w->private_data;

	DirtyRectList_Init( &rects );
	i = TASK_SET_TEXT;
	if( txt->tasks[i].is_valid ) {
		txt->tasks[i].is_valid = FALSE;
		TextLayer_SetTextW( txt->layer, txt->tasks[i].text, NULL );
		txt->tasks[TASK_UPDATE].is_valid = TRUE;
	}
	i = TASK_SET_AUTOWRAP;
	if( txt->tasks[i].is_valid ) {
		txt->tasks[i].is_valid = FALSE;
		TextLayer_SetAutoWrap( txt->layer, txt->tasks[i].autowrap );
		txt->tasks[TASK_UPDATE].is_valid = TRUE;
	}
	i = TASK_UPDATE;
	if( txt->tasks[i].is_valid ) {
		txt->tasks[i].is_valid = FALSE;
		TextView_OnUpdate( w );
	}
}

/** 绘制 TextView 部件 */
static void TextView_OnPaint( LCUI_Widget w, LCUI_PaintContext paint )
{
	LCUI_TextView *txt;
	LCUI_Rect content_rect, rect;
	LCUI_Pos layer_pos;

	txt = (LCUI_TextView*)w->private_data;
	content_rect.x = w->base.box.content.left - w->base.box.graph.left;
	content_rect.y = w->base.box.content.top - w->base.box.graph.top;
	content_rect.width = w->base.box.content.width;
	content_rect.height = w->base.box.content.height;
	LCUIRect_GetOverlayRect( &content_rect, &paint->rect, &rect );
	rect.x -= content_rect.x;
	rect.y -= content_rect.y;
	layer_pos.x = -rect.x;
	layer_pos.y = -rect.y;
	TextLayer_DrawToGraph( txt->layer, rect, layer_pos, &paint->canvas );
}

/*-------------------------- End Private -----------------------------*/

/*---------------------------- Public --------------------------------*/

/** 设定与 TextView 关联的文本内容 */
LCUI_API int TextView_SetTextW( LCUI_Widget w, const wchar_t *text )
{
	int len;
	wchar_t *text_ptr;
	LCUI_TextView *txt;
	LCUI_WidgetTask task;

	len = text ? wcslen( text ):0;
	text_ptr = (wchar_t*)malloc( sizeof(wchar_t)*(len+1) );
	if( !text_ptr ) {
		return -1;
	}
	if( !text ) {
		text_ptr[0] = 0;
	} else {
		wcscpy( text_ptr, text );
	}
	Widget_Lock( w );
	txt = (LCUI_TextView*)w->private_data;
	if( txt->tasks[TASK_SET_TEXT].is_valid
	 && txt->tasks[TASK_SET_TEXT].text ) {
		free( txt->tasks[TASK_SET_TEXT].text );
	}
	txt->tasks[TASK_SET_TEXT].is_valid = TRUE;
	txt->tasks[TASK_SET_TEXT].text = text_ptr;
	Widget_AddTask( w, (task.type = WTT_USER, &task) );
	Widget_Unlock( w );
	return 0;
}

LCUI_API int TextView_SetText( LCUI_Widget w, const char *utf8_text )
{
	int ret;
	wchar_t *wstr;

	LCUICharset_UTF8ToUnicode( utf8_text, &wstr );
	ret = TextView_SetTextW( w, wstr );
	if( wstr ) {
		free( wstr );
	}
	return ret;
}

/*-------------------------- End Public ------------------------------*/

/** 添加 TextView 部件类型 */
void LCUIWidget_AddTextView( void )
{
	LCUI_WidgetClass *wc = LCUIWidget_AddClass( "textview" );
	wc->methods.init = TextView_OnInit;
	wc->methods.paint = TextView_OnPaint;
	wc->methods.destroy = TextView_OnDestroy;
	wc->task_handler = TextView_OnTask;
}
