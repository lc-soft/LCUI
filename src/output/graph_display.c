/* ***************************************************************************
 * graph_display.c -- graphical display processing.
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
 * graph_display.c -- 图形显示处理。
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

//#define DEBUG
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/gui/widget.h>
#include <LCUI/display.h>
#include <LCUI/input.h>
#include <LCUI/cursor.h>
#include <LCUI/thread.h>

#include <time.h>

#ifdef LCUI_BUILD_IN_WIN32
#include <Windows.h>
#endif

/** surface 记录 */
typedef struct SurfaceRecord {
	LCUI_Surface	surface;	/**< surface */
	LCUI_Widget	widget;		/**< surface 所映射的 widget */
} SurfaceRecord;

/** 图形显示功能的上下文数据 */
static struct DisplayContext {
	LCUI_BOOL		is_working;	/**< 标志，指示当前模块是否处于工作状态 */
	FrameCtrlCtx		fc_ctx;		/**< 上下文句柄，用于画面更新时的帧数控制 */
	LCUI_Thread		thread;		/**< 线程，负责画面更新工作 */
	LinkedList		surfaces;	/**< surface 列表 */
	LCUI_Mutex		mutex;
	int			mode;
	LCUI_DisplayInfo	info;
	LCUI_SurfaceMethods	*methods;
} display = { FALSE, NULL, 0 };

/** 获取当前的屏幕内容每秒更新的帧数 */
int LCUIDisplay_GetFPS(void)
{
	return FrameControl_GetFPS( display.fc_ctx );
}

/** 更新各种图形元素的显示 */
static void LCUIDisplay_Update(void)
{
	SurfaceRecord *p_sr;
	LinkedList rlist;
	LinkedListNode *sn, *rn;
	LCUI_PaintContext paint;

	LinkedList_Init( &rlist );
	/* 遍历当前的 surface 记录列表 */
	LinkedList_ForEach( sn, &display.surfaces ) {
		p_sr = (SurfaceRecord*)sn->data;
		if( !p_sr->widget || !p_sr->surface
		 || !Surface_IsReady(p_sr->surface) ) {
			continue;
		}
		/* 更新表面 */
		Surface_Update( p_sr->surface );
		/* 收集无效区域记录 */
		Widget_ProcInvalidArea( p_sr->widget, &rlist );
		/* 在 surface 上逐个重绘无效区域 */
		LinkedList_ForEach( rn, &rlist ) {
			paint = Surface_BeginPaint( p_sr->surface, rn->data );
			DEBUG_MSG( "[%s]: render rect: (%d,%d,%d,%d)\n",
				p_sr->widget->type, paint->rect.left,
				paint->rect.top, paint->rect.w, paint->rect.h );
			Widget_Render( p_sr->widget, paint );
			Surface_EndPaint( p_sr->surface, paint );
		}
		if( rlist.length > 0 ) {
			Surface_Present( p_sr->surface );
		}
		LinkedList_Clear( &rlist, free );
	}
	LinkedList_Clear( &rlist, free );
}

/** 获取与 surface 绑定的 widget */
LCUI_Widget LCUIDisplay_GetBindWidget( LCUI_Surface surface )
{
	SurfaceRecord *sr;
	LinkedListNode *node;
	LinkedList_ForEach( node, &display.surfaces ) {
		sr = (SurfaceRecord*)node->data;
		if( sr && sr->surface == surface ) {
			return sr->widget;
		}
	}
	return NULL;
}

static LCUI_Surface LCUIDisplay_GetBindSurface( LCUI_Widget widget )
{
	SurfaceRecord *sr;
	LinkedListNode *node;

	LinkedList_ForEach( node, &display.surfaces ) {
		sr = (SurfaceRecord*)node->data;
		if( sr && sr->widget == widget ) {
			return sr->surface;
		}
	}
	return NULL;
}

/** 将 widget 与 sruface 进行绑定 */
static void LCUIDisplay_BindSurface( LCUI_Widget widget )
{
	LCUI_Rect *p_rect;
	SurfaceRecord *p_sr;

	if( LCUIDisplay_GetBindSurface(widget) ) {
		return;
	}
	p_sr = NEW( SurfaceRecord, 1 );
	p_sr->widget = widget;
	/**
	 * 初次调用 Surface_New() 耗时较长， 所以先为 p_sr->surface 设置初
	 * 始值，避免在 Surface_New() 返回前被当成正常指针使用。
	 */
	p_sr->surface = NULL;
	p_sr->surface = Surface_New();
	Surface_SetCaptionW( p_sr->surface, widget->title );
	p_rect = &widget->box.graph;
	Surface_Move( p_sr->surface, p_rect->x, p_rect->y );
	Surface_Resize( p_sr->surface, p_rect->w, p_rect->h );
	if( widget->computed_style.visible ) {
		Surface_Show( p_sr->surface );
	} else {
		Surface_Hide( p_sr->surface );
	}
	Widget_InvalidateArea( widget, NULL, SV_GRAPH_BOX );
	LinkedList_Append( &display.surfaces, p_sr );
}

/** 解除 widget 与 sruface 的绑定 */
static void LCUIDisplay_UnbindSurface( LCUI_Widget widget )
{
	SurfaceRecord *sr;
	LinkedListNode *node;
	LinkedList_ForEach( node, &display.surfaces ) {
		sr = (SurfaceRecord*)node->data;
		if( sr && sr->widget == widget ) {
			Surface_Delete( sr->surface );
			LinkedList_DeleteNode( &display.surfaces, node );
			break;
		}
	}
}

static void LCUIDisplay_CleanSurfaces( void )
{
	SurfaceRecord *sr;
	LinkedListNode *node;
	LinkedList_ForEach( node, &display.surfaces ) {
		sr = (SurfaceRecord*)node->data;
		Surface_Delete( sr->surface );
		LinkedList_DeleteNode( &display.surfaces, node );
	}
}

static int LCUIDisplay_Windowed( void )
{
	switch( display.mode ) {
	case LDM_FULLSCREEN:
	case LDM_WINDOWED:
		return 0;
	case LDM_SEAMLESS:
	default:
		LCUIDisplay_CleanSurfaces();
		LCUIDisplay_BindSurface( LCUIRootWidget );
		break;
	}
	Widget_Show( LCUIRootWidget );
	Widget_Resize( LCUIRootWidget, display.info.getWidth(),
		       display.info.getHeight() );
	display.mode = LDM_WINDOWED;
	return 0;
}

static int LCUIDisplay_FullScreen( void )
{
	switch( display.mode ) {
	case LDM_SEAMLESS:
		LCUIDisplay_CleanSurfaces();
		LCUIDisplay_BindSurface( LCUIRootWidget );
	case LDM_WINDOWED:
	default: break;
	case LDM_FULLSCREEN:
		return 0;
	}
	display.mode = LDM_FULLSCREEN;
	LCUIDisplay_SetSize( display.info.getWidth(), display.info.getHeight() );
	return 0;
}

static int LCUIDisplay_Seamless( void )
{
	LinkedListNode *node;
	DEBUG_MSG("display.mode: %d\n", display.mode);
	switch( display.mode ) {
	case LDM_SEAMLESS:
		return 0;
	case LDM_FULLSCREEN:
	case LDM_WINDOWED:
	default:
		LCUIDisplay_CleanSurfaces();
		break;
	}
	LinkedList_ForEach( node, &LCUIRootWidget->children ) {
		LCUIDisplay_BindSurface( node->data );
	}
	display.mode = LDM_SEAMLESS;
	return 0;
}

/* 设置呈现模式 */
int LCUIDisplay_SetMode( int mode )
{
	int ret;
	DEBUG_MSG("mode: %d\n", mode);
	LCUIMutex_Lock( &display.mutex );
	switch( mode ) {
	case LDM_WINDOWED:
		ret = LCUIDisplay_Windowed();
		break;
	case LDM_SEAMLESS:
		ret = LCUIDisplay_Seamless();
		break;
	case LDM_FULLSCREEN:
	default:
		ret = LCUIDisplay_FullScreen();
		break;
	}
	LCUIMutex_Unlock( &display.mutex );
	return ret;
}

/* 获取呈现模式 */
int LCUIDisplay_GetMode(void)
{
	return display.mode;
}

void LCUIDisplay_ExecResize( int width, int height )
{
	// ...
}

/** 设置显示区域的尺寸，仅在窗口化、全屏模式下有效 */
void LCUIDisplay_SetSize( int width, int height )
{
	if( display.mode == LDM_SEAMLESS ) {
		return;
	}
	Widget_Resize( LCUIRootWidget, width, height );
}

/** 获取屏幕宽度 */
int LCUIDisplay_GetWidth( void )
{
	return display.info.getWidth();
}

/** 获取屏幕高度 */
int LCUIDisplay_GetHeight( void )
{
	return display.info.getHeight();
}

/** LCUI的图形显示处理线程 */
static void LCUIDisplay_Thread( void *unused )
{
	while( LCUI_IsActive() && display.is_working ) {
		LCUICursor_UpdatePos();		/* 更新鼠标位置 */
		LCUIWidget_StepTask();		/* 处理所有部件任务 */
		LCUIWidget_StepEvent();		/* 派发所有事件 */
		LCUIMutex_Lock( &display.mutex );
		LCUIDisplay_Update();
		LCUIMutex_Unlock( &display.mutex );
		/* 让本帧停留一段时间 */
		FrameControl_Remain( display.fc_ctx );
	}
	LCUIThread_Exit(NULL);
}

/** 响应顶级部件的各种事件 */
static void OnTopLevelWidgetEvent( LCUI_Widget w, LCUI_WidgetEvent *e, void *arg )
{
	int e_type = *((int*)&arg);
	LCUI_Surface surface;
	LCUI_Rect *p_rect;

	DEBUG_MSG("tip, widget: %s, e_type = %d\n", w->type, e_type);
	surface = LCUIDisplay_GetBindSurface( e->target );
	if( display.mode == LDM_SEAMLESS ) {
		if( !surface && e_type != WET_ADD ) {
			return;
		}
	} else if ( e->target == LCUIRootWidget ) {
		if( !surface && e_type != WET_ADD ) {
			return;
		}
	} else {
		return;
	}
	p_rect = &e->target->box.graph;
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
	case WET_TITLE:
		DEBUG_MSG("%S\n", e->target->title );
		Surface_SetCaptionW( surface, e->target->title );
		break;
	case WET_RESIZE:
		DEBUG_MSG( "resize, w: %d, h: %d\n", p_rect->w, p_rect->h );
		Surface_Resize( surface, p_rect->w, p_rect->h );
		Widget_InvalidateArea( w, NULL, SV_GRAPH_BOX );
		if( w == LCUIRootWidget && display.mode != LDM_SEAMLESS ) {
			LCUIDisplay_ExecResize( p_rect->w, p_rect->h );
		}
		break;
	case WET_MOVE:
		DEBUG_MSG( "x: %d, y: %d\n", p_rect->left, p_rect->top );
		Surface_Move( surface, p_rect->left, p_rect->top );
		break;
	default: break;
	}
}

static void OnEvent( LCUI_Surface surface, LCUI_SystemEvent *e )
{
#ifdef LCUI_BUILD_IN_WIN32
	if( display.mode == LDM_SEAMLESS ) {
		return;
	}
	//DEBUG_MSG("surface: %p, event: %d, rel_x: %d, rel_y: %d\n", surface, e->type, e->rel_x, e->rel_y);
	if( e->type == LCUI_MOUSEMOVE ) {
		LCUIMouse_SetPos( e->rel_x, e->rel_y );
		return;
	}
	LCUI_PostEvent( e );
#endif
}

/** 在 surface 主动产生无效区域的时候 */
static void OnInvalidRect( LCUI_Surface surface, LCUI_Rect *rect )
{
	SurfaceRecord *sr;
	LinkedListNode *node;
	LinkedList_ForEach( node, &display.surfaces ) {
		sr = (SurfaceRecord*)node->data;
		if( sr && sr->surface != surface ) {
			continue;
		}
		Widget_InvalidateArea( sr->widget, rect, SV_GRAPH_BOX );
	}
}

void Surface_Delete( LCUI_Surface surface )
{
	display.methods->delete( surface );
}

LCUI_Surface Surface_New( void )
{
	return display.methods->new();
}

LCUI_BOOL Surface_IsReady( LCUI_Surface surface )
{
	if( display.methods->isReady ) {
		return display.methods->isReady( surface );
	}
	return TRUE;
}

void Surface_Move( LCUI_Surface surface, int x, int y )
{
	if( display.methods->move ) {
		display.methods->move( surface, x, y );
	}
}

void Surface_Resize( LCUI_Surface surface, int w, int h )
{
	if( display.methods->resize ) {
		display.methods->resize( surface, w, h );
	}
}

void Surface_SetCaptionW( LCUI_Surface surface, const wchar_t *str )
{
	if( display.methods->setCaptionW ) {
		display.methods->setCaptionW( surface, str );
	}
}

void Surface_Show( LCUI_Surface surface )
{
	if( display.methods->show ) {
		display.methods->show( surface );
	}
}

void Surface_Hide( LCUI_Surface surface )
{
	if( display.methods->hide ) {
		display.methods->hide( surface );
	}
}

/** 设置 Surface 的渲染模式 */
void Surface_SetRenderMode( LCUI_Surface surface, int mode )
{
	if( display.methods->setRenderMode ) {
		display.methods->setRenderMode( surface, mode );
	}
}

/** 更新 surface，应用缓存的变更 */
void Surface_Update( LCUI_Surface surface )
{
	if( display.methods->update ) {
		display.methods->update( surface );
	}
}

/**
 * 准备绘制 Surface 中的内容
 * @param[in] surface	目标 surface
 * @param[in] rect	需进行绘制的区域，若为NULL，则绘制整个 surface
 * @return		返回绘制上下文句柄
 */
LCUI_PaintContext Surface_BeginPaint( LCUI_Surface surface, LCUI_Rect *rect )
{
	return display.methods->beginPaint( surface, rect );
}

/**
 * 结束对 Surface 的绘制操作
 * @param[in] surface	目标 surface
 * @param[in] paint_ctx	绘制上下文句柄
 */
void Surface_EndPaint( LCUI_Surface surface, LCUI_PaintContext paint_ctx )
{
	display.methods->endPaint( surface, paint_ctx );
}

/** 将帧缓存中的数据呈现至Surface的窗口内 */
void Surface_Present( LCUI_Surface surface )
{
	display.methods->present( surface );
}

/** 初始化图形输出模块 */
int LCUI_InitDisplay( void )
{
	if( display.is_working ) {
		return -1;
	}
	display.is_working = TRUE;
	display.mode = LDM_DEFAULT;
	LCUIMutex_Init( &display.mutex );
	LinkedList_Init( &display.surfaces );
#ifdef LCUI_BUILD_IN_WIN32
	display.methods = LCUIDisplay_InitWin32( &display.info );
#elif defined(LCUI_VIDEO_DRIVER_FRAMEBUFFER)
	display.methods = LCUIDisplay_InitLinuxFB( &display.info );
#endif
	display.methods->onInvalidRect = OnInvalidRect;
	display.methods->onEvent = OnEvent;
	display.fc_ctx = FrameControl_Create();
	FrameControl_SetMaxFPS( display.fc_ctx, MAX_FRAMES_PER_SEC );
	Widget_BindEvent( LCUIRootWidget, "TopLevelWidget",
			  OnTopLevelWidgetEvent, NULL, NULL );
	return LCUIThread_Create( &display.thread, LCUIDisplay_Thread, NULL );
}

/** 停用图形输出模块 */
int LCUI_ExitDisplay( void )
{
	if( !display.is_working ) {
		return -1;
	}
	display.is_working = FALSE;
	LCUIMutex_Destroy( &display.mutex );
	FrameControl_Destroy( display.fc_ctx );
	return LCUIThread_Join( display.thread, NULL );
}
