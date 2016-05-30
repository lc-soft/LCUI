
#ifndef LCUI_LIB_TIME_H
#define LCUI_LIB_TIME_H

LCUI_BEGIN_HEADER

LCUI_API void LCUI_StartTicks( void );

LCUI_API int64_t LCUI_GetTickCount( void );

LCUI_API int64_t LCUI_GetTicks( int64_t start_ticks );

LCUI_END_HEADER

#endif
