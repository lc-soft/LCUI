## time

提供 Linux 和 Windows 上的高精度时间的统一接口。

下面看下例子：
```c
void test_time(void)
{
	int64_t time1 = get_time_ms();

	// delay 1s
	sleep_s(1);

	//返回 time1 到现在时间的时间差
	get_time_delta(time1);

	int64_t time2 = get_time_us();

	// delay 100ms
	sleep_ms(100);
}

```