// C++版的hello,world程序
#include <LCUI_Build.h> 
#include LC_LCUI_H
#include LC_WIDGET_H
#include <unistd.h>
#include <iostream>
using namespace std;

void *test_thread(void *object)
{
	LCUIThread *t = (LCUIThread *)object;
	cout<<"thread id: %lu"<<t->getid()<<endl;
	t->exit( NULL );
}

int main(int argc, char*argv[]) 
{
	LCUI app(argc, argv); 
	LCUIWidget window( "window" ), label( "label" );
	LCUIGraph icon;
	LCUIThread t;
	
	t.create( NULL, test_thread, &t );
	t.join( NULL );
	
	icon.load.icon.LCUI_18x18();
	window.Window.setIcon( icon );
	window.resize( Size(320, 240) ); 
	window.Window.setTitle( "测试窗口" ); 
	window.Window.addToClientArea( label ); 
	 
	label.setAlign( ALIGN_MIDDLE_CENTER, Pos(0,0) ); 
	label.Label.setText(
		"Hello,World!\n"
		"<color=255,0,0>世</color><color=100,155,0>界</color>"
		"<color=0,0,0>,</color><color=0,255,0>你</color>"
		"<color=0,100,155>好</color><color=0,0,255>！</color>\n" 
	); 
	label.Label.setFont( 24, NULL );
	
	label.show();
	window.show();
	return app.main(); 
}

