# Layout Size Debugging Design

**Goal:** Trace layout-time `width` and `height` reads and writes with concise source-aware debug logs.

## Scope

- Track only `computed_style.width` and `computed_style.height`.
- Do not track derived box dimensions, layout coordinates, margins, `flex_basis`, or min/max dimensions.
- Disable the existing verbose `UI_DEBUG_ENABLED` layout traces in the block and flexbox layout files.
- Use `${widget}` in every new `ui_debug()` format string.

## Debug API

`lib/ui/src/ui_debug.c` owns six internal helpers declared by
`lib/ui/src/ui_debug.h`:

```c
void ui_debug_read_size(ui_widget_t *w, const char *prop,
                        const char *reason);
void ui_debug_write_size(ui_widget_t *w, const char *prop, float old_value,
                         float new_value, const char *reason);
float ui_debug_get_width(ui_widget_t *w, const char *reason);
float ui_debug_get_height(ui_widget_t *w, const char *reason);
void ui_debug_set_width(ui_widget_t *w, float value, const char *reason);
void ui_debug_set_height(ui_widget_t *w, float value, const char *reason);
```

The getter helpers read `computed_style.width` or `computed_style.height`, log
the reason, and return the value. The setter helpers capture the old value,
apply `CSS_SET_FIXED_LENGTH`, and log the old and new values. Call sites pass
the reason, so no temporary old-value variable is needed.

Expected log forms:

```text
[size read] ${widget} width (flexbox row item flex-basis seed)
[size write] ${widget} height: 24 -> 48 (flexbox row item cross stretch)
```

## Value Sources

- `fixed width` / `fixed height`: a fixed declared size is restored and used
  as the flex-basis seed.
- `fit-content`: a size calculated from the widget's content or accumulated
  layout content is applied.
- `fill-available`: a block child receives the available content width of its
  parent.
- Flexbox item writes identify axis and algorithm source:
  `flexbox row item main flex-basis`, `flexbox row item main flex-grow`,
  `flexbox row item main flex-shrink`, and their `column` equivalents.
- Flexbox stretch writes identify the cross axis:
  `flexbox row item cross stretch` or `flexbox column item cross stretch`.

`ui_widget_set_content_width()` and `ui_widget_set_content_height()` always
receive content-derived values at their current call sites, so their reason is
`fit-content`, not `content width` or `content height` and not a fixed style
value.

## File Changes

- `lib/ui/src/ui_debug.h`: declare the size logging and debug width/height
  access helpers.
- `lib/ui/src/ui_debug.c`: implement those helpers using `ui_debug()` and the
  `${widget}` placeholder.
- `lib/ui/src/ui_widget.c`: route `ui_widget_set_content_width()` and
  `ui_widget_set_content_height()` through the debug setters with
  `fit-content`.
- `lib/ui/src/ui_resizer.c`: route width fit-content and fill-available writes
  through `ui_debug_set_width()` with their respective reasons.
- `lib/ui/src/ui_widget_layout.c`: route reflow-time fit-content width/height
  writes through debug setters.
- `lib/ui/src/ui_block_layout.c`: retain the existing semantic helper calls;
  disable legacy `UI_DEBUG_ENABLED` flow logging.
- `lib/ui/src/ui_flexbox_layout.c`: route direct width/height writes through
  debug setters, route flex-basis seed reads through debug getters, and
  disable legacy `UI_DEBUG_ENABLED` flow logging.

## Verification

- Format every changed C source and header with the repository clang-format.
- Build the affected UI library and the layout test binary with xmake.
- Run the existing flex and block layout integration suites. The change only
  adds debug-level logs and must not alter computed sizes or layout results.
