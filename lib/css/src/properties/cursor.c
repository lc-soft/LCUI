/*
 * lib/css/src/properties/cursor.c
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include "../properties.h"

int css_cascade_cursor(const css_style_array_value_t input,
                       css_computed_style_t *computed)
{
        uint8_t value = CSS_CURSOR_AUTO;

        switch (input[0].keyword_value) {
        case CSS_KEYWORD_DEFAULT:
                value = CSS_CURSOR_DEFAULT;
                break;
        case CSS_KEYWORD_NONE:
                value = CSS_CURSOR_NONE;
                break;
        case CSS_KEYWORD_CONTEXT_MENU:
                value = CSS_CURSOR_CONTEXT_MENU;
                break;
        case CSS_KEYWORD_HELP:
                value = CSS_CURSOR_HELP;
                break;
        case CSS_KEYWORD_POINTER:
                value = CSS_CURSOR_POINTER;
                break;
        case CSS_KEYWORD_PROGRESS:
                value = CSS_CURSOR_PROGRESS;
                break;
        case CSS_KEYWORD_WAIT:
                value = CSS_CURSOR_WAIT;
                break;
        case CSS_KEYWORD_CELL:
                value = CSS_CURSOR_CELL;
                break;
        case CSS_KEYWORD_CROSSHAIR:
                value = CSS_CURSOR_CROSSHAIR;
                break;
        case CSS_KEYWORD_TEXT:
                value = CSS_CURSOR_TEXT;
                break;
        case CSS_KEYWORD_VERTICAL_TEXT:
                value = CSS_CURSOR_VERTICAL_TEXT;
                break;
        case CSS_KEYWORD_ALIAS:
                value = CSS_CURSOR_ALIAS;
                break;
        case CSS_KEYWORD_COPY:
                value = CSS_CURSOR_COPY;
                break;
        case CSS_KEYWORD_MOVE:
                value = CSS_CURSOR_MOVE;
                break;
        case CSS_KEYWORD_NO_DROP:
                value = CSS_CURSOR_NO_DROP;
                break;
        case CSS_KEYWORD_NOT_ALLOWED:
                value = CSS_CURSOR_NOT_ALLOWED;
                break;
        case CSS_KEYWORD_GRAB:
                value = CSS_CURSOR_GRAB;
                break;
        case CSS_KEYWORD_GRABBING:
                value = CSS_CURSOR_GRABBING;
                break;
        case CSS_KEYWORD_ALL_SCROLL:
                value = CSS_CURSOR_ALL_SCROLL;
                break;
        case CSS_KEYWORD_COL_RESIZE:
                value = CSS_CURSOR_COL_RESIZE;
                break;
        case CSS_KEYWORD_ROW_RESIZE:
                value = CSS_CURSOR_ROW_RESIZE;
                break;
        case CSS_KEYWORD_N_RESIZE:
                value = CSS_CURSOR_N_RESIZE;
                break;
        case CSS_KEYWORD_E_RESIZE:
                value = CSS_CURSOR_E_RESIZE;
                break;
        case CSS_KEYWORD_S_RESIZE:
                value = CSS_CURSOR_S_RESIZE;
                break;
        case CSS_KEYWORD_W_RESIZE:
                value = CSS_CURSOR_W_RESIZE;
                break;
        case CSS_KEYWORD_EW_RESIZE:
                value = CSS_CURSOR_EW_RESIZE;
                break;
        case CSS_KEYWORD_NS_RESIZE:
                value = CSS_CURSOR_NS_RESIZE;
                break;
        case CSS_KEYWORD_NE_RESIZE:
                value = CSS_CURSOR_NE_RESIZE;
                break;
        case CSS_KEYWORD_NW_RESIZE:
                value = CSS_CURSOR_NW_RESIZE;
                break;
        case CSS_KEYWORD_SE_RESIZE:
                value = CSS_CURSOR_SE_RESIZE;
                break;
        case CSS_KEYWORD_SW_RESIZE:
                value = CSS_CURSOR_SW_RESIZE;
                break;
        case CSS_KEYWORD_NESW_RESIZE:
                value = CSS_CURSOR_NESW_RESIZE;
                break;
        case CSS_KEYWORD_NWSE_RESIZE:
                value = CSS_CURSOR_NWSE_RESIZE;
                break;
        case CSS_KEYWORD_ZOOM_IN:
                value = CSS_CURSOR_ZOOM_IN;
                break;
        case CSS_KEYWORD_ZOOM_OUT:
                value = CSS_CURSOR_ZOOM_OUT;
                break;
        case CSS_KEYWORD_AUTO:
        default:
                value = CSS_CURSOR_AUTO;
                break;
        }
        computed->type_bits.cursor = value;
        return 0;
}
