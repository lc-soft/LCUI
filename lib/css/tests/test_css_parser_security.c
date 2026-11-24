#include <string.h>
#include <css.h>
#include <ctest.h>

/* Parse in 511-byte chunks to mirror the input pattern of ui_load_css_file() */
static int parse_chunked_css(const char *space, const char *css)
{
        char buff[512];
        size_t len = strlen(css);
        size_t offset = 0;
        size_t n;
        css_parser_t *parser;

        parser = css_parser_create(space);
        while (offset < len) {
                n = len - offset;
                if (n > 511) {
                        n = 511;
                }
                memcpy(buff, css + offset, n);
                buff[n] = 0;
                css_parser_parse(parser, buff);
                offset += n;
        }
        css_parser_destroy(parser);
        return (int)css_remove_style_rules(space);
}

static void check_selector_spanning_chunks(void)
{
        char css[640];
        int count;

        memset(css, 'a', 600);
        strcpy(css + 600, "{display:block;}");
        count = parse_chunked_css("parser-security/chunked", css);
        ctest_equal_int("should reject selector spanning chunks", count, 0);
}

static void check_selector_name_over_limit(void)
{
        char css[300];
        int count;

        memset(css, 'a', 256);
        strcpy(css + 256, "{display:block;}");
        count = parse_chunked_css("parser-security/long-name", css);
        ctest_equal_int("should reject selector name over 255 chars", count, 0);
}

static void check_selector_name_at_limit(void)
{
        char css[300];
        int count;

        memset(css, 'a', 255);
        strcpy(css + 255, "{display:block;}");
        count = parse_chunked_css("parser-security/limit", css);
        ctest_equal_int("should accept selector name of 255 chars", count, 1);
}

static void check_class_selector_over_limit(void)
{
        char css[440];
        int count;

        memset(css, 'a', 200);
        css[200] = '.';
        memset(css + 201, 'b', 200);
        strcpy(css + 401, "{display:block;}");
        count = parse_chunked_css("parser-security/classes", css);
        ctest_equal_int("should survive overlong class selector", count, 1);
}

void test_css_parser_security(void)
{
        css_init();
        ctest_describe("oversized selector", check_selector_spanning_chunks);
        ctest_describe("selector name over limit",
                       check_selector_name_over_limit);
        ctest_describe("selector name at limit", check_selector_name_at_limit);
        ctest_describe("class selector over limit",
                       check_class_selector_over_limit);
}
