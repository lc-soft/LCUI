/* Hash Tables Implementation.
 *
 * This file implements in memory hash tables with insert/del/replace/find/
 * get-random-element operations. Hash tables will auto resize if needed
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
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <ctype.h>

#include "yutil/keywords.h"
#include "yutil/time.h"
#include "yutil/dict.h"

/* ------------------------------- Types ------------------------------------*/

/* ------------------------------- Macros ------------------------------------*/
/* This is the initial size of every hash table */
#define DICT_HT_INITIAL_SIZE 4
/* Unused arguments generate annoying warnings... */
#define dict_not_used(V) ((void)V)

#define time_in_milliseconds get_time_ms

/* Using dict_enable_resize() / dictDisable_resize() we make possible to
 * enable/disable resizing of the hash table as needed. */
static int dict_can_resize = 1;
static unsigned int dict_force_resize_ratio = 5;

/* -------------------------- private prototypes ---------------------------- */

static int _dict_expand_if_needed(dict_t *ht);
static unsigned long _dict_next_power(unsigned long size);
static long _dict_key_index(dict_t *ht, const void *key, uint64_t hash,
			    dict_entry_t **existing);
static int _dict_init(dict_t *ht, dict_type_t *type, void *priv_data_ptr);

/* -------------------------- hash functions -------------------------------- */

static uint8_t dict_hash_function_seed[16];

void dict_set_hash_function_seed(uint8_t seed)
{
	memcpy(dict_hash_function_seed, (const void *)(long long)seed,
	       sizeof(dict_hash_function_seed));
}

uint8_t dict_get_hash_function_seed(void)
{
	return (uint8_t)(long long)dict_hash_function_seed;
}

/* The default hashing function uses SipHash implementation
 * in siphash.c. */

uint64_t siphash(const uint8_t *in, const size_t inlen, const uint8_t *k);
uint64_t siphash_nocase(const uint8_t *in, const size_t inlen,
			const uint8_t *k);

uint64_t dict_gen_hash_function(const void *key, int len)
{
	return siphash((const uint8_t *)key, len, dict_hash_function_seed);
}

/* And a case insensitive version */
uint64_t dict_gen_case_hash_function(const unsigned char *buf, int len)
{
	return siphash_nocase((const uint8_t *)buf, len,
			      dict_hash_function_seed);
}

/* ----------------------------- API implementation ------------------------- */

/* Reset a hash table already initialized with ht_init().
 * NOTE: This function should only be called by ht_destroy(). */
static void _dict_reset(dict_hash_map_t *ht)
{
	ht->table = NULL;
	ht->size = 0;
	ht->size_mask = 0;
	ht->used = 0;
}

/* Create a new hash table */
dict_t *dict_create(dict_type_t *type, void *priv_data_ptr)
{
	dict_t *d = (dict_t *)malloc(sizeof(*d));

	_dict_init(d, type, priv_data_ptr);
	return d;
}

/* Initialize the hash table */
int _dict_init(dict_t *d, dict_type_t *type, void *priv_data_ptr)
{
	_dict_reset(&d->ht[0]);
	_dict_reset(&d->ht[1]);
	d->type = type;
	d->priv_data = priv_data_ptr;
	d->rehash_index = -1;
	d->pause_rehash = 0;
	return DICT_OK;
}

/* _resize the table to the minimal size that contains all the elements,
 * but with the invariant of a USED/BUCKETS ratio near to <= 1 */
int dict_resize(dict_t *d)
{
	unsigned long minimal;

	if (!dict_can_resize || dict_is_rehashing(d))
		return DICT_ERR;
	minimal = d->ht[0].used;
	if (minimal < DICT_HT_INITIAL_SIZE)
		minimal = DICT_HT_INITIAL_SIZE;
	return dict_expand(d, minimal);
}

/* Expand or create the hash table,
 * when malloc_failed is non-NULL, it'll avoid panic if malloc fails (in which
 * case it'll be set to 1). Returns DICT_OK if expand was performed, and
 * DICT_ERR if skipped. */
int _dict_expand(dict_t *d, unsigned long size, int *malloc_failed)
{
	if (malloc_failed)
		*malloc_failed = 0;

	/* the size is invalid if it is smaller than the number of
	 * elements already inside the hash table */
	if (dict_is_rehashing(d) || d->ht[0].used > size)
		return DICT_ERR;

	dict_hash_map_t n; /* the new hash table */
	unsigned long realsize = _dict_next_power(size);

	/* Rehashing to the same table size is not useful. */
	if (realsize == d->ht[0].size)
		return DICT_ERR;

	/* Allocate the new hash table and initialize all pointers to NULL */
	n.size = realsize;
	n.size_mask = realsize - 1;
	if (malloc_failed) {
		n.table = (dict_entry_t **)calloc(
		    1, realsize * sizeof(dict_entry_t *));
		*malloc_failed = n.table == NULL;
		if (*malloc_failed)
			return DICT_ERR;
	} else
		n.table = (dict_entry_t **)calloc(
		    1, realsize * sizeof(dict_entry_t *));

	n.used = 0;

	/* Is this the first initialization? If so it's not really a rehashing
	 * we just set the first hash table so that it can accept keys. */
	if (d->ht[0].table == NULL) {
		d->ht[0] = n;
		return DICT_OK;
	}

	/* Prepare a second hash table for incremental rehashing */
	d->ht[1] = n;
	d->rehash_index = 0;
	return DICT_OK;
}

/* return DICT_ERR if expand was not performed */
int dict_expand(dict_t *d, unsigned long size)
{
	return _dict_expand(d, size, NULL);
}

/* return DICT_ERR if expand failed due to memory allocation failure */
int dict_try_expand(dict_t *d, unsigned long size)
{
	int malloc_failed;
	_dict_expand(d, size, &malloc_failed);
	return malloc_failed ? DICT_ERR : DICT_OK;
}

/* Performs N steps of incremental rehashing. Returns 1 if there are still
 * keys to move from the old to the new hash table, otherwise 0 is returned.
 *
 * Note that a rehashing step consists in moving a bucket (that may have more
 * than one key as we use chaining) from the old to the new hash table, however
 * since part of the hash table may be composed of empty spaces, it is not
 * guaranteed that this function will rehash even a single bucket, since it
 * will visit at max N*10 empty buckets in total, otherwise the amount of
 * work it does would be unbound and the function may block for a long time. */
int dict_rehash(dict_t *d, int n)
{
	int empty_visits = n * 10; /* Max number of empty buckets to visit. */
	if (!dict_is_rehashing(d))
		return 0;

	while (n-- && d->ht[0].used != 0) {
		dict_entry_t *de, *nextde;

		/* Note that rehash_index can't overflow as we are sure there
		 * are more elements because ht[0].used != 0 */
		assert(d->ht[0].size > (unsigned long)d->rehash_index);
		while (d->ht[0].table[d->rehash_index] == NULL) {
			d->rehash_index++;
			if (--empty_visits == 0)
				return 1;
		}
		de = d->ht[0].table[d->rehash_index];
		/* Move all the keys in this bucket from the old to the new hash
		 * HT */
		while (de) {
			uint64_t h;

			nextde = de->next;
			/* Get the index in the new hash table */
			h = dict_hash_key(d, de->key) & d->ht[1].size_mask;
			de->next = d->ht[1].table[h];
			d->ht[1].table[h] = de;
			d->ht[0].used--;
			d->ht[1].used++;
			de = nextde;
		}
		d->ht[0].table[d->rehash_index] = NULL;
		d->rehash_index++;
	}

	/* Check if we already rehashed the whole table... */
	if (d->ht[0].used == 0) {
		free(d->ht[0].table);
		d->ht[0] = d->ht[1];
		_dict_reset(&d->ht[1]);
		d->rehash_index = -1;
		return 0;
	}

	/* More to rehash... */
	return 1;
}

/* Rehash in ms+"delta" milliseconds. The value of "delta" is larger
 * than 0, and is smaller than 1 in most cases. The exact upper bound
 * depends on the running time of dict_rehash(d,100).*/
int dict_rehash_milliseconds(dict_t *d, int ms)
{
	if (d->pause_rehash > 0)
		return 0;

	long long start = time_in_milliseconds();
	int rehashes = 0;

	while (dict_rehash(d, 100)) {
		rehashes += 100;
		if (time_in_milliseconds() - start > ms)
			break;
	}
	return rehashes;
}

/* This function performs just a step of rehashing, and only if hashing has
 * not been paused for our hash table. When we have iterators in the
 * middle of a rehashing we can't mess with the two hash tables otherwise
 * some element can be missed or duplicated.
 *
 * This function is called by common lookup or update operations in the
 * dictionary so that the hash table automatically migrates from H1 to H2
 * while it is actively used. */
static void _dict_rehash_step(dict_t *d)
{
	if (d->pause_rehash == 0)
		dict_rehash(d, 1);
}

/* Add an element to the target hash table */
int dict_add(dict_t *d, void *key, void *val)
{
	dict_entry_t *entry = dict_add_raw(d, key, NULL);

	if (!entry)
		return DICT_ERR;
	dict_set_val(d, entry, val);
	return DICT_OK;
}

/* Low level add or find:
 * This function adds the entry but instead of setting a value returns the
 * dict_entry_t structure to the user, that will make sure to fill the value
 * field as they wish.
 *
 * This function is also directly exposed to the user API to be called
 * mainly in order to store non-pointers inside the hash value, example:
 *
 * entry = dict_add_raw(dict_t,mykey,NULL);
 * if (entry != NULL) dict_set_signed_integer_val(entry,1000);
 *
 * Return values:
 *
 * If key already exists NULL is returned, and "*existing" is populated
 * with the existing entry if existing is not NULL.
 *
 * If key was added, the hash entry is returned to be manipulated by the caller.
 */
dict_entry_t *dict_add_raw(dict_t *d, void *key, dict_entry_t **existing)
{
	long index;
	dict_entry_t *entry;
	dict_hash_map_t *ht;

	if (dict_is_rehashing(d))
		_dict_rehash_step(d);

	/* Get the index of the new element, or -1 if
	 * the element already exists. */
	if ((index = _dict_key_index(d, key, dict_hash_key(d, key),
				     existing)) == -1)
		return NULL;

	/* Allocate the memory and store the new entry.
	 * Insert the element in top, with the assumption that in a database
	 * system it is more likely that recently added entries are accessed
	 * more frequently. */
	ht = dict_is_rehashing(d) ? &d->ht[1] : &d->ht[0];
	entry = (dict_entry_t *)malloc(sizeof(*entry));
	entry->next = ht->table[index];
	ht->table[index] = entry;
	ht->used++;

	/* Set the hash entry fields. */
	dict_set_key(d, entry, key);
	return entry;
}

/* Add or Overwrite:
 * Add an element, discarding the old value if the key already exists.
 * Return 1 if the key was added from scratch, 0 if there was already an
 * element with such key and dict_replace() just performed a value update
 * operation. */
int dict_replace(dict_t *d, void *key, void *val)
{
	dict_entry_t *entry, *existing, auxentry;

	/* Try to add the element. If the key
	 * does not exists dict_add will succeed. */
	entry = dict_add_raw(d, key, &existing);
	if (entry) {
		dict_set_val(d, entry, val);
		return 1;
	}

	/* Set the new value and free the old one. Note that it is important
	 * to do that in this order, as the value may just be exactly the same
	 * as the previous one. In this context, think to reference counting,
	 * you want to increment (set), and then decrement (free), and not the
	 * reverse. */
	auxentry = *existing;
	dict_set_val(d, existing, val);
	dict_free_val(d, &auxentry);
	return 0;
}

/* Add or Find:
 * dict_add_or_find() is simply a version of dict_add_raw() that always
 * returns the hash entry of the specified key, even if the key already
 * exists and can't be added (in that case the entry of the already
 * existing key is returned.)
 *
 * See dict_add_raw() for more information. */
dict_entry_t *dict_add_or_find(dict_t *d, void *key)
{
	dict_entry_t *entry, *existing;
	entry = dict_add_raw(d, key, &existing);
	return entry ? entry : existing;
}

/* Search and remove an element. This is a helper function for
 * dict_delete() and dict_unlink(), please check the top comment
 * of those functions. */
static dict_entry_t *dict_generic_delete(dict_t *d, const void *key, int nofree)
{
	uint64_t h, idx;
	dict_entry_t *he, *prevHe;
	int table;

	/* dict_t is empty */
	if (dict_size(d) == 0)
		return NULL;

	if (dict_is_rehashing(d))
		_dict_rehash_step(d);
	h = dict_hash_key(d, key);

	for (table = 0; table <= 1; table++) {
		idx = h & d->ht[table].size_mask;
		he = d->ht[table].table[idx];
		prevHe = NULL;
		while (he) {
			if (key == he->key ||
			    dict_compare_keys(d, key, he->key)) {
				/* Unlink the element from the list */
				if (prevHe)
					prevHe->next = he->next;
				else
					d->ht[table].table[idx] = he->next;
				if (!nofree) {
					dict_free_unlinked_entry(d, he);
				}
				d->ht[table].used--;
				return he;
			}
			prevHe = he;
			he = he->next;
		}
		if (!dict_is_rehashing(d))
			break;
	}
	return NULL; /* not found */
}

/* Remove an element, returning DICT_OK on success or DICT_ERR if the
 * element was not found. */
int dict_delete(dict_t *ht, const void *key)
{
	return dict_generic_delete(ht, key, 0) ? DICT_OK : DICT_ERR;
}

/* Remove an element from the table, but without actually releasing
 * the key, value and dictionary entry. The dictionary entry is returned
 * if the element was found (and unlinked from the table), and the user
 * should later call `dict_free_unlinked_entry()` with it in order to destroy
 * it. Otherwise if the key is not found, NULL is returned.
 *
 * This function is useful when we want to remove something from the hash
 * table but want to use its value before actually deleting the entry.
 * Without this function the pattern would require two lookups:
 *
 *  entry = dict_find(...);
 *  // Do something with entry
 *  dict_delete(dictionary,entry);
 *
 * Thanks to this function it is possible to avoid this, and use
 * instead:
 *
 * entry = dict_unlink(dictionary,entry);
 * // Do something with entry
 * dict_free_unlinked_entry(entry); // <- This does not need to lookup again.
 */
dict_entry_t *dict_unlink(dict_t *ht, const void *key)
{
	return dict_generic_delete(ht, key, 1);
}

/* You need to call this function to really free the entry after a call
 * to dict_unlink(). It's safe to call this function with 'he' = NULL. */
void dict_free_unlinked_entry(dict_t *d, dict_entry_t *he)
{
	if (he == NULL)
		return;
	dict_free_key(d, he);
	dict_free_val(d, he);
	free(he);
}

/* Destroy an entire dictionary */
int _dict_clear(dict_t *d, dict_hash_map_t *ht, void(callback)(void *))
{
	unsigned long i;

	/* Free all the elements */
	for (i = 0; i < ht->size && ht->used > 0; i++) {
		dict_entry_t *he, *nextHe;

		if (callback && (i & 65535) == 0)
			callback(d->priv_data);

		if ((he = ht->table[i]) == NULL)
			continue;
		while (he) {
			nextHe = he->next;
			dict_free_key(d, he);
			dict_free_val(d, he);
			free(he);
			ht->used--;
			he = nextHe;
		}
	}
	/* Free the table and the allocated cache structure */
	free(ht->table);
	/* Re-initialize the table */
	_dict_reset(ht);
	return DICT_OK; /* never fails */
}

/* Clear & Release the hash table */
void dict_destroy(dict_t *d)
{
	_dict_clear(d, &d->ht[0], NULL);
	_dict_clear(d, &d->ht[1], NULL);
	free(d);
}

dict_entry_t *dict_find(dict_t *d, const void *key)
{
	dict_entry_t *he;
	uint64_t h, idx, table;

	if (dict_size(d) == 0)
		return NULL; /* dict_t is empty */
	if (dict_is_rehashing(d))
		_dict_rehash_step(d);
	h = dict_hash_key(d, key);
	for (table = 0; table <= 1; table++) {
		idx = h & d->ht[table].size_mask;
		he = d->ht[table].table[idx];
		while (he) {
			if (key == he->key ||
			    dict_compare_keys(d, key, he->key))
				return he;
			he = he->next;
		}
		if (!dict_is_rehashing(d))
			return NULL;
	}
	return NULL;
}

void *dict_fetch_value(dict_t *d, const void *key)
{
	dict_entry_t *he;

	he = dict_find(d, key);
	return he ? dict_get_val(he) : NULL;
}

/* A fingerprint is a 64 bit number that represents the state of the dictionary
 * at a given time, it's just a few dict_t properties xored together.
 * When an unsafe iterator is initialized, we get the dict_t fingerprint, and
 * check the fingerprint again when the iterator is destroyd. If the two
 * fingerprints are different it means that the user of the iterator performed
 * forbidden operations against the dictionary while iterating. */
long long dict_fingerprint(dict_t *d)
{
	long long integers[6], hash = 0;
	int j;

	integers[0] = (long)(long long)d->ht[0].table;
	integers[1] = d->ht[0].size;
	integers[2] = d->ht[0].used;
	integers[3] = (long)(long long)d->ht[1].table;
	integers[4] = d->ht[1].size;
	integers[5] = d->ht[1].used;

	/* We hash N integers by summing every successive integer with the
	 * integer hashing of the previous sum. Basically:
	 *
	 * Result = hash(hash(hash(int1)+int2)+int3) ...
	 *
	 * This way the same set of integers in a different order will (likely)
	 * hash to a different number. */
	for (j = 0; j < 6; j++) {
		hash += integers[j];
		/* For the hashing step we use Tomas Wang's 64 bit integer hash.
		 */
		hash = (~hash) +
		       (hash << 21);    // hash = (hash << 21) - hash - 1;
		hash = hash ^ (hash >> 24);
		hash = (hash + (hash << 3)) + (hash << 8);    // hash * 265
		hash = hash ^ (hash >> 14);
		hash = (hash + (hash << 2)) + (hash << 4);    // hash * 21
		hash = hash ^ (hash >> 28);
		hash = hash + (hash << 31);
	}
	return hash;
}

dict_iterator_t *dict_get_iterator(dict_t *d)
{
	dict_iterator_t *iter = (dict_iterator_t *)malloc(sizeof(*iter));
	if (iter == NULL)
		return NULL;
	iter->d = d;
	iter->table = 0;
	iter->index = -1;
	iter->safe = 0;
	iter->entry = NULL;
	iter->next_entry = NULL;
	return iter;
}

dict_iterator_t *dict_get_safe_iterator(dict_t *d)
{
	dict_iterator_t *i = dict_get_iterator(d);

	i->safe = 1;
	return i;
}

dict_entry_t *dict_next(dict_iterator_t *iter)
{
	while (1) {
		if (iter->entry == NULL) {
			dict_hash_map_t *ht = &iter->d->ht[iter->table];
			if (iter->index == -1 && iter->table == 0) {
				if (iter->safe)
					dict_pause_rehashing(iter->d);
				else
					iter->fingerprint =
					    dict_fingerprint(iter->d);
			}
			iter->index++;
			if (iter->index >= (long)ht->size) {
				if (dict_is_rehashing(iter->d) &&
				    iter->table == 0) {
					iter->table++;
					iter->index = 0;
					ht = &iter->d->ht[1];
				} else {
					break;
				}
			}
			iter->entry = ht->table[iter->index];
		} else {
			iter->entry = iter->next_entry;
		}
		if (iter->entry) {
			/* We need to save the 'next' here, the iterator user
			 * may delete the entry we are returning. */
			iter->next_entry = iter->entry->next;
			return iter->entry;
		}
	}
	return NULL;
}

void dict_destroy_iterator(dict_iterator_t *iter)
{
	if (!(iter->index == -1 && iter->table == 0)) {
		if (iter->safe)
			dict_resume_rehashing(iter->d);
		else
			assert(iter->fingerprint == dict_fingerprint(iter->d));
	}
	free(iter);
}

/* Return a random entry from the hash table. Useful to
 * implement randomized algorithms */
dict_entry_t *dict_get_random_key(dict_t *d)
{
	dict_entry_t *he, *orighe;
	unsigned long h;
	int listlen, listele;

	if (dict_size(d) == 0)
		return NULL;
	if (dict_is_rehashing(d))
		_dict_rehash_step(d);
	if (dict_is_rehashing(d)) {
		do {
			/* We are sure there are no elements in indexes from 0
			 * to rehash_index-1 */
			h = d->rehash_index +
			    (random_u_long() %
			     (dict_slots(d) - d->rehash_index));
			he = (h >= d->ht[0].size)
				 ? d->ht[1].table[h - d->ht[0].size]
				 : d->ht[0].table[h];
		} while (he == NULL);
	} else {
		do {
			h = random_u_long() & d->ht[0].size_mask;
			he = d->ht[0].table[h];
		} while (he == NULL);
	}

	/* Now we found a non empty bucket, but it is a linked
	 * list and we need to get a random element from the list.
	 * The only sane way to do so is counting the elements and
	 * select a random index. */
	listlen = 0;
	orighe = he;
	while (he) {
		he = he->next;
		listlen++;
	}
	listele = rand() % listlen;
	he = orighe;
	while (listele--) he = he->next;
	return he;
}

/* This function samples the dictionary to return a few keys from random
 * locations.
 *
 * It does not guarantee to return all the keys specified in 'count', nor
 * it does guarantee to return non-duplicated elements, however it will make
 * some effort to do both things.
 *
 * Returned pointers to hash table entries are stored into 'des' that
 * points to an array of dict_entry_t pointers. The array must have room for
 * at least 'count' elements, that is the argument we pass to the function
 * to tell how many random elements we need.
 *
 * The function returns the number of entrys stored into 'des', that may
 * be less than 'count' if the hash table has less than 'count' elements
 * inside, or if not enough elements were found in a reasonable amount of
 * steps.
 *
 * Note that this function is not suitable when you need a good distribution
 * of the returned entrys, but only when you need to "sample" a given number
 * of continuous elements to run some kind of algorithm or to produce
 * statistics. However the function is much faster than dict_get_random_key()
 * at producing N elements. */
unsigned int dict_get_some_keys(dict_t *d, dict_entry_t **des,
				unsigned int count)
{
	unsigned long j;      /* internal hash table id, 0 or 1. */
	unsigned long tables; /* 1 or 2 tables? */
	unsigned long stored = 0, maxsizemask;
	unsigned long maxsteps;

	if (dict_size(d) < count)
		count = dict_size(d);
	maxsteps = count * 10;

	/* Try to do a rehashing work proportional to 'count'. */
	for (j = 0; j < count; j++) {
		if (dict_is_rehashing(d))
			_dict_rehash_step(d);
		else
			break;
	}

	tables = dict_is_rehashing(d) ? 2 : 1;
	maxsizemask = d->ht[0].size_mask;
	if (tables > 1 && maxsizemask < d->ht[1].size_mask)
		maxsizemask = d->ht[1].size_mask;

	/* Pick a random point inside the larger table. */
	unsigned long i = random_u_long() & maxsizemask;
	unsigned long emptylen = 0; /* Continuous empty entries so far. */
	while (stored < count && maxsteps--) {
		for (j = 0; j < tables; j++) {
			/* Invariant of the dict_t.c rehashing: up to the
			 * indexes already visited in ht[0] during the
			 * rehashing, there are no populated
			 * buckets, so we can skip ht[0] for indexes between 0
			 * and idx-1. */
			if (tables == 2 && j == 0 &&
			    i < (unsigned long)d->rehash_index) {
				/* Moreover, if we are currently out of range in
				 * the second table, there will be no elements
				 * in both tables up to the current rehashing
				 * index, so we jump if possible.
				 * (this happens when going from big to small
				 * table). */
				if (i >= d->ht[1].size)
					i = d->rehash_index;
				else
					continue;
			}
			if (i >= d->ht[j].size)
				continue; /* Out of range for this table. */
			dict_entry_t *he = d->ht[j].table[i];

			/* Count contiguous empty buckets, and jump to other
			 * locations if they reach 'count' (with a minimum of
			 * 5). */
			if (he == NULL) {
				emptylen++;
				if (emptylen >= 5 && emptylen > count) {
					i = random_u_long() & maxsizemask;
					emptylen = 0;
				}
			} else {
				emptylen = 0;
				while (he) {
					/* Collect all the elements of the
					 * buckets found non empty while
					 * iterating. */
					*des = he;
					des++;
					he = he->next;
					stored++;
					if (stored == count)
						return stored;
				}
			}
		}
		i = (i + 1) & maxsizemask;
	}
	return stored;
}

/* This is like dict_get_random_key() from the POV of the API, but will do more
 * work to ensure a better distribution of the returned element.
 *
 * This function improves the distribution because the dict_get_random_key()
 * problem is that it selects a random bucket, then it selects a random
 * element from the chain in the bucket. However elements being in different
 * chain lengths will have different probabilities of being reported. With
 * this function instead what we do is to consider a "linear" range of the table
 * that may be constituted of N buckets with chains of different lengths
 * appearing one after the other. Then we report a random element in the range.
 * In this way we smooth away the problem of different chain lengths. */
#define GETFAIR_NUM_ENTRIES 15
dict_entry_t *dict_get_fair_random_key(dict_t *d)
{
	dict_entry_t *entries[GETFAIR_NUM_ENTRIES];
	unsigned int count =
	    dict_get_some_keys(d, entries, GETFAIR_NUM_ENTRIES);
	/* Note that dict_get_some_keys() may return zero elements in an unlucky
	 * run() even if there are actually elements inside the hash table. So
	 * when we get zero, we call the true dict_get_random_key() that will
	 * always yield the element if the hash table has at least one. */
	if (count == 0)
		return dict_get_random_key(d);
	unsigned int idx = rand() % count;
	return entries[idx];
}

/* Function to reverse bits. Algorithm from:
 * http://graphics.stanford.edu/~seander/bithacks.html#ReverseParallel */
static unsigned long rev(unsigned long v)
{
	unsigned long s =
	    CHAR_BIT * sizeof(v);    // bit size; must be power of 2
	unsigned long mask = ~0UL;
	while ((s >>= 1) > 0) {
		mask ^= (mask << s);
		v = ((v >> s) & mask) | ((v << s) & ~mask);
	}
	return v;
}

/* dict_scan() is used to iterate over the elements of a dictionary.
 *
 * Iterating works the following way:
 *
 * 1) Initially you call the function using a cursor (v) value of 0.
 * 2) The function performs one step of the iteration, and returns the
 *    new cursor value you must use in the next call.
 * 3) When the returned cursor is 0, the iteration is complete.
 *
 * The function guarantees all elements present in the
 * dictionary get returned between the start and end of the iteration.
 * However it is possible some elements get returned multiple times.
 *
 * For every element returned, the callback argument 'fn' is
 * called with 'priv_data' as first argument and the dictionary entry
 * 'de' as second argument.
 *
 * HOW IT WORKS.
 *
 * The iteration algorithm was designed by Pieter Noordhuis.
 * The main idea is to increment a cursor starting from the higher order
 * bits. That is, instead of incrementing the cursor normally, the bits
 * of the cursor are reversed, then the cursor is incremented, and finally
 * the bits are reversed again.
 *
 * This strategy is needed because the hash table may be resized between
 * iteration calls.
 *
 * dict.c hash tables are always power of two in size, and they
 * use chaining, so the position of an element in a given table is given
 * by computing the bitwise AND between _hash(key) and SIZE-1
 * (where SIZE-1 is always the mask that is equivalent to taking the rest
 *  of the division between the _hash of the key and SIZE).
 *
 * For example if the current hash table size is 16, the mask is
 * (in binary) 1111. The position of a key in the hash table will always be
 * the last four bits of the hash output, and so forth.
 *
 * WHAT HAPPENS IF THE TABLE CHANGES IN SIZE?
 *
 * If the hash table grows, elements can go anywhere in one multiple of
 * the old bucket: for example let's say we already iterated with
 * a 4 bit cursor 1100 (the mask is 1111 because hash table size = 16).
 *
 * If the hash table will be resized to 64 elements, then the new mask will
 * be 111111. The new buckets you obtain by substituting in ??1100
 * with either 0 or 1 can be targeted only by keys we already visited
 * when scanning the bucket 1100 in the smaller hash table.
 *
 * By iterating the higher bits first, because of the inverted counter, the
 * cursor does not need to restart if the table size gets bigger. It will
 * continue iterating using cursors without '1100' at the end, and also
 * without any other combination of the final 4 bits already explored.
 *
 * Similarly when the table size shrinks over time, for example going from
 * 16 to 8, if a combination of the lower three bits (the mask for size 8
 * is 111) were already completely explored, it would not be visited again
 * because we are sure we tried, for example, both 0111 and 1111 (all the
 * variations of the higher bit) so we don't need to test it again.
 *
 * WAIT... YOU HAVE *TWO* TABLES DURING REHASHING!
 *
 * Yes, this is true, but we always iterate the smaller table first, then
 * we test all the expansions of the current cursor into the larger
 * table. For example if the current cursor is 101 and we also have a
 * larger table of size 16, we also test (0)101 and (1)101 inside the larger
 * table. This reduces the problem back to having only one table, where
 * the larger one, if it exists, is just an expansion of the smaller one.
 *
 * LIMITATIONS
 *
 * This iterator is completely stateless, and this is a huge advantage,
 * including no additional memory used.
 *
 * The disadvantages resulting from this design are:
 *
 * 1) It is possible we return elements more than once. However this is usually
 *    easy to deal with in the application level.
 * 2) The iterator must return multiple elements per call, as it needs to always
 *    return all the keys chained in a given bucket, and all the expansions, so
 *    we are sure we don't miss keys moving during rehashing.
 * 3) The reverse cursor is somewhat hard to understand at first, but this
 *    comment is supposed to help.
 */
unsigned long dict_scan(dict_t *d, unsigned long v, dict_scan_function *fn,
			dict_scan_bucket_function *bucketfn, void *priv_data)
{
	dict_hash_map_t *t0, *t1;
	const dict_entry_t *de, *next;
	unsigned long m0, m1;

	if (dict_size(d) == 0)
		return 0;

	/* This is needed in case the scan callback tries to do dict_find or
	 * alike. */
	dict_pause_rehashing(d);

	if (!dict_is_rehashing(d)) {
		t0 = &(d->ht[0]);
		m0 = t0->size_mask;

		/* Emit entries at cursor */
		if (bucketfn)
			bucketfn(priv_data, &t0->table[v & m0]);
		de = t0->table[v & m0];
		while (de) {
			next = de->next;
			fn(priv_data, de);
			de = next;
		}

		/* Set unmasked bits so incrementing the reversed cursor
		 * operates on the masked bits */
		v |= ~m0;

		/* Increment the reverse cursor */
		v = rev(v);
		v++;
		v = rev(v);
	} else {
		t0 = &d->ht[0];
		t1 = &d->ht[1];

		/* Make sure t0 is the smaller and t1 is the bigger table */
		if (t0->size > t1->size) {
			t0 = &d->ht[1];
			t1 = &d->ht[0];
		}

		m0 = t0->size_mask;
		m1 = t1->size_mask;

		/* Emit entries at cursor */
		if (bucketfn)
			bucketfn(priv_data, &t0->table[v & m0]);
		de = t0->table[v & m0];
		while (de) {
			next = de->next;
			fn(priv_data, de);
			de = next;
		}

		/* Iterate over indices in larger table that are the expansion
		 * of the index pointed to by the cursor in the smaller table */
		do {
			/* Emit entries at cursor */
			if (bucketfn)
				bucketfn(priv_data, &t1->table[v & m1]);
			de = t1->table[v & m1];
			while (de) {
				next = de->next;
				fn(priv_data, de);
				de = next;
			}

			/* Increment the reverse cursor not covered by the
			 * smaller mask.*/
			v |= ~m1;
			v = rev(v);
			v++;
			v = rev(v);

			/* Continue while bits covered by mask difference is
			 * non-zero */
		} while (v & (m0 ^ m1));
	}

	dict_resume_rehashing(d);

	return v;
}

/* ------------------------- private functions ------------------------------ */

/* Because we may need to allocate huge memory chunk at once when dict_t
 * expands, we will check this allocation is allowed or not if the dict_t
 * type has _expand_allowed member function. */
static int dict_type_expand_allowed(dict_t *d)
{
	if (d->type->expand_allowed == NULL)
		return 1;
	return d->type->expand_allowed(
	    _dict_next_power(d->ht[0].used + 1) * sizeof(dict_entry_t *),
	    (double)d->ht[0].used / d->ht[0].size);
}

/* Expand the hash table if needed */
static int _dict_expand_if_needed(dict_t *d)
{
	/* Incremental rehashing already in progress. Return. */
	if (dict_is_rehashing(d))
		return DICT_OK;

	/* If the hash table is empty expand it to the initial size. */
	if (d->ht[0].size == 0)
		return dict_expand(d, DICT_HT_INITIAL_SIZE);

	/* If we reached the 1:1 ratio, and we are allowed to resize the hash
	 * table (global setting) or we should avoid it but the ratio between
	 * elements/buckets is over the "safe" threshold, we resize doubling
	 * the number of buckets. */
	if (d->ht[0].used >= d->ht[0].size &&
	    (dict_can_resize ||
	     d->ht[0].used / d->ht[0].size > dict_force_resize_ratio) &&
	    dict_type_expand_allowed(d)) {
		return dict_expand(d, d->ht[0].used + 1);
	}
	return DICT_OK;
}

/* Our hash table capability is a power of two */
static unsigned long _dict_next_power(unsigned long size)
{
	unsigned long i = DICT_HT_INITIAL_SIZE;

	if (size >= LONG_MAX)
		return LONG_MAX + 1LU;
	while (1) {
		if (i >= size)
			return i;
		i *= 2;
	}
}

/* Returns the index of a free slot that can be populated with
 * a hash entry for the given 'key'.
 * If the key already exists, -1 is returned
 * and the optional output parameter may be filled.
 *
 * Note that if we are in the process of rehashing the hash table, the
 * index is always returned in the context of the second (new) hash table. */
static long _dict_key_index(dict_t *d, const void *key, uint64_t hash,
			    dict_entry_t **existing)
{
	unsigned long idx, table;
	dict_entry_t *he;
	if (existing)
		*existing = NULL;

	/* Expand the hash table if needed */
	if (_dict_expand_if_needed(d) == DICT_ERR)
		return -1;
	for (table = 0; table <= 1; table++) {
		idx = hash & d->ht[table].size_mask;
		/* Search if this slot does not already contain the given key */
		he = d->ht[table].table[idx];
		while (he) {
			if (key == he->key ||
			    dict_compare_keys(d, key, he->key)) {
				if (existing)
					*existing = he;
				return -1;
			}
			he = he->next;
		}
		if (!dict_is_rehashing(d))
			break;
	}
	return idx;
}

void dict_empty(dict_t *d, void(callback)(void *))
{
	_dict_clear(d, &d->ht[0], callback);
	_dict_clear(d, &d->ht[1], callback);
	d->rehash_index = -1;
	d->pause_rehash = 0;
}

void dict_enable_resize(void)
{
	dict_can_resize = 1;
}

void dict_disable_resize(void)
{
	dict_can_resize = 0;
}

uint64_t dict_get_hash(dict_t *d, const void *key)
{
	return dict_hash_key(d, key);
}

/* Finds the dict_entry_t reference by using pointer and pre-calculated hash.
 * oldkey is a dead pointer and should not be accessed.
 * the hash value should be provided using dict_get_hash.
 * no string / key comparison is performed.
 * return value is the reference to the dict_entry_t if found, or NULL if not
 * found. */
dict_entry_t **dict_find_entry_ref_by_ptr_and_hash(dict_t *d,
						   const void *old_ptr,
						   uint64_t hash)
{
	dict_entry_t *he, **heref;
	unsigned long idx, table;

	if (dict_size(d) == 0)
		return NULL; /* dict_t is empty */
	for (table = 0; table <= 1; table++) {
		idx = hash & d->ht[table].size_mask;
		heref = &d->ht[table].table[idx];
		he = *heref;
		while (he) {
			if (old_ptr == he->key)
				return heref;
			heref = &he->next;
			he = *heref;
		}
		if (!dict_is_rehashing(d))
			return NULL;
	}
	return NULL;
}

/* ------------------------------- Debugging ---------------------------------*/

#define DICT_STATS_VECTLEN 50
size_t _dict_get_stats_ht(char *buf, size_t buf_size, dict_hash_map_t *ht,
			  int tableid)
{
	unsigned long i, slots = 0, chainlen, maxchainlen = 0;
	unsigned long totchainlen = 0;
	unsigned long clvector[DICT_STATS_VECTLEN];
	size_t l = 0;

	if (ht->used == 0) {
		return snprintf(buf, buf_size,
				"No stats available for empty dictionaries\n");
	}

	/* Compute stats. */
	for (i = 0; i < DICT_STATS_VECTLEN; i++) clvector[i] = 0;
	for (i = 0; i < ht->size; i++) {
		dict_entry_t *he;

		if (ht->table[i] == NULL) {
			clvector[0]++;
			continue;
		}
		slots++;
		/* For each hash entry on this slot... */
		chainlen = 0;
		he = ht->table[i];
		while (he) {
			chainlen++;
			he = he->next;
		}
		clvector[(chainlen < DICT_STATS_VECTLEN)
			     ? chainlen
			     : (DICT_STATS_VECTLEN - 1)]++;
		if (chainlen > maxchainlen)
			maxchainlen = chainlen;
		totchainlen += chainlen;
	}

	/* Generate human readable stats. */
	l += snprintf(buf + l, buf_size - l,
		      "_hash table %d stats (%s):\n"
		      " table size: %lu\n"
		      " number of elements: %lu\n"
		      " different slots: %lu\n"
		      " max chain length: %lu\n"
		      " avg chain length (counted): %.02f\n"
		      " avg chain length (computed): %.02f\n"
		      " Chain length distribution:\n",
		      tableid,
		      (tableid == 0) ? "main hash table" : "rehashing target",
		      ht->size, ht->used, slots, maxchainlen,
		      (float)totchainlen / slots, (float)ht->used / slots);

	for (i = 0; i < DICT_STATS_VECTLEN - 1; i++) {
		if (clvector[i] == 0)
			continue;
		if (l >= buf_size)
			break;
		l += snprintf(
		    buf + l, buf_size - l, "   %s%ld: %ld (%.02f%%)\n",
		    (i == DICT_STATS_VECTLEN - 1) ? ">= " : "", i, clvector[i],
		    ((float)clvector[i] / ht->size) * 100);
	}

	/* Unlike snprintf(), return the number of characters actually written.
	 */
	if (buf_size)
		buf[buf_size - 1] = '\0';
	return strlen(buf);
}

void dict_get_stats(char *buf, size_t buf_size, dict_t *d)
{
	size_t l;
	char *orig_buf = buf;
	size_t orig_bufsize = buf_size;

	l = _dict_get_stats_ht(buf, buf_size, &d->ht[0], 0);
	buf += l;
	buf_size -= l;
	if (dict_is_rehashing(d) && buf_size > 0) {
		_dict_get_stats_ht(buf, buf_size, &d->ht[1], 1);
	}
	/* Make sure there is a NULL term at the end. */
	if (orig_bufsize)
		orig_buf[orig_bufsize - 1] = '\0';
}

uint64_t dict_string_hash(const void *key)
{
	return dict_gen_hash_function(key, (int)strlen((const char *)key));
}

int dict_string_key_compare(void *privdata, const void *key1, const void *key2)
{
	dict_not_used(privdata);
	return strcmp((const char *)key1, (const char *)key2) == 0;
}

void *dict_string_key_dup(void *privdata, const void *key)
{
	dict_not_used(privdata);
	char *newkey =
	    (char *)malloc((strlen((const char *)key) + 1) * sizeof(char));
	if (newkey == NULL)
		return NULL;
	strcpy((char *)newkey, (const char *)key);
	return newkey;
}

void dict_string_key_destructor(void *privdata, void *key)
{
	dict_not_used(privdata);
	free(key);
}

void dict_init_string_key_type(dict_type_t *t)
{
	t->hash_function = dict_string_hash;
	t->key_dup = NULL;
	t->val_dup = NULL;
	t->key_compare = dict_string_key_compare;
	t->key_destructor = NULL;
	t->val_destructor = NULL;
	t->expand_allowed = NULL;
}

void dict_init_string_copy_key_type(dict_type_t *t)
{
	t->hash_function = dict_string_hash;
	t->key_dup = dict_string_key_dup;
	t->val_dup = NULL;
	t->key_compare = dict_string_key_compare;
	t->key_destructor = dict_string_key_destructor;
	t->val_destructor = NULL;
	t->expand_allowed = 0;
	t->expand_allowed = NULL;
}
