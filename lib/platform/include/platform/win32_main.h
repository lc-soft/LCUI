/*
 * lib/platform/include/platform/win32_main.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIBPLATFORM_INCLUDE_PLATFORM_MAIN_H
#define LIBPLATFORM_INCLUDE_PLATFORM_MAIN_H

#include <Windows.h>
#include <platform.h>

#ifdef LIBPLAT_WIN_DESKTOP
#include <yutil.h>
#include <errno.h>

extern int main(int argc, char *argv[]);

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
		     _In_opt_ HINSTANCE hPrevInstance,
		     _In_ LPSTR lpCmdLine,
		     _In_ int nCmdShow)
{
	char *cmdline;
	int ret, argc = 0;
	char **argv = NULL;
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	app_set_instance(hInstance);
	cmdline = GetCommandLineA();
	argc = cmdsplit(cmdline, &argv);
	ret = main(argc, (char**)argv);
	while (argc-- > 0) {
		free(argv[argc]);
	}
	free(argv);
	return ret;
}

#endif
#endif
