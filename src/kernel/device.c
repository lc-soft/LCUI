/* ***************************************************************************
 * device.c -- The input device processing module
 * 
 * Copyright (C) 2012-2014 by
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
 * 版权所有 (C) 2012-2014 归属于
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

/** 
 * 本模块主要是为了将键盘、鼠标的输入处理放在一个线程上进行，看起来没多大作用，
 * 就只减少一个线程，至于是否需要整改，具体看以后的情况了。
 */

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/kernel/device.h>
#include <LCUI/misc/linkedlist.h>
#include <LCUI/misc/delay.h>
#include <LCUI/thread.h>

typedef struct _dev_data_rec_ {
	LCUI_BOOL (*init)(void);
	LCUI_BOOL (*proc)(void);
	LCUI_BOOL (*exit)(void);
} DeviceData;

static LCUI_BOOL is_running;
static LCUI_Mutex list_mutex;
static LinkedList dev_list;
static LCUI_Thread dev_tid;

int LCUIDevice_Add( LCUI_BOOL (*init)(void), LCUI_BOOL (*proc)(void),
		    LCUI_BOOL (*exit)(void) )
{
	DeviceData *dev = NEW(DeviceData, 1);
	dev->init = init;
	dev->proc = proc;
	dev->exit = exit;
	LCUIMutex_Lock( &list_mutex );
	if( !LinkedList_Append( &dev_list, dev ) ) {
		LCUIMutex_Unlock( &list_mutex );
		return -1;
	}
	LCUIMutex_Unlock( &list_mutex );
	if( dev->init ) {
		dev->init();
	}
	return 0;
}

/** 设备处理线程 */
static void DeviceThread( void *arg )
{
	DeviceData *dev;
	LinkedListNode *node;
	int timeout = 0;
	
	is_running = TRUE;
	while( is_running ) {
		LCUIMutex_Lock( &list_mutex );
		LinkedList_ForEach( node, &dev_list ) {
			dev = (DeviceData*)node->data;
			if( !dev || !dev->proc ) {
				continue;
			}
			if( dev->proc() ) {
				++timeout;
			}
		}
		LCUIMutex_Unlock( &list_mutex );
		if( timeout > 20 ) {
			LCUI_MSleep( 50 );
			timeout = 0;
		}
		LCUI_MSleep( 10 );
	}
	LCUIThread_Exit(NULL);
}

/** 初始化设备处理模块 */
int LCUI_InitDevice(void)
{
	LCUIMutex_Init( &list_mutex );
	LinkedList_Init( &dev_list );
	return LCUIThread_Create( &dev_tid, DeviceThread, NULL );
}

/** 停用设备处理模块 */
void LCUI_ExitDevice(void)
{
	DeviceData *dev;
	LinkedListNode *node;
	
	LCUIMutex_Lock( &list_mutex );
	LinkedList_ForEach( node, &dev_list ) {
		dev = (DeviceData*)node->data;
		if( dev && dev->exit ) {
			dev->exit();
		}
	}
	LinkedList_Clear( &dev_list, free );
	LCUIMutex_Unlock( &list_mutex );
	LCUIMutex_Destroy( &list_mutex );
}
