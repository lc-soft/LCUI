/* ***************************************************************************
* win32_main.c -- win32 platform WinMain function for LCUI.
*
* Copyright (C) 2015 by Liu Chao <lc-soft@live.cn>
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
* win32_main.c -- win32平台下的 WinMain 函数定义
*
* 版权所有 (C) 2015 归属于 刘超 <lc-soft@live.cn>
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
#include <tchar.h>
#include <Windows.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#ifdef _UNICODE
#define _tcstok_s wcstok_s
#define _tcscpy_s wcscpy_s
#else
#define _tcstok_s strtok_s
#define _tcscpy_s strcpy_s
#endif

extern int main( int argc, char *argv[] );

int APIENTRY WinMain( _In_ HINSTANCE hInstance,
		      _In_opt_ HINSTANCE hPrevInstance,
		      _In_ LPSTR lpCmdLine,
		      _In_ int nCmdShow )
{
	int ret, len, i = 0, argc = 0;
	char *cmdline, *cmdline_buff;
	char *token = NULL, *next_token = NULL, **argv = NULL;

	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );
	LCUI_PreInitApp( hInstance );
	cmdline = GetCommandLineA();
	len = strlen( cmdline ) + 1;
	cmdline_buff = (char*)malloc( sizeof( char )*len );
	if( !cmdline_buff ) {
		return -1;
	}
	strcpy_s( cmdline_buff, len, cmdline );
	token = strtok_s( cmdline_buff, " \r\t\n", &next_token );
	while( token ) {
		argc++;
		token = strtok_s( NULL, " \r\t\n", &next_token );
	}
	if( argc > 0 ) {
		argv = (char**)malloc( sizeof( char* )*argc );
	}
	if( !argv ) {
		return -1;
	}
	strcpy_s( cmdline_buff, len, cmdline );
	token = strtok_s( cmdline_buff, " \r\t\n", &next_token );
	while( token && i < argc ) {
		len = strlen( token ) + 1;
		argv[i] = (char*)malloc( sizeof( char )*len );
		if( argv[i] == NULL ) {
			return -1;
		}
		strcpy_s( argv[i], len, token );
		token = strtok_s( NULL, " \r\t\n", &next_token );
		++i;
	}
	ret = main( argc, (char**)argv );
	for( i = 0; i < argc; ++i ) {
		free( argv[i] );
	}
	free( argv );
	free( cmdline_buff );
	return ret;
}
