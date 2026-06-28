#include "test.h"
#include "libtest.h"
#include "yutil/keywords.h"
#include "yutil/math.h"

static void test_math_max(void)
{
        it_b("y_max() should work", (int)y_max(1, 2) == 2, true);
}

static void test_math_min(void)
{
        int ix = 1, iy = 2;
        unsigned ux = 1, uy = 2;
        double dx = 1.0, dy = 1.0000001;

        it_b("y_min() should work for int", (int)y_min(ix, iy) == ix, true);
        it_b("y_min() should work for unsigned int", (int)y_min(ux, uy) == ux,
             true);
        it_b("y_min() should work for double", (int)y_min(dx, dy) == dx, true);
}

void test_math(void)
{
        test_math_max();
        test_math_min();
}
