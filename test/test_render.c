#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <LCUI.h>
#include <LCUI/main.h>
#include <LCUI/graph.h>
#include <LCUI/gui/widget/textview.h>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900
#define BLOCK_COUNT 60

static struct TestStatus {
	size_t color_index;
	unsigned int fps;
	ui_widget_t* box;
} self;

void UpdateWidgetStyle(ui_widget_t* w, void *arg)
{
	size_t index;
	pd_color_t color;

	index = w->index % BLOCK_COUNT + w->index / BLOCK_COUNT;
	index = ((index + self.color_index) * 256 / BLOCK_COUNT) % 512;
	color.red = 255;
	color.green = (unsigned char)(index > 255 ? 511 - index : index);
	color.blue = 0;
	color.alpha = 255;
	ui_widget_set_style(w, css_key_background_color, color, color);
	ui_widget_update_style(w);
}

void UpdateFrame(void *arg)
{
	ui_widget_each(arg, UpdateWidgetStyle, NULL);
}

void UpdateRenderStatus(void *arg)
{
	char str[32];

	sprintf(str, "[size=24px]FPS: %d[/size]", self.fps);
	TextView_SetText(arg, str);
	self.fps = 0;
}

void InitModal(void)
{
	ui_widget_t* dimmer;
	ui_widget_t* dialog;

	dimmer = ui_create_widget(NULL);
	dialog = ui_create_widget(NULL);
	ui_widget_resize(dialog, 400, 400);
	ui_widget_set_style_string(dialog, "margin", "100px auto");
	ui_widget_set_style_string(dialog, "border-radius", "6px");
	ui_widget_set_style_string(dialog, "opacity", "0.9");
	ui_widget_set_box_shadow(dialog, 0, 4, 8, ARGB(100, 0, 0, 0));
	ui_widget_set_style_string(dialog, "background-color", "#fff");

	ui_widget_set_style(dimmer, css_key_top, 0, px);
	ui_widget_set_style(dimmer, css_key_left, 0, px);
	ui_widget_set_style(dimmer, css_key_width, 1.0f, scale);
	ui_widget_set_style(dimmer, css_key_height, 1.0f, scale);
	ui_widget_set_style(dimmer, css_key_position, CSS_KEYWORD_ABSOLUTE, style);
	ui_widget_set_style_string(dimmer, "background-color", "rgba(0,0,0,0.5)");

	ui_widget_append(dimmer, dialog);
	ui_widget_append(ui_root(), dimmer);
}

void InitBackground(void)
{
	size_t i;
	size_t c;
	size_t n = BLOCK_COUNT;
	ui_widget_t* w;
	ui_widget_t* root;
	pd_color_t color;
	ui_widget_rules_t rules = { 0 };
	const float width = SCREEN_WIDTH * 1.0f / n;
	const float height = SCREEN_HEIGHT * 1.0f / n;

	color.red = 255;
	color.green = 0;
	color.blue = 0;
	color.alpha = 255;
	root = ui_root();
	self.box = ui_create_widget(NULL);
	for (i = 0; i < n * n; ++i) {
		if (i % n == 0) {
			++self.color_index;
		}
		w = ui_create_widget(NULL);
		c = ((i % n + i / n) * 256 / n) % 512;
		color.green = (unsigned char)(c > 255 ? 511 - c : c);
		ui_widget_resize(w, width, height);
		ui_widget_set_style(w, css_key_display, CSS_KEYWORD_INLINE_BLOCK, style);
		ui_widget_set_style(w, css_key_background_color, color, color);
		ui_widget_append(self.box, w);
	}
	rules.cache_children_style = TRUE;
	rules.ignore_classes_change = TRUE;
	rules.ignore_status_change = TRUE;
	rules.max_update_children_count = -1;
	rules.max_render_children_count = 0;
	ui_widget_generate_hash(self.box);
	ui_widget_set_rules(self.box, &rules);
	ui_widget_append(root, self.box);
}

void InitRenderStatus(void)
{
	ui_widget_t* root;
	ui_widget_t* status;
	pd_color_t white = RGB(255, 255, 255);
	pd_color_t black = RGB(0, 0, 0);

	root = ui_root();
	status = ui_create_widget("textview");
	ui_widget_set_style(status, css_key_top, 10, px);
	ui_widget_set_style(status, css_key_right, 10, px);
	ui_widget_set_style(status, css_key_position, CSS_KEYWORD_ABSOLUTE, style);
	ui_widget_set_style(status, css_key_background_color, black, color);
	ui_widget_set_padding(status, 10, 15, 10, 15);
	TextView_SetColor(status, white);
	ui_widget_append(root, status);
	UpdateRenderStatus(status);
	lcui_set_interval(1000, UpdateRenderStatus, status);
}

int main(int argc, char *argv[])
{
	size_t i;
	int64_t t;

	logger_set_level(LOGGER_LEVEL_WARNING);
	lcui_init();
	ui_widget_resize(ui_root(), SCREEN_WIDTH, SCREEN_HEIGHT);
	InitBackground();
	InitModal();
	InitRenderStatus();
	printf("running rendering performance test\n");
	t = get_time_ms();
	self.color_index = 0;
	for (i = 0; i < 600; ++i) {
		UpdateFrame(self.box);
		lcui_process_timers();
		lcui_update_ui();
		lcui_render_ui();
		app_present();
		++self.fps;
		++self.color_index;
	}
	t = get_time_delta(t);
	logger_warning(
	    "rendered %zu frames in %.2lfs, rendering speed is %.2lf fps\n", i,
	    t / 1000.f, i * 1000.f / t);
	return 0;
}
