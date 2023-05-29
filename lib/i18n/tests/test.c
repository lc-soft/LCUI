#include <i18n.h>
#include <ctest.h>

void test_i18n(void)
{
        size_t len;
        wchar_t lang[32];

        len = i18n_detect_language(lang, 32);
        ctest_euqal_bool("i18n_detect_language()", len > 0, true);
        ctest_euqal_bool(
            "i18n_load_language(L\"zh-CN\", \"locales/zh-CN.yml\")",
            i18n_load_language(L"zh-CN", "locales/zh-CN.yml"), true);

        ctest_euqal_bool("i18n_load_language(L\"en\", \"locales/en.yml\")",
                         i18n_load_language(L"en", "locales/en.yml"),
                         true);

        ctest_euqal_bool(
            "i18n_load_language(L\"notfound\", \"locales/notfound.yml\")",
            i18n_load_language(L"notfound", "locales/notfound.yml"), false);

	i18n_change_language(L"en");

        ctest_euqal_bool("i18n_translate(L\"name\") == L\"English\")",
                         wcscmp(i18n_translate(L"name"), L"English") == 0,
                         true);

        ctest_euqal_bool("i18n_translate(L\"button.ok\") == L\"Ok\")",
                         wcscmp(i18n_translate(L"button.ok"), L"Ok") == 0,
                         true);

        ctest_euqal_bool("i18n_translate(L\"button.notfound\") == NULL)",
                         i18n_translate(L"button.notfound")== NULL,
                         true);

	i18n_change_language(L"zh-CN");

        ctest_euqal_bool("i18n_translate(L\"name\") == L\"中文\")",
                         wcscmp(i18n_translate(L"name"), L"中文") == 0,
                         true);

        ctest_euqal_bool("i18n_translate(L\"button.ok\") == L\"确定\")",
                         wcscmp(i18n_translate(L"button.ok"), L"确定") == 0,
                         true);

        ctest_euqal_bool("i18n_translate(L\"button.notfound\") == NULL)",
                         i18n_translate(L"button.notfound")== NULL,
                         true);
	i18n_clear();
}

int main(void)
{
        logger_set_level(LOGGER_LEVEL_OFF);
        ctest_describe("i18n", test_i18n);
        return ctest_finish();
}
