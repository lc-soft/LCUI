#include "app.h"
#include "ime.h"
#include "events.h"
#include "clipboard.h"

int app_init(const wchar_t *name)
{
	if (app_init_engine(name) != 0) {
		return -1;
	}
	app_init_ime();
	app_init_events();
	clipboard_init();
	return 0;
}

void app_destroy(void)
{
	clipboard_destroy();
	app_destroy_events();
	app_destroy_ime();
	app_destroy_engine();
}
