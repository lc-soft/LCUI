/* dirent.c -- Directory entry operation set.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn>
 * Copyright (c) 2021, Li Zihao <yidianyiko@foxmail.com>
 * All rights reserved.
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
#include <errno.h>
#include <locale.h>
#include <wchar.h>
#ifndef _WIN32
#include <dirent.h>
#include "yutil/keywords.h"
#include "yutil/dirent.h"
#include "yutil/charset.h"

#define DIRENT_MAX_LEN 4096

typedef DIR *dir_handle_t;

struct dir_entry_t {
	struct dirent dirent;
	wchar_t name[DIRENT_MAX_LEN];
};
struct dir_t {
	dir_handle_t handle;
	dir_entry_t entry;
	int cached;
};

dir_t *dir_create()
{
	dir_t *dir = (dir_t *)malloc(sizeof(dir_t));
	dir_entry_t t = { 0 };
	dir->cached = 0;
	dir->entry = t;
	dir->handle = NULL;
	return dir;
}

void dir_destroy(dir_t *dir)
{
	free(dir);
}

int dir_open_a(const char *path, dir_t *dir)
{
	dir->handle = opendir(path);
	if (!dir->handle) {
		return -1;
	}
	return 0;
}

int dir_open_w(const wchar_t *path, dir_t *dir)
{
	int len;
	char *newpath;

	len = encode_string(NULL, path, 0, ENCODING_UTF8) + 1;
	newpath = malloc(len * sizeof(wchar_t));
	encode_string(newpath, path, len, ENCODING_UTF8);

	dir->handle = opendir(newpath);
	free(newpath);
	if (!dir->handle) {
		return -1;
	}
	return 0;
}

dir_entry_t *dir_read_a(dir_t *dir)
{
	int len;
	struct dirent *d;
	d = readdir(dir->handle);
	if (!d) {
		return NULL;
	}
	dir->entry.dirent = *d;
	len = sizeof(d->d_name);
	if (len >= DIRENT_MAX_LEN) {
		return NULL;
	}
	dir->entry.name[len] = 0;
	return &dir->entry;
}

dir_entry_t *dir_read_w(dir_t *dir)
{
	int len;
	struct dirent *d;
	d = readdir(dir->handle);
	if (!d) {
		return NULL;
	}
	dir->entry.dirent = *d;
	len = decode_string(dir->entry.name, d->d_name, DIRENT_MAX_LEN,
			    ENCODING_UTF8);
	if (len >= DIRENT_MAX_LEN) {
		return NULL;
	}
	dir->entry.name[len] = 0;
	return &dir->entry;
}

int dir_close(dir_t *dir)
{
	return closedir(dir->handle);
}

char *dir_get_file_name_a(dir_entry_t *entry)
{
	return entry->dirent.d_name;
}

wchar_t *dir_get_file_name_w(dir_entry_t *entry)
{
	return entry->name;
}

int dir_entry_is_directory(dir_entry_t *entry)
{
	return entry->dirent.d_type == DT_DIR;
}

int dir_entry_is_regular(dir_entry_t *entry)
{
	return entry->dirent.d_type == DT_REG;
}
#endif