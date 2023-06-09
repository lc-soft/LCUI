#include <assert.h>
#include <css.h>
#include <ui/base.h>
#include <ui/events.h>
#include <ui/style.h>
#include <ui/prototype.h>

bool ui_widget_is_visible(ui_widget_t *w)
{
	return css_computed_display(&w->computed_style) != CSS_DISPLAY_NONE &&
	       css_computed_visibility(&w->computed_style) !=
		   CSS_VISIBILITY_HIDDEN;
}

void ui_widget_move(ui_widget_t *w, float left, float top)
{
	ui_widget_set_style_unit_value(w, css_prop_top, top, CSS_UNIT_PX);
	ui_widget_set_style_unit_value(w, css_prop_left, left, CSS_UNIT_PX);
}

void ui_widget_resize(ui_widget_t *w, float width, float height)
{
	ui_widget_set_style_unit_value(w, css_prop_width, width, CSS_UNIT_PX);
	ui_widget_set_style_unit_value(w, css_prop_height, height, CSS_UNIT_PX);
}

void ui_widget_show(ui_widget_t *w)
{
	css_style_value_t v;

	ui_widget_get_style(w, css_prop_display, &v);
	if (v.type == CSS_ARRAY_VALUE &&
	    v.array_value[0].type == CSS_KEYWORD_VALUE &&
	    v.array_value[0].keyword_value == CSS_KEYWORD_NONE) {
		ui_widget_unset_style(w, css_prop_display);
		ui_widget_update_style(w);
	}
}

void ui_widget_hide(ui_widget_t *w)
{
	ui_widget_set_style_keyword_value(w, css_prop_display, CSS_KEYWORD_NONE);
	ui_widget_update_style(w);
}

ui_widget_t *ui_widget_get_closest(ui_widget_t *w, const char *type)
{
	ui_widget_t *target;

	for (target = w; target; target = target->parent) {
		if (ui_check_widget_type(target, type)) {
			return target;
		}
	}
	return NULL;
}

static void ui_widget_collect_reference(ui_widget_t *w, void *arg)
{
	const char *ref = ui_widget_get_attr(w, "ref");

	if (ref) {
		dict_add(arg, (void *)ref, w);
	}
}

dict_t *ui_widget_collect_references(ui_widget_t *w)
{
	dict_t *dict;
	static dict_type_t t;

	dict_init_string_key_type(&t);
	dict = dict_create(&t, NULL);
	ui_widget_each(w, ui_widget_collect_reference, dict);
	return dict;
}
