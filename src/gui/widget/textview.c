/* ***************************************************************************
 * textview.c -- LCUI's TextView Widget
 *
 * Copyright (C) 2015 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2015 归属于 刘超 <lc-soft@live.cn>
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
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>

enum TaskType {
	TASK_SET_TEXT,
	TASK_SET_AUTOWRAP,
	TASK_SET_TEXT_ALIGN,
	TASK_UPDATE,
	TASK_TOTAL
};

typedef struct LCUI_TextView_ {
	LCUI_TextStyle style;
	LCUI_TextLayer layer;	/**< 文本图层 */
	struct {
		LCUI_BOOL is_valid;
		union {
			wchar_t *text;
			LCUI_BOOL autowrap;
			int align;
		};
	} tasks[TASK_TOTAL];
} LCUI_TextView;

/*---------------------------- Private -------------------------------*/

enum FontStyleKey {
	key_color,
	key_font_size,
	key_font_style,
	key_font_weight,
	key_font_family,
	key_line_height,
	key_text_align,
	key_content,
	TOTAL_FONT_STYLE_KEY
};

enum FontStyleType {
	FS_NORMAL,
	FS_ITALIC,
	FS_OBLIQUE
};

static int style_key_map[TOTAL_FONT_STYLE_KEY];

static int unescape( const wchar_t *instr, wchar_t *outstr )
{
	int i = -1;
	char buff[6];
	wchar_t *pout = outstr;
	const wchar_t *pin = instr;

	while( *pin++ ) {
		if( i >= 0 ) {
			buff[i++] = *pin;
			if( i >= 4 ) {
				sscanf( buff, "%x", pout );
				++pout;
				i = -1;
			}
			continue;
		}
		if( *pin == '\\' ) {
			i = 0;
			continue;
		}
		*pout++ = *pin;
	}
	if( i >= 4 ) {
		sscanf( buff, "%x", pout );
		++pout;
	}
	*pout = 0;
	return pout - outstr;
}

static int OnParseContent( LCUI_StyleSheet ss, int key, const char *str )
{
	wchar_t *content;
	LCUICharset_UTF8ToUnicode( str, &content );
	unescape( content, content );
	SetStyle( ss, style_key_map[key], content, wstring );
	return 0;
}

static int OnParseColor( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_Style *s = &ss->sheet[style_key_map[key]];
	if( ParseColor( s, str ) ) {
		return 0;
	}
	return -1;
}

static int OnParseFontSize( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_Style *s = &ss->sheet[style_key_map[key]];
	if( ParseNumber( s, str ) ) {
		return 0;
	}
	return -1;
}

static int OnParseFontFamily( LCUI_StyleSheet ss, int key, const char *str )
{
	if( ss->sheet[style_key_map[key]].is_valid
	 && ss->sheet[style_key_map[key]].string) {
		free( ss->sheet[style_key_map[key]].string );
	}
	SetStyle( ss, style_key_map[key], strdup(str), string );
	return 0;
}

static int OnParseFontStyle( LCUI_StyleSheet ss, int key, const char *str )
{
	if( strcmp(str, "normal") == 0 ) {
		SetStyle( ss, style_key_map[key], 0, int );
		return 0;
	}
	return -1;
}

static int OnParseFontWeight( LCUI_StyleSheet ss, int key, const char *str )
{
	return -1;
}

static int OnParseTextAlign( LCUI_StyleSheet ss, int key, const char *str )
{
	int val = ParseStyleOption( str );
	if( val < 0 ) {
		return -1;
	}
	SetStyle( ss, style_key_map[key], val, style );
	return 0;
}

static int OnParseLineHeight( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_Style sv;
	if( !ParseNumber(&sv, str) ) {
		return -1;
	}
	if( sv.type == SVT_PX || sv.type == SVT_SCALE ) {
		ss->sheet[style_key_map[key]] = sv;
		return 0;
	}
	return -1;
}

static LCUI_StyleParser style_parsers[] = { 
	{ key_color, "color", OnParseColor },
	{ key_font_family, "font-family", OnParseFontFamily },
	{ key_font_size, "font-size", OnParseFontSize },
	{ key_font_style, "font-style", OnParseFontStyle },
	{ key_font_weight, "font-weight", OnParseFontWeight },
	{ key_text_align, "text-align", OnParseTextAlign },
	{ key_line_height, "line-height", OnParseLineHeight },
	{ key_content, "content", OnParseContent },
};

static void TextView_UpdateStyle( LCUI_Widget w )
{
	int i;
	LCUI_Style *s;
	LCUI_TextView *txt = (LCUI_TextView*)w->private_data;

	TextStyle_Init( &txt->style );
	for( i = 0; i < TOTAL_FONT_STYLE_KEY; ++i ) {
		if( style_key_map[i] < 0 ) {
			continue;
		}
		s = &w->style->sheet[style_key_map[i]];
		if( !s->is_valid ) {
			continue;
		}
		switch( i ) {
		case key_color:
			txt->style.fore_color = s->color;
			txt->style.has_fore_color = TRUE;
			break;
		case key_font_family:
			TextStyle_SetFont( &txt->style, s->string );
			break;
		case key_font_size:
			if( s->type == SVT_PX ) {
				txt->style.pixel_size = s->px;
			} else if( s->type == SVT_PT ) {
				// ...
			}
			txt->style.has_pixel_size = TRUE;
			break;
		case key_font_style:
			txt->style.style = s->value;
			txt->style.has_style = TRUE;
			break;
		case key_font_weight:
			break;
		case key_text_align:
			TextLayer_SetTextAlign( txt->layer, s->style );
			break;
		case key_line_height:
			TextLayer_SetLineHeight( txt->layer, s );
			break;
		case key_content:
			TextView_SetTextW( w, s->wstring );
		default:break;
		}
	}
	TextLayer_SetTextStyle( txt->layer, &txt->style );
	txt->tasks[TASK_UPDATE].is_valid = TRUE;
	Widget_AddTask( w, WTT_USER );
}

static void TextView_OnResize( LCUI_Widget w, LCUI_WidgetEvent *e, void *arg )
{
	LinkedList rects;
	LinkedListNode *node;
	LCUI_Size new_size = {16, 16};
	LCUI_TextView *txt = (LCUI_TextView*)w->private_data;
	
	DEBUG_MSG("on resize\n");
	if( w->width > new_size.w ) {
		new_size.w = w->width;
	}
	if( w->height > new_size.h ) {
		new_size.h = w->height;
	}
	LinkedList_Init( &rects );
	TextLayer_SetMaxSize( txt->layer, new_size );
	TextLayer_Update( txt->layer, &rects );
	LinkedList_ForEach( node, &rects ) {
		Widget_InvalidateArea( w, node->data, SV_CONTENT_BOX );
	}
	LinkedList_Clear( &rects, free );
	TextLayer_ClearInvalidRect( txt->layer );
}

/** 初始化 TextView 部件数据 */
static void TextView_OnInit( LCUI_Widget w )
{
	int i;
	LCUI_TextView *txt;

	txt = Widget_NewPrivateData( w, LCUI_TextView );
	TextStyle_Init( &txt->style );
	for( i = 0; i < TASK_TOTAL; ++i ) {
		txt->tasks[i].is_valid = FALSE;
	}
	/* 初始化文本图层 */
	txt->layer = TextLayer_New();
	/* 启用多行文本显示 */
	TextLayer_SetMultiline( txt->layer, TRUE );
	/* 启用样式标签的支持 */
	TextLayer_SetUsingStyleTags( txt->layer, TRUE );
	Widget_BindEvent( w, "resize", TextView_OnResize, NULL, NULL );
}

/** 释放 TextView 部件占用的资源 */
static void TextView_OnDestroy( LCUI_Widget w )
{
	LCUI_TextView *txt;
	txt = (LCUI_TextView*)w->private_data;
	TextLayer_Destroy( &txt->layer );
}

static void TextView_AutoSize( LCUI_Widget w, int *width, int *height )
{
	LCUI_TextView *txt = (LCUI_TextView*)w->private_data;
	*width = TextLayer_GetWidth( txt->layer );
	*height = TextLayer_GetHeight( txt->layer );
	DEBUG_MSG("width: %d, height: %d\n", *width, *height);
}

/** 私有的任务处理接口 */
static void TextView_OnTask( LCUI_Widget w )
{
	int i;
	LinkedList rects;
	LinkedListNode *node;
	LCUI_TextView *txt = (LCUI_TextView*)w->private_data;

	LinkedList_Init( &rects );
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
	if( !txt->tasks[i].is_valid ) {
		return;
	}
	txt->tasks[i].is_valid = FALSE;
	txt = (LCUI_TextView*)w->private_data;
	LinkedList_Init( &rects );
	TextLayer_Update( txt->layer, &rects );
	LinkedList_ForEach( node, &rects ) {
		Widget_InvalidateArea( w, node->data, SV_CONTENT_BOX );
	}
	LinkedList_Clear( &rects, free );
	TextLayer_ClearInvalidRect( txt->layer );
	if( w->computed_style.w.type == SVT_AUTO
	 || w->computed_style.h.type == SVT_AUTO ) {
		Widget_AddTask( w, WTT_RESIZE );
	}
}

/** 绘制 TextView 部件 */
static void TextView_OnPaint( LCUI_Widget w, LCUI_PaintContext paint )
{
	LCUI_TextView *txt;
	LCUI_Rect content_rect, rect;
	LCUI_Pos layer_pos;

	txt = (LCUI_TextView*)w->private_data;
	content_rect.x = w->box.content.left - w->box.graph.left;
	content_rect.y = w->box.content.top - w->box.graph.top;
	content_rect.width = w->box.content.width;
	content_rect.height = w->box.content.height;
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
int TextView_SetTextW( LCUI_Widget w, const wchar_t *text )
{
	int len;
	wchar_t *text_ptr;
	LCUI_TextView *txt;

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
	Widget_AddTask( w, WTT_USER );
	Widget_Unlock( w );
	return 0;
}

int TextView_SetText( LCUI_Widget w, const char *utf8_text )
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

void TextView_SetLineHeight( LCUI_Widget w, LCUI_Style *val )
{
	LCUI_TextView *txt = (LCUI_TextView*)w->private_data;
	TextLayer_SetLineHeight( txt->layer, val );
	txt->tasks[TASK_UPDATE].is_valid = TRUE;
	Widget_AddTask( w, WTT_USER );
}

void TextView_SetTextStyle( LCUI_Widget w, LCUI_TextStyle *style ) 
{
	LCUI_TextView *txt = (LCUI_TextView*)w->private_data;
	TextLayer_SetTextStyle( txt->layer, style );
	txt->tasks[TASK_UPDATE].is_valid = TRUE;
	Widget_AddTask( w, WTT_USER );
}

void TextView_GetTextStyle( LCUI_Widget w, LCUI_TextStyle *style )
{
	LCUI_TextView *txt = (LCUI_TextView*)w->private_data;
	*style = txt->layer->text_style;
}

void TextView_SetTextAlign( LCUI_Widget w, int align )
{
	LCUI_TextView *txt = (LCUI_TextView*)w->private_data;
	txt->tasks[TASK_SET_TEXT_ALIGN].align = align;
	txt->tasks[TASK_SET_TEXT_ALIGN].is_valid = TRUE;
	Widget_AddTask( w, WTT_USER );
}

/*-------------------------- End Public ------------------------------*/

/** 添加 TextView 部件类型 */
void LCUIWidget_AddTextView( void )
{
	int i;
	LCUI_WidgetClass *wc = LCUIWidget_AddClass( "textview" );
	wc->methods.init = TextView_OnInit;
	wc->methods.paint = TextView_OnPaint;
	wc->methods.destroy = TextView_OnDestroy;
	wc->methods.autosize = TextView_AutoSize;
	wc->methods.update = TextView_UpdateStyle;
	wc->methods.set_text = TextView_SetText;
	wc->task_handler = TextView_OnTask;
	
	for( i = 0; i < TOTAL_FONT_STYLE_KEY; ++i ) {
		style_key_map[style_parsers[i].key] = 
		LCUICSSParser_Register( &style_parsers[i] );
	}
}
