#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/gui/widget/textedit.h>

static void OnBtnClick(ui_widget_t* self, ui_event_t* e, void *arg)
{
	wchar_t str[256];
	ui_widget_t* edit = ui_get_widget("edit");
	ui_widget_t* txt = ui_get_widget("text-hello");

	TextEdit_GetTextW(edit, 0, 255, str);
	TextView_SetTextW(txt, str);
}

int main(int argc, char **argv)
{
	ui_widget_t* root, pack, btn;

	LCUI_Init();
	root = ui_root();
	pack = LCUIBuilder_LoadFile("helloworld.xml");
	if (!pack) {
		return -1;
	}
	ui_widget_append(root, pack);
	ui_widget_unwrap(pack);
	btn = ui_get_widget("btn");
	ui_widget_on(btn, "click", OnBtnClick, NULL, NULL);
	return LCUI_Main();
}
