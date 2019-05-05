#include <wchar.h>
#include <math.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/util/logger.h>
#include <LCUI/util/object.h>
#include "test.h"

static void on_object_change(LCUI_Object object, void *data)
{
	int *count = data;

	*count += 1;
}

static int test_string_object(void)
{
	int ret = 0;
	int object_changes = 0;
	LCUI_Object tmp;
	LCUI_Object str1 = String_New("hello");
	LCUI_Object str2 = String_New("world");
	LCUI_ObjectRec str3;
	LCUI_ObjectWatcher watcher;

	watcher = Object_Watch(str1, on_object_change, &object_changes);
	CHECK(watcher != NULL);

	String_Init(&str3, "helloworld");
	tmp = Object_Operate(str1, "+", str2);
	CHECK(Object_Compare(&str3, tmp) == 0);
	CHECK(Object_Operate(str1, "+=", str2) == str1);
	CHECK(Object_Compare(str1, &str3) == 0);
	CHECK(strcmp(str1->value.string, "helloworld") == 0);
	CHECK(object_changes == 1);

	ObjectWatcher_Delete(watcher);
	Object_Delete(tmp);
	Object_Delete(str1);
	Object_Delete(str2);
	Object_Destroy(&str3);
	return ret;
}

static int test_wstring_object(void)
{
	int ret = 0;
	LCUI_Object tmp;
	LCUI_Object str1 = WString_New(L"测试");
	LCUI_Object str2 = WString_New(L"文本");
	LCUI_ObjectRec str3;
	LCUI_Object str4;

	WString_Init(&str3, L"测试文本");
	tmp = Object_Operate(str1, "+", str2);
	CHECK(Object_Compare(&str3, tmp) == 0);
	CHECK(Object_Operate(str1, "+=", str2) == str1);
	CHECK(Object_Compare(str1, &str3) == 0);
	CHECK(wcscmp(str1->value.wstring, L"测试文本") == 0);

	WString_SetValue(&str3, L"1000");
	str4 = Object_ToString(&str3);
	CHECK(strcmp(str4->value.string, "1000") == 0);

	Object_Delete(tmp);
	Object_Delete(str1);
	Object_Delete(str2);
	Object_Delete(str4);
	Object_Destroy(&str3);
	return ret;
}

static int test_number_object(void)
{
	int ret = 0;
	int object_changes = 0;
	LCUI_Object tmp;
	LCUI_Object str;
	LCUI_ObjectRec num1;
	LCUI_ObjectRec num2;
	LCUI_ObjectWatcher watcher;

	Number_Init(&num1, 1.25);
	Number_Init(&num2, 4.0);

	watcher = Object_Watch(&num1, on_object_change, &object_changes);
	CHECK(watcher != NULL);

	tmp = Object_Operate(&num1, "*", &num2);
	CHECK(Object_Operate(&num1, "*=", &num2) == &num1);
	CHECK(Object_Compare(&num1, tmp) == 0);
	CHECK(tmp->value.number == 5.0);
	CHECK(object_changes == 1);
	Object_Delete(tmp);

	tmp = Object_Operate(&num1, "/", &num2);
	CHECK(Object_Operate(&num1, "/=", &num2) == &num1);
	CHECK(Object_Compare(&num1, tmp) == 0);
	CHECK(tmp->value.number == 1.25);
	CHECK(object_changes == 2);
	Object_Delete(tmp);

	Number_SetValue(&num1, 4.5);
	Number_SetValue(&num2, 5.5);
	CHECK(object_changes == 3);

	tmp = Object_Operate(&num1, "+", &num2);
	CHECK(Object_Operate(&num1, "+=", &num2) == &num1);
	CHECK(Object_Compare(&num1, tmp) == 0);
	CHECK(tmp->value.number == 10.0);
	CHECK(object_changes == 4);
	Object_Delete(tmp);

	Number_SetValue(&num1, 12.5);
	Number_SetValue(&num2, 7.5);
	CHECK(object_changes == 5);

	tmp = Object_Operate(&num1, "-", &num2);
	CHECK(Object_Operate(&num1, "-=", &num2) == &num1);
	CHECK(Object_Compare(&num1, tmp) == 0);
	CHECK(tmp->value.number == 5.0);
	CHECK(object_changes == 6);
	Object_Delete(tmp);

	str = Object_ToString(&num1);
	CHECK(strcmp(str->value.string, "5") == 0);

	ObjectWatcher_Delete(watcher);
	Object_Destroy(&num1);
	Object_Delete(str);
	return ret;
}

int test_object(void)
{
	int ret = 0;

	ret += test_string_object();
	ret += test_wstring_object();
	ret += test_number_object();
	return ret;
}
