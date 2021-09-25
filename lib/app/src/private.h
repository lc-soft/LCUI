#include "config.h"
#include <app.h>

#ifdef APP_PLATFORM_LINUX

void fb_app_driver_init(app_driver_t *dirver);
void fb_app_window_driver_init(app_window_driver_t *driver);

#ifdef USE_LIBX11

void x11_app_driver_init(app_driver_t *dirver);
void x11_app_window_driver_init(app_window_driver_t *dirver);

#endif

#endif
