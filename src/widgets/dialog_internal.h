/*
 * src/widgets/dialog_internal.h: -- internal shared state of dialog widgets
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_SRC_WIDGETS_DIALOG_INTERNAL_H
#define LCUI_SRC_WIDGETS_DIALOG_INTERNAL_H

#include <LCUI/widgets/dialog.h>
#include <ui/types.h>

struct ui_dialog {
        ui_widget_t *owner;
        ui_widget_t *trigger;
        ui_widget_t *portal;
        ui_widget_t *overlay;
        ui_widget_t *content;
        ui_widget_t *last_focus;
        list_node_t node;
        bool open;
        bool dismissible;
        bool close_button;
        bool bound;
        bool loose;
        char *key;
};

ui_widget_t *ui_dialog_find_owner(ui_widget_t *w);
ui_widget_t *ui_dialog_find_descendant(ui_widget_t *w,
                                       ui_widget_prototype_t *proto);
void ui_dialog_bind_semantic_node(ui_widget_t *w, bool is_action);

#endif
