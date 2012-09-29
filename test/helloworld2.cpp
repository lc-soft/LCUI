// C++版的hello,world程序
#include <LCUI_Build.h> 
#include LC_LCUI_H	/* 包含LCUI的相关头文件 */
#include LC_WIDGET_H 

int main(int argc, char*argv[]) 
{
	LCUI		app(argc, argv); 
	LCUIWidget	window( "window" ), label( "label" );
	LCUIGraph	icon;
	
	/* 载入LCUI的18x18尺寸的图标 */
	icon.load.icon.LCUI_18x18();
	/* 为窗口设定图标 */
	window.Window.setIcon( icon );
	/* 改变窗口的尺寸 */
	window.resize( Size(320, 240) ); 
	/* 设定窗口的标题文本 */
	window.Window.setTitle( "测试窗口" ); 
	/* 将label部件添加至窗口客户区 */
	window.Window.addToClientArea( label ); 
	/* 设定label部件的对齐方式 */
	label.setAlign( ALIGN_MIDDLE_CENTER, Pos(0,0) ); 
	/* 设定label部件显示的文本 */
	label.Label.setText(
		"Hello,World!\n"
		"<color=255,0,0>世</color><color=100,155,0>界</color>"
		"<color=0,0,0>,</color><color=0,255,0>你</color>"
		"<color=0,100,155>好</color><color=0,0,255>！</color>\n" 
	); 
	/* 设定字体大小 */
	label.Label.setFont( 24, NULL );
	/* 显示这两个部件 */
	label.show();
	window.show();
	/* 进入主循环 */
	return app.main();
}
