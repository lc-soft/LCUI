/*
 * lib/ptk/src/clipboard.c: -- Clipboard support
 *
 * Copyright (c) 2022-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <yutil.h>
#include <stdlib.h>
#include <string.h>
#include "ptk/app.h"
#include "linux/x11clipboard.h"

static struct ptk_clipboard_module_t {
	char *text;
	size_t text_len;
} ptk_clipboard;

int ptk_clipboard_request_text(ptk_clipboard_callback_t callback, void *arg)
{
#ifdef PTK_HAS_LIBX11
	if (ptk_get_app_id() == PTK_APP_ID_LINUX_X11) {
		return ptk_x11clipboard_request_text(callback, arg);
	}
#endif
	size_t len = ptk_clipboard.text_len + 1;
	wchar_t *wstr = malloc(sizeof(wchar_t) * len);
	ptk_clipboard_t clipboard = { 0 };

	len = decode_utf8(wstr, ptk_clipboard.text, len);
	wstr[len] = 0;
	// Assign the data
	clipboard.text = wstr;
	clipboard.len = len;
	clipboard.image = NULL;
	callback(&clipboard, arg);
	free(wstr);
	return 0;
}

int ptk_clipboard_set_text(const wchar_t *text, size_t len)
{
#ifdef PTK_HAS_LIBX11
	if (ptk_get_app_id() == PTK_APP_ID_LINUX_X11) {
		return ptk_x11clipboard_set_text(text, len);
	}
#endif
	size_t raw_len = encode_utf8(NULL, text, 0);
	char *raw_text = malloc((raw_len + 1) * sizeof(char));
	raw_len = encode_utf8(raw_text, text, raw_len);

	if (ptk_clipboard.text) {
		free(ptk_clipboard.text);
	}
	raw_text[raw_len] = '\0';
	ptk_clipboard.text = raw_text;
	ptk_clipboard.text_len = raw_len;
	return 0;
}

void ptk_clipboard_init(void)
{
#ifdef PTK_HAS_LIBX11
	if (ptk_get_app_id() == PTK_APP_ID_LINUX_X11) {
		ptk_x11clipboard_init();
		return;
	}
#endif
}

void ptk_clipboard_destroy(void)
{
#ifdef PTK_HAS_LIBX11
	if (ptk_get_app_id() == PTK_APP_ID_LINUX_X11) {
		ptk_x11clipboard_destroy();
		return;
	}
#endif
	if (ptk_clipboard.text) {
		free(ptk_clipboard.text);
	}
	ptk_clipboard.text = NULL;
}
