#include <string.h>
#include <assert.h>
#include <css/style_value.h>
#include <css/computed.h>
#include <ui.h>
#include <ui/mutation_observer.h>
#include "ui_widget.h"
#include "ui_widget_id.h"
#include "ui_widget_attributes.h"
#include "ui_widget_classes.h"
#include "ui_widget_prototype.h"
#include "ui_widget_status.h"
#include "ui_widget_observer.h"
#include "ui_widget_style.h"
#include "ui_events.h"

static void ui_widget_destroy_children(ui_widget_t* w);

static void ui_widget_init(ui_widget_t* w)
{
	memset(w, 0, sizeof(ui_widget_t));
	w->tab_index = -1;
	w->state = UI_WIDGET_STATE_CREATED;
	list_create(&w->children);
	list_create(&w->stacking_context);
	w->node.data = w;
	w->node_show.data = w;
	w->node.next = w->node.prev = NULL;
	w->node_show.next = w->node_show.prev = NULL;
}

void ui_widget_destroy(ui_widget_t* w)
{
	if (w->parent) {
		ui_widget_request_reflow(w->parent);
		ui_widget_unlink(w);
	}
	ui_widget_destroy_listeners(w);
	ui_widget_destroy_children(w);
	ui_widget_destroy_prototype(w);
	if (w->title) {
		free(w->title);
		w->title = NULL;
	}
	ui_widget_destroy_id(w);
	ui_widget_destroy_style(w);
	ui_widget_destroy_attrs(w);
	ui_widget_destroy_classes(w);
	ui_widget_destroy_status(w);
	free(w->extra);
	free(w);
}

static void ui_widget_on_destroy_child(void* arg)
{
	ui_widget_destroy(arg);
}

static void ui_widget_destroy_children(ui_widget_t* w)
{
	/* 先释放显示列表，后销毁部件列表，因为部件在这两个链表中的节点是和它共用
	 * 一块内存空间的，销毁部件列表会把部件释放掉，所以把这个操作放在后面 */
	list_destroy_without_node(&w->stacking_context, NULL);
	list_destroy_without_node(&w->children, ui_widget_on_destroy_child);
}

ui_widget_t* ui_create_widget(const char* type)
{
	ui_widget_t* widget = malloc(sizeof(ui_widget_t));

	ui_widget_init(widget);
	widget->proto = ui_get_widget_prototype(type);
	if (widget->proto->name) {
		widget->type = widget->proto->name;
	} else if (type) {
		widget->type = strdup2(type);
	}
	widget->proto->init(widget);
	ui_widget_request_refresh_style(widget);
	return widget;
}

ui_widget_t* ui_create_widget_with_prototype(const ui_widget_prototype_t* proto)
{
	ui_widget_t* widget = malloc(sizeof(ui_widget_t));

	ui_widget_init(widget);
	widget->proto = proto;
	widget->type = widget->proto->name;
	widget->proto->init(widget);
	ui_widget_request_refresh_style(widget);
	return widget;
}

void ui_widget_add_state(ui_widget_t* w, ui_widget_state_t state)
{
	/* 如果部件还处于未准备完毕的状态 */
	if (w->state < UI_WIDGET_STATE_READY) {
		w->state |= state;
		/* 如果部件已经准备完毕则触发 ready 事件 */
		if (w->state == UI_WIDGET_STATE_READY) {
			ui_event_t e = { 0 };
			e.type = UI_EVENT_READY;
			e.cancel_bubble = true;
			ui_widget_emit_event(w, e, NULL);
			w->state = UI_WIDGET_STATE_NORMAL;
		}
	}
}

void ui_widget_set_title(ui_widget_t* w, const wchar_t* title)
{
	size_t len;
	wchar_t *new_title, *old_title;
	ui_mutation_record_t* record;

	len = wcslen(title) + 1;
	new_title = (wchar_t*)malloc(sizeof(wchar_t) * len);
	if (!new_title) {
		return;
	}
	wcsncpy(new_title, title, len);
	old_title = w->title;
	w->title = new_title;
	if (old_title) {
		free(old_title);
	}
	if (ui_widget_has_observer(w, UI_MUTATION_RECORD_TYPE_PROPERTIES)) {
		record = ui_mutation_record_create(
		    w, UI_MUTATION_RECORD_TYPE_PROPERTIES);
		record->property_name = strdup2("title");
		ui_widget_add_mutation_record(w, record);
		ui_mutation_record_destroy(record);
	}
}

void ui_widget_set_text(ui_widget_t* w, const char* text)
{
	if (w->proto && w->proto->settext) {
		w->proto->settext(w, text);
	}
}

ui_widget_extra_data_t* ui_create_extra_data(ui_widget_t* widget)
{
	widget->extra = calloc(sizeof(ui_widget_extra_data_t), 1);
	if (widget->extra) {
		return widget->extra;
	}
	return NULL;
}

void ui_widget_get_offset(ui_widget_t* w, ui_widget_t* parent, float* offset_x,
			  float* offset_y)
{
	float x = 0, y = 0;
	while (w != parent) {
		x += w->border_box.x;
		y += w->border_box.y;
		w = w->parent;
		if (w) {
			x += w->padding_box.x - w->border_box.x;
			y += w->padding_box.y - w->border_box.y;
		} else {
			break;
		}
	}
	*offset_x = x;
	*offset_y = y;
}

bool ui_widget_in_viewport(ui_widget_t* w)
{
	list_node_t* node;
	ui_rect_t rect;
	ui_widget_t *self, *parent, *child;
	css_computed_style_t* style;

	rect = w->padding_box;
	/* If the size of the widget is not fixed, then set the maximum size to
	 * avoid it being judged invisible all the time. */
	if (rect.width < 1 && !IS_CSS_FIXED_LENGTH(&w->computed_style, width)) {
		rect.width = w->parent->padding_box.width;
	}
	if (rect.height < 1 && !IS_CSS_FIXED_LENGTH(&w->computed_style, height)) {
		rect.height = w->parent->padding_box.height;
	}
	for (self = w, parent = w->parent; parent;
	     self = parent, parent = parent->parent) {
		if (!ui_widget_is_visible(parent)) {
			return false;
		}
		for (node = self->node_show.prev; node && node->prev;
		     node = node->prev) {
			child = node->data;
			style = &child->computed_style;
			if (child->state < UI_WIDGET_STATE_LAYOUTED ||
			    child == self || !ui_widget_is_visible(child)) {
				continue;
			}
			DEBUG_MSG("rect: (%g,%g,%g,%g), child rect: "
				  "(%g,%g,%g,%g), child: %s %s\n",
				  rect.x, rect.y, rect.width, rect.height,
				  child->border_box.x, child->border_box.y,
				  child->border_box.width,
				  child->border_box.height, child->type,
				  child->id);
			if (!ui_rect_is_include(&child->border_box, &rect)) {
				continue;
			}
			if (style->opacity == 1.0f &&
			    css_color_alpha(style->background_color) == 255) {
				return false;
			}
		}
		rect.x += parent->padding_box.x;
		rect.y += parent->padding_box.y;
		ui_rect_correct(&rect, parent->padding_box.width,
				parent->padding_box.height);
		if (rect.width < 1 || rect.height < 1) {
			return false;
		}
	}
	return true;
}
