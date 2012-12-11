#ifdef __cplusplus
// C++版的hello,world程序
#include <LCUI_Build.h> 
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_GRAPH_H
#include LC_WINDOW_H
#include LC_LABEL_H

int main(int argc, char*argv[]) 
{
	LCUIApp app( argc, argv );
	LCUIWindow window;
	LCUILabel text;
	LCUIGraph icon;
	
	Load_Graph_Icon_LCUI_18x18( icon.getGraph() );
	window.setTitleText( "测试窗口" );
	window.resize( Size(320, 240) );
	window.setTitleIcon( icon );
	window.addToClientArea( label );
	text.setAlign( ALIGN_MIDDLE_CENTER, Pos(0,0) );
	text.setText(
		"<size=24px>Hello,World!\n"
		"<color=255,0,0>世</color>"
		"<color=100,155,0>界</color>"
		"<color=0,0,0>,</color>"
		"<color=0,255,0>你</color>"
		"<color=0,100,155>好</color>"
		"<color=0,0,255>！</color>\n</size>");
	/* 显示部件 */
	text.show();
	window.show();
	return app.main();
}

#else 

// 用LCUI实现hello,world程序

#include <LCUI_Build.h>
#include LC_LCUI_H 
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_LABEL_H 
#include LC_GRAPH_H
#include LC_RES_H
#include <unistd.h>

int main(int argc, char*argv[]) 
{
	LCUI_Widget *window, *label;
	LCUI_TextStyle style;
	LCUI_Graph pic;
	
	LCUI_Init(argc, argv);
	
	/* 初始化结构体 */
	Graph_Init( &pic );
	TextStyle_Init( &style );
	/* 载入库中自带的图形数据，这个图形是18x18尺寸的LCUI的图标 */
	Load_Graph_Icon_LCUI_18x18(&pic);
	/* 创建部件 */
	window  = Create_Widget("window");
	label   = Create_Widget("label");
	
	/* 设定窗口标题的文本 */
	Set_Window_Title_Text(window, "测试窗口");
	/* 设定窗口标题的图标 */
	Set_Window_Title_Icon(window, &pic);
	/* 改变窗口的尺寸 */
	window->resize(window, Size(320, 240));
	/* 将窗口客户区作为label部件的容器添加进去 */
	Window_Client_Area_Add(window, label);
	/* 居中显示 */
	label->set_align(label, ALIGN_MIDDLE_CENTER, Pos(0,0));
	/* 
	 * 设定label部件显示的文本
	 * <color=R,G,B>文字</color> 表示的是：“文字”将使用自定义颜色
	 * <size=24px>文字</size> 表示的是：显示的“文字”，大小为24像素
	 **/
	Label_Text(label, 
		"<size=24px>Hello,World!\n"
		"<color=255,0,0>世</color>"
		"<color=100,155,0>界</color>"
		"<color=0,0,0>,</color>"
		"<color=0,255,0>你</color>"
		"<color=0,100,155>好</color>"
		"<color=0,0,255>！</color>\n</size>");
	/* 显示部件 */
	label->show(label); 
	window->show(window); 
	return LCUI_Main(); /* 进入主循环 */
}
#endif
