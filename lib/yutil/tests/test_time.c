#include "test.h"
#include "libtest.h"
#include <stdint.h>
#include "yutil/keywords.h"
#include "yutil/time.h"

void test_time(void)
{
        int64_t time1 = y_gettime();
        it_b("y_gettime() should work", time1 != 0, true);

        y_sleep(1000);

        it_b("y_gettime() subtraction should work", y_gettime() - time1 != 0,
             true);
        it_b("y_sleep() should work", y_gettime() - time1 >= 900, true);

        int64_t time2 = y_gettime();
        y_sleep(100);
        it_b("y_sleep() should work", y_gettime() - time2 >= 90, true);
}
