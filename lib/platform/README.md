# Platform

平台支持库，为应用程序的平台相关功能提供跨平台统一的接口。

## API

目前该库主要提供窗口和事件循环相关操作接口，这里仅以示例代码的形式简单展示部分接口的用法，如需了解更多请查看头文件。

### Platform::App

应用程序。

```c
#include <LCUI/platform.h>

int main(int argc, char *argv[])
{
    // 初始化应用程序，传入的字符串用于注册窗口类名
    app_init(L"My Application");

    // 运行主循环
    return app_run();
}
```

`app_run()` 会进入事件循环中一直处理并等待下个事件，直到收到 QUIT 事件时才会返回。

### Platform::App::Events

事件队列。

```c
#include <stdio.h>
#include <LCUI/platform.h>

#define MY_CUSTOM_EVENT (APP_EVENT_USER + 1)

void on_my_custom_event(app_event_t *e, void *arg)
{
    const char *str = arg;

    printf("my custom event, str: %s\n", str);
}

int main(int argc, char *argv[])
{
    app_window_t *wnd;
    app_event_t e = { 0 };
    const char *listener_data = "event listener data";
    const char *event_data = "event data";

    // 设置事件类型
    e.type = MY_CUSTOM_EVENT;
    // 设置事件相关数据
    e.data = event_data;

    app_init(L"My Application");
    // 添加事件处理器，也就是将回调函数与事件绑定
    app_on_event(MY_CUSTOM_EVENT, on_my_custom_event, listener_data);
    // 投递事件到事件队列，等待被事件循环处理
    app_post_event(&e);
    return app_run();
}
```

### Platform::App::Window

窗口。

```c
#include <LCUI/graph.h>
#include <LCUI/platform.h>

void on_window_paint(app_event_t *e, void *arg)
{
    app_window_paint_t paint = { 0 };
    // 开始绘制，创建绘制上下文
    paint = app_window_begin_paint(e->window, &e->paint.rect);
    // 自定义绘制窗口内容，例如填充白色：
    pd_canvas_fill_rect(&paint->canvas, RGB(255, 255, 255), NULL, true);
    // 结束绘制，销毁绘制上下文
	app_window_end_paint(e->window, paint);
}

int main(int argc, char *argv[])
{
    app_window_t *wnd;

    app_init(L"My Application");
    // 创建一个窗口，并设置初始标题、位置和尺寸
    wnd = app_window_create("Main window", 200, 200, 800, 600, NULL);
    // 设置尺寸
    app_window_set_size(wnd, 320, 240);
    // 设置位置
    app_window_set_position(wnd, 100, 100);
    // 激活窗口
    app_window_activate(wnd);
    // 添加窗口绘制事件处理器
    app_on_event(APP_EVENT_PAINT, on_window_paint, NULL);
    return app_run();
}
```

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
    timer.is_fixed_time_step = true;
	step_timer_init(&timer);
    while (frames <= 240) {
        step_timer_tick(&timer, on_tick, &frames);
    }
    return 0;
}
```
