
#ifndef __LCUI_KERNEL_TASK_H__
#define __LCUI_KERNEL_TASK_H__

LCUI_BEGIN_HEADER

typedef struct {
	/* 
	 * 函数ID，部件库需要这ID标识函数类型，往程序的任务队列添加
	 * 任务也需要该ID来标识目标程序ID 
	 * */
	LCUI_ID id;
	void (*func)();  /* 函数指针 */
	
	/* 以下参数该怎么传给回调函数，具体要看是如何处理事件的 */  
	void *arg[2];		/* 传给函数的两个参数 */
	LCUI_BOOL destroy_arg[2];	/* 指定是否在调用完回调函数后，销毁参数 */
} LCUI_Func, LCUI_Task;

LCUI_EXPORT(void) AppTasks_Init( LCUI_Queue *tasks );

/*
 * 功能：发送任务给程序，使这个程序进行指定任务
 * 说明：LCUI_Task结构体中的成员变量 id，保存的是目标程序的id
 */
LCUI_EXPORT(int) AppTasks_Add( LCUI_Task *task );

/*
 * 功能：使用自定义方式添加程序任务
 * 用法示例：
 * 在函数的各参数与队列中的函数及各参数不重复时，添加它
 * AppTasks_CustomAdd(ADD_MODE_NOT_REPEAT | AND_ARG_F | AND_ARG_S, task);
 * 只要函数和参数1不重复则添加
 * AppTasks_CustomAdd(ADD_MODE_NOT_REPEAT | AND_ARG_F, task);
 * 要函数不重复则添加
 * AppTasks_CustomAdd(ADD_MODE_NOT_REPEAT, task);
 * 添加新的，不管是否有重复的
 * AppTasks_CustomAdd(ADD_MODE_ADD_NEW, task);
 * 有相同函数则覆盖，没有则新增
 * AppTasks_CustomAdd(ADD_MODE_REPLACE, task);
 * */
LCUI_EXPORT(int) AppTasks_CustomAdd( int mode, LCUI_Task *task );

LCUI_END_HEADER

#endif
