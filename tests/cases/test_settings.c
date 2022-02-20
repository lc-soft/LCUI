#include <stdio.h>
#include <LCUI.h>
#include <LCUI/timer.h>
#include "ctest.h"

static void check_settings_frame_rate_cap(void *arg)
{
	char str[256];
	int fps_limit = *((int *)arg);
	int fps = (int)lcui_get_fps();

	sprintf(str, "should work when frame cap is %u (actual %u)", fps_limit,
		fps);
	it_b(str, fps <= fps_limit + 2 && fps > fps_limit / 2, TRUE);
	lcui_quit();
}

static void test_default_settings(void)
{
	lcui_settings_t settings;

	lcui_init();
	lcui_reset_settings();
	lcui_get_settings(&settings);

	it_i("check default frame rate cap", settings.frame_rate_cap, 120);
	it_i("check default parallel rendering threads",
	     settings.parallel_rendering_threads, 4);
	it_b("check default record profile", settings.record_profile, FALSE);
	it_b("check default fps meter", settings.fps_meter, FALSE);
	it_b("check default paint flashing", settings.paint_flashing, FALSE);
	lcui_quit();
	lcui_main();
}

static void test_apply_settings(void)
{
	lcui_settings_t settings;

	settings.frame_rate_cap = 60;
	settings.parallel_rendering_threads = 2;
	settings.record_profile = TRUE;
	settings.fps_meter = TRUE;
	settings.paint_flashing = TRUE;

	lcui_init();
	lcui_apply_settings(&settings);
	lcui_get_settings(&settings);
	it_i("check frame rate cap", settings.frame_rate_cap, 60);
	it_i("check parallel rendering threads",
	     settings.parallel_rendering_threads, 2);
	it_b("check record profile", settings.record_profile, TRUE);
	it_b("check fps meter", settings.fps_meter, TRUE);
	it_b("check paint flashing", settings.paint_flashing, TRUE);

	settings.frame_rate_cap = -1;
	settings.parallel_rendering_threads = -1;

	lcui_apply_settings(&settings);
	lcui_get_settings(&settings);

	it_i("check frame rate cap minimum", settings.frame_rate_cap, 1);
	it_i("check parallel rendering threads minimum",
	     settings.parallel_rendering_threads, 1);

	lcui_reset_settings();

	lcui_get_settings(&settings);
	it_i("check frame rate cap", settings.frame_rate_cap, LCUI_MAX_FRAMES_PER_SEC);
	lcui_quit();
	lcui_main();
}

void test_settings_frame_rate_cap(void)
{
	lcui_settings_t settings;

	lcui_init();
	lcui_get_settings(&settings);

	settings.frame_rate_cap = 30;
	lcui_apply_settings(&settings);
	lcui_set_timeout(2000, check_settings_frame_rate_cap,
			&settings.frame_rate_cap);
	lcui_main();

	lcui_init();
	settings.frame_rate_cap = 5;
	lcui_apply_settings(&settings);
	lcui_set_timeout(2000, check_settings_frame_rate_cap,
			&settings.frame_rate_cap);
	lcui_main();

	lcui_init();
	settings.frame_rate_cap = 90;
	lcui_apply_settings(&settings);
	lcui_set_timeout(2000, check_settings_frame_rate_cap,
			&settings.frame_rate_cap);
	lcui_main();

	lcui_init();
	settings.frame_rate_cap = 25;
	lcui_apply_settings(&settings);
	lcui_set_timeout(2000, check_settings_frame_rate_cap,
			&settings.frame_rate_cap);
	lcui_main();
}

void test_settings(void)
{
	describe("test default settings", test_default_settings);
	describe("test apply settings", test_apply_settings);
	describe("test settings.frame_rate_cap", test_settings_frame_rate_cap);
}
