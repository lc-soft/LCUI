#include "test.h"
#include "ctest.h"

int main()
{
	int ret = 0;

	ctest_describe("test_canvas_mix", test_canvas_mix);
	return ret - ctest_finish();
}
