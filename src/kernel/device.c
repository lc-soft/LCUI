/* ***************************************************************************
 * device.c -- The input device processing module
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
 * device.c -- 输入设备的处理模块
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
#include <LCUI_Build.h>
#include LC_LCUI_H

static LCUI_Queue dev_list; /* 设备列表 */

/*----------------------------- Device -------------------------------*/
/** 设备列表初始化 */
static void dev_list_init( LCUI_Queue *dev_list )
{
	Queue_Init( dev_list, sizeof(dev_func_data), NULL );
}

/** 
 * 功能：注册设备
 * 说明：为指定设备添加处理函数
 * */
LCUI_API int LCUIDevice_Add(	LCUI_BOOL (*init_func)(void), 
				LCUI_BOOL (*proc_func)(void), 
				LCUI_BOOL (*destroy_func)(void) )
{
	dev_func_data data;
	
	if( init_func ) {
		init_func();
	}
	data.init_func = init_func;
	data.proc_func = proc_func;
	data.destroy_func = destroy_func;
	Queue_Lock( &dev_list );
	if( Queue_Add( &dev_list, &data ) ) {
		Queue_Unlock( &dev_list );
		return 0;
	}
	Queue_Unlock( &dev_list );
	return -1;
}

/** 处理列表中的设备的数据 */
static void proc_dev_list( void *arg )
{
	LCUI_Queue *dev_list;
	dev_func_data *data_ptr;
	int total, i, timeout_count = 0;
	
	dev_list = (LCUI_Queue *)arg;
	while( LCUI_Active() ) {
		Queue_Lock( dev_list );
		total = Queue_GetTotal( dev_list );
		for(i=0; i<total; ++i) {
			data_ptr = (dev_func_data*)Queue_Get( dev_list, i );
			if( !data_ptr || !data_ptr->proc_func ) {
				continue;
			}
			if( data_ptr->proc_func() ) {
				++timeout_count;
			}
		}
		Queue_Unlock( dev_list );
		if( timeout_count > 20 ) {
			LCUI_MSleep( 10 );
			timeout_count = 0;
		}
		LCUI_MSleep( 5 );
	}
	LCUIThread_Exit(NULL);
}

/** 初始化设备处理模块 */
LCUI_API int LCUIModule_Device_Init(void)
{
	dev_list_init( &dev_list );
	return _LCUIThread_Create(	&LCUI_Sys.dev_thread,
					proc_dev_list, &dev_list );
}

/** 停用设备处理模块 */
LCUI_API void LCUIModule_Device_End(void)
{
	int total, i;
	dev_func_data *data_ptr;
	
	Queue_Lock( &dev_list );
	total = Queue_GetTotal( &dev_list );
	for(i=0; i<total; ++i) {
		data_ptr = (dev_func_data*)Queue_Get( &dev_list, i );
		if( !data_ptr || !data_ptr->destroy_func ) {
			continue;
		}
		data_ptr->destroy_func();
	}
	Queue_Unlock( &dev_list );
}
/*--------------------------- End Device -----------------------------*/
