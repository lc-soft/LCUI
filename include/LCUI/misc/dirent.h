// dirent.h -- Directory Entry pperation set.
#ifndef __LCUI_DIRECTORY_ENTRY_H__
#define __LCUI_DIRECTORY_ENTRY_H__

LCUI_BEGIN_HEADER

#if defined (LCUI_BUILD_IN_WIN32) || (_WIN32)
#include <Windows.h>
typedef HANDLE LCUI_DirHandle;
typedef union LCUI_DirEntry_ {
	WIN32_FIND_DATAA dataA;
	WIN32_FIND_DATAW dataW;
} LCUI_DirEntry;

#define PATH_DELIMITER '\\'

#elif defined(LCUI_BUILD_IN_LINUX)
typedef DIR* LCUI_DirHandle;
typedef struct dirent LCUI_DirEntry;

#define PATH_DELIMITER '/'

#else
#error 'Does not support your platform!'
#endif

typedef struct LCUI_Dir_ {
	LCUI_DirHandle handle;
	LCUI_DirEntry entry;
	int cached;
} LCUI_Dir;

#ifdef _UNICODE 
#define LCUI_OpenDir LCUI_OpenDirW
#define LCUI_ReadDir LCUI_ReadDirW
#define LCUI_GetFileName LCUI_GetFileNameW
#else
#define LCUI_OpenDir LCUI_OpenDirA
#define LCUI_ReadDir LCUI_ReadDirA
#define LCUI_GetFileName LCUI_GetFileNameA
#endif


LCUI_API int LCUI_OpenDirW( wchar_t *filepath, LCUI_Dir *dir_data );

LCUI_API int LCUI_OpenDirA( char *filepath, LCUI_Dir *dir_data );

LCUI_API LCUI_DirEntry* LCUI_ReadDirA( LCUI_Dir *dir_data );

LCUI_API LCUI_DirEntry* LCUI_ReadDirW( LCUI_Dir *dir_data );

LCUI_API int LCUI_CloseDir( LCUI_Dir *dir_data );

LCUI_API char *LCUI_GetFileNameA( LCUI_DirEntry *entry );

LCUI_API wchar_t *LCUI_GetFileNameW( LCUI_DirEntry *entry );

LCUI_API int LCUI_FileIsDirectory( LCUI_DirEntry *entry );

LCUI_API int LCUI_FileIsArchive( LCUI_DirEntry *entry );

LCUI_END_HEADER

#endif