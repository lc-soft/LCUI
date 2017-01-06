/* ***************************************************************************
 * dirent.c -- directory entry operation set.
 *
 * Copyright (C) 2015-2017 by Liu Chao <lc-soft@live.cn>
 *
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 *
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 *
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *
 * The LCUI project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 *
 * You should have received a copy of the GPLv2 along with this file. It is
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/

/* ****************************************************************************
 * dirent.c -- 目录操作集
 *
 * 版权所有 (C) 2015-2017 归属于 刘超 <lc-soft@live.cn>
 *
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 *
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 *
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>.
 * ****************************************************************************/

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
	/* 由于entry是共用体，entry->dataA和entry->dataW的dwFileAttributes
	 * 都是在同一内存空间，因此可直接用entry->dataA */
	return entry->dataA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
#else
	return entry->dirent.d_type == DT_DIR;
#endif
}

int LCUI_FileIsArchive( LCUI_DirEntry *entry )
{
#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
	return entry->dataA.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE;
#else
	return entry->dirent.d_type == DT_REG;
#endif
}
