/* dirent_win32.c -- Directory entry operation set for windows.
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
#ifdef _WIN32
#include <windows.h>

#include "yutil/keywords.h"
#include "yutil/dirent.h"

typedef HANDLE dir_handle_t;

union dir_entry_t {
	WIN32_FIND_DATAA data_a;
	WIN32_FIND_DATAW data_w;
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
	size_t len;
	char *newpath;
	char name[] = "\\*";

	len = strlen(path) + 1;
	newpath = (char *)malloc(len * sizeof(char) + sizeof(name));
	if (newpath == NULL) {
		return -ENOMEM;
	}
	strcpy(newpath, path);
	strcpy(newpath + len - 1, name);
	dir->handle = FindFirstFileA(newpath, &dir->entry.data_a);
	free(newpath);
	if (dir->handle == INVALID_HANDLE_VALUE) {
		switch (GetLastError()) {
		case ERROR_FILE_NOT_FOUND:
			return -ENOENT;
		case ERROR_ACCESS_DENIED:
			return -EACCES;
		default:
			break;
		}
		return -1;
	}
	dir->cached = 1;

	return 0;
}

int dir_open_w(const wchar_t *path, dir_t *dir)
{
	size_t len;
	wchar_t *newpath;
	wchar_t name[] = L"\\*";

	len = wcslen(path) + 1;
	newpath = (wchar_t *)malloc(len * sizeof(wchar_t) + sizeof(name));
	if (!newpath) {
		return -ENOMEM;
	}
	wcscpy(newpath, path);
	wcscpy(newpath + len - 1, name);
	dir->handle = FindFirstFileW(newpath, &dir->entry.data_w);
	free(newpath);
	if (dir->handle == INVALID_HANDLE_VALUE) {
		switch (GetLastError()) {
		case ERROR_FILE_NOT_FOUND:
			return -ENOENT;
		case ERROR_ACCESS_DENIED:
			return -EACCES;
		default:
			break;
		}
		return -1;
	}
	dir->cached = 1;

	return 0;
}

dir_entry_t *dir_read_a(dir_t *dir)
{
	if (dir->handle == INVALID_HANDLE_VALUE) {
		return NULL;
	}
	if (dir->cached) {
		dir->cached = 0;
		return &dir->entry;
	}
	if (FindNextFileA(dir->handle, &dir->entry.data_a)) {
		return &dir->entry;
	}
	FindClose(dir->handle);
	dir->handle = INVALID_HANDLE_VALUE;

	return NULL;
}

dir_entry_t *dir_read_w(dir_t *dir)
{
	if (dir->handle == INVALID_HANDLE_VALUE) {
		return NULL;
	}
	if (dir->cached) {
		dir->cached = 0;
		return &dir->entry;
	}
	if (FindNextFileW(dir->handle, &dir->entry.data_w)) {
		return &dir->entry;
	}
	FindClose(dir->handle);
	dir->handle = INVALID_HANDLE_VALUE;
	return NULL;
}

int dir_close(dir_t *dir)
{
	if (!FindClose(dir->handle)) {
		return -1;
	}

	return 0;
}

char *dir_get_file_name_a(dir_entry_t *entry)
{
	return entry->data_a.cFileName;
}

wchar_t *dir_get_file_name_w(dir_entry_t *entry)
{
	return entry->data_w.cFileName;
}

int dir_entry_is_directory(dir_entry_t *entry)
{
	return entry->data_w.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

int dir_entry_is_regular(dir_entry_t *entry)
{
	return !(entry->data_w.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

#endif