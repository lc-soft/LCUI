
#ifndef __LCUI_LIB_TIME_H__
#define __LCUI_LIB_TIME_H__

LCUI_BEGIN_HEADER

LCUI_API void LCUI_StartTicks( void );

LCUI_API int64_t LCUI_GetTickCount( void );

LCUI_API int64_t LCUI_GetTicks( int64_t start_ticks );

LCUI_END_HEADER

#endif
