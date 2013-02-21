#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_RES_H
#include LC_DISPLAY_H
#include LC_WIDGET_H
#include LC_LABEL_H
#include LC_BUTTON_H
#include LC_PICBOX_H
#include LC_WINDOW_H

#define TEXT_MB_BTN_OK		"确定"
#define TEXT_MB_BTN_CANCEL	"取消"
#define TEXT_MB_BTN_ABORT	"终止"
#define TEXT_MB_BTN_IGNORE	"忽略"
#define TEXT_MB_BTN_RETRY	"重试"
#define TEXT_MB_BTN_YES		" 是 "
#define TEXT_MB_BTN_NO		" 否 "

#define WIDGET_ID_MB_MSG_AREA	30

#define MB_BTN_SIZE		Size(65, 28)
#define MB_BTN_AREA_BGCOLOR	RGB(215,235,240)
#define MB_BTN_AREA_LINECOLOR	RGB(180,200,240)
#define MB_ICONBOX_SIZE		Size(48,48)
#define WIDGET_ID_MESSAGEBOX	1<<5
#define MSGBOX_NONE_ICON	1<<6
#define MB_BTN_AREA_HEIGHT	40
#define MB_MSG_AREA_HEIGHT	55
#define MB_BTN_AREA_HEIGHT_TEXT	"40px"
#define MB_MSG_AREA_HEIGHT_TEXT	"55px"

typedef struct {
	LCUI_MainLoop *mainloop;
	LCUI_Widget *msgbox;
	MB_BTN clicked_button;
} MB_data;

static LCUI_BOOL msgbox_list_init = FALSE;
static LCUI_Queue msgbox_list;

static LCUI_MainLoop*
msgbox_mainloop_new( LCUI_Widget *msgbox )
{
	MB_data *data;
	
	if( !msgbox_list_init ) {
		Queue_Init( &msgbox_list, sizeof(MB_data), NULL );
		msgbox_list_init = TRUE;
	}
	
	data = malloc( sizeof(MB_data) );
	if( data == NULL ) {
		return NULL;
	}
	
	data->msgbox = msgbox;
	data->mainloop = LCUI_MainLoop_New();
	Queue_Insert_Pointer( &msgbox_list, 0, data );
	return data->mainloop;
}

static MB_data *
msgbox_data_find( LCUI_Widget *msgbox )
{
	int i, n;
	MB_data *data;
	n = Queue_Get_Total( &msgbox_list );
	for(i=0; i<n; ++i) {
		data = Queue_Get( &msgbox_list, i );
		if( !data ) {
			continue;
		}
		if( data->msgbox == msgbox ) {
			return data;
		}
	}
	return NULL;
}

static int
msgbox_data_delete( LCUI_Widget *msgbox )
{
	int i, n;
	MB_data *data;
	n = Queue_Get_Total( &msgbox_list );
	for(i=0; i<n; ++i) {
		data = Queue_Get( &msgbox_list, i );
		if( !data ) {
			continue;
		}
		if( data->msgbox == msgbox ) {
			Queue_Delete( &msgbox_list, i );
			return 0;
		}
	}
	return -1;
}

static void 
msgbox_mainloop_quit( LCUI_Widget *btn, LCUI_WidgetEvent *unused )
{
	LCUI_Widget *msgbox;
	MB_data *data;
	
	msgbox = Widget_GetParent( btn, "window" );
	data = msgbox_data_find( msgbox );
	if( !data ) {
		return;
	}
	data->clicked_button = btn->self_id;
	LCUI_MainLoop_Quit( data->mainloop );
}

/* 响应用于显示文本的label部件的尺寸变动，并自动调整消息框的尺寸 */
static void
auto_resize_msgbox(LCUI_Widget *textlabel, LCUI_WidgetEvent *event )
{
	int area_height;
	LCUI_Widget *msgbox, *msg_area;
	LCUI_Size msgbox_size;
	
	msgbox = Widget_GetParent( textlabel, "window" );
	if( !msgbox ) {
		return;
	}
	if( (msgbox->self_id & WIDGET_ID_MESSAGEBOX) != WIDGET_ID_MESSAGEBOX ) {
		return;
	}
	/* 以下只是估算消息框的尺寸，不要纠结 为何要+=14、为何要+=30 等之类的问题 */
	if( (msgbox->self_id & MSGBOX_NONE_ICON) == MSGBOX_NONE_ICON ) {
		msgbox_size.h = area_height = MB_MSG_AREA_HEIGHT;
		msgbox_size.w = 0;
	} else {
		msgbox_size = MB_ICONBOX_SIZE;
	}
	msgbox_size.w += 14;
	msgbox_size.w += event->resize.new_size.w;
	area_height = msgbox_size.h;
	if( area_height < event->resize.new_size.h ) {
		area_height = event->resize.new_size.h;
	}
	if( area_height < MB_MSG_AREA_HEIGHT ) {
		area_height = MB_MSG_AREA_HEIGHT;
	}
	
	DEBUG_MSG("area_height: %d\n", area_height);
	/* 加上按钮区域的固定高度 */
	msgbox_size.h = area_height + MB_BTN_AREA_HEIGHT;
	/* 加上窗口边框尺寸 */
	msgbox_size.w += 14;
	msgbox_size.h += 30;

	DEBUG_MSG("widget: %p, old size: (%d,%d), new size: (%d,%d)\n",
		textlabel, 
		event->resize.old_size.w, event->resize.old_size.h, 
		event->resize.new_size.w, event->resize.new_size.h);
	DEBUG_MSG("msgbox size: (%d,%d)\n", msgbox_size.w, msgbox_size.h);
	
	/* 获取消息框内的消息区域的指针 */
	msg_area = Widget_GetChildByID(	Window_GetClientArea(msgbox), 
					WIDGET_ID_MB_MSG_AREA );
	if( !msg_area ) {
		return;
	}
	Widget_SetHeight( msg_area, area_height );
	Widget_Resize( msgbox, msgbox_size );
}

static void
msgbox_add_BTN_OK( LCUI_Widget *btn_area )
{
	LCUI_Widget *btn;
	btn = Widget_New("button");
	btn->self_id = MB_BTN_IS_OK;
	Widget_Resize( btn, MB_BTN_SIZE );
	Widget_SetAutoSize( btn, FALSE, 0 );
	Set_Button_Text( btn, TEXT_MB_BTN_OK );
	Widget_Container_Add( btn_area, btn );
	Widget_SetAlign( btn, ALIGN_MIDDLE_CENTER, Pos(0,0) );
	Widget_Event_Connect( btn, EVENT_CLICKED, msgbox_mainloop_quit );
	Widget_Show( btn );
}

static void
msgbox_add_BTN_OKCANCEL( LCUI_Widget *btn_area )
{
	LCUI_Size btn_size;
	LCUI_Widget *btn[2];
	btn[0] = Widget_New("button");
	btn[1] = Widget_New("button");
	btn[0]->self_id = MB_BTN_IS_OK;
	btn[1]->self_id = MB_BTN_IS_CANCEL;
	Widget_Resize( btn[0], MB_BTN_SIZE );
	Widget_Resize( btn[1], MB_BTN_SIZE );
	Widget_SetAutoSize( btn[0], FALSE, 0 );
	Widget_SetAutoSize( btn[1], FALSE, 0 );
	Set_Button_Text( btn[0], TEXT_MB_BTN_OK );
	Set_Button_Text( btn[1], TEXT_MB_BTN_CANCEL );
	Widget_Container_Add( btn_area, btn[0] );
	Widget_Container_Add( btn_area, btn[1] );
	btn_size = MB_BTN_SIZE;
	Widget_SetAlign( btn[0], ALIGN_MIDDLE_CENTER, Pos(0-(btn_size.w/2.0+3),0) );
	Widget_SetAlign( btn[1], ALIGN_MIDDLE_CENTER, Pos((btn_size.w/2.0+3),0) );
	Widget_Event_Connect( btn[0], EVENT_CLICKED, msgbox_mainloop_quit );
	Widget_Event_Connect( btn[1], EVENT_CLICKED, msgbox_mainloop_quit );
	Widget_Show( btn[0] );
	Widget_Show( btn[1] );
}

static void
msgbox_add_BTN_ABORTRETRYIGNORE( LCUI_Widget *btn_area )
{
	LCUI_Size btn_size;
	LCUI_Widget *btn[3];
	btn[0] = Widget_New("button");
	btn[1] = Widget_New("button");
	btn[2] = Widget_New("button");
	btn[0]->self_id = MB_BTN_IS_ABORT;
	btn[1]->self_id = MB_BTN_IS_RETRY;
	btn[2]->self_id = MB_BTN_IS_IGNORE;
	Widget_Resize( btn[0], MB_BTN_SIZE );
	Widget_Resize( btn[1], MB_BTN_SIZE );
	Widget_Resize( btn[2], MB_BTN_SIZE );
	Widget_SetAutoSize( btn[0], FALSE, 0 );
	Widget_SetAutoSize( btn[1], FALSE, 0 );
	Widget_SetAutoSize( btn[2], FALSE, 0 );
	Set_Button_Text( btn[0], TEXT_MB_BTN_ABORT );
	Set_Button_Text( btn[1], TEXT_MB_BTN_RETRY );
	Set_Button_Text( btn[2], TEXT_MB_BTN_IGNORE );
	Widget_Container_Add( btn_area, btn[0] );
	Widget_Container_Add( btn_area, btn[1] );
	Widget_Container_Add( btn_area, btn[2] );
	btn_size = MB_BTN_SIZE;
	Widget_SetAlign( btn[0], ALIGN_MIDDLE_CENTER, Pos(0-(btn_size.w+3),0) );
	Widget_SetAlign( btn[1], ALIGN_MIDDLE_CENTER, Pos(0,0) );
	Widget_SetAlign( btn[2], ALIGN_MIDDLE_CENTER, Pos((btn_size.w+3),0) );
	Widget_Event_Connect( btn[0], EVENT_CLICKED, msgbox_mainloop_quit );
	Widget_Event_Connect( btn[1], EVENT_CLICKED, msgbox_mainloop_quit );
	Widget_Event_Connect( btn[2], EVENT_CLICKED, msgbox_mainloop_quit );
	Widget_Show( btn[0] );
	Widget_Show( btn[1] );
	Widget_Show( btn[2] );
}

static void
msgbox_add_BTN_RETRYCANCEL( LCUI_Widget *btn_area )
{
	LCUI_Size btn_size;
	LCUI_Widget *btn[3];
	btn[0] = Widget_New("button");
	btn[1] = Widget_New("button");
	btn[0]->self_id = MB_BTN_IS_RETRY;
	btn[1]->self_id = MB_BTN_IS_CANCEL;
	Widget_Resize( btn[0], MB_BTN_SIZE );
	Widget_Resize( btn[1], MB_BTN_SIZE );
	Widget_SetAutoSize( btn[0], FALSE, 0 );
	Widget_SetAutoSize( btn[1], FALSE, 0 );
	Set_Button_Text( btn[0], TEXT_MB_BTN_RETRY );
	Set_Button_Text( btn[1], TEXT_MB_BTN_CANCEL );
	Widget_Container_Add( btn_area, btn[0] );
	Widget_Container_Add( btn_area, btn[1] );
	btn_size = MB_BTN_SIZE;
	Widget_SetAlign( btn[0], ALIGN_MIDDLE_CENTER, Pos(0-(btn_size.w/2.0+3),0) );
	Widget_SetAlign( btn[1], ALIGN_MIDDLE_CENTER, Pos((btn_size.w/2.0+3),0) );
	Widget_Event_Connect( btn[0], EVENT_CLICKED, msgbox_mainloop_quit );
	Widget_Event_Connect( btn[1], EVENT_CLICKED, msgbox_mainloop_quit );
	Widget_Show( btn[0] );
	Widget_Show( btn[1] );
}

static void
msgbox_add_BTN_YESNO( LCUI_Widget *btn_area )
{
	LCUI_Size btn_size;
	LCUI_Widget *btn[2];
	btn[0] = Widget_New("button");
	btn[1] = Widget_New("button");
	btn[0]->self_id = MB_BTN_IS_YES;
	btn[1]->self_id = MB_BTN_IS_NO;
	Widget_Resize( btn[0], MB_BTN_SIZE );
	Widget_Resize( btn[1], MB_BTN_SIZE );
	Widget_SetAutoSize( btn[0], FALSE, 0 );
	Widget_SetAutoSize( btn[1], FALSE, 0 );
	Set_Button_Text( btn[0], TEXT_MB_BTN_YES );
	Set_Button_Text( btn[1], TEXT_MB_BTN_NO );
	Widget_Container_Add( btn_area, btn[0] );
	Widget_Container_Add( btn_area, btn[1] );
	btn_size = MB_BTN_SIZE;
	Widget_SetAlign( btn[0], ALIGN_MIDDLE_CENTER, Pos(0-(btn_size.w/2.0+3),0) );
	Widget_SetAlign( btn[1], ALIGN_MIDDLE_CENTER, Pos((btn_size.w/2.0+3),0) );
	Widget_Event_Connect( btn[0], EVENT_CLICKED, msgbox_mainloop_quit );
	Widget_Event_Connect( btn[1], EVENT_CLICKED, msgbox_mainloop_quit );
	Widget_Show( btn[0] );
	Widget_Show( btn[1] );
}

static void
msgbox_add_BTN_YESNOCANCEL( LCUI_Widget *btn_area )
{
	LCUI_Size btn_size;
	LCUI_Widget *btn[3];
	btn[0] = Widget_New("button");
	btn[1] = Widget_New("button");
	btn[2] = Widget_New("button");
	btn[0]->self_id = MB_BTN_IS_YES;
	btn[1]->self_id = MB_BTN_IS_NO;
	btn[2]->self_id = MB_BTN_IS_CANCEL;
	Widget_Resize( btn[0], MB_BTN_SIZE );
	Widget_Resize( btn[1], MB_BTN_SIZE );
	Widget_Resize( btn[2], MB_BTN_SIZE );
	Widget_SetAutoSize( btn[0], FALSE, 0 );
	Widget_SetAutoSize( btn[1], FALSE, 0 );
	Widget_SetAutoSize( btn[2], FALSE, 0 );
	Set_Button_Text( btn[0], TEXT_MB_BTN_YES );
	Set_Button_Text( btn[1], TEXT_MB_BTN_NO );
	Set_Button_Text( btn[2], TEXT_MB_BTN_CANCEL );
	Widget_Container_Add( btn_area, btn[0] );
	Widget_Container_Add( btn_area, btn[1] );
	Widget_Container_Add( btn_area, btn[2] );
	btn_size = MB_BTN_SIZE;
	Widget_SetAlign( btn[0], ALIGN_MIDDLE_CENTER, Pos(0-(btn_size.w+3),0) );
	Widget_SetAlign( btn[1], ALIGN_MIDDLE_CENTER, Pos(0,0) );
	Widget_SetAlign( btn[2], ALIGN_MIDDLE_CENTER, Pos((btn_size.w+3),0) );
	Widget_Event_Connect( btn[0], EVENT_CLICKED, msgbox_mainloop_quit );
	Widget_Event_Connect( btn[1], EVENT_CLICKED, msgbox_mainloop_quit );
	Widget_Event_Connect( btn[2], EVENT_CLICKED, msgbox_mainloop_quit );
	Widget_Show( btn[0] );
	Widget_Show( btn[1] );
	Widget_Show( btn[2] );
}

/* LCUI的消息框
 * 参数：
 * icon_type	: 指定消息框中显示的图标，可取以下值：
 * 	MB_ICON_NONE	无图标
 * 	MB_ICON_ERROR	“错误”图标
 * 	MB_ICON_HELP	”帮助“图标
 * 	MB_ICON_INFO	“信息”图标
 * 	MB_ICON_WARNING	“警告”图标
 * text		: 消息框内显示的消息内容
 * title	: 消息框的标题文本
 * button	: 指定消息框内显示的按钮，可取以下值：
 * 	MB_BTN_OK		: 一个“确定”按钮
 * 	MB_BTN_OKCANCEL		: “确定”和“取消”按钮
 * 	MB_BTN_ABORTRETRYIGNORE	: “终止”、"重试"和“忽略”按钮
 * 	MB_BTN_RETRYCANCEL	: "重试"和“取消”按钮
 * 	MB_BTN_YESNO		: "是"和“否”按钮
 * 	MB_BTN_YESNOCANCEL	: “是”、"否"和“取消”按钮
 * 返回值：
 * -1	: button参数的值异常
 * -2	: 无法获取消息框关闭时所点击的按钮ID
 * 正常则会返回按钮的ID，有以下值：
 * 	MB_BTN_IS_OK	: "确定"按钮	
 * 	MB_BTN_IS_CANCEL: "取消"按钮
 * 	MB_BTN_IS_ABORT	: "终止"按钮
 * 	MB_BTN_IS_RETRY	: "重试"按钮
 * 	MB_BTN_IS_IGNORE: "忽略"按钮
 * 	MB_BTN_IS_YES	: "是"按钮
 * 	MB_BTN_IS_NO	: "否"按钮
 * 	MB_BTN_IS_QUIT	: 窗口右上角的关闭按钮
 *  */
int LCUI_MessageBox(	MB_ICON_TYPE icon_type, const char *text, 
			const char *title, MB_BTN_TYPE button )
{
	int ret, icon_flag;
	LCUI_Graph icon;
	LCUI_MainLoop *loop;
	MB_data *msgbox_data;
	LCUI_Border btn_area_border;
	LCUI_Widget *iconbox, *textbox, *msgbox;
	LCUI_Widget *btn_close;
	LCUI_Widget *msg_area, *btn_area;
	LCUI_Size min_size, max_size;
	
	iconbox = Widget_New("picture_box");
	textbox = Widget_New("label");
	msg_area = Widget_New(NULL);
	btn_area = Widget_New(NULL);
	msg_area->self_id = WIDGET_ID_MB_MSG_AREA;
	
	Graph_Init( &icon );
	/* 根据参数的值，载入相应的图标 */
	switch(icon_type) {
	case MB_ICON_NONE:break;
	case MB_ICON_ERROR:
		Load_Graph_Icon_Error( &icon );
		break;
	case MB_ICON_HELP:
		Load_Graph_Icon_Help( &icon );
		break;
	case MB_ICON_INFO:
		Load_Graph_Icon_Info( &icon );
		break;
	case MB_ICON_WARNING:
		Load_Graph_Icon_Warning( &icon );
		break;
	}
	/* 添加图标后文本进消息区域 */
	Widget_Container_Add( msg_area, iconbox );
	Widget_Container_Add( msg_area, textbox );
	/* 设定图标框的默认尺寸 */
	Widget_Resize( iconbox, MB_ICONBOX_SIZE );
	/* 为图标框设置需显示的图标 */
	PictureBox_SetImage( iconbox, &icon );
	PictureBox_SetSizeMode( iconbox, SIZE_MODE_CENTER );
	/* 判断是否有图标，并调整文本位置 */
	if( icon_type == MB_ICON_NONE ) {
		icon_flag = MSGBOX_NONE_ICON;
		Widget_SetAlign( textbox, ALIGN_MIDDLE_LEFT, Pos(5,0) );
	} else {
		icon_flag = 0;
		Widget_SetAlign( iconbox, ALIGN_MIDDLE_LEFT, Pos(5,0) );
		Widget_SetAlign( textbox, ALIGN_MIDDLE_LEFT, 
				Pos(icon.width + 10,0) );
	}
	/* 根据按钮数量，得出相应的最小尺寸 */
	switch( button ) {
	case MB_BTN_OK:
		msgbox_add_BTN_OK( btn_area );
		min_size = Size(150,125);
		break;
	case MB_BTN_OKCANCEL:
		msgbox_add_BTN_OKCANCEL( btn_area );
		min_size = Size(180,125);
		break;
	case MB_BTN_ABORTRETRYIGNORE:
		msgbox_add_BTN_ABORTRETRYIGNORE( btn_area );
		min_size = Size(215,125);
		break;
	case MB_BTN_RETRYCANCEL:
		msgbox_add_BTN_RETRYCANCEL( btn_area );
		min_size = Size(180,125);
		break;
	case MB_BTN_YESNO:
		msgbox_add_BTN_YESNO( btn_area );
		min_size = Size(180,125);
		break;
	case MB_BTN_YESNOCANCEL:
		msgbox_add_BTN_YESNOCANCEL( btn_area );
		min_size = Size(215,125);
		break;
	default: return -1;
	}
	/* 创建消息框，并设置ID */
	msgbox = Window_New( title, NULL, min_size );
	msgbox->self_id = WIDGET_ID_MESSAGEBOX | button | icon_flag;
	/* 获取窗口右上角的按钮指针，并设定按钮的ID */
	btn_close = Window_GetCloseButton( msgbox );
	btn_close->self_id = MB_BTN_IS_QUIT;
	/* 限制消息框的尺寸 */
	max_size = LCUIScreen_GetSize();
	Widget_LimitSize( msgbox, min_size, max_size );
	/* 设置为模态部件 */
	Widget_SetModal( msgbox, TRUE );
	/* 添加这两个区域至消息框内 */
	Window_ClientArea_Add( msgbox, msg_area );
	Window_ClientArea_Add( msgbox, btn_area );
	/* 设置区域的停靠方式及初始尺寸 */
	Widget_SetDock( msg_area, DOCK_TYPE_TOP );
	Widget_SetDock( btn_area, DOCK_TYPE_BOTTOM );
	Widget_SetSize( msg_area, NULL, MB_MSG_AREA_HEIGHT_TEXT );
	Widget_SetSize( btn_area, NULL, MB_BTN_AREA_HEIGHT_TEXT );
	
	/* 设定按钮区域的样式 */
	Widget_SetBackgroundTransparent( btn_area, FALSE );
	Widget_SetBackgroundColor( btn_area, MB_BTN_AREA_BGCOLOR );
	Border_Init( &btn_area_border );
	btn_area_border.top_width = 1;
	btn_area_border.top_style = BORDER_STYLE_SOLID;
	btn_area_border.top_color = MB_BTN_AREA_LINECOLOR;
	Widget_SetBorder( btn_area, btn_area_border );
	
	Label_Text( textbox, text );
	Widget_Show( textbox );
	if( icon_type != MB_ICON_NONE ) {
		Widget_Show( iconbox );
	}
	Widget_Show( msgbox );
	Widget_Show( btn_area );
	Widget_Show( msg_area );
	
	loop = msgbox_mainloop_new( msgbox );
	/* 关联消息框右上角关闭按钮的CLICKED事件 */
	Widget_Event_Connect( btn_close, EVENT_CLICKED, msgbox_mainloop_quit );
	/* 关联用于显示文本的label部件的RESIZE事件 */
	Widget_Event_Connect( textbox, EVENT_RESIZE, auto_resize_msgbox );
	/* 进入主循环 */
	LCUI_MainLoop_Run( loop );
	msgbox_data = msgbox_data_find( msgbox );
	if( msgbox_data ) {
		Widget_Destroy( msgbox );
		ret = msgbox_data->clicked_button;
		msgbox_data_delete( msgbox );
		return ret;
	}
	return -2;
}
