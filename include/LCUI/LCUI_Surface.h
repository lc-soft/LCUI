#ifdef LCUI_BUILD_IN_WIN32

typedef struct LCUI_SurfaceRec_ {
	HWND hwnd;
	LCUI_Widget *target;
	LCUI_DirtyRectList rect;
} LCUI_Surface;

LCUI_API int LCUISurface_Init(void);

/** 新建一个 Surface */
LCUI_API LCUI_Surface *Surface_New(void);

LCUI_API void Surface_Move( LCUI_Surface *surface, int x, int y );

LCUI_API void Surface_Resize( LCUI_Surface *surface, int w, int h );

LCUI_API void LCUISurface_Loop(void);

LCUI_API void Surface_SetCaptionW( LCUI_Surface *surface, const wchar_t *str );

LCUI_API void Surface_Show( LCUI_Surface *surface );

#endif
