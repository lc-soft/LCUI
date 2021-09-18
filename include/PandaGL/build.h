// Copyright to be defined

#ifndef PANDAGL_INCLUDE_PANDAGL_BUILD_H_
#define PANDAGL_INCLUDE_PANDAGL_BUILD_H_


#define PD_API __declspec(dllexport)
// #if defined(__GNUC__)
// #define PD_API extern
// #else
// #ifdef PD_EXPORTS
// #define PD_API __declspec(dllexport)
// #else
// #define PD_API __declspec(dllimport)
// #endif
// #endif

#if defined(_WIN32) && !defined(__cplusplus)
#define INLINE __inline
#else
#define INLINE static inline
#endif

#endif // !PANDAGL_INCLUDE_PANDAGL_BUILD_H_
