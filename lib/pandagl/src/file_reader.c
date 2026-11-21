/*
 * lib/pandagl/src/file_reader.c
 *
 * Copyright (c) 2024-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pandagl/file_reader.h>

typedef struct pd_memory_stream {
        unsigned char *data;
        size_t size;
        size_t pos;
} pd_memory_stream_t;

static size_t pd_file_stream_on_read(void *data, void *buffer, size_t size)
{
        return fread(buffer, 1, size, data);
}

static void pd_file_stream_on_skip(void *data, long offset)
{
        fseek(data, offset, SEEK_CUR);
}

static void pd_file_stream_on_rewind(void *data)
{
        rewind(data);
}

static void pd_file_stream_on_close(void *data)
{
        fclose(data);
}

static size_t pd_memory_stream_on_read(void *data, void *buffer, size_t size)
{
        pd_memory_stream_t *stream = data;
        size_t count = stream->size - stream->pos;

        if (count > size) {
                count = size;
        }
        memcpy(buffer, stream->data + stream->pos, count);
        stream->pos += count;
        return count;
}

static void pd_memory_stream_on_skip(void *data, long offset)
{
        pd_memory_stream_t *stream = data;
        size_t pos = stream->pos;

        if (offset < 0) {
                size_t count = (size_t)(-offset);

                stream->pos = count > pos ? 0 : pos - count;
        } else {
                size_t count = (size_t)offset;

                stream->pos =
                    count > stream->size - pos ? stream->size : pos + count;
        }
}

static void pd_memory_stream_on_rewind(void *data)
{
        pd_memory_stream_t *stream = data;

        stream->pos = 0;
}

static void pd_memory_stream_on_close(void *data)
{
        pd_memory_stream_t *stream = data;

        free(stream->data);
        free(stream);
}

pd_file_reader_t *pd_file_reader_create(void)
{
        return calloc(1, sizeof(pd_file_reader_t));
}

pd_file_reader_t *pd_file_reader_create_from_memory(const void *data,
                                                    size_t size)
{
        pd_file_reader_t *reader;
        pd_memory_stream_t *stream;

        if (!data || size == 0) {
                return NULL;
        }
        stream = calloc(1, sizeof(pd_memory_stream_t));
        if (!stream) {
                return NULL;
        }
        stream->data = malloc(size);
        if (!stream->data) {
                free(stream);
                return NULL;
        }
        memcpy(stream->data, data, size);
        stream->size = size;

        reader = pd_file_reader_create();
        if (!reader) {
                free(stream->data);
                free(stream);
                return NULL;
        }
        reader->stream_data = stream;
        reader->fn_read = pd_memory_stream_on_read;
        reader->fn_skip = pd_memory_stream_on_skip;
        reader->fn_rewind = pd_memory_stream_on_rewind;
        reader->fn_close = pd_memory_stream_on_close;
        return reader;
}

pd_file_reader_t *pd_file_reader_create_from_file(const char *filename)
{
        FILE *fp;
        pd_file_reader_t *reader;

        fp = fopen(filename, "rb");
        if (fp == NULL) {
                return NULL;
        }
        reader = pd_file_reader_create();
        reader->stream_data = fp;
        reader->fn_skip = pd_file_stream_on_skip;
        reader->fn_read = pd_file_stream_on_read;
        reader->fn_rewind = pd_file_stream_on_rewind;
        reader->fn_close = pd_file_stream_on_close;
        return reader;
}

void pd_file_reader_destroy(pd_file_reader_t *reader)
{
        reader->fn_close(reader->stream_data);
        free(reader);
}
