/*
 * lib/i18n/src/yaml.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <yutil.h>
#include "i18n-private.h"

#ifdef LIBI18N_HAS_YAML

#include <yaml.h>

static wchar_t *yaml_token_getwcs(yaml_token_t *token)
{
        char *str = (char *)token->data.scalar.value;
        size_t len = token->data.scalar.length;
        wchar_t *wcs = malloc(sizeof(wchar_t) * (len + 1));
        len = decode_utf8(wcs, str, len);
        if (len < 1) {
                abort();
        }
        wcs[len] = 0;
        return wcs;
}

static char *load_file_to_buffer(const char *path, size_t *size)
{
        char *buffer, *p;
        size_t buffer_size = 0, read_size = 256;
        FILE *file = fopen(path, "rb");
        if (!file) {
                return NULL;
        }
        buffer = NULL;
        while (!feof(file)) {
                p = realloc(buffer, buffer_size + sizeof(char) * 256);
                if (!p) {
                        free(buffer);
                        fclose(file);
                        return NULL;
                }
                buffer = p;
                p = buffer + buffer_size;
                read_size = fread(p, sizeof(char), 256, file);
                buffer_size += read_size;
        };
        *size = buffer_size;
        buffer[buffer_size] = 0;
        fclose(file);
        return buffer;
}

dict_t *i18n_load_yaml_file(const char *path)
{
        size_t size;
        char *buffer;
        yaml_token_t token;
        yaml_parser_t parser;
        dict_t *dict, *parent_dict;
        dict_value_t *value, *parent_value;
        int state = 0;

        logger_debug("[i18n] load language file: %s\n", path);
        buffer = load_file_to_buffer(path, &size);
        if (buffer == NULL) {
                logger_error("[i18n] failed to open file: %s\n", path);
                return NULL;
        }
        if (!yaml_parser_initialize(&parser)) {
		free(buffer);
                logger_error("[i18n] failed to initialize parser!\n");
                return NULL;
        }
        parent_value = value = NULL;
        parent_dict = dict = i18n_dict_create();
        yaml_parser_set_input_string(&parser, (unsigned char *)buffer, size);
        do {
                if (!yaml_parser_scan(&parser, &token)) {
                        logger_error("[i18n] error: %s\n", parser.problem);
                        dict_destroy(dict);
                        dict = NULL;
                        break;
                }
                switch (token.type) {
                case YAML_KEY_TOKEN:
                        state = 0;
                        break;
                case YAML_VALUE_TOKEN:
                        state = 1;
                        break;
                case YAML_BLOCK_MAPPING_START_TOKEN:
                        if (!value) {
                                break;
                        }
                        value->type = DICT;
                        value->dict = i18n_dict_create();
                        value->parent_value = parent_value;
                        parent_dict = value->dict;
                        parent_value = value;
                        break;
                case YAML_BLOCK_END_TOKEN:
                        if (parent_value) {
                                parent_value = parent_value->parent_value;
                                if (parent_value) {
                                        parent_dict = parent_value->dict;
                                } else {
                                        parent_dict = dict;
                                }
                        }
                        break;
                case YAML_SCALAR_TOKEN:
                        if (state == 0) {
                                value = malloc(sizeof(dict_value_t));
                                value->type = NONE;
                                value->parent_dict = parent_dict;
                                value->parent_value = parent_value;
                                value->key = yaml_token_getwcs(&token);
                                dict_add(parent_dict, value->key, value);
                                break;
                        }
                        value->type = STRING;
                        value->string.data = yaml_token_getwcs(&token);
                        value->string.length = wcslen(value->string.data);
                        break;
                default:
                        break;
                }
                if (token.type != YAML_STREAM_END_TOKEN) {
                        yaml_token_delete(&token);
                }
        } while (token.type != YAML_STREAM_END_TOKEN);
        yaml_token_delete(&token);
        yaml_parser_delete(&parser);
        free(buffer);
        return dict;
}

#endif
