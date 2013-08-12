/** ******************************************************************************
 * @file	sleeper.c
 * @brief	a sleep function, can be break
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
 * @file	sleeper.c
 * @brief	可被打断的睡眠功能
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

#include <LCUI_Build.h>
#include LC_LCUI_H

/** 新建一个睡眠者 */
LCUI_Sleeper LCUISleeper_New(void)
{
	/* 创建一个事件对象：
	 * 使用默认安全符
	 * 在事件被释放后，自动复原为无信号状态
	 * 初始状态为无信号状态
	 * 不指定名字，无名
	 */
	return CreateEvent( NULL, FALSE, FALSE, NULL );
}

/** 让指定睡眠者睡一段时间 */
unsigned int LCUISleeper_StartSleep( LCUI_Sleeper waiter, unsigned int ms )
{
	int ret;
	int64_t lost_time;
	lost_time = LCUI_GetTickCount();
	ret = WaitForSingleObject( waiter, ms );
	lost_time = LCUI_GetTicks( lost_time );
	switch( ret ) {
	case WAIT_OBJECT_0: return (unsigned int)lost_time;
	case WAIT_TIMEOUT: return ms;
	case WAIT_FAILED: 
	default: break;
	}
	return 0;
}

/** 打断目标睡眠者的睡眠 */
int LCUISleeper_BreakSleep( LCUI_Sleeper waiter )
{
	if( SetEvent( waiter ) ) {
		return 0;
	}
	return -1;
}
