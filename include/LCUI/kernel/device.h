
#ifndef __LCUI_KERNEL_DEVICE_H__
#define __LCUI_KERNEL_DEVICE_H__

typedef struct _dev_func_data
{
	BOOL (*init_func)();
	BOOL (*proc_func)();
	BOOL (*destroy_func)();
}
dev_func_data;

LCUI_BEGIN_HEADER

/* 
 * 功能：注册设备
 * 说明：为指定设备添加处理函数
 * */
int LCUI_Dev_Add(	BOOL (*init_func)(), 
			BOOL (*proc_func)(), 
			BOOL (*destroy_func)() );

/* 初始化设备 */
int LCUI_Dev_Init();

/* 撤销设备线程 */
void LCUI_Dev_Destroy();

LCUI_END_HEADER

#endif
