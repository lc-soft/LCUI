#ifndef LCUI_LINUX_DISPLAY_H
#define LCUI_LINUX_DISPLAY_H

#include <LCUI/platform/linux/linux_fbdisplay.h>
#include <LCUI/platform/linux/linux_x11display.h>

int LCUI_InitLinuxDisplay( LCUI_DisplayDriver driver );

int LCUI_ExitLinuxDisplay( void );

#endif
