// dirent.c -- Directory Entry pperation set.
#include <LCUI_Build.h>
#include LC_LCUI_H

LCUI_API int LCUI_OpenDirA( char *filepath, LCUI_Dir *dir_data )
{
#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
	int len;
	char *new_filepath;

	len = strlen( filepath )+5;
	new_filepath = (char*)malloc( len * sizeof(char) );
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

LCUI_API int LCUI_OpenDirW( wchar_t *filepath, LCUI_Dir *dir_data )
{
#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
	int len;
	wchar_t *new_filepath;

	len = wcslen( filepath )+5;
	new_filepath = (wchar_t*)malloc( len * sizeof(wchar_t) );
	if( new_filepath == NULL ) {
		return -1;
	}
	swprintf( new_filepath, len, L"%s\\*", filepath );
	dir_data->handle = FindFirstFileW( new_filepath, &dir_data->entry.dataW );
	free( new_filepath );
	if( dir_data->handle == INVALID_HANDLE_VALUE ) {
		return -1;
	}
	dir_data->cached = 1;
#endif
	return 0;
}

LCUI_API LCUI_DirEntry* LCUI_ReadDirA( LCUI_Dir *dir_data )
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

LCUI_API LCUI_DirEntry* LCUI_ReadDirW( LCUI_Dir *dir_data )
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
#endif
	return NULL;
}

LCUI_API int LCUI_CloseDir( LCUI_Dir *dir_data )
{
#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
	if( !FindClose( dir_data->handle ) ) {
		return -1;
	}
#endif
	return 0;
}

char *LCUI_GetFileNameA( LCUI_DirEntry *entry )
{
	return entry->dataA.cFileName;
}

wchar_t *LCUI_GetFileNameW( LCUI_DirEntry *entry )
{
	return entry->dataW.cFileName;
}

int LCUI_FileIsDirectory( LCUI_DirEntry *entry )
{
	/* 由于entry是共用体，entry->dataA和entry->dataW的dwFileAttributes
	 * 都是在同一内存空间，因此可直接用entry->dataA */
	return entry->dataA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

int LCUI_FileIsNormal( LCUI_DirEntry *entry )
{
	return entry->dataA.dwFileAttributes & FILE_ATTRIBUTE_NORMAL;
}
