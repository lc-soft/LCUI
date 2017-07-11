#include "pch.h"
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/platform.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/gui/widget/textedit.h>
#include <LCUI/gui/builder.h>
#include LCUI_APP_H

class App : public LCUI::Application {
	void Load( Platform::String^ entryPoint );
};

static void OnBtnClick( LCUI_Widget self, LCUI_WidgetEvent e, void *arg )
{
	wchar_t str[256];
	LCUI_Widget edit = LCUIWidget_GetById( "edit" );
	LCUI_Widget txt = LCUIWidget_GetById( "text-hello" );
	TextEdit_GetTextW( edit, 0, 255, str );
	TextView_SetTextW( txt, str );
}

void App::Load( Platform::String^ entryPoint )
{
	LCUI_Widget btn, root, pack;
	root = LCUIWidget_GetRoot();
	pack = LCUIBuilder_LoadFile( "helloworld.xml" );
	if( !pack ) {
		return;
	}
	Widget_Append( root, pack ); 
	Widget_Unwrap( pack );
	btn = LCUIWidget_GetById( "btn-ok" );
	Widget_BindEvent( btn, "click", OnBtnClick, NULL, NULL );
}

[Platform::MTAThread]
int main( Platform::Array<Platform::String^>^ )
{
	App app;
	LCUI::Initialize();
	LCUI::Run( app );
	return 0;
}
