/* ***************************************************************************
 * LCUI_Misc.h -- no specific categories of miscellaneous function
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
 * LCUI_Misc.h -- 没有具体分类的杂项功能
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

#ifndef __LCUI_MISC_H__
#define __LCUI_MISC_H__

#include LC_MISC_DELAY_H
#include LC_MISC_RECT_H
#include LC_MISC_STRING_H

LCUI_BEGIN_HEADER

/* 检测result是否包含option */
LCUI_EXPORT(int)
Check_Option(int result, int option);

LCUI_EXPORT(void)
Padding_Init( LCUI_Padding *padding );

LCUI_EXPORT(void)
Margin_Init( LCUI_Margin *margin );

/* 转换成LCUI_Pos类型 */
LCUI_EXPORT(LCUI_Pos)
Pos(int x, int y);

/* 转换成LCUI_Size类型 */
LCUI_EXPORT(LCUI_Size)
Size(int w, int h);

/* 
 * 功能：对比两个尺寸
 * 说明：a大于b，返回1， b大于a，返回-1，相等则返回0
 * */
LCUI_EXPORT(int)
Size_Cmp(LCUI_Size a, LCUI_Size b);

/* 根据容器尺寸，区域尺寸以及对齐方式，获取该区域的位置 */
LCUI_EXPORT(LCUI_Pos)
Align_Get_Pos( LCUI_Size container, LCUI_Size child, int align );

LCUI_EXPORT(LCUI_Padding)
Padding(int top, int bottom, int left, int right);

/* 求两个LCUI_Pos类型变量的和 */
LCUI_EXPORT(LCUI_Pos)
Pos_Add( LCUI_Pos a, LCUI_Pos b );

/* 对比两个坐标是否一致 */
LCUI_EXPORT(int)
Pos_Cmp( LCUI_Pos a, LCUI_Pos b );

/* 求两个LCUI_Pos类型变量的差 */
LCUI_EXPORT(LCUI_Pos)
Pos_Sub( LCUI_Pos a, LCUI_Pos b );

/* 初始化PX_P_t */
LCUI_EXPORT(void)
PX_P_t_init( PX_P_t *combo_num );

/* 初始化PX_PT_t */
LCUI_EXPORT(void)
PX_PT_t_init( PX_PT_t *combo_num );

/* 根据传入的字符串，获取字符串实际表达的数值，确定数值的单位是PX还是百分比 */
LCUI_EXPORT(int)
get_PX_P_t( char *str, PX_P_t *combo_num );

/* 根据传入的字符串，获取字符串实际表达的数值，确定数值的单位是PX还是PT */
LCUI_EXPORT(int)
get_PX_PT_t( char *str, PX_PT_t *combo_num );

/* 录制屏幕指定区域的内容 */
LCUI_EXPORT(int)
LCUIScreen_StartRecord( LCUI_Rect area );

/* 结束录制 */
LCUI_EXPORT(int)
LCUIScreen_EndRecord( void );

LCUI_END_HEADER

#endif
