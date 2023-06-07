
#include "i18n-private.h"

static struct i18n_module_t {
        size_t length;       /**< 语言列表的长度 */
        wchar_t **languages; /**< 语言列表 */
        wchar_t *language;   /**< 当前选中的语言 */
        dict_t *locales;     /**< 文本库 */
} i18n = { 0 };

static bool i18n_add_language(const wchar_t *name, dict_t *dict)
{
        size_t i, pos;
        size_t len;
        wchar_t **langs;

        len = i18n.length + 2;
        langs = realloc(i18n.languages, sizeof(wchar_t *) * len);
        if (langs == NULL) {
                return false;
        }
        langs[len - 1] = NULL;
        for (i = 0, pos = i18n.length; i < i18n.length; ++i) {
                if (wcscmp(name, langs[i]) < 0) {
                        pos = i;
                        break;
                }
        }
        for (i = i18n.length; i > pos; --i) {
                langs[i] = langs[i - 1];
        }
        langs[pos] = wcsdup2(name);
        i18n.languages = langs;
        i18n.length += 1;
        if (i18n.locales == NULL) {
                i18n.locales = i18n_dict_create();
        }
        return i18n_dict_add_dict(i18n.locales, name, dict);
}

void i18n_clear(void)
{
        size_t i;

        if (i18n.languages) {
                for (i = 0; i < i18n.length; ++i) {
                        free(i18n.languages[i]);
                        i18n.languages[i] = NULL;
                }
                free(i18n.languages);
        }
        if (i18n.locales) {
                dict_destroy(i18n.locales);
        }
        i18n.languages = NULL;
        i18n.language = NULL;
        i18n.locales = NULL;
        i18n.length = 0;
}

bool i18n_load_language(const wchar_t *name, const char *filename)
{
        dict_t *dict = i18n_load_yaml_file(filename);
        return dict == NULL ? false : i18n_add_language(name, dict);
}

const wchar_t *i18n_translate(const wchar_t *text)
{
        size_t i = 0;
        const wchar_t *p = text;
        wchar_t key[I18N_MAX_KEY_LEN];
        dict_t *dict;
        dict_value_t *value;

        if (i18n.locales == NULL || i18n.language == NULL) {
                return NULL;
        }
        value = dict_fetch_value(i18n.locales, i18n.language);
        if (!value || value->type != DICT) {
                return NULL;
        }
        dict = value->dict;
        for (value = NULL; *p && i < I18N_MAX_KEY_LEN; ++p) {
                if (*p != L'.') {
                        key[i++] = *p;
                        continue;
                }
                key[i] = 0;
                value = dict_fetch_value(dict, key);
                if (value && value->type == DICT) {
                        dict = value->dict;
                } else {
                        return NULL;
                }
                i = 0;
        }
        if (i > 0) {
                key[i] = 0;
                value = dict_fetch_value(dict, key);
        }
        if (value && value->type == STRING) {
                return value->string.data;
        }
        return NULL;
}

size_t i18n_get_languages(wchar_t ***languages)
{
        *languages = i18n.languages;
        return i18n.length;
}

bool i18n_change_language(const wchar_t *name)
{
        size_t i;
        wchar_t *lang;

        for (i = 0; i < i18n.length; ++i) {
                lang = i18n.languages[i];
                if (wcscmp(name, lang) == 0) {
                        i18n.language = lang;
                        return true;
                }
        }
        return false;
}

#ifdef _WIN32
#include <Windows.h>
#endif

size_t i18n_detect_language(wchar_t *lang, size_t max_len)
{
#ifdef _WIN32
        return (size_t)GetUserDefaultLocaleName(lang, (int)max_len);
#else
        size_t len;
        const char *str = getenv("LANG");

        if (str) {
                len = decode_utf8(lang, str, max_len);
                // zh_CN -> zh-CN
                if (len > 3 && max_len > 3) {
                        lang[2] = '-';
                }
                return len;
        }
#endif
        return wcslen(wcsncpy(lang, L"en-US", max_len));
}
