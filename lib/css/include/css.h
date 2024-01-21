/*
 * lib/css/include/css.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIBCSS_INCLUDE_CSS_H
#define LIBCSS_INCLUDE_CSS_H

#include "css/common.h"
#include "css/types.h"
#include "css/selector.h"
#include "css/keywords.h"
#include "css/style_value.h"
#include "css/style_decl.h"
#include "css/data_types.h"
#include "css/computed.h"
#include "css/properties.h"
#include "css/library.h"
#include "css/parser.h"
#include "css/utils.h"
#include "css/value.h"

LIBCSS_PUBLIC void css_init(void);
LIBCSS_PUBLIC void css_destroy(void);

#endif
