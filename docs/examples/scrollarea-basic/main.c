#include <LCUI.h>

void scrollarea_basic_init(ui_widget_t *parent)
{
        ui_widget_t *scroll, *content, *vbar, *text;

        scroll = ui_create_scrollarea();
        ui_widget_set_style_string(scroll, "width", "300px");
        ui_widget_set_style_string(scroll, "height", "200px");
        ui_widget_set_style_string(scroll, "border", "1px solid #d0d7de");

        content = ui_create_scrollarea_content();
        text = ui_create_widget("text");
        ui_text_set_content_w(
            text,
            L"这是一段用于演示滚动区域功能的示例文本。\n"
            L"当文本内容超出容器高度时，用户可以通过滚动条来查看其余内容。\n"
            L"滚动区域适合阅读长篇文章、展示数据列表。\n"
            L"在实际项目中，滚动区域的尺寸通常由其父布局决定，"
            L"开发者只需关注内容本身。\n"
            L"滚动区域可以嵌套使用，构建复杂的多层滚动界面。\n"
            L"合理配置滚动方向和样式，可以让界面更加整洁且易于使用。");
        ui_widget_append(content, text);

        vbar = ui_create_widget("scrollbar");

        ui_widget_append(scroll, content);
        ui_widget_append(scroll, vbar);
        ui_widget_append(parent, scroll);
}
