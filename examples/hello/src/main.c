#include <LCUI.h>
#include <platform/main.h>

static void on_btn_click(ui_widget_t *self, ui_event_t *e, void *arg)
{
        wchar_t str[256];
        ui_widget_t *edit = ui_get_widget("edit");
        ui_widget_t *txt = ui_get_widget("text-hello");

        ui_textinput_get_text_w(edit, 0, 255, str);
        ui_text_set_content_w(txt, str);
}

int main(int argc, char **argv)
{
        ui_widget_t *pack;

        lcui_init();
        pack = ui_load_xml_file("hello.xml");
        if (!pack) {
                return -1;
        }
        ui_root_append(pack);
        ui_widget_unwrap(pack);
        ui_widget_on(ui_get_widget("btn"), "click", on_btn_click, NULL);
        return lcui_main();
}
