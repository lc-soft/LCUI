#ifndef YUTIL_KEYWORDS_H
#define YUTIL_KEYWORDS_H

// inline
#ifndef __cplusplus
#ifdef _MSC_VER
#if (_MSC_VER < 1900)
#undef inline
#define inline __inline
#endif
#endif
#endif

// extern c
#ifdef __cplusplus

#define Y_BEGIN_DECLS extern "C" {
#define Y_END_DECLS }
#else

#define Y_BEGIN_DECLS
#define Y_END_DECLS
#endif

#endif    // YUTIL_KEYWORDS_H