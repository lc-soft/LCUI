/* Hash Tables Implementation.
 *
 * This file implements in-memory hash tables with insert/del/replace/find/
 * get-random-element operations. Hash tables will auto-resize if needed
 * tables of power of two in size are used, collisions are handled by
 * chaining. See the source code for more information... :)
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * Copyright (c) 2021, Li Zihao <yidianyiko@foxmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef UTIL_DICT_H
#define UTIL_DICT_H

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

Y_BEGIN_DECLS

/* ------------------------------- Types ------------------------------------*/
/** the dictionary entry type */
typedef struct dict_entry_t dict_entry_t;

/** the data in dictionary type */
typedef struct dict_type_t dict_type_t;

/** the dictionary hash map type */
typedef struct dict_hash_map_t dict_hash_map_t;

/** the dictionary type */
typedef struct dict_t dict_t;

/** the dictionary safe iterator type */
typedef struct dict_iterator_t dict_iterator_t;

typedef void(dict_scan_function)(void *priv_data, const dict_entry_t *de);
typedef void(dict_scan_bucket_function)(void *priv_data,
					dict_entry_t **bucket_ref);

struct dict_entry_t {
	void *key;
	union {
		void *val;
		uint64_t u64;
		int64_t s64;
		double d;
	} v;
	struct dict_entry_t *next;
};

struct dict_type_t {
	uint64_t (*hash_function)(const void *key);
	void *(*key_dup)(void *priv_data, const void *key);
	void *(*val_dup)(void *priv_data, const void *obj);
	int (*key_compare)(void *priv_data, const void *key1, const void *key2);
	void (*key_destructor)(void *priv_data, void *key);
	void (*val_destructor)(void *priv_data, void *obj);
	int (*expand_allowed)(size_t more_mem, double used_ratio);
};

/* This is our hash table structure. Every dict has two of this as we
 * implement incremental rehashing, for the old to the new table. */
struct dict_hash_map_t {
	dict_entry_t **table;
	unsigned long size;
	unsigned long size_mask;
	unsigned long used;
};

struct dict_t {
	dict_type_t *type;
	void *priv_data;
	dict_hash_map_t ht[2];
	long rehash_index; /* rehashing not in progress if rehash_index == -1 */
	int16_t pause_rehash; /* If >0 rehashing is paused (<0 indicates coding
				 error) */
};

/* If safe is set to 1 this is a safe iterator, that means, you can call
 * dict_add, dict_find, and other functions against the dict even while
 * iterating. Otherwise it is a non safe iterator, and only dict_next()
 * should be called while iterating. */
struct dict_iterator_t {
	dict_t *d;
	long index;
	int table, safe;
	dict_entry_t *entry, *next_entry;
	/* unsafe iterator fingerprint for misuse detection. */
	long long fingerprint;
};

/* ------------------------------- Macros ------------------------------------*/
#define DICT_OK 0
#define DICT_ERR 1

#define dict_free_val(d, entry)        \
	if ((d)->type->val_destructor) \
	(d)->type->val_destructor((d)->priv_data, (entry)->v.val)

#define dict_set_val(d, entry, _val_)                                  \
	do {                                                           \
		if ((d)->type->val_dup)                                \
			(entry)->v.val =                               \
			    (d)->type->val_dup((d)->priv_data, _val_); \
		else                                                   \
			(entry)->v.val = (_val_);                      \
	} while (0)

#define dict_set_signed_integer_val(entry, _val_) \
	do {                                      \
		(entry)->v.s64 = _val_;           \
	} while (0)

#define dict_set_unsigned_integer_val(entry, _val_) \
	do {                                        \
		(entry)->v.u64 = _val_;             \
	} while (0)

#define dict_set_double_val(entry, _val_) \
	do {                              \
		(entry)->v.d = _val_;     \
	} while (0)

#define dict_free_key(d, entry)        \
	if ((d)->type->key_destructor) \
	(d)->type->key_destructor((d)->priv_data, (entry)->key)

#define dict_set_key(d, entry, _key_)                                  \
	do {                                                           \
		if ((d)->type->key_dup)                                \
			(entry)->key =                                 \
			    (d)->type->key_dup((d)->priv_data, _key_); \
		else                                                   \
			(entry)->key = (_key_);                        \
	} while (0)

#define dict_compare_keys(d, key1, key2)                          \
	(((d)->type->key_compare)                                 \
	     ? (d)->type->key_compare((d)->priv_data, key1, key2) \
	     : (key1) == (key2))

#define dict_hash_key(d, key) (d)->type->hash_function(key)
#define dict_get_key(he) ((he)->key)
#define dict_get_val(he) ((he)->v.val)
#define dict_get_signed_integer_val(he) ((he)->v.s64)
#define dict_get_unsigned_integer_val(he) ((he)->v.u64)
#define dict_get_double_val(he) ((he)->v.d)
#define dict_slots(d) ((d)->ht[0].size + (d)->ht[1].size)
#define dict_size(d) ((d)->ht[0].used + (d)->ht[1].used)
#define dict_is_rehashing(d) ((d)->rehash_index != -1)
#define dict_pause_rehashing(d) (d)->pause_rehash++
#define dict_resume_rehashing(d) (d)->pause_rehash--

/* If our unsigned long type can store a 64 bit number, use a 64 bit PRNG. */
#define random_u_long() rand()

/* API */
YUTIL_API dict_t *dict_create(dict_type_t *type, void *priv_data_ptr);
YUTIL_API int dict_expand(dict_t *d, unsigned long size);
YUTIL_API int dict_try_expand(dict_t *d, unsigned long size);
YUTIL_API int dict_add(dict_t *d, void *key, void *val);
YUTIL_API dict_entry_t *dict_add_raw(dict_t *d, void *key, dict_entry_t **existing);
YUTIL_API dict_entry_t *dict_add_or_find(dict_t *d, void *key);
YUTIL_API int dict_replace(dict_t *d, void *key, void *val);
YUTIL_API int dict_delete(dict_t *d, const void *key);
YUTIL_API dict_entry_t *dict_unlink(dict_t *ht, const void *key);
YUTIL_API void dict_free_unlinked_entry(dict_t *d, dict_entry_t *he);
YUTIL_API void dict_destroy(dict_t *d);
YUTIL_API dict_entry_t *dict_find(dict_t *d, const void *key);
YUTIL_API void *dict_fetch_value(dict_t *d, const void *key);
YUTIL_API int dict_resize(dict_t *d);
YUTIL_API dict_iterator_t *dict_get_iterator(dict_t *d);
YUTIL_API dict_iterator_t *dict_get_safe_iterator(dict_t *d);
YUTIL_API dict_entry_t *dict_next(dict_iterator_t *iter);
YUTIL_API void dict_destroy_iterator(dict_iterator_t *iter);
YUTIL_API dict_entry_t *dict_get_random_key(dict_t *d);
YUTIL_API dict_entry_t *dict_get_fair_random_key(dict_t *d);
YUTIL_API unsigned int dict_get_some_keys(dict_t *d, dict_entry_t **des,
				unsigned int count);
YUTIL_API void dict_get_stats(char *buf, size_t buf_size, dict_t *d);
YUTIL_API uint64_t dict_gen_hash_function(const void *key, int len);
YUTIL_API uint64_t dict_gen_case_hash_function(const unsigned char *buf, int len);
YUTIL_API void dict_empty(dict_t *d, void(callback)(void *));
YUTIL_API void dict_enable_resize(void);
YUTIL_API void dict_disable_resize(void);
YUTIL_API int dict_rehash(dict_t *d, int n);
YUTIL_API int dict_rehash_milliseconds(dict_t *d, int ms);
YUTIL_API void dict_set_hash_function_seed(uint8_t seed);
YUTIL_API uint8_t dict_get_hash_function_seed(void);
YUTIL_API unsigned long dict_scan(dict_t *d, unsigned long v, dict_scan_function *fn,
			dict_scan_bucket_function *bucketfn, void *priv_data);
YUTIL_API uint64_t dict_get_hash(dict_t *d, const void *key);
YUTIL_API dict_entry_t **dict_find_entry_ref_by_ptr_and_hash(dict_t *d,
						   const void *old_ptr,
						   uint64_t hash);

YUTIL_API uint64_t dict_string_hash(const void *key);
YUTIL_API int dict_string_key_compare(void *privdata, const void *key1, const void *key2);
YUTIL_API void *dict_string_key_dup(void *privdata, const void *key);
YUTIL_API void dict_string_key_destructor(void *privdata, void *key);
YUTIL_API void dict_init_string_key_type(dict_type_t *t);
YUTIL_API void dict_init_string_copy_key_type(dict_type_t *t);

Y_END_DECLS

#endif /* UTIL_DICT_H */
