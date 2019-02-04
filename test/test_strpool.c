#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/util/logger.h>
#include <LCUI/util/strpool.h>
#include "test.h"

int test_strpool(void)
{
	int ret = 0;
	char *str1, *str2;
	strpool_t *pool;

	CHECK(pool = strpool_create());
	CHECK(str1 = strpool_alloc_str(pool, "hello"));
	CHECK(str2 = strpool_alloc_str(pool, "hello"));
	CHECK(str1 == str2);
	CHECK(strpool_size(pool) > 0);
	CHECK(strpool_free_str(str1) == 0);
	CHECK(strcmp(str1, "hello") == 0);
	CHECK(strpool_free_str(str2) == 0);
	CHECK(strpool_size(pool) == 0);
	strpool_destroy(pool);
	return ret;
}
