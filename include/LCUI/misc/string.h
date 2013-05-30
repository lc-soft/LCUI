/* ***************************************************************************
 * string.h -- The string operation set.
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
 * string.h -- 字符串操作集
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
#ifndef __LCUI_MISC_STRING_H__
#define __LCUI_MISC_STRING_H__

LCUI_BEGIN_HEADER

/* windows系统里并没有strcasecmp函数，因此，使用自定义函数的代替 */
#ifdef LCUI_BUILD_IN_WIN32
#define strcasecmp(str1, str2)	lcui_strcasecmp(str1, str2)
#endif

/* 不区分大小写，对比两个字符串 */
LCUI_API int
lcui_strcasecmp( const char *str1, const char *str2 );

/* 初始化字符串 */
LCUI_API void
LCUIString_Init( LCUI_String *in );

/* 初始化宽字符串 */
LCUI_API void
LCUIWString_Init( LCUI_WString *in );

/* 拷贝源字符串至目标字符串中 */
LCUI_API void
_LCUIString_Copy( LCUI_String * des, const char *src );

/* 宽字符串拷贝 */
LCUI_API void
_LCUIWString_Copy( LCUI_WString *des, const wchar_t *src );

LCUI_API void
LCUIWString_Copy( LCUI_WString *des_str, LCUI_WString *src_str );

/* 字符串对比 */
LCUI_API int
_LCUIString_Cmp( LCUI_String *str1, const char *str2 );

LCUI_API int
LCUIString_Cmp( LCUI_String *str1, LCUI_String *str2 );

/* 宽字符串对比 */
LCUI_API int
_LCUIWString_Cmp( LCUI_WString *str1, const wchar_t *str2 );

LCUI_API int
LCUIWString_Cmp( LCUI_WString *str1, LCUI_WString *str2 );

/* 字符串拷贝 */
LCUI_API int
LCUIString_Copy( LCUI_String *str1, LCUI_String *str2 );

LCUI_API void
LCUIString_Free( LCUI_String *in );

/* 释放宽字符串占用的资源 */
LCUI_API void
LCUIWString_Free( LCUI_WString *str );

LCUI_END_HEADER

#endif
