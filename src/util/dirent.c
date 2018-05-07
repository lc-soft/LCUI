/* dirent.c -- Directory entry operation set.
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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font/charset.h>

int LCUI_OpenDirA( const char *filepath, LCUI_Dir *dir )
{
#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
	int len;
	char *newpath;

	len = strlen( filepath ) + 5;
	newpath = malloc( len * sizeof(char) );
	if( newpath == NULL ) {
		return -ENOMEM;
	}
	/* 需要加上通配符 */
	sprintf_s( newpath, len, "%s\\*", filepath );
	dir->handle = FindFirstFileA( newpath, &dir->entry.dataA );
	free( newpath );
	if( dir->handle == INVALID_HANDLE_VALUE ) {
		switch( GetLastError() ) {
		case ERROR_FILE_NOT_FOUND: return -ENOENT;
		case ERROR_ACCESS_DENIED: return -EACCES;
		default: break;
		}
		return -1;
	}
	dir->cached = 1;
#endif
	return 0;
}

int LCUI_OpenDirW( const wchar_t *path, LCUI_Dir *dir )
{
#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
	int len;
	wchar_t *newpath;

	len = wcslen( path ) + 5;
	newpath = malloc( len * sizeof(wchar_t) );
	if( !newpath ) {
		return -ENOMEM;
	}
	swprintf( newpath, len, L"%s\\*", path );
	dir->handle = FindFirstFileW( newpath, &dir->entry.dataW );
	free( newpath );
	if( dir->handle == INVALID_HANDLE_VALUE ) {
		switch( GetLastError() ) {
		case ERROR_FILE_NOT_FOUND: return -ENOENT;
		case ERROR_ACCESS_DENIED: return -EACCES;
		default: break;
		}
		return -1;
	}
	dir->cached = 1;
#else

	int len;
	char *newpath;
	len = LCUI_EncodeString( NULL, path, 0, ENCODING_UTF8 ) + 1;
	newpath = malloc( len * sizeof(wchar_t) );
	LCUI_EncodeString( newpath, path, len, ENCODING_UTF8 );
	dir->handle = opendir( newpath );
	free( newpath );
	if( !dir->handle ) {
		return -1;
	}
#endif
	return 0;
}

LCUI_DirEntry* LCUI_ReadDirA( LCUI_Dir *dir )
{
#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
	if( dir->handle == INVALID_HANDLE_VALUE ) {
		return NULL;
	}
	if( dir->cached ) {
		dir->cached = 0;
		return &dir->entry;
	}
	if( FindNextFileA( dir->handle, &dir->entry.dataA ) ) {
		return &dir->entry;
	}
	FindClose( dir->handle );
	dir->handle = INVALID_HANDLE_VALUE;
#endif
	return NULL;
}

LCUI_DirEntry* LCUI_ReadDirW( LCUI_Dir *dir )
{
#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
	if( dir->handle == INVALID_HANDLE_VALUE ) {
		return NULL;
	}
	if( dir->cached ) {
		dir->cached = 0;
		return &dir->entry;
	}
	if( FindNextFileW( dir->handle, &dir->entry.dataW ) ) {
		return &dir->entry;
	}
	 FindClose( dir->handle );
	 dir->handle = INVALID_HANDLE_VALUE;
	 return NULL;
#else
	struct dirent *d;
	d = readdir( dir->handle );
	if( !d ) {
		return NULL;
	}
	dir->entry.dirent = *d;
	LCUI_DecodeString( dir->entry.name, d->d_name, 
			   d->d_reclen + 1, ENCODING_UTF8 );
	return &dir->entry;
#endif
}

int LCUI_CloseDir( LCUI_Dir *dir )
{
#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
	if( !FindClose( dir->handle ) ) {
		return -1;
	}
#else
	closedir( dir->handle );
#endif
	return 0;
}

char *LCUI_GetFileNameA( LCUI_DirEntry *entry )
{
#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
	return entry->dataA.cFileName;
#endif
	return NULL;
}

wchar_t *LCUI_GetFileNameW( LCUI_DirEntry *entry )
{
#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
	return entry->dataW.cFileName;
#else
	return entry->name;
#endif
	return NULL;
}

int LCUI_FileIsDirectory( LCUI_DirEntry *entry )
{
#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
	return entry->dataW.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
#else
	return entry->dirent.d_type == DT_DIR;
#endif
}

int LCUI_FileIsRegular( LCUI_DirEntry *entry )
{
#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
	return !(entry->dataW.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
#else
	return entry->dirent.d_type == DT_REG;
#endif
}
