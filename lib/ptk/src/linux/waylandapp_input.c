#define _GNU_SOURCE
#include "ptk.h"

#if defined(PTK_LINUX) && defined(PTK_HAS_WAYLAND)

#include "wayland_internal.h"

static void ptk_waylandapp_on_pointer_enter(
    void *data, struct wl_pointer *wl_pointer, uint32_t serial,
    struct wl_surface *surface, wl_fixed_t surface_x, wl_fixed_t surface_y)
{
        wl_app.pointer_focus = ptk_waylandapp_get_window(surface);
        /* surface_x/y are in logical pixels (compositor coordinate space);
         * store physical coords internally for cursor hotspot math */
        wl_app.pointer_x = wl_fixed_to_double(surface_x);
        wl_app.pointer_y = wl_fixed_to_double(surface_y);

        if (wl_app.default_cursor && wl_app.cursor_surface) {
                struct wl_cursor_image *image =
                    wl_app.default_cursor->images[0];
                wl_pointer_set_cursor(wl_pointer, serial, wl_app.cursor_surface,
                                      image->hotspot_x, image->hotspot_y);
                wl_surface_set_buffer_scale(wl_app.cursor_surface,
                                            wl_app.output_scale);
                wl_surface_attach(wl_app.cursor_surface,
                                  wl_cursor_image_get_buffer(image), 0, 0);
                wl_surface_damage_buffer(wl_app.cursor_surface, 0, 0,
                                         image->width, image->height);
                wl_surface_commit(wl_app.cursor_surface);
        }

        /* Wayland enter does not emit motion, but LCUI needs initial coordinate
         * state applied immediately */
        if (wl_app.pointer_focus) {
                ptk_event_t e = { 0 };
                e.type = PTK_EVENT_MOUSEMOVE;
                e.window = wl_app.pointer_focus;
                e.mouse.x = (int)(wl_app.pointer_x * wl_app.output_scale);
                e.mouse.y = (int)(wl_app.pointer_y * wl_app.output_scale);
                ptk_post_event(&e);
        }
}

static void ptk_waylandapp_on_pointer_leave(void *data,
                                            struct wl_pointer *wl_pointer,
                                            uint32_t serial,
                                            struct wl_surface *surface)
{
        wl_app.pointer_focus = NULL;
}

static void ptk_waylandapp_on_pointer_motion(void *data,
                                             struct wl_pointer *wl_pointer,
                                             uint32_t time,
                                             wl_fixed_t surface_x,
                                             wl_fixed_t surface_y)
{
        ptk_event_t e = { 0 };

        /* surface_x/y are in logical pixels (compositor coordinate space) */
        wl_app.pointer_x = wl_fixed_to_double(surface_x);
        wl_app.pointer_y = wl_fixed_to_double(surface_y);

        if (wl_app.pointer_focus) {
                e.type = PTK_EVENT_MOUSEMOVE;
                e.window = wl_app.pointer_focus;
                e.mouse.x = (int)(wl_app.pointer_x * wl_app.output_scale);
                e.mouse.y = (int)(wl_app.pointer_y * wl_app.output_scale);
                ptk_post_event(&e);
        }
}

static void ptk_waylandapp_on_pointer_button(void *data,
                                             struct wl_pointer *wl_pointer,
                                             uint32_t serial, uint32_t time,
                                             uint32_t button, uint32_t state)
{
        ptk_event_t e = { 0 };

        if (wl_app.pointer_focus) {
                e.type = state == WL_POINTER_BUTTON_STATE_PRESSED
                             ? PTK_EVENT_MOUSEDOWN
                             : PTK_EVENT_MOUSEUP;
                e.window = wl_app.pointer_focus;
                e.mouse.x = (int)(wl_app.pointer_x * wl_app.output_scale);
                e.mouse.y = (int)(wl_app.pointer_y * wl_app.output_scale);
                if (button == (0x110)) { /* BTN_LEFT */
                        e.mouse.button = MOUSE_BUTTON_LEFT;
                } else if (button == (0x111)) { /* BTN_RIGHT */
                        e.mouse.button = MOUSE_BUTTON_RIGHT;
                } else if (button == (0x112)) { /* BTN_MIDDLE */
                        e.mouse.button = MOUSE_BUTTON_MIDDLE;
                }
                ptk_post_event(&e);
        }
}

static void ptk_waylandapp_on_pointer_axis(void *data,
                                           struct wl_pointer *wl_pointer,
                                           uint32_t time, uint32_t axis,
                                           wl_fixed_t value)
{
        ptk_event_t e = { 0 };

        if (wl_app.pointer_focus) {
                e.type = PTK_EVENT_WHEEL;
                e.window = wl_app.pointer_focus;
                e.wheel.delta_mode = APP_WHEEL_DELTA_PIXEL;
                if (axis == WL_POINTER_AXIS_VERTICAL_SCROLL) {
                        e.wheel.delta_y = (int)wl_fixed_to_double(value);
                } else if (axis == WL_POINTER_AXIS_HORIZONTAL_SCROLL) {
                        e.wheel.delta_x = (int)wl_fixed_to_double(value);
                }
                ptk_post_event(&e);
        }
}

static void ptk_waylandapp_on_pointer_frame(void *data,
                                            struct wl_pointer *wl_pointer)
{
}

static void ptk_waylandapp_on_pointer_axis_source(void *data,
                                                  struct wl_pointer *wl_pointer,
                                                  uint32_t axis_source)
{
}

static void ptk_waylandapp_on_pointer_axis_stop(void *data,
                                                struct wl_pointer *wl_pointer,
                                                uint32_t time, uint32_t axis)
{
}

static void ptk_waylandapp_on_pointer_axis_discrete(
    void *data, struct wl_pointer *wl_pointer, uint32_t axis, int32_t discrete)
{
}

static const struct wl_pointer_listener pointer_listener = {
        .enter = ptk_waylandapp_on_pointer_enter,
        .leave = ptk_waylandapp_on_pointer_leave,
        .motion = ptk_waylandapp_on_pointer_motion,
        .button = ptk_waylandapp_on_pointer_button,
        .axis = ptk_waylandapp_on_pointer_axis,
        .frame = ptk_waylandapp_on_pointer_frame,
        .axis_source = ptk_waylandapp_on_pointer_axis_source,
        .axis_stop = ptk_waylandapp_on_pointer_axis_stop,
        .axis_discrete = ptk_waylandapp_on_pointer_axis_discrete,
};

/* Map an XKB keysym to LCUI's keycode (Web/Windows-style virtual key codes). */
static int ptk_waylandapp_keysym_to_keycode(xkb_keysym_t sym)
{
        /* Latin letters: XKB_KEY_a..z → KEY_A..Z ('A'..'Z') */
        if (sym >= XKB_KEY_a && sym <= XKB_KEY_z) {
                return KEY_A + (sym - XKB_KEY_a);
        }
        /* Already uppercase */
        if (sym >= XKB_KEY_A && sym <= XKB_KEY_Z) {
                return (int)sym; /* XKB_KEY_A = 0x41 = 'A' = KEY_A */
        }
        /* Digits: XKB_KEY_0..9 = 0x30..0x39 = '0'..'9' = KEY_0..KEY_9 */
        if (sym >= XKB_KEY_0 && sym <= XKB_KEY_9) {
                return (int)sym;
        }
        switch (sym) {
        case XKB_KEY_BackSpace:
                return KEY_BACKSPACE;
        case XKB_KEY_Tab:
                return KEY_TAB;
        case XKB_KEY_Return:
                return KEY_ENTER;
        case XKB_KEY_Escape:
                return KEY_ESCAPE;
        case XKB_KEY_space:
                return KEY_SPACE;
        case XKB_KEY_Delete:
                return KEY_DELETE;
        case XKB_KEY_Insert:
                return KEY_INSERT;
        case XKB_KEY_Home:
                return KEY_HOME;
        case XKB_KEY_End:
                return KEY_END;
        case XKB_KEY_Page_Up:
                return KEY_PAGEUP;
        case XKB_KEY_Page_Down:
                return KEY_PAGEDOWN;
        case XKB_KEY_Left:
                return KEY_LEFT;
        case XKB_KEY_Right:
                return KEY_RIGHT;
        case XKB_KEY_Up:
                return KEY_UP;
        case XKB_KEY_Down:
                return KEY_DOWN;
        case XKB_KEY_Shift_L:
        case XKB_KEY_Shift_R:
                return KEY_SHIFT;
        case XKB_KEY_Control_L:
        case XKB_KEY_Control_R:
                return KEY_CONTROL;
        case XKB_KEY_Alt_L:
        case XKB_KEY_Alt_R:
                return KEY_ALT;
        case XKB_KEY_Caps_Lock:
                return KEY_CAPITAL;
        case XKB_KEY_semicolon:
                return KEY_SEMICOLON;
        case XKB_KEY_equal:
                return KEY_EQUAL;
        case XKB_KEY_comma:
                return KEY_COMMA;
        case XKB_KEY_minus:
                return KEY_MINUS;
        case XKB_KEY_period:
                return KEY_PERIOD;
        case XKB_KEY_slash:
                return KEY_SLASH;
        case XKB_KEY_grave:
                return KEY_GRAVE;
        case XKB_KEY_bracketleft:
                return KEY_BRACKETLEFT;
        case XKB_KEY_backslash:
                return KEY_BACKSLASH;
        case XKB_KEY_bracketright:
                return KEY_BRACKETRIGHT;
        case XKB_KEY_apostrophe:
                return KEY_APOSTROPHE;
        default:
                return 0;
        }
}

static void ptk_waylandapp_on_keyboard_keymap(void *data,
                                              struct wl_keyboard *wl_keyboard,
                                              uint32_t format, int32_t fd,
                                              uint32_t size)
{
        void *map_str;

        if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) {
                close(fd);
                return;
        }
        map_str = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
        if (map_str == MAP_FAILED) {
                close(fd);
                return;
        }
        if (!wl_app.xkb_context) {
                wl_app.xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
                if (!wl_app.xkb_context) {
                        munmap(map_str, size);
                        close(fd);
                        return;
                }
        }
        if (wl_app.xkb_state) {
                xkb_state_unref(wl_app.xkb_state);
                wl_app.xkb_state = NULL;
        }
        if (wl_app.xkb_map) {
                xkb_keymap_unref(wl_app.xkb_map);
        }
        wl_app.xkb_map = xkb_keymap_new_from_string(
            wl_app.xkb_context, map_str, XKB_KEYMAP_FORMAT_TEXT_V1,
            XKB_KEYMAP_COMPILE_NO_FLAGS);
        munmap(map_str, size);
        close(fd);
        if (!wl_app.xkb_map) {
                return;
        }
        wl_app.xkb_state = xkb_state_new(wl_app.xkb_map);
        /* Cache modifier indices for fast lookup in modifiers handler */
        wl_app.shift_mask =
            1u << xkb_keymap_mod_get_index(wl_app.xkb_map, "Shift");
        wl_app.ctrl_mask =
            1u << xkb_keymap_mod_get_index(wl_app.xkb_map, "Control");
        wl_app.alt_mask = 1u
                          << xkb_keymap_mod_get_index(wl_app.xkb_map, "Mod1");
        wl_app.meta_mask = 1u
                           << xkb_keymap_mod_get_index(wl_app.xkb_map, "Super");
}

static void ptk_waylandapp_on_keyboard_enter(void *data,
                                             struct wl_keyboard *wl_keyboard,
                                             uint32_t serial,
                                             struct wl_surface *surface,
                                             struct wl_array *keys)
{
        wl_app.keyboard_focus = ptk_waylandapp_get_window(surface);
}

static void ptk_waylandapp_on_keyboard_leave(void *data,
                                             struct wl_keyboard *wl_keyboard,
                                             uint32_t serial,
                                             struct wl_surface *surface)
{
        wl_app.keyboard_focus = NULL;
}

static void ptk_waylandapp_on_keyboard_key(void *data,
                                           struct wl_keyboard *wl_keyboard,
                                           uint32_t serial, uint32_t time,
                                           uint32_t key, uint32_t state)
{
        /* evdev scancode → XKB keycode: XKB adds a fixed offset of 8 */
        xkb_keycode_t xkb_key = key + 8;
        xkb_keysym_t sym = XKB_KEY_NoSymbol;
        uint32_t codepoint;
        ptk_event_t e = { 0 };
        bool pressed;

        /* Some compositors send WL_KEYBOARD_KEY_STATE_REPEATED for
         * auto-repeat. Map it to pressed, same as SDL does
         * (SDL_waylandevents.c). */
        if (state == WL_KEYBOARD_KEY_STATE_PRESSED ||
            state == WL_KEYBOARD_KEY_STATE_REPEATED) {
                pressed = true;
        } else {
                pressed = false;
        }

        if (wl_app.xkb_state) {
                sym = xkb_state_key_get_one_sym(wl_app.xkb_state, xkb_key);
        }
        if (!wl_app.keyboard_focus || sym == XKB_KEY_NoSymbol) {
                return;
        }
        e.type = pressed ? PTK_EVENT_KEYDOWN : PTK_EVENT_KEYUP;
        e.window = wl_app.keyboard_focus;
        e.key.code = ptk_waylandapp_keysym_to_keycode(sym);
        e.key.shift_key = !!(wl_app.modifiers & wl_app.shift_mask);
        e.key.ctrl_key = !!(wl_app.modifiers & wl_app.ctrl_mask);
        e.key.alt_key = !!(wl_app.modifiers & wl_app.alt_mask);
        e.key.meta_key = !!(wl_app.modifiers & wl_app.meta_mask);
        ptk_post_event(&e);

        /* After KEYDOWN/REPEATED, post PTK_EVENT_KEYPRESS for printable chars
         * (non-Ctrl combinations). key.code = Unicode codepoint, matching the
         * X11 backend and what the LCUI text widget / IME layer expect. */
        if (pressed && !e.key.ctrl_key && wl_app.xkb_state) {
                codepoint = xkb_state_key_get_utf32(wl_app.xkb_state, xkb_key);
                if (codepoint >= 0x20 && codepoint != 0x7f) {
                        e.type = PTK_EVENT_KEYPRESS;
                        e.key.code = (int)codepoint;
                        ptk_post_event(&e);
                }
        }
}

static void ptk_waylandapp_on_keyboard_modifiers(
    void *data, struct wl_keyboard *wl_keyboard, uint32_t serial,
    uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked,
    uint32_t group)
{
        if (!wl_app.xkb_state) {
                return;
        }
        xkb_state_update_mask(wl_app.xkb_state, mods_depressed, mods_latched,
                              mods_locked, 0, 0, group);
        wl_app.modifiers = xkb_state_serialize_mods(wl_app.xkb_state,
                                                    XKB_STATE_MODS_EFFECTIVE);
}

static void ptk_waylandapp_on_keyboard_repeat_info(
    void *data, struct wl_keyboard *wl_keyboard, int32_t rate, int32_t delay)
{
        /* rate: characters per second (0 = no repeat); delay: ms before repeat
         * starts. Store for future key-repeat implementation (cf. SDL
         * keyboard_handle_repeat_info). */
        wl_app.repeat_rate = rate;
        wl_app.repeat_delay = delay;
}

static const struct wl_keyboard_listener keyboard_listener = {
        .keymap = ptk_waylandapp_on_keyboard_keymap,
        .enter = ptk_waylandapp_on_keyboard_enter,
        .leave = ptk_waylandapp_on_keyboard_leave,
        .key = ptk_waylandapp_on_keyboard_key,
        .modifiers = ptk_waylandapp_on_keyboard_modifiers,
        .repeat_info = ptk_waylandapp_on_keyboard_repeat_info,
};

static void ptk_waylandapp_on_seat_capabilities(void *data,
                                                struct wl_seat *seat,
                                                uint32_t caps)
{
        if ((caps & WL_SEAT_CAPABILITY_POINTER) && !wl_app.pointer) {
                wl_app.pointer = wl_seat_get_pointer(seat);
                wl_pointer_add_listener(wl_app.pointer, &pointer_listener,
                                        NULL);
                if (wl_app.shm && !wl_app.cursor_theme) {
                        wl_app.cursor_theme =
                            wl_cursor_theme_load(NULL, 32, wl_app.shm);
                        if (wl_app.cursor_theme) {
                                wl_app.default_cursor =
                                    wl_cursor_theme_get_cursor(
                                        wl_app.cursor_theme, "left_ptr");
                                wl_app.cursor_surface =
                                    wl_compositor_create_surface(
                                        wl_app.compositor);
                        }
                }
        } else if (!(caps & WL_SEAT_CAPABILITY_POINTER) && wl_app.pointer) {
                wl_pointer_destroy(wl_app.pointer);
                wl_app.pointer = NULL;
        }

        if ((caps & WL_SEAT_CAPABILITY_KEYBOARD) && !wl_app.keyboard) {
                wl_app.keyboard = wl_seat_get_keyboard(seat);
                wl_keyboard_add_listener(wl_app.keyboard, &keyboard_listener,
                                         NULL);
        } else if (!(caps & WL_SEAT_CAPABILITY_KEYBOARD) && wl_app.keyboard) {
                wl_keyboard_destroy(wl_app.keyboard);
                wl_app.keyboard = NULL;
        }
}

static void ptk_waylandapp_on_seat_name(void *data, struct wl_seat *seat,
                                        const char *name)
{
}

const struct wl_seat_listener seat_listener = {
        .capabilities = ptk_waylandapp_on_seat_capabilities,
        .name = ptk_waylandapp_on_seat_name,
};

#endif /* PTK_LINUX && PTK_HAS_WAYLAND */
