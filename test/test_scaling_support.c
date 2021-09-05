#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/timer.h>
#include <LCUI/display.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>

enum { TYPE_DENSITY, TYPE_SCALED_DENSITY, TYPE_SCALE };

static void OnButtonClick(ui_widget_t* w, ui_event_t* e, void *arg)
{
	int *data = e->data;
	switch (data[0]) {
	case TYPE_DENSITY:
		ui_set_density_level(data[1]);
		break;
	case TYPE_SCALED_DENSITY:
		ui_set_scaled_density_level(data[1]);
		break;
	case TYPE_SCALE:
		ui_set_scale(data[1] / 100.0f);
		break;
	}
	ui_refresh_style();
	LCUIDisplay_InvalidateArea(NULL);
}

static void SetButton(const char *id, int type, int level)
{
	int *data;
	ui_widget_t* btn;
	data = malloc(sizeof(int) * 2);
	data[0] = type;
	data[1] = level;
	btn = ui_get_widget(id);
	ui_widget_on(btn, "click", OnButtonClick, data, free);
}

static void InitButtons(void)
{
	SetButton("btn-density-small", TYPE_DENSITY, UI_DENSITY_LEVEL_SMALL);
	SetButton("btn-density-normal", TYPE_DENSITY, UI_DENSITY_LEVEL_NORMAL);
	SetButton("btn-density-large", TYPE_DENSITY, UI_DENSITY_LEVEL_LARGE);
	SetButton("btn-density-big", TYPE_DENSITY, UI_DENSITY_LEVEL_BIG);
	SetButton("btn-scaled-density-small", TYPE_SCALED_DENSITY,
		  UI_DENSITY_LEVEL_SMALL);
	SetButton("btn-scaled-density-normal", TYPE_SCALED_DENSITY,
		  UI_DENSITY_LEVEL_NORMAL);
	SetButton("btn-scaled-density-large", TYPE_SCALED_DENSITY,
		  UI_DENSITY_LEVEL_LARGE);
	SetButton("btn-scaled-density-big", TYPE_SCALED_DENSITY,
		  UI_DENSITY_LEVEL_BIG);
	SetButton("btn-scale-small", TYPE_SCALE, 75);
	SetButton("btn-scale-normal", TYPE_SCALE, 100);
	SetButton("btn-scale-large", TYPE_SCALE, 150);
	SetButton("btn-scale-big", TYPE_SCALE, 200);
}

int main(int argc, char **argv)
{
	ui_widget_t* root, pack;

	LCUI_Init();
	root = ui_root();
	pack = LCUIBuilder_LoadFile("test_scaling_support.xml");
	if (!pack) {
		return -1;
	}
	ui_widget_update_style(root, TRUE);
	ui_widget_append(root, pack);
	ui_widget_unwrap(pack);
	InitButtons();
	return LCUI_Main();
}
