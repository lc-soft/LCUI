#ifndef LIBPLAT_INCLUDE_PLATFORM_APP_H
#define LIBPLAT_INCLUDE_PLATFORM_APP_H

#include "platform/types.h"
#include "platform/common.h"

LIBPLAT_BEGIN_DECLS

LIBPLAT_PUBLIC int open_uri(const char *uri);

LIBPLAT_PUBLIC void app_set_instance(void *instance);
LIBPLAT_PUBLIC app_id_t app_get_id(void);
LIBPLAT_PUBLIC void app_present(void);
LIBPLAT_PUBLIC int app_init(const wchar_t *name);
LIBPLAT_PUBLIC void app_exit(int exit_code);
LIBPLAT_PUBLIC void app_destroy(void);

LIBPLAT_END_DECLS

#endif
