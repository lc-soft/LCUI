// dirent.c -- Directory Entry pperation set.
#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font/charset.h>

int LCUI_OpenDirA( const char *filepath, LCUI_Dir *dir_data )
{
#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
	int len;
	char *new_filepath;

	len = strlen( filepath ) + 5;
	new_filepath = malloc( len * sizeof(char) );
	if( new_filepath == NULL ) {
		return -1;
	}
	/* 需要加上通配符 */
	sprintf_s( new_filepath, len, "%s\\*", filepath );
	dir_data->handle = FindFirstFileA( new_filepath, &dir_data->entry.dataA );
	free( new_filepath );
	if( dir_data->handle == INVALID_HANDLE_VALUE ) {
		return -1;
	}
	dir_data->cached = 1;
#endif
	return 0;
}

int LCUI_OpenDirW( const wchar_t *filepath, LCUI_Dir *dir_data )
{
#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
	int len;
	wchar_t *new_filepath;

	len = wcslen( filepath ) + 5;
	new_filepath = malloc( len * sizeof(wchar_t) );
	if( !new_filepath ) {
		return -1;
	}
	swprintf( new_filepath, len, L"%s\\*", filepath );
	dir_data->handle = FindFirstFileW( new_filepath, &dir_data->entry.dataW );
	free( new_filepath );
	if( dir_data->handle == INVALID_HANDLE_VALUE ) {
		return -1;
	}
	dir_data->cached = 1;
#else

	int len;
	char *path;
	len = LCUI_EncodeString( NULL, filepath, 0, ENCODING_UTF8 ) + 1;
	path = malloc( len * sizeof(wchar_t) );
	LCUI_EncodeString( path, filepath, len, ENCODING_UTF8 );
	dir_data->handle = opendir( path );
	free( path );
	if( !dir_data->handle ) {
		return -1;
	}
#endif
	return 0;
}

LCUI_DirEntry* LCUI_ReadDirA( LCUI_Dir *dir_data )
{
#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
	if( dir_data->handle == INVALID_HANDLE_VALUE ) {
		return NULL;
	}
	if( dir_data->cached ) {
		dir_data->cached = 0;
		return &dir_data->entry;
	}
	if( FindNextFileA( dir_data->handle, &dir_data->entry.dataA ) ) {
		return &dir_data->entry;
	} else {
		 FindClose( dir_data->handle );
		 dir_data->handle = INVALID_HANDLE_VALUE;
		 return NULL;
	}
#endif
	return NULL;
}

LCUI_DirEntry* LCUI_ReadDirW( LCUI_Dir *dir_data )
{
#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
	if( dir_data->handle == INVALID_HANDLE_VALUE ) {
		return NULL;
	}
	if( dir_data->cached ) {
		dir_data->cached = 0;
		return &dir_data->entry;
	}
	if( FindNextFileW( dir_data->handle, &dir_data->entry.dataW ) ) {
		return &dir_data->entry;
	} else {
		 FindClose( dir_data->handle );
		 dir_data->handle = INVALID_HANDLE_VALUE;
		 return NULL;
	}
#else
	struct dirent *d;
	_DEBUG_MSG("handle: %p\n", dir_data->handle);
	d = readdir( dir_data->handle );
	if( !d ) {
		perror("opendir() failed");
		return NULL;
	}
	dir_data->entry.dirent = *d;
	LCUI_DecodeString( dir_data->entry.name, d->d_name, 
			   d->d_reclen + 1, ENCODING_UTF8 );
	return &dir_data->entry;
#endif
}

int LCUI_CloseDir( LCUI_Dir *dir_data )
{
#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
	if( !FindClose( dir_data->handle ) ) {
		return -1;
	}
#else
	closedir( dir_data->handle );
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
