
#include <LCUI.h>
#include "../include/ui.h"
#include "private.h"

void ui_widget_reflow(ui_widget_t* w, ui_layout_rule_t rule)
{
	ui_event_t ev = { 0 };

	switch (w->computed_style.display) {
	case SV_BLOCK:
	case SV_INLINE_BLOCK:
		ui_widget_update_block_layout(w, rule);
		break;
	case SV_FLEX:
		LCUIFlexBoxLayout_Reflow(w, rule);
		break;
	case SV_NONE:
	default:
		break;
	}
	ev.cancel_bubble = TRUE;
	ev.type = UI_EVENT_AFTERLAYOUT;
	ui_widget_emit_event(w, &ev, NULL);
	DEBUG_MSG("id: %s, type: %s, size: (%g, %g)\n", w->id, w->type,
		  w->width, w->height);
}

LCUI_BOOL ui_widget_auto_reflow(ui_widget_t* w, ui_layout_rule_t rule)
{
	float content_width = w->box.padding.width;
	float content_height = w->box.padding.height;
	ui_widget_layout_diff_t diff;

	ui_widget_begin_layout_diff(w, &diff);
	Widget_ComputeSizeStyle(w);
	ui_widget_update_box_size(w);
	ui_widget_update_box_position(w);
	ui_widget_add_state(w, LCUI_WSTATE_LAYOUTED);
	if (content_width == w->box.padding.width &&
	    content_height == w->box.padding.height) {
		return FALSE;
	}
	ui_widget_reflow(w, rule);
	ui_widget_end_layout_diff(w, &diff);
	w->task.states[UI_WIDGET_TASK_REFLOW] = FALSE;
	return TRUE;
}
