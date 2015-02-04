
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
int Surface_SetRenderMode( LCUI_Surface surface, int mode );

#endif
