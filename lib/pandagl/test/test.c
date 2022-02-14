#include "test.h"
#include "ctest.h"

int main()
{
	int ret = 0;

	describe("test_canvas_mix", test_canvas_mix);
	return ret - print_test_result();
}
