/*
 * clipboard.c -- Clipboard support
 *
 * Copyright (c) 2022, Liu chao <lc-soft@live.cn> All rights reserved.
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
#include "./internal.h"

#include <string.h>

static struct clipboard_module_t {
	char *text;
	size_t text_len;
} clipboard;

int clipboard_request_text(clipboard_callback_t callback, void *arg)
{
#ifdef HAVE_LIBX11
	if (app_get_id() == APP_ID_LINUX_X11) {
		return x11_clipboard_request_text(callback, arg);
	}
#endif
	size_t len = clipboard.text_len + 1;
	wchar_t *wstr = malloc(sizeof(wchar_t) * len);
	clipboard_t clipboard_data = { 0 };

	len = decode_utf8(wstr, clipboard.text, len);
	wstr[len] = 0;
	// Assign the data
	clipboard_data.text = wstr;
	clipboard_data.len = len;
	clipboard_data.image = NULL;
	callback(&clipboard_data, arg);
	free(wstr);
	return 0;
}

int clipboard_set_text(const wchar_t *text, size_t len)
{
#ifdef HAVE_LIBX11
	if (app_get_id() == APP_ID_LINUX_X11) {
		return x11_clipboard_set_text(text, len);
	}
#endif
	size_t raw_len = encode_utf8(NULL, text, 0);
	char *raw_text = malloc((raw_len + 1) * sizeof(char));
	raw_len = encode_utf8(raw_text, text, raw_len);

	if (clipboard.text) {
		free(clipboard.text);
	}
	raw_text[raw_len] = '\0';
	clipboard.text = raw_text;
	clipboard.text_len = raw_len;
	return 0;
}

void clipboard_init(void)
{
#ifdef HAVE_LIBX11
	if (app_get_id() == APP_ID_LINUX_X11) {
		x11_clipboard_init();
		return;
	}
#endif
}

void clipboard_destroy(void)
{
#ifdef HAVE_LIBX11
	if (app_get_id() == APP_ID_LINUX_X11) {
		x11_clipboard_destroy();
		return;
	}
#endif
	if (clipboard.text) {
		free(clipboard.text);
	}
	clipboard.text = NULL;
}
