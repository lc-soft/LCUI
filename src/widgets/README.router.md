# Router widgets

(**English**/[中文](README.router.zh-cn.md))

The router widgets connect [librouter](../../lib/router/README.md) to
[libui](../../lib/ui/) widgets.

They are part of the LCUI core and are registered by `lcui_init()`. The
module provides two widgets:

- `router-link` — a clickable widget that triggers navigation; mirrors `<router-link>` from Vue Router. Toggles `router-link-active` and `router-link-exact-active` classes based on the current route.
- `router-view` — a route outlet that renders the matched component for the current route.

## Usage

```c
#include <LCUI.h>

lcui_init();

/* "router-link" and "router-view" widget types are available. */
```

When embedding `libui` without the LCUI top-level shell, include
`<LCUI/widgets/router.h>` and call `ui_register_router_link()` and
`ui_register_router_view()` manually.

## License

[MIT licensed](../../LICENSE.TXT).
