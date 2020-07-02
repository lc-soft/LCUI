#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/settings.h>
#include <LCUI/main.h>
#include <LCUI/timer.h>
#include "test.h"
#include "libtest.h"

static int settings_change_count = 0;

static void on_settings_change(LCUI_SysEvent object, void *data)
{
	++settings_change_count;
}

static void check_settings_frame_rate_cap(void *arg)
{
	char str[256];
	int fps_limit = *((int *)arg);
	int fps = LCUI_GetFrameCount();

	sprintf(str, "should work when frame cap is %d (actual %d)", fps_limit,
		fps);
	it_b(str, fps <= fps_limit && fps > fps_limit - 10, TRUE);
	LCUI_Quit();
}

static void test_default_settings(void)
{
	LCUI_SettingsRec settings;

	LCUI_Init();
	LCUI_ResetSettings();
	Settings_Init(&settings);

	it_i("check default frame rate cap", settings.frame_rate_cap, 120);
	it_i("check default parallel rendering threads",
	     settings.parallel_rendering_threads, 4);
	it_b("check default record profile", settings.record_profile, FALSE);
	it_b("check default fps meter", settings.fps_meter, FALSE);
	it_b("check default paint flashing", settings.paint_flashing, FALSE);
	LCUI_Destroy();
}

static void test_apply_settings(void)
{
	LCUI_SettingsRec settings;

	LCUI_Init();
	int handler = LCUI_BindEvent(LCUI_SETTINGS_CHANGE, on_settings_change, NULL, NULL);

	settings.frame_rate_cap = 60;
	settings.parallel_rendering_threads = 2;
	settings.record_profile = TRUE;
	settings.fps_meter = TRUE;
	settings.paint_flashing = TRUE;

	LCUI_ApplySettings(&settings);
	Settings_Init(&settings);
	it_i("check frame rate cap", settings.frame_rate_cap, 60);
	it_i("check parallel rendering threads",
	     settings.parallel_rendering_threads, 2);
	it_b("check record profile", settings.record_profile, TRUE);
	it_b("check fps meter", settings.fps_meter, TRUE);
	it_b("check paint flashing", settings.paint_flashing, TRUE);

	it_i("check settings change count", settings_change_count, 1);

	settings.frame_rate_cap = -1;
	settings.parallel_rendering_threads = -1;

	LCUI_ApplySettings(&settings);
	Settings_Init(&settings);
	it_i("check frame rate cap minimum", settings.frame_rate_cap, 1);
	it_i("check parallel rendering threads minimum",
	     settings.parallel_rendering_threads, 1);
	it_i("check settings change count", settings_change_count, 2);

	LCUI_ResetSettings();
	it_i("check settings change count", settings_change_count, 3);
	LCUI_UnbindEvent(handler);
	LCUI_Destroy();
}

void test_settings_frame_rate_cap(void)
{
	LCUI_SettingsRec settings;
	LCUI_Init();
	Settings_Init(&settings);

	settings.frame_rate_cap = 30;
	LCUI_ApplySettings(&settings);
	LCUI_SetTimeout(1000, check_settings_frame_rate_cap,
			&settings.frame_rate_cap);
	LCUI_Main();

	LCUI_Init();
	settings.frame_rate_cap = 5;
	LCUI_ApplySettings(&settings);
	LCUI_SetTimeout(1000, check_settings_frame_rate_cap,
			&settings.frame_rate_cap);
	LCUI_Main();

	LCUI_Init();
	settings.frame_rate_cap = 90;
	LCUI_ApplySettings(&settings);
	LCUI_SetTimeout(1000, check_settings_frame_rate_cap,
			&settings.frame_rate_cap);
	LCUI_Main();

	LCUI_Init();
	settings.frame_rate_cap = 25;
	LCUI_ApplySettings(&settings);
	LCUI_SetTimeout(1000, check_settings_frame_rate_cap,
			&settings.frame_rate_cap);
	LCUI_Main();
}

void test_settings(void)
{
	describe("test default settings", test_default_settings);
	describe("test apply settings", test_apply_settings);
	describe("test settings.frame_rate_cap", test_settings_frame_rate_cap);
}
