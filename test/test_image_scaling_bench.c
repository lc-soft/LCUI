#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/timer.h>
#include <LCUI/display.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>

#ifdef LCUI_BUILD_IN_WIN32
static void LoggerHandler(const char *str)
{
	OutputDebugStringA(str);
}

static void LoggerHandlerW(const wchar_t *str)
{
	OutputDebugStringW(str);
}
#endif

int main(int argc, char **argv)
{
	int i;
	int64_t t0, t1, t2;
	int resx[] = { 480, 960, 1280, 1366, 1920, 2560, 3840 }, resy;
	char s_res[32], s_t0[32], s_t1[32];
	
	LCUI_Graph g_src, g_dst;
	LCUI_Color t_color;

#ifdef LCUI_BUILD_IN_WIN32
	Logger_SetHandler(LoggerHandler);
	Logger_SetHandlerW(LoggerHandlerW);
#endif
	Graph_Init(&g_src);
	g_src.color_type = LCUI_COLOR_TYPE_ARGB;
	if (Graph_Create(&g_src, 960, 540) < 0) {
		return -2;
	}
	t_color.value = 0xffaa5500;
	Graph_FillRect(&g_src, t_color, NULL, false);
	LOG("%-20s%-20s%s\n", "image size\\method", "Graph_Zoom()",
	    "Graph_ZoomBilinear()");
	for (i = 0; i < sizeof(resx) / sizeof(int); i++) {
		resy = resx[i] * 9 / 16;
		t0 = LCUI_GetTime();
		Graph_Init(&g_dst);
		Graph_Zoom(&g_src, &g_dst, false, resx[i], resy);
		Graph_Free(&g_dst);
		t1 = LCUI_GetTime();
		Graph_Init(&g_dst);
		Graph_ZoomBilinear(&g_src, &g_dst, false, resx[i], resy);
		Graph_Free(&g_dst);
		t2 = LCUI_GetTime();
		sprintf(s_res, "%dx%d", resx[i], resy);
		sprintf(s_t0, "%ldms", t1 - t0);
		sprintf(s_t1, "%ldms", t2 - t1);
		LOG("%-20s%-20s%-20s\n", s_res, s_t0, s_t1);
	}
	Graph_Free(&g_src);
	return 0;
}
