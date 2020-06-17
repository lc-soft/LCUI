#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include "test.h"
#include "libtest.h"

static void test_cmdsplit(void)
{
	char **argv;
	int argc = 0;
	const char *cmd = "\"C:\\Program Files\\WindowsApps\\Test\\app.exe\" "
			  "--print   \"Hello, Wolrd !\"";
	const char *results[3] = {
		"\"C:\\Program Files\\WindowsApps\\Test\\app.exe\"", "--print",
		"\"Hello, Wolrd !\""
	};
	it_i("executable in command only", cmdsplit("  test.exe   ", NULL), 1);
	it_i("command with two parameters", cmdsplit(cmd, NULL), 3);
	argc = cmdsplit(cmd, &argv);
	char messageBuffer[1024];
	while (argc-- > 0) {
		snprintf(messageBuffer, sizeof(messageBuffer),
			 "check argv[%d]: check [%s] == [%s]", argc, argv[argc],
			 results[argc]);
		it_s(messageBuffer, argv[argc], results[argc]);
		free(argv[argc]);
	}
	free(argv);
}

void test_strs(void)
{
	char **strs = NULL;
	strlist_add(&strs, "first-child");
	strlist_add(&strs, "last-child");
	strlist_add(&strs, "one two");
	it_b("check first-child is in the list",
	     strlist_has(strs, "first-child"), TRUE);
	it_b("check last-child is in the list", strlist_has(strs, "last-child"),
	     TRUE);
	it_b("check one is in the list", strlist_has(strs, "one"), TRUE);
	it_b("check two is in the list", strlist_has(strs, "two"), TRUE);
	it_b("check remove(last_child)", strlist_remove(&strs, "last-child"),
	     TRUE);
	it_b("check remove(first-child", strlist_remove(&strs, "first-child"),
	     TRUE);
	it_b("check remove(one)", strlist_remove(&strs, "one"), TRUE);
	it_b("check first-child is removed", strlist_has(strs, "first-child"),
	     FALSE);
	it_b("check one is removed", strlist_has(strs, "one"), FALSE);
	it_b("check two is still in the list", strlist_has(strs, "two"), TRUE);
	it_i("check strhash(123, \"123\") is consistent", strhash(123, "123"),
	     strhash(123, "123"));
	it_b("check strhash(123, \"123\") != strhash(123, \"312\")",
	     strhash(123, "123") == strhash(123, "312"), FALSE);
	it_b("check strhash(100, \"123\") != strhash(123, \"123\")",
	     strhash(100, "123") == strhash(123, "123"), FALSE);
	it_b("check adding first-child", strlist_add(&strs, "first-child"),
	     TRUE);
	strlist_free(strs);
}

void test_string(void)
{
	describe("test cmdsplit", test_cmdsplit);
	describe("test strs", test_strs);
}
