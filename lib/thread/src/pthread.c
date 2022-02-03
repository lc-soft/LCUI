
#ifndef _WIN32

#include <errno.h>
#include <sys/time.h>
#include <pthread.h>
#include <LCUI/util.h>
#include "../include/thread.h"

typedef union thread_mutex_record_t {
	pthread_mutex_t handle;
} thread_mutex_record_t;

typedef union thread_cond_record_t {
	pthread_cond_t handle;
} thread_cond_record_t;

typedef struct thread_info_t {
	void (*func)(void *);
	void *arg;
	thread_t tid;
	list_node_t node;
} thread_info_t;

static struct thread_manager_module_t {
	int active;
	thread_mutex_t mutex;
	list_t threads;
} thread_manager;

/** 初始化一个条件变量 */
int thread_cond_init(thread_cond_t *cond)
{
	*cond = malloc(sizeof(thread_cond_record_t));
	if (!*cond) {
		return -1;
	}
	if (pthread_cond_init(&(*cond)->handle, NULL) != 0) {
		free(*cond);
		*cond = NULL;
		return -1;
	}
	return 0;
}

/** 销毁一个条件变量 */
int thread_cond_destroy(thread_cond_t *cond)
{
	if (pthread_cond_destroy(&(*cond)->handle) != 0) {
		return -1;
	}
	free(*cond);
	*cond = NULL;
	return 0;
}

/** 阻塞当前线程，等待条件成立 */
int thread_cond_wait(thread_cond_t *cond, thread_mutex_t *mutex)
{
	return pthread_cond_wait(&(*cond)->handle, &(*mutex)->handle);
}

/** 计时阻塞当前线程，等待条件成立 */
int thread_cond_timedwait(thread_cond_t *cond, thread_mutex_t *mutex,
			  unsigned int ms)
{
	int ret;
	long int out_usec;
	struct timeval now;
	struct timespec outtime;

	gettimeofday(&now, NULL);
	out_usec = now.tv_usec + ms * 1000;
	outtime.tv_sec = now.tv_sec + out_usec / 1000000;
	outtime.tv_nsec = (out_usec % 1000000) * 1000;
	DEBUG_MSG("wait, ms = %u, outtime.tv_nsec: %ld\n", ms, outtime.tv_nsec);
	ret = pthread_cond_timedwait(&(*cond)->handle, &(*mutex)->handle, &outtime);
	DEBUG_MSG("ret: %d, ETIMEDOUT = %d, EINVAL = %d\n", ret, ETIMEDOUT,
		  EINVAL);
	switch (ret) {
	case 0:
		return 0;
	case ETIMEDOUT:
		return 1;
	default:
		break;
	}
	return -1;
}

/** 唤醒一个阻塞等待条件成立的线程 */
int thread_cond_signal(thread_cond_t *cond)
{
	return pthread_cond_signal(&(*cond)->handle);
}

/** 唤醒所有阻塞等待条件成立的线程 */
int thread_cond_broadcast(thread_cond_t *cond)
{
	return pthread_cond_broadcast(&(*cond)->handle);
}

/* init the mutex */
int thread_mutex_init(thread_mutex_t *mutex)
{
	pthread_mutexattr_t attr;

	*mutex = malloc(sizeof(thread_mutex_record_t));
	if (!*mutex) {
		return -1;
	}
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	if (pthread_mutex_init(&(*mutex)->handle, &attr) != 0) {
		free(*mutex);
		*mutex = NULL;
		return -1;
	}
	return 0;
}

/* Free the mutex */
void thread_mutex_destroy(thread_mutex_t *mutex)
{
	pthread_mutex_destroy(&(*mutex)->handle);
	free(*mutex);
	*mutex = NULL;
}

/* Try lock the mutex */
int thread_mutex_trylock(thread_mutex_t *mutex)
{
	return pthread_mutex_trylock(&(*mutex)->handle);
}

/* Lock the mutex */
int thread_mutex_lock(thread_mutex_t *mutex)
{
	return pthread_mutex_lock(&(*mutex)->handle);
}

/* Unlock the mutex */
int thread_mutex_unlock(thread_mutex_t *mutex)
{
	return pthread_mutex_unlock(&(*mutex)->handle);
}

static void *run_thread(void *arg)
{
	thread_info_t *thread = arg;

	thread->func(thread->arg);
	thread_mutex_lock(&thread_manager.mutex);
	list_unlink(&thread_manager.threads, &thread->node);
	thread_mutex_unlock(&thread_manager.mutex);
	free(thread);
	pthread_exit(NULL);
}

static thread_info_t *get_thread_info(thread_t tid)
{
	list_node_t *node;
	thread_info_t *thread;

	for (list_each(node, &thread_manager.threads)) {
		thread = node->data;
		if (thread && thread->tid == tid) {
			return thread;
		}
	}
	return NULL;
}

static thread_info_t *unlink_thread_info(thread_t tid)
{
	thread_info_t *thread;

	thread_mutex_lock(&thread_manager.mutex);
	thread = get_thread_info(tid);
	if (thread) {
		list_unlink(&thread_manager.threads, &thread->node);
	}
	thread_mutex_unlock(&thread_manager.mutex);
	return thread;
}

int thread_create(thread_t *tid, void (*func)(void *), void *arg)
{
	int ret;
	thread_info_t *thread;

	if (!thread_manager.active) {
		list_create(&thread_manager.threads);
		thread_mutex_init(&thread_manager.mutex);
		thread_manager.active = TRUE;
	}
	thread = malloc(sizeof(thread_info_t));
	if (!thread) {
		return -ENOMEM;
	}
	thread->arg = arg;
	thread->func = func;
	thread->node.data = thread;
	ret = pthread_create(&thread->tid, NULL, run_thread, thread);
	if (ret != 0) {
		free(thread);
		return ret;
	}
	thread_mutex_lock(&thread_manager.mutex);
	list_append_node(&thread_manager.threads, &thread->node);
	thread_mutex_unlock(&thread_manager.mutex);
	*tid = thread->tid;
	return ret;
}

thread_t thread_self(void)
{
	return pthread_self();
}

void thread_exit(void *retval)
{
	thread_t tid;
	thread_info_t *thread;

	tid = thread_self();
	thread = unlink_thread_info(tid);
	if (thread) {
		free(thread);
	}
	pthread_exit(retval);
}

void thread_cancel(thread_t tid)
{
#ifdef PTHREAD_CANCEL_ASYNCHRONOUS
	pthread_cancel(tid);
#else
	pthread_kill(tid, SIGKILL);
#endif
}

int thread_join(thread_t tid, void **retval)
{
	return pthread_join(tid, retval);
}

#endif
