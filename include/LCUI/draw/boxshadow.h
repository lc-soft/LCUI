/* ****************************************************************************
 * boxshadow.c -- draw box shadow.
 * 
 * Copyright (C) 2014 by
 * Liu Chao <lc-soft@live.cn>
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
 * boxshadow.c -- 矩形阴影
 *
 * 版权所有 (C) 2014 归属于
 * 刘超 <lc-soft@live.cn>
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

#ifndef __LCUI_DRAW_BOXSHADOW__
#define __LCUI_DRAW_BOXSHADOW__

typedef struct LCUI_BoxShadowRec_ {
	int offset_x, offset_y;
	int size;
	LCUI_Color top_color;
	LCUI_Color bottom_color;
	LCUI_Color left_color;
	LCUI_Color right_color;
} LCUI_BoxShadow;

static inline LCUI_BoxShadow BoxShadow( int x, int y, int size, LCUI_Color color )
{
	LCUI_BoxShadow shadow;
	shadow.offset_x = x;
	shadow.offset_y = y;
	shadow.left_color = color;
	shadow.right_color = color;
	shadow.top_color = color;
	shadow.bottom_color = color;
	return shadow;
}

/** 计算Box在添加阴影后的宽度 */
static inline int BoxShadow_GetWidth( LCUI_BoxShadow *shadow, int box_w )
{
	if( shadow->offset_x > 0 ) {
		return box_w + shadow->size*2 + shadow->offset_x;
	} else {
		return box_w + shadow->size*2 - shadow->offset_x;
	}
}

/** 计算Box在添加阴影后的高度 */
static inline int BoxShadow_GetHeight( LCUI_BoxShadow *shadow, int box_h )
{
	if( shadow->offset_y > 0 ) {
		return box_h + shadow->size*2 + shadow->offset_y;
	} else {
		return box_h + shadow->size*2 - shadow->offset_y;
	}
}

static inline int BoxShadow_GetBoxX( LCUI_BoxShadow *shadow )
{
	if( shadow->offset_x >= shadow->size ) {
		return 0;
	} else {
		return shadow->size - shadow->offset_x;
	}
}

static inline int BoxShadow_GetBoxY( LCUI_BoxShadow *shadow )
{
	if( shadow->offset_y >= shadow->size ) {
		return 0;
	} else {
		return shadow->size - shadow->offset_y;
	}
}

static inline int BoxShadow_GetY( LCUI_BoxShadow *shadow )
{
	if( shadow->offset_y <= shadow->size ) {
		return 0;
	} else {
		return shadow->offset_y - shadow->size;
	}
}

static inline int BoxShadow_GetX( LCUI_BoxShadow *shadow )
{
	if( shadow->offset_x <= shadow->size ) {
		return 0;
	} else {
		return shadow->offset_x - shadow->size;
	}
}

int Graph_DrawBoxShadowEx( LCUI_Graph *graph, LCUI_Rect area, LCUI_BoxShadow shadow );

#endif
