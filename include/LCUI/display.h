/* ***************************************************************************
 * display.h -- graphic display control
 *
 * Copyright (C) 2012-2016 by Liu Chao <lc-soft@live.cn>
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
 * display.h -- 图形显示控制
 *
 * 版权所有 (C) 2012-2016 归属于 刘超 <lc-soft@live.cn>
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

#ifndef LCUI_DISPLAY_CONTROL_H
#define LCUI_DISPLAY_CONTROL_H

#include <LCUI/gui/widget.h>
#include <LCUI/surface.h>

LCUI_BEGIN_HEADER

/** 图形显示模式 */
enum LCUIDisplayMode {
	LCDM_WINDOWED = 1,	/**< 窗口化 */
	LCDM_SEAMLESS,		/**< 与系统GUI无缝结合 */
	LCDM_FULLSCREEN		/**< 全屏模式 */
};

#define LCDM_DEFAULT LCDM_WINDOWED

/** 显示驱动的事件类型 */
enum LCUI_DisplayEventType {
	DET_NONE,
	DET_PAINT,
	DET_RESIZE,
	DET_READY
};

/** 显示驱动的事件数据结构 */
typedef struct LCUI_DisplayEventRec_ {
	int type;
	union {
		struct {
			LCUI_Rect rect;
		} paint;
		struct {
			int width, height;
		} resize;
	};
	LCUI_Surface surface;
} LCUI_DisplayEventRec, *LCUI_DisplayEvent;

/** surface 的操作方法集 */
typedef struct LCUI_DisplayDriverRec_ {
	char			name[256];
	int			(*getWidth)(void);
	int			(*getHeight)(void);
	LCUI_Surface		(*create)(void);
	void			(*destroy)(LCUI_Surface);
	void			(*resize)(LCUI_Surface,int,int);
	void			(*move)(LCUI_Surface,int,int);
	void			(*show)(LCUI_Surface);
	void			(*hide)(LCUI_Surface);
	void			(*update)(LCUI_Surface);
	void			(*present)(LCUI_Surface);
	LCUI_BOOL		(*isReady)(LCUI_Surface);
	LCUI_PaintContext	(*beginPaint)(LCUI_Surface,LCUI_Rect*);
	void			(*endPaint)(LCUI_Surface,LCUI_PaintContext);
	void			(*setCaptionW)(LCUI_Surface,const wchar_t*);
	void			(*setRenderMode)(LCUI_Surface,int);
	void*			(*getHandle)(LCUI_Surface);
	void			(*setOpacity)(LCUI_Surface,float);
	int			(*bindEvent)(int,LCUI_EventFunc,void*,void(*)(void*));
} LCUI_DisplayDriverRec, *LCUI_DisplayDriver;

/** 一秒内的最大画面帧数 */
#define MAX_FRAMES_PER_SEC 100

/* 设置呈现模式 */
LCUI_API int LCUIDisplay_SetMode( int mode );

/** 获取屏幕显示模式 */
LCUI_API int LCUIDisplay_GetMode( void );

LCUI_API void LCUIDisplay_ShowRectBorder( void );

LCUI_API void LCUIDisplay_HideRectBorder( void );

/** 设置显示区域的尺寸，仅在窗口化、全屏模式下有效 */
LCUI_API void LCUIDisplay_SetSize( int width, int height );

/** 获取屏幕宽度 */
LCUI_API int LCUIDisplay_GetWidth( void );

/** 获取屏幕高度 */
LCUI_API int LCUIDisplay_GetHeight( void );

/** 添加无效区域 */
LCUI_API void LCUIDisplay_InvalidateArea( LCUI_Rect *rect );

/** 获取当前部件所属的 surface */
LCUI_API LCUI_Surface LCUIDisplay_GetSurfaceOwner( LCUI_Widget w );

/** 绑定 surface 触发的事件 */
LCUI_API int LCUIDisplay_BindEvent( int event_id, LCUI_EventFunc func, void *arg,
				    void *data, void( *destroy_data )(void*) );

/** 获取当前的屏幕内容每秒更新的帧数 */
LCUI_API int LCUIDisplay_GetFPS(void);

/** 初始化图形输出模块 */
LCUI_API int LCUI_InitDisplay( LCUI_DisplayDriver driver );

/** 停用图形输出模块 */
LCUI_API int LCUI_ExitDisplay( void );

LCUI_END_HEADER

#endif
