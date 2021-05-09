/* Hash Tables Implementation.
 *
 * This file implements in-memory hash tables with insert/del/replace/find/
 * get-random-element operations. Hash tables will auto-resize if needed
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

#include <stdint.h>

#ifndef LCUI_UTIL_DICT_H
#define LCUI_UTIL_DICT_H

LCUI_BEGIN_HEADER

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
	unsigned int(*hashFunction)(const void *key);
	void *(*keyDup)(void *privdata, const void *key);
	void *(*valDup)(void *privdata, const void *obj);
	int(*keyCompare)(void *privdata, const void *key1, const void *key2);
	void(*keyDestructor)(void *privdata, void *key);
	void(*valDestructor)(void *privdata, void *obj);
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

/** 创建一个新字典 */
LCUI_API Dict *Dict_Create(DictType *type, void *privdata);

/** 对字典进行扩展 */
LCUI_API int Dict_Expand(Dict *d, unsigned long size);

/**
 * 将元素添加到目标哈希表中
 * @param[in] d 字典指针
 * @param[in] key 新元素的关键字
 * @param[in] val 新元素的值
 * @returns 添加成功为 0，添加出错为 -1
 */
LCUI_API int Dict_Add(Dict *d, void *key, void *val);
/**
 * 将元素添加到目标哈希表中
 * 功能与 Dict_Add() 相同，但必须指定 valDup 才能添加成功
*/
LCUI_API int Dict_AddCopy(Dict *d, void *key, const void *val);

/** 添加元素的底层实现函数(由 Dict_Add 调用) */
LCUI_API DictEntry *Dict_AddRaw(Dict *d, void *key);

/**
 * 将新元素添加到字典，如果 key 已经存在，那么新元素覆盖旧元素。
 * @param[in] key 新元素的关键字
 * @param[in] val 新元素的值
 * @return 1 key 不存在，新建元素添加成功
 * @return 0 key 已经存在，旧元素被新元素覆盖
 */
LCUI_API int Dict_Replace(Dict *d, void *key, void *val);
LCUI_API DictEntry *Dict_ReplaceRaw(Dict *d, void *key);
LCUI_API int Dict_Delete(Dict *d, const void *key);
LCUI_API int Dict_DeleteNoFree(Dict *d, const void *key);

/** 删除字典，释放内存资源 */
LCUI_API void Dict_Release(Dict *d);

/**
 * 在字典中按指定的 key 查找
 * 查找过程是典型的 separate chaining find 操作
 * 具体参见：http://en.wikipedia.org/wiki/Hash_table#Separate_chaining
 */
LCUI_API DictEntry * Dict_Find(Dict *d, const void *key);

/** 查找给定 key 在字典 d 中的值 */
LCUI_API void *Dict_FetchValue(Dict *d, const void *key);

/** 重新调整字典的大小，缩减多余空间 */
LCUI_API int Dict_Resize(Dict *d);

/**
 * 创建一个迭代器，用于遍历哈希表节点。
 *
 * safe 属性指示迭代器是否安全，如果迭代器是安全的，那么它可以在遍历的过程中
 * 进行增删操作，反之，如果迭代器是不安全的，那么它只能执行 Dict_Next 操作。
 *
 * 因为迭代进行的时候可以对列表的当前节点进行修改，为了避免修改造成指针丢失，
 * 所以不仅要有指向当前节点的 entry 属性，还需要指向下一节点的 next_entry 属性
 */
LCUI_API DictIterator *Dict_GetIterator(Dict *d);

/** 创建一个安全迭代器 */
LCUI_API DictIterator *Dict_GetSafeIterator(Dict *d);

/** 迭代器的推进函数 */
LCUI_API DictEntry *Dict_Next(DictIterator *iter);

/** 删除迭代器 */
LCUI_API void Dict_ReleaseIterator(DictIterator *iter);

/** 从字典中随机获取一项 */
LCUI_API DictEntry *Dict_GetRandomKey(Dict *d);

/** 打印字典的统计信息 */
LCUI_API void Dict_PrintStats(Dict *d);

LCUI_API unsigned int Dict_GenHashFunction(const unsigned char *buf, int len);
LCUI_API unsigned int Dict_GenCaseHashFunction(const unsigned char *buf, int len);
LCUI_API unsigned int Dict_IntHashFunction(unsigned int key);

/* Identity hash function for integer keys */
LCUI_API unsigned int Dict_IdentityHashFunction(unsigned int key);

/** 清空字典 */
LCUI_API void Dict_Empty(Dict *d);
LCUI_API void Dict_EnableResize(void);
LCUI_API void Dict_DisableResize(void);

/**
 * 字典的 rehash 函数
 * @param[in] n 要执行 rehash 的元素数量
 * @return 0 所有元素 rehash 完毕
 * @return 1 还有元素没有 rehash
*/
LCUI_API int Dict_Rehash(Dict *d, int n);

/*
 * 在指定的时间内完成 rehash 操作
 * @param[in] ms 进行 rehash 的时间，以毫秒为单位
 * @returns rehashes 完成 rehash 的元素的数量
 */
LCUI_API int Dict_RehashMilliseconds(Dict *d, int ms);
LCUI_API void Dict_SetHashFunctionSeed(unsigned int initval);
LCUI_API unsigned int Dict_GetHashFunctionSeed(void);

LCUI_API unsigned int StringKeyDict_KeyHash(const void *key);
LCUI_API int StringKeyDict_KeyCompare(void *privdata, const void *key1,
				      const void *key2);
LCUI_API void *StringKeyDict_KeyDup(void *privdata, const void *key);
LCUI_API void StringKeyDict_KeyDestructor(void *privdata, void *key);

LCUI_API void Dict_InitStringKeyType(DictType *t);
LCUI_API void Dict_InitStringCopyKeyType(DictType *t);

LCUI_END_HEADER

#endif /* LCUI_UTIL_DICT_H */
