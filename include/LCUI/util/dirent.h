/*
 * dirent.h -- Directory entry operation set.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
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

#ifndef LCUI_UTIL_DIRENT_H
#define LCUI_UTIL_DIRENT_H

LCUI_BEGIN_HEADER

#ifdef _WIN32
#include <Windows.h>
typedef HANDLE LCUI_DirHandle;
typedef union LCUI_DirEntry_ {
	WIN32_FIND_DATAA dataA;
	WIN32_FIND_DATAW dataW;
} LCUI_DirEntry;

#define PATH_DELIMITER '\\'
#else
#include <dirent.h>
#define LCUI_DIRENT_NAME_LEN 256

typedef DIR* LCUI_DirHandle;
typedef struct LCUI_DirEntry_ {
	struct dirent dirent;
	wchar_t name[LCUI_DIRENT_NAME_LEN];
} LCUI_DirEntry;

#define PATH_DELIMITER '/'

#endif

typedef struct LCUI_Dir_ {
	LCUI_DirHandle handle;
	LCUI_DirEntry entry;
	int cached;
} LCUI_Dir;

#if defined(_UNICODE) || !defined(_WIN32)
#define LCUI_OpenDir LCUI_OpenDirW
#define LCUI_ReadDir LCUI_ReadDirW
#define LCUI_GetFileName LCUI_GetFileNameW
#else
#define LCUI_OpenDir LCUI_OpenDirA
#define LCUI_ReadDir LCUI_ReadDirA
#define LCUI_GetFileName LCUI_GetFileNameA
#endif


LCUI_API int LCUI_OpenDirW(const wchar_t *filepath, LCUI_Dir *dir);

LCUI_API int LCUI_OpenDirA(const char *filepath, LCUI_Dir *dir);

LCUI_API LCUI_DirEntry* LCUI_ReadDirA(LCUI_Dir *dir);

LCUI_API LCUI_DirEntry* LCUI_ReadDirW(LCUI_Dir *dir);

LCUI_API int LCUI_CloseDir(LCUI_Dir *dir);

LCUI_API char *LCUI_GetFileNameA(LCUI_DirEntry *entry);

LCUI_API wchar_t *LCUI_GetFileNameW(LCUI_DirEntry *entry);

LCUI_API int LCUI_FileIsDirectory(LCUI_DirEntry *entry);

LCUI_API int LCUI_FileIsRegular(LCUI_DirEntry *entry);

LCUI_END_HEADER

#endif
