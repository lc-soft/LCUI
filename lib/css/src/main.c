#include "../include/css.h"

void css_init(void)
{
	css_init_value_definitons();
	css_init_data_types();
	css_init_library();
	css_init_keywords();
	css_init_properties();
}

void css_destroy(void)
{
	css_destroy_properties();
	css_destroy_keywords();
	css_destroy_library();
	css_destroy_value_definitons();
}
