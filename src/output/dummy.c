#include "config.h"
#include <LCUI_Build.h>
#include LC_LCUI_H

#ifdef GRAPH_OUTPUT_USE_DUMMY
void Fill_Pixel(LCUI_Pos pos, LCUI_RGB color)
{
	return;
}

int Get_Screen_Graph(LCUI_Graph *out)
{
	return -1;
}

int Screen_Init()
{
	return -1;
}

int Graph_Display (LCUI_Graph * src, LCUI_Pos pos)
{
	return -1;
}
#endif
