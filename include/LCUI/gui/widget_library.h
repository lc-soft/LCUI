/* ***************************************************************************
 * widget_library.h -- LCUI widget library management module.
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
 * ***************************************************************************/
 
/* ****************************************************************************
 * widget_library.h -- LCUI部件库管理模块
 *
 * 版权所有 (C) 2012-2014 归属于 刘超 <lc-soft@live.cn>
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

#ifndef __LCUI_WIDGET_LIBRARY_H__
#define __LCUI_WIDGET_LIBRARY_H__

LCUI_BEGIN_HEADER

typedef struct LCUI_WidgetClass {
	char *name;	/**< 类名 */
	struct {
		/** 构造函数 */
		void (*init)(LCUI_Widget);
		/** 析构函数 */		
		void (*destroy)(LCUI_Widget);
		/** 大小计算，用于部件的自动大小调整功能 */
		void (*autosize)(LCUI_Widget, int*, int*);
		/** 扩展样式的更新处理函数 */
		void (*update)(LCUI_Widget);
		/** 绘制函数 */
		void (*paint)(LCUI_Widget, LCUI_PaintContext);
	} methods;	/**< 该类部件的自定义方法集 */
	/** 部件任务处理器 */
	void (*task_handler)(LCUI_Widget);
} LCUI_WidgetClass;

void LCUIWidget_InitLibrary(void);

void LCUIWidget_DestroyLibrary(void);

/** 获取部件类数据 */
LCUI_API LCUI_WidgetClass* LCUIWidget_GetClass( const char *class_name );

/** 添加一个部件类型 */
LCUI_API LCUI_WidgetClass* LCUIWidget_AddClass( const char *class_name );

/** 移除一个部件类型 */
LCUI_API int LCUIWidget_RemoveClass( const char *class_name );


LCUI_END_HEADER

#endif
