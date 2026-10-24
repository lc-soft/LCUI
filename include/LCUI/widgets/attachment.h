/*
 * include/LCUI/widgets/attachment.h: -- LCUI's Attachment widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_ATTACHMENT_H
#define LCUI_INCLUDE_WIDGETS_ATTACHMENT_H

#include <LCUI/common.h>
#include <ui/base.h>

LCUI_BEGIN_HEADER

typedef enum ui_attachment_state {
        UI_ATTACHMENT_STATE_IDLE,
        UI_ATTACHMENT_STATE_UPLOADING,
        UI_ATTACHMENT_STATE_PROCESSING,
        UI_ATTACHMENT_STATE_ERROR,
        UI_ATTACHMENT_STATE_DONE,
} ui_attachment_state_t;

typedef enum ui_attachment_size {
        UI_ATTACHMENT_SIZE_DEFAULT,
        UI_ATTACHMENT_SIZE_SM,
        UI_ATTACHMENT_SIZE_XS,
} ui_attachment_size_t;

typedef enum ui_attachment_orientation {
        UI_ATTACHMENT_ORIENTATION_HORIZONTAL,
        UI_ATTACHMENT_ORIENTATION_VERTICAL,
} ui_attachment_orientation_t;

typedef enum ui_attachment_media_variant {
        UI_ATTACHMENT_MEDIA_VARIANT_ICON,
        UI_ATTACHMENT_MEDIA_VARIANT_IMAGE,
} ui_attachment_media_variant_t;

LCUI_API ui_widget_t *ui_create_attachment(void);
LCUI_API ui_widget_t *ui_create_attachment_media(void);
LCUI_API ui_widget_t *ui_create_attachment_content(void);
LCUI_API ui_widget_t *ui_create_attachment_title(void);
LCUI_API ui_widget_t *ui_create_attachment_description(void);
LCUI_API ui_widget_t *ui_create_attachment_actions(void);
LCUI_API ui_widget_t *ui_create_attachment_action(void);
LCUI_API ui_widget_t *ui_create_attachment_trigger(void);
LCUI_API ui_widget_t *ui_create_attachment_group(void);

LCUI_API void ui_attachment_set_state(ui_widget_t *attachment,
                                      ui_attachment_state_t state);
LCUI_API ui_attachment_state_t ui_attachment_get_state(ui_widget_t *attachment);

LCUI_API void ui_attachment_set_size(ui_widget_t *attachment,
                                     ui_attachment_size_t size);
LCUI_API ui_attachment_size_t ui_attachment_get_size(ui_widget_t *attachment);

LCUI_API void ui_attachment_set_orientation(
    ui_widget_t *attachment, ui_attachment_orientation_t orientation);
LCUI_API ui_attachment_orientation_t
ui_attachment_get_orientation(ui_widget_t *attachment);

LCUI_API void ui_attachment_media_set_variant(
    ui_widget_t *media, ui_attachment_media_variant_t variant);
LCUI_API ui_attachment_media_variant_t
ui_attachment_media_get_variant(ui_widget_t *media);

/* Set the media preview from a local image file. The image is painted as a
 * background so the media box can keep its square/rounded clipping without an
 * extra child widget. */
LCUI_API void ui_attachment_media_set_image_url(ui_widget_t *media,
                                                const char *path);

LCUI_API void ui_attachment_set_title(ui_widget_t *attachment,
                                      const char *text);
LCUI_API void ui_attachment_set_description(ui_widget_t *attachment,
                                            const char *text);

LCUI_API void ui_register_attachment(void);

LCUI_END_HEADER

#endif
