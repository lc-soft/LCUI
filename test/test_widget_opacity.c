#define PREVIEW_MODE
#include "./cases/test_widget_opacity.c"

static void OnOpacityPlus(LCUI_Widget w, LCUI_WidgetEvent e, void *arg)
{
	wchar_t str[32];
	float opacity = self.parent->computed_style.opacity - 0.1f;

	if (opacity < 0.1f) {
		opacity = 0.1f;
	}
	swprintf(str, 32, L"%.1f", opacity);
	Widget_SetOpacity(self.parent, opacity);
	TextView_SetTextW(self.text, str);
}

static void OnOpacityMinus(LCUI_Widget w, LCUI_WidgetEvent e, void *arg)
{
	wchar_t str[32];
	float opacity = self.parent->computed_style.opacity + 0.1f;

	if (opacity > 1.0f) {
		opacity = 1.0f;
	}
	swprintf(str, 32, L"%.1f", opacity);
	Widget_SetOpacity(self.parent, opacity);
	TextView_SetTextW(self.text, str);
}

int main(void)
{
	LCUI_Widget btn_plus, btn_minus;

	LCUI_Init();
	build();
	btn_plus = LCUIWidget_GetById("btn-plus");
	btn_minus = LCUIWidget_GetById("btn-minus");
	Widget_BindEvent(btn_plus, "click", OnOpacityPlus, NULL, NULL);
	Widget_BindEvent(btn_minus, "click", OnOpacityMinus, NULL, NULL);
	return LCUI_Main();
}
