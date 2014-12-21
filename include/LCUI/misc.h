/* ***************************************************************************
 * misc.h  no specific categories of miscellaneous function
 * 
 * Copyright (C) 2012-2014 by
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
 * misc.h  没有具体分类的杂项功能
 *
 * 版权所有 (C) 2012-2014 归属于
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

#ifndef __LCUI_MISC_H__
#define __LCUI_MISC_H__

LCUI_BEGIN_HEADER

/** 对齐方式 */
enum AlignType {
	ALIGN_NONE,		/* 无 */
	ALIGN_TOP_LEFT,	  	/* 向左上角对齐 */
	ALIGN_TOP_CENTER,	/* 向上中间对齐 */
	ALIGN_TOP_RIGHT,	/* 向右上角对齐 */
	ALIGN_MIDDLE_LEFT,	/* 向中央偏左对齐 */
	ALIGN_MIDDLE_CENTER,	/* 向正中央对齐 */
	ALIGN_MIDDLE_RIGHT,	/* 向中央偏由对齐 */
	ALIGN_BOTTOM_LEFT,	/* 向底部偏左对齐 */
	ALIGN_BOTTOM_CENTER,	/* 向底部居中对齐 */
	ALIGN_BOTTOM_RIGHT	/* 向底部偏右对齐 */
};

/** 布局方式 */
enum LayoutType {
	LAYOUT_NONE	= 0,	  /* 无 */
	LAYOUT_NORMAL	= 0,
	LAYOUT_ZOOM	= 1,	  /* 缩放 */
	LAYOUT_STRETCH	= 1<<1,	  /* 拉伸 */
	LAYOUT_CENTER	= 1<<2,	  /* 居中 */
	LAYOUT_TILE	= 1<<3	  /* 平铺 */
};

/** 转换成LCUI_Pos类型 */
LCUI_Pos Pos(int x, int y);

/** 转换成LCUI_Size类型 */
LCUI_Size Size(int w, int h);

/* 
 * 功能：对比两个尺寸
 * 说明：a大于b，返回1， b大于a，返回-1，相等则返回0
 * */
int Size_Cmp( LCUI_Size a, LCUI_Size b );

/** 根据容器尺寸，区域尺寸以及对齐方式，获取该区域的位置 */
LCUI_Pos GetPosByAlign( LCUI_Size box, LCUI_Size child, int align );

/** 根据给定的字符串，生成一个ID */
unsigned int BKDRHash( const char *str );

LCUI_END_HEADER

#include <LCUI/misc/delay.h>
#include <LCUI/misc/dirent.h>
#include <LCUI/misc/rbtree.h>
#include <LCUI/misc/linkedlist.h>
#include <LCUI/misc/rect.h>
#include <LCUI/misc/framectrl.h>
#include <LCUI/misc/parse.h>

#endif
