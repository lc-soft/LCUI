# libui-router

(**English**/[中文](README.zh-cn.md))

`libui-router` is the UI binding layer that connects [librouter](../router/README.md) to [libui](../ui/) widgets.

It ships two widgets:

- `router-link` — a clickable widget that triggers navigation; mirrors `<router-link>` from Vue Router. Toggles `router-link-active` and `router-link-exact-active` classes based on the current route.
- `router-view` — a route outlet that renders the matched component for the current route.

## Usage

```c
#include <ui.h>
#include <ui_router.h>

ui_init();
ui_register_router_link();
ui_register_router_view();

/* Now "router-link" and "router-view" widget types are available. */
```

When LCUI is used as a whole, `lcui_init()` already calls these registration functions through `lcui_widgets_init()`; this lib only needs to be called manually if you embed `libui-router` without the LCUI top-level shell.

## License

[MIT licensed](../../LICENSE.TXT).
