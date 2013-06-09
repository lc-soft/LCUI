/* ****************************************************************************
 * LCUI_Display.h -- APIs of graphics display.
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
 * LCUI_Display.h -- 与图形显示相关的APIs
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

#ifndef __LCUI_DISPLAY_H__
#define __LCUI_DISPLAY_H__

LCUI_BEGIN_HEADER

/****************************** 屏幕信息 ********************************/
typedef struct LCUI_Screen_
{
	int		mode;		/* 屏幕显示模式 */
	char		dev_name[256];	/* 图形输出设备的名称 */
	LCUI_Size	size;		/* 屏幕尺寸 */
	int		bits;		/* 每个像素的用多少位表示 */
} LCUI_Screen;
/***********************************************************************/

#ifdef LCUI_BUILD_IN_WIN32
#define WIN32_WINDOW_WIDTH 800
#define WIN32_WINDOW_HEIGHT 600

LCUI_API HWND Win32_GetSelfHWND( void );

LCUI_API void Win32_SetSelfHWND( HWND hwnd );
#endif

/* 
 * 功能：在屏幕上指定位置放置图形
 * 说明：此函数使用帧缓冲（FrameBuffer）进行图形输出
 * *注：主要代码参考自mgaveiw的mga_vfb.c文件中的write_to_fb函数.
 * */
LCUI_API int
LCUIScreen_PutGraph (LCUI_Graph *src, LCUI_Pos pos );

/* 
 * 功能：直接读取帧缓冲中的图像数据
 * 说明：效率较高，但捕获的图像有可能会有问题。
 * */
LCUI_API void
LCUIScreen_CatchGraph( LCUI_Rect area, LCUI_Graph *out );

/* 将帧缓冲的图像同步显示至屏幕 */
LCUI_API void
LCUIScreen_SyncFrameBuffer( void );

/*
 * 功能：获取屏幕宽度
 * 返回值：屏幕的宽度，单位为像素，必须在使用LCUI_Init()函数后使用，否则无效
 * */
LCUI_API int
LCUIScreen_GetWidth( void );

/*
 * 功能：获取屏幕高度
 * 返回值：屏幕的高度，单位为像素，必须在使用LCUI_Init()函数后使用，否则无效
 * */
LCUI_API int
LCUIScreen_GetHeight( void );

/* 获取屏幕尺寸 */
LCUI_API LCUI_Size
LCUIScreen_GetSize( void );

/* 填充指定位置的像素点的颜色 */
LCUI_API void
LCUIScreen_FillPixel( LCUI_Pos pos, LCUI_RGB color );

/* 获取屏幕内显示的图像 */
LCUI_API int
LCUIScreen_GetGraph( LCUI_Graph *out );

/* 设置屏幕内的指定区域为无效区域，以便刷新该区域内的图形显示 */
LCUI_API int
LCUIScreen_InvalidArea( LCUI_Rect rect );

/* 功能：获取屏幕中的每个像素的表示所用的位数 */
LCUI_API int
LCUIScreen_GetBits( void );

/* 获取屏幕显示模式 */
LCUI_API int
LCUIScreen_GetMode( void );

/* 获取屏幕信息 */
LCUI_API void LCUIScreen_GetInfo( LCUI_Screen *info );

/* 设置屏幕信息 */
LCUI_API void LCUIScreen_SetInfo( LCUI_Screen *info );

/* 获取屏幕中心点的坐标 */
LCUI_API LCUI_Pos
LCUIScreen_GetCenter( void );

/* 为图层树锁上互斥锁 */
LCUI_API void 
LCUIScreen_LockGraphLayerTree( void );

/* 解除图层树互斥锁 */
LCUI_API void 
LCUIScreen_UnlockGraphLayerTree( void );

/* 获取屏幕中指定区域内实际要显示的图形 */
LCUI_API void
LCUIScreen_GetRealGraph( LCUI_Rect rect, LCUI_Graph *graph );

/* 标记需要同步无效区域 */
LCUI_API void LCUIScreen_MarkSync(void);

/* 设置模式 */
LCUI_API int LCUIScreen_SetMode( int w, int h, int mode );

/* 初始化屏幕 */
LCUI_API int LCUIScreen_Init( int w, int h, int mode );

/* 销毁屏幕 */
LCUI_API int LCUIScreen_Destroy( void );

/* 初始化图形输出模块 */
LCUI_API int
LCUIModule_Video_Init( int w, int h, int mode );

/* 停用图形输出模块 */
LCUI_API int
LCUIModule_Video_End( void );

LCUI_END_HEADER

#endif
