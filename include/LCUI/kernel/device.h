
#ifndef __LCUI_KERNEL_DEVICE_H__
#define __LCUI_KERNEL_DEVICE_H__

LCUI_BEGIN_HEADER

typedef struct _dev_func_data
{
	LCUI_BOOL (*init_func)();
	LCUI_BOOL (*proc_func)();
	LCUI_BOOL (*destroy_func)();
}
dev_func_data;

/* 
 * 功能：注册设备
 * 说明：为指定设备添加处理函数
 * */
LCUI_EXPORT(int)
LCUI_Dev_Add(	LCUI_BOOL (*init_func)(), 
		LCUI_BOOL (*proc_func)(), 
		LCUI_BOOL (*destroy_func)() );

/* 初始化设备处理模块 */
LCUI_EXPORT(int) LCUIModule_Device_Init(void);

/* 停用设备处理模块 */
LCUI_EXPORT(void) LCUIModule_Device_End(void);

LCUI_END_HEADER

#endif
