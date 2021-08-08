/*
 * logger.h -- Logger module
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn>
 * Copyright (c) 2021, Li Zihao <yidianyiko@foxmail.com>
 * All rights reserved.
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

#ifndef UTIL_LOGGER_H
#define UTIL_LOGGER_H

Y_BEGIN_DECLS

enum logger_level_e {
	LOGGER_LEVEL_ALL,
	LOGGER_LEVEL_DEBUG,
	LOGGER_LEVEL_INFO,
	LOGGER_LEVEL_WARNING,
	LOGGER_LEVEL_ERROR,
	LOGGER_LEVEL_OFF
};

typedef enum logger_level_e logger_level_e;

YUTIL_API void logger_set_level(logger_level_e level);

YUTIL_API int logger_log(logger_level_e level, const char* fmt, ...);

YUTIL_API int logger_log_w(logger_level_e level, const wchar_t* fmt, ...);

YUTIL_API void logger_set_handler(void (*handler)(const char*));

YUTIL_API void logger_set_handler_w(void (*handler)(const wchar_t*));

#define logger_info(fmt, ...) logger_log(LOGGER_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define logger_debug(fmt, ...) \
	logger_log(LOGGER_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define logger_warning(fmt, ...) \
	logger_log(LOGGER_LEVEL_WARNING, fmt, ##__VA_ARGS__)
#define logger_error(fmt, ...) \
	logger_log(LOGGER_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define logger_info_w(fmt, ...) \
	logger_log_w(LOGGER_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define logger_debug_w(fmt, ...) \
	logger_log_w(LOGGER_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define logger_warning_w(fmt, ...) \
	logger_log_w(LOGGER_LEVEL_WARNING, fmt, ##__VA_ARGS__)
#define logger_error_w(fmt, ...) \
	logger_log_w(LOGGER_LEVEL_ERROR, fmt, ##__VA_ARGS__)

Y_END_DECLS
#endif
