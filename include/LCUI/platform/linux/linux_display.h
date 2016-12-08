#ifndef LCUI_LINUX_DISPLAY_H
#define LCUI_LINUX_DISPLAY_H

#include <LCUI/platform/linux/linux_fbdisplay.h>
#include <LCUI/platform/linux/linux_x11display.h>

LCUI_DisplayDriver LCUI_CreateLinuxDisplayDriver( void );

void LCUI_DestroyLinuxDisplayDriver( LCUI_DisplayDriver driver );

#endif
