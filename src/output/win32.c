#include <LCUI_Build.h>
#include LC_LCUI_H

#ifdef LCUI_VIDEO_DRIVER_WIN32
void LCUIScreen_FillPixel( LCUI_Pos pos, LCUI_RGB color )
{
	return;
}

int LCUIScreen_GetGraph( LCUI_Graph *out )
{
	return -1;
}

int LCUIScreen_Init( void )
{
	return -1;
}

int LCUIScreen_Destroy( void )
{
	return -1;
}

int LCUIScreen_PutGraph (LCUI_Graph * src, LCUI_Pos pos)
{
	return -1;
}
#endif
