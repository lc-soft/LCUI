#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "test.h"
#include "libtest.h"
#include "yutil/keywords.h"
#include "yutil/string.h"
#include "yutil/strlist.h"

void test_strs(void)
{
        char **strs = NULL;
        strlist_add(&strs, "first-child");
        strlist_add(&strs, "last-child");
        strlist_add(&strs, "one two");
        it_b("check first-child is in the list",
             strlist_has(strs, "first-child"), true);
        it_b("check last-child is in the list", strlist_has(strs, "last-child"),
             true);
        it_b("check one is in the list", strlist_has(strs, "one"), true);
        it_b("check two is in the list", strlist_has(strs, "two"), true);
        it_b("check remove(last_child)", strlist_remove(&strs, "last-child"),
             true);
        it_b("check remove(first-child", strlist_remove(&strs, "first-child"),
             true);
        it_b("check remove(one)", strlist_remove(&strs, "one"), true);
        it_b("check first-child is removed", strlist_has(strs, "first-child"),
             false);
        it_b("check one is removed", strlist_has(strs, "one"), false);
        it_b("check two is still in the list", strlist_has(strs, "two"), true);
        it_i("check y_strhash(123, \"123\") is consistent",
             y_strhash(123, "123"), y_strhash(123, "123"));
        it_b("check y_strhash(123, \"123\") != y_strhash(123, \"312\")",
             y_strhash(123, "123") == y_strhash(123, "312"), false);
        it_b("check y_strhash(100, \"123\") != y_strhash(123, \"123\")",
             y_strhash(100, "123") == y_strhash(123, "123"), false);
        it_b("check adding first-child", strlist_add(&strs, "first-child"),
             true);
        strlist_free(strs);
}

void test_string(void)
{
        describe("test strs", test_strs);
}
