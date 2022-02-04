#include <LCUI.h>
#include <LCUI/main.h>
#include <LCUI/ui/builder.h>
#include <LCUI/ui/widgets/textview.h>
#include <LCUI/ui/widgets/textedit.h>

static void OnBtnClick(ui_widget_t* self, ui_event_t* e, void *arg)
{
	wchar_t str[256];
	ui_widget_t* edit = ui_get_widget("edit");
	ui_widget_t* txt = ui_get_widget("text-hello");

	ui_textedit_get_text_w(edit, 0, 255, str);
	ui_textview_set_text_w(txt, str);
}

int main(int argc, char **argv)
{
	ui_widget_t *pack, *btn;

	lcui_init();
	pack = ui_load_xml_file("helloworld.xml");
	if (!pack) {
		return -1;
	}
	ui_root_append(pack);
	ui_widget_unwrap(pack);
	btn = ui_get_widget("btn");
	ui_widget_on(btn, "click", OnBtnClick, NULL, NULL);
	return lcui_main();
}
