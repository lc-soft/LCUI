#include <LCUI.h>
#include <LCUI/main.h>
#include <LCUI/ui/builder.h>
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
	ui_widget_t *pack, *btn;

	lcui_init();
	pack = ui_load_xml("helloworld.xml");
	if (!pack) {
		return -1;
	}
	ui_root_append(pack);
	ui_widget_unwrap(pack);
	btn = ui_get_widget("btn");
	ui_widget_on(btn, "click", OnBtnClick, NULL, NULL);
	return lcui_main();
}
