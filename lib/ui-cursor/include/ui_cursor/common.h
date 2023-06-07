#include "config.h"
#ifdef __cplusplus
#define LIBUI_CURSOR_BEGIN_DECLS extern "C" {
#define LIBUI_CURSOR_END_DECLS }
#else
#define LIBUI_CURSOR_BEGIN_DECLS
#define LIBUI_CURSOR_END_DECLS
#endif

#ifndef LIBUI_CURSOR_PUBLIC
#if defined(_MSC_VER) && !defined(LIBUI_CURSOR_STATIC_BUILD)
#ifdef LIBUI_CURSOR_DLL_EXPORT
#define LIBUI_CURSOR_PUBLIC __declspec(dllexport)
#else
#define LIBUI_CURSOR_PUBLIC __declspec(dllimport)
#endif
#elif __GNUC__ >= 4
#define LIBUI_CURSOR_PUBLIC extern __attribute__((visibility("default")))
#else
#define LIBUI_CURSOR_PUBLIC extern
#endif
#endif
