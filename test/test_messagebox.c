// 测试MessageBox

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_BUTTON_H
#include LC_RADIOBTN_H
#include LC_LABEL_H
#include <unistd.h>

static LCUI_Widget 
*window, *label_btn, *label_icon, *rb_btn[6], *rb_icon[5], *btn;

static char rb_btn_text[6][25]={
	"MB_BTN_OK",
	"MB_BTN_OKCANCEL",
	"MB_BTN_ABORTRETRYIGNORE",
	"MB_BTN_RETRYCANCEL",
	"MB_BTN_YESNO",
	"MB_BTN_YESNOCANCEL"
};

static char rb_icon_text[5][25]={
	"MB_ICON_NONE",
	"MB_ICON_ERROR",
	"MB_ICON_HELP",
	"MB_ICON_INFO",
	"MB_ICON_WARNING"
};

static void 
show_msgbox( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	int i, ret;
	MB_BTN_TYPE btn_type;
	MB_ICON_TYPE icon_type;
	/* 根据所选中的单选框，得出参数 */
	for(i=0,btn_type=0; i<6; ++i,++btn_type) {
		if( RadioButton_IsOn(rb_btn[i]) ) {
			break;
		}
	}
	for(i=0,icon_type=0; i<5; ++i,++icon_type) {
		if( RadioButton_IsOn(rb_icon[i]) ) {
			break;
		}
	}
	/* 调用LCUI_MessageBox函数，显示消息框 */
	ret = LCUI_MessageBox( icon_type, "这是一条消息文本。", "消息框", btn_type);
	printf("clicked button: %d\n", ret);
}

static void
program_quit( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit( NULL );
}

int main(void)
{
	int  i, x, y = 3;
	
	LCUI_Init();
	
	window = Window_New( "测试MessageBox", NULL, Size(320,240) );
	label_btn = Widget_New( "label" );
	label_icon = Widget_New( "label" );
	btn = Widget_New( "button" );
	
	Window_ClientArea_Add( window, label_btn );
	Label_Text( label_btn, "请选择按钮类型：" );
	Widget_Move( label_btn, Pos(5,y) );
	Widget_Show( label_btn );
	
	for(i=0; i<6; ++i) {
		rb_btn[i] = RadioButton_New( rb_btn_text[i] );
		if( i>0 ) {
			RadioButton_CreateMutex( rb_btn[0], rb_btn[i] );
		} else {
			RadioButton_SetOn( rb_btn[0] );
		}
		Window_ClientArea_Add( window, rb_btn[i] );
		y += 16;
		Widget_Move( rb_btn[i], Pos(5,y) );
		Widget_Show( rb_btn[i] );
	}
	
	y += 16;
	Window_ClientArea_Add( window, label_icon );
	Label_Text( label_icon, "请选择图标类型：" );
	Widget_Move( label_icon, Pos(5,y) );
	Widget_Show( label_icon );
	
	for(i=0; i<5; ++i) {
		rb_icon[i] = RadioButton_New( rb_icon_text[i] );
		if( i>0 ) {
			RadioButton_CreateMutex( rb_icon[0], rb_icon[i] );
		} else {
			RadioButton_SetOn( rb_icon[0] );
		}
		Window_ClientArea_Add( window, rb_icon[i] );
		/* 计算当前单选框的坐标 */
		if(i%2 == 0) {
			y += 16;
			x = 5;
		} else {
			x = 150;
		}
		Widget_Move( rb_icon[i], Pos(x,y) );
		Widget_Show( rb_icon[i] );
	}
	
	Button_Text( btn, "显示MessageBox" );
	Widget_SetAlign( btn, ALIGN_BOTTOM_CENTER, Pos(0,-3) );
	Widget_Resize( btn, Size(80, 20) );
	Window_ClientArea_Add( window, btn );
	Widget_Show( btn );
	
	Widget_Event_Connect( btn, EVENT_CLICKED, show_msgbox );
	Widget_Event_Connect( Window_GetCloseButton(window), EVENT_CLICKED, program_quit );
	Widget_Show( window );
	return LCUI_Main();
}
