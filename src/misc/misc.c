/* ***************************************************************************
 * misc.c -- no specific categories of miscellaneous function
 * 
 * Copyright (C) 2012 by
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
 * misc.c -- 没有具体分类的杂项功能
 *
 * 版权所有 (C) 2012 归属于 
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
#include LC_LCUI_H 

int Check_Option(int result, int option)
/* 功能：检测result是否包含option */
{
	if((result & option) == option) {
		return 1; 
	}
	return 0;
}

void Border_Init(LCUI_Border *in)
/* 功能：初始化边框 */
{ 
	in->left = 0;
	in->bottom = 0;
	in->right = 0;
	in->top = 0; 
}

LCUI_Pos Pos(int x, int y)
/* 功能：转换成LCUI_Pos类型 */
{
	LCUI_Pos p;
	p.x = x;
	p.y = y;
	return p;
}

LCUI_Size Size(int w, int h)
/* 功能：转换成LCUI_Size类型 */
{
	LCUI_Size s;
	s.w = w;
	s.h = h;
	return s;
}

int Size_Cmp(LCUI_Size a, LCUI_Size b)
/* 
 * 功能：对比两个尺寸
 * 说明：a大于b，返回1， b大于a，返回-1，相等则返回0
 * */
{
	if(a.w > b.w || a.h > b.h) {
		return 1;
	}
	else if(a.w == b.w && a.h == b.h) {
		return 0;
	} else {
		return -1;
	}
}

LCUI_Pos Align_Get_Pos(LCUI_Size container, LCUI_Size child, int align)
/* 功能：根据容器尺寸，区域尺寸以及对齐方式，获取该区域的位置 */
{
	LCUI_Pos pos;
	pos.x = pos.y = 0; 
	 
	switch(align) {
	    case ALIGN_NONE :
	    case ALIGN_TOP_LEFT : /* 向左上角对齐 */  
		break;
	    case ALIGN_TOP_CENTER : /* 向上中间对齐 */ 
		pos.x = (container.w - child.w) / 2; 
		break;
	    case ALIGN_TOP_RIGHT : /* 向右上角对齐 */ 
		pos.x = container.w - child.w; 
		break;
	    case ALIGN_MIDDLE_LEFT : /* 向中央偏左对齐 */  
		pos.y = (container.h - child.h) / 2;
		break;
	    case ALIGN_MIDDLE_CENTER : /* 向正中央对齐 */ 
		pos.x = (container.w - child.w) / 2;
		pos.y = (container.h - child.h) / 2;
		break;
	    case ALIGN_MIDDLE_RIGHT : /* 向中央偏由对齐 */ 
		pos.x = container.w - child.w;
		pos.y = (container.h - child.h) / 2;
		break;
	    case ALIGN_BOTTOM_LEFT : /* 向底部偏左对齐 */  
		pos.y = container.h - child.h;
		break;
	    case ALIGN_BOTTOM_CENTER : /* 向底部居中对齐 */ 
		pos.x = (container.w - child.w) / 2;
		pos.y = container.h - child.h;
		break;
	    case ALIGN_BOTTOM_RIGHT : /* 向底部偏右对齐 */ 
		pos.x = container.w - child.w;
		pos.y = container.h - child.h;
		break; 
	    default :  break; 
	}
	return pos;
}

LCUI_Border Border(int top, int bottom, int left, int right)
{
	LCUI_Border border;
	border.top = top;
	border.left = left;
	border.right = right;
	border.bottom = bottom;
	return border;
	
}

LCUI_Pos Pos_Add(LCUI_Pos a, LCUI_Pos b)
/* 功能：求两个LCUI_Pos类型变量的和 */
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

int Pos_Cmp(LCUI_Pos a, LCUI_Pos b)
/* 功能：对比两个坐标是否一致 */
{
	if(a.x != b.x || a.y != b.y) {
		return -1;
	}
	return 0;
}

LCUI_Pos Pos_Sub(LCUI_Pos a, LCUI_Pos b)
/* 功能：求两个LCUI_Pos类型变量的差 */
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}
