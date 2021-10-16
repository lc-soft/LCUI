#include <LCUI/platform.h>
#include LCUI_CLIPBOARD_H

void LCUI_UseClipboard(void *widget, void *action)
{
	LCUI_UseClipboardImpl(widget, action);
}
