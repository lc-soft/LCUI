/** ******************************************************************************
 * @file	fontlibrary.h
 * @brief	The font database management module.
 * @author	Liu Chao <lc-soft@live.cn>
 * @warning
 * Copyright (C) 2012-2013 by							\n
 * Liu Chao									\n
 * 										\n
 * This file is part of the LCUI project, and may only be used, modified, and	\n
 * distributed under the terms of the GPLv2.					\n
 * 										\n
 * (GPLv2 is abbreviation of GNU General Public License Version 2)		\n
 * 										\n
 * By continuing to use, modify, or distribute this file you indicate that you	\n
 * have read the license and understand and accept it fully.			\n
 *  										\n
 * The LCUI project is distributed in the hope that it will be useful, but 	\n
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 	\n
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.	\n
 * 										\n
 * You should have received a copy of the GPLv2 along with this file. It is 	\n
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.	\n
 * ******************************************************************************/
 
/** ******************************************************************************
 * @file	fontlibrary.h
 * @brief	字体数据库管理模块。
 * @author	刘超 <lc-soft@live.cn>
 * @warning
 * 版权所有 (C) 2012-2013 归属于						\n
 * 刘超										\n
 * 										\n
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。	\n
 * 										\n
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)				\n
 * 										\n
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。	\n
 * 										\n
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特定\n
 * 用途的隐含担保，详情请参照GPLv2许可协议。					\n
 * 										\n
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果	\n
 * 没有，请查看：<http://www.gnu.org/licenses/>. 				\n
 * ******************************************************************************/

#ifndef __LCUI_FONT_LIBRARY_H__
#define __LCUI_FONT_LIBRARY_H__

LCUI_BEGIN_HEADER

/** 初始化字体数据库 */
LCUI_API void FontLIB_Init( void );

/** 通过字体文件路径来查找字体信息，并获取字体ID */
LCUI_API int FontLIB_FindInfoByFilePath( const char *filepath );

/** 获取指定字族名的字体ID */
LCUI_API int FontLIB_GetFontIDByFamilyName( const char *family_name );

/** 获取默认的字体ID */
LCUI_API int FontLIB_GetDefaultFontID( void );

/** 设定默认的字体 */
LCUI_API void FontLIB_SetDefaultFont( int id );

/**
 * 添加一个字体位图数据至数据库中
 * @warning
 * 调用此函数后，作为参数fontbmp_buff的变量，不能被free掉，否则，数据库中记录
 * 的此数据会无效 
 * */
LCUI_API LCUI_FontBMP* FontLIB_AddFontBMP(	wchar_t char_code,
						int font_id,
						int pixel_size,
						LCUI_FontBMP *fontbmp_buff );

/** 获取库中的字体位图 */
LCUI_API LCUI_FontBMP* FontLIB_GetFontBMP(	wchar_t char_code,
						int font_id,
						int pixel_size );

/** 获取库中现有的字体位图，若没有则载入新的 */
LCUI_API LCUI_FontBMP* FontLIB_GetExistFontBMP(	int font_id,
						wchar_t ch,
						int pixel_size );

/** 载入字体值数据库中 */
LCUI_API int FontLIB_LoadFontFile( const char *filepath );

LCUI_END_HEADER

#endif
