/*
 * lib/css/src/properties.h
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <css/computed.h>
#include <css/properties.h>
#include <css/style_value.h>
#include <css/style_decl.h>
#include <css/value.h>

void css_compute_font_size_value(const css_computed_style_t *parent,
                                 css_computed_style_t *computed);
