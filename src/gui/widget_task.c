/* ***************************************************************************
 * widget_task.c -- LCUI widget task module.
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
 * widget_task.c -- LCUI部件任务处理模块
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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/misc/rbtree.h>
#include <LCUI/widget_build.h>

typedef struct LCUI_WidgetTaskPack {
	int type;				/**< 任务类型 */
	LCUI_BOOL is_valid;			/**< 是否有任务 */
	LCUI_Widget target;			/**< 目标部件 */
	LCUI_WidgetTask data;			/**< 任务数据 */
} LCUI_WidgetTaskPack;

static int using_tree_n = 0;		/**< 当前使用的记录 */
static LCUI_RBTree widget_tree[2];	/**< 两个记录 */

/** 添加任务 */
int Widget_AddTask( LCUI_Widget widget, int type, LCUI_WidgetTask *data )
{
	return 0;
}

/** 初始化 LCUI 部件任务处理功能 */
void LCUIWidget_Task_Init(void)
{

}

/** 销毁（释放） LCUI 部件任务处理功能的相关资源 */
void LCUIWidget_Task_Destroy(void)
{

}

/** 处理一次当前积累的部件任务 */
void LCUIWidget_Task_Step(void)
{

}
