#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include "test.h"

static int test_cmdsplit(void)
{
	char **argv;
	int argc, ret = 0;
	const char *cmd = "\"C:\\Program Files\\WindowsApps\\Test\\app.exe\" "
			  "--print   \"Hello, Wolrd !\"";
	const char *results[3] = {
		"\"C:\\Program Files\\WindowsApps\\Test\\app.exe\"", "--print",
		"\"Hello, Wolrd !\""
	};
	CHECK(cmdsplit("  test.exe   ", NULL) == 1);
	CHECK(cmdsplit(cmd, NULL) == 3);
	argc = cmdsplit(cmd, &argv);
	while (argc-- > 0) {
		TEST_LOG("argv[%d]: check [%s] == [%s]\n", argc, argv[argc],
			 results[argc]);
		CHECK(strcmp(argv[argc], results[argc]) == 0);
		free(argv[argc]);
	}
	free(argv);
	return ret;
}

int test_strs(void)
{
	int ret = 0;
	char **strs = NULL;
	strlist_add(&strs, "first-child");
	strlist_add(&strs, "last-child");
	strlist_add(&strs, "one two");
	CHECK(strlist_has(strs, "first-child"));
	CHECK(strlist_has(strs, "last-child"));
	CHECK(strlist_has(strs, "one"));
	CHECK(strlist_has(strs, "two"));
	CHECK(strlist_remove(&strs, "last-child"));
	CHECK(strlist_remove(&strs, "first-child"));
	CHECK(strlist_remove(&strs, "one"));
	CHECK(!strlist_has(strs, "first-child"));
	CHECK(!strlist_has(strs, "one"));
	CHECK(strlist_has(strs, "two"));
	CHECK(strlist_add(&strs, "first-child"));
	strlist_free(strs);
	return ret;
}

int test_string(void)
{
	int ret = 0;
	ret += test_cmdsplit();
	ret += test_strs();
	return ret;
}
