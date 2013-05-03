/* ***************************************************************************
 * label.c -- LCUI's Label widget
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
 * label.c -- LCUI 的文本标签部件
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

//#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_MISC_H 
#include LC_GRAPH_H
#include LC_LABEL_H

/*---------------------------- Private -------------------------------*/
/* 初始化label部件数据 */
static void 
Label_ExecInit( LCUI_Widget *widget )
{
	LCUI_Label *label;
	/* label部件不需要焦点 */
	widget->focus = FALSE;
	label = WidgetPrivData_New( widget, sizeof(LCUI_Label) );
	label->auto_size = TRUE;
	/* 初始化文本块缓冲队列 */
	label->text_buff = NULL;
	/* 初始化文本图层 */
	TextLayer_Init( &label->layer ); 
	/* 启用多行文本显示 */
	TextLayer_Multiline( &label->layer, TRUE );
	Widget_SetAutoSize( widget, FALSE, 0 );
	/* 启用样式标签的支持 */
	TextLayer_UsingStyleTags( &label->layer, TRUE );
}

/* 释放label部件占用的资源 */
static void 
Destroy_Label( LCUI_Widget *widget )
{
	LCUI_Label *label;
	
	label = Widget_GetPrivData( widget );
	Destroy_TextLayer( &label->layer );
}

/* 刷新label部件内的字体位图 */
static void 
Refresh_Label_FontBitmap( LCUI_Widget *widget )
{
	LCUI_Label *label;
	
	label = Widget_GetPrivData( widget );
	TextLayer_Refresh( &label->layer ); 
}

/* 更新label部件 */
static void 
Label_ExecUpdate( LCUI_Widget *widget )
{
	int mode; 
	LCUI_Size max;
	LCUI_Label *label;
	
	if( widget == NULL ) {
		return;
	}
	label = Widget_GetPrivData( widget );
	if( label == NULL ) {
		return;
	}
	if(!Graph_IsValid(&widget->background.image)) {
		mode = GRAPH_MIX_FLAG_REPLACE; /* 替换模式 */
	} else {
		mode = GRAPH_MIX_FLAG_OVERLAY; /* 叠加模式 */ 
	}
	if( label->text_buff != NULL ) {
		TextLayer_TextW( &label->layer, label->text_buff );
		free( label->text_buff );
		label->text_buff = NULL;
	}
	/* 先绘制文本位图，在绘制前它会更新位图尺寸 */
	TextLayer_Draw( widget, &label->layer, mode );
	max = TextLayer_GetSize( &label->layer ); /* 获取尺寸 */
	if( widget->dock == DOCK_TYPE_NONE && label->auto_size
	 && Size_Cmp( max, widget->size ) != 0 ) {
		/* 如果开启了自动调整大小,并且尺寸有改变 */ 
		Widget_Resize(widget, max );
		Widget_Refresh(widget);
	}
}

/* 重绘Label部件 */
static void 
Label_ExecDraw( LCUI_Widget *widget )
{
	Refresh_Label_FontBitmap( widget );
	Label_ExecUpdate( widget );
}

/*-------------------------- End Private -----------------------------*/

/*---------------------------- Public --------------------------------*/
/* 设定与标签关联的文本内容 */
LCUI_API int
Label_TextW( LCUI_Widget *widget, const wchar_t *unicode_text )
{
	int len;
	LCUI_Label *label;
	
	if( widget == NULL ) {
		return -1;
	}
	if( unicode_text == NULL ) {
		len = 0;
	} else {
		len = wcslen(unicode_text);
	}
	label = Widget_GetPrivData( widget );
	if( label == NULL ) {
		return -2;
	}
	if( label->text_buff != NULL ) {
		label->text_buff = realloc( label->text_buff, 
				sizeof(wchar_t)*(len+1) );
	} else {
		label->text_buff = malloc( sizeof(wchar_t)*(len+1) );
	}
	if( label->text_buff == NULL ) {
		return -1;
	}
	if( unicode_text == NULL ) {
		label->text_buff[0] = '\0';
	} else {
		wcscpy( label->text_buff, unicode_text );
	}
	Widget_Update( widget );
	return 0;
}

LCUI_API void
Label_Text( LCUI_Widget *widget, const char *utf8_text )
{
	wchar_t *unicode_text;
	LCUICharset_UTF8ToUnicode( utf8_text, &unicode_text );
	Label_TextW( widget, unicode_text );
	if( unicode_text != NULL ) {
		free( unicode_text );
	}
}

LCUI_API void
Label_TextA( LCUI_Widget *widget, const char *ascii_text )
{
	wchar_t *unicode_text;
	LCUICharset_ASCIIToUnicode( ascii_text, &unicode_text );
	Label_TextW( widget, unicode_text );
	if( unicode_text != NULL ) {
		free( unicode_text );
	}
}

/* 为Label部件内显示的文本设定文本样式 */
LCUI_API int
Label_TextStyle( LCUI_Widget *widget, LCUI_TextStyle style )
{
	LCUI_Label *label;
	
	label = Widget_GetPrivData( widget );
	TextLayer_Text_SetDefaultStyle( &label->layer, style );
	Widget_Draw( widget ); 
	return 0;
}

/* 获取Label部件的文本样式 */
LCUI_API LCUI_TextStyle
Label_GetTextStyle( LCUI_Widget *widget )
{
	LCUI_TextLayer *layer;
	layer = Label_GetTextLayer( widget );
	return layer->default_data;
}

/* 获取label部件内的文本图层的指针 */
LCUI_API LCUI_TextLayer*
Label_GetTextLayer( LCUI_Widget *widget )
{
	LCUI_Label *label;
	
	label = Widget_GetPrivData( widget );
	return &label->layer;
}

/* 刷新label部件显示的文本 */
LCUI_API void
Label_Refresh( LCUI_Widget *widget )
{
	LCUI_TextLayer *layer;
	layer = Label_GetTextLayer( widget );
	TextLayer_Refresh( layer );
}

/* 启用或禁用Label部件的自动尺寸调整功能 */
LCUI_API void
Label_AutoSize( LCUI_Widget *widget, LCUI_BOOL flag, AUTOSIZE_MODE mode )
{
	LCUI_Label *label;
	
	label = Widget_GetPrivData( widget );
	label->auto_size = flag;
	label->mode = mode;
	Widget_Update( widget );
}
/*-------------------------- End Public ------------------------------*/


/* 注册label部件类型 */
LCUI_API void
Register_Label(void)
{
	WidgetType_Add("label");
	WidgetFunc_Add("label",	Label_ExecInit,	FUNC_TYPE_INIT);
	WidgetFunc_Add("label",	Label_ExecDraw,	FUNC_TYPE_DRAW); 
	WidgetFunc_Add("label",	Label_ExecUpdate, FUNC_TYPE_UPDATE); 
	WidgetFunc_Add("label", Destroy_Label,	FUNC_TYPE_DESTROY);
}
