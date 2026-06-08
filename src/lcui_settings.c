/*
 * src/lcui_settings.c: -- global settings.
 *
 * Copyright (c) 2020, James Duong <duong.james@gmail.com>
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io>
 * All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <strings.h>
#endif
#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif
#include <ui.h>
#include <ui_server.h>
#include <LCUI/settings.h>
#include <LCUI/app.h>

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

typedef struct lcui_store_rendering {
        int fps_cap;
        int parallel_threads;
        bool paint_flashing;
} lcui_store_rendering_t;

static struct lcui_settings_module {
        bool loaded;
        bool dirty;
        char *path;
        lcui_store_rendering_t rendering;
} lcui_settings;

static const lcui_store_rendering_t lcui_default_rendering_settings = {
        LCUI_DEFAULT_FPS_CAP, LCUI_DEFAULT_PARALLEL_THREADS, false
};

static char *lcui_strdup(const char *str)
{
        size_t len;
        char *copy;

        if (!str) {
                return NULL;
        }
        len = strlen(str);
        copy = malloc(sizeof(char) * (len + 1));
        if (!copy) {
                return NULL;
        }
        strcpy(copy, str);
        return copy;
}

static int lcui_mkdir(const char *path)
{
#ifdef _WIN32
        return _mkdir(path);
#else
        return mkdir(path, 0700);
#endif
}

static bool lcui_mkdir_recursive(const char *path)
{
        size_t i, len;
        char ch;
        char *buffer;

        if (!path || path[0] == 0) {
                return false;
        }
        len = strlen(path);
        buffer = lcui_strdup(path);
        if (!buffer) {
                return false;
        }
        for (i = 1; i <= len; ++i) {
                if (buffer[i] != '/' && buffer[i] != '\\' && buffer[i] != 0) {
                        continue;
                }
                ch = buffer[i];
                if (ch != 0) {
                        buffer[i] = 0;
                }
                if (buffer[0] == 0) {
                        if (ch != 0) {
                                buffer[i] = ch;
                        }
                        continue;
                }
#ifdef _WIN32
                if (isalpha((unsigned char)buffer[0]) && buffer[1] == ':' &&
                    buffer[2] == 0) {
                        if (ch != 0) {
                                buffer[i] = ch;
                        }
                        continue;
                }
#endif
                if (lcui_mkdir(buffer) != 0 && errno != EEXIST) {
                        free(buffer);
                        return false;
                }
                if (ch != 0) {
                        buffer[i] = ch;
                }
        }
        free(buffer);
        return true;
}

static char *lcui_join_path(const char *left, const char *right)
{
        size_t left_len, right_len;
        char *path;

        if (!left || !right) {
                return NULL;
        }
        left_len = strlen(left);
        right_len = strlen(right);
        path = malloc(sizeof(char) * (left_len + right_len + 2));
        if (!path) {
                return NULL;
        }
        strcpy(path, left);
        if (left_len > 0 && path[left_len - 1] != '/' &&
            path[left_len - 1] != '\\') {
                path[left_len] = '/';
                path[left_len + 1] = 0;
        }
        strcat(path, right);
        return path;
}

static char *lcui_settings_get_config_root(void)
{
#ifdef _WIN32
        const char *base = getenv("APPDATA");
        if (base && base[0] != 0) {
                return lcui_strdup(base);
        }
        base = getenv("USERPROFILE");
        if (base && base[0] != 0) {
                return lcui_join_path(base, "AppData/Roaming");
        }
#elif defined(__APPLE__)
        const char *base = getenv("HOME");
        if (base && base[0] != 0) {
                return lcui_join_path(base, "Library/Application Support");
        }
#else
        const char *base = getenv("XDG_CONFIG_HOME");
        if (base && base[0] != 0) {
                return lcui_strdup(base);
        }
        base = getenv("HOME");
        if (base && base[0] != 0) {
                return lcui_join_path(base, ".config");
        }
#endif
        return NULL;
}

static bool lcui_settings_resolve_path(char **out_path)
{
        char *config_root, *app_dir, *settings_path;
        const char *app_id = lcui_get_app_id();

        if (!app_id || app_id[0] == 0) {
                return false;
        }
        config_root = lcui_settings_get_config_root();
        if (!config_root) {
                return false;
        }
        app_dir = lcui_join_path(config_root, app_id);
        free(config_root);
        if (!app_dir) {
                return false;
        }
        if (!lcui_mkdir_recursive(app_dir)) {
                free(app_dir);
                return false;
        }
        settings_path = lcui_join_path(app_dir, "settings.ini");
        free(app_dir);
        if (!settings_path) {
                return false;
        }
        *out_path = settings_path;
        return true;
}

static char *lcui_store_trim(char *str)
{
        size_t len;

        while (*str && isspace((unsigned char)*str)) {
                ++str;
        }
        len = strlen(str);
        while (len > 0 && isspace((unsigned char)str[len - 1])) {
                str[--len] = 0;
        }
        return str;
}

static bool lcui_store_parse_int(const char *str, int *out)
{
        long value;
        char *endptr;

        if (!str || !out) {
                return false;
        }
        errno = 0;
        value = strtol(str, &endptr, 10);
        if (errno != 0 || endptr == str || *endptr != 0 || value > INT_MAX ||
            value < INT_MIN) {
                return false;
        }
        *out = (int)value;
        return true;
}

static bool lcui_store_parse_bool(const char *str, bool *out)
{
        if (!str || !out) {
                return false;
        }
        if (strcmp(str, "1") == 0 || strcasecmp(str, "true") == 0) {
                *out = true;
                return true;
        }
        if (strcmp(str, "0") == 0 || strcasecmp(str, "false") == 0) {
                *out = false;
                return true;
        }
        return false;
}

static bool lcui_store_load(const char *path, lcui_store_rendering_t *settings,
                            bool *dirty, bool *exists)
{
        int value;
        bool flag;
        bool has_version = false;
        char line[1024];
        char section[64] = { 0 };
        FILE *fp = fopen(path, "r");

        if (!fp) {
                if (errno == ENOENT) {
                        *exists = false;
                        return true;
                }
                return false;
        }
        *exists = true;
        while (fgets(line, sizeof(line), fp)) {
                char *data = lcui_store_trim(line);
                char *equal;

                if (data[0] == 0 || data[0] == ';' || data[0] == '#') {
                        continue;
                }
                if (data[0] == '[') {
                        char *end = strchr(data, ']');
                        if (!end) {
                                continue;
                        }
                        *end = 0;
                        snprintf(section, sizeof(section), "%s", data + 1);
                        continue;
                }
                equal = strchr(data, '=');
                if (!equal) {
                        continue;
                }
                *equal++ = 0;
                data = lcui_store_trim(data);
                equal = lcui_store_trim(equal);
                if (strcasecmp(section, "meta") == 0 &&
                    strcasecmp(data, "version") == 0) {
                        has_version = true;
                        if (!lcui_store_parse_int(equal, &value) ||
                            value != 1) {
                                *dirty = true;
                        }
                        continue;
                }
                if (strcasecmp(section, "rendering") != 0) {
                        continue;
                }
                if (strcasecmp(data, "fps_cap") == 0) {
                        if (lcui_store_parse_int(equal, &value)) {
                                settings->fps_cap = value;
                        } else {
                                *dirty = true;
                        }
                        continue;
                }
                if (strcasecmp(data, "parallel_threads") == 0) {
                        if (lcui_store_parse_int(equal, &value)) {
                                settings->parallel_threads = value;
                        } else {
                                *dirty = true;
                        }
                        continue;
                }
                if (strcasecmp(data, "paint_flashing") == 0) {
                        if (lcui_store_parse_bool(equal, &flag)) {
                                settings->paint_flashing = flag;
                        } else {
                                *dirty = true;
                        }
                }
        }
        fclose(fp);
        if (!has_version) {
                *dirty = true;
        }
        return true;
}

static bool lcui_store_save(const char *path,
                            const lcui_store_rendering_t *settings)
{
        size_t path_len;
        char *tmp_path;
        FILE *fp;
        int ret;

        path_len = strlen(path);
        tmp_path = malloc(sizeof(char) * (path_len + 5));
        if (!tmp_path) {
                return false;
        }
        strcpy(tmp_path, path);
        strcat(tmp_path, ".tmp");
        fp = fopen(tmp_path, "w");
        if (!fp) {
                free(tmp_path);
                return false;
        }
        fprintf(fp, "[meta]\nversion=1\n\n");
        fprintf(fp, "[rendering]\n");
        fprintf(fp, "fps_cap=%d\n", settings->fps_cap);
        fprintf(fp, "parallel_threads=%d\n", settings->parallel_threads);
        fprintf(fp, "paint_flashing=%d\n", settings->paint_flashing ? 1 : 0);
        fclose(fp);
#ifdef _WIN32
        ret = MoveFileExA(tmp_path, path,
                          MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)
                  ? 0
                  : -1;
#else
        ret = rename(tmp_path, path);
#endif
        free(tmp_path);
        return ret == 0;
}

static bool lcui_settings_repair_rendering(lcui_store_rendering_t *settings)
{
        bool valid = true;

        if (settings->fps_cap < LCUI_RENDERING_FPS_CAP_MIN ||
            settings->fps_cap > LCUI_RENDERING_FPS_CAP_MAX) {
                settings->fps_cap = lcui_default_rendering_settings.fps_cap;
                valid = false;
        }
        if (settings->parallel_threads < LCUI_RENDERING_PARALLEL_THREADS_MIN ||
            settings->parallel_threads > LCUI_RENDERING_PARALLEL_THREADS_MAX) {
                settings->parallel_threads =
                    lcui_default_rendering_settings.parallel_threads;
                valid = false;
        }
        return valid;
}

static bool lcui_settings_validate_rendering(
    const lcui_rendering_settings_t *settings)
{
        if (!settings) {
                return false;
        }
        if (settings->fps_cap < LCUI_RENDERING_FPS_CAP_MIN ||
            settings->fps_cap > LCUI_RENDERING_FPS_CAP_MAX) {
                return false;
        }
        if (settings->parallel_threads < LCUI_RENDERING_PARALLEL_THREADS_MIN ||
            settings->parallel_threads > LCUI_RENDERING_PARALLEL_THREADS_MAX) {
                return false;
        }
        return true;
}

static void lcui_settings_apply_rendering(void)
{
        ui_server_set_threads(lcui_settings.rendering.parallel_threads);
        ui_server_set_paint_flashing_enabled(
            lcui_settings.rendering.paint_flashing);
        lcui_set_fps_cap((unsigned)lcui_settings.rendering.fps_cap);
}

bool lcui_settings_load(void)
{
        bool exists = false;
        char *path;

        if (lcui_settings.loaded) {
                return true;
        }
        if (!lcui_settings_resolve_path(&path)) {
                return false;
        }
        lcui_settings.rendering = lcui_default_rendering_settings;
        lcui_settings.dirty = false;
        if (!lcui_store_load(path, &lcui_settings.rendering,
                             &lcui_settings.dirty, &exists)) {
                free(path);
                return false;
        }
        if (!exists ||
            !lcui_settings_repair_rendering(&lcui_settings.rendering)) {
                lcui_settings.dirty = true;
        }
        lcui_settings.loaded = true;
        lcui_settings.path = path;
        lcui_settings_apply_rendering();
        return true;
}

bool lcui_settings_flush(void)
{
        if (!lcui_settings.loaded || !lcui_settings.path) {
                return false;
        }
        if (!lcui_settings.dirty) {
                return true;
        }
        if (!lcui_store_save(lcui_settings.path, &lcui_settings.rendering)) {
                return false;
        }
        lcui_settings.dirty = false;
        return true;
}

void lcui_settings_unload(void)
{
        free(lcui_settings.path);
        lcui_settings.path = NULL;
        lcui_settings.loaded = false;
        lcui_settings.dirty = false;
        lcui_settings.rendering = lcui_default_rendering_settings;
}

bool lcui_settings_get_rendering(lcui_rendering_settings_t *out)
{
        if (!lcui_settings.loaded || !out) {
                return false;
        }
        out->fps_cap = lcui_settings.rendering.fps_cap;
        out->parallel_threads = lcui_settings.rendering.parallel_threads;
        out->paint_flashing = lcui_settings.rendering.paint_flashing;
        return true;
}

bool lcui_settings_set_rendering(const lcui_rendering_settings_t *in)
{
        if (!lcui_settings.loaded || !in ||
            !lcui_settings_validate_rendering(in)) {
                return false;
        }
        if (lcui_settings.rendering.fps_cap == in->fps_cap &&
            lcui_settings.rendering.parallel_threads == in->parallel_threads &&
            lcui_settings.rendering.paint_flashing == in->paint_flashing) {
                return true;
        }
        lcui_settings.rendering.fps_cap = in->fps_cap;
        lcui_settings.rendering.parallel_threads = in->parallel_threads;
        lcui_settings.rendering.paint_flashing = in->paint_flashing;
        lcui_settings_apply_rendering();
        lcui_settings.dirty = true;
        return true;
}

/* Initialize settings with the current global settings. */
void lcui_get_settings(lcui_settings_t *settings)
{
        if (!settings) {
                return;
        }
        settings->fps_cap = lcui_settings.rendering.fps_cap;
        settings->parallel_rendering_threads =
            lcui_settings.rendering.parallel_threads;
        settings->paint_flashing = lcui_settings.rendering.paint_flashing;
}

/* Update global settings with the given input. */
void lcui_apply_settings(lcui_settings_t *settings)
{
        lcui_rendering_settings_t rendering;

        if (!settings) {
                return;
        }
        rendering.fps_cap = settings->fps_cap;
        rendering.parallel_threads = settings->parallel_rendering_threads;
        rendering.paint_flashing = settings->paint_flashing;
        if (!lcui_settings_validate_rendering(&rendering)) {
                lcui_store_rendering_t repaired = { rendering.fps_cap,
                                                    rendering.parallel_threads,
                                                    rendering.paint_flashing };
                lcui_settings_repair_rendering(&repaired);
                rendering.fps_cap = repaired.fps_cap;
                rendering.parallel_threads = repaired.parallel_threads;
        }
        if (lcui_settings.loaded) {
                (void)lcui_settings_set_rendering(&rendering);
                return;
        }
        lcui_settings.rendering.fps_cap = rendering.fps_cap;
        lcui_settings.rendering.parallel_threads = rendering.parallel_threads;
        lcui_settings.rendering.paint_flashing = rendering.paint_flashing;
        lcui_settings_apply_rendering();
}

/* Reset global settings to their defaults. */
void lcui_reset_settings(void)
{
        lcui_rendering_settings_t settings = {
                lcui_default_rendering_settings.fps_cap,
                lcui_default_rendering_settings.parallel_threads,
                lcui_default_rendering_settings.paint_flashing
        };

        if (lcui_settings.loaded) {
                (void)lcui_settings_set_rendering(&settings);
                return;
        }
        lcui_settings.rendering = lcui_default_rendering_settings;
        lcui_settings_apply_rendering();
}
