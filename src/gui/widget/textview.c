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
#include <LCUI/gui/widget/textview.h>

enum TaskType {
	TASK_SET_TEXT,
	TASK_SET_AUTOWRAP,
	TASK_SET_TEXT_ALIGN,
	TASK_UPDATE,
	TASK_TOTAL
};

typedef struct LCUI_TextViewRec_ {
	LCUI_TextStyle style;
	LCUI_BOOL has_content;		/**< 是否有设置 content 属性 */
	LCUI_Mutex mutex;		/**< 互斥锁 */
	LCUI_TextLayer layer;		/**< 文本图层 */
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

enum FontStyleKey {
	key_color,
	key_font_size,
	key_font_style,
	key_font_weight,
	key_font_family,
	key_line_height,
	key_text_align,
	key_content,
	key_white_space,
	TOTAL_FONT_STYLE_KEY
};

enum FontStyleType {
	FS_NORMAL,
	FS_ITALIC,
	FS_OBLIQUE
};

static struct LCUI_TextViewModule {
	LCUI_WidgetPrototype prototype;
	int keys[TOTAL_FONT_STYLE_KEY];
} self;

static int unescape( const wchar_t *instr, wchar_t *outstr )
{
	int i = -1;
	wchar_t buff[6];
	wchar_t *pout = outstr;
	const wchar_t *pin = instr;

	for( ; *pin; ++pin ) {
		if( i >= 0 ) {
			buff[i++] = *pin;
			if( i >= 4 ) {
				buff[i] = 0;
				swscanf( buff, L"%hx", pout );
				++pout;
				i = -1;
			}
			continue;
		}
		if( *pin == L'\\' ) {
			i = 0;
			continue;
		}
		*pout++ = *pin;
	}
	if( i >= 4 ) {
		buff[i] = 0;
		swscanf( buff, L"%hx", pout );
		++pout;
	}
	*pout = 0;
	return pout - outstr;
}

static int OnParseContent( LCUI_StyleSheet ss, int key, const char *str )
{
	int i;
	wchar_t *content;
	size_t len = strlen( str ) + 1;
	content = malloc( len * sizeof( wchar_t ) );
	LCUI_DecodeString( content, str, (int)len, ENCODING_UTF8 );
	if( content[0] == '"' ) {
		for( i = 0; content[i + 1]; ++i ) {
			content[i] = content[i + 1];
		}
		if( content[i - 1] != '"' ) {
			free( content );
			return -1;
		}
		content[i - 1] = 0;
	}
	unescape( content, content );
	SetStyle( ss, self.keys[key], content, wstring );
	return 0;
}

static int OnParseColor( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_Style s = &ss->sheet[self.keys[key]];
	if( ParseColor( s, str ) ) {
		return 0;
	}
	return -1;
}

static int OnParseFontSize( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_Style s = &ss->sheet[self.keys[key]];
	if( ParseNumber( s, str ) ) {
		return 0;
	}
	return -1;
}

static int OnParseFontFamily( LCUI_StyleSheet ss, int key, const char *str )
{
	char *name = strdup2( str );
	if( ss->sheet[self.keys[key]].is_valid
	 && ss->sheet[self.keys[key]].string) {
		free( ss->sheet[self.keys[key]].string );
	}
	SetStyle( ss, self.keys[key], name, string );
	return 0;
}

static int OnParseFontStyle( LCUI_StyleSheet ss, int key, const char *str )
{
	if( strcmp(str, "normal") == 0 ) {
		SetStyle( ss, self.keys[key], 0, int );
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
	int val = LCUI_GetStyleValue( str );
	if( val < 0 ) {
		return -1;
	}
	SetStyle( ss, self.keys[key], val, style );
	return 0;
}

static int OnParseLineHeight( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_StyleRec sv;
	if( !ParseNumber(&sv, str) ) {
		return -1;
	}
	ss->sheet[self.keys[key]] = sv;
	return 0;
}

static int OnParseStyleOption( LCUI_StyleSheet ss, int key, const char *str )
{
	LCUI_Style s = &ss->sheet[self.keys[key]];
	int v = LCUI_GetStyleValue( str );
	if( v < 0 ) {
		return -1;
	}
	s->style = v;
	s->type = SVT_STYLE;
	s->is_valid = TRUE;
	return 0;
}

static LCUI_StyleParserRec style_parsers[] = {
	{ key_color, "color", OnParseColor },
	{ key_font_family, "font-family", OnParseFontFamily },
	{ key_font_size, "font-size", OnParseFontSize },
	{ key_font_style, "font-style", OnParseFontStyle },
	{ key_font_weight, "font-weight", OnParseFontWeight },
	{ key_text_align, "text-align", OnParseTextAlign },
	{ key_line_height, "line-height", OnParseLineHeight },
	{ key_content, "content", OnParseContent },
	{ key_white_space, "white-space", OnParseStyleOption }
};

static void TextView_UpdateStyle( LCUI_Widget w )
{
	int i, value;
	LCUI_Style s;
	LCUI_TextView txt = Widget_GetData( w, self.prototype );
	LCUI_TextStyle *ts = &txt->style;
	TextStyle_Init( ts );
	for( i = 0; i < TOTAL_FONT_STYLE_KEY; ++i ) {
		if( self.keys[i] < 0 ) {
			continue;
		}
		s = &w->style->sheet[self.keys[i]];
		switch( i ) {
		case key_color:
			if( s->is_valid ) {
				ts->fore_color = s->color;
				ts->has_fore_color = TRUE;
			} else {
				ts->has_fore_color = FALSE;
			}
			break;
		case key_font_family:
			if( s->is_valid ) {
				TextStyle_SetFont( &txt->style, s->string );
			} else {
				TextStyle_SetFont( &txt->style, NULL );
			}
			break;
		case key_font_size:
			if( !s->is_valid ) {
				ts->pixel_size = 14;
				ts->has_pixel_size = TRUE;
				break;
			}
			value = LCUIMetrics_ComputeActual( s->value, s->type );
			ts->pixel_size = max( 12, value );
			ts->has_pixel_size = TRUE;
			break;
		case key_font_style:
			ts->has_style = FALSE;
			if( s->is_valid ) {
				ts->style = s->value;
				ts->has_style = TRUE;
			}
			break;
		case key_font_weight:
			break;
		case key_text_align:
			if( s->is_valid ) {
				TextLayer_SetTextAlign( txt->layer, s->style );
			} else {
				TextLayer_SetTextAlign( txt->layer, SV_LEFT );
			}
			break;
		case key_line_height:
			if( !s->is_valid ) {
				LCUI_StyleRec style;
				style.val_scale = 1.5;
				style.type = SVT_SCALE;
				style.is_valid = TRUE;
				TextLayer_SetLineHeight( txt->layer, &style );
				break;
			}
			switch( s->type ) {
			case SVT_SCALE:
			case SVT_PX: break;
			default:
				s->val_px = LCUIMetrics_ComputeActual( s->value, s->type );
				s->type = SVT_PX;
				break;
			}
			TextLayer_SetLineHeight( txt->layer, s );
			break;
		case key_content:
			if( s->is_valid ) {
				txt->has_content = TRUE;
				TextView_SetTextW( w, s->wstring );
			} else {
				if( txt->has_content ) {
					txt->has_content = FALSE;
					TextView_SetTextW( w, NULL );
				}
			}
			break;
		case key_white_space:
			if( !s->is_valid || s->type != SVT_STYLE ) {
				TextLayer_SetAutoWrap( txt->layer, TRUE );
				break;
			}
			if( s->style == SV_NOWRAP ) {
				TextLayer_SetAutoWrap( txt->layer, FALSE );
			} else {
				TextLayer_SetAutoWrap( txt->layer, TRUE );
			}
		default:break;
		}
	}
	TextLayer_SetTextStyle( txt->layer, ts );
	txt->tasks[TASK_UPDATE].is_valid = TRUE;
	Widget_AddTask( w, WTT_USER );
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
	txt = Widget_AddData( w, self.prototype, sizeof( LCUI_TextViewRec ) );
	TextStyle_Init( &txt->style );
	for( i = 0; i < TASK_TOTAL; ++i ) {
		txt->tasks[i].is_valid = FALSE;
	}
	txt->has_content = FALSE;
	/* 初始化文本图层 */
	txt->layer = TextLayer_New();
	/* 启用多行文本显示 */
	TextLayer_SetAutoWrap( txt->layer, TRUE );
	TextLayer_SetMultiline( txt->layer, TRUE );
	/* 启用样式标签的支持 */
	TextLayer_SetUsingStyleTags( txt->layer, TRUE );
	Widget_BindEvent( w, "resize", TextView_OnResize, NULL, NULL );
	LCUIMutex_Init( &txt->mutex );
}

/** 释放 TextView 部件占用的资源 */
static void TextView_OnDestroy( LCUI_Widget w )
{
	LCUI_TextView txt = Widget_GetData( w, self.prototype );
	TextLayer_Destroy( txt->layer );
	LCUIMutex_Unlock( &txt->mutex );
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
	LCUI_Rect content_rect, rect;
	LCUI_TextView txt;

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

static void TextView_OnParseText( LCUI_Widget w, const char *text )
{
	TextView_SetText( w, text );
}

void TextView_SetLineHeight( LCUI_Widget w, LCUI_Style val )
{
	LCUI_TextView txt = Widget_GetData( w, self.prototype );
	TextLayer_SetLineHeight( txt->layer, val );
	txt->tasks[TASK_UPDATE].is_valid = TRUE;
	Widget_AddTask( w, WTT_USER );
}

void TextView_SetTextStyle( LCUI_Widget w, LCUI_TextStyle *style )
{
	LCUI_TextView txt = Widget_GetData( w, self.prototype );
	TextLayer_SetTextStyle( txt->layer, style );
	txt->tasks[TASK_UPDATE].is_valid = TRUE;
	Widget_AddTask( w, WTT_USER );
}

void TextView_GetTextStyle( LCUI_Widget w, LCUI_TextStyle *style )
{
	LCUI_TextView txt = Widget_GetData( w, self.prototype );
	*style = txt->layer->text_style;
}

void TextView_SetTextAlign( LCUI_Widget w, int align )
{
	LCUI_TextView txt = Widget_GetData( w, self.prototype );
	txt->tasks[TASK_SET_TEXT_ALIGN].align = align;
	txt->tasks[TASK_SET_TEXT_ALIGN].is_valid = TRUE;
	Widget_AddTask( w, WTT_USER );
}

void TextView_SetAutoWrap( LCUI_Widget w, LCUI_BOOL autowrap )
{
	LCUI_TextView txt = Widget_GetData( w, self.prototype );
	txt->tasks[TASK_SET_AUTOWRAP].autowrap = autowrap;
	txt->tasks[TASK_SET_AUTOWRAP].is_valid = TRUE;
	Widget_AddTask( w, WTT_USER );
}

/*-------------------------- End Public ------------------------------*/

/** 添加 TextView 部件类型 */
void LCUIWidget_AddTextView( void )
{
	int i;
	self.prototype = LCUIWidget_NewPrototype( "textview", NULL );
	self.prototype->init = TextView_OnInit;
	self.prototype->paint = TextView_OnPaint;
	self.prototype->destroy = TextView_OnDestroy;
	self.prototype->autosize = TextView_AutoSize;
	self.prototype->update = TextView_UpdateStyle;
	self.prototype->settext = TextView_OnParseText;
	self.prototype->runtask = TextView_OnTask;
	for( i = 0; i < TOTAL_FONT_STYLE_KEY; ++i ) {
		LCUI_StyleParser parser = &style_parsers[i];
		self.keys[parser->key] = LCUI_AddStyleName( parser->name );
		LCUI_AddCSSParser( parser );
	}
}
