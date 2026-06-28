# 定时器 timer

定时器用于将一些操作推迟到指定时间之后执行。

timer 需要开发者基于定时器接口自己实现一个定时器线程。

下面以 LCUI 中实现的主线程定时器为例：

```c
/*----------------------------- Timer --------------------------------*/

static timer_list_t *main_timer_list = NULL;
static LCUI_Mutex mutex; /**< 定时器记录操作互斥锁 */

/*----------------------------- Private ------------------------------*/

//处理定时器链表
size_t lcui_timer_list_process()
{
	size_t count = 0;
	LCUIMutex_Lock(&mutex);
	count = timer_list_process(main_timer_list);
	LCUIMutex_Unlock(&mutex);
	return count;
}

//初始化定时器列表
void lcui_timer_list_create(void)
{
	LCUIMutex_Init(&mutex);
	main_timer_list = timer_list_create();
}

//销毁定时器列表
void lcui_timer_list_destroy()
{
	if (!main_timer_list) {
		return;
	}
	timer_list_destroy(main_timer_list);
	LCUIMutex_Destroy(&mutex);
	main_timer_list = NULL;
}

//销毁定时器
int lcui_timer_destroy(int timer_id)
{
	int ret;
	//加锁
	LCUIMutex_Lock(&mutex);
	ret = timer_destroy(main_timer_list, timer_id);
	LCUIMutex_Unlock(&mutex);
	return ret;
}

//暂定操作
int lcui_timer_pause(int timer_id)
{
	int ret;
	LCUIMutex_Lock(&mutex);
	ret = timer_pause(main_timer_list, timer_id);
	LCUIMutex_Unlock(&mutex);
	return ret;
}

//继续被暂定的定时器
int lcui_timer_continue(int timer_id)
{
	int ret;
	LCUIMutex_Lock(&mutex);
	ret = timer_continue(main_timer_list, timer_id);
	LCUIMutex_Unlock(&mutex);
	return ret;
}

//重新设置定时器的间隔
int lcui_timer_reset(int timer_id, long int n_ms)
{
	int ret;
	LCUIMutex_Lock(&mutex);
	ret = timer_reset(main_timer_list, timer_id, n_ms);
	LCUIMutex_Unlock(&mutex);
	return ret;
}

//添加执行一次的定时器
int lcui_timer_add_timeout(long int n_ms, void (*callback)(void *), void *arg)
{
	return timer_list_add_timeout(main_timer_list, n_ms, callback, arg);
}

//添加定时重复的定时器
int lcui_timer_add_interval(long int n_ms, void (*callback)(void *), void *arg)
{
	return timer_list_add_interval(main_timer_list, n_ms, callback, arg);
}

```