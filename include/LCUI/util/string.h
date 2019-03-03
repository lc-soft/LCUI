/*
 * string.h -- The string operation set.
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

#ifndef LCUI_UTIL_STRING_H
#define LCUI_UTIL_STRING_H

LCUI_BEGIN_HEADER

/** strdup() 的再实现版本，用于清除编译错误 */
LCUI_API char *strdup2(const char *str);
LCUI_API wchar_t *wcsdup2(const wchar_t *str);

/** 计算字符串占用的内存空间大小 */
LCUI_API size_t strsize(const char *str);
LCUI_API size_t wcssize(const wchar_t *str);

/** 将字符串中的字母转成小写字母 */
LCUI_API size_t strtolower(char *outstr, const char *instr);

LCUI_API size_t strntolower(char *outstr, size_t max_len, const char *instr);

/**
 * 清除字符串首尾的字符
 * @param[out] outstr 处理后的字符串
 * @param[in] instr 需要处理的字符串
 * @param[in] charlist 需要清除的字符列表，当为NULL时，默认清除空白符
 * @return 处理后的字符串的长度
 */
LCUI_API size_t strtrim(char *outstr, const char *instr, const char *charlist);
LCUI_API size_t wcstrim(wchar_t *outstr, const wchar_t *instr,
			const wchar_t *charlist);

LCUI_API unsigned strhash(unsigned hash, const char *str);

/**
 * 字符串替换
 * @param[in][out] str 需要处理的字符串，替换成功后字符串内容也会被修改
 * @param[in] max_len 输出字符串的最大长度
 * @param[in] substr 字符串中需要被替换的子字符串
 * @param[in] newstr 替换的新字符串
 * @returns 替换后的字符串长度，若未替换，则返回 0
 */
LCUI_API size_t wcsreplace(wchar_t *str, size_t max_len, const wchar_t *substr,
			   const wchar_t *newstr);

LCUI_API size_t strreplace(char *str, size_t max_len, const char *substr,
			   const char *newstr);

/**
 * 分割命令行字符串
 * @param[in] cmd 需分割的命令行字符串
 * @param[out] outargv 分割后的命令行参数列表
 * @return 参数数量
 */
LCUI_API int cmdsplit(const char *cmd, char ***outargv);

/**
 * 分割字符串
 * @param[in] instr 需分割的字符串
 * @param[in] sep 分割标记字符串
 * @param[out] outstrs 分割后的字符串列表
 */
LCUI_API int strsplit(const char *instr, const char *sep, char ***outstrs);

LCUI_END_HEADER

#endif
