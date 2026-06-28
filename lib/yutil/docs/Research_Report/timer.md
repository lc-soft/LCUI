# timer



## LCUI

**实现方式：**

```c

/*----------------------------- Timer --------------------------------*/

typedef struct TimerRec_ {
	int state;			/**< 状态 */
	long int id;			/**< 定时器ID */
	LCUI_BOOL reuse;		/**< 是否重复使用该定时器 */

	int64_t start_time;		/**< 定时器启动时的时间 */
	int64_t pause_time;		/**< 定时器暂停时的时间 */
	long int total_ms;		/**< 定时时间（单位：毫秒） */
	long int pause_ms;		/**< 定时器处于暂停状态的时长（单位：毫秒） */

	void (*callback)(void *);	/**< 回调函数 */
	void *arg;			/**< 函数的参数 */

	LinkedListNode node;		/**< 位于定时器列表中的结点 */
} TimerRec, *Timer;

static struct TimerModule {
	int id_count;         /**< 定时器ID计数 */
	LCUI_BOOL active;     /**< 定时器线程是否正在运行 */
	LCUI_Mutex mutex;     /**< 定时器记录操作互斥锁 */
	LinkedList timers;    /**< 定时器数据记录 */
} self;

```





## 结论

1. 重新设计结构：原有的定时器结构依赖过多，重新设计的**结构要求**：

   - 尽量保留原有结构，去掉过多的依赖项。

   重新设计的数据结构使用**有序链表**。

   - 具有**有序性**，可以**快速**找到**到期**任务；
   - 其**过期执行**、**插入**(添加定时任务)和**删除**(取消定时任务)的频率比较高。

2. 执行到期任务：**不依赖事件循环，**列如，用一个列表记录定时器，每当处理函数被调用时从列表取出已超时的定时器，然后调用这些定时器的回调函数。

3. 不再是全局共用同一个定时器列表。

参考文章：

- [如何实现一个定时器？看这一篇就够了-技术圈 (proginn.com)](https://jishuin.proginn.com/p/763bfbd54473)

