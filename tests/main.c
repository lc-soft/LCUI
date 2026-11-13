/*
 * tests/main.c
 *
 * Copyright (c) 2023-2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest.h>

extern void test_settings(void);
extern void test_clipboard(void);
extern void test_mainloop(void);
extern void test_worker(void);
extern void test_router_components(void);
extern void test_widget_event(void);
extern void test_widget_rect(void);
extern void test_css_inheritance(void);
extern void test_text_resize(void);
extern void test_text_decoration(void);
extern void test_textinput(void);
extern void test_font_face(void);
extern void test_widget_opacity(void);
extern void test_block_layout(void);
extern void test_flex_layout(void);
extern void test_flex_layout_percentage_wrap(void);
extern void test_flex_contract(void);
extern void test_empty_layout(void);
extern void test_field_layout(void);
extern void test_home_layout(void);
extern void test_gallery_layout(void);
extern void test_background_style(void);
extern void test_style_refresh(void);
extern void test_flex_hidden_gap(void);
extern void test_sizehint(void);
extern void test_scrollbar(void);
extern void test_scrollarea(void);
extern void test_progress(void);
extern void test_label(void);
extern void test_checkbox(void);
extern void test_field(void);
extern void test_radio_group(void);
extern void test_portal(void);
extern void test_select(void);
extern void test_sidebar(void);
extern void test_cursor(void);
extern void test_icon(void);
extern void test_icon_centering(void);
extern void test_button(void);
extern void test_button_group(void);
extern void test_badge(void);
extern void test_card(void);
extern void test_alert(void);
extern void test_item(void);
extern void test_attachment(void);
extern void test_collapsible(void);
extern void test_dropdown_menu(void);
extern void test_empty(void);
extern void test_tooltip(void);
extern void test_popover(void);
extern void test_draw_backend(void);
extern void test_spinner(void);
extern void test_spinner_perf(void);
extern void test_shimmer(void);
extern void test_style_nova(void);
extern void test_toast(void);
extern void test_accordion(void);
extern void test_breadcrumb(void);
extern void test_input_group(void);

static const ctest_suite_t suites[] = {
        { "settings", test_settings },
        { "clipboard", test_clipboard },
        { "mainloop", test_mainloop },
        { "worker", test_worker },
        { "router components", test_router_components },
        { "widget event", test_widget_event },
        { "widget rect", test_widget_rect },
        { "css inheritance", test_css_inheritance },
        { "text resize", test_text_resize },
        { "text decoration", test_text_decoration },
        { "textinput", test_textinput },
        { "font face", test_font_face },
        { "widget opacity", test_widget_opacity },
        { "block layout", test_block_layout },
        { "flex layout", test_flex_layout },
        { "flex layout percentage wrap", test_flex_layout_percentage_wrap },
        { "flex contract", test_flex_contract },
        { "empty layout", test_empty_layout },
        { "field layout", test_field_layout },
        { "home layout", test_home_layout },
        { "gallery layout", test_gallery_layout },
        { "background style", test_background_style },
        { "style refresh", test_style_refresh },
        { "flex hidden gap", test_flex_hidden_gap },
        { "sizehint", test_sizehint },
        { "scrollbar", test_scrollbar },
        { "scrollarea", test_scrollarea },
        { "progress", test_progress },
        { "label", test_label },
        { "checkbox", test_checkbox },
        { "field", test_field },
        { "radio-group", test_radio_group },
        { "portal", test_portal },
        { "select", test_select },
        { "sidebar", test_sidebar },
        { "cursor", test_cursor },
        { "icon", test_icon },
        { "icon centering", test_icon_centering },
        { "button", test_button },
        { "button group", test_button_group },
        { "badge", test_badge },
        { "card", test_card },
        { "alert", test_alert },
        { "item", test_item },
        { "attachment", test_attachment },
        { "collapsible", test_collapsible },
        { "dropdown-menu", test_dropdown_menu },
        { "empty", test_empty },
        { "tooltip", test_tooltip },
        { "popover", test_popover },
        { "draw backend", test_draw_backend },
        { "spinner", test_spinner },
        { "spinner perf", test_spinner_perf },
        { "shimmer", test_shimmer },
        { "style nova", test_style_nova },
        { "toast", test_toast },
        { "accordion", test_accordion },
        { "breadcrumb", test_breadcrumb },
        { "input group", test_input_group },
        { NULL, NULL }
};

int main(int argc, char **argv)
{
        logger_set_level(LOGGER_LEVEL_ERROR);
        if (ctest_parse_args(argc, argv) != 0) {
                return 1;
        }
        return ctest_run_suites(suites);
}
