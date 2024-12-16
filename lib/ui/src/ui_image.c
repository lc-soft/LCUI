/*
 * lib/ui/src/ui_image.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <yutil.h>
#include <pandagl.h>
#include <ui/image.h>

#define UI_IMAGE_PROGRESS_EVENT_INTERVAL 100

typedef struct ui_image_event_listener {
        ui_image_event_type_t type;
        void *data;
        ui_image_event_handler_t handler;
} ui_image_event_listener_t;

typedef struct ui_image_source {
        ui_image_t image;
        size_t refs_count;
        pd_image_reader_t *reader;
        /** list_t<ui_image_event_listener_t> */
        list_t listeners;
        list_node_t node;
} ui_image_source_t;

typedef struct ui_image_mutation {
        ui_image_event_type_t type;
        ui_image_source_t *src;
        list_node_t node;
} ui_image_mutation_t;

typedef struct ui_image_loader {
        int64_t progress_tick_time;
        void (*callback)(ui_image_t *);

        /** dict_t<string, ui_image_source_t*> */
        dict_t *cache;
        dict_type_t dict_type;

        /** list_t<ui_image_mutation_t*> */
        list_t mutations;

        /** list_t<ui_image_source_t*> */
        list_t images;

        bool changed;
} ui_image_loader_t;

static ui_image_loader_t ui_image_loader;

static void ui_image_force_destroy(ui_image_source_t *src)
{
        list_destroy_without_node(&src->listeners, free);
        pd_image_reader_destroy(src->reader);
        pd_canvas_destroy(&src->image.data);
        free(src->image.path);
        src->reader = NULL;
        src->refs_count = 0;
        src->image.path = NULL;
        free(src);
}

static void ui_image_loader_add_mutation(ui_image_source_t *src,
                                         ui_event_type_t type)
{
        ui_image_mutation_t *mutation;

        mutation = malloc(sizeof(ui_image_mutation_t));
        mutation->type = type;
        mutation->src = src;
        mutation->node.data = mutation;
        list_append_node(&ui_image_loader.mutations, &mutation->node);
}

static void ui_image_loader_process_mutation(ui_image_mutation_t *mutation)
{
        list_node_t *node, *next;
        ui_image_event_listener_t *listener;
        ui_image_event_t e = { .image = &mutation->src->image,
                               .type = mutation->type };

        for (node = list_get_first_node(&mutation->src->listeners); node;
             node = next) {
                next = node->next;
                listener = node->data;
                if (listener->type == mutation->type) {
                        e.image = &mutation->src->image;
                        e.data = listener->data;
                        listener->handler(&e);
                }
        }
}

ui_image_t *ui_get_image(const char *path)
{
        return dict_fetch_value(ui_image_loader.cache, path);
}

ui_image_t *ui_image_create(const char *path)
{
        ui_image_source_t *src;

        src = (ui_image_source_t *)ui_get_image(path);
        if (src) {
                src->refs_count++;
                list_unlink(&ui_image_loader.images, &src->node);
        } else {
                src = malloc(sizeof(ui_image_source_t));
                if (!src) {
                        return NULL;
                }
                src->image.error = PD_OK;
                src->refs_count = 1;
                src->image.state = UI_IMAGE_STATE_PENDING;
                src->image.path = strdup2(path);
                src->node.data = src;
                pd_canvas_init(&src->image.data);
                list_create(&src->listeners);
                dict_add(ui_image_loader.cache, src->image.path, src);
        }
        list_insert_node(&ui_image_loader.images, 0, &src->node);
        ui_image_loader.changed = true;
        ui_image_loader.progress_tick_time = get_time_ms();
        if (ui_image_loader.callback) {
                ui_image_loader.callback(&src->image);
        }
        return &src->image;
}

void ui_image_destroy(ui_image_t *image)
{
        assert(((ui_image_source_t *)image)->refs_count > 0);
        ((ui_image_source_t *)image)->refs_count--;
}

int ui_image_add_event_listener(ui_image_t *image, ui_image_event_type_t type,
                                ui_image_event_handler_t handler, void *data)
{
        ui_image_event_t ev = { .type = type, .image = image, .data = data };
        ui_image_event_listener_t *listener;

        if (image->state == UI_IMAGE_STATE_COMPLETE) {
                if ((image->error == PD_OK && type == UI_IMAGE_EVENT_LOAD) ||
                    (image->error != PD_OK && type == UI_IMAGE_EVENT_ERROR)) {
                        handler(&ev);
                }
                return 0;
        }
        listener = malloc(sizeof(ui_image_event_listener_t));
        if (listener == NULL) {
                return -1;
        }
        listener->type = type;
        listener->handler = handler;
        listener->data = data;
        list_append(&((ui_image_source_t *)image)->listeners, listener);
        return 0;
}

int ui_image_remove_event_listener(ui_image_t *image,
                                   ui_image_event_type_t type,
                                   ui_image_event_handler_t handler, void *data)
{
        list_node_t *node, *next;
        ui_image_source_t *src = (ui_image_source_t *)image;
        ui_image_event_listener_t *listener;

        for (node = list_get_first_node(&src->listeners); node;
             node = next) {
                next = node->next;
                listener = node->data;
                if (listener->type == type && listener->handler == handler &&
                    listener->data == data) {
                        list_unlink(&src->listeners, node);
                        free(listener);
                }
        }
        return -1;
}

static bool ui_image_loader_create_reader(ui_image_source_t *src)
{
        src->reader = pd_image_reader_create_from_file(src->image.path);
        if (!src->reader) {
                src->image.error = PD_ERROR_NOT_FOUND;
                return false;
        }
        src->image.error = pd_image_reader_read_header(src->reader);
        if (src->image.error != PD_OK) {
                return false;
        }
        src->image.error =
            pd_image_reader_create_buffer(src->reader, &src->image.data);
        if (src->image.error != PD_OK) {
                return false;
        }
        return true;
}

static void ui_image_loader_reject(ui_image_source_t *src)
{
        src->image.state = UI_IMAGE_STATE_COMPLETE;
        ui_image_loader_add_mutation(src, UI_IMAGE_EVENT_ERROR);
}

static void ui_image_loader_load(ui_image_source_t *src)
{
        if (src->image.state == UI_IMAGE_STATE_COMPLETE) {
                return;
        }
        if (src->image.state == UI_IMAGE_STATE_PENDING) {
                if (!ui_image_loader_create_reader(src)) {
                        ui_image_loader_reject(src);
                        return;
                }
                src->image.state = UI_IMAGE_STATE_LOADING;
                pd_image_reader_start(src->reader);
        }
        if (setjmp(*pd_image_reader_jmpbuf(src->reader))) {
                src->image.error = PD_ERROR_IMAGE_READING;
                ui_image_loader_reject(src);
                return;
        }
        while (src->image.state != UI_IMAGE_STATE_COMPLETE &&
               !ui_image_loader.changed) {
                pd_image_reader_read_row(src->reader, &src->image.data);
                src->image.progress = 100.f * src->reader->read_row_index /
                                      src->reader->header.height *
                                      (src->reader->pass + 1) /
                                      src->reader->passes;
                if (src->reader->read_row_index >= src->reader->header.height) {
                        src->reader->read_row_index = 0;
                        src->reader->pass++;
                }
                if (src->reader->pass >= src->reader->passes) {
                        pd_image_reader_finish(src->reader);
                        src->image.state = UI_IMAGE_STATE_COMPLETE;
                        src->image.progress = 100;
                        ui_image_loader_add_mutation(src, UI_IMAGE_EVENT_LOAD);
                }
        }
}

void ui_load_images(void)
{
        list_node_t *node;

        do {
                ui_image_loader.changed = false;
        for (list_each(node, &ui_image_loader.images)) {
                if (ui_image_loader.changed) {
                                break;
                }
                ui_image_loader_load(node->data);
        }
        } while (ui_image_loader.changed);
}

void ui_process_image_events(void)
{
        int64_t now;
        list_node_t *node;
        list_t mutations;
        ui_image_source_t *src;
        ui_image_mutation_t mutation = { .type = UI_IMAGE_EVENT_PROGRESS };

        now = get_time_ms();
        list_create(&mutations);
        list_concat(&mutations, &ui_image_loader.mutations);
        for (list_each(node, &mutations)) {
                ui_image_loader_process_mutation(node->data);
        }
        list_destroy_without_node(&mutations, free);
        if (now < ui_image_loader.progress_tick_time) {
                return;
        }
        ui_image_loader.progress_tick_time =
            now + UI_IMAGE_PROGRESS_EVENT_INTERVAL;
        for (list_each(node, &ui_image_loader.images)) {
                src = node->data;
                if (src->image.state == UI_IMAGE_STATE_LOADING) {
                        mutation.src = src;
                        ui_image_loader_process_mutation(&mutation);
                }
        }
}

void ui_clear_images(void)
{
        ui_image_source_t *src;
        dict_entry_t *entry;
        dict_iterator_t *iter;

        iter = dict_get_safe_iterator(ui_image_loader.cache);
        while ((entry = dict_next(iter))) {
                src = dict_get_val(entry);
                if (src->refs_count < 1) {
                        logger_debug(
                            "[ui-image-loader] free unused image: %s\n",
                            src->image.path);
                        dict_delete(ui_image_loader.cache, src->image.path);
                        list_unlink(&ui_image_loader.images, &src->node);
                        ui_image_force_destroy(src);
                }
        }
        dict_destroy_iterator(iter);
}

void ui_init_image_loader(void)
{
        dict_init_string_key_type(&ui_image_loader.dict_type);
        ui_image_loader.cache = dict_create(&ui_image_loader.dict_type, NULL);
        ui_image_loader.progress_tick_time = get_time_ms();
        list_create(&ui_image_loader.images);
}

void ui_destroy_image_loader(void)
{
        list_destroy_without_node(
            &ui_image_loader.images,
            (list_item_destructor_t)ui_image_force_destroy);
        dict_destroy(ui_image_loader.cache);
        ui_image_loader.cache = NULL;
}

void ui_set_image_loader_callback(void (*callback)(ui_image_t *))
{
        ui_image_loader.callback = callback;
}
