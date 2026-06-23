/**
 * app/main.c - Documentation viewer entry point.
 *
 * lcui-cli regenerates app/main.h on every build with:
 *   - app_router_init()  -- every route from sidebars.json
 *   - app_init()         -- lcui_init, resource loaders, widget registrations,
 *                          and the initial root_layout mount
 *   - app_run()          -- the LCUI main loop
 *
 * This main.c only orchestrates the entry point and registers the doc
 * viewer's semantic text aliases (h1-h6, p) before any page widget is
 * created.  LCUI ships only a "text" prototype; without these aliases,
 * <h1>/<p>/... widgets emitted from MDX would have no text renderer
 * and their content would not paint.
 *
 * The aliases are registered AFTER app_init() (so lcui_init() has set up
 * the prototype system) and BEFORE app_run() (so they exist before the
 * router instantiates the first page widget).  app_init() itself only
 * mounts the root layout, which contains no h1/p widgets -- those
 * appear only once a page is routed in.
 */

#include "main.h"
#include "components/list.h"
#include "locale.h"
#include <stdio.h>

static void register_semantic_text_aliases(void)
{
        static const char *aliases[] = {
                "h1", "h2", "h3", "h4", "h5", "h6", "p", NULL,
        };
        const char **name;

        for (name = aliases; *name; ++name) {
                ui_create_widget_prototype(*name, "text");
        }
}

int main(int argc, char *argv[])
{
        lcui_set_app_id("dev.lcui.doc-viewer");
        app_init();
        register_semantic_text_aliases();
        ui_register_list_widgets();

        router_t *router = router_get_by_name("AppRouter");
        const char *locale = app_get_locale();
        char initial_path[128];
        router_location_t *location;

        snprintf(initial_path, sizeof(initial_path), "/%s/overview/quick-start",
                 locale ? locale : "en");
        location = router_location_create(NULL, initial_path);
        router_push(router, location);
        router_location_destroy(location);

        ui_widget_resize(ui_root(), 1280, 720);
        lcui_bind_window_state(ui_root(), "main");

        return app_run();
}
