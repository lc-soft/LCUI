/*
 * strpool.c -- string pool
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
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "yutil/keywords.h"
#include "yutil/string.h"
#include "yutil/strpool.h"
#include "yutil/dict.h"

#define STRPOOL_MARK 6699

typedef struct strpool_entry_t strpool_entry_t;

struct strpool_t {
	size_t size;
	dict_type_t type;
	dict_t *dict;
};

struct strpool_entry_t {
	int32_t mark;
	strpool_t *pool;
	size_t count;
	size_t length;
	char *string;
};

strpool_t *strpool_create(void)
{
	strpool_t *pool;

	pool = (strpool_t *)malloc(sizeof(strpool_t));
	if (!pool) {
		return NULL;
	}
	pool->size = 0;
	dict_init_string_key_type(&pool->type);
	pool->dict = dict_create(&pool->type, NULL);
	return pool;
}

char *strpool_alloc_str(strpool_t *pool, const char *str)
{
	size_t size;
	size_t length;
	strpool_entry_t *entry;

	entry = (strpool_entry_t *)dict_fetch_value(pool->dict, str);
	if (!entry) {
		length = strlen(str);
		size = sizeof(strpool_entry_t) + length + 1;
		entry = (strpool_entry_t *)malloc(size);
		if (!entry) {
			return NULL;
		}
		entry->mark = STRPOOL_MARK;
		entry->pool = pool;
		entry->count = 0;
		entry->length = length;
		entry->string = ((char *)entry) + sizeof(strpool_entry_t);
		strcpy(entry->string, str);
		if (dict_add(pool->dict, entry->string, entry) != 0) {
			free(entry);
			return NULL;
		}
		pool->size += size;
	}
	entry->count += 1;
	return entry->string;
}

int strpool_free_str(char *str)
{
	strpool_entry_t *entry;

	entry = (strpool_entry_t *)(str - sizeof(strpool_entry_t));
	if (entry->mark != STRPOOL_MARK) {
		return -1;
	}
	entry->count -= 1;
	if (entry->count > 0) {
		return 0;
	}
	entry->pool->size -= sizeof(strpool_entry_t) + strlen(str) + 1;
	dict_delete(entry->pool->dict, str);
	free(entry);
	return 0;
}

size_t strpool_size(strpool_t *pool)
{
	return pool->size;
}

void strpool_destroy(strpool_t *pool)
{
	pool->size = 0;
	dict_destroy(pool->dict);
	free(pool);
}
