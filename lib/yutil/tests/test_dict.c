#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "test.h"
#include "libtest.h"
#include "yutil/keywords.h"
#include "yutil/dict.h"
#include "yutil/time.h"

uint64_t hash_callback(const void *key)
{
	return dict_gen_hash_function((unsigned char *)key,
				      (int)strlen((char *)key));
}

int compare_callback(void *privdata, const void *key1, const void *key2)
{
	(void)privdata;
	int l1, l2;

	l1 = (int)strlen((char *)key1);
	l2 = (int)strlen((char *)key2);
	if (l1 != l2)
		return 0;
	return memcmp(key1, key2, l1) == 0;
}

void free_callback(void *privdata, void *val)
{
	(void)(privdata);

	free(val);
}

char *string_from_long_long(long long value)
{
	char buf[32];
	int len;
	char *s;

	len = sprintf(buf, "%lld", value);
	s = (char *)malloc(len + 1);
	memcpy(s, buf, len);
	s[len] = '\0';
	return s;
}

dict_type_t BenchmarkDictType = { hash_callback, NULL, NULL, compare_callback,
				  free_callback, NULL, NULL };

#define start_benchmark() start = get_time_ms()
#define end_benchmark(msg)                                              \
	do {                                                            \
		elapsed = get_time_ms() - start;                        \
		printf(msg ": %ld items in %lld ms\n", count, elapsed); \
	} while (0)

/* ./redis-server test dict [<count> | --accurate] */
void test_dict(void)
{
	long long j;
	long long start, elapsed;
	dict_t *dict = dict_create(&BenchmarkDictType, NULL);
	long count = 5000;

	start_benchmark();
	for (j = 0; j < count; j++) {
		int retval =
		    dict_add(dict, string_from_long_long(j), (void *)j);
		assert(retval == DICT_OK);
	}
	end_benchmark("Inserting");
	assert((long)dict_size(dict) == count);

	/* Wait for rehashing. */
	while (dict_is_rehashing(dict)) {
		dict_rehash_milliseconds(dict, 100);
	}

	start_benchmark();
	for (j = 0; j < count; j++) {
		char *key = string_from_long_long(j);
		dict_entry_t *de = dict_find(dict, key);
		assert(de != NULL);
		free(key);
	}
	end_benchmark("Linear access of existing elements");

	start_benchmark();
	for (j = 0; j < count; j++) {
		char *key = string_from_long_long(j);
		dict_entry_t *de = dict_find(dict, key);
		assert(de != NULL);
		free(key);
	}
	end_benchmark("Linear access of existing elements (2nd round)");

	start_benchmark();
	for (j = 0; j < count; j++) {
		char *key = string_from_long_long(rand() % count);
		dict_entry_t *de = dict_find(dict, key);
		assert(de != NULL);
		free(key);
	}
	end_benchmark("Random access of existing elements");

	start_benchmark();
	for (j = 0; j < count; j++) {
		dict_entry_t *de = dict_get_random_key(dict);
		assert(de != NULL);
	}
	end_benchmark("Accessing random keys");

	start_benchmark();
	for (j = 0; j < count; j++) {
		char *key = string_from_long_long(rand() % count);
		key[0] = 'X';
		dict_entry_t *de = dict_find(dict, key);
		assert(de == NULL);
		free(key);
	}
	end_benchmark("Accessing missing");

	start_benchmark();
	for (j = 0; j < count; j++) {
		char *key = string_from_long_long(j);
		int retval = dict_delete(dict, key);
		assert(retval == DICT_OK);
		key[0] += 17; /* Change first number to letter. */
		retval = (int)dict_add(dict, (void *)key, (void *)j);
		assert(retval == DICT_OK);
	}
	end_benchmark("Removing and adding");
	dict_destroy(dict);
}