/*
 * lib/ptk/include/ptk/win32_main.h
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIBPLATFORM_INCLUDE_PLATFORM_MAIN_H
#define LIBPLATFORM_INCLUDE_PLATFORM_MAIN_H

#include <Windows.h>
#include <stdlib.h>
#include <ptk.h>

#ifdef PTK_WIN_DESKTOP

extern int main(int argc, char *argv[]);

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
        ptk_set_instance(hInstance);
        return main(__argc, __argv);
}

#endif
#endif
