#include <LCUI/platform.h>
#include LCUI_CLIPBOARD_H

void LCUI_UseClipboard(void *widget, void *action)
{
	LCUI_UseClipboardImpl(widget, action);
}

// @WhoAteDaCake
// TODO: Could this be done via #define instead?
void LCUI_CopyToClipboard(void *text)
{
	LCUI_CopyToClipboardImpl(text);
}
