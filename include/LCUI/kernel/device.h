
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

/* 初始化设备处理模块 */
int LCUIModule_Device_Init();

/* 停用设备处理模块 */
void LCUIModule_Device_End();

LCUI_END_HEADER

#endif
