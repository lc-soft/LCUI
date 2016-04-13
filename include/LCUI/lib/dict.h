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

#include <stdint.h>

#ifndef __DICT_H__
#define __DICT_H__

/** 哈希表节点结构 */
typedef struct DictEntry {
	void *key;
	union {
		void *val;
		uint64_t u64;
		int64_t s64;
	} v;
	struct DictEntry *next; /**< 指向下一个哈希节点(形成链表) */
} DictEntry;

/** 字典内数据的类型 */
typedef struct DictType {
	unsigned int( *hashFunction )(const void *key);
	void *(*keyDup)(void *privdata, const void *key);
	void *(*valDup)(void *privdata, const void *obj);
	int( *keyCompare )(void *privdata, const void *key1, const void *key2);
	void( *keyDestructor )(void *privdata, void *key);
	void( *valDestructor )(void *privdata, void *obj);
} DictType;

/** 哈希表结构 */
typedef struct DictHashTable {
	DictEntry **table;	/**< 节点指针数组 */
	unsigned long size;	/**< 桶的数量 */
	unsigned long sizemask;	/**< mask 码，用于地址索引计算 */
	unsigned long used;	/**< 已有节点数量 */
} DictHashTable;

/** 字典结构 */
typedef struct Dict {
	DictType *type;		/**< 为哈希表中不同类型的值所使用的一族函数 */
	void *privdata;
	DictHashTable ht[2];	/**< 每个字典使用两个哈希表 */
	int rehashidx;		/**< rehash 进行到的索引位置，如果没有在 rehash ，就为 -1 */
	int iterators;		/**< 当前正在使用的 iterator 的数量 */
} Dict;

/** 用于遍历字典的迭代器 */
typedef struct DictIterator {
	Dict *d;		/**< 迭代器所指向的字典 */
	int table;		/**< 使用的哈希表号码 */
	int index;		/**< 迭代进行的索引 */
	int safe;		/**< 是否安全 */
	DictEntry *entry;	/**< 指向哈希表的当前节点 */
	DictEntry *next_entry;	/**< 指向哈希表的下个节点 */
} DictIterator;

#define DICT_HT_INITIAL_SIZE 4

#define Dict_FreeVal(d, entry) \
    if ((d)->type->valDestructor) \
        (d)->type->valDestructor((d)->privdata, (entry)->v.val)

#define Dict_SetVal(d, entry, _val_) do { \
    if ((d)->type->valDup) \
        entry->v.val = (d)->type->valDup((d)->privdata, _val_); \
    else \
        entry->v.val = (_val_); \
} while(0)

#define Dict_SetSignedIntegerVal(entry, _val_) \
    do { entry->v.s64 = _val_; } while(0)

#define Dict_SetUnsignedIntegerVal(entry, _val_) \
    do { entry->v.u64 = _val_; } while(0)

#define Dict_FreeKey(d, entry) \
    if ((d)->type->keyDestructor) \
        (d)->type->keyDestructor((d)->privdata, (entry)->key)

#define Dict_SetKey(d, entry, _key_) do { \
    if ((d)->type->keyDup) \
        entry->key = (d)->type->keyDup((d)->privdata, _key_); \
    else \
        entry->key = (_key_); \
} while(0)

#define Dict_CompareKeys(d, key1, key2) \
    (((d)->type->keyCompare) ? \
        (d)->type->keyCompare((d)->privdata, key1, key2) : \
        (key1) == (key2))

#define Dict_HashKey(d, key) (d)->type->hashFunction(key)
#define DictEntry_GetKey(he) ((he)->key)
#define DictEntry_GetVal(he) ((he)->v.val)
#define DictEntry_GetSignedIntegerVal(he) ((he)->v.s64)
#define DictEntry_GetUnsignedIntegerVal(he) ((he)->v.u64)
#define Dict_Slots(d) ((d)->ht[0].size+(d)->ht[1].size)
#define Dict_Size(d) ((d)->ht[0].used+(d)->ht[1].used)
#define Dict_IsRehashing(ht) ((ht)->rehashidx != -1)

LCUI_API Dict *Dict_Create( DictType *type, void *privdata );
LCUI_API int Dict_Expand( Dict *d, unsigned long size );
LCUI_API int Dict_Add( Dict *d, void *key, void *val );
LCUI_API DictEntry *Dict_AddRaw( Dict *d, void *key );
LCUI_API int Dict_Replace( Dict *d, void *key, void *val );
LCUI_API DictEntry *Dict_ReplaceRaw( Dict *d, void *key );
LCUI_API int Dict_Delete( Dict *d, const void *key );
LCUI_API int Dict_DeleteNoFree( Dict *d, const void *key );
LCUI_API void Dict_Release( Dict *d );
LCUI_API DictEntry * Dict_Find( Dict *d, const void *key );
LCUI_API void *Dict_FetchValue( Dict *d, const void *key );
LCUI_API int Dict_Resize( Dict *d );
LCUI_API DictIterator *Dict_GetIterator( Dict *d );
LCUI_API DictIterator *Dict_GetSafeIterator( Dict *d );
LCUI_API DictEntry *Dict_Next( DictIterator *iter );
LCUI_API void Dict_ReleaseIterator( DictIterator *iter );
LCUI_API DictEntry *Dict_GetRandomKey( Dict *d );
LCUI_API void Dict_PrintStats( Dict *d );
LCUI_API unsigned int Dict_GenHashFunction( const unsigned char *buf, int len );
LCUI_API unsigned int Dict_GenCaseHashFunction( const unsigned char *buf, int len );
LCUI_API void Dict_Empty( Dict *d );
LCUI_API void Dict_EnableResize( void );
LCUI_API void Dict_DisableResize( void );
LCUI_API int Dict_Rehash( Dict *d, int n );
LCUI_API int Dict_RehashMilliseconds( Dict *d, int ms );
LCUI_API void Dict_SetHashFunctionSeed( unsigned int initval );
LCUI_API unsigned int Dict_GetHashFunctionSeed( void );

/* Hash table types */
extern DictType dictTypeHeapStringCopyKey;
extern DictType dictTypeHeapStrings;
extern DictType dictTypeHeapStringCopyKeyValue;

#endif /* __DICT_H__ */
