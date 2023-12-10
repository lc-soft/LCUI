/*
 * lib/ui-widgets/src/textstyle.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <pandagl/text.h>
#include <ui.h>

void convert_font_style_to_text_style(ui_text_style_t *fs, pd_text_style_t *ts);
