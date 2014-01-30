/* ***************************************************************************
 * fontlibrary.h -- The font info and font bitmap cache module.
 * 
 * Copyright (C) 2012-2014 by Liu Chao <lc-soft@live.cn>
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
 * fontlibrary.h -- 字体信息和字体位图缓存模块。
 *
 * 版权所有 (C) 2012-2013 归属于 刘超 <lc-soft@live.cn>
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
