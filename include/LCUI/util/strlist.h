/*
 * strlist.h -- string list
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

#ifndef LCUI_UTIL_STRLIST_H
#define LCUI_UTIL_STRLIST_H

typedef char** strlist_t;

LCUI_API int sortedstrlist_add(strlist_t *strlist, const char *str);

LCUI_API int strlist_add_one(strlist_t *strlist, const char *str);

LCUI_API int strlist_remove_one(strlist_t *strlist, const char *str);

/**
 * 向字符串组添加字符串
 * @param[in][out] strlist 字符串组
 * @param[in] str 字符串
 */
LCUI_API int strlist_add(strlist_t *strlist, const char *str);

/**
 * 判断字符串组中是否包含指定字符串
 * @param[in][out] strlist 字符串组
 * @param[in] str 字符串
 * @returns 如果包含则返回 1， 否则返回 0
 */
LCUI_API int strlist_has(strlist_t strlist, const char *str);

/**
 * 从字符串组中移除指定字符串
 * @param[in][out] strlist 字符串组
 * @param[in] str 字符串
 * @returns 如果删除成功则返回 1， 否则返回 0
 */
LCUI_API int strlist_remove(strlist_t *strlist, const char *str);

/**
 * 向已排序的字符串组添加字符串
 * @param[in][out] strlist 字符串组
 * @param[in] str 字符串
 */
LCUI_API int sortedstrlist_add(strlist_t *strlist, const char *str);

/** 释放字符串组 */
LCUI_API void strlist_free(strlist_t strs);

#endif
