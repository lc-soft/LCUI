/*
 * anchor.c -- The anchor widget, used to link view resource, its function is
 * similar to <a> element in HTML.
 *
 * Copyright (c) 2018-2022, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ui_xml.h>
#include <ui_widgets/anchor.h>
#include <worker.h>
#include <platform.h>

typedef struct xml_loader_t {
        /** 键，作为在视图加载完后传给事件处理器的额外参数 */
        char* key;
        char* filepath;      /**< 视图文件路径 */
        char* target_id;     /**< 目标容器部件的标识 */
        ui_widget_t* pack;   /**< 已经加载的视图内容包 */
        ui_widget_t* widget; /**< 触发视图加载器的部件 */
} xml_loader_t;

static struct ui_anchor_module_t {
        ui_widget_prototype_t* proto;
        worker_t * worker;
} ui_anchor;

static void xml_loader_on_widget_destroy(ui_widget_t* w, ui_event_t* e,
                                         void* arg)
{
        xml_loader_t* loader = e->data;

        loader->widget = NULL;
}

static void xml_loader_destroy(xml_loader_t* loader)
{
        if (loader->widget) {
                ui_widget_off(loader->widget, "destroy",
                              xml_loader_on_widget_destroy, NULL);
        }
        if (loader->key) {
                free(loader->key);
        }
        loader->key = NULL;
        loader->pack = NULL;
        loader->widget = NULL;
        free(loader->target_id);
        free(loader->filepath);
        free(loader);
}

static xml_loader_t* xml_loader_create(ui_widget_t* w)
{
        xml_loader_t* loader;
        const char* key = ui_widget_get_attr(w, "key");

        loader = malloc(sizeof(xml_loader_t));
        if (!loader) {
                return NULL;
        }
        loader->widget = w;
        loader->filepath = strdup2(ui_widget_get_attr(w, "href"));
        loader->target_id = strdup2(ui_widget_get_attr(w, "target"));
        ui_widget_on(w, "destroy", xml_loader_on_widget_destroy, loader);
        if (key) {
                loader->key = strdup2(key);
        } else {
                loader->key = NULL;
        }
        return loader;
}

static void ui_anchor_on_load(ui_widget_t* w, ui_event_t* e, void* arg)
{
        xml_loader_t* loader = arg;
        ui_widget_t *target, *root;
        ui_event_t ev = { 0 };

        target = ui_get_widget(loader->target_id);
        if (!target) {
                return;
        }
        root = ui_root();
        ui_widget_append(target, loader->pack);
        ui_widget_unwrap(loader->pack);
        ui_event_init(&ev, "loaded.anchor");
        ev.cancel_bubble = TRUE;
        ev.target = loader->widget;
        ui_widget_emit_event(root, ev, loader->key);
}

static void xml_loader_load(xml_loader_t* loader)
{
        ui_widget_t* pack;
        ui_event_t e;
        char *path, dirname[] = "assets/views/";

        if (loader->filepath[0] != '/') {
                path = malloc((strlen(loader->filepath) + 1) * sizeof(char) +
                              sizeof(dirname));
                if (!path) {
                        logger_error("[anchor] out of memory\n");
                        xml_loader_destroy(loader);
                        return;
                }
                strcpy(path, dirname);
                strcat(path, loader->filepath);
                pack = ui_load_xml_file(path);
                free(path);
        } else {
                pack = ui_load_xml_file(loader->filepath);
        }
        if (!pack) {
                logger_error("[anchor] href (%s): cannot load xml resource\n",
                             loader->filepath);
                xml_loader_destroy(loader);
                return;
        }
        ui_event_init(&e, "load.anchor");
        e.target = loader->widget;
        e.cancel_bubble = TRUE;
        loader->pack = pack;
        ui_post_event(&e, loader,
                      (ui_event_arg_destructor_t)xml_loader_destroy);
}

static void ui_anchor_on_startload(ui_widget_t* w, ui_event_t* e, void* arg)
{
        ui_widget_t* target;
        xml_loader_t* loader = arg;
        worker_task_t task = { 0 };

        target = ui_get_widget(loader->target_id);
        if (!target) {
                logger_error("[anchor] target (%s): not found\n",
                             loader->target_id);
                xml_loader_destroy(loader);
                return;
        }
        ui_widget_empty(target);
        task.arg[0] = loader;
        task.callback = (worker_callback_t)xml_loader_load;
        worker_post_task(ui_anchor.worker, &task);
}

void ui_anchor_open(ui_widget_t* w)
{
        ui_event_t e;
        xml_loader_t* loader;
        const char* attr_href = ui_widget_get_attr(w, "href");

        if (!attr_href) {
                logger_error("[anchor] href are required\n");
                return;
        }
        if (strstr(attr_href, "file:") == attr_href) {
                open_uri(attr_href + 5);
                return;
        }
        if (strstr(attr_href, "http://") == attr_href ||
            strstr(attr_href, "https://") == attr_href) {
                open_uri(attr_href);
                return;
        }
        loader = xml_loader_create(w);
        if (!loader) {
                logger_error("[anchor] out of memory\n");
                return;
        }
        ui_event_init(&e, "startload.anchor");
        e.cancel_bubble = TRUE;
        ui_post_event(&e, loader, NULL);
}

static void ui_anchor_on_click(ui_widget_t* w, ui_event_t* e, void* arg)
{
        worker_task_t task = { 0 };

        task.callback = (worker_callback_t)ui_anchor_open;
        task.arg[0] = w;
        worker_post_task(ui_anchor.worker, &task);
}

static void ui_anchor_on_init(ui_widget_t* w)
{
        ui_widget_on(w, "click", ui_anchor_on_click, NULL);
        ui_widget_on(w, "startload.anchor", ui_anchor_on_startload, NULL);
        ui_widget_on(w, "load.anchor", ui_anchor_on_load, NULL);
        ui_anchor.proto->proto->init(w);
}

void ui_register_anchor(void)
{
        ui_anchor.proto = ui_create_widget_prototype("a", "text");
        ui_anchor.proto->init = ui_anchor_on_init;
        ui_anchor.worker = worker_create();
        worker_run_async(ui_anchor.worker);
}

void ui_unregister_anchor(void)
{
        worker_destroy(ui_anchor.worker);
        ui_anchor.worker = NULL;
}
