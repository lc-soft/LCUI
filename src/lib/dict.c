/* Hash Tables Implementation.
*
* This file implements in memory hash tables with insert/del/replace/find/
* get-rand-element operations. Hash tables will auto resize if needed
* tables of power of two in size are used, collisions are handled by
* chaining. See the source code for more information... :)
*
* Copyright (c) 2006-2010, Salvatore Sanfilippo <antirez at gmail dot com>
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

/** 创建一个新字典 */
Dict *Dict_Create( DictType *type, void *privdata )
{
	static int inited = 0;
	Dict *d = malloc( sizeof( Dict ) );
	Dict_Init( d, type, privdata );
	if( !inited ) {
		srand( time( NULL ) );
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

/** 重新调整字典的大小，缩减多余空间 */
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

/** 对字典进行扩展 */
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

/** 
 * 字典的 rehash 函数
 * @param[in] n 要执行 rehash 的元素数量
 * @return 0 所有元素 rehash 完毕
 * @return 1 还有元素没有 rehash
*/
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

#define timeInMilliseconds LCUI_GetTickCount

/*
 * 在指定的时间内完成 rehash 操作
 * @param[in] ms 进行 rehash 的时间，以毫秒为单位
 * @returns rehashes 完成 rehash 的元素的数量
 */
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

/** 
 * 将元素添加到目标哈希表中
 * @param[in] d 字典指针
 * @param[in] key 新元素的关键字
 * @param[in] val 新元素的值
 * @returns 添加成功为 0，添加出错为 -1
 */
int Dict_Add( Dict *d, void *key, void *val )
{
	DictEntry *entry = Dict_AddRaw( d, key );
	if( !entry ) {
		return -1;
	}
	Dict_SetVal( d, entry, val );
	return 0;
}

/** 添加元素的底层实现函数(由 Dict_Add 调用) */
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

/**
 * 将新元素添加到字典，如果 key 已经存在，那么新元素覆盖旧元素。
 * @param[in] key 新元素的关键字
 * @param[in] val 新元素的值
 * @return 1 key 不存在，新建元素添加成功
 * @return 0 key 已经存在，旧元素被新元素覆盖
 */
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
void Dict_Clear( Dict *d, DictHashTable *ht )
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

/** 删除字典 */
void Dict_Release( Dict *d )
{
	Dict_Clear( d, &d->ht[0] );
	Dict_Clear( d, &d->ht[1] );
	free( d );
}

/** 
 * 在字典中按指定的 key 查找
 * 查找过程是典型的 separate chaining find 操作
 * 具体参见：http://en.wikipedia.org/wiki/Hash_table#Separate_chaining
 */
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

/** 查找给定 key 在字典 d 中的值 */
void *Dict_FetchValue( Dict *d, const void *key )
{
	DictEntry *he = Dict_Find( d, key );
	return he ? DictEntry_GetVal( he ) : NULL;
}

/**
 * 创建一个迭代器，用于遍历哈希表节点。
 *
 * safe 属性指示迭代器是否安全，如果迭代器是安全的，那么它可以在遍历的过程中
 * 进行增删操作，反之，如果迭代器是不安全的，那么它只能执行 Dict_Next 操作。
 *
 * 因为迭代进行的时候可以对列表的当前节点进行修改，为了避免修改造成指针丢失，
 * 所以不仅要有指向当前节点的 entry 属性，还需要指向下一节点的 next_entry 属性
 */
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

/** 创建一个安全迭代器 */
DictIterator *Dict_GetSafeIterator( Dict *d )
{
	DictIterator *i = Dict_GetIterator( d );
	i->safe = 1;
	return i;
}

/** 迭代器的推进函数 */
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

/** 删除迭代器 */
void Dict_ReleaseIterator( DictIterator *iter )
{
	if( iter->safe && !(iter->index == -1 && iter->table == 0) ) {
		iter->d->iterators--;
	}
	free( iter );
}

/* 从字典中随机获取一项 */
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

/** 清空字典 */
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
		printf( "No stats available for empty dictionaries\n" );
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
	printf( "Hash table stats:\n" );
	printf( " table size: %ld\n", ht->size );
	printf( " number of elements: %ld\n", ht->used );
	printf( " different slots: %ld\n", slots );
	printf( " max chain length: %ld\n", maxchainlen );
	printf( " avg chain length (counted): %.02f\n", (float)totchainlen / slots );
	printf( " avg chain length (computed): %.02f\n", (float)ht->used / slots );
	printf( " Chain length distribution:\n" );
	for( i = 0; i < DICT_STATS_VECTLEN - 1; i++ ) {
		if( clvector[i] == 0 ) {
			continue;
		}
		printf( "   %s%ld: %ld (%.02f%%)\n", 
			(i == DICT_STATS_VECTLEN - 1) ? ">= " : "", 
			i, clvector[i], ((float)clvector[i] / ht->size)*100 );
	}
}

void Dict_PrintStats( Dict *d )
{
	Dict_PrintStatsHt( &d->ht[0] );
	if( Dict_IsRehashing( d ) ) {
		printf( "-- Rehashing into ht[1]:\n" );
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
