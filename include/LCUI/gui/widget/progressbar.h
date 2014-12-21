/* ***************************************************************************
 * progressbar.h -- LCUI's ProgressBar widget
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
 * progresspar.h -- LCUI 的进度条部件
 *
 * 版权所有 (C) 2012-2013 归属于
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


#ifndef __LCUI_PROGBAR_H__
#define __LCUI_PROGBAR_H__

LCUI_BEGIN_HEADER
	
enum ProgressBarStyle {
	PROGBAR_STYLE_BLUE,
	PROGBAR_STYLE_GREEN,
	PROGBAR_STYLE_RED
};

typedef struct LCUI_ProgressBar_
{
	LCUI_Widget fore_wdg;		/* 进度条 */ 
	LCUI_Widget glisten_wdg;	/* 闪光 */
	
	int move_speed;		/* 闪光的移动速度，单位为：像素/秒 */
	int sleep_time;		/* 闪光每趟移动的间隔时间，单位为：毫秒 */
	int max_value;		/* 总进度的值 */
	int value;		/* 当前进度的值 */
} LCUI_ProgressBar;

/* 设定进度条最大值 */
LCUI_API void ProgressBar_SetMaxValue(LCUI_Widget widget, int max_value);

/* 获取进度条最大值 */
LCUI_API int ProgressBar_GetMaxValue( LCUI_Widget widget );

/* 设定进度条当前值 */
LCUI_API void ProgressBar_SetValue( LCUI_Widget widget, int value );

/* 获取进度条当前值 */
LCUI_API int ProgressBar_GetValue( LCUI_Widget widget );

/* 注册进度条部件类型 */
LCUI_API void Register_ProgressBar(void);

LCUI_END_HEADER

#endif
