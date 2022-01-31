#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/css.h"

#define SPLIT_NUMBER 1
#define SPLIT_COLOR (1 << 1)
#define SPLIT_STYLE (1 << 2)

static struct css_property_parser_module_t {
	dict_t *dict;
	size_t count;
} css_property_parser;

#define set_current_property(VALUE) \
	css_style_parser_set_property(parser, parser->prop_key, VALUE)

#define set_property(KEY, VALUE) \
	css_style_parser_set_property(parser, KEY, VALUE)

static int split_values(const char *str, css_unit_value_t *slist, int max_len,
			int mode)
{
	char **values;
	const char *p;
	int val, vi = 0, vj = 0, n_quotes = 0;

	memset(slist, 0, sizeof(css_unit_value_t) * max_len);
	values = (char **)calloc(max_len, sizeof(char *));
	values[0] = (char *)malloc(sizeof(char) * 64);
	for (p = str; *p; ++p) {
		if (*p == '(') {
			n_quotes += 1;
			values[vi][vj++] = *p;
			continue;
		} else if (*p == ')') {
			n_quotes -= 1;
			values[vi][vj++] = *p;
			continue;
		} else if (*p != ' ' || n_quotes != 0) {
			values[vi][vj++] = *p;
			continue;
		}
		if (vj > 0) {
			values[vi][vj] = 0;
			++vi;
			vj = 0;
			if (vi >= max_len) {
				goto clean;
			}
			values[vi] = (char *)malloc(sizeof(char) * 64);
		}
	}
	values[vi][vj] = 0;
	vi++;
	for (vj = 0; vj < vi; ++vj) {
		DEBUG_MSG("[%d] %s\n", vj, values[vj]);
		if (strcmp(values[vj], "auto") == 0) {
			slist[vj].unit = CSS_UNIT_AUTO;
			slist[vj].val_keyword = CSS_KEYWORD_AUTO;
			slist[vj].is_valid = TRUE;
			continue;
		}
		if (mode & SPLIT_NUMBER) {
			if (css_parse_number(&slist[vj], values[vj])) {
				DEBUG_MSG("[%d]:parse ok\n", vj);
				continue;
			}
		}
		if (mode & SPLIT_COLOR) {
			if (css_parse_color(&slist[vj], values[vj])) {
				DEBUG_MSG("[%d]:parse ok\n", vj);
				continue;
			}
		}
		if (mode & SPLIT_STYLE) {
			val = css_get_keyword_key(values[vj]);
			if (val > 0) {
				slist[vj].keyword = val;
				slist[vj].unit = CSS_UNIT_KEYWORD;
				slist[vj].is_valid = TRUE;
				DEBUG_MSG("[%d]:parse ok\n", vj);
				continue;
			}
		}
		vi = -1;
		DEBUG_MSG("[%d]:parse error\n", vj);
		goto clean;
	}
clean:
	for (vj = 0; vj < max_len; ++vj) {
		values[vj] ? free(values[vj]) : 0;
	}
	free(values);
	return vi;
}

static int css_parse_value(css_style_parser_t *parser, const char *str)
{
	css_unit_value_t s;

	if (sscanf(str, "%d", &s.val_int) == 1) {
		s.is_valid = TRUE;
		s.unit = CSS_UNIT_INT;
		set_current_property(&s);
		return 0;
	}
	return -1;
}

static int css_parse_number_value(css_style_parser_t *parser, const char *str)
{
	css_unit_value_t s;

	if (css_parse_number(&s, str)) {
		set_current_property(&s);
		return 0;
	}
	if (strcmp("auto", str) == 0) {
		s.is_valid = TRUE;
		s.unit = CSS_UNIT_AUTO;
		s.val_keyword = CSS_KEYWORD_AUTO;
		set_current_property(&s);
		return 0;
	}
	return -1;
}

static int css_parse_string_value(css_style_parser_t *parser, const char *str)
{
	css_unit_value_t s;

	s.is_valid = TRUE;
	s.unit = CSS_UNIT_STRING;
	s.val_string = strdup2(str);
	set_current_property(&s);
	return 0;
}

static int css_parse_boolean_value(css_style_parser_t *parser, const char *str)
{
	css_unit_value_t s = { 0 };

	if (strcmp(str, "true") == 0) {
		s.is_valid = TRUE;
		s.unit = CSS_UNIT_BOOL;
		s.val_bool = TRUE;
		set_current_property(&s);
		return 0;
	}
	if (strcmp(str, "false") == 0) {
		s.is_valid = TRUE;
		s.unit = CSS_UNIT_BOOL;
		s.val_bool = FALSE;
		set_current_property(&s);
		return 0;
	}
	if (strcmp("auto", str) == 0) {
		s.is_valid = TRUE;
		s.unit = CSS_UNIT_AUTO;
		s.val_keyword = CSS_KEYWORD_AUTO;
		set_current_property(&s);
		return 0;
	}
	return -1;
}

static int css_parse_color_value(css_style_parser_t *parser, const char *str)
{
	css_unit_value_t s;

	if (css_parse_color(&s, str)) {
		set_current_property(&s);
		return 0;
	}
	return -1;
}

static int css_parse_image_value(css_style_parser_t *parser, const char *str)
{
	css_unit_value_t s;

	if (css_parse_url(&s, str, parser->dirname)) {
		set_current_property(&s);
		return 0;
	}
	return -1;
}

static int css_parse_keyword_value(css_style_parser_t *parser, const char *str)
{
	css_unit_value_t s;
	int v = css_get_keyword_key(str);

	if (v < 0) {
		return -1;
	}
	s.val_keyword = v;
	s.unit = CSS_UNIT_KEYWORD;
	s.is_valid = TRUE;
	set_current_property(&s);
	return 0;
}

static int css_parse_border_property(css_style_parser_t *parser,
				     const char *str)
{
	css_unit_value_t slist[3];
	int i, mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;

	if (split_values(str, slist, 3, mode) < 1) {
		return -1;
	}
	for (i = 0; i < 3; ++i) {
		if (!slist[i].is_valid) {
			break;
		}
		switch (slist[i].unit) {
		case CSS_UNIT_COLOR:
			set_property(css_key_border_top_color,
				     &slist[i]);
			set_property(css_key_border_right_color,
				     &slist[i]);
			set_property(css_key_border_bottom_color,
				     &slist[i]);
			set_property(css_key_border_left_color,
				     &slist[i]);
			break;
		case CSS_UNIT_PX:
		case CSS_UNIT_INT:
			set_property(css_key_border_top_width,
				     &slist[i]);
			set_property(css_key_border_right_width,
				     &slist[i]);
			set_property(css_key_border_bottom_width,
				     &slist[i]);
			set_property(css_key_border_left_width,
				     &slist[i]);
			break;
		case CSS_UNIT_KEYWORD:
			set_property(css_key_border_top_style,
				     &slist[i]);
			set_property(css_key_border_right_style,
				     &slist[i]);
			set_property(css_key_border_bottom_style,
				     &slist[i]);
			set_property(css_key_border_left_style,
				     &slist[i]);
			break;
		default:
			return -1;
		}
	}
	return 0;
}

static int css_parse_border_radius_property(css_style_parser_t *parser,
					    const char *str)
{
	css_unit_value_t s;

	if (!css_parse_number(&s, str)) {
		return -1;
	}
	set_property(css_key_border_top_left_radius, &s);
	set_property(css_key_border_top_right_radius, &s);
	set_property(css_key_border_bottom_left_radius, &s);
	set_property(css_key_border_bottom_right_radius, &s);
	return 0;
}

static int css_parse_border_left_property(css_style_parser_t *parser,
					  const char *str)
{
	css_unit_value_t slist[3];
	int i, mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;

	if (split_values(str, slist, 3, mode) < 1) {
		return -1;
	}
	for (i = 0; i < 3; ++i) {
		switch (slist[i].unit) {
		case CSS_UNIT_COLOR:
			set_property(css_key_border_left_color,
				     &slist[i]);
			break;
		case CSS_UNIT_PX:
		case CSS_UNIT_INT:
			set_property(css_key_border_left_width,
				     &slist[i]);
			break;
		case CSS_UNIT_keyword:
			set_property(css_key_border_left_style,
				     &slist[i]);
			break;
		default:
			return -1;
		}
	}
	return 0;
}

static int css_parse_border_top_property(css_style_parser_t *parser,
					 const char *str)
{
	css_unit_value_t slist[3];
	int i, mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;

	if (split_values(str, slist, 3, mode) < 1) {
		return -1;
	}
	for (i = 0; i < 3; ++i) {
		switch (slist[i].unit) {
		case CSS_UNIT_COLOR:
			set_property(css_key_border_top_color,
				     &slist[i]);
			break;
		case CSS_UNIT_PX:
		case CSS_UNIT_INT:
			set_property(css_key_border_top_width,
				     &slist[i]);
			break;
		case CSS_UNIT_KEYWORD:
			set_property(css_key_border_top_style,
				     &slist[i]);
			break;
		default:
			return -1;
		}
	}
	return 0;
}

static int css_parse_border_right_property(css_style_parser_t *parser,
					   const char *str)
{
	css_unit_value_t slist[3];
	int i, mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;

	if (split_values(str, slist, 3, mode) < 1) {
		return -1;
	}
	for (i = 0; i < 3; ++i) {
		switch (slist[i].unit) {
		case CSS_UNIT_COLOR:
			set_property(css_key_border_right_color,
				     &slist[i]);
			break;
		case CSS_UNIT_PX:
		case CSS_UNIT_INT:
			set_property(css_key_border_right_width,
				     &slist[i]);
			break;
		case CSS_UNIT_KEYWORD:
			set_property(css_key_border_right_style,
				     &slist[i]);
			break;
		default:
			return -1;
		}
	}
	return 0;
}

static int css_parse_border_bottom_property(css_style_parser_t *parser,
					    const char *str)
{
	css_unit_value_t slist[3];
	int i, mode = SPLIT_COLOR | SPLIT_NUMBER | SPLIT_STYLE;

	if (split_values(str, slist, 3, mode) < 1) {
		return -1;
	}
	for (i = 0; i < 3; ++i) {
		switch (slist[i].unit) {
		case CSS_UNIT_COLOR:
			set_property(css_key_border_bottom_color,
				     &slist[i]);
			break;
		case CSS_UNIT_PX:
		case CSS_UNIT_INT:
			set_property(css_key_border_bottom_width,
				     &slist[i]);
			break;
		case CSS_UNIT_KEYWORD:
			set_property(css_key_border_bottom_style,
				     &slist[i]);
			break;
		default:
			return -1;
		}
	}
	return 0;
}

static int css_parse_border_color_property(css_style_parser_t *parser,
					   const char *str)
{
	css_unit_value_t s;

	// TODO: support parsing multiple values
	// border-color: #eee transparent;
	// border-color: #f00 #0f0 transparent;
	// border-color: #0f0 #f00 #f00 #0f0;
	if (!css_parse_color(&s, str)) {
		return -1;
	}
	set_property(css_key_border_top_color, &s);
	set_property(css_key_border_right_color, &s);
	set_property(css_key_border_bottom_color, &s);
	set_property(css_key_border_left_color, &s);
	return 0;
}

static int css_parse_border_width_property(css_style_parser_t *parser,
					   const char *str)
{
	css_unit_value_t s;

	// TODO: support parsing multiple values
	// border-width: 4px 0;
	// border-width: 4px 8px 0;
	// border-width: 4px 0 0 4px;
	if (!css_parse_number(&s, str)) {
		return -1;
	}
	set_property(css_key_border_top_width, &s);
	set_property(css_key_border_right_width, &s);
	set_property(css_key_border_bottom_width, &s);
	set_property(css_key_border_left_width, &s);
	return 0;
}

static int css_parse_border_style_property(css_style_parser_t *parser,
					   const char *str)
{
	css_unit_value_t s;

	s.is_valid = TRUE;
	s.val_keyword = css_get_keyword_key(str);
	if (s.val_keyword < 0) {
		return -1;
	}
	set_property(css_key_border_top_style, &s);
	set_property(css_key_border_right_style, &s);
	set_property(css_key_border_bottom_style, &s);
	set_property(css_key_border_left_style, &s);
	return 0;
}

static int css_parse_padding_property(css_style_parser_t *parser,
				      const char *str)
{
	css_unit_value_t s[4];

	switch (split_values(str, s, 4, SPLIT_NUMBER)) {
	case 1:
		set_property(css_key_padding_top, &s[0]);
		set_property(css_key_padding_right, &s[0]);
		set_property(css_key_padding_bottom, &s[0]);
		set_property(css_key_padding_left, &s[0]);
		break;
	case 2:
		set_property(css_key_padding_top, &s[0]);
		set_property(css_key_padding_bottom, &s[0]);
		set_property(css_key_padding_left, &s[1]);
		set_property(css_key_padding_right, &s[1]);
		break;
	case 3:
		set_property(css_key_padding_top, &s[0]);
		set_property(css_key_padding_left, &s[1]);
		set_property(css_key_padding_right, &s[1]);
		set_property(css_key_padding_bottom, &s[2]);
		break;
	case 4:
		set_property(css_key_padding_top, &s[0]);
		set_property(css_key_padding_right, &s[1]);
		set_property(css_key_padding_bottom, &s[2]);
		set_property(css_key_padding_left, &s[3]);
	default:
		break;
	}
	return 0;
}

static int css_parse_margin_property(css_style_parser_t *parser,
				     const char *str)
{
	css_unit_value_t s[4];

	switch (split_values(str, s, 4, SPLIT_NUMBER)) {
	case 1:
		set_property(css_key_margin_top, &s[0]);
		set_property(css_key_margin_right, &s[0]);
		set_property(css_key_margin_bottom, &s[0]);
		set_property(css_key_margin_left, &s[0]);
		break;
	case 2:
		set_property(css_key_margin_top, &s[0]);
		set_property(css_key_margin_bottom, &s[0]);
		set_property(css_key_margin_left, &s[1]);
		set_property(css_key_margin_right, &s[1]);
		break;
	case 3:
		set_property(css_key_margin_top, &s[0]);
		set_property(css_key_margin_left, &s[1]);
		set_property(css_key_margin_right, &s[1]);
		set_property(css_key_margin_bottom, &s[2]);
		break;
	case 4:
		set_property(css_key_margin_top, &s[0]);
		set_property(css_key_margin_right, &s[1]);
		set_property(css_key_margin_bottom, &s[2]);
		set_property(css_key_margin_left, &s[3]);
	default:
		break;
	}
	return 0;
}

static int css_parse_box_shadow_property(css_style_parser_t *parser,
					 const char *str)
{
	css_unit_value_t s[5];

	if (strcmp(str, "none") == 0) {
		s[0].val_int = 0;
		s[0].is_valid = TRUE;
		s[0].unit = CSS_UNIT_NONE;
		set_property(css_key_box_shadow_x, &s[0]);
		set_property(css_key_box_shadow_y, &s[0]);
		set_property(css_key_box_shadow_blur, &s[0]);
		set_property(css_key_box_shadow_spread, &s[0]);
		set_property(css_key_box_shadow_color, &s[0]);
		return 0;
	}
	switch (split_values(str, s, 5, SPLIT_NUMBER | SPLIT_COLOR)) {
	case 5:
		set_property(css_key_box_shadow_x, &s[0]);
		set_property(css_key_box_shadow_y, &s[1]);
		set_property(css_key_box_shadow_blur, &s[2]);
		set_property(css_key_box_shadow_spread, &s[3]);
		set_property(css_key_box_shadow_color, &s[4]);
		break;
	case 4:
		set_property(css_key_box_shadow_x, &s[0]);
		set_property(css_key_box_shadow_y, &s[1]);
		set_property(css_key_box_shadow_blur, &s[2]);
		set_property(css_key_box_shadow_color, &s[3]);
		break;
	default:
		return -1;
	}
	return 0;
}

static int css_parse_background_property(css_style_parser_t *parser,
					 const char *str)
{
	// TODO: support parsing multiple values
	// background: #eee;
	// background: url(image.png);
	// background: url(image.png) bottom center #eee;
	return 0;
}

static int css_parse_background_position_property(css_style_parser_t *parser,
						  const char *str)
{
	css_unit_value_t slist[2];
	int ret = css_parse_keyword_value(parser, str);

	if (ret == 0) {
		return 0;
	}
	if (split_values(str, slist, 2, SPLIT_NUMBER) == 2) {
		set_property(css_key_background_position_x, &slist[0]);
		set_property(css_key_background_position_y, &slist[1]);
		return 0;
	}
	return -2;
}

static int css_parse_background_size_property(css_style_parser_t *parser,
					      const char *str)
{
	css_unit_value_t none;
	css_unit_value_t slist[2];
	int ret = css_parse_keyword_value(parser, str);

	none.is_valid = TRUE;
	none.val_none = 0;
	none.unit = CSS_UNIT_NONE;
	if (ret == 0) {
		set_property(css_key_background_size_width, &none);
		set_property(css_key_background_size_height, &none);
		return 0;
	}
	ret = split_values(str, slist, 2, SPLIT_NUMBER | SPLIT_STYLE);
	if (ret != 2) {
		return -1;
	}
	set_property(css_key_background_size, &none);
	set_property(css_key_background_size_width, &slist[0]);
	set_property(css_key_background_size_height, &slist[1]);
	return 0;
}

static int css_parse_background_repeat_property(css_style_parser_t *parser,
						const char *str)
{
	return 0;
}

static int css_parse_visibility_property(css_style_parser_t *parser,
					 const char *str)
{
	css_unit_value_t s;

	if (strcmp(str, "visible") == 0 || strcmp(str, "hidden") == 0) {
		s.is_valid = TRUE;
		s.unit = CSS_UNIT_STRING;
		s.val_string = strdup2(str);
		set_current_property(&s);
		return 0;
	}
	return -1;
}

/* See more: https://developer.mozilla.org/en-US/docs/Web/CSS/flex */

static int css_parse_flex_property(css_style_parser_t *parser, const char *str)
{
	css_unit_value_t s;
	css_unit_value_t slist[3];
	int i, mode = SPLIT_NUMBER | SPLIT_STYLE;

	if (strcmp("initial", str) == 0) {
		s.unit = CSS_UNIT_INT;
		s.is_valid = TRUE;
		s.val_int = 0;
		set_property(css_key_flex_grow, &s);

		s.val_int = 1;
		set_property(css_key_flex_shrink, &s);

		s.unit = CSS_UNIT_AUTO;
		s.val_keyword = CSS_KEYWORD_AUTO;
		set_property(css_key_flex_basis, &s);
		return 0;
	}
	if (strcmp("auto", str) == 0) {
		s.unit = CSS_UNIT_INT;
		s.is_valid = TRUE;
		s.val_int = 1;
		set_property(css_key_flex_grow, &s);

		s.val_int = 1;
		set_property(css_key_flex_shrink, &s);

		s.unit = CSS_UNIT_AUTO;
		s.val_keyword = CSS_KEYWORD_AUTO;
		set_property(css_key_flex_basis, &s);
		return 0;
	}
	if (strcmp("none", str) == 0) {
		s.unit = CSS_UNIT_INT;
		s.is_valid = TRUE;
		s.val_int = 0;
		set_property(css_key_flex_grow, &s);

		s.val_int = 0;
		set_property(css_key_flex_shrink, &s);

		s.unit = CSS_UNIT_AUTO;
		s.val_keyword = CSS_KEYWORD_AUTO;
		set_property(css_key_flex_basis, &s);
		return 0;
	}

	i = split_values(str, slist, 3, mode);
	if (i == 3) {
		set_property(css_key_flex_grow, &slist[0]);
		set_property(css_key_flex_shrink, &slist[1]);
		set_property(css_key_flex_basis, &slist[2]);
		return 0;
	}
	if (i == 2) {
		set_property(css_key_flex_shrink, &slist[0]);
		set_property(css_key_flex_basis, &slist[1]);
		return 0;
	}
	if (i == 1) {
		if (slist[0].unit == CSS_UNIT_INT) {
			set_property(css_key_flex_grow, &slist[0]);
			return 0;
		}
		set_property(css_key_flex_basis, &slist[0]);
		return 0;
	}
	return -1;
}

/* See more: https://developer.mozilla.org/en-US/docs/Web/CSS/flex-flow */

static int css_parse_flex_flow_property(css_style_parser_t *parser,
					const char *str)
{
	css_unit_value_t s;
	css_unit_value_t slist[2];

	if (strcmp(str, "wrap") == 0) {
		s.is_valid = TRUE;
		s.unit = CSS_UNIT_KEYWORD;
		s.val_keyword = CSS_KEYWORD_WRAP;
		set_property(css_key_flex_wrap, &s);

		s.unit = CSS_UNIT_KEYWORD;
		s.val_keyword = CSS_KEYWORD_INITIAL;
		set_property(css_key_flex_direction, &s);
		return 0;
	}
	if (strcmp(str, "nowrap") == 0) {
		s.is_valid = TRUE;
		s.unit = CSS_UNIT_KEYWORD;
		s.val_keyword = CSS_KEYWORD_NOWRAP;
		set_property(css_key_flex_wrap, &s);

		s.unit = CSS_UNIT_KEYWORD;
		s.val_keyword = CSS_KEYWORD_INITIAL;
		set_property(css_key_flex_direction, &s);
		return 0;
	}
	if (strcmp(str, "row") == 0) {
		s.is_valid = TRUE;
		s.unit = CSS_UNIT_KEYWORD;
		s.val_keyword = CSS_KEYWORD_ROW;
		set_property(css_key_flex_direction, &s);

		s.unit = CSS_UNIT_KEYWORD;
		s.val_keyword = CSS_KEYWORD_INITIAL;
		set_property(css_key_flex_wrap, &s);
		return 0;
	}
	if (strcmp(str, "column") == 0) {
		s.is_valid = TRUE;
		s.unit = CSS_UNIT_KEYWORD;
		s.val_keyword = CSS_KEYWORD_COLUMN;
		set_property(css_key_flex_direction, &s);

		s.unit = CSS_UNIT_KEYWORD;
		s.val_keyword = CSS_KEYWORD_INITIAL;
		set_property(css_key_flex_wrap, &s);
		return 0;
	}
	if (split_values(str, slist, 2, SPLIT_STYLE) == 2) {
		set_property(css_key_flex_direction, &slist[0]);
		set_property(css_key_flex_wrap, &slist[1]);
		return 0;
	}
	return -1;
}

static int css_parse_flex_basis_property(css_style_parser_t *parser,
					 const char *str)
{
	css_unit_value_t s;

	if (css_parse_keyword_value(parser, str) == 0) {
		return 0;
	}
	if (css_parse_number(&s, str)) {
		set_property(css_key_flex_basis, &s);
		return 0;
	}
	return -1;
}

static int css_parse_flex_grow_property(css_style_parser_t *parser,
					const char *str)
{
	css_unit_value_t s;

	if (css_parse_number(&s, str)) {
		set_property(css_key_flex_grow, &s);
		return 0;
	}
	return -1;
}

static int css_parse_flex_shrink_property(css_style_parser_t *parser,
					  const char *str)
{
	css_unit_value_t s;

	if (css_parse_number(&s, str)) {
		set_property(css_key_flex_grow, &s);
		return 0;
	}
	return -1;
}

static int css_parse_font_style_property(css_style_parser_t *parser, const char *str)
{
	css_unit_value_t s;

	if (css_parse_font_style(str, &s.val_int)) {
		s.is_valid = TRUE;
		s.unit = CSS_UNIT_INT;
		set_current_property(&s);
		return 0;
	}
	return -1;
}

static int css_parse_font_weight_property(css_style_parser_t *parser, const char *str)
{
	css_unit_value_t s;

	if (css_parse_font_weight(str, &s.val_int)) {
		s.is_valid = TRUE;
		s.unit = CSS_UNIT_INT;
		set_current_property(&s);
		return 0;
	}
	return -1;
}

static int css_parse_text_value(css_style_parser_t *parser, const char *str)
{
	size_t len;
	css_unit_value_t s;

	len = strlen(str);
	if (len < 1 || (str[0] == '"' && str[len - 1] != '"')) {
		return -1;
	}
	s.is_valid = TRUE;
	s.unit = CSS_UNIT_STRING;
	s.val_string = malloc(sizeof(char) * (len + 1));
	if (!s.val_string) {
		return -1;
	}
	strcpy(s.val_string, str);
	set_current_property(&s);
	return 0;
}

css_property_parser_t *css_get_property_parser(const char *name)
{
	return dict_fetch_value(css_property_parser.dict, name);
}

static void css_prop_parser_dict_val_destructor(void *privdata, void *val)
{
	css_property_parser_t *sp = val;
	free(sp->name);
	free(sp);
}

int css_register_property_parser(int key, const char *name,
				 css_property_parser_method_t parse)
{
	css_property_parser_t *parser;

	if (!name && key >= 0) {
		name = css_get_property_name(key);
	}
	if (!name || strlen(name) < 1) {
		return -1;
	}
	if (dict_fetch_value(css_property_parser.dict, name)) {
		return -2;
	}
	parser = malloc(sizeof(css_property_parser_t));
	if (!parser) {
		return -ENOMEM;
	}
	parser->key = key;
	parser->name = strdup2(name);
	parser->parse = parse;
	css_property_parser.count += 1;
	dict_add(css_property_parser.dict, parser->name, parser);
	return 0;
}

#define register_parser css_register_property_parser

void css_init_preset_property_parsers(void)
{
	static dict_type_t dt;

	dict_init_string_key_type(&dt);
	dt.val_destructor = css_prop_parser_dict_val_destructor;
	css_property_parser.dict = dict_create(&dt, NULL);
	css_property_parser.count = 0;

	register_parser(css_key_width, NULL, css_parse_number_value);
	register_parser(css_key_height, NULL, css_parse_number_value);
	register_parser(css_key_min_width, NULL, css_parse_number_value);
	register_parser(css_key_min_height, NULL, css_parse_number_value);
	register_parser(css_key_max_width, NULL, css_parse_number_value);
	register_parser(css_key_max_height, NULL, css_parse_number_value);
	register_parser(css_key_top, NULL, css_parse_number_value);
	register_parser(css_key_right, NULL, css_parse_number_value);
	register_parser(css_key_bottom, NULL, css_parse_number_value);
	register_parser(css_key_left, NULL, css_parse_number_value);
	register_parser(css_key_z_index, NULL, css_parse_value);
	register_parser(css_key_opacity, NULL, css_parse_number_value);
	register_parser(css_key_position, NULL, css_parse_keyword_value);
	register_parser(css_key_visibility, NULL,
			css_parse_visibility_property);
	register_parser(css_key_vertical_align, NULL, css_parse_keyword_value);
	register_parser(css_key_display, NULL, css_parse_keyword_value);
	register_parser(css_key_background_color, NULL, css_parse_color_value);
	register_parser(css_key_background_image, NULL, css_parse_image_value);
	register_parser(css_key_background_position, NULL,
			css_parse_background_position_property);
	register_parser(css_key_background_size, NULL,
			css_parse_background_size_property);
	register_parser(css_key_background_repeat, NULL,
			css_parse_background_repeat_property);
	register_parser(css_key_border_top_color, NULL, css_parse_color_value);
	register_parser(css_key_border_right_color, NULL,
			css_parse_color_value);
	register_parser(css_key_border_bottom_color, NULL,
			css_parse_color_value);
	register_parser(css_key_border_left_color, NULL, css_parse_color_value);
	register_parser(css_key_border_top_width, NULL, css_parse_number_value);
	register_parser(css_key_border_right_width, NULL,
			css_parse_number_value);
	register_parser(css_key_border_bottom_width, NULL,
			css_parse_number_value);
	register_parser(css_key_border_left_width, NULL,
			css_parse_number_value);
	register_parser(css_key_border_top_style, NULL,
			css_parse_keyword_value);
	register_parser(css_key_border_right_style, NULL,
			css_parse_keyword_value);
	register_parser(css_key_border_bottom_style, NULL,
			css_parse_keyword_value);
	register_parser(css_key_border_left_style, NULL,
			css_parse_keyword_value);
	register_parser(css_key_border_top_left_radius, NULL,
			css_parse_number_value);
	register_parser(css_key_border_top_right_radius, NULL,
			css_parse_number_value);
	register_parser(css_key_border_bottom_left_radius, NULL,
			css_parse_number_value);
	register_parser(css_key_border_bottom_right_radius, NULL,
			css_parse_number_value);
	register_parser(css_key_padding_top, NULL, css_parse_number_value);
	register_parser(css_key_padding_right, NULL, css_parse_number_value);
	register_parser(css_key_padding_bottom, NULL, css_parse_number_value);
	register_parser(css_key_padding_left, NULL, css_parse_number_value);
	register_parser(css_key_margin_top, NULL, css_parse_number_value);
	register_parser(css_key_margin_right, NULL, css_parse_number_value);
	register_parser(css_key_margin_bottom, NULL, css_parse_number_value);
	register_parser(css_key_margin_left, NULL, css_parse_number_value);
	register_parser(css_key_focusable, NULL, css_parse_boolean_value);
	register_parser(css_key_pointer_events, NULL, css_parse_keyword_value);
	register_parser(css_key_box_sizing, NULL, css_parse_keyword_value);

	register_parser(css_key_flex_basis, NULL,
			css_parse_flex_basis_property);
	register_parser(css_key_flex_grow, NULL, css_parse_flex_grow_property);
	register_parser(css_key_flex_shrink, NULL,
			css_parse_flex_shrink_property);
	register_parser(css_key_flex_direction, NULL, css_parse_keyword_value);
	register_parser(css_key_flex_wrap, NULL, css_parse_keyword_value);
	register_parser(css_key_justify_content, NULL, css_parse_keyword_value);
	register_parser(css_key_align_content, NULL, css_parse_keyword_value);
	register_parser(css_key_align_items, NULL, css_parse_keyword_value);

	// css properties for text rendering
	register_parser(css_key_color, NULL, css_parse_color_value);
	register_parser(css_key_font_family, NULL, css_parse_string_value);
	register_parser(css_key_font_size, NULL, css_parse_number_value);
	register_parser(css_key_font_style, NULL, css_parse_font_style_property);
	register_parser(css_key_text_align, NULL, css_parse_keyword_value);
	register_parser(css_key_line_height, NULL, css_parse_number_value);
	register_parser(css_key_content, NULL, css_parse_text_value);
	register_parser(css_key_white_space, NULL, css_parse_keyword_value);

	register_parser(-1, "border", css_parse_border_property);
	register_parser(-1, "border-left", css_parse_border_left_property);
	register_parser(-1, "border-top", css_parse_border_top_property);
	register_parser(-1, "border-right", css_parse_border_right_property);
	register_parser(-1, "border-bottom", css_parse_border_bottom_property);
	register_parser(-1, "border-color", css_parse_border_color_property);
	register_parser(-1, "border-width", css_parse_border_width_property);
	register_parser(-1, "border-style", css_parse_border_style_property);
	register_parser(-1, "border-radius", css_parse_border_radius_property);
	register_parser(-1, "padding", css_parse_padding_property);
	register_parser(-1, "margin", css_parse_margin_property);
	register_parser(-1, "box-shadow", css_parse_box_shadow_property);
	register_parser(-1, "background", css_parse_background_property);
	register_parser(-1, "flex-flow", css_parse_flex_flow_property);
	register_parser(-1, "flex", css_parse_flex_property);
}

void css_destroy_preset_property_parsers(void)
{
	dict_destroy(css_property_parser.dict);
	css_property_parser.dict = NULL;
	css_property_parser.count = 0;
}
