#include <LCUI.h>

static const char *spinner_size_css = "\
.spinner-size-demo {\
  display: flex;\
  flex-direction: row;\
  align-items: center;\
  gap: 16px;\
}\
.spinner-size-demo .sz-sm {\
  width: 24px;\
  height: 24px;\
}\
.spinner-size-demo .sz-md {\
  width: 40px;\
  height: 40px;\
}\
.spinner-size-demo .sz-lg {\
  width: 64px;\
  height: 64px;\
}\
";

void spinner_size_init(ui_widget_t *parent)
{
        ui_widget_t *box;
        ui_widget_t *spinner;
        const char *sizes[] = { "sz-sm", "sz-md", "sz-lg" };
        size_t i;

        ui_load_css_string(spinner_size_css, "spinner-size-demo");
        box = ui_create_widget(NULL);
        ui_widget_add_class(box, "spinner-size-demo");
        for (i = 0; i < 3; ++i) {
                spinner = ui_create_spinner();
                ui_widget_add_class(spinner, sizes[i]);
                ui_widget_append(box, spinner);
        }
        ui_widget_append(parent, box);
}
