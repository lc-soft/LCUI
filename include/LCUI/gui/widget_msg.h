#ifndef __LCUI_WIDGET_MSG_H__
#define __LCUI_WIDGET_MSG_H__

LCUI_BEGIN_HEADER

typedef enum WidgetMsgID_ {
	WIDGET_UPDATE,		// 更新
	WIDGET_PAINT,		// 重绘
	WIDGET_MOVE,		// 移动
	WIDGET_CHGSTATE,	// 状态改变（Change State）
	WIDGET_CHGALPHA,	// 更新透明度（Change Alpha）
	WIDGET_RESIZE,		// 调整尺寸
	WIDGET_SORT,		// 排序子部件
	WIDGET_SHOW,		// 显示
	WIDGET_HIDE,		// 隐藏
	WIDGET_REFRESH,		// 刷新显示
	WIDGET_DESTROY,		// 销毁
	WIDGET_USER		// 用户自定义
} WidgetMsgID;

typedef union union_widget_data {
	LCUI_Pos pos;
	LCUI_Size size;
	int state;
	uchar_t alpha;
	void *ptr;
} u_wdata;

typedef struct WidgetMsgData_ {
	LCUI_Widget *target;	/* 针对的部件 */
	LCUI_BOOL need_proc;	/* 是否需要处理 */
	uint_t msg_id;		/* 消息的类型 */
	LCUI_BOOL valid;	/* 数据是否有效 */
	LCUI_BOOL need_free;	/* 数据是否需要释放 */
	u_wdata data;		/* 需要更新的数据 */
} WidgetMsgData;

typedef void (*WidgetProcFunc)(LCUI_Widget*,void*);

LCUI_API void WidgetMsgBuff_Init( LCUI_Widget *widget );

LCUI_API void WidgetMsgFunc_Init( LCUI_Widget *widget );

LCUI_API int WidgetMsg_AddToTask( LCUI_Widget *widget, WidgetMsgData *data_ptr );

LCUI_API void WidgetMsg_Proc( LCUI_Widget *widget );

#define LCUIWidget_ProcMessage() WidgetMsg_Proc(NULL)

LCUI_API int WidgetMsg_Post(	LCUI_Widget *widget,
				uint_t msg_id,
				void *data,
				LCUI_BOOL only_one,
				LCUI_BOOL need_free );

LCUI_API int WidgetMsg_Connect(	LCUI_Widget *widget,
				uint_t msg_id,
				WidgetProcFunc func );


LCUI_END_HEADER

#endif