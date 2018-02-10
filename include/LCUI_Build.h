/*
 * LCUI_Build.h -- Build-related configuration definitions
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


#ifndef LCUI_BUILD_H
#define LCUI_BUILD_H

#if defined(__GNUC__)
#  define LCUI_API
#elif (defined(_MSC_VER) && _MSC_VER < 800) ||\
    (defined(__BORLANDC__) && __BORLANDC__ < 0x500)
  /* older Borland and MSC
   * compilers used '__export' and required this to be after
   * the type.
   */
#  define LCUI_API __export
#else /* newer compiler */
#  ifdef LCUI_EXPORTS
#    define LCUI_API __declspec(dllexport)
#  else
#    define LCUI_API
#  endif
#endif /* compiler */

#ifdef DEBUG
#define DEBUG_MSG _DEBUG_MSG
#else
#define DEBUG_MSG(format, ...)
#endif

#define LOG Logger_Log
#define LOGW Logger_LogW
#define _DEBUG_MSG(format, ...) Logger_Log(__FILE__" %d: %s(): "format, __LINE__, __FUNCTION__,##__VA_ARGS__)

#if defined(WIN32) || defined(_WIN32)
#define LCUI_BUILD_IN_WIN32
#define LCUI_THREAD_WIN32
#define LCUI_VIDEO_DRIVER_WIN32
#define LCUI_FONT_ENGINE_FREETYPE
#define USE_LIBPNG
#define USE_LIBJPEG
#define USE_LCUI_BUILDER
#define ENABLE_TOUCH_SUPPORT
#undef LCUI_THREAD_PTHREAD
#undef LCUI_VIDEO_DRIVER_FRAMEBUFFER
#else
#include <LCUI/config.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#define LCUI_BUILD_IN_LINUX
#define LCUI_KEYBOARD_DRIVER_LINUX
#define LCUI_MOUSE_DRIVER_LINUX
#endif

#ifdef __cplusplus
#define LCUI_BEGIN_HEADER  extern "C"{
#define LCUI_END_HEADER  }
#else
#define LCUI_BEGIN_HEADER  /* nothing */
#define LCUI_END_HEADER
#endif

#endif
