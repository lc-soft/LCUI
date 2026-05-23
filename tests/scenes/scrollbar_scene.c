/*
 * tests/scenes/scrollbar_scene.c
 *
 * Copyright (c) 2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include "scrollbar_scene.h"

static const char *scrollbar_scene_css = "\
.container {\
        width: 300px;\
        height: 240px;\
        padding: 10px;\
        margin: 20px auto 0 auto;\
        border: 1px solid #eee;\
}\
\
text {\
        white-space: nowrap;\
        display: inline-block;\
}\
";

static const char *scrollbar_scene_content = "\n\
/* ***************************************************************************\n\
 * test_scrollbar.c -- test scrollbar\n\
 *\n\
 * Copyright (c) 2021, Liu chao <lc-soft@live.cn> All rights reserved.\n\
 *\n\
 * Redistribution and use in source and binary forms, with or without\n\
 * modification, are permitted provided that the following conditions are met:\n\
 *\n\
 *   * Redistributions of source code must retain the above copyright notice,\n\
 *     this list of conditions and the following disclaimer.\n\
 *   * Redistributions in binary form must reproduce the above copyright\n\
 *     notice, this list of conditions and the following disclaimer in the\n\
 *     documentation and/or other materials provided with the distribution.\n\
 *   * Neither the name of LCUI nor the names of its contributors may be used\n\
 *     to endorse or promote products derived from this software without\n\
 *     specific prior written permission.\n\
 *\n\
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n\
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n\
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE\n\
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE\n\
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR\n\
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF\n\
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS\n\
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN\n\
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)\n\
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE\n\
 * POSSIBILITY OF SUCH DAMAGE.\n\
 */\n\
";

void scrollbar_scene_build(void)
{
        ui_widget_t *container = ui_create_scrollarea();
        ui_widget_t *content = ui_create_scrollarea_content();
        ui_widget_t *text = ui_create_widget("text");
        ui_widget_t *vscrollbar = ui_create_scrollbar();
        ui_widget_t *hscrollbar = ui_create_scrollbar();

        ui_widget_resize(ui_root(), 800, 640);
        ui_load_css_string(scrollbar_scene_css, __FILE__);
        ui_widget_set_id(content, "license_content");
        ui_text_set_content(text, scrollbar_scene_content);
        ui_scrollbar_set_orientation(hscrollbar, UI_SCROLLBAR_HORIZONTAL);
        ui_widget_add_class(container, "container");
        ui_widget_append(content, text);
        ui_widget_append(container, content);
        ui_widget_append(container, vscrollbar);
        ui_widget_append(container, hscrollbar);
        ui_root_append(container);
}
