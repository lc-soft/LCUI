# Platform

平台支持库，为应用程序的平台相关功能提供跨平台统一的接口。

## API

目前该库主要提供窗口和事件循环相关操作接口，这里仅以示例代码的形式简单展示部分接口的用法，如需了解更多请查看头文件。

### Platform::App

应用程序。

`app_run()` 会进入事件循环中一直处理并等待下个事件，直到收到 QUIT 事件时才会返回。

### Platform::App::Events

### Platform::App::Window

窗口。

### Platform::StepTimer

步进定时器，用于管理定时循环。典型的用法是在渲染每帧之前调用一次 Tick 回调函数，例如 1 秒内调用 60 次以实现每秒渲染 60 帧画面。

代码修改自：https://github.com/Microsoft/DirectXTK/wiki/StepTimer

```c
#include <LCUI/platform.h>

void on_tick(step_timer_t *timer, void *data)
{
    int *frames = data;

    *frames += 1;
    printf("tick\n");
}

int main(int argc, char *argv)
{
    int frames = 0;
    step_timer_t timer;

    // 设置每秒 60 tick
    timer.target_elapsed_time = 1000 / 60;
    timer.is_fixed_time_step = TRUE;
	step_timer_init(&timer);
    while (frames <= 240) {
        step_timer_tick(&timer, on_tick, &frames);
    }
    return 0;
}
```
