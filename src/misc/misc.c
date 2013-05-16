/* ***************************************************************************
 * misc.c -- no specific categories of miscellaneous function
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
 * misc.c -- 没有具体分类的杂项功能
 *
 * 版权所有 (C) 2012-2013 归属于
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

//#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H 
#include LC_GRAPH_H

/* 检测result是否包含option */
LCUI_API int
Check_Option(int result, int option)
{
	if((result & option) == option) {
		return 1; 
	}
	return 0;
}

LCUI_API void
Padding_Init( LCUI_Padding *padding )
{
	padding->left = 0;
	padding->bottom = 0;
	padding->right = 0;
	padding->top = 0; 
}

LCUI_API void
Margin_Init( LCUI_Margin *margin )
{
	margin->left = 0;
	margin->bottom = 0;
	margin->right = 0;
	margin->top = 0; 
}

/* 转换成LCUI_Pos类型 */
LCUI_API LCUI_Pos
Pos(int x, int y)
{
	LCUI_Pos p;
	p.x = x;
	p.y = y;
	return p;
}

/* 转换成LCUI_Size类型 */
LCUI_API LCUI_Size
Size(int w, int h)
{
	LCUI_Size s;
	s.w = w;
	s.h = h;
	return s;
}

/* 
 * 功能：对比两个尺寸
 * 说明：a大于b，返回1， b大于a，返回-1，相等则返回0
 * */
LCUI_API int
Size_Cmp(LCUI_Size a, LCUI_Size b)
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

/* 根据容器尺寸，区域尺寸以及对齐方式，获取该区域的位置 */
LCUI_API LCUI_Pos
GetPosByAlign( LCUI_Size container, LCUI_Size child, int align )
{
	LCUI_Pos pos;
	pos.x = pos.y = 0; 
	 
	switch(align) {
	    case ALIGN_NONE :
	    case ALIGN_TOP_LEFT : /* 向左上角对齐 */  
		break;
	    case ALIGN_TOP_CENTER : /* 向上中间对齐 */ 
		pos.x = (container.w - child.w) / 2.0; 
		break;
	    case ALIGN_TOP_RIGHT : /* 向右上角对齐 */ 
		pos.x = container.w - child.w; 
		break;
	    case ALIGN_MIDDLE_LEFT : /* 向中央偏左对齐 */  
		pos.y = (container.h - child.h) / 2.0;
		break;
	    case ALIGN_MIDDLE_CENTER : /* 向正中央对齐 */ 
		pos.x = (container.w - child.w) / 2.0;
		pos.y = (container.h - child.h) / 2.0;
		break;
	    case ALIGN_MIDDLE_RIGHT : /* 向中央偏由对齐 */ 
		pos.x = container.w - child.w;
		pos.y = (container.h - child.h) / 2.0;
		break;
	    case ALIGN_BOTTOM_LEFT : /* 向底部偏左对齐 */  
		pos.y = container.h - child.h;
		break;
	    case ALIGN_BOTTOM_CENTER : /* 向底部居中对齐 */ 
		pos.x = (container.w - child.w) / 2.0;
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

LCUI_API LCUI_Padding
Padding(int top, int bottom, int left, int right)
{
	LCUI_Padding padding;
	
	padding.top = top;
	padding.bottom = bottom;
	padding.left = left;
	padding.right = right;
	
	return padding;
}

/* 求两个LCUI_Pos类型变量的和 */
LCUI_API LCUI_Pos
Pos_Add( LCUI_Pos a, LCUI_Pos b )
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

/* 对比两个坐标是否一致 */
LCUI_API int
Pos_Cmp( LCUI_Pos a, LCUI_Pos b )
{
	if(a.x != b.x || a.y != b.y) {
		return -1;
	}
	return 0;
}

/* 求两个LCUI_Pos类型变量的差 */
LCUI_API LCUI_Pos
Pos_Sub( LCUI_Pos a, LCUI_Pos b )
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

/* 初始化IntOrFloat_t */
LCUI_API void
IntOrFloat_Init( IntOrFloat_t *combo_num )
{
	combo_num->which_one = 0;
	combo_num->px = 0;
	combo_num->scale = 0.0;
}

/* 初始化PX_PT_t */
LCUI_API void
PX_PT_t_init( PX_PT_t *combo_num )
{
	combo_num->which_one = 0;
	combo_num->px = 0;
	combo_num->pt = 0;
}

/* 根据传入的字符串，获取字符串实际表达的数值，确定数值的单位是PX还是百分比 */
LCUI_API int
GetIntOrFloat( char *str, IntOrFloat_t *combo_num )
{
	char buff[256];
	int bits, j, i, len;
	
	DEBUG_MSG( "enter\n" );
	if( !str ) {
		DEBUG_MSG( "!str, quit\n" );
		return -1;
	}
	DEBUG_MSG( "string: %s\n", str );
	len = strlen(str);
	if(len == 0) {
		DEBUG_MSG( "len == 0, quit\n" );
		return -1;
	}
	for(bits=0,j=0,i=0; i<len; ++i, ++j) {
		if(str[i] == ' ') {
			--j;
			continue;
		}
		/* 判断正负符号 */
		if( str[i] == '-' || str[i] == '+' ) {
			/* 正负符号必须在首位 */
			if( j == 0 ) {
				buff[j] = str[i];
				continue;
			} else {
				return -1;
			}
		}
		if((str[i] >= '0' && str[i] <= '9') || str[i] == '.' ) {
			buff[j] = str[i];
			++bits;
			continue;
		}
		else if(str[i] == '%') {/* 如果有%，取浮点数 */ 
			buff[j] = 0; 
			DEBUG_MSG( "buff: %s\n", buff );
			sscanf( buff, "%lf", &combo_num->scale );
			combo_num->scale/=100.0; 
			combo_num->which_one = 1;
			DEBUG_MSG( "scale: %.2f, quit\n", combo_num->scale );
			return 0;
		}
		else if (str[i] == 'p' || str[i] == 'P') { 
			if(i<len-1) { 
				if (str[i+1] == 'x' || str[i+1] == 'X') {
					buff[j+1] = 0;
					sscanf( str, "%d", &combo_num->px ); 
					combo_num->which_one = 0;
					DEBUG_MSG( "px: %d, quit\n", combo_num->px );
					return 0;
				}
			}
			DEBUG_MSG( "1, quit\n" );
			return -1;
		} else {
			DEBUG_MSG( "2, quit\n" );
			break;
		}
	}
	/* 如果数字位数不大于0，也就是没有数字，则退出 */
	if( bits <= 0 ) {
		return -1;
	}
	/* 不包含px和%，那单位就默认为px，取整数 */
	sscanf( buff, "%d", &combo_num->px ); 
	combo_num->which_one = 0;
	DEBUG_MSG( "quit\n" );
	return 0;
}


/* 根据传入的字符串，获取字符串实际表达的数值，确定数值的单位是PX还是PT */
LCUI_API int
get_PX_PT_t( char *str, PX_PT_t *combo_num )
{
	char buff[256];
	int j, i, len; 
	
	if(str == NULL) {
		return -1;
	}
	
	len = strlen(str);
	if(len == 0) {
		return -1;
	}
	for(j=0,i=0; i<len; ++i, ++j) {
		if(str[i] == ' ') {
			--j;
			continue;
		}
		if(str[i] >= '0' && str[i] <= '9' ) {
			buff[j] = str[i];
			continue;
		}
		else if (str[i] == 'p' || str[i] == 'P') { 
			if(i<len-1) { 
				if (str[i+1] == 'x' || str[i+1] == 'X') {
					buff[j+1] = 0;
					sscanf( str, "%d", &combo_num->px ); 
					combo_num->which_one = 0;
					return 0;
				} 
				else if(str[i+1] == 't' || str[i+1] == 'T') {
					buff[j+1] = 0;
					sscanf( str, "%d", &combo_num->pt ); 
					combo_num->which_one = 1;
					return 0;
				}
			}
			return -1;
		} else {
			break;
		}
	}
	/* 不包含px和pt，那单位就默认为px，取整数 */
	sscanf( buff, "%d", &combo_num->px ); 
	combo_num->which_one = 0;
	return 0;
}

