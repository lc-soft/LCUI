/*
 * include/LCUI/widgets/portal.h: -- Portal widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_PORTAL_H
#define LCUI_INCLUDE_WIDGETS_PORTAL_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

/** Create a Portal widget. */
LCUI_API ui_widget_t *ui_create_portal(void);

/**
 * Set the content widget for the Portal.
 *
 * The content widget is reparented to portal-root; the Portal keeps a
 * reference only.
 */
LCUI_API void ui_portal_set_content(ui_widget_t *portal, ui_widget_t *content);

/** Get the current content widget of the Portal. */
LCUI_API ui_widget_t *ui_portal_get_content(ui_widget_t *portal);

/** Set the anchor widget for positioning. */
LCUI_API void ui_portal_set_anchor(ui_widget_t *portal, ui_widget_t *anchor);

/**
 * Set the side from which the portal content expands relative to the anchor.
 *
 * @param side One of "top", "bottom" (default), "left", "right".
 *    When not set, defaults to "bottom".
 */
LCUI_API void ui_portal_set_side(ui_widget_t *portal, const char *side);

/** Set the side offset in pixels. */
LCUI_API void ui_portal_set_side_offset(ui_widget_t *portal, int offset);

/**
 * Set the alignment of the portal content along the side axis.
 *
 * @param align One of "start", "center" (default), "end".
 *    When not set, defaults to "center".
 */
LCUI_API void ui_portal_set_align(ui_widget_t *portal, const char *align);

/** Set the alignment offset in pixels. */
LCUI_API void ui_portal_set_align_offset(ui_widget_t *portal, int offset);

/** Open the Portal, making the content visible. */
LCUI_API void ui_portal_open(ui_widget_t *portal);

/** Close the Portal, hiding the content. */
LCUI_API void ui_portal_close(ui_widget_t *portal);

/** Get the global portal-root instance (lazily created). */
LCUI_API ui_widget_t *ui_get_portal_root(void);

/** Register the Portal widget type. */
LCUI_API void ui_register_portal(void);

LCUI_END_HEADER

#endif
