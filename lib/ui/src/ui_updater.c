// #define UI_DEBUG_ENABLED
#include <string.h>
#include <time.h>
#include <LCUI/css/selector.h>
#include <LCUI/css/style_decl.h>
#include "internal.h"
#include "ui_debug.h"

// TODO: 考虑将 style_cache 移动到 ui_updater 内

typedef struct ui_updater_profile_t ui_updater_profile_t;
struct ui_updater_profile_t {
	unsigned style_hash;
	/**
	 * dict_t<hash, css_style_decl_t>
	 */
	dict_t* style_cache;
	ui_updater_profile_t* parent;
	ui_profile_t* profile;
};

static struct ui_updater_t {
	dict_type_t style_cache_dict;
	ui_metrics_t metrics;
	LCUI_BOOL refresh_all;
	ui_widget_function_t handlers[UI_TASK_TOTAL_NUM];
} ui_updater;

static size_t ui_widget_update_with_context(ui_widget_t* w,
					    ui_updater_profile_t* ctx);

static uint64_t ui_style_dict_hash(const void* key)
{
	return (*(unsigned int*)key);
}

static int ui_style_dict_string_key_compare(void* privdata, const void* key1,
					    const void* key2)
{
	return *(unsigned int*)key1 == *(unsigned int*)key2;
}

static void ui_style_dict_string_key_free(void* privdata, void* key)
{
	free(key);
}

static void* ui_style_dict_string_key_dup(void* privdata, const void* key)
{
	unsigned int* newkey = malloc(sizeof(unsigned int));
	*newkey = *(unsigned int*)key;
	return newkey;
}

static void ui_style_dict_val_free(void* privdata, void* val)
{
	css_style_decl_destroy(val);
}

static void ui_widget_on_refresh_style(ui_widget_t* w)
{
	// TODO：是否有必要将样式计算功能分为 refresh 和 update？
	ui_widget_force_refresh_style(w);
	w->update.states[UI_TASK_UPDATE_STYLE] = FALSE;
}

static void ui_widget_on_update_style(ui_widget_t* w)
{
	ui_widget_force_update_style(w);
}

void ui_widget_add_task_for_children(ui_widget_t* widget, ui_task_type_t task)
{
	ui_widget_t* child;
	list_node_t* node;

	widget->update.for_children = TRUE;
	for (list_each(node, &widget->children)) {
		child = node->data;
		ui_widget_add_task(child, task);
		ui_widget_add_task_for_children(child, task);
	}
}

void ui_widget_add_task(ui_widget_t* widget, int task)
{
	if (widget->state == UI_WIDGET_STATE_DELETED) {
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

void ui_widget_set_rules(ui_widget_t* w, const ui_widget_rules_t* rules)
{
	ui_widget_use_extra_data(w);
	if (w->extra->style_cache) {
		dict_destroy(w->extra->style_cache);
	}
	if (rules) {
		w->extra->rules = *rules;
	} else {
		memset(&w->extra->rules, 0, sizeof(ui_widget_rules_t));
	}
	w->extra->style_cache = NULL;
	w->extra->update_progress = 0;
	w->extra->default_max_update_count = 2048;
}

void ui_widget_update_stacking_context(ui_widget_t* w)
{
	ui_widget_t *child, *target;
	css_computed_style_t *s, *ts;
	list_node_t *node, *target_node;
	list_t* list;

	list = &w->stacking_context;
	list_destroy_without_node(list, NULL);
	for (list_each(node, &w->children)) {
		child = node->data;
		s = &child->computed_style;
		if (child->state < UI_WIDGET_STATE_READY) {
			continue;
		}
		for (list_each(target_node, list)) {
			target = target_node->data;
			ts = &target->computed_style;
			if (s->z_index == ts->z_index) {
				if (s->type_bits.position ==
				    ts->type_bits.position) {
					if (child->index < target->index) {
						continue;
					}
				} else if (s->type_bits.position <
					   ts->type_bits.position) {
					continue;
				}
			} else if (s->z_index < ts->z_index) {
				continue;
			}
			list_link(list, target_node->prev, &child->node_show);
			break;
		}
		if (!target_node) {
			list_append_node(list, &child->node_show);
		}
	}
}

#define set_task_handler(ID, HANDLER) \
	ui_updater.handlers[UI_TASK_##ID] = HANDLER

void ui_init_updater(void)
{
	dict_type_t* dt = &ui_updater.style_cache_dict;

	dt->val_dup = NULL;
	dt->key_dup = ui_style_dict_string_key_dup;
	dt->key_compare = ui_style_dict_string_key_compare;
	dt->hash_function = ui_style_dict_hash;
	dt->key_destructor = ui_style_dict_string_key_free;
	dt->val_destructor = ui_style_dict_val_free;
	set_task_handler(UPDATE_STYLE, ui_widget_on_update_style);
	set_task_handler(REFRESH_STYLE, ui_widget_on_refresh_style);
	set_task_handler(REFLOW, NULL);
	ui_updater.refresh_all = TRUE;
}

static ui_updater_profile_t* ui_widget_begin_update(ui_widget_t* w,
						    ui_updater_profile_t* ctx)
{
	unsigned hash;
	css_selector_t* selector;
	css_style_decl_t* style;
	const css_style_decl_t* matched_style;
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
	if (!self_ctx->style_cache && w->extra &&
	    w->extra->rules.cache_children_style) {
		if (!w->extra->style_cache) {
			w->extra->style_cache =
			    dict_create(&ui_updater.style_cache_dict, NULL);
		}
		ui_widget_generate_self_hash(w);
		self_ctx->style_hash = w->hash;
		self_ctx->style_cache = w->extra->style_cache;
	}
	matched_style = w->matched_style;
	if (self_ctx->style_cache && w->hash) {
		hash = self_ctx->style_hash;
		hash = ((hash << 5) + hash) + w->hash;
		style = dict_fetch_value(self_ctx->style_cache, &hash);
		if (!style) {
			selector = ui_widget_create_selector(w);
			style = css_select_style_with_cache(selector);
			dict_add(self_ctx->style_cache, &hash, style);
			css_selector_destroy(selector);
		}
		w->matched_style = style;
	} else {
		selector = ui_widget_create_selector(w);
		w->matched_style = css_select_style_with_cache(selector);
		css_selector_destroy(selector);
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

static size_t ui_widget_update_visible_children(ui_widget_t* w,
						ui_updater_profile_t* ctx)
{
	size_t total = 0, count;
	LCUI_BOOL found = FALSE;
	ui_rect_t rect, visible_rect;
	ui_widget_t *child, *parent;
	list_node_t *node, *next;

	rect = w->padding_box;
	if (rect.width < 1 &&
	    w->computed_style.type_bits.width == CSS_WIDTH_AUTO) {
		rect.width = w->parent->padding_box.width;
	}
	if (rect.height < 1 &&
	    w->computed_style.type_bits.height == CSS_HEIGHT_AUTO) {
		rect.height = w->parent->padding_box.height;
	}
	for (child = w, parent = w->parent; parent;
	     child = parent, parent = parent->parent) {
		if (child == w) {
			continue;
		}
		rect.x += child->padding_box.x;
		rect.y += child->padding_box.y;
		ui_rect_correct(&rect, parent->padding_box.width,
				parent->padding_box.height);
	}
	visible_rect = rect;
	rect = w->padding_box;
	ui_widget_get_offset(w, NULL, &rect.x, &rect.y);
	if (!ui_rect_overlap(&visible_rect, &rect, &visible_rect)) {
		return 0;
	}
	visible_rect.x -= w->padding_box.x;
	visible_rect.y -= w->padding_box.y;
	for (node = w->children.head.next; node; node = next) {
		child = node->data;
		next = node->next;
		if (!ui_rect_overlap(&visible_rect, &child->border_box,
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
	ui_widget_rules_t* rules;
	list_node_t *node, *next;
	size_t total = 0, update_count = 0, count;

	if (!w->update.for_children) {
		return 0;
	}
	node = w->children.head.next;
	rules = w->extra ? &w->extra->rules : NULL;
	if (rules) {
		msec = clock();
		if (rules->only_on_visible) {
			if (!ui_widget_in_viewport(w)) {
				DEBUG_MSG("%s %s: is not visible\n", w->type,
					  w->id);
				return 0;
			}
		}
		DEBUG_MSG("%s %s: is visible\n", w->type, w->id);
		if (rules->first_update_visible_children) {
			total += ui_widget_update_visible_children(w, ctx);
			DEBUG_MSG("first update visible children "
				  "count: %zu\n",
				  total);
		}
		if (!w->update.for_children) {
			return 0;
		}
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
		if (!rules || rules->max_update_children_count == 0) {
			continue;
		}
		if (count > 0) {
			w->extra->update_progress =
			    y_max(child->index, w->extra->update_progress);
			if (w->extra->update_progress >
			    w->stacking_context.length) {
				w->extra->update_progress = child->index;
			}
			update_count += 1;
		}
		if (rules->max_update_children_count > 0) {
			if (update_count >=
			    (size_t)rules->max_update_children_count) {
				w->update.for_children = TRUE;
				break;
			}
		}
		if (update_count < w->extra->default_max_update_count) {
			continue;
		}
		w->update.for_children = TRUE;
		msec = (clock() - msec);
		if (msec < 1) {
			w->extra->default_max_update_count += 128;
			continue;
		}
		w->extra->default_max_update_count =
		    update_count * CLOCKS_PER_SEC / 120 / msec;
		if (w->extra->default_max_update_count < 1) {
			w->extra->default_max_update_count = 32;
		}
		break;
	}
	if (rules) {
		if (!w->update.for_children) {
			w->extra->update_progress = w->stacking_context.length;
		}
		if (rules->on_update_progress) {
			rules->on_update_progress(w, w->extra->update_progress);
		}
	}
	return total;
}

static void ui_widget_update_self(ui_widget_t* w, ui_updater_profile_t* ctx)
{
	int i;
	LCUI_BOOL* states;
	ui_style_diff_t style_diff = { 0 };

	if (!w->update.for_self) {
		return;
	}
	ui_style_diff_init(&style_diff, w);
	if (!ui_updater.refresh_all) {
		ui_style_diff_begin(&style_diff, w);
	}
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
	ui_widget_add_state(w, UI_WIDGET_STATE_UPDATED);
	ui_style_diff_end(&style_diff, w);
}

static size_t ui_widget_update_with_context(ui_widget_t* w,
					    ui_updater_profile_t* ctx)
{
	size_t count = 0;
	ui_updater_profile_t* self_ctx;
	ui_layout_diff_t diff;

	if (ui_updater.refresh_all) {
		w->update.for_self = TRUE;
		w->update.for_children = TRUE;
		w->update.states[UI_TASK_REFRESH_STYLE] = TRUE;
		w->update.states[UI_TASK_REFLOW] = TRUE;
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
	}
	if (!w->update.for_self && !w->update.for_children) {
		return 0;
	}
	ui_layout_diff_begin(&diff, w);
	self_ctx = ui_widget_begin_update(w, ctx);
	ui_widget_update_self(w, self_ctx);
#ifdef UI_DEBUG_ENABLED
	{
		UI_WIDGET_STR(w, str);
		UI_DEBUG_MSG("%s: start update children", str);
		ui_debug_msg_indent++;
	}
#endif
	count += ui_widget_update_children(w, self_ctx);

#ifdef UI_DEBUG_ENABLED
	{
		ui_debug_msg_indent--;
		UI_WIDGET_STR(w, str);
		UI_DEBUG_MSG("%s: end update children", str);
	}
#endif
	if (w->update.states[UI_TASK_REFLOW]) {
#ifdef UI_DEBUG_ENABLED
		{
			UI_WIDGET_STR(w, str);
			UI_DEBUG_MSG("%s: start initiative reflow", str);
		}
#endif
		ui_widget_reset_size(w);
		ui_widget_reflow(w);
		w->max_content_width = w->content_box.width;
		w->max_content_height = w->content_box.height;
		if (!w->parent || !ui_widget_in_layout_flow(w)) {
			ui_widget_update_box_position(w);
		}
		w->update.states[UI_TASK_REFLOW] = FALSE;
#ifdef UI_DEBUG_ENABLED
		{
			UI_WIDGET_STR(w, str);
			UI_DEBUG_MSG("%s: end initiative reflow", str);
		}
#endif
	}
	ui_widget_end_update(self_ctx);
	ui_widget_update_stacking_context(w);
	ui_layout_diff_end(&diff, w);
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

void ui_refresh_style(void)
{
	ui_updater.refresh_all = TRUE;
}

size_t ui_update(void)
{
	size_t count;
	ui_widget_t* root;
	ui_layout_diff_t diff;

	if (memcmp(&ui_metrics, &ui_updater.metrics, sizeof(ui_metrics_t))) {
		ui_updater.refresh_all = TRUE;
	}
	if (ui_updater.refresh_all) {
		ui_refresh_style();
	}
	root = ui_root();
	ui_layout_diff_begin(&diff, root);
	count = ui_widget_update(root);
	ui_updater.metrics = ui_metrics;
	ui_updater.refresh_all = FALSE;
	ui_layout_diff_end(&diff, root);
	ui_process_mutation_observers();
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

// TODO: 删除性能监测相关代码
void ui_update_with_profile(ui_profile_t* profile)
{
	ui_widget_t* root;

	profile->time = clock();
	if (memcmp(&ui_metrics, &ui_updater.metrics, sizeof(ui_metrics_t))) {
		ui_updater.refresh_all = TRUE;
	}
	if (ui_updater.refresh_all) {
		ui_refresh_style();
	}
	root = ui_root();
	ui_widget_update_with_profile(root, profile);
	ui_widget_add_state(root, UI_WIDGET_STATE_LAYOUTED);
	profile->time = clock() - profile->time;
	profile->destroy_time = clock();
	profile->destroy_count = ui_trash_clear();
	profile->destroy_time = clock() - profile->destroy_time;
}
