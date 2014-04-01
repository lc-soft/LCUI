/* ***************************************************************************
 * label.c -- LCUI's Label widget
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
 * label.c -- LCUI 的文本标签部件
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
#include LC_LABEL_H

enum label_msg_id {
	LABEL_TEXT = WIDGET_USER+1,
	LABEL_AUTO_WRAP,
	LABEL_STYLE
};

typedef struct LCUI_Label_ {
	LCUI_BOOL auto_size;	/* 指定是否根据文本图层的尺寸来调整部件尺寸 */
	AUTOSIZE_MODE mode;	/* 自动尺寸调整的模式 */
	LCUI_TextLayer layer;	/* 文本图层 */
} LCUI_Label;

/*---------------------------- Private -------------------------------*/

/** 获取label部件内的文本图层的指针 */
static LCUI_TextLayer* Label_GetTextLayer( LCUI_Widget *widget )
{
	LCUI_Label *label;
	label = (LCUI_Label*)Widget_GetPrivateData( widget );
	return &label->layer;
}

static void Label_UpdateTextLayer( LCUI_Widget *widget )
{
	LCUI_Label *label;
	LCUI_Rect *p_rect;
	LinkedList rect_list;
	LCUI_Size new_size;

	label = (LCUI_Label*)Widget_GetPrivateData( widget );
	DirtyRectList_Init( &rect_list );
	/* 先更新文本图层的数据 */
	TextLayer_Update( &label->layer, &rect_list );

	new_size.w = TextLayer_GetWidth( &label->layer );
	new_size.h = TextLayer_GetHeight( &label->layer );
	/* 如果部件尺寸不是由LCUI自动调整的 */
	if( widget->dock != DOCK_TYPE_NONE || !label->auto_size ) {
		new_size = Widget_GetSize(widget);
		/* 最小尺寸为20x20 */
		if( new_size.w < 20 ) {
			new_size.w = 20;
		}
		if( new_size.h < 20 ) {
			new_size.h = 20;
		}
		TextLayer_SetMaxSize( &label->layer, new_size );
		TextLayer_Update( &label->layer, &rect_list );
	}
	else if( !label->layer.is_autowrap_mode ) {
		/* 既然未启用自动换行，那么自动调整部件尺寸 */
		if( new_size.w != widget->size.w
		 || new_size.h != widget->size.h ) {
			TextLayer_SetMaxSize( &label->layer, new_size );
			Widget_Resize( widget, new_size );
			TextLayer_Update( &label->layer, &rect_list );
		}
	} else {
		/* 能到这里，则说明部件启用了文本自动换行，以及自动尺寸调整 */
		new_size = Widget_GetSize(widget);
		/* 将部件所在容器的宽度作为图像宽度 */
		new_size.w = Widget_GetContainerWidth( widget->parent );
		/* 高度必须有效，最小高度为20 */
		if( new_size.h < 20 ) {
			new_size.h = 20;
		}
		if( label->layer.graph.w != new_size.w
		 || label->layer.graph.h != new_size.h ) {
			/* 重新设置最大尺寸 */
			TextLayer_SetMaxSize( &label->layer, new_size );
			TextLayer_Update( &label->layer, &rect_list );
			/* 重新计算文本图层的尺寸 */
			new_size.w = TextLayer_GetWidth( &label->layer );
			new_size.h = TextLayer_GetHeight( &label->layer );
			TextLayer_SetMaxSize( &label->layer, new_size );
			Widget_Resize( widget, new_size );
		}
	}

	LinkedList_Goto( &rect_list, 0 );
	/* 将得到的无效区域导入至部件的无效区域列表 */
	while( p_rect = (LCUI_Rect*)LinkedList_Get(&rect_list) ) {
		Widget_InvalidateArea( widget, p_rect );
	}
	DirtyRectList_Destroy( &rect_list );
	TextLayer_ClearInvalidRect( &label->layer );
}

static void Label_OnSetTextStyle( LCUI_Widget *widget, void *arg )
{
	LCUI_TextStyle *style;
	LCUI_Label *label;
	
	DEBUG_MSG("recv LABEL_STYLE msg, lock widget\n");
	style = (LCUI_TextStyle*)arg;
	label = (LCUI_Label*)Widget_GetPrivateData( widget );
	if( !label ) {
		return;
	}
	Widget_Lock( widget );
	TextLayer_SetTextStyle( &label->layer, style );
	Widget_Update( widget );
	DEBUG_MSG("unlock widget\n");
	Widget_Unlock( widget );
}

static void Label_OnSetTextW( LCUI_Widget *widget, void *arg )
{
	LCUI_Label *label;
	DEBUG_MSG("recv LABEL_TEXT msg, lock widget\n");
	label = (LCUI_Label*)Widget_GetPrivateData( widget );
	if( !label ) {
		return;
	}
	Widget_Lock( widget );
	TextLayer_SetTextW( &label->layer, (wchar_t*)arg, NULL );
	Label_UpdateTextLayer( widget );
	DEBUG_MSG("unlock widget\n");
	Widget_Unlock( widget );
}

static void Label_OnSetAutoWrap( LCUI_Widget *widget, void *arg )
{
	LCUI_BOOL flag;
	LCUI_Label *label;

	flag = arg?TRUE:FALSE;
	label = (LCUI_Label*)Widget_GetPrivateData( widget );
	if( !label ) {
		return;
	}
	Widget_Lock( widget );
	TextLayer_SetAutoWrap( &label->layer, flag );
	Widget_Update( widget );
	Widget_Unlock( widget );
}

/** 初始化label部件数据 */
static void Label_OnInit( LCUI_Widget *widget )
{
	LCUI_Label *label;
	/* label部件不需要焦点 */
	widget->focus = FALSE;
	label = Widget_NewPrivateData( widget, LCUI_Label );
	label->auto_size = TRUE;
	/* 初始化文本图层 */
	TextLayer_Init( &label->layer ); 
	/* 启用多行文本显示 */
	TextLayer_SetMultiline( &label->layer, TRUE );
	Widget_SetAutoSize( widget, FALSE, 0 );
	/* 启用样式标签的支持 */
	TextLayer_SetUsingStyleTags( &label->layer, TRUE );
	/* 将回调函数与自定义消息关联 */
	WidgetMsg_Connect( widget, LABEL_TEXT, Label_OnSetTextW );
	WidgetMsg_Connect( widget, LABEL_STYLE, Label_OnSetTextStyle );
	WidgetMsg_Connect( widget, LABEL_AUTO_WRAP, Label_OnSetAutoWrap );
}

/** 释放label部件占用的资源 */
static void Label_OnDestroy( LCUI_Widget *widget )
{
	LCUI_Label *label;
	label = (LCUI_Label*)Widget_GetPrivateData( widget );
	TextLayer_Destroy( &label->layer );
}

/** 更新Label部件的数据 */
static void Label_OnUpdate( LCUI_Widget *widget )
{
	Label_UpdateTextLayer( widget );
}

/** 绘制label部件 */
static void Label_OnPaint( LCUI_Widget *widget )
{
	LCUI_Label *label;
	LCUI_Rect area;
	LCUI_Pos layer_pos;
	LCUI_Graph *widget_graph, area_graph;

	label = (LCUI_Label*)Widget_GetPrivateData( widget );
	if( !Widget_BeginPaint( widget, &area ) ) {
		return;
	}
	widget_graph = Widget_GetSelfGraph( widget );
	Graph_Init( &area_graph );
	Graph_Quote( &area_graph, widget_graph, area );
	layer_pos.x = widget->border.left_width + 1;
	layer_pos.y = widget->border.top_width + 1;
	TextLayer_DrawToGraph( &label->layer, area, layer_pos, 
			widget->background.transparent, &area_graph );
}

/*-------------------------- End Private -----------------------------*/

/*---------------------------- Public --------------------------------*/
/** 设定与标签关联的文本内容 */
LCUI_API int Label_SetTextW( LCUI_Widget *widget, const wchar_t *text )
{
	int len;
	wchar_t *p_text;

	if( !widget ) {
		return -1;
	}
	if( !text ) {
		len = 0;
	} else {
		len = wcslen( text );
	}
	p_text = (wchar_t*)malloc( sizeof(wchar_t)*(len+1) );
	if( !p_text ) {
		return -1;
	}
	if( !text ) {
		p_text[0] = '\0';
	} else {
		wcscpy( p_text, text );
	}
	DEBUG_MSG("post LABEL_TEXT msg\n");
	WidgetMsg_Post( widget, LABEL_TEXT, p_text, TRUE, TRUE );
	return 0;
}

LCUI_API int Label_SetText( LCUI_Widget *widget, const char *utf8_text )
{
	int ret;
	wchar_t *wstr;

	LCUICharset_UTF8ToUnicode( utf8_text, &wstr );
	ret = Label_SetTextW( widget, wstr );
	if( wstr ) {
		free( wstr );
	}
	return ret;
}

LCUI_API int Label_SetTextA( LCUI_Widget *widget, const char *ansi_text )
{
	int ret;
	wchar_t *wstr;
	ret = LCUICharset_GB2312ToUnicode( ansi_text, &wstr );
	ret |= Label_SetTextW( widget, wstr );
	if( wstr ) {
		free( wstr );
	}
	return ret;
}

/** 设置Label部件显示的文本是否自动换行 */
LCUI_API void Label_SetAutoWrap( LCUI_Widget *widget, LCUI_BOOL flag )
{
	WidgetMsg_Post( widget, LABEL_AUTO_WRAP, flag?((void*)(1)):NULL, TRUE, FALSE );
}

/** 设置文本对齐方式 */
LCUI_API void Label_SetTextAlign( LCUI_Widget *widget, TextAlignType align )
{
	LCUI_TextLayer *layer;
	layer = Label_GetTextLayer( widget );
	TextLayer_SetTextAlign( layer, align );
	Widget_Update( widget );
}

/** 为Label部件内显示的文本设定文本样式 */
LCUI_API int Label_SetTextStyle( LCUI_Widget *widget, LCUI_TextStyle style )
{
	LCUI_TextStyle *p_style;
	
	if( !widget ) {
		return -1;
	}
	p_style = (LCUI_TextStyle*)malloc( sizeof(LCUI_TextStyle) );
	if( !p_style ) {
		return -2;
	}
	*p_style = style;
	DEBUG_MSG("post LABEL_STYLE msg\n");
	WidgetMsg_Post( widget, LABEL_STYLE, p_style, TRUE, TRUE );
	return 0;
}

/** 获取Label部件的全局文本样式 */
LCUI_API void Label_GetTextStyle( LCUI_Widget *widget, LCUI_TextStyle *style )
{
	LCUI_TextLayer *layer;
	layer = Label_GetTextLayer( widget );
	*style = layer->text_style;
}

/** 启用或禁用Label部件的自动尺寸调整功能 */
LCUI_API void Label_SetAutoSize( LCUI_Widget *widget, LCUI_BOOL flag,
							AUTOSIZE_MODE mode )
{
	LCUI_Label *label;
	label = (LCUI_Label*)Widget_GetPrivateData( widget );
	label->auto_size = flag;
	label->mode = mode;
	Widget_Update( widget );
}
/*-------------------------- End Public ------------------------------*/

/** 注册label部件类型 */
void RegisterLabel(void)
{
	WidgetType_Add(WIDGET_LABEL);
	WidgetFunc_Add(WIDGET_LABEL, Label_OnInit, FUNC_TYPE_INIT);
	WidgetFunc_Add(WIDGET_LABEL, Label_OnPaint, FUNC_TYPE_PAINT);
	WidgetFunc_Add(WIDGET_LABEL, Label_OnUpdate, FUNC_TYPE_UPDATE);
	WidgetFunc_Add(WIDGET_LABEL, Label_OnDestroy, FUNC_TYPE_DESTROY);
}
