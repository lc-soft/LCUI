// 此程序用于测试部件的样式自定义功能
// 运行程序前，请将 test 目录下的style.css文件复制到程序所在目录
// 0.15.0版本的LCUI存在问题，此程序无法正常实现效果，计划以后再对此功能进行完善

#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_GRAPH_H
#include LC_WINDOW_H
#include LC_LABEL_H

int main( int argc, char **argv )
{
	LCUI_Widget *widget;

	LCUI_Init(800,600,0);
	/* 从文件中载入样式数据 */
	WidgetStyle_LoadFromFile( "style.css" );
	widget = Widget_New(NULL);
	Widget_SetStyleName( widget, "my-widget" );
	Widget_Draw( widget );
	Widget_Show( widget );
	return LCUI_Main();
}
