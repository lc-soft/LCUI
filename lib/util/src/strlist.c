/*
 * strlist.c -- string list
 *
 * Copyright (c) 2019, Liu chao <lc-soft@live.cn> All rights reserved.
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/util/strpool.h>
#include <LCUI/util/strlist.h>

static strpool_t *pool = NULL;

int sortedstrlist_add(strlist_t *strlist, const char *str)
{
	int i, pos, n;
	strlist_t newlist;

	if (*strlist) {
		for (i = 0; (*strlist)[i]; ++i)
			;
		n = i + 2;
	} else {
		n = 2;
	}
	newlist = realloc(*strlist, sizeof(char *) * n);
	if (!newlist) {
		return -ENOMEM;
	}
	newlist[n - 2] = NULL;
	for (i = 0, pos = -1; newlist[i]; ++i) {
		int tmp = strcmp(newlist[i], str);
		if (tmp < 0) {
			continue;
		} else if (tmp == 0) {
			return 1;
		} else {
			pos = i;
			break;
		}
	}
	if (!pool) {
		pool = strpool_create();
	}
	if (pos >= 0) {
		for (i = n - 2; i > pos; --i) {
			newlist[i] = newlist[i - 1];
		}
	} else {
		pos = n - 2;
	}
	newlist[pos] = strpool_alloc_str(pool, str);
	newlist[n - 1] = NULL;
	*strlist = newlist;
	return 0;
}

int strlist_add_one(strlist_t *strlist, const char *str)
{
	int i = 0;
	char **newlist;

	if (!*strlist) {
		newlist = (char **)malloc(sizeof(char *) * 2);
		goto check_done;
	}
	for (i = 0; (*strlist)[i]; ++i) {
		if (strcmp((*strlist)[i], str) == 0) {
			return 1;
		}
	}
	newlist = (char **)realloc(*strlist, (i + 2) * sizeof(char *));

check_done:
	if (!newlist) {
		return -ENOMEM;
	}
	if (!pool) {
		pool = strpool_create();
	}
	newlist[i] = strpool_alloc_str(pool, str);
	newlist[i + 1] = NULL;
	*strlist = newlist;
	return 0;
}

int strlist_add(strlist_t *strlist, const char *str)
{
	char buff[256];
	int count = 0, i, head;

	for (head = 0, i = 0; str[i]; ++i) {
		if (str[i] != ' ') {
			continue;
		}
		if (i > head) {
			strncpy(buff, &str[head], i - head);
			buff[i - head] = 0;
			if (strlist_add_one(strlist, buff) == 0) {
				count += 1;
			}
		}
		head = i + 1;
	}
	if (i > head) {
		strncpy(buff, &str[head], i - head);
		buff[i - head] = 0;
		if (strlist_add_one(strlist, buff) == 0) {
			count += 1;
		}
	}
	return count;
}

int strlist_has(strlist_t strlist, const char *str)
{
	int i;
	if (!strlist) {
		return 0;
	}
	for (i = 0; strlist[i]; ++i) {
		if (strcmp(strlist[i], str) == 0) {
			return 1;
		}
	}
	return 0;
}

int strlist_remove_one(strlist_t *strlist, const char *str)
{
	int i, pos, len;
	strlist_t newlist;

	if (!*strlist) {
		return 0;
	}
	for (len = 0; (*strlist)[len]; ++len)
		;
	for (pos = -1, i = 0; i < len; ++i) {
		if (strcmp((*strlist)[i], str) == 0) {
			pos = i;
			break;
		}
	}
	if (pos == -1) {
		return 0;
	}
	strpool_free_str((*strlist)[pos]);
	if (pos == 0 && len < 2) {
		free(*strlist);
		*strlist = NULL;
		return 1;
	}
	newlist = (char **)malloc(len * sizeof(char *));
	for (i = 0; i < pos; ++i) {
		newlist[i] = (*strlist)[i];
	}
	for (i = pos; i < len; ++i) {
		newlist[i] = (*strlist)[i + 1];
	}
	newlist[len - 1] = NULL;
	free(*strlist);
	*strlist = newlist;
	return 1;
}

int strlist_remove(strlist_t *strlist, const char *str)
{
	char buff[256];
	int count = 0, i, head;

	for (head = 0, i = 0; str[i]; ++i) {
		if (str[i] != ' ') {
			continue;
		}
		if (i - 1 > head) {
			strncpy(buff, &str[head], i - head);
			buff[i - head] = 0;
			count += strlist_remove_one(strlist, buff);
		}
		head = i + 1;
	}
	if (i - 1 > head) {
		strncpy(buff, &str[head], i - head);
		buff[i - head] = 0;
		count += strlist_remove_one(strlist, buff);
	}
	return count;
}

void strlist_free(strlist_t strlist)
{
	int i = 0;
	while (strlist[i]) {
		strpool_free_str(strlist[i]);
		++i;
	}
	free(strlist);
}
