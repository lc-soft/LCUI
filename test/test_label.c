// 测试label部件的示例程序

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_WINDOW_H 
#include LC_LABEL_H 
#include LC_GRAPH_H
#include LC_RES_H
#include <time.h>

static void destroy( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

/* 动态改变label部件的文本内容 */
static void change_label_text(void *arg)
{
	time_t rawtime;
	struct tm * timeinfo;
	char day[][20] = {"星期天","星期一","星期二","星期三","星期四","星期五","星期六"};
	char text[512];
	LCUI_Widget *widget;
	
	widget = (LCUI_Widget *)arg; /* 转换类型 */
	while(1) {
		time ( &rawtime );
		timeinfo = localtime ( &rawtime ); /* 获取系统当前时间 */
		sprintf( text, "当前时间: %4d年%02d月%02d日 %s %02d:%02d:%02d",
			timeinfo->tm_year+1900, timeinfo->tm_mon+1, 
			timeinfo->tm_mday, day[(int) (timeinfo->tm_wday)], 
			timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec );
		/* 更改文本内容 */
		Label_Text( widget, text );
		LCUI_Sleep(1);/* 暂停1秒 */
	}
	LCUIThread_Exit(NULL);
}

int main(void)
{
	LCUI_Thread t;
	LCUI_Widget *window, *label, *time_text;
	
	LCUI_Init();
	window = Widget_New("window");
	label = Widget_New("label");
	time_text = Widget_New("label");
	
	/* 设定窗口标题的文本 */
	Window_SetTitleText(window, "测试label部件的文本显示"); 
	/* 改变窗口的尺寸 */
	Widget_Resize(window, Size(320, 240));
	/* 将窗口客户区作为lael部件的容器添加进去 */
	Window_ClientArea_Add(window, label);
	Window_ClientArea_Add(window, time_text);
	/* 左上角显示，向右偏移4，向下偏移4 */
	Widget_SetAlign(label, ALIGN_TOP_LEFT, Pos(4, 4));
	/* 右下角显示，向左偏移10，向上偏移5 */
	Widget_SetAlign(time_text, ALIGN_BOTTOM_RIGHT, Pos(-10, -5));
	Label_Text(label, 
		"<color=50,200,50>/* 经典的C语言代码 */</color>\n\n"
		"<color=0,155,100>#include</color> <color=255,135,0><stdio.h></color>\n"
		"<color=50,50,200>int</color> main(<color=50,50,200>int</color> argc, <color=50,50,200>char</color> *argv[])\n"
		"{\n"
		"  printf(<color=100,100,100><size=20px>\"hello world!\\n\"</size></color>);\n"
		"  <color=0,0,200>return</color> 0;\n"
		"}\n"
	);
	/* 创建线程，此函数和pthread_create函数用法一样 */
	LCUIThread_Create(&t, change_label_text, (void*)time_text);
	Widget_Event_Connect( Window_GetCloseButton(window), EVENT_CLICKED, destroy );
	Widget_Show(label);
	Widget_Show(time_text);
	Widget_Show(window);
	return LCUI_Main(); 
}

