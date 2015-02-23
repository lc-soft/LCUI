
#ifdef LCUI_BUILD_IN_WIN32

#define RENDER_MODE_BIT_BLT	0
#define RENDER_MODE_STRETCH_BLT 1

#ifdef __IN_SURFACE_SOURCE_FILE__
typedef struct LCUI_SurfaceRec_ * LCUI_Surface;
#else
typedef void* LCUI_Surface;
#endif

LCUI_API int LCUISurface_Init(void);

/** 删除 Surface */
LCUI_API void Surface_Delete( LCUI_Surface surface );

/** 新建一个 Surface */
LCUI_API LCUI_Surface Surface_New(void);

LCUI_API void Surface_Move( LCUI_Surface surface, int x, int y );

LCUI_API void Surface_Resize( LCUI_Surface surface, int w, int h );

LCUI_API void Surface_SetCaptionW( LCUI_Surface surface, const wchar_t *str );

LCUI_API void Surface_Show( LCUI_Surface surface );

LCUI_API void Surface_Hide( LCUI_Surface surface );

/** 设置 Surface 的渲染模式 */
LCUI_API void Surface_SetRenderMode( LCUI_Surface surface, int mode );

/** 
 * 准备绘制 Surface 中的内容
 * @param[in] surface	目标 surface
 * @param[in] rect	需进行绘制的区域，若为NULL，则绘制整个 surface
 * @return		返回绘制上下文句柄
 */
LCUI_API LCUI_PaintContext Surface_BeginPaint( LCUI_Surface surface, LCUI_Rect *rect );

/** 
 * 结束对 Surface 的绘制操作
 * @param[in] surface	目标 surface
 * @param[in] paint_ctx	绘制上下文句柄
 */
LCUI_API void Surface_EndPaint( LCUI_Surface surface, LCUI_PaintContext paint_ctx );

/** 将帧缓存中的数据呈现至Surface的窗口内 */
LCUI_API void Surface_Present( LCUI_Surface surface );

#endif
