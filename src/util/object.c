/* object.c -- Object wrapper and operation set
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

#include <assert.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/util/object.h>
#include <LCUI/util/string.h>
#include <LCUI/util/charset.h>

typedef struct LCUI_ObjectWatcherRec_ {
	void *data;
	LCUI_ObjectWatcherFunc func;
	LCUI_Object target;
	LinkedListNode node;
} LCUI_ObjectWatcherRec;

LCUI_ObjectType ObjectType_New(const char *name)
{
	LCUI_ObjectType type;

	type = malloc(sizeof(LCUI_ObjectTypeRec));
	if (!type) {
		return NULL;
	}
	type->hash = strhash(32, name);
	type->init = NULL;
	type->destroy = NULL;
	type->duplicate = NULL;
	type->opreate= NULL;
	return type;
}

void ObjectType_Delete(LCUI_ObjectType type)
{
	free(type);
}

void Object_Init(LCUI_Object object, const LCUI_ObjectTypeRec *type)
{
	object->size = 0;
	object->type = type;
	object->watchers = NULL;
	object->value.data = NULL;
	if (type && type->init) {
		type->init(object);
	}
}

void Object_Destroy(LCUI_Object object)
{
	if (object->type && object->type->destroy) {
		object->type->destroy(object);
	}
	if (object->watchers) {
		LinkedList_ClearData(object->watchers, free);
		free(object->watchers);
	}
}

LCUI_Object Object_New(const LCUI_ObjectTypeRec *type)
{
	LCUI_Object object;

	object = malloc(sizeof(LCUI_ObjectRec));
	if (!object) {
		return NULL;
	}
	Object_Init(object, type);
	return object;
}

void Object_Delete(LCUI_Object object)
{
	Object_Destroy(object);
	free(object);
}

LCUI_Object Object_Duplicate(LCUI_Object src)
{
	LCUI_Object dest;

	dest = Object_New(src->type);
	if (!dest) {
		return NULL;
	}
	if (src->type && src->type->duplicate) {
		src->type->duplicate(dest, src);
	}
	return dest;
}

int Object_Compare(LCUI_Object a, LCUI_Object b)
{
	if (a->type == a->type && a->type->compare) {
		return a->type->compare(a, b);
	}
	return a->value.string - b->value.string;
}

LCUI_Object Object_Operate(LCUI_Object self, const char *operator_str,
			   LCUI_Object another)
{
	assert(self->type == another->type);

	if (self->type->opreate) {
		return self->type->opreate(self, operator_str, another);
	}
	return self;
}

LCUI_Object Object_ToString(LCUI_Object object)
{
	LCUI_Object str;

	assert(object->type && object->type->tostring);
	str = String_New(NULL);
	object->type->tostring(object, str);
	return str;
}

LCUI_ObjectWatcher Object_Watch(LCUI_Object object, LCUI_ObjectWatcherFunc func,
				void *data)
{
	LCUI_ObjectWatcher watcher;

	if (!object->watchers) {
		object->watchers = malloc(sizeof(LinkedList));
		if (!object->watchers) {
			return NULL;
		}
		LinkedList_Init(object->watchers);
	}
	watcher = malloc(sizeof(LCUI_ObjectWatcherRec));
	if (!watcher) {
		return NULL;
	}
	watcher->target = object;
	watcher->func = func;
	watcher->data = data;
	watcher->node.data = watcher;
	LinkedList_AppendNode(object->watchers, &watcher->node);
	return watcher;
}

size_t Object_Notify(LCUI_Object object)
{
	size_t count = 0;
	LinkedListNode *node;
	LCUI_ObjectWatcher watcher;

	if (!object->watchers) {
		return 0;
	}
	for (LinkedList_Each(node, object->watchers)) {
		watcher = node->data;
		watcher->func(object, watcher->data);
		++count;
	}
	return count;
}

void ObjectWatcher_Delete(LCUI_ObjectWatcher watcher)
{
	LinkedList_Unlink(watcher->target->watchers, &watcher->node);
	free(watcher);
}

static void String_Destructor(LCUI_Object object)
{
	free(object->value.string);
	object->value.string = NULL;
}

static int String_Comparator(LCUI_Object a, LCUI_Object b)
{
	return strcmp(a->value.string, b->value.string);
}

static int String_Realloc(LCUI_Object str, size_t size)
{
	char *p;

	if (str->size >= size) {
		return 0;
	}
	p = realloc(str->value.string, size);
	if (p) {
		str->value.string = p;
		str->size = size;
		return 0;
	}
	return -1;
}

static LCUI_Object String_Operator(LCUI_Object a, const char *operator_str,
				   const LCUI_ObjectRec *b)
{
	LCUI_Object str;
	size_t size = (strlen(b->value.string) + 1) * sizeof(char);

	switch (operator_str[0]) {
	case '=':
		assert(String_Realloc(a, size) == 0);
		strcpy(a->value.string, b->value.string);
		Object_Notify(a);
		break;
	case '+':
		size += strlen(a->value.string) * sizeof(char);
		if (operator_str[1] == '=') {
			assert(String_Realloc(a, size) == 0);
			strcat(a->value.string, b->value.string);
			Object_Notify(a);
			break;
		}
		if (operator_str[1]) {
			break;
		}
		str = Object_Duplicate(a);
		assert(str != NULL);
		assert(String_Realloc(str, size) == 0);
		strcpy(str->value.string, a->value.string);
		strcat(str->value.string, b->value.string);
		return str;
	}
	return a;
}

static void WString_Destructor(LCUI_Object object)
{
	free(object->value.wstring);
	object->value.wstring = NULL;
}

static void String_Duplicator(LCUI_Object dest, const LCUI_ObjectRec *src)
{
	assert(String_Realloc(dest, src->size) == 0);
	strcpy(dest->value.string, src->value.string);
}

void String_SetValue(LCUI_Object str, const char *value)
{
	size_t size = sizeof(char);

	if (value) {
		size = sizeof(char) * (strlen(value) + 1);
		assert(String_Realloc(str, size) == 0);
		strcpy(str->value.string, value);
	} else {
		assert(String_Realloc(str, size) == 0);
		str->value.string[0] = 0;
	}
	Object_Notify(str);
}

void String_Init(LCUI_Object object, const char *value)
{
	Object_Init(object, LCUI_StringObject);
	String_SetValue(object, value);
}

LCUI_Object String_New(const char *value)
{
	LCUI_Object object;

	object = Object_New(LCUI_StringObject);
	String_SetValue(object, value);
	return object;
}

static void String_ToString(LCUI_Object str, LCUI_Object newstr)
{
	String_Duplicator(newstr, str);
}

static int WString_Comparator(LCUI_Object a, LCUI_Object b)
{
	return wcscmp(a->value.wstring, b->value.wstring);
}

static int WString_Realloc(LCUI_Object str, size_t size)
{
	wchar_t *p;

	if (str->size >= size) {
		return 0;
	}
	p = realloc(str->value.wstring, size);
	if (p) {
		str->value.wstring = p;
		str->size = size;
		return 0;
	}
	return -1;
}

static LCUI_Object WString_Operator(LCUI_Object a, const char *operator_str,
				    const LCUI_ObjectRec *b)
{
	LCUI_Object str;
	size_t size = (wcslen(b->value.wstring) + 1) * sizeof(wchar_t);

	switch (operator_str[0]) {
	case '=':
		assert(WString_Realloc(a, b->size) == 0);
		wcscpy(a->value.wstring, b->value.wstring);
		Object_Notify(a);
		break;
	case '+':
		size += wcslen(b->value.wstring) * sizeof(wchar_t);
		if (operator_str[1] == '=') {
			assert(WString_Realloc(a, size) == 0);
			wcscat(a->value.wstring, b->value.wstring);
			Object_Notify(a);
			break;
		}
		if (operator_str[1]) {
			break;
		}
		str = Object_Duplicate(a);
		assert(str != NULL);
		assert(WString_Realloc(str, size) == 0);
		wcscpy(str->value.wstring, a->value.wstring);
		wcscat(str->value.wstring, b->value.wstring);
		return str;
	}
	return a;
}

static void WString_Duplicator(LCUI_Object dest, const LCUI_ObjectRec *src)
{
	assert(WString_Realloc(dest, src->size) == 0);
	wcscpy(dest->value.wstring, src->value.wstring);
}

void WString_SetValue(LCUI_Object str, const wchar_t *value)
{
	size_t size = sizeof(wchar_t);

	if (value) {
		size = sizeof(wchar_t) * (wcslen(value) + 1);
		assert(WString_Realloc(str, size) == 0);
		wcscpy(str->value.wstring, value);
	} else {
		assert(WString_Realloc(str, size) == 0);
		str->value.wstring[0] = 0;
	}
	Object_Notify(str);
}

void WString_Init(LCUI_Object object, const wchar_t *value)
{
	Object_Init(object, LCUI_WStringObject);
	WString_SetValue(object, value);
}

LCUI_Object WString_New(const wchar_t *value)
{
	LCUI_Object object;

	object = Object_New(LCUI_WStringObject);
	WString_SetValue(object, value);
	return object;
}

static void WString_ToString(LCUI_Object str, LCUI_Object newstr)
{
	const size_t len = LCUI_EncodeUTF8String(NULL, str->value.wstring, 0) + 1;

	String_Realloc(newstr, len);
	LCUI_EncodeUTF8String(newstr->value.string, str->value.wstring, len);
}

void Number_Init(LCUI_Object object, double value)
{
	Object_Init(object, LCUI_NumberObject);
	object->value.number = value;
	object->size = sizeof(double);
}

void Number_SetValue(LCUI_Object object, double value)
{
	object->value.number = value;
	object->size = sizeof(double);
	Object_Notify(object);
}

LCUI_Object Number_New(double value)
{
	LCUI_Object object;

	object = Object_New(LCUI_NumberObject);
	object->value.number = value;
	object->size = sizeof(double);
	return object;
}

static int Number_Comparator(LCUI_Object a, LCUI_Object b)
{
	return (int)(a->value.number - b->value.number);
}

static LCUI_Object Number_Operator(LCUI_Object a, const char *operator_str,
				   const LCUI_ObjectRec *b)
{
	LCUI_Object tmp;

	switch (operator_str[0]) {
	case '=':
		assert(operator_str[1] == 0);
		a->value.number = b->value.number;
		Object_Notify(a);
		break;
	case '+':
		if (operator_str[1] == '=') {
			a->value.number += b->value.number;
			Object_Notify(a);
			break;
		}
		if (operator_str[1] == '+') {
			++a->value.number;
			Object_Notify(a);
			break;
		}
		assert(operator_str[1] == 0);
		tmp = Object_New(a->type);
		tmp->value.number = a->value.number + b->value.number;
		return tmp;
	case '-':
		if (operator_str[1] == '=') {
			a->value.number -= b->value.number;
			Object_Notify(a);
			break;
		}
		if (operator_str[1] == '-') {
			--a->value.number;
			Object_Notify(a);
			break;
		}
		assert(operator_str[1] == 0);
		tmp = Object_New(a->type);
		tmp->value.number = a->value.number - b->value.number;
		return tmp;
	case '*':
		if (operator_str[1] == '=') {
			a->value.number *= b->value.number;
			Object_Notify(a);
			break;
		}
		assert(operator_str[1] == 0);
		tmp = Object_New(a->type);
		tmp->value.number = a->value.number * b->value.number;
		return tmp;
	case '/':
		if (operator_str[1] == '=') {
			a->value.number /= b->value.number;
			Object_Notify(a);
			break;
		}
		assert(operator_str[1] == 0);
		tmp = Object_New(a->type);
		tmp->value.number = a->value.number / b->value.number;
		return tmp;
	}
	return a;
}

static void Number_Duplicator(LCUI_Object dest, const LCUI_ObjectRec *src)
{
	dest->value.number = src->value.number;
}

static void Number_ToString(LCUI_Object num, LCUI_Object str)
{
	String_Realloc(str, 24);
	snprintf(str->value.string, 24, "%g", num->value.number);
	str->value.string[23] = 0;
}

LCUI_ObjectTypeRec LCUI_NumberObjectRec = { 1,
					    NULL,
					    NULL,
					    Number_Comparator,
					    Number_Operator,
					    Number_Duplicator,
					    Number_ToString };

LCUI_ObjectTypeRec LCUI_WStringObjectRec = { 2,
					     NULL,
					     WString_Destructor,
					     WString_Comparator,
					     WString_Operator,
					     WString_Duplicator,
					     WString_ToString };

LCUI_ObjectTypeRec LCUI_StringObjectRec = { 3,
					    NULL,
					    String_Destructor,
					    String_Comparator,
					    String_Operator,
					    String_Duplicator,
					    String_ToString };

const LCUI_ObjectTypeRec *LCUI_WStringObject = &LCUI_WStringObjectRec;
const LCUI_ObjectTypeRec *LCUI_StringObject = &LCUI_StringObjectRec;
const LCUI_ObjectTypeRec *LCUI_NumberObject = &LCUI_NumberObjectRec;
