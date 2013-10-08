/** ******************************************************************************
 * @file	LCUI_Memory.h
 * @brief	memory management.
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
 * @file	LCUI_Memory.h
 * @brief	内存管理.
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
  *用途的隐含担保，详情请参照GPLv2许可协议。					\n
 * 										\n
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果	\n
 * 没有，请查看：<http://www.gnu.org/licenses/>. 				\n
 * ******************************************************************************/

#ifndef __LCUI_MEMORY_H__ 
#define __LCUI_MEMORY_H__ 

LCUI_BEGIN_HEADER

/** 初始化LCUI的内存管理模块 */
void LCUIMM_Init(void);

/**
 * 添加新的类别记录
 *
 * @class_name[in]
 *	类别名
 * @return
 *	与该类别名对应的ID
 * @warning
 *	class_name指向的最好是全局常量字符串，不能是生存周期短的局部变量
 */
unsigned int LCUIMM_NewClass( char *class_name );

LCUI_API void LCUIMM_PrintAllClassInfo( void );

/**
 * 分配指定大小的内存空间
 *
 * @param[in] 
 *	size 要分配的内存空间的大小(字节)
 * @param[in]
 *	class_id 该内存的分类ID，用于统计不同用途的内存空间大小
 * @return
 *	正常返回分配的内存空间的首地址，失败返回NULL
 */
void *LCUIMM_Alloc( size_t size, unsigned int class_id );

/** 
 * 释放已动态分配的内存空间
 *
 * @param[in]
 *	mem_ptr	指向要释放的内存空间的指针
 * @return
 *	正常返回0，内存不能被释放则返回-1
 */
int LCUIMM_Free( void *mem_ptr );

size_t LCUIMM_GetMemSize( unsigned int id );

size_t LCUIMM_GetMemSizeByName( char *mem_class_name );

LCUI_END_HEADER

#endif
