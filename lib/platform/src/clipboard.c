/*
 * lib/platform/src/clipboard.c: -- Clipboard support
 *
 * Copyright (c) 2022-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include "app.h"
#include "clipboard.h"

static struct clipboard_module_t {
	char *text;
	size_t text_len;
} clipboard;

int clipboard_request_text(clipboard_callback_t callback, void *arg)
{
#ifdef LIBPLAT_HAS_LIBX11
	if (app_get_id() == APP_ID_LINUX_X11) {
		return x11_clipboard_request_text(callback, arg);
	}
#endif
	size_t len = clipboard.text_len + 1;
	wchar_t *wstr = malloc(sizeof(wchar_t) * len);
	clipboard_t clipboard_data = { 0 };

	len = mbstowcs(wstr, clipboard.text, len);
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
#ifdef LIBPLAT_HAS_LIBX11
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
#ifdef LIBPLAT_HAS_LIBX11
	if (app_get_id() == APP_ID_LINUX_X11) {
		x11_clipboard_init();
		return;
	}
#endif
}

void clipboard_destroy(void)
{
#ifdef LIBPLAT_HAS_LIBX11
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
