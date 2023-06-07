#include "config.h"
#ifdef __cplusplus
#define LIBWORKER_BEGIN_DECLS extern "C" {
#define LIBWORKER_END_DECLS }
#else
#define LIBWORKER_BEGIN_DECLS
#define LIBWORKER_END_DECLS
#endif

#ifndef LIBWORKER_PUBLIC
#if defined(_MSC_VER) && !defined(LIBWORKER_STATIC_BUILD)
#ifdef LIBWORKER_DLL_EXPORT
#define LIBWORKER_PUBLIC __declspec(dllexport)
#else
#define LIBWORKER_PUBLIC __declspec(dllimport)
#endif
#elif __GNUC__ >= 4
#define LIBWORKER_PUBLIC extern __attribute__((visibility("default")))
#else
#define LIBWORKER_PUBLIC extern
#endif
#endif
