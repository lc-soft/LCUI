#include "config.h"
#ifdef __cplusplus
#define LIBUI_WIDGETS_BEGIN_DECLS extern "C" {
#define LIBUI_WIDGETS_END_DECLS }
#else
#define LIBUI_WIDGETS_BEGIN_DECLS
#define LIBUI_WIDGETS_END_DECLS
#endif

#ifndef LIBUI_WIDGETS_PUBLIC
#if defined(_MSC_VER) && !defined(LIBUI_WIDGETS_STATIC_BUILD)
#ifdef LIBUI_WIDGETS_DLL_EXPORT
#define LIBUI_WIDGETS_PUBLIC __declspec(dllexport)
#else
#define LIBUI_WIDGETS_PUBLIC __declspec(dllimport)
#endif
#elif __GNUC__ >= 4
#define LIBUI_WIDGETS_PUBLIC extern __attribute__((visibility("default")))
#else
#define LIBUI_WIDGETS_PUBLIC extern
#endif
#endif
