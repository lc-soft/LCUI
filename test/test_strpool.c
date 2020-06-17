#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/util/logger.h>
#include <LCUI/util/strpool.h>
#include "test.h"
#include "libtest.h"

void test_strpool(void)
{
	char *str1, *str2;
	strpool_t *pool;

	it_b("check strpool_create()", (pool = strpool_create()) != NULL, TRUE);
	it_b("check strpool_alloc",
	     (str1 = strpool_alloc_str(pool, "hello")) != NULL, TRUE);
	it_b("check strpool_alloc of already pooled string",
	     (str2 = strpool_alloc_str(pool, "hello")) != NULL, TRUE);
	it_b("check string reused", str1 == str2, TRUE);
	it_b("check strpool_size", strpool_size(pool) > 0, TRUE);
	it_i("check release str1", strpool_free_str(str1), 0);
	it_s("check str1 is still valid", str1, "hello");
	it_i("check release str2", strpool_free_str(str2), 0);
	it_i("check strpool is empty", strpool_size(pool), 0);
	strpool_destroy(pool);
}
