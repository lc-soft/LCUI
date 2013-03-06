// 用LCUI实现hello,world程序

#include <LCUI_Build.h>
#include LC_LCUI_H 
#include LC_GRAPH_H
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_LABEL_H 
#include LC_RES_H

#ifdef __cplusplus
#ifdef LCUI_BUILD_IN_WIN32
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	Win32_LCUI_Init( hInstance );
#else
int main(int argc, char*argv[]) 
{
#endif
	LCUIApp app;
	LCUIWindow wind;
	LCUILabel text;
	LCUIGraph icon;

	Load_Graph_Icon_LCUI_18x18( icon.getGraph() );
	wind.setTitleText( "测试窗口" );
	wind.setTitleIcon( icon );
	wind.resize( 320,240 );
	text.setText( "Hello, World!\n你好，世界！" );
	text.setTextStyle( 24 );
	text.setAlign( ALIGN_MIDDLE_CENTER );
	wind.addToClientArea( text );
	text.show();
	wind.show();
	app.main();
}

#else
#ifdef LCUI_BUILD_IN_WIN32
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
#else
int main(int argc, char*argv[]) 
#endif
{
	LCUI_Widget *window, *label;
	LCUI_TextStyle style;
	LCUI_Graph pic;
	
#ifdef LCUI_BUILD_IN_WIN32
	Win32_LCUI_Init( hInstance );
#endif
	LCUI_Init();
	
	/* 初始化结构体 */
	Graph_Init( &pic );
	TextStyle_Init( &style );
	/* 载入库中自带的图形数据，这个图形是18x18尺寸的LCUI的图标 */
	Load_Graph_Icon_LCUI_18x18(&pic);
	/* 创建部件 */
	window  = Window_New("测试窗口", &pic, Size(320, 240));
	label   = Widget_New("label");
	
	/* 将窗口客户区作为label部件的容器添加进去 */
	Window_ClientArea_Add(window, label);
	/* 居中显示 */
	label->set_align(label, ALIGN_MIDDLE_CENTER, Pos(0,0));
	/* 
	 * 设定label部件显示的文本
	 * <color=R,G,B>文字</color> 表示的是：“文字”将使用自定义颜色
	 * <size=24px>文字</size> 表示的是：显示的“文字”，大小为24像素
	 **/
#ifdef test_this_code
	Label_Text(label, 
		"<size=24px>Hello,World!\n"
		"<color=255,0,0>世</color>"
		"<color=100,155,0>界</color>"
		"<color=0,0,0>,</color>"
		"<color=0,255,0>你</color>"
		"<color=0,100,155>好</color>"
		"<color=0,0,255>！</color>\n</size>");
#else
	Label_Text(label, 
		"<size=24px>Hello,World!\n"
		"世界,你好!\n</size>");
#endif
	/* 显示部件 */
	label->show(label); 
	window->show(window); 
	return LCUI_Main(); /* 进入主循环 */
}

#endif