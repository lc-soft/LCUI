/* ***************************************************************************
 * bitmapfont.h -- The Bitmap Font operation set.
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
 * bitmapfont.h -- 位图字体的操作集
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

#ifndef __LCUI_BITMAPFONT_H__
#define __LCUI_BITMAPFONT_H__

LCUI_BEGIN_HEADER
	
/*---------------- 字体位图数据 ------------------*/
typedef struct LCUI_FontBMP_ {
	int top;		/* 与顶边框的距离 */
	int left;		/* 与左边框的距离 */
	int width;		/* 位图宽度 */
	int rows;		/* 位图行数 */
	int pitch;
	uchar_t *buffer;	/* 字体位图数据 */
	short num_grays;
	char pixel_mode;
	LCUI_Pos advance;	/* XY轴的跨距 */
} LCUI_FontBMP;
/*------------------- END ---------------------*/

/*------------------------ 宽字符位图及相关数据 -----------------------*/
typedef struct LCUI_WChar_ {
	wchar_t		char_code;	/* 字符码 */
	LCUI_FontBMP	*bitmap;	/* 字符的位图数据 */
	LCUI_RGB	color;		/* 该文字的配色 */
	LCUI_BOOL	update;		/* 标明这个字符是否需要刷新 */
	int		color_type;	/* 颜色类型(DEFAULT / CUSTOM) */
} LCUI_WChar;
/*-----------------------------END ----------------------------------*/


LCUI_API uchar_t const * in_core_font_8x8( void );

LCUI_API LCUI_BOOL FontBMP_Valid(LCUI_FontBMP *bitmap);
/*
 * 功能：检测位图数据是否有效
 * 返回值：有效返回帧，无效返回假
 */

LCUI_API void Print_FontBMP_Info(LCUI_FontBMP *bitmap);
/* 功能：打印位图的信息 */

LCUI_API void FontBMP_Init(LCUI_FontBMP *bitmap);
/* 初始化字体位图 */

LCUI_API void FontBMP_Free(LCUI_FontBMP *bitmap);
/* 释放字体位图占用的资源 */

LCUI_API int FontBMP_Create(LCUI_FontBMP *bitmap, int width, int rows);
/* 功能：创建字体位图 */

LCUI_API void Get_Default_FontBMP(unsigned short code, LCUI_FontBMP *out_bitmap);
/* 功能：根据字符编码，获取已内置的字体位图 */

LCUI_API int Show_FontBMP(LCUI_FontBMP *fontbmp);
/* 功能：在屏幕打印以0和1表示字体位图 */

LCUI_API int
FontBMP_Mix(	LCUI_Graph	*graph, LCUI_Pos	des_pos,
		LCUI_FontBMP	*bitmap, LCUI_RGB	color,
		int flag );
/* 功能：将字体位图绘制到背景图形上 */

/* 获取现有的字体位图数据 */
LCUI_API LCUI_FontBMP *
Get_ExistFontBMP( int font_id, wchar_t ch, int pixel_size );

LCUI_END_HEADER

#endif
