/* logger.c -- Logger module
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
