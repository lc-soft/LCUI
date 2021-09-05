#include <LCUI.h>
#include "../include/ui.h"
#include "private.h"

typedef struct ui_updater_rules_data_t {
	ui_widget_rules_t rules;
	Dict* style_cache;
	size_t default_max_update_count;
	size_t progress;
} ui_updater_rules_data_t;

typedef struct ui_updater_profile_t ui_updater_profile_t;
struct ui_updater_profile_t {
	unsigned style_hash;
	Dict* style_cache;
	ui_widget_style_diff_t style_diff;
	ui_widget_layout_diff_t layout_diff;
	ui_updater_profile_t* parent;
	ui_profile_t* profile;
};

static struct ui_updater_t {
	DictType style_cache_dict;
	ui_metrics_t metrics;
	LCUI_BOOL refresh_all;
	ui_widget_function_t handlers[UI_TASK_TOTAL_NUM];
} ui_updater;

static size_t ui_widget_update_with_context(ui_widget_t* w,
					    ui_updater_profile_t* ctx);

static unsigned int ui_style_dict_hash(const void* key)
{
	return Dict_IdentityHashFunction(*(unsigned int*)key);
}

static int ui_style_dict_key_compare(void* privdata, const void* key1,
				 const void* key2)
{
	return *(unsigned int*)key1 == *(unsigned int*)key2;
}

static void ui_style_dict_key_free(void* privdata, void* key)
{
	free(key);
}

static void* ui_style_dict_key_dup(void* privdata, const void* key)
{
	unsigned int* newkey = malloc(sizeof(unsigned int));
	*newkey = *(unsigned int*)key;
	return newkey;
}

static void ui_style_dict_val_free(void* privdata, void* val)
{
	StyleSheet_Delete(val);
}

static void ui_widget_on_refresh_style(ui_widget_t* w)
{
	int i;

	ui_widget_force_refresh_style(w);
	for (i = UI_TASK_UPDATE_STYLE + 1; i < UI_TASK_REFLOW; ++i) {
		w->update.states[i] = TRUE;
	}
	w->update.states[UI_TASK_UPDATE_STYLE] = FALSE;
}

static void ui_widget_on_update_style(ui_widget_t* w)
{
	ui_widget_force_update_style(w);
}

static void ui_widget_on_set_title(ui_widget_t* w)
{
	ui_widget_post_surface_event(w, UI_EVENT_TITLE, TRUE);
}

void ui_widget_add_task_for_children(ui_widget_t* widget, ui_task_type_t task)
{
	ui_widget_t* child;
	LinkedListNode* node;

	widget->update.for_children = TRUE;
	for (LinkedList_Each(node, &widget->children)) {
		child = node->data;
		ui_widget_add_task(child, task);
		ui_widget_add_task_for_children(child, task);
	}
}

void ui_widget_add_task(ui_widget_t* widget, int task)
{
	if (widget->state == LCUI_WSTATE_DELETED) {
		return;
	}
	DEBUG_MSG("[%lu] %s, %d\n", widget->index, widget->type, task);
	widget->update.for_self = TRUE;
	widget->update.states[task] = TRUE;
	widget = widget->parent;
	/* 向没有标记的父级部件添加标记 */
	while (widget && !widget->update.for_children) {
		widget->update.for_children = TRUE;
		widget = widget->parent;
	}
}

LCUI_BOOL ui_widget_in_viewport(ui_widget_t* w)
{
	LinkedListNode* node;
	LCUI_RectF rect;
	ui_widget_t *self, *parent, *child;
	ui_widget_style_t* style;

	rect = w->box.padding;
	/* If the size of the widget is not fixed, then set the maximum size to
	 * avoid it being judged invisible all the time. */
	if (rect.width < 1 && ui_widget_has_auto_style(w, key_width)) {
		rect.width = w->parent->box.padding.width;
	}
	if (rect.height < 1 && ui_widget_has_auto_style(w, key_height)) {
		rect.height = w->parent->box.padding.height;
	}
	for (self = w, parent = w->parent; parent;
	     self = parent, parent = parent->parent) {
		if (!Widget_IsVisible(parent)) {
			return FALSE;
		}
		for (node = self->node_show.prev; node && node->prev;
		     node = node->prev) {
			child = node->data;
			style = &child->computed_style;
			if (child->state < LCUI_WSTATE_LAYOUTED ||
			    child == self || !Widget_IsVisible(child)) {
				continue;
			}
			DEBUG_MSG("rect: (%g,%g,%g,%g), child rect: "
				  "(%g,%g,%g,%g), child: %s %s\n",
				  rect.x, rect.y, rect.width, rect.height,
				  child->box.border.x, child->box.border.y,
				  child->box.border.width,
				  child->box.border.height, child->type,
				  child->id);
			if (!LCUIRectF_IsIncludeRect(&child->box.border,
						     &rect)) {
				continue;
			}
			if (style->opacity == 1.0f &&
			    style->background.color.alpha == 255) {
				return FALSE;
			}
		}
		rect.x += parent->box.padding.x;
		rect.y += parent->box.padding.y;
		LCUIRectF_ValidateArea(&rect, parent->box.padding.width,
				       parent->box.padding.height);
		if (rect.width < 1 || rect.height < 1) {
			return FALSE;
		}
	}
	return TRUE;
}

int ui_widget_set_update_rules(ui_widget_t* w,
			       const ui_widget_rules_t* rules)
{
	ui_updater_rules_data_t* data;

	data = (ui_updater_rules_data_t*)w->rules;
	if (data) {
		Dict_Release(data->style_cache);
		free(data);
		w->rules = NULL;
	}
	if (!rules) {
		return 0;
	}
	data = malloc(sizeof(ui_updater_rules_data_t));
	if (!data) {
		return -ENOMEM;
	}
	data->rules = *rules;
	data->progress = 0;
	data->style_cache = NULL;
	data->default_max_update_count = 2048;
	w->rules = (ui_widget_rules_t*)data;
	return 0;
}

void ui_widget_update_stacking_context(ui_widget_t* w)
{
	ui_widget_t *child, *target;
	ui_widget_style_t *s, *ts;
	LinkedListNode *node, *target_node;
	LinkedList* list;

	list = &w->stacking_context;
	LinkedList_ClearData(list, NULL);
	for (LinkedList_Each(node, &w->children)) {
		child = node->data;
		s = &child->computed_style;
		if (child->state < LCUI_WSTATE_READY) {
			continue;
		}
		for (LinkedList_Each(target_node, list)) {
			target = target_node->data;
			ts = &target->computed_style;
			if (s->z_index == ts->z_index) {
				if (s->position == ts->position) {
					if (child->index < target->index) {
						continue;
					}
				} else if (s->position < ts->position) {
					continue;
				}
			} else if (s->z_index < ts->z_index) {
				continue;
			}
			LinkedList_Link(list, target_node->prev,
					&child->node_show);
			break;
		}
		if (!target_node) {
			LinkedList_AppendNode(list, &child->node_show);
		}
	}
}

#define set_task_handler(ID, HANDLER) \
	ui_updater.handlers[UI_TASK_##ID] = HANDLER

void ui_init_updater(void)
{
	DictType* dt = &ui_updater.style_cache_dict;

	dt->valDup = NULL;
	dt->keyDup = ui_style_dict_key_dup;
	dt->keyCompare = ui_style_dict_key_compare;
	dt->hashFunction = ui_style_dict_hash;
	dt->keyDestructor = ui_style_dict_key_free;
	dt->valDestructor = ui_style_dict_val_free;
	set_task_handler(VISIBLE, ui_widget_compute_visibility_style);
	set_task_handler(POSITION, ui_widget_compute_position_style);
	set_task_handler(RESIZE, ui_widget_compute_size_style);
	set_task_handler(SHADOW, ui_widget_compute_box_shadow_style);
	set_task_handler(BORDER, ui_widget_compute_border_style);
	set_task_handler(OPACITY, ui_widget_compute_opacity_style);
	set_task_handler(MARGIN, ui_widget_compute_margin_style);
	set_task_handler(PADDING, ui_widget_compute_padding_style);
	set_task_handler(BACKGROUND, ui_widget_compute_background_style);
	set_task_handler(ZINDEX, ui_widget_compute_zindex_style);
	set_task_handler(DISPLAY, ui_widget_compute_display_style);
	set_task_handler(FLEX, ui_widget_compute_flex_style);
	set_task_handler(PROPS, ui_widget_compute_properties);
	set_task_handler(UPDATE_STYLE, ui_widget_on_update_style);
	set_task_handler(REFRESH_STYLE, ui_widget_on_refresh_style);
	set_task_handler(TITLE, ui_widget_on_set_title);
	set_task_handler(REFLOW, NULL);
	ui_updater.refresh_all = TRUE;
}

void ui_destroy_updater(void)
{
}

static ui_updater_profile_t* ui_widget_begin_update(
    ui_widget_t* w, ui_updater_profile_t* ctx)
{
	unsigned hash;
	LCUI_Selector selector;
	LCUI_StyleSheet style;
	ui_updater_rules_data_t* data;
	LCUI_CachedStyleSheet matched_style;
	ui_updater_profile_t* self_ctx;
	ui_updater_profile_t* parent_ctx;

	self_ctx = malloc(sizeof(ui_updater_profile_t));
	if (!self_ctx) {
		return NULL;
	}
	self_ctx->parent = ctx;
	self_ctx->style_cache = NULL;
	for (parent_ctx = ctx; parent_ctx; parent_ctx = parent_ctx->parent) {
		if (parent_ctx->style_cache) {
			self_ctx->style_cache = parent_ctx->style_cache;
			self_ctx->style_hash = parent_ctx->style_hash;
			break;
		}
	}
	if (ctx && ctx->profile) {
		self_ctx->profile = ctx->profile;
	} else {
		self_ctx->profile = NULL;
	}
	if (w->hash && w->update.states[UI_TASK_REFRESH_STYLE]) {
		ui_widget_generate_self_hash(w);
	}
	if (!self_ctx->style_cache && w->rules &&
	    w->rules->cache_children_style) {
		data = (ui_updater_rules_data_t*)w->rules;
		if (!data->style_cache) {
			data->style_cache =
			    Dict_Create(&ui_updater.style_cache_dict, NULL);
		}
		ui_widget_generate_self_hash(w);
		self_ctx->style_hash = w->hash;
		self_ctx->style_cache = data->style_cache;
	}
	matched_style = w->matched_style;
	if (self_ctx->style_cache && w->hash) {
		hash = self_ctx->style_hash;
		hash = ((hash << 5) + hash) + w->hash;
		style = Dict_FetchValue(self_ctx->style_cache, &hash);
		if (!style) {
			style = StyleSheet();
			selector = ui_widget_create_selector(w);
			LCUI_GetStyleSheet(selector, style);
			Dict_Add(self_ctx->style_cache, &hash, style);
			Selector_Delete(selector);
		}
		w->matched_style = style;
	} else {
		selector = ui_widget_create_selector(w);
		w->matched_style = LCUI_GetCachedStyleSheet(selector);
		Selector_Delete(selector);
	}
	if (w->matched_style != matched_style) {
		ui_widget_add_task(w, UI_TASK_REFRESH_STYLE);
	}
	return self_ctx;
}

static void ui_widget_end_update(ui_updater_profile_t* ctx)
{
	ctx->style_cache = NULL;
	ctx->parent = NULL;
	free(ctx);
}

static size_t ui_widget_update_visible_children(
    ui_widget_t* w, ui_updater_profile_t* ctx)
{
	size_t total = 0, count;
	LCUI_BOOL found = FALSE;
	LCUI_RectF rect, visible_rect;
	ui_widget_t *child, *parent;
	LinkedListNode *node, *next;

	rect = w->box.padding;
	if (rect.width < 1 && ui_widget_has_auto_style(w, key_width)) {
		rect.width = w->parent->box.padding.width;
	}
	if (rect.height < 1 && ui_widget_has_auto_style(w, key_height)) {
		rect.height = w->parent->box.padding.height;
	}
	for (child = w, parent = w->parent; parent;
	     child = parent, parent = parent->parent) {
		if (child == w) {
			continue;
		}
		rect.x += child->box.padding.x;
		rect.y += child->box.padding.y;
		LCUIRectF_ValidateArea(&rect, parent->box.padding.width,
				       parent->box.padding.height);
	}
	visible_rect = rect;
	rect = w->box.padding;
	ui_widget_get_offset(w, NULL, &rect.x, &rect.y);
	if (!LCUIRectF_GetOverlayRect(&visible_rect, &rect, &visible_rect)) {
		return 0;
	}
	visible_rect.x -= w->box.padding.x;
	visible_rect.y -= w->box.padding.y;
	for (node = w->children.head.next; node; node = next) {
		child = node->data;
		next = node->next;
		if (!LCUIRectF_GetOverlayRect(&visible_rect, &child->box.border,
					      &rect)) {
			if (found) {
				break;
			}
			continue;
		}
		found = TRUE;
		count = ui_widget_update_with_context(child, ctx);
		if (child->update.for_self || child->update.for_children) {
			w->update.for_children = TRUE;
		}
		total += count;
		node = next;
	}
	return total;
}

static size_t ui_widget_update_children(ui_widget_t* w,
					ui_updater_profile_t* ctx)
{
	clock_t msec = 0;
	ui_widget_t* child;
	ui_updater_rules_data_t* data;
	LinkedListNode *node, *next;
	size_t total = 0, update_count = 0, count;

	if (!w->update.for_children) {
		return 0;
	}
	data = (ui_updater_rules_data_t*)w->rules;
	node = w->children.head.next;
	if (data) {
		msec = clock();
		if (data->rules.only_on_visible) {
			if (!ui_widget_in_viewport(w)) {
				DEBUG_MSG("%s %s: is not visible\n", w->type,
					  w->id);
				return 0;
			}
		}
		DEBUG_MSG("%s %s: is visible\n", w->type, w->id);
		if (data->rules.first_update_visible_children) {
			total += ui_widget_update_visible_children(w, ctx);
			DEBUG_MSG("first update visible children "
				  "count: %zu\n",
				  total);
		}
	}
	if (!w->update.for_children) {
		return 0;
	}
	w->update.for_children = FALSE;
	while (node) {
		child = node->data;
		next = node->next;
		count = ui_widget_update_with_context(child, ctx);
		if (child->update.for_self || child->update.for_children) {
			w->update.for_children = TRUE;
		}
		total += count;
		node = next;
		if (!data) {
			continue;
		}
		if (count > 0) {
			data->progress = max(child->index, data->progress);
			if (data->progress > w->stacking_context.length) {
				data->progress = child->index;
			}
			update_count += 1;
		}
		if (data->rules.max_update_children_count < 0) {
			continue;
		}
		if (data->rules.max_update_children_count > 0) {
			if (update_count >=
			    (size_t)data->rules.max_update_children_count) {
				w->update.for_children = TRUE;
				break;
			}
		}
		if (update_count < data->default_max_update_count) {
			continue;
		}
		w->update.for_children = TRUE;
		msec = (clock() - msec);
		if (msec < 1) {
			data->default_max_update_count += 128;
			continue;
		}
		data->default_max_update_count = update_count * CLOCKS_PER_SEC /
						 LCUI_MAX_FRAMES_PER_SEC / msec;
		if (data->default_max_update_count < 1) {
			data->default_max_update_count = 32;
		}
		break;
	}
	if (data) {
		if (!w->update.for_children) {
			data->progress = w->stacking_context.length;
		}
		if (data->rules.on_update_progress) {
			data->rules.on_update_progress(w, data->progress);
		}
	}
	return total;
}

static void ui_widget_update_self(ui_widget_t* w,
				  ui_updater_profile_t* ctx)
{
	int i;
	LCUI_BOOL* states;

	states = w->update.states;
	w->update.for_self = FALSE;
	for (i = 0; i < UI_TASK_REFLOW; ++i) {
		if (states[i]) {
			if (w->proto && w->proto->runtask) {
				w->proto->runtask(w, i);
			}
			states[i] = FALSE;
			if (ui_updater.handlers[i]) {
				ui_updater.handlers[i](w);
			}
		}
	}
	if (states[UI_TASK_USER] && w->proto && w->proto->runtask) {
		states[UI_TASK_USER] = FALSE;
		w->proto->runtask(w, UI_TASK_USER);
	}
	ui_widget_add_state(w, LCUI_WSTATE_UPDATED);
}

static size_t ui_widget_update_with_context(ui_widget_t* w,
					    ui_updater_profile_t* ctx)
{
	size_t count = 0;
	ui_updater_profile_t* self_ctx;

	if (!w->update.for_self && !w->update.for_children) {
		return 0;
	}
	if (ui_updater.refresh_all) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
	}
	self_ctx = ui_widget_begin_update(w, ctx);
	ui_widget_begin_layout_diff(w, &self_ctx->layout_diff);
	if (w->update.for_self) {
		if (ui_updater.refresh_all) {
			memset(&self_ctx->style_diff, 0,
			       sizeof(ui_widget_style_diff_t));
			ui_widget_init_style_diff(w, &self_ctx->style_diff);
		} else {
			ui_widget_init_style_diff(w, &self_ctx->style_diff);
			ui_widget_begin_style_diff(w, &self_ctx->style_diff);
		}
		ui_widget_update_self(w, self_ctx);
		ui_widget_end_style_diff(w, &self_ctx->style_diff);
	}
	if (w->update.for_children) {
		count += ui_widget_update_children(w, self_ctx);
	}
	if (w->update.states[UI_TASK_REFLOW]) {
		ui_widget_reflow(w, UI_LAYOUT_RULE_AUTO);
		w->update.states[UI_TASK_REFLOW] = FALSE;
	}
	ui_widget_end_layout_diff(w, &self_ctx->layout_diff);
	ui_widget_end_update(self_ctx);
	ui_widget_update_stacking_context(w);
	return count;
}

size_t ui_widget_update(ui_widget_t* w)
{
	size_t count;
	ui_updater_profile_t* ctx;

	ctx = ui_widget_begin_update(w, NULL);
	count = ui_widget_update_with_context(w, ctx);
	ui_widget_end_update(ctx);
	return count;
}

size_t ui_update(void)
{
	size_t count;
	ui_widget_t* root;
	const ui_metrics_t* metrics;

	metrics = ui_get_metrics();
	if (memcmp(metrics, &ui_updater.metrics, sizeof(ui_metrics_t))) {
		ui_updater.refresh_all = TRUE;
	}
	if (ui_updater.refresh_all) {
		ui_refresh_style();
	}
	root = ui_root();
	count = ui_widget_update(root);
	root->state = LCUI_WSTATE_NORMAL;
	ui_updater.metrics = *metrics;
	ui_updater.refresh_all = FALSE;
	ui_trash_clear();
	return count;
}

void ui_widget_update_with_profile(ui_widget_t* w, ui_profile_t* profile)
{
	ui_updater_profile_t* ctx;

	ctx = ui_widget_begin_update(w, NULL);
	ctx->profile = profile;
	ui_widget_update_with_context(w, ctx);
	ui_widget_end_update(ctx);
}

void ui_update_with_profile(ui_profile_t* profile)
{
	ui_widget_t* root;
	const ui_metrics_t* metrics;

	profile->time = clock();
	metrics = ui_get_metrics();
	if (memcmp(metrics, &ui_updater.metrics, sizeof(ui_metrics_t))) {
		ui_updater.refresh_all = TRUE;
	}
	if (ui_updater.refresh_all) {
		ui_refresh_style();
	}
	if (ui_updater.refresh_all) {
		ui_refresh_style();
	}
	root = ui_root();
	ui_widget_update_with_profile(root, profile);
	root->state = LCUI_WSTATE_NORMAL;
	profile->time = clock() - profile->time;
	profile->destroy_time = clock();
	profile->destroy_count = ui_trash_clear();
	profile->destroy_time = clock() - profile->destroy_time;
}

void ui_refresh_style(void)
{
	ui_widget_t* root = ui_root();
	ui_widget_update_style(root, TRUE);
	ui_widget_add_task_for_children(root, UI_TASK_REFRESH_STYLE);
}
