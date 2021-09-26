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

int main(int argc, char **argv)
{
	int i;
	int64_t t0, t1, t2;
	int resx[] = { 480, 960, 1280, 1366, 1920, 2560, 3840 }, resy;
	char s_res[32], s_t0[32], s_t1[32];

	pd_canvas_t g_src, g_dst;
	pd_color_t t_color;

	pd_graph_init(&g_src);
	g_src.color_type = PD_COLOR_TYPE_ARGB;
	if (pd_graph_create(&g_src, 960, 540) < 0) {
		return -2;
	}
	t_color.value = 0xffaa5500;
	pd_graph_fill_rect(&g_src, t_color, NULL, false);
	Logger_Info("%-20s%-20s%s\n", "image size\\method", "pd_graph_zoom()",
		    "pd_graph_zoom_bilinear()");
	for (i = 0; i < sizeof(resx) / sizeof(int); i++) {
		resy = resx[i] * 9 / 16;
		t0 = LCUI_GetTime();
		pd_graph_init(&g_dst);
		pd_graph_zoom(&g_src, &g_dst, false, resx[i], resy);
		pd_graph_free(&g_dst);
		t1 = LCUI_GetTime();
		pd_graph_init(&g_dst);
		pd_graph_zoom_bilinear(&g_src, &g_dst, false, resx[i], resy);
		pd_graph_free(&g_dst);
		t2 = LCUI_GetTime();
		sprintf(s_res, "%dx%d", resx[i], resy);
		sprintf(s_t0, "%ldms", t1 - t0);
		sprintf(s_t1, "%ldms", t2 - t1);
		Logger_Info("%-20s%-20s%-20s\n", s_res, s_t0, s_t1);
	}
	pd_graph_free(&g_src);
	return 0;
}
