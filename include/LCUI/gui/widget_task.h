/* ***************************************************************************
 * widget_task.h -- LCUI widget task module.
 * 
 * Copyright (C) 2014-2017 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2014-2017 归属于 刘超 <lc-soft@live.cn>
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

#ifndef LCUI_WIDGET_TASK_H
#define LCUI_WIDGET_TASK_H

LCUI_BEGIN_HEADER

/** 更新当前任务状态，确保部件的任务能够被处理到 */
LCUI_API void Widget_UpdateTaskStatus( LCUI_Widget widget );

/** 添加任务 */
LCUI_API void Widget_AddTask( LCUI_Widget widget, int task_type );

/** 处理部件中当前积累的任务 */
LCUI_API int Widget_Update( LCUI_Widget w );

/** 将部件标记为垃圾，等待销毁 */
LCUI_API void Widget_AddToTrash( LCUI_Widget w );

/** 为子级部件添加任务 */
LCUI_API void Widget_AddTaskForChildren( LCUI_Widget widget, int task );

/** 初始化 LCUI 部件任务处理功能 */
LCUI_API void LCUIWidget_InitTasks( void );

/** 销毁（释放） LCUI 部件任务处理功能的相关资源 */
LCUI_API void LCUIWidget_FreeTasks( void );

/** 处理一次当前积累的部件任务 */
LCUI_API void LCUIWidget_Update( void );

/** 刷新所有部件的样式 */
LCUI_API void LCUIWidget_RefreshStyle( void );

LCUI_END_HEADER

#endif
