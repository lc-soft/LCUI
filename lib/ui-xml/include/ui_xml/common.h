#include "config.h"
#ifdef __cplusplus
#define LIBUI_XML_BEGIN_DECLS extern "C" {
#define LIBUI_XML_END_DECLS }
#else
#define LIBUI_XML_BEGIN_DECLS
#define LIBUI_XML_END_DECLS
#endif

#ifndef LIBUI_XML_PUBLIC
#if defined(_MSC_VER) && !defined(LIBUI_XML_STATIC_BUILD)
#ifdef LIBUI_XML_DLL_EXPORT
#define LIBUI_XML_PUBLIC __declspec(dllexport)
#else
#define LIBUI_XML_PUBLIC __declspec(dllimport)
#endif
#elif __GNUC__ >= 4
#define LIBUI_XML_PUBLIC extern __attribute__((visibility("default")))
#else
#define LIBUI_XML_PUBLIC extern
#endif
#endif
