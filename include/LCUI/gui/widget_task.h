/* ***************************************************************************
 * widget_task.h -- LCUI widget task module.
 * 
 * Copyright (C) 2014 by Liu Chao <lc-soft@live.cn>
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
 * widget_task.h -- LCUI部件任务处理模块
 *
 * 版权所有 (C) 2014 归属于 刘超 <lc-soft@live.cn>
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

#ifndef __LCUI_WIDGET_TASK_H__
#define __LCUI_WIDGET_TASK_H__

LCUI_BEGIN_HEADER

/** 部件任务类型，主要是按所对应的部件属性或具体刷新区域进行区分 */
enum WidgetTaskType {
	WTT_AUTO_SIZE,
	WTT_AUTO_LAYOUT,
	WTT_TITLE,
	WTT_PADDING,
	WTT_MARGIN,
	WTT_MOVE,
	WTT_SHOW,
	WTT_SHADOW,
	WTT_BORDER,
	WTT_RESIZE,
	WTT_OPACITY,
	WTT_BODY,
	WTT_REFRESH,
	WTT_USER,
	WTT_DESTROY
};

#define WTT_TOTAL_NUM (WTT_DESTROY+1)

typedef struct LCUI_WidgetTask {
	int type;
	/** 主要用于记录更新前的属性值，在更新时通过对比新旧属性来计算脏矩形 */
	union {
		struct {
			int x, y;
		} move;				/**< 移动位置 */
		struct {
			int w, h;
		} resize;			/**< 调整大小 */
		LCUI_BoxShadow shadow;		/**< 阴影 */
		LCUI_Border border;		/**< 边框 */
		LCUI_BOOL visible;		/**< 显示/隐藏 */
		float opacity;			/**< 不透明度 */
		void *data;			/**< 自定义任务数据 */
	};
} LCUI_WidgetTask;	/**< 部件任务数据 */

#ifndef __IN_WIDGET_TASK_SOURCE_FILE__
typedef void* LCUI_WidgetTaskBox;
#else
typedef struct LCUI_WidgetTaskBoxRec_* LCUI_WidgetTaskBox;
#endif

/** 更新当前任务状态，确保部件的任务能够被处理到 */
LCUI_API void Widget_UpdateTaskStatus( LCUI_Widget widget );

/** 添加任务 */
LCUI_API int Widget_AddTask( LCUI_Widget widget, LCUI_WidgetTask *data );

/** 初始化部件的任务处理 */
void Widget_InitTaskBox( LCUI_Widget widget );

/** 销毁（释放）部件的任务处理功能的相关资源 */
void Widget_DestroyTaskBox( LCUI_Widget widget );

/** 初始化 LCUI 部件任务处理功能 */
void LCUIWidget_Task_Init(void);

/** 销毁（释放） LCUI 部件任务处理功能的相关资源 */
void LCUIWidget_Task_Destroy(void);

/** 处理一次当前积累的部件任务 */
void LCUIWidget_Task_Step(void);

LCUI_END_HEADER

#endif
