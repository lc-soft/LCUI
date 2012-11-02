// 测试LCUI的定时器

#include <LCUI_Build.h>
#include LC_LCUI_H 
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_LABEL_H
#include <unistd.h>

static int status = 0;
static LCUI_Widget *label;

void display_text()
{
	if( status == 0 ) {
		Show_Widget( label );
		status = 1;
	} else {
		Hide_Widget( label );
		status = 0;
	}
}

int main(int argc, char*argv[]) 
{
        LCUI_Widget *window;
        LCUI_TextStyle style;
        
        LCUI_Init(argc, argv);
        TextStyle_Init( &style );
        
        window  = Create_Widget("window");
        label   = Create_Widget("label");
        
        Set_Window_Title_Text(window, "测试定时器");
        window->resize(window, Size(320, 240));
        Window_Client_Area_Add(window, label);
        label->set_align(label, ALIGN_MIDDLE_CENTER, Pos(0,0));
        Set_Label_Text(label, "利用定时器实现的文本闪烁");
        TextStyle_FontSize( &style, 20 );
        Set_Label_TextStyle( label, style );
        window->show( window ); 
        /* 设置定时器，每隔500毫秒调用display_text函数，重复调用 */
        set_timer( 500, display_text, TRUE );
        return LCUI_Main(); /* 进入主循环 */
}
