/*
 * lib/ui/src/ui_events.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

void ui_widget_destroy_listeners(ui_widget_t *w);

/** 初始化 LCUI 部件的事件系统 */
void ui_init_events(void);

/** 销毁（释放） LCUI 部件的事件系统的相关资源 */
void ui_destroy_events(void);
