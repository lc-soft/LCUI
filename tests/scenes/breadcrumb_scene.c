/*
 * tests/scenes/breadcrumb_scene.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <stdio.h>
#include "breadcrumb_scene.h"

static const char *breadcrumb_scene_css = ".bc-panel {\
  display: block;\
  padding: 16px;\
  background: #ffffff;\
}\
.bc-row {\
  display: block;\
  width: 100%;\
  margin-bottom: 18px;\
}\
.bc-narrow {\
  width: 360px;\
}\
.bc-tiny {\
  width: 200px;\
}\
.bc-doc {\
  width: 320px;\
}\
.bc-caption {\
  display: block;\
  margin-bottom: 6px;\
  color: #666666;\
  font-size: 12px;\
}";

static const char *breadcrumb_scene_label(const char *segment, const char *path,
                                          void *arg)
{
        static const char *labels[] = {
                "Workspace",
                "Docs",
                "Components",
                "Breadcrumb",
        };
        static char buffer[128];
        size_t index = (size_t)(path[0] ? strlen(path) : 0);

        snprintf(buffer, sizeof(buffer), "%s (%s)", labels[index % 4], segment);
        return buffer;
}

static ui_widget_t *make_caption(const char *text)
{
        ui_widget_t *caption = ui_create_widget("text");

        ui_widget_add_class(caption, "bc-caption");
        ui_text_set_content(caption, text);
        return caption;
}

static ui_widget_t *make_row(const char *caption)
{
        ui_widget_t *row = ui_create_widget(NULL);

        ui_widget_add_class(row, "bc-row");
        if (caption) {
                ui_widget_append(row, make_caption(caption));
        }
        return row;
}

static ui_widget_t *make_custom_button(void)
{
        ui_widget_t *button = ui_create_button();
        ui_widget_t *label = ui_create_widget("text");

        ui_widget_set_attr(button, "data-key", "settings");
        ui_widget_set_attr(button, "data-label", "Settings");
        ui_text_set_content(label, "Settings");
        ui_widget_append(button, label);
        return button;
}

static ui_widget_t *make_explicit_breadcrumb(void)
{
        ui_widget_t *breadcrumb = ui_create_breadcrumb();
        ui_widget_t *item;
        ui_widget_t *link;
        ui_widget_t *page;
        ui_widget_t *text;
        const char *paths[] = { "/root", "/root/docs",
                                "/root/docs/breadcrumb" };
        size_t i;

        for (i = 0; i < 3; ++i) {
                item = ui_create_breadcrumb_item();
                ui_widget_set_attr(item, "data-path", paths[i]);
                text = ui_create_widget("text");
                ui_text_set_content(text, paths[i]);
                if (i + 1 < 3) {
                        link = ui_create_breadcrumb_link();
                        ui_widget_append(link, text);
                        ui_widget_append(item, link);
                } else {
                        page = ui_create_breadcrumb_page();
                        ui_widget_append(page, text);
                        ui_widget_append(item, page);
                }
                ui_widget_append(breadcrumb, item);
        }
        return breadcrumb;
}

void breadcrumb_scene_build(breadcrumb_scene_t *scene)
{
        ui_widget_t *panel;
        ui_widget_t *row;
        ui_widget_t *breadcrumb;

        ui_widget_resize(ui_root(), 760, 500);
        ui_load_css_string(breadcrumb_scene_css, __FILE__);

        panel = ui_create_widget(NULL);
        ui_widget_add_class(panel, "bc-panel");
        ui_widget_append(ui_root(), panel);

        row = make_row("Path mode, full width");
        scene->wide = ui_create_breadcrumb();
        ui_breadcrumb_build_from_path(scene->wide, "/workspace/docs/components",
                                      NULL, NULL);
        ui_widget_append(row, scene->wide);
        ui_widget_append(panel, row);

        row = make_row("Path mode, 360px wide, labels from provider");
        scene->narrow = ui_create_breadcrumb();
        ui_widget_add_class(scene->narrow, "bc-narrow");
        ui_breadcrumb_build_from_path(scene->narrow,
                                      "/workspace/docs/components",
                                      breadcrumb_scene_label, NULL);
        ui_widget_append(row, scene->narrow);
        ui_widget_append(panel, row);

        row = make_row("Path mode, 320px wide, auto collapse");
        ui_widget_add_class(row, "bc-narrow");
        breadcrumb = ui_create_breadcrumb();
        ui_widget_add_class(breadcrumb, "bc-doc");
        ui_breadcrumb_build_from_path(
            breadcrumb,
            "/workspace/projects/lcui/components/breadcrumb/navigation", NULL,
            NULL);
        ui_widget_append(row, breadcrumb);
        ui_widget_append(panel, row);

        row = make_row("Append mode with a custom button item");
        scene->custom = ui_create_breadcrumb();
        ui_breadcrumb_append_item(scene->custom, "workspace", "Workspace");
        ui_breadcrumb_append_item(scene->custom, "docs", "Docs");
        ui_breadcrumb_append(scene->custom, make_custom_button());
        ui_widget_append(row, scene->custom);
        ui_widget_append(panel, row);

        row = make_row("Explicit mode, nodes identified by data-path");
        scene->explicit_mode = make_explicit_breadcrumb();
        ui_widget_append(row, scene->explicit_mode);
        ui_widget_append(panel, row);

        row = make_row(
            "Tiny 200px container, everything before the last item collapses");
        ui_widget_add_class(row, "bc-narrow");
        breadcrumb = ui_create_breadcrumb();
        ui_widget_add_class(breadcrumb, "bc-tiny");
        ui_breadcrumb_build_from_path(breadcrumb, "/a/b/c/d/e/f",
                                      breadcrumb_scene_label, NULL);
        ui_widget_append(row, breadcrumb);
        ui_widget_append(panel, row);
}
