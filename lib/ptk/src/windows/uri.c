/*
 * lib/ptk/src/windows/uri.c: -- uri processing
 *
 * Copyright (c) 2018-2025, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ptk/app.h"

#if defined(PTK_WIN32) && !defined(WINAPI_PARTITION_APP)
#pragma warning(disable:4091)
#include <Windows.h>
#include <ShlObj.h>

int ptk_open_uri(const char *uri)
{
	ShellExecuteA(NULL, "open", uri, NULL, NULL, SW_SHOW);
	return 0;
}

#else

int ptk_open_uri(const char *uri)
{
	return -1;
}
#endif
