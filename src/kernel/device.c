#include <LCUI_Build.h>
#include LC_LCUI_H

/*----------------------------- Device -------------------------------*/
/* 设备列表初始化 */
static void 
dev_list_init( LCUI_Queue *dev_list )
{
	Queue_Init( dev_list, sizeof(dev_func_data), NULL );
}

/* 
 * 功能：注册设备
 * 说明：为指定设备添加处理函数
 * */
int LCUI_Dev_Add(	BOOL (*init_func)(), 
			BOOL (*proc_func)(), 
			BOOL (*destroy_func)() )
{
	dev_func_data data;
	
	if( init_func ) {
		init_func();
	}
	data.init_func = init_func;
	data.proc_func = proc_func;
	data.destroy_func = destroy_func;
	if( 0<= Queue_Add( &LCUI_Sys.dev_list, &data ) ) {
		return 0;
	}
	return -1;
}

/* 处理列表中的设备的数据 */
static void
proc_dev_list ( void *arg )
{
	LCUI_Queue *dev_list;
	dev_func_data *data_ptr;
	int total, i, result, sleep_time = 1;
	
	dev_list = (LCUI_Queue *)arg;
	while( LCUI_Active() ) {
		result = 0;
		total = Queue_Get_Total( dev_list );
		for(i=0; i<total; ++i) {
			data_ptr = Queue_Get( dev_list, i );
			if( !data_ptr || !data_ptr->proc_func ) {
				continue;
			}
			result += data_ptr->proc_func();
		}
		if( result > 0 ) {
			sleep_time = 1;
		} else {
			LCUI_MSleep( sleep_time );
			if( sleep_time < 100 ) {
				sleep_time += 1;
			}
		}
	}
	LCUIThread_Exit(NULL);
}

/* 初始化设备处理模块 */
int LCUIModule_Device_Init()
{
	dev_list_init( &LCUI_Sys.dev_list );
	return _LCUIThread_Create( &LCUI_Sys.dev_thread,
			proc_dev_list, &LCUI_Sys.dev_list );
}

/* 停用设备处理模块 */
void LCUIModule_Device_End()
{
	int total, i;
	dev_func_data *data_ptr;
	LCUI_Queue *dev_list;
	
	dev_list = &LCUI_Sys.dev_list; 
	total = Queue_Get_Total( dev_list );
	for(i=0; i<total; ++i) {
		data_ptr = Queue_Get( dev_list, i );
		if( !data_ptr || !data_ptr->destroy_func ) {
			continue;
		}
		data_ptr->destroy_func();
	}
}
/*--------------------------- End Device -----------------------------*/
