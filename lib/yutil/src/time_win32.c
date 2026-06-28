/* time.c -- The time operation set.
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

#include <stdint.h>
#if defined(_WIN32)
#include <windows.h>
#include "yutil/keywords.h"
#include "yutil/time.h"

#define TIME_WRAP_VALUE (~(int64_t)0)

int64_t get_time_ms(void)
{
	int64_t time;
	LARGE_INTEGER hires = { { 0 } };
	LARGE_INTEGER hires_now = { { 0 } };
	if (QueryPerformanceFrequency(&hires)) {
		QueryPerformanceCounter(&hires_now);
		time = hires_now.QuadPart * 1000;
		return time / hires.QuadPart;
	}
	return 0;
}

int64_t get_time_us()
{
	LARGE_INTEGER hires = { { 0 } };
	LARGE_INTEGER hires_now = { { 0 } };
	if (QueryPerformanceFrequency(&hires)) {
		if (!QueryPerformanceFrequency(&hires_now))
			return 0;

		return (hires_now.QuadPart * 1000000) / hires.QuadPart;
	}
	return 0;
}

int64_t get_time_delta(int64_t start)
{
	int64_t now = get_time_ms();
	return (now < start) ? ((TIME_WRAP_VALUE - start) + now)
			     : (now - start);
}

void sleep_ms(unsigned int ms)
{
	Sleep((DWORD)ms);
}

void sleep_s(unsigned int s)
{
	sleep_ms(s * 1000);
}
#endif