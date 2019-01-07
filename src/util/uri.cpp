/*
 * uri.c -- uri processing for UWP
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

#include <wrl.h>
#include <wrl/client.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/util/uri.h>
#include <LCUI/util/charset.h>

#if defined(LCUI_BUILD_IN_WIN32) && defined(WINAPI_PARTITION_APP)
int OpenUri(const char *uristr)
{
	size_t len;
	wchar_t *wuri;

	len = LCUI_DecodeString(NULL, uristr, 0, ENCODING_ANSI);
	if (len < 1) {
		return -1;
	}
	wuri = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	if (!wuri) {
		return -2;
	}
	LCUI_DecodeString(wuri, uristr, len, ENCODING_ANSI);
	wuri[len] = 0;

	auto str = ref new Platform::String(wuri);
	auto uri = ref new Windows::Foundation::Uri(str);
	Windows::System::Launcher::LaunchUriAsync(uri);
	free(wuri);
	return 0;
}
#endif
