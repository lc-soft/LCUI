#include "config.h"
#ifdef __cplusplus
#define LIBPLAT_BEGIN_DECLS extern "C" {
#define LIBPLAT_END_DECLS }
#else
#define LIBPLAT_BEGIN_DECLS
#define LIBPLAT_END_DECLS
#endif

#ifndef LIBPLAT_PUBLIC
#if defined(_MSC_VER) && !defined(LIBPLAT_STATIC_BUILD)
#ifdef LIBPLAT_DLL_EXPORT
#define LIBPLAT_PUBLIC __declspec(dllexport)
#else
#define LIBPLAT_PUBLIC __declspec(dllimport)
#endif
#elif __GNUC__ >= 4
#define LIBPLAT_PUBLIC extern __attribute__((visibility("default")))
#else
#define LIBPLAT_PUBLIC extern
#endif
#endif

#if defined(_WIN32) && !defined(__cplusplus)
#define LIBPLAT_INLINE __inline
#else
#define LIBPLAT_INLINE static inline
#endif

#ifdef _WIN32
	#define LIBPLAT_WIN32
	#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
	#define LIBPLAT_UWP
	#else
	#define LIBPLAT_WIN_DESKTOP
	#endif
#else
	#define LIBPLAT_LINUX
#endif
