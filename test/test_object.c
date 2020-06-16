#include <wchar.h>
#include <math.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/util/logger.h>
#include <LCUI/util/object.h>
#include "test.h"
#include "libtest.h"

static void on_object_change(LCUI_Object object, void *data)
{
	int *count = data;

	*count += 1;
}

static void test_string_object(void)
{
	int object_changes = 0;
	LCUI_Object tmp;
	LCUI_Object str1 = String_New("hello");
	LCUI_Object str2 = String_New("world");
	LCUI_ObjectRec str3;
	LCUI_ObjectWatcher watcher;

	watcher = Object_Watch(str1, on_object_change, &object_changes);
	it_b("check Object_Watch() is valid", watcher != NULL, TRUE);

	String_Init(&str3, "helloworld");
	tmp = Object_Operate(str1, "+", str2);
	it_i("check string object concatenation", Object_Compare(&str3, tmp),
	     0);
	it_b("check string object append stores result in correct object",
	     Object_Operate(str1, "+=", str2) == str1, TRUE);
	it_i("check string object append has correct result",
	     Object_Compare(str1, &str3), 0);
	it_s("check raw string from string object", str1->value.string,
	     "helloworld");
	it_i("check object change notification", object_changes, 1);

	ObjectWatcher_Delete(watcher);
	Object_Delete(tmp);
	Object_Delete(str1);
	Object_Delete(str2);
	Object_Destroy(&str3);
}

static void test_wstring_object(void)
{
	LCUI_Object tmp;
	LCUI_Object str1 = WString_New(L"测试");
	LCUI_Object str2 = WString_New(L"文本");
	LCUI_ObjectRec str3;
	LCUI_Object str4;

	WString_Init(&str3, L"测试文本");
	tmp = Object_Operate(str1, "+", str2);
	it_i("check wstring object concatenation", Object_Compare(&str3, tmp),
	     0);
	it_b("check wstring object append stores result in correct object",
	     Object_Operate(str1, "+=", str2) == str1, TRUE);
	it_i("check wstring object append has correct result",
	     Object_Compare(str1, &str3), 0);
	it_b("check raw wstring from string object",
	     wcscmp(str1->value.wstring, L"测试文本") == 0, TRUE);

	WString_SetValue(&str3, L"1000");
	str4 = Object_ToString(&str3);
	it_s("check convert wstring object to raw string", str4->value.string,
	     "1000");

	Object_Delete(tmp);
	Object_Delete(str1);
	Object_Delete(str2);
	Object_Delete(str4);
	Object_Destroy(&str3);
}

static void test_number_object(void)
{
	int object_changes = 0;
	LCUI_Object tmp;
	LCUI_Object str;
	LCUI_ObjectRec num1;
	LCUI_ObjectRec num2;
	LCUI_ObjectWatcher watcher;

	Number_Init(&num1, 1.25);
	Number_Init(&num2, 4.0);

	watcher = Object_Watch(&num1, on_object_change, &object_changes);
	it_b("check Object_Watch() is valid", watcher != NULL, TRUE);

	tmp = Object_Operate(&num1, "*", &num2);
	it_b("check number object *= stores result in left object",
	     Object_Operate(&num1, "*=", &num2) == &num1, TRUE);
	it_i("check result of number object *=", Object_Compare(&num1, tmp), 0);
	it_b("check raw value of number object", tmp->value.number == 5.0,
	     TRUE);
	it_i("check object change notifications", object_changes, 1);
	Object_Delete(tmp);

	tmp = Object_Operate(&num1, "/", &num2);
	it_b("check number object /= stores result in left object",
	     Object_Operate(&num1, "/=", &num2) == &num1, TRUE);
	it_i("check result of number object /=", Object_Compare(&num1, tmp), 0);
	it_b("check raw value of number", tmp->value.number == 1.25, TRUE);
	it_i("check number object change notifications", object_changes, 2);
	Object_Delete(tmp);

	Number_SetValue(&num1, 4.5);
	Number_SetValue(&num2, 5.5);
	it_i("check numnber object change notifications", object_changes, 3);

	tmp = Object_Operate(&num1, "+", &num2);
	it_b("check number object += stores result in left object",
	     Object_Operate(&num1, "+=", &num2) == &num1, TRUE);
	it_i("check result of number object +=", Object_Compare(&num1, tmp), 0);
	it_b("check raw value of number", tmp->value.number == 10.0, TRUE);
	it_i("check number of object change notifications", object_changes, 4);
	Object_Delete(tmp);

	Number_SetValue(&num1, 12.5);
	Number_SetValue(&num2, 7.5);
	it_i("check number of object change notifications", object_changes, 5);

	tmp = Object_Operate(&num1, "-", &num2);
	it_b("check number object -= stores result in left object",
	     Object_Operate(&num1, "-=", &num2) == &num1, TRUE);
	it_i("check result of number object -=", Object_Compare(&num1, tmp), 0);
	it_b("check raw value of number", tmp->value.number == 5.0, TRUE);
	it_i("check number of object change notifications", object_changes, 6);
	Object_Delete(tmp);

	str = Object_ToString(&num1);
	it_s("check result of converting number object to string",
	     str->value.string, "5");

	ObjectWatcher_Delete(watcher);
	Object_Destroy(&num1);
	Object_Delete(str);
}

void test_object(void)
{
	describe("test string object", test_string_object);
	describe("test wstring object", test_wstring_object);
	describe("test number object", test_number_object);
}
