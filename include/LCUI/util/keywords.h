#ifndef YUTIL_KEYWORDS_H
#define YUTIL_KEYWORDS_H

#if defined(__GNUC__)
	#define YUTIL_API extern __attribute__((visibility("default")))
#else
	#ifdef YUTIL_EXPORTS
		#define YUTIL_API __declspec(dllexport)
	#else
		#define YUTIL_API __declspec(dllimport)
	#endif
#endif

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
