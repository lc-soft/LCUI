/* ***************************************************************************
 * logger.c -- logger module
 *
 * Copyright (C) 2016-1017 by Liu Chao <lc-soft@live.cn>
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
 * logger.c -- 日志记录器
 *
 * 版权所有 (C) 2016-1017 归属于 刘超 <lc-soft@live.cn>
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
#include <stdarg.h>
#include <wchar.h>
#include <LCUI_Build.h>
#include <LCUI/thread.h>
#include <LCUI/util/logger.h>

#define BUFFER_SIZE 2048

static struct Logger {
	char inited;
	char buffer[BUFFER_SIZE];
	wchar_t bufferw[BUFFER_SIZE];
	void( *handler )(const char*);
	void( *handlerw )(const wchar_t*);
	LCUI_Mutex mutex;
} logger = { 0 };

int Logger_Log( const char* fmt, ... )
{
	int len;
	va_list args;
	if( !logger.inited ) {
		LCUIMutex_Init( &logger.mutex );
		logger.inited = 1;
	}
	va_start( args, fmt );
	LCUIMutex_Lock( &logger.mutex );
	if( logger.handler ) {
		len = vsnprintf( logger.buffer, BUFFER_SIZE, fmt, args );
		logger.buffer[BUFFER_SIZE - 1] = 0;
		logger.handler( logger.buffer );
	} else {
		len = vprintf( fmt, args );
	}
	LCUIMutex_Unlock( &logger.mutex );
	va_end( args );
	return len;
}

int Logger_LogW( const wchar_t* fmt, ... )
{
	int len;
	va_list args;
	if( !logger.inited ) {
		LCUIMutex_Init( &logger.mutex );
		logger.inited = 1;
	}
	va_start( args, fmt );
	LCUIMutex_Lock( &logger.mutex );
	if( logger.handlerw ) {
		len = vswprintf( logger.bufferw, BUFFER_SIZE, fmt, args );
		logger.bufferw[BUFFER_SIZE - 1] = 0;
		logger.handlerw( logger.bufferw );
	} else {
		len = vwprintf( fmt, args );
	}
	LCUIMutex_Unlock( &logger.mutex );
	va_end( args );
	return len;
}

void Logger_SetHandler( void (*handler)(const char*) )
{
	logger.handler = handler;
}

void Logger_SetHandlerW( void (*handler)(const wchar_t*) )
{
	logger.handlerw = handler;
}
