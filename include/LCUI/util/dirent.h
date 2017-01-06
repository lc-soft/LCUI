/* ***************************************************************************
* dirent.h -- directory entry operation set.
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
* dirent.h -- 目录操作集
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

#ifndef LCUI_UTIL_DIRENT_H
#define LCUI_UTIL_DIRENT_H

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
#include <dirent.h>
typedef DIR* LCUI_DirHandle;
typedef struct LCUI_DirEntry_ {
	struct dirent dirent;
	wchar_t name[256];
} LCUI_DirEntry;

#define PATH_DELIMITER '/'

#else
#error 'Does not support your platform!'
#endif

typedef struct LCUI_Dir_ {
	LCUI_DirHandle handle;
	LCUI_DirEntry entry;
	int cached;
} LCUI_Dir;

#if defined (LCUI_BUILD_IN_LINUX) || _UNICODE
#define LCUI_OpenDir LCUI_OpenDirW
#define LCUI_ReadDir LCUI_ReadDirW
#define LCUI_GetFileName LCUI_GetFileNameW
#else
#define LCUI_OpenDir LCUI_OpenDirA
#define LCUI_ReadDir LCUI_ReadDirA
#define LCUI_GetFileName LCUI_GetFileNameA
#endif


LCUI_API int LCUI_OpenDirW( const wchar_t *filepath, LCUI_Dir *dir );

LCUI_API int LCUI_OpenDirA( const char *filepath, LCUI_Dir *dir );

LCUI_API LCUI_DirEntry* LCUI_ReadDirA( LCUI_Dir *dir );

LCUI_API LCUI_DirEntry* LCUI_ReadDirW( LCUI_Dir *dir );

LCUI_API int LCUI_CloseDir( LCUI_Dir *dir );

LCUI_API char *LCUI_GetFileNameA( LCUI_DirEntry *entry );

LCUI_API wchar_t *LCUI_GetFileNameW( LCUI_DirEntry *entry );

LCUI_API int LCUI_FileIsDirectory( LCUI_DirEntry *entry );

LCUI_API int LCUI_FileIsArchive( LCUI_DirEntry *entry );

LCUI_END_HEADER

#endif
