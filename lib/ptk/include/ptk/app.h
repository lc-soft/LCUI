/*
 * lib/ptk/include/ptk/app.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef PTK_INCLUDE_PLATFORM_APP_H
#define PTK_INCLUDE_PLATFORM_APP_H

#include "ptk/types.h"
#include "ptk/common.h"

PTK_BEGIN_DECLS

PTK_PUBLIC int ptk_open_uri(const char *uri);

PTK_PUBLIC int ptk_app_init(const wchar_t *name);
PTK_PUBLIC int ptk_app_destroy(void);
PTK_PUBLIC void ptk_set_instance(void *instance);
PTK_PUBLIC ptk_app_id_t ptk_get_app_id(void);
PTK_PUBLIC void ptk_app_present(void);
PTK_PUBLIC int ptk_init(const wchar_t *name);
PTK_PUBLIC int ptk_run(void);
PTK_PUBLIC void ptk_app_exit(int exit_code);
PTK_PUBLIC void ptk_destroy(void);

PTK_END_DECLS

#endif
