#ifndef __LCUI_PLATFORM_H__
#define __LCUI_PLATFORM_H__

#ifdef LCUI_BUILD_IN_WIN32
#define LCUI_InitApp LCUI_InitWinApp
#define LCUI_ExitApp LCUI_ExitWinApp
#define LCUI_PreInitApp LCUI_PreInitWinApp
#define LCUI_InitDisplayDriver LCUI_InitWinDisplay
#define LCUI_ExitDisplayDriver LCUI_ExitWinDisplay
#define LCUI_InitMouseDriver LCUI_InitWinMouse
#define LCUI_ExitMouseDriver LCUI_ExitWinMouse
#define LCUI_InitKeyboardDriver LCUI_InitWinKeyboard
#define LCUI_ExitKeyboardDriver LCUI_ExitWinKeyboard
#define LCUI_EVENTS_H	<LCUI/platform/windows/windows_events.h>
#define LCUI_MOUSE_H	<LCUI/platform/windows/windows_mouse.h>
#define LCUI_KEYBOARD_H	<LCUI/platform/windows/windows_keyboard.h>
#define LCUI_DISPLAY_H	<LCUI/platform/windows/windows_display.h>
#elif defined(LCUI_BUILD_IN_LINUX)
#define LCUI_InitApp LCUI_InitLinuxApp
#define LCUI_ExitApp LCUI_ExitLinuxApp
#define LCUI_PreInitApp LCUI_PreInitLinuxApp
#define LCUI_InitDisplayDriver LCUI_InitLinuxDisplay
#define LCUI_ExitDisplayDriver LCUI_ExitLinuxDisplay
#define LCUI_InitMouseDriver LCUI_InitLinuxMouse
#define LCUI_ExitMouseDriver LCUI_ExitLinuxMouse
#define LCUI_InitKeyboardDriver LCUI_InitLinuxKeyboard
#define LCUI_ExitKeyboardDriver LCUI_ExitLinuxKeyboard
#ifdef linux
#undef linux
#endif
#define LCUI_EVENTS_H	<LCUI/platform/linux/linux_events.h>
#define LCUI_MOUSE_H	<LCUI/platform/linux/linux_mouse.h>
#define LCUI_KEYBOARD_H	<LCUI/platform/linux/linux_keyboard.h>
#define LCUI_DISPLAY_H	<LCUI/platform/linux/linux_display.h>
#else
#error current platform is not supported.
#endif

#endif
