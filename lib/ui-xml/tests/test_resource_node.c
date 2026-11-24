#include <string.h>
#include <ui.h>
#include <ui_xml.h>
#include <ctest.h>

static void check_resource_missing_type(void)
{
        const char *xml =
            "<lcui-app><ui><resource src=\"theme.css\"/></ui></lcui-app>";
        ui_widget_t *pack;

        pack = ui_load_xml_string(xml, (int)strlen(xml));
        ctest_equal_bool("should parse without crashing", pack != NULL, true);
        if (pack) {
                ui_widget_destroy(pack);
        }
}

static void check_resource_missing_type_and_src(void)
{
        const char *xml = "<lcui-app><ui><resource/></ui></lcui-app>";
        ui_widget_t *pack;

        pack = ui_load_xml_string(xml, (int)strlen(xml));
        ctest_equal_bool("should return pack for empty resource", pack != NULL,
                         true);
        if (pack) {
                ui_widget_destroy(pack);
        }
}

static void check_inline_css_resource(void)
{
        const char *xml = "<lcui-app><ui><resource type=\"text/css\">"
                          "a{display:block;}</resource></ui></lcui-app>";
        ui_widget_t *pack;

        pack = ui_load_xml_string(xml, (int)strlen(xml));
        ctest_equal_bool("should parse inline css resource", pack != NULL,
                         true);
        if (pack) {
                ui_widget_destroy(pack);
        }
}

void test_ui_xml_resource_node(void)
{
        ui_init();
        ctest_describe("resource missing type", check_resource_missing_type);
        ctest_describe("resource missing type and src",
                       check_resource_missing_type_and_src);
        ctest_describe("inline css resource", check_inline_css_resource);
        ui_destroy();
}
