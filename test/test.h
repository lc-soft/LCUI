#include <stdio.h>

#define TEST_LOG(format, ...) \
	printf("[test] %s(): " format, __FUNCTION__, ##__VA_ARGS__)

#define PRINT_TEST_RESULT(N) \
	printf("[test] %d tests, %d pass.\n", tests_count, tests_count + N)

#define CHECK(X)                                                          \
	do {                                                              \
		tests_count += 1;                                         \
		printf("[test] %s(): %s. # %s\n", __FUNCTION__, "" #X "", \
		       (X) ? "PASS" : (ret -= 1, "NO PASS!"));            \
	} while (0);

#define CHECK_WITH_TEXT(TEXT, X)                                      \
	do {                                                          \
		tests_count += 1;                                     \
		printf("[test] %s(): %s. # %s\n", __FUNCTION__, TEXT, \
		       (X) ? "PASS" : (ret -= 1, "NO PASS!"));        \
	} while (0);

#define CHECK2(X)                                                             \
	do {                                                                  \
		if (!(X)) {                                                   \
			printf("[test] %s(): %s. # NO PASS!\n", __FUNCTION__, \
			       "" #X "");                                     \
			ret -= 1;                                             \
		}                                                             \
	} while (0);

extern int tests_count;

int test_charset(void);
int test_string(void);
int test_object(void);
int test_thread(void);
int test_font_load(void);
int test_xml_parser(void);
int test_strpool(void);
int test_linkedlist(void);
int test_widget_rect(void);
int test_widget_opacity(void);
int test_widget_event(void);
int test_textview_resize(void);
int test_textedit(void);
int test_image_reader(void);

void test_css_parser(void);
void test_block_layout(void);
void test_flex_layout(void);
