/* ***************************************************************************
 * display.c -- graphical display control
 *
 * Copyright (C) 2012-2017 by Liu Chao <lc-soft@live.cn>
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
 * display.c -- 图形显示控制
 *
 * 版权所有 (C) 2012-2017 归属于 刘超 <lc-soft@live.cn>
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
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/input.h>
#include <LCUI/timer.h>
#include <LCUI/cursor.h>
#include <LCUI/thread.h>
#include <LCUI/display.h>
#include <LCUI/platform.h>
#include LCUI_DISPLAY_H

#define DEFAULT_WIDTH	800
#define DEFAULT_HEIGHT	600

/** surface 记录 */
typedef struct SurfaceRecordRec_ {
	LCUI_BOOL rendered;		/**< 是否已渲染了新内容 */
	LinkedList rects;		/**< 需重绘的区域列表 */
	LCUI_Surface surface;		/**< surface */
	LCUI_Widget widget;		/**< surface 所映射的 widget */
} SurfaceRecordRec, *SurfaceRecord;

/** 图形显示功能的上下文数据 */
static struct DisplayContext {
	int mode;			/**< 显示模式 */
	LCUI_BOOL show_rect_border;	/**< 是否为重绘的区域显示边框 */
	LCUI_BOOL is_working;		/**< 标志，指示当前模块是否处于工作状态 */
	LCUI_Thread thread;		/**< 线程，负责画面更新工作 */
	LinkedList surfaces;		/**< surface 列表 */
	LinkedList rects;		/**< 无效区域列表 */
	LCUI_DisplayDriver driver;
} display;

static void DrawBorder( LCUI_PaintContext paint )
{
	LCUI_Pos pos;
	LCUI_Color color;
	int end_x = paint->rect.width - 1;
	int end_y = paint->rect.height - 1;
	pos.x = pos.y = 0;
	color = RGB( 255, 0, 0 );
	Graph_DrawHorizLine( &paint->canvas, color, 1, pos, end_x );
	Graph_DrawVertiLine( &paint->canvas, color, 1, pos, end_y );
	pos.x = paint->rect.width - 1;
	Graph_DrawVertiLine( &paint->canvas, color, 1, pos, end_y );
	pos.x = 0;
	pos.y = paint->rect.height - 1;
	Graph_DrawHorizLine( &paint->canvas, color, 1, pos, end_x );
}

void LCUIDisplay_Update( void )
{
	LCUI_Surface surface;
	LinkedListNode *node;
	SurfaceRecord record = NULL;

	if( !display.is_working ) {
		return;
	}
	LCUICursor_Update();
	LCUIWidget_Update();
	/* 遍历当前的 surface 记录列表 */
	for( LinkedList_Each( node, &display.surfaces ) ) {
		record = node->data;
		surface = record->surface;
		if( !record->widget || !surface ||
		    !Surface_IsReady( surface ) ) {
			continue;
		}
		Surface_Update( surface );
		/* 收集无效区域记录 */
		Widget_ProcInvalidArea( record->widget, &record->rects );
	}
	if( display.mode == LCDM_SEAMLESS || !record ) {
		return;
	}
	LinkedList_Concat( &record->rects, &display.rects );
}

void LCUIDisplay_Render( void )
{
	LinkedListNode *sn, *rn;
	LCUI_PaintContext paint;

	if( !display.is_working ) {
		return;
	}
	/* 遍历当前的 surface 记录列表 */
	for( LinkedList_Each( sn, &display.surfaces ) ) {
		SurfaceRecord record = sn->data;
		LCUI_Surface surface = record->surface;

		if( !record->widget || !surface ||
		    !Surface_IsReady( surface ) ) {
			continue;
		}
		record->rendered = FALSE;
		/* 在 surface 上逐个重绘无效区域 */
		for( LinkedList_Each( rn, &record->rects ) ) {
			paint = Surface_BeginPaint( surface, rn->data );
			if( !paint ) {
				continue;
			}
			DEBUG_MSG( "[%s]: render rect: (%d,%d,%d,%d)\n",
				   record->widget->type,
				   paint->rect.x, paint->rect.y,
				   paint->rect.width, paint->rect.height );
			Widget_Render( record->widget, paint );
			if( display.show_rect_border ) {
				DrawBorder( paint );
			}
			Surface_EndPaint( surface, paint );
			record->rendered = TRUE;
		}
		RectList_Clear( &record->rects );
	}
}

void LCUIDisplay_Present( void )
{
	LinkedListNode *sn;
	if( !display.is_working ) {
		return;
	}
	for( LinkedList_Each( sn, &display.surfaces ) ) {
		SurfaceRecord record = sn->data;
		LCUI_Surface surface = record->surface;
		if( !surface || !Surface_IsReady( surface ) ) {
			continue;
		}
		if( record->rendered ) {
			Surface_Present( surface );
		}
	}
}

void LCUIDisplay_InvalidateArea( LCUI_Rect *rect )
{
	if( !display.is_working ) {
		return;
	}
	RectList_Add( &display.rects, rect );
}

static LCUI_Widget LCUIDisplay_GetBindWidget( LCUI_Surface surface )
{
	LinkedListNode *node;
	for( LinkedList_Each( node, &display.surfaces ) ) {
		SurfaceRecord record = node->data;
		if( record && record->surface == surface ) {
			return record->widget;
		}
	}
	return NULL;
}

static LCUI_Surface LCUIDisplay_GetBindSurface( LCUI_Widget widget )
{
	LinkedListNode *node;
	for( LinkedList_Each( node, &display.surfaces ) ) {
		SurfaceRecord record = node->data;
		if( record && record->widget == widget ) {
			return record->surface;
		}
	}
	return NULL;
}

LCUI_Surface LCUIDisplay_GetSurfaceOwner( LCUI_Widget w )
{
	if( LCUIDisplay_GetMode() == LCDM_SEAMLESS ) {
		while( w->parent ) {
			w = w->parent;
		}
	} else {
		w = LCUIWidget_GetRoot();
	}
	return LCUIDisplay_GetBindSurface( w );
}

/** 将 widget 与 sruface 进行绑定 */
static void LCUIDisplay_BindSurface( LCUI_Widget widget )
{
	LCUI_RectF *rect;
	SurfaceRecord record;
	int width, height;

	if( LCUIDisplay_GetBindSurface(widget) ) {
		return;
	}
	rect = &widget->box.graph;
	record = NEW( SurfaceRecordRec, 1 );
	record->surface = Surface_New();
	record->widget = widget;
	record->rendered = FALSE;
	LinkedList_Init( &record->rects );
	Surface_SetCaptionW( record->surface, widget->title );
	if( widget->style->sheet[key_top].is_valid &&
	    widget->style->sheet[key_left].is_valid ) {
		Surface_Move( record->surface, rect->x, rect->y );
	}
	width = roundi( rect->width );
	height = roundi( rect->height );
	Surface_Resize( record->surface, width, height );
	if( widget->computed_style.visible ) {
		Surface_Show( record->surface );
	} else {
		Surface_Hide( record->surface );
	}
	Widget_InvalidateArea( widget, NULL, SV_GRAPH_BOX );
	LinkedList_Append( &display.surfaces, record );
}

/** 解除 widget 与 sruface 的绑定 */
static void LCUIDisplay_UnbindSurface( LCUI_Widget widget )
{
	LinkedListNode *node;
	for( LinkedList_Each( node, &display.surfaces ) ) {
		SurfaceRecord record = node->data;
		if( record && record->widget == widget ) {
			Surface_Delete( record->surface );
			LinkedList_DeleteNode( &display.surfaces, node );
			break;
		}
	}
}

static void LCUIDisplay_CleanSurfaces( void )
{
	LinkedListNode *node;
	for( LinkedList_Each( node, &display.surfaces ) ) {
		SurfaceRecord record = node->data;
		Surface_Delete( record->surface );
		LinkedList_DeleteNode( &display.surfaces, node );
	}
}

static int LCUIDisplay_Windowed( void )
{
	LCUI_Widget root = LCUIWidget_GetRoot();
	switch( display.mode ) {
	case LCDM_WINDOWED:
		return 0;
	case LCDM_FULLSCREEN:
		break;
	case LCDM_SEAMLESS:
	default:
		LCUIDisplay_CleanSurfaces();
		LCUIDisplay_BindSurface( root );
		break;
	}
	Widget_Show( root );
	Widget_Resize( root, DEFAULT_WIDTH, DEFAULT_HEIGHT );
	display.mode = LCDM_WINDOWED;
	return 0;
}

static int LCUIDisplay_FullScreen( void )
{
	LCUI_Widget root = LCUIWidget_GetRoot();
	switch( display.mode ) {
	case LCDM_SEAMLESS:
		LCUIDisplay_CleanSurfaces();
		LCUIDisplay_BindSurface(root );
	case LCDM_WINDOWED:
	default: break;
	case LCDM_FULLSCREEN:
		return 0;
	}
	display.mode = LCDM_FULLSCREEN;
	LCUIDisplay_SetSize( LCUIDisplay_GetWidth(), LCUIDisplay_GetHeight() );
	return 0;
}

static int LCUIDisplay_Seamless( void )
{
	LinkedListNode *node;
	LCUI_Widget root = LCUIWidget_GetRoot();
	DEBUG_MSG("display.mode: %d\n", display.mode);
	switch( display.mode ) {
	case LCDM_SEAMLESS:
		return 0;
	case LCDM_FULLSCREEN:
	case LCDM_WINDOWED:
	default:
		LCUIDisplay_CleanSurfaces();
		break;
	}
	for( LinkedList_Each( node, &root->children ) ) {
		LCUIDisplay_BindSurface( node->data );
	}
	display.mode = LCDM_SEAMLESS;
	return 0;
}

/* 设置呈现模式 */
int LCUIDisplay_SetMode( int mode )
{
	int ret;
	DEBUG_MSG("mode: %d\n", mode);
	switch( mode ) {
	case LCDM_WINDOWED:
		ret = LCUIDisplay_Windowed();
		break;
	case LCDM_SEAMLESS:
		ret = LCUIDisplay_Seamless();
		break;
	case LCDM_FULLSCREEN:
	default:
		ret = LCUIDisplay_FullScreen();
		break;
	}
	return ret;
}

/* 获取呈现模式 */
int LCUIDisplay_GetMode(void)
{
	return display.mode;
}

void LCUIDisplay_ShowRectBorder(void)
{
	display.show_rect_border = TRUE;
}

void LCUIDisplay_HideRectBorder( void )
{
	display.show_rect_border = FALSE;
}

/** 设置显示区域的尺寸，仅在窗口化、全屏模式下有效 */
void LCUIDisplay_SetSize( int width, int height )
{
	LCUI_Widget root;
	if( display.mode == LCDM_SEAMLESS ) {
		return;
	}
	root = LCUIWidget_GetRoot();
	Widget_Resize( root, width, height );
}

int LCUIDisplay_GetWidth( void )
{
	if( !display.is_working ) {
		return 0;
	}
	if( display.mode == LCDM_WINDOWED ||
	    display.mode == LCDM_FULLSCREEN ) {
		return LCUIWidget_GetRoot()->width;
	}
	return display.driver->getWidth();
}

int LCUIDisplay_GetHeight( void )
{
	if( !display.is_working ) {
		return 0;
	}
	if( display.mode == LCDM_WINDOWED ||
	    display.mode == LCDM_FULLSCREEN ) {
		return LCUIWidget_GetRoot()->height;
	}
	return display.driver->getHeight();
}

void Surface_Delete( LCUI_Surface surface )
{
	if( display.is_working ) {
		display.driver->destroy( surface );
	}
}

LCUI_Surface Surface_New( void )
{
	if( display.is_working ) {
		return display.driver->create();
	}
	return NULL;
}

LCUI_BOOL Surface_IsReady( LCUI_Surface surface )
{
	if( display.is_working ) {
		return display.driver->isReady( surface );
	}
	return TRUE;
}

void Surface_Move( LCUI_Surface surface, int x, int y )
{
	if( display.is_working ) {
		display.driver->move( surface, x, y );
	}
}

void Surface_Resize( LCUI_Surface surface, int w, int h )
{
	if( display.is_working ) {
		LCUI_Rect rect;
		display.driver->resize( surface, w, h );
		rect.x = rect.y = 0;
		rect.width = w;
		rect.height = h;
		LCUIDisplay_InvalidateArea( &rect );
	}
}

void Surface_SetCaptionW( LCUI_Surface surface, const wchar_t *str )
{
	if( display.is_working ) {
		display.driver->setCaptionW( surface, str );
	}
}

void Surface_Show( LCUI_Surface surface )
{
	if( display.is_working ) {
		display.driver->show( surface );
	}
}

void Surface_Hide( LCUI_Surface surface )
{
	if( display.is_working ) {
		display.driver->hide( surface );
	}
}

void *Surface_GetHandle( LCUI_Surface surface )
{
	if( display.is_working ) {
		return display.driver->getHandle( surface );
	}
	return NULL;
}

void Surface_SetRenderMode( LCUI_Surface surface, int mode )
{
	if( display.is_working ) {
		display.driver->setRenderMode( surface, mode );
	}
}

void Surface_Update( LCUI_Surface surface )
{
	if( display.is_working ) {
		display.driver->update( surface );
	}
}

LCUI_PaintContext Surface_BeginPaint( LCUI_Surface surface, LCUI_Rect *rect )
{
	if( display.is_working ) {
		return display.driver->beginPaint( surface, rect );
	}
	return NULL;
}

void Surface_EndPaint( LCUI_Surface surface, LCUI_PaintContext paint_ctx )
{
	if( display.is_working ) {
		display.driver->endPaint( surface, paint_ctx );
	}
}

void Surface_Present( LCUI_Surface surface )
{
	if( display.is_working ) {
		display.driver->present( surface );
	}
}

/** 响应顶级部件的各种事件 */
static void OnSurfaceEvent( LCUI_Widget w, LCUI_WidgetEvent e, void *arg )
{
	int e_type;
	LCUI_Widget root;
	LCUI_RectF *rect;
	LCUI_Surface surface;
	e_type = *((int*)&arg);
	root = LCUIWidget_GetRoot();
	surface = LCUIDisplay_GetBindSurface( e->target );
	if( display.mode == LCDM_SEAMLESS ) {
		if( !surface && e_type != WET_ADD ) {
			return;
		}
	} else if ( e->target == root ) {
		if( !surface && e_type != WET_ADD ) {
			return;
		}
	} else {
		return;
	}
	rect = &e->target->box.graph;
	switch( e_type ) {
	case WET_ADD:
		LCUIDisplay_BindSurface( e->target );
		break;
	case WET_REMOVE:
	case WET_DESTROY:
		LCUIDisplay_UnbindSurface( e->target );
		break;
	case WET_SHOW:
		Surface_Show( surface );
		break;
	case WET_HIDE:
		Surface_Hide( surface );
		break;
	case WET_RESIZE:
	{
		LCUI_Rect area;
		area.x = area.y = 0;
		area.width = roundi( rect->width );
		area.height = roundi( rect->height );
		LCUIDisplay_InvalidateArea( &area );
		break;
	}
	case WET_TITLE:
		DEBUG_MSG("%S\n", e->target->title );
		Surface_SetCaptionW( surface, e->target->title );
		break;
	default: break;
	}
}

/** 在 surface 主动产生无效区域并需要绘制的时候 */
static void OnPaint( LCUI_Event e, void *arg )
{
	LinkedListNode *node;
	LCUI_DisplayEvent dpy_ev = arg;
	for( LinkedList_Each( node, &display.surfaces ) ) {
		SurfaceRecord record = node->data;
		if( record && record->surface != dpy_ev->surface ) {
			continue;
		}
		Widget_InvalidateArea( record->widget, 
				       &dpy_ev->paint.rect, SV_GRAPH_BOX );
	}
}

static void OnResize( LCUI_Event e, void *arg )
{
	LCUI_Widget widget;
	LCUI_DisplayEvent dpy_ev = arg;
	float width = (float)( dpy_ev->resize.width );
	float height = (float)( dpy_ev->resize.height );
	LOG( "[display] resize: (%.2f,%.2f)\n", width, height );
	widget = LCUIDisplay_GetBindWidget( dpy_ev->surface );
	if( widget ) {
		Widget_Resize( widget, width, height );
	}
}

int LCUIDisplay_BindEvent( int event_id, 
			   LCUI_EventFunc func, void *arg,
			   void *data, void( *destroy_data )(void*) )
{
	if( display.is_working ) {
		return display.driver->bindEvent( event_id, func,
						  data, destroy_data );
	}
	return -1;
}

int LCUI_InitDisplay( LCUI_DisplayDriver driver )
{
	LCUI_Widget root;
	if( display.is_working ) {
		return -1;
	}
	LOG( "[display] init ...\n" );
	display.mode = 0;
	root = LCUIWidget_GetRoot();
	LinkedList_Init( &display.rects );
	LinkedList_Init( &display.surfaces );
	if( !driver ) {
		driver = LCUI_CreateDisplayDriver();
		if( !driver ) {
			LOG( "[display] init failed\n" );
			return -2;
		}
	}
	display.driver = driver;
	display.is_working = TRUE;
	display.driver->bindEvent( DET_RESIZE, OnResize, NULL, NULL );
	display.driver->bindEvent( DET_PAINT, OnPaint, NULL, NULL );
	Widget_BindEvent( root, "surface", OnSurfaceEvent, NULL, NULL );
	LCUIDisplay_SetMode( LCDM_DEFAULT );
	LOG( "[display] init ok, driver name: %s\n", display.driver->name );
	return 0;
}

/** 停用图形输出模块 */
int LCUI_ExitDisplay( void )
{
	if( !display.is_working ) {
		return -1;
	}
	display.is_working = FALSE;
	RectList_Clear( &display.rects );
	LCUIDisplay_CleanSurfaces();
	return 0;
}
