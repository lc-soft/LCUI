/* Hash Tables Implementation.
 *
 * This file implements in memory hash tables with insert/del/replace/find/
 * get-random-element operations. Hash tables will auto resize if needed
 * tables of power of two in size are used, collisions are handled by
 * chaining. See the source code for more information... :)
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <ctype.h>
#include <time.h>

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>

/* Using Dict_EnableResize() / Dict_DisableResize() we make possible to
* enable/disable resizing of the hash table as needed. This is very important
* for Redis, as we use copy-on-write and don't want to move too much memory
* around when there is a child performing saving operations.
*
* Note that even when dict_can_resize is set to 0, not all resizes are
* prevented: an hash table is still allowed to grow if the ratio between
* the number of elements and the buckets > dict_force_resize_ratio. */
static int dict_can_resize = 1;
static unsigned int dict_force_resize_ratio = 5;

/* -------------------------- private prototypes ---------------------------- */

static int Dict_ExpandIfNeeded( Dict *ht );
static unsigned long Dict_NextPower( unsigned long size );
static int Dict_KeyIndex( Dict *ht, const void *key );
static int Dict_Init( Dict *ht, DictType *type, void *privdata );

/* -------------------------- hash functions -------------------------------- */

unsigned int Dict_IntHashFunction( unsigned int key )
{
	key += ~(key << 15);
	key ^= (key >> 10);
	key += (key << 3);
	key ^= (key >> 6);
	key += ~(key << 11);
	key ^= (key >> 16);
	return key;
}

/* Identity hash function for integer keys */
unsigned int Dict_IdentityHashFunction( unsigned int key )
{
	return key;
}

static int dict_hash_function_seed = 5381;

void Dict_SetHashFunctionSeed( unsigned int seed )
{
	dict_hash_function_seed = seed;
}

unsigned int Dict_GetHashFunctionSeed( void )
{
	return dict_hash_function_seed;
}

/* Generic hash function (a popular one from Bernstein).
* I tested a few and this was the best. */
unsigned int Dict_GenHashFunction( const unsigned char *buf, int len )
{
	unsigned int hash = dict_hash_function_seed;
	while( len-- ) {
		hash = ((hash << 5) + hash) + (*buf++); /* hash * 33 + c */
	}
	return hash;
}

/* And a case insensitive version */
unsigned int Dict_GenCaseHashFunction( const unsigned char *buf, int len )
{
	unsigned int hash = dict_hash_function_seed;
	while( len-- ) {
		hash = ((hash << 5) + hash) + (tolower( *buf++ )); /* hash * 33 + c */
	}
	return hash;
}

/* ----------------------------- API implementation ------------------------- */

/** 重置哈希表 */
static void Dict_Reset( DictHashTable *ht )
{
	ht->table = NULL;
	ht->size = 0;
	ht->sizemask = 0;
	ht->used = 0;
}

Dict *Dict_Create( DictType *type, void *privdata )
{
	static int inited = 0;
	Dict *d = malloc( sizeof( Dict ) );
	Dict_Init( d, type, privdata );
	if( !inited ) {
		srand( (unsigned int)time( NULL ) );
		inited = 1;
	}
	return d;
}

/** 初始化字典 */
static int Dict_Init( Dict *d, DictType *type, void *privdata )
{
	d->type = type;
	d->iterators = 0;
	d->rehashidx = -1;
	d->privdata = privdata;
	Dict_Reset( &d->ht[0] );
	Dict_Reset( &d->ht[1] );
	return 0;
}

int Dict_Resize( Dict *d )
{
	int minimal;
	if( !dict_can_resize || Dict_IsRehashing( d ) ) {
		return -1;
	}
	minimal = d->ht[0].used;
	if( minimal < DICT_HT_INITIAL_SIZE ) {
		minimal = DICT_HT_INITIAL_SIZE;
	}
	return Dict_Expand( d, minimal );
}

int Dict_Expand( Dict *d, unsigned long size )
{
	DictHashTable n;
	unsigned long realsize;
	if( Dict_IsRehashing( d ) || d->ht[0].used > size ) {
		return -1;
	}
	/* 计算哈希表的(真正)大小 */
	realsize = Dict_NextPower( size );
	n.used = 0;
	n.size = realsize;
	n.sizemask = realsize - 1;
	n.table = calloc( realsize, sizeof( DictEntry* ) );
	/* 如果字典的 0 号哈希表未初始化，则将新建的哈希表作为字典的 0 号哈
	 * 希表，否则，将新建哈希表作为字典的 1 号哈希表，并将它用于 rehash 
	 */
	if( d->ht[0].table == NULL ) {
		d->ht[0] = n;
	} else {
		d->ht[1] = n;
		d->rehashidx = 0;
	}
	return 0;
}

int Dict_Rehash( Dict *d, int n )
{
	if( !Dict_IsRehashing( d ) ) {
		return 0;
	}
	while( n-- ) {
		DictEntry *de, *nextde;
		if( d->ht[0].used == 0 ) {
			free( d->ht[0].table );
			d->ht[0] = d->ht[1];
			Dict_Reset( &d->ht[1] );
			d->rehashidx = -1;
			return 0;
		}
		assert( d->ht[0].size > (unsigned)d->rehashidx );
		while( d->ht[0].table[d->rehashidx] == NULL ) {
			d->rehashidx++;
		}
		de = d->ht[0].table[d->rehashidx];
		while( de ) {
			unsigned int h;
			nextde = de->next;
			h = Dict_HashKey( d, de->key ) & d->ht[1].sizemask;
			de->next = d->ht[1].table[h];
			d->ht[1].table[h] = de;
			d->ht[0].used--;
			d->ht[1].used++;
			de = nextde;
		}
		d->ht[0].table[d->rehashidx] = NULL;
		d->rehashidx++;
	}
	return 1;
}

#define timeInMilliseconds LCUI_GetTime

int Dict_RehashMilliseconds( Dict *d, int ms )
{
	long long start = timeInMilliseconds();
	int rehashes = 0;

	while( Dict_Rehash( d, 100 ) ) {
		rehashes += 100;
		if( timeInMilliseconds() - start > ms ) {
			break;
		}
	}
	return rehashes;
}

/** 在字典没有迭代器的情况下，rehash 一个元素 */
static void Dict_RehashStep( Dict *d )
{
	if( d->iterators == 0 ) {
		Dict_Rehash( d, 1 );
	}
}

int Dict_Add( Dict *d, void *key, void *val )
{
	DictEntry *entry = Dict_AddRaw( d, key );
	if( !entry ) {
		return -1;
	}
	Dict_SetVal( d, entry, val );
	return 0;
}

int Dict_AddCopy( Dict *d, void *key, const void *val )
{
	DictEntry *entry;
	if( !d->type->valDup ) {
		return -2;
	}
	entry = Dict_AddRaw( d, key );
	if( !entry ) {
		return -1;
	}
	entry->v.val = d->type->valDup( d->privdata, val );
	return 0;
}

DictEntry *Dict_AddRaw( Dict *d, void *key )
{
	int index;
	DictEntry *entry;
	DictHashTable *ht;

	/* 如果可以执行 rehash 操作，则执行平摊 rehash 操作 */
	if( Dict_IsRehashing( d ) ) {
		Dict_RehashStep( d );
	}
	/* 计算 key 的 index 值，如果 key 已经存在，Dict_KeyIndex 返回 -1 */
	if( (index = Dict_KeyIndex( d, key )) == -1 ) {
		return NULL;
	}
	/* 判断是否正在进行 rehash ，选择相应的表 */
	ht = Dict_IsRehashing( d ) ? &d->ht[1] : &d->ht[0];
	entry = malloc( sizeof( *entry ) );
	entry->next = ht->table[index];
	ht->table[index] = entry;
	ht->used++;
	Dict_SetKey( d, entry, key );
	return entry;
}

int Dict_Replace( Dict *d, void *key, void *val )
{
	DictEntry *entry, auxentry;
	if( Dict_Add( d, key, val ) == 0 ) {
		return 1;
	}
	entry = Dict_Find( d, key );
	auxentry = *entry;
	Dict_SetVal( d, entry, val );
	Dict_FreeVal( d, &auxentry );
	return 0;
}

DictEntry *Dict_ReplaceRaw( Dict *d, void *key )
{
	DictEntry *entry = Dict_Find( d, key );
	return entry ? entry : Dict_AddRaw( d, key );
}

/* 删除字典中的指定元素 */
static int Dict_GenericDelete( Dict *d, const void *key, int nofree )
{
	int table;
	unsigned int h, idx;
	DictEntry *he, *prev_he;
	if( d->ht[0].size == 0 ) {
		return -1;
	}
	if( Dict_IsRehashing( d ) ) {
		Dict_RehashStep( d );
	}
	h = Dict_HashKey( d, key );
	for( table = 0; table <= 1; table++ ) {
		idx = h & d->ht[table].sizemask;
		he = d->ht[table].table[idx];
		prev_he = NULL;
		while( he ) {
			if( Dict_CompareKeys( d, key, he->key ) ) {
				if( prev_he ) {
					prev_he->next = he->next;
				} else {
					d->ht[table].table[idx] = he->next;
				}
				if( !nofree ) {
					Dict_FreeKey( d, he );
					Dict_FreeVal( d, he );
				}
				free( he );
				d->ht[table].used--;
				return 0;
			}
			prev_he = he;
			he = he->next;
		}
		if( !Dict_IsRehashing( d ) ) {
			break;
		}
	}
	return -1;
}

int Dict_Delete( Dict *ht, const void *key )
{
	return Dict_GenericDelete( ht, key, 0 );
}

int Dict_DeleteNoFree( Dict *ht, const void *key )
{
	return Dict_GenericDelete( ht, key, 1 );
}

/** 清除字典中指定的哈希表 */
static void Dict_Clear( Dict *d, DictHashTable *ht )
{
	unsigned long i;
	DictEntry *he, *next_he;
	for( i = 0; i < ht->size && ht->used > 0; i++ ) {
		he = ht->table[i];
		while( he ) {
			next_he = he->next;
			Dict_FreeKey( d, he );
			Dict_FreeVal( d, he );
			free( he );
			ht->used--;
			he = next_he;
		}
	}
	free( ht->table );
	Dict_Reset( ht );
}

void Dict_Release( Dict *d )
{
	Dict_Clear( d, &d->ht[0] );
	Dict_Clear( d, &d->ht[1] );
	free( d );
}

DictEntry *Dict_Find( Dict *d, const void *key )
{
	DictEntry *he;
	unsigned int h, idx, table;
	if( d->ht[0].size == 0 ) {
		return NULL;
	}
	if( Dict_IsRehashing( d ) ) {
		Dict_RehashStep( d );
	}
	h = Dict_HashKey( d, key );
	for( table = 0; table <= 1; table++ ) {
		idx = h & d->ht[table].sizemask;
		he = d->ht[table].table[idx];
		while( he ) {
			if( Dict_CompareKeys( d, key, he->key ) ) {
				return he;
			}
			he = he->next;
		}
		if( !Dict_IsRehashing( d ) ) {
			return NULL;
		}
	}
	return NULL;
}

void *Dict_FetchValue( Dict *d, const void *key )
{
	DictEntry *he = Dict_Find( d, key );
	return he ? DictEntry_GetVal( he ) : NULL;
}

DictIterator *Dict_GetIterator( Dict *d )
{
	DictIterator *iter = malloc( sizeof( *iter ) );
	iter->d = d;
	iter->table = 0;
	iter->index = -1;
	iter->safe = 0;
	iter->entry = NULL;
	iter->next_entry = NULL;
	return iter;
}

DictIterator *Dict_GetSafeIterator( Dict *d )
{
	DictIterator *i = Dict_GetIterator( d );
	i->safe = 1;
	return i;
}

DictEntry *Dict_Next( DictIterator *iter )
{
	DictHashTable *ht;
	while( 1 ) {
		if( iter->entry ) {
			iter->entry = iter->next_entry;
			if( iter->entry ) {
				iter->next_entry = iter->entry->next;
				return iter->entry;
			}
			continue;
		}
		/* 如果迭代器是新的(未使用过)，那么初始化新迭代器 */
		ht = &iter->d->ht[iter->table];
		if( iter->safe && iter->index == -1 && iter->table == 0 ) {
			iter->d->iterators++;
		}
		iter->index++;
		if( iter->index >= (signed)ht->size ) {
			if( Dict_IsRehashing( iter->d ) && iter->table == 0 ) {
				iter->table++;
				iter->index = 0;
				ht = &iter->d->ht[1];
			} else {
				break;
			}
		}
		iter->entry = ht->table[iter->index];
		if( iter->entry ) {
			iter->next_entry = iter->entry->next;
			return iter->entry;
		}
	}
	return NULL;
}

void Dict_ReleaseIterator( DictIterator *iter )
{
	if( iter->safe && !(iter->index == -1 && iter->table == 0) ) {
		iter->d->iterators--;
	}
	free( iter );
}

DictEntry *Dict_GetRandomKey( Dict *d )
{
	DictEntry *he, *orighe;
	unsigned int h;
	int listlen, listele;

	if( Dict_Size( d ) == 0 ) {
		return NULL;
	}
	if( Dict_IsRehashing( d ) ) {
		Dict_RehashStep( d );
	}
	if( Dict_IsRehashing( d ) ) {
		do {
			h = rand() % (d->ht[0].size + d->ht[1].size);
			if( h >= d->ht[0].size ) {
				he = d->ht[1].table[h - d->ht[0].size];
			} else {
				he = d->ht[0].table[h];
			}
		} while( he == NULL );
	} else {
		do {
			h = rand() & d->ht[0].sizemask;
			he = d->ht[0].table[h];
		} while( he == NULL );
	}
	listlen = 0;
	orighe = he;
	while( he ) {
		he = he->next;
		listlen++;
	}
	/* 从链表里随机取一项 */
	listele = rand() % listlen;
	he = orighe;
	while( listele-- ) he = he->next;
	return he;
}

/* ------------------------- private functions ------------------------------ */

/* Expand the hash table if needed */
static int Dict_ExpandIfNeeded( Dict *d )
{
	/* Incremental rehashing already in progress. Return. */
	if( Dict_IsRehashing( d ) ) {
		return 0;
	}
	/* If the hash table is empty expand it to the intial size. */
	if( d->ht[0].size == 0 ) {
		return Dict_Expand( d, DICT_HT_INITIAL_SIZE );
	}
	/* If we reached the 1:1 ratio, and we are allowed to resize the hash
	* table (global setting) or we should avoid it but the ratio between
	* elements/buckets is over the "safe" threshold, we resize doubling
	* the number of buckets. */
	if( d->ht[0].used >= d->ht[0].size &&
	    (dict_can_resize ||
	      d->ht[0].used / d->ht[0].size > dict_force_resize_ratio) ) {
		return Dict_Expand( d, ((d->ht[0].size > d->ht[0].used) ?
					d->ht[0].size : d->ht[0].used) * 2 );
	}
	return 0;
}

static unsigned long Dict_NextPower( unsigned long size )
{
	unsigned long i = DICT_HT_INITIAL_SIZE;

	if( size >= LONG_MAX ) return LONG_MAX;
	while( 1 ) {
		if( i >= size )
			return i;
		i *= 2;
	}
}

/** 
 * 获取字典中与 key 对应的空槽的索引 
 * @returns 如果 key 已经存在，则返回 -1 
 */
static int Dict_KeyIndex( Dict *d, const void *key )
{
	DictEntry *he;
	unsigned int h, idx, table;
	if( Dict_ExpandIfNeeded( d ) == -1 ) {
		return -1;
	}
	h = Dict_HashKey( d, key );
	for( table = 0; table <= 1; table++ ) {
		idx = h & d->ht[table].sizemask;
		he = d->ht[table].table[idx];
		while( he ) {
			if( Dict_CompareKeys( d, key, he->key ) ) {
				return -1;
			}
			he = he->next;
		}
		if( !Dict_IsRehashing( d ) ) {
			break;
		}
	}
	return idx;
}

void Dict_Empty( Dict *d )
{
	Dict_Clear( d, &d->ht[0] );
	Dict_Clear( d, &d->ht[1] );
	d->rehashidx = -1;
	d->iterators = 0;
}

#define DICT_STATS_VECTLEN 50
static void Dict_PrintStatsHt( DictHashTable *ht )
{
	unsigned long totchainlen = 0;
	unsigned long clvector[DICT_STATS_VECTLEN];
	unsigned long i, slots = 0, chainlen, maxchainlen = 0;
	if( ht->used == 0 ) {
		LOG( "No stats available for empty dictionaries\n" );
		return;
	}
	for( i = 0; i < DICT_STATS_VECTLEN; i++ ) clvector[i] = 0;
	for( i = 0; i < ht->size; i++ ) {
		DictEntry *he;
		if( ht->table[i] == NULL ) {
			clvector[0]++;
			continue;
		}
		slots++;
		chainlen = 0;
		he = ht->table[i];
		while( he ) {
			chainlen++;
			he = he->next;
		}
		if( chainlen < DICT_STATS_VECTLEN ) {
			clvector[chainlen]++;
		} else {
			clvector[DICT_STATS_VECTLEN - 1]++;
		}
		if( chainlen > maxchainlen ) {
			maxchainlen = chainlen;
		}
		totchainlen += chainlen;
	}
	LOG( "Hash table stats:\n" );
	LOG( " table size: %ld\n", ht->size );
	LOG( " number of elements: %ld\n", ht->used );
	LOG( " different slots: %ld\n", slots );
	LOG( " max chain length: %ld\n", maxchainlen );
	LOG( " avg chain length (counted): %.02f\n", (float)totchainlen / slots );
	LOG( " avg chain length (computed): %.02f\n", (float)ht->used / slots );
	LOG( " Chain length distribution:\n" );
	for( i = 0; i < DICT_STATS_VECTLEN - 1; i++ ) {
		if( clvector[i] == 0 ) {
			continue;
		}
		LOG( "   %s%ld: %ld (%.02f%%)\n", 
			(i == DICT_STATS_VECTLEN - 1) ? ">= " : "", 
			i, clvector[i], ((float)clvector[i] / ht->size)*100 );
	}
}

void Dict_PrintStats( Dict *d )
{
	Dict_PrintStatsHt( &d->ht[0] );
	if( Dict_IsRehashing( d ) ) {
		LOG( "-- Rehashing into ht[1]:\n" );
		Dict_PrintStatsHt( &d->ht[1] );
	}
}

void Dict_EnableResize( void )
{
	dict_can_resize = 1;
}

void Dict_DisableResize( void )
{
	dict_can_resize = 0;
}

static unsigned int StringCopyKeyDict_KeyHash( const void *key )
{
	const char *buf = key;
	unsigned int hash = 5381;
	while( *buf ) {
		hash = ((hash << 5) + hash) + (*buf++);
	}
	return hash;
}

static int StringCopyKeyDict_KeyCompare( void *privdata, const void *key1,
					 const void *key2 )
{
	if( strcmp( key1, key2 ) == 0 ) {
		return 1;
	}
	return 0;
}

static void *StringCopyKeyDict_KeyDup( void *privdata, const void *key )
{
	char *newkey = malloc( (strlen( key ) + 1)*sizeof( char ) );
	strcpy( newkey, key );
	return newkey;
}

static void StringCopyKeyDict_KeyDestructor( void *privdata, void *key )
{
	free( key );
}

DictType DictType_StringKey = {
	StringCopyKeyDict_KeyHash,
	NULL,
	NULL,
	StringCopyKeyDict_KeyCompare,
	NULL,
	NULL
};

DictType DictType_StringCopyKey = {
	StringCopyKeyDict_KeyHash,
	StringCopyKeyDict_KeyDup,
	NULL,
	StringCopyKeyDict_KeyCompare,
	StringCopyKeyDict_KeyDestructor,
	NULL
};
