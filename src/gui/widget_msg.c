#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H

static void WidgetMsg_Destroy( void *arg )
{

}

LCUI_API void WidgetMsgBuff_Init( LCUI_Widget *widget )
{
	LCUI_Queue *msg_buff;
	msg_buff = Widget_GetMsgBuff( widget );
	Queue_Init( msg_buff, sizeof(WidgetMsgData), WidgetMsg_Destroy );
}

LCUI_API void WidgetMsgFunc_Init( LCUI_Widget *widget )
{
	LCUI_Queue *msg_func;
	msg_func = Widget_GetMsgFunc( widget );
	Queue_Init( msg_func, sizeof(LCUI_Func), NULL );
}

LCUI_API int WidgetMsg_AddToTask( LCUI_Widget *widget, WidgetMsgData *data_ptr )
{
	int i,n;
	LCUI_Queue *msg_func;
	LCUI_Task *task_ptr, task;
	
	/* LCUI系统消息不能作为任务让程序在主循环里处理 */
	if( data_ptr->msg_id < WIDGET_USER ) {
		return -1;
	}
	msg_func = Widget_GetMsgFunc( widget );
	if( msg_func == NULL ) {
		return -2;
	}
	n = Queue_GetTotal( msg_func );
	for(i=0; i<n; ++i) {
		task_ptr = (LCUI_Task*)Queue_Get( msg_func, i );
		if( task_ptr == NULL ) {
			continue;
		}
		if( task_ptr->id != data_ptr->msg_id ) {
			continue;
		}
		task.func = task_ptr->func;
		task.arg[0] = widget;
		task.arg[1] = data_ptr->data.ptr;
		task.destroy_arg[0] = FALSE;
		task.destroy_arg[1] = data_ptr->need_free;
		LCUI_AddTask( &task );
	}
	return 0;
}

LCUI_API void WidgetMsg_Proc( LCUI_Widget *widget )
{
	int i,n;
	WidgetMsgData *data_ptr;
	LCUI_Widget *child;
	LCUI_Queue *msg_buff, *child_list;

	if( widget == NULL ) {
		widget = RootWidget_GetSelf();
	}
	msg_buff = Widget_GetMsgBuff( widget );
	child_list = Widget_GetChildList( widget );
	//Queue_Lock( msg_buff );
	n = Queue_GetTotal( msg_buff );
	for(i=0; i<n; ++i) {
		DEBUG_MSG("[%d/%d]get msg\n", i, n);
		data_ptr = (WidgetMsgData*)Queue_Get( msg_buff, i );
		DEBUG_MSG("[%d/%d]dispatch msg\n", i, n);
		if( WidgetMsg_Dispatch( widget, data_ptr ) ) {
			DEBUG_MSG("[%d/%d]delete msg\n", i, n);
			Queue_Delete( msg_buff, i );
			n = Queue_GetTotal( msg_buff );
			--i;
		}
		DEBUG_MSG("[%d/%d]skip msg\n", i, n);
	}
	//Queue_Unlock( msg_buff );
	Queue_Lock( child_list );
	n = Queue_GetTotal( child_list );
	/* 从尾到首,递归处理子部件的更新 */
	while(n--) {
		child = (LCUI_Widget*)Queue_Get( child_list, n );
		if( child ) {
			DEBUG_MSG("proc child msg\n");
			WidgetMsg_Proc( child );
		}
	}
	Queue_Unlock( child_list );
}

LCUI_API int WidgetMsg_Post(	LCUI_Widget *widget,
				uint_t msg_id,
				void *data,
				LCUI_BOOL only_one,
				LCUI_BOOL need_free )
{
	int i, total, n_found;
	WidgetMsgData tmp_msg, *tmp_msg_ptr;
	LCUI_Queue *des_queue;

	if( !widget ) {
		return -1;
	}
	tmp_msg.msg_id = msg_id;
	tmp_msg.need_free = need_free;
	tmp_msg.target = widget;
	if( data ) {
		tmp_msg.valid = TRUE;
	} else {
		tmp_msg.valid = FALSE;
	}
	switch(tmp_msg.msg_id) {
	    case WIDGET_MOVE:
		if(tmp_msg.valid) {
			tmp_msg.data.pos = *((LCUI_Pos*)data);
		}
		break;
	    case WIDGET_RESIZE:
		if(tmp_msg.valid) {
			tmp_msg.data.size = *((LCUI_Size*)data);
		}
		break;
	    case WIDGET_CHGSTATE:
		if(tmp_msg.valid) {
			tmp_msg.data.state = *((int*)data);
		}
		break;
	    case WIDGET_CHGALPHA:
		if(tmp_msg.valid) {
			tmp_msg.data.alpha = *((int*)data);
		}
		break;
	    case WIDGET_UPDATE_SHADOW:
	    case WIDGET_PAINT:
	    case WIDGET_UPDATE:
	    case WIDGET_SORT:
		tmp_msg.valid = FALSE;
		break;
		/* 部件的显示、隐藏和销毁消息，需要发送到父部件 */
	    case WIDGET_SHOW:
	    case WIDGET_HIDE:
	    case WIDGET_DESTROY:
		/* 如果不是根部件 */
		if( widget != RootWidget_GetSelf() ) {
			widget = widget->parent;
		} 
		tmp_msg.valid = FALSE;
		break;
	    default:
		tmp_msg.data.ptr = data;
		break;
	}
	des_queue = Widget_GetMsgBuff( widget );
	Queue_Lock( des_queue );
	total = Queue_GetTotal( des_queue );
	for(n_found=0,i=0; i<total; ++i) {
		tmp_msg_ptr = (WidgetMsgData*)Queue_Get( des_queue, i );
		if( !tmp_msg_ptr ) {
			continue;
		}
		if(tmp_msg_ptr->valid != tmp_msg.valid
		|| tmp_msg_ptr->msg_id != tmp_msg.msg_id
		|| tmp_msg_ptr->target != tmp_msg.target ) {
			continue;
		}
		++n_found;
		/* 如果已存在的数量少于2 */
		if( !only_one && n_found < 2 ) {
			continue;
		}
		/* 否则，需要进行替换 */
		switch(tmp_msg.msg_id) {
		    case WIDGET_MOVE:
			if(tmp_msg.valid) {
				tmp_msg_ptr->data.pos = tmp_msg.data.pos;
				tmp_msg_ptr->valid = TRUE;
			} else {
				tmp_msg_ptr->valid = FALSE;
			}
			break;
		    case WIDGET_RESIZE:
			if(tmp_msg.valid) {
				tmp_msg_ptr->data.size = tmp_msg.data.size;
				tmp_msg_ptr->valid = TRUE;
			} else {
				tmp_msg_ptr->valid = FALSE;
			}
			break;
		    case WIDGET_CHGSTATE:
			if(tmp_msg.valid) {
				tmp_msg_ptr->data.state = tmp_msg.data.state;
			} else {
				tmp_msg_ptr->valid = FALSE;
			}
			break;
		    case WIDGET_CHGALPHA:
			if(tmp_msg.valid) {
				tmp_msg_ptr->data.alpha = tmp_msg.data.alpha;
			} else {
				tmp_msg_ptr->valid = FALSE;
			}
			break;
		    case WIDGET_UPDATE_SHADOW:
		    case WIDGET_PAINT:
		    case WIDGET_HIDE:
		    case WIDGET_UPDATE:
		    case WIDGET_SHOW:
		    case WIDGET_SORT:
			tmp_msg.valid = FALSE;
			break;
		    default:
			if( tmp_msg_ptr->need_free ) {
				free( tmp_msg_ptr->data.ptr );
			}
			tmp_msg_ptr->need_free = tmp_msg.need_free;
			tmp_msg_ptr->data.ptr = tmp_msg.data.ptr;
			break;
		}
		break;
	}
	/* 未找到，则添加新的 */
	if( i>= total ) {
		if( Queue_Add( des_queue, &tmp_msg ) ) {
			Queue_Unlock( des_queue );
			return 0;
		}
		return -2;
	}
	Queue_Unlock( des_queue );
	return 0;
}

LCUI_API int WidgetMsg_Connect(	LCUI_Widget *widget,
				uint_t msg_id,
				WidgetProcFunc func )
{
	int i,n;
	LCUI_Queue *msg_func;
	LCUI_Task *task_ptr, task;

	msg_func = Widget_GetMsgFunc( widget );
	if( msg_func == NULL ) {
		return -1;
	}
	n = Queue_GetTotal( msg_func );
	for(i=0; i<n; ++i) {
		task_ptr = (LCUI_Task*)Queue_Get( msg_func, i );
		if( task_ptr == NULL ) {
			continue;
		}
		if( task_ptr->id != msg_id ) {
			continue;
		}
		task_ptr->func = (CallBackFunc)func;
		return 0;
	}
	task.id = msg_id;
	task.func = (CallBackFunc)func;
	if( Queue_Add( msg_func, &task ) ) {
		return 0;
	}
	return -2;
}
