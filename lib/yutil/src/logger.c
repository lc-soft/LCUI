/* logger.c -- Logger module
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

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <wchar.h>
#include "yutil/keywords.h"
#include "yutil/list_entry.h"
#include "yutil/logger.h"

#define BUFFER_SIZE 2048

struct logger_t {
	char inited;
	void (*handler)(const char *);
	void (*handler_w)(const wchar_t *);
	logger_level_e level;
};

struct logger_buffer_t {
	char buffer[BUFFER_SIZE];
	wchar_t buffer_w[BUFFER_SIZE];
	list_entry_t buf_entry;
};

typedef struct logger_t logger_t;
typedef struct logger_buffer_t logger_buffer_t;

static logger_t logger = { 0 };
static list_entry_head_t logger_buffer_head = { 0 };

/* buffered output enabled flag */
static bool is_enabled = false;

void logger_set_level(logger_level_e level)
{
	logger.level = level;
}

int logger_log(logger_level_e level, const char *fmt, ...)
{
	if (level < logger.level) {
		return 0;
	}
	if (!logger.inited) {
		list_entry_head_init(&logger_buffer_head, logger_buffer_t,
				     buf_entry);
		logger.inited = 1;
	}
	int len;
	va_list args;
	logger_buffer_t *node;
	node = (logger_buffer_t *)malloc(sizeof(logger_buffer_t));
	if (node == NULL)
		return 0;

	va_start(args, fmt);
	len = vsnprintf(node->buffer, BUFFER_SIZE, fmt, args);
	va_end(args);
	node->buffer[BUFFER_SIZE - 1] = 0;
	list_entry_add_tail(&logger_buffer_head, &node->buf_entry);

	if (is_enabled) {
		return 0;
	}
	is_enabled = true;
	while (!list_entry_is_empty(&logger_buffer_head)) {
		unsigned int i = 0;
		list_entry_t *entry = NULL;
		logger_buffer_t *output = NULL;
		list_entry_head_t logger_buffer_head_copy;

		logger_buffer_head_copy = logger_buffer_head;
		list_entry_clear(&logger_buffer_head);

		list_entry_for_each_by_length(&logger_buffer_head_copy, i,
					      entry)
		{
			if (output) {
				free(output);
			}
			output = list_entry(&logger_buffer_head_copy, entry,
					    logger_buffer_t);

			if (logger.handler) {
				logger.handler((const char *)&output->buffer);
			} else {
				printf("%s", output->buffer);
			}
		}
		if (output) {
			free(output);
		}
		list_entry_exit(&logger_buffer_head_copy);
	}
	is_enabled = false;
	return len;
}

int logger_log_w(logger_level_e level, const wchar_t *fmt, ...)
{
	if (level < logger.level) {
		return 0;
	}
	if (!logger.inited) {
		list_entry_head_init(&logger_buffer_head, logger_buffer_t,
				     buf_entry);
		logger.inited = 1;
	}
	int len;
	va_list args;
	logger_buffer_t *node;
	node = (logger_buffer_t *)malloc(sizeof(logger_buffer_t));
	if (node == NULL)
		return 0;

	va_start(args, fmt);
	len =
	    vswprintf((wchar_t *const)&node->buffer_w, BUFFER_SIZE, fmt, args);
	va_end(args);
	node->buffer_w[BUFFER_SIZE - 1] = 0;
	list_entry_add_tail(&logger_buffer_head, &node->buf_entry);

	if (is_enabled) {
		return 0;
	}
	is_enabled = true;
	while (!list_entry_is_empty(&logger_buffer_head)) {
		unsigned int i = 0;
		list_entry_t *entry = NULL;
		logger_buffer_t *output = { 0 };
		list_entry_head_t logger_buffer_head_copy;

		logger_buffer_head_copy = logger_buffer_head;
		list_entry_clear(&logger_buffer_head);

		list_entry_for_each_by_length(&logger_buffer_head_copy, i,
					      entry)
		{
			output = list_entry(&logger_buffer_head_copy, entry,
					    logger_buffer_t);

			if (logger.handler) {
				logger.handler_w(
				    (const wchar_t *)&output->buffer_w);
			} else {
				wprintf(L"%s", output->buffer_w);
			}
			free(output);
		}
	}
	is_enabled = false;
	return len;
}

void logger_set_handler(void (*handler)(const char *))
{
	logger.handler = handler;
}

void logger_set_handler_w(void (*handler)(const wchar_t *))
{
	logger.handler_w = handler;
}
