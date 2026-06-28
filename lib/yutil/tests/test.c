#include "test.h"
#include "libtest.h"

int main()
{
        int ret = 0;

        describe("test_list", test_list);
        describe("test_math", test_math);
        describe("test_time", test_time);
        describe("test_logger", test_logger);
        describe("test_list_entry", test_list_entry);
        describe("test_dirent", test_dirent);
        describe("test_string", test_strs);
        describe("test_strpool", test_strpool);
        describe("test_encoding", test_encoding);
        describe("test_dict", test_dict);
        describe("test_rbtree", test_rbtree);
        return ret - print_test_result();
}
