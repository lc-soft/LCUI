#include "test.h"
#include "libtest.h"
#include <stdint.h>
#include "yutil/keywords.h"
#include "yutil/time.h"

void test_time(void)
{
	int64_t time1 = get_time_ms();
	it_b("get_time_ms() should work", time1 != 0, true);

	it_b("get_time_us() should work", get_time_us() != 0, true);

	sleep_s(1);

	it_b("get_time_delta() should work", get_time_delta(time1) != 0, true);
	it_b("sleep_ms() should work", get_time_delta(time1) >= 90, true);
	int64_t time2 = get_time_ms();

	sleep_ms(100);

	it_b("sleep_ms() should work", get_time_delta(time2) >= 90, true);
}