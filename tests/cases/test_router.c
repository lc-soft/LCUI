
#include <router.h>
#include <ctest.h>
#include <LCUI/widgets.h>

void test_router_components(void)
{
        router_t *router;
        router_config_t *config;
        ui_widget_t *root;
        ui_widget_t *link_foo;
        ui_widget_t *link_foobar;
        ui_widget_t *link_bar;
        ui_widget_t *view;
        ui_widget_t *matched_widget;
        ui_event_t e;

        router = router_create(NULL);
        config = router_config_create();
        router_config_set_path(config, "/foo");
        router_config_set_component(config, NULL, "foo");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "/foo/bar");
        router_config_set_component(config, NULL, "foobar");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        config = router_config_create();
        router_config_set_path(config, "/bar");
        router_config_set_component(config, NULL, "bar");
        router_add_route_record(router, config, NULL);
        router_config_destroy(config);

        ui_init();
        ui_register_text();
        ui_register_router_link();
        ui_register_router_view();
        ui_create_widget_prototype("foo", NULL);
        ui_create_widget_prototype("foobar", NULL);
        ui_create_widget_prototype("bar", NULL);
        root = ui_root();
        view = ui_create_widget("router-view");
        link_foo = ui_create_widget("router-link");
        link_foobar = ui_create_widget("router-link");
        link_bar = ui_create_widget("router-link");
        ui_widget_set_attr(link_foo, "to", "/foo");
        ui_widget_set_attr(link_foobar, "to", "/foo/bar");
        ui_widget_set_attr(link_bar, "to", "/bar");
        ui_widget_append(root, link_foo);
        ui_widget_append(root, link_foobar);
        ui_widget_append(root, link_bar);
        ui_widget_append(root, view);
        ui_update();

        ui_event_init(&e, "click");
        ui_widget_emit_event(link_foo, e, NULL);
        matched_widget = ui_router_view_get_matched_widget(view);
        ctest_equal_bool("[/foo] <router-view> widget should load <foo> widget",
                         strcmp(matched_widget->type, "foo") == 0, true);
        ctest_equal_bool(
            "[/foo] linkFoo should has active classes",
            (ui_widget_has_class(link_foo, "router-link-active") &&
             ui_widget_has_class(link_foo, "router-link-exact-active")),
            true);
        ui_widget_emit_event(link_bar, e, NULL);
        matched_widget = ui_router_view_get_matched_widget(view);
        ctest_equal_bool("[/bar] <router-view> widget should load <bar> widget",
                         strcmp(matched_widget->type, "bar") == 0, true);
        ctest_equal_bool(
            "[/bar] linkFoo should not has active classes",
            (!ui_widget_has_class(link_foo, "router-link-exact-active") &&
             !ui_widget_has_class(link_foo, "router-link-active")),
            true);

        ui_widget_emit_event(link_foobar, e, NULL);
        matched_widget = ui_router_view_get_matched_widget(view);
        ctest_equal_bool(
            "[/foo/bar] <router-view> widget should load <foobar> widget",
            strcmp(matched_widget->type, "foobar") == 0, true);
        ctest_equal_bool(
            "[/foo/bar] linkFooBar should has active classes",
            (ui_widget_has_class(link_foobar, "router-link-exact-active") &&
             ui_widget_has_class(link_foobar, "router-link-active")),
            true);
        ctest_equal_bool(
            "[/foo/bar] linkFoo should only has the exact active class",
            (!ui_widget_has_class(link_foo, "router-link-exact-active") &&
             ui_widget_has_class(link_foo, "router-link-active")),
            true);
        ui_widget_set_attr(link_foo, "exact", "exact");
        ui_widget_emit_event(link_foobar, e, NULL);
        ctest_equal_bool(
            "[/foo/bar] linkFoo should not has any active classes (exact)",
            (!ui_widget_has_class(link_foo, "router-link-exact-active") &&
             !ui_widget_has_class(link_foo, "router-link-active")),
            true);

        ui_destroy();
        router_destroy(router);
}
