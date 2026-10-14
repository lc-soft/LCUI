#include <LCUI.h>

static const char *spinner_custom_content_css = "\
.spinner-custom-content-demo {\
  display: flex;\
  align-items: center;\
  gap: 28px;\
}\
.spinner-custom-content-demo spinner-content {\
  display: flex;\
  align-items: center;\
  justify-content: center;\
}\
.spinner-custom-content-demo spinner-content .c-dot {\
  position: absolute;\
  left: 15px;\
  top: 3px;\
  width: 10px;\
  height: 10px;\
  background-color: #4a90d9;\
  border-radius: 8px;\
}\
.spinner-custom-content-demo spinner-content .c-bar {\
  position: absolute;\
  left: 18px;\
  top: 6px;\
  width: 4px;\
  height: 20px;\
  background-color: #d9534f;\
  border-radius: 2px;\
}\
.spinner-custom-content-demo spinner-content .c-text {\
  white-space: nowrap;\
  font-size: 18px;\
  color: #e5484d;\
}\
";

static ui_widget_t *create_shape_content(void)
{
        ui_widget_t *content, *dot, *bar;

        content = ui_create_widget("spinner-content");
        dot = ui_create_widget(NULL);
        bar = ui_create_widget(NULL);
        ui_widget_add_class(dot, "c-dot");
        ui_widget_add_class(bar, "c-bar");
        ui_widget_append(content, dot);
        ui_widget_append(content, bar);
        return content;
}

static ui_widget_t *create_text_content(void)
{
        ui_widget_t *content, *text;

        content = ui_create_widget("spinner-content");
        text = ui_create_widget("text");
        ui_text_set_content(text, "Hello");
        ui_widget_add_class(text, "c-text");
        ui_widget_append(content, text);
        return content;
}

void spinner_custom_content_init(ui_widget_t *parent)
{
        ui_widget_t *row, *spinner;

        ui_load_css_string(spinner_custom_content_css,
                           "spinner-custom-content-demo");
        row = ui_create_widget(NULL);
        ui_widget_add_class(row, "spinner-custom-content-demo");

        spinner = ui_create_spinner();
        ui_widget_resize(spinner, 40, 40);
        ui_spinner_set_content(spinner, create_shape_content());
        ui_widget_append(row, spinner);

        spinner = ui_create_spinner();
        ui_widget_resize(spinner, 96, 32);
        ui_spinner_set_content(spinner, create_text_content());
        ui_widget_append(row, spinner);

        ui_widget_append(parent, row);
}
