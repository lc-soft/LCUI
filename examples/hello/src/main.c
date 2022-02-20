#include <LCUI.h>
#include <LCUI/main.h>
#include <LCUI/ui/builder.h>
#include <LCUI/ui/widgets/textview.h>
#include <LCUI/ui/widgets/textedit.h>

static void on_btn_click(ui_widget_t* self, ui_event_t* e, void *arg)
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
	pack = ui_load_xml_file("main.xml");
	if (!pack) {
		return -1;
	}
	ui_root_append(pack);
	ui_widget_unwrap(pack);
	btn = ui_get_widget("btn");
	ui_widget_on(btn, "click", on_btn_click, NULL, NULL);
	return lcui_main();
}
