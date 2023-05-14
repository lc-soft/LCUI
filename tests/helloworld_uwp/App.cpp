#include "pch.h"
#include <LCUI.h>
#include <platform.h>
#include <ui.h>
#include <ui_widgets/text.h>
#include <ui_widgets/textedit.h>
#include <ui_xml.h>
#include LCUI_APP_H

class App : public LCUI::Application
{
	void Load(Platform::String ^ entryPoint);
};

static void OnBtnClick(ui_widget_t* self, ui_event_t* e, void *arg)
{
	wchar_t str[256];
	ui_widget_t* edit = ui_get_widget("edit");
	ui_widget_t* txt = ui_get_widget("text-hello");
	ui_textedit_get_text_w(edit, 0, 255, str);
	ui_text_set_content_w(txt, str);
}

void App::Load(Platform::String ^ entryPoint)
{
	ui_widget_t* btn, root, pack;
	root = ui_root();
	pack = ui_load_xml_file("helloworld.xml");
	if (!pack) {
		return;
	}
	ui_widget_append(root, pack);
	ui_widget_unwrap(pack);
	btn = ui_get_widget("btn");
	ui_widget_on(btn, "click", OnBtnClick, NULL, NULL);
}

[Platform::MTAThread] int main(Platform::Array<Platform::String ^> ^) {
	App app;
	LCUI::Initialize();
	LCUI::Run(app);
	return 0;
}
