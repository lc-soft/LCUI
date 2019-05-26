#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/gui/widget/textedit.h>

static void OnBtnClick(LCUI_Widget self, LCUI_WidgetEvent e, void *arg)
{
	wchar_t str[256];
	LCUI_Widget edit = LCUIWidget_GetById("edit");
	LCUI_Widget txt = LCUIWidget_GetById("text-hello");

	TextEdit_GetTextW(edit, 0, 255, str);
	TextView_SetTextW(txt, str);
}

int main(int argc, char **argv)
{
	LCUI_Widget root, pack, btn;

	LCUI_Init();
	root = LCUIWidget_GetRoot();
	pack = LCUIBuilder_LoadFile("helloworld.xml");
	if (!pack) {
		return -1;
	}
	Widget_Append(root, pack);
	Widget_Unwrap(pack);
	btn = LCUIWidget_GetById("btn");
	Widget_BindEvent(btn, "click", OnBtnClick, NULL, NULL);
	return LCUI_Main();
}
