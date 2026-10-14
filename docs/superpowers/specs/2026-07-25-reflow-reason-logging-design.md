# Reflow Reason Logging Design

## Goal

Add debug output at widget reflow call sites so layout traces explain why a
widget was reflowed. Preserve the existing public reflow API.

## API Design

Keep the public functions unchanged:

```c
void ui_widget_reflow(ui_widget_t *w);
void ui_widget_reflow_if_width_changed(ui_widget_t *w);
void ui_widget_reflow_if_height_changed(ui_widget_t *w);
```

Add internal reason-aware variants in `ui_widget_layout.h`:

```c
void ui_widget_reflow_with_reason(ui_widget_t *w, const char *reason);
void ui_widget_reflow_if_width_changed_with_reason(ui_widget_t *w,
                                                    const char *reason);
void ui_widget_reflow_if_height_changed_with_reason(ui_widget_t *w,
                                                     const char *reason);
```

The public functions delegate to these variants with `"explicit reflow"`.
Layout engines call the internal variants with a call-site-specific reason.

## Logging

Add a debug helper following the existing width and height logging pattern:

```text
.choice-card-content: reflow (parent flexbox determined item width)
text: reflow (parent flexbox stretched item width)
```

Log only when a reflow is actually performed. A conditional width or height
check that detects no box-size change must not emit a reflow message.

The helper is compiled under `UI_DEBUG_ENABLED`. Non-debug builds preserve the
current behavior and avoid logger calls.

## Reasons

Use specific static reason strings at existing layout call sites:

- `parent flexbox determined item width`
- `parent flexbox determined item height`
- `parent flexbox stretched item width`
- `parent flexbox stretched item height`
- `parent block layout determined item width`
- `percentage width resolved from parent`
- `percentage height resolved from parent`
- `parent layout updated out-of-flow item size`
- `explicit reflow`

Width and height variants used together for an out-of-flow item may share the
same reason because each function logs only if its dimension causes reflow.

## Call-Site Mapping

Flexbox main-axis application uses `parent flexbox determined item width` or
`height`. Cross-axis stretch uses the corresponding `stretched` reason.
Percentage branches use the corresponding `percentage ... resolved from
parent` reason. Out-of-flow layout branches use `parent layout updated
out-of-flow item size`.

Block layout width allocation uses `parent block layout determined item
width`. Its percentage-height update uses `percentage height resolved from
parent`. The out-of-flow branch uses the shared out-of-flow reason.

## Verification

Build `lcui-tests`, run `xmake run lcui-tests --grep=choice`, and run the
choice-card preview briefly to inspect reason-bearing reflow messages. Run
clang-format and `git diff --check` for modified source files.
