/* ***************************************************************************
 * dummy.c --  if not found graphical output support, use this sourcefile
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
 * dummy.c -- 通常在找不到合适的图形输出支持的情况下，使用该源文件，用虚设函数凑数
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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>

#ifdef LCUI_VIDEO_DRIVER_NONE

LCUI_API void
LCUIScreen_FillPixel( LCUI_Pos pos, LCUI_RGB color )
{
	return;
}

LCUI_API int
LCUIScreen_GetGraph( LCUI_Graph *out )
{
	return -1;
}


LCUI_API int
LCUIScreen_Init( int w, int h, int mode )
{
	return -1;
}


/* 设置视频输出模式 */
LCUI_API int
LCUIScreen_SetMode( int w, int h, int mode )
{
	return -1;
}


LCUI_API int
LCUIScreen_Destroy( void )
{
	return -1;
}


LCUI_API void
LCUIScreen_SyncFrameBuffer( void )
{
	return;
}


LCUI_API int
LCUIScreen_PutGraph (LCUI_Graph *graph, LCUI_Pos des_pos )
{
	return -1;
}

LCUI_API void
LCUIScreen_CatchGraph( LCUI_Rect area, LCUI_Graph *out )
{
	return;
}

#endif
