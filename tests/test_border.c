#include <LCUI.h>
#include <LCUI/pandagl.h>
#include <LCUI/image.h>
#include <LCUI/ui/builder.h>
#include <LCUI/main.h>

int main(void)
{
	int ret = 0;
	ui_widget_t *root, *box;

	lcui_init();
	box = ui_load_xml_file("test_border.xml");
	if (!box) {
		lcui_destroy();
		return ret;
	}
	root = ui_root();
	ui_widget_append(root, box);
	ui_widget_unwrap(box);
	return lcui_main();
}
