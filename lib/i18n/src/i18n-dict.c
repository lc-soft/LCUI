#include "i18n-private.h"

static uint64_t i18n_dict_hash(const void *key)
{
        return dict_gen_hash_function(key, (int)wcslen((const wchar_t *)key));
}

static int i18n_dict_key_compare(void *privdata, const void *key1,
                                 const void *key2)
{
        return wcscmp((const wchar_t *)key1, (const wchar_t *)key2) == 0;
}

static void i18n_dict_on_destroy_value(void *privdata, void *data)
{
        dict_value_t *value = data;
        if (value->type == DICT) {
                dict_destroy(value->dict);
        } else if (value->type == STRING) {
                free(value->string.data);
        }
        if (value->key) {
                free(value->key);
        }
        free(data);
}

dict_t *i18n_dict_create(void)
{
        static dict_type_t dict_type = { .key_compare = i18n_dict_key_compare,
                                         .hash_function = i18n_dict_hash,
                                         .val_destructor =
                                             i18n_dict_on_destroy_value,
                                         0 };
        return dict_create(&dict_type, NULL);
}

bool i18n_dict_add_dict(dict_t *dict, const wchar_t *key, dict_t *child_dict)
{
        dict_value_t *value;

        value = malloc(sizeof(dict_value_t));
        value->type = DICT;
        value->parent_dict = dict;
        value->dict = child_dict;
        value->key = wcsdup2(key);
        return dict_add(dict, (void *)value->key, value) == DICT_OK;
}
