#include "test.h"
#include "ctest.h"

int main()
{
	ctest_describe("test_canvas_mix", test_canvas_mix);
	return ctest_finish();
}
