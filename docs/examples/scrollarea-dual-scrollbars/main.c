#include <stdio.h>
#include <LCUI.h>

void scrollarea_dual_scrollbars_init(ui_widget_t *parent)
{
        int i;
        char buf[8];
        ui_widget_t *area, *content, *hbar, *vbar, *box;

        area = ui_create_scrollarea();
        ui_widget_set_style_string(area, "width", "420px");
        ui_widget_set_style_string(area, "height", "300px");
        ui_widget_set_style_string(area, "border", "1px solid #d0d7de");

        content = ui_create_scrollarea_content();
        ui_widget_set_style_string(content, "display", "flex");
        ui_widget_set_style_string(content, "flex-wrap", "wrap");
        ui_widget_set_style_string(content, "width", "1088px");
        ui_widget_set_style_string(content, "height", "1088px");
        ui_widget_set_style_string(content, "gap", "8px");
        ui_widget_set_style_string(content, "padding", "8px");

        for (i = 0; i < 100; ++i) {
                snprintf(buf, sizeof(buf), "%d", i + 1);
                box = ui_create_widget("text");
                ui_text_set_content(box, buf);
                ui_widget_set_style_string(box, "width", "100px");
                ui_widget_set_style_string(box, "height", "100px");
                ui_widget_set_style_string(box, "background", "#e1e5e9");
                ui_widget_set_style_string(box, "border", "1px solid #b0b8c0");
                ui_widget_set_style_string(box, "text-align", "center");
                ui_widget_set_style_string(box, "line-height", "100px");
                ui_widget_append(content, box);
        }

        hbar = ui_create_widget("scrollbar");
        ui_widget_set_attr(hbar, "orientation", "horizontal");
        vbar = ui_create_widget("scrollbar");

        ui_widget_append(area, content);
        ui_widget_append(area, hbar);
        ui_widget_append(area, vbar);
        ui_widget_append(parent, area);
}
