/* ***************************************************************************
 * surface.h -- graphic presentation layer.
 *
 * Copyright (C) 2014-2015 by Liu Chao <lc-soft@live.cn>
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
 * ***************************************************************************/

/* ****************************************************************************
 * surface.h -- 图形呈现层。
 *
 * 版权所有 (C) 2014-2015 归属于 刘超 <lc-soft@live.cn>
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
 * ***************************************************************************/

#ifndef __LCUI_SURFACE_H__
#define __LCUI_SURFACE_H__

#define RENDER_MODE_BIT_BLT	0
#define RENDER_MODE_STRETCH_BLT 1

#ifdef __IN_SURFACE_SOURCE_FILE__
typedef struct LCUI_SurfaceRec_ * LCUI_Surface;
#else
typedef void* LCUI_Surface;
#endif

/** surface 的操作方法集 */
typedef struct LCUI_SurfaceMethods {
	char			name[32];
	LCUI_Surface		(*new)(void);
	void			(*delete)(LCUI_Surface);
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
	void			(*setOpacity)(LCUI_Surface,float);
	void			(*onInvalidRect)(LCUI_Surface,LCUI_Rect*);
	void			(*onEvent)(LCUI_Surface,LCUI_SystemEvent*);
} LCUI_SurfaceMethods;

#ifdef LCUI_BUILD_IN_WIN32
/** 初始化适用于 Win32 平台的 surface 支持 */
LCUI_SurfaceMethods *LCUISurface_InitWin32();
#endif

/** 删除 Surface */
LCUI_API void Surface_Delete( LCUI_Surface surface );

/** 新建一个 Surface */
LCUI_API LCUI_Surface Surface_New(void);

LCUI_API LCUI_BOOL Surface_IsReady( LCUI_Surface surface );

LCUI_API void Surface_Move( LCUI_Surface surface, int x, int y );

LCUI_API void Surface_Resize( LCUI_Surface surface, int w, int h );

LCUI_API void Surface_SetCaptionW( LCUI_Surface surface, const wchar_t *str );

LCUI_API void Surface_Show( LCUI_Surface surface );

LCUI_API void Surface_Hide( LCUI_Surface surface );

/** 设置 Surface 的渲染模式 */
LCUI_API void Surface_SetRenderMode( LCUI_Surface surface, int mode );

/** 更新 surface，应用缓存的变更 */
LCUI_API void Surface_Update( LCUI_Surface surface );

/**
 * 准备绘制 Surface 中的内容
 * @param[in] surface	目标 surface
 * @param[in] rect	需进行绘制的区域，若为NULL，则绘制整个 surface
 * @return		返回绘制上下文句柄
 */
LCUI_API LCUI_PaintContext Surface_BeginPaint( LCUI_Surface surface, LCUI_Rect *rect );

/**
 * 结束对 Surface 的绘制操作
 * @param[in] surface	目标 surface
 * @param[in] paint_ctx	绘制上下文句柄
 */
LCUI_API void Surface_EndPaint( LCUI_Surface surface, LCUI_PaintContext paint_ctx );

/** 将帧缓存中的数据呈现至Surface的窗口内 */
LCUI_API void Surface_Present( LCUI_Surface surface );

#endif
