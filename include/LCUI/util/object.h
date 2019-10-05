/* object.h -- Object wrapper and operation set
 *
 * Copyright (c) 2019, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
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

#ifndef LCUI_OBJECT_H
#define LCUI_OBJECT_H

#include <wchar.h>
#include <LCUI/util/linkedlist.h>

LCUI_BEGIN_HEADER

typedef struct LCUI_ObjectTypeRec_ LCUI_ObjectTypeRec;
typedef struct LCUI_ObjectTypeRec_ *LCUI_ObjectType;
typedef struct LCUI_ObjectRec_ *LCUI_Object;
typedef struct LCUI_ObjectRec_ LCUI_ObjectRec;
typedef struct LCUI_ObjectWatcherRec_ *LCUI_ObjectWatcher;
typedef void (*LCUI_ObjectWatcherFunc)(LCUI_Object, void *);

struct LCUI_ObjectTypeRec_ {
	unsigned hash;
	void (*init)(LCUI_Object);
	void (*destroy)(LCUI_Object);
	int (*compare)(LCUI_Object, LCUI_Object);
	LCUI_Object (*opreate)(LCUI_Object, const char *,
				const LCUI_ObjectRec *);
	void (*duplicate)(LCUI_Object, const LCUI_ObjectRec *);
	void (*tostring)(LCUI_Object, LCUI_Object);
};

typedef struct LCUI_ObjectRec_ {
	const LCUI_ObjectTypeRec *type;
	union LCUI_ObjectValueUnion {
		double number;
		char *string;
		wchar_t *wstring;
		void *data;
	} value;
	size_t size;
	LinkedList *watchers;
} LCUI_ObjectRec;

LCUI_API LCUI_ObjectType ObjectType_New(const char *name);

LCUI_API void ObjectType_Delete(LCUI_ObjectType type);

LCUI_API void Object_Init(LCUI_Object object, const LCUI_ObjectTypeRec *type);

LCUI_API void Object_Destroy(LCUI_Object object);

LCUI_API LCUI_Object Object_New(const LCUI_ObjectTypeRec *type);

LCUI_API void Object_Delete(LCUI_Object object);

LCUI_API LCUI_Object Object_Duplicate(LCUI_Object src);

LCUI_API int Object_Compare(LCUI_Object a, LCUI_Object b);

LCUI_API LCUI_Object Object_Operate(LCUI_Object self, const char *operator_str,
				    LCUI_Object another);

LCUI_API LCUI_Object Object_ToString(LCUI_Object object);

LCUI_API LCUI_ObjectWatcher Object_Watch(LCUI_Object object,
					 LCUI_ObjectWatcherFunc func,
					 void *data);

LCUI_API size_t Object_Notify(LCUI_Object object);

LCUI_API void ObjectWatcher_Delete(LCUI_ObjectWatcher watcher);

LCUI_API void WString_SetValue(LCUI_Object str, const wchar_t *value);

LCUI_API void WString_Init(LCUI_Object object, const wchar_t *value);

LCUI_API LCUI_Object WString_New(const wchar_t *value);

LCUI_API void String_SetValue(LCUI_Object str, const char *value);

LCUI_API void String_Init(LCUI_Object object, const char *value);

LCUI_API LCUI_Object String_New(const char *value);

LCUI_API void Number_Init(LCUI_Object object, double value);

LCUI_API void Number_SetValue(LCUI_Object object, double value);

LCUI_API LCUI_Object Number_New(double value);

extern const LCUI_ObjectTypeRec *LCUI_WStringObject;

extern const LCUI_ObjectTypeRec *LCUI_StringObject;

extern const LCUI_ObjectTypeRec *LCUI_NumberObject;

LCUI_END_HEADER

#endif
