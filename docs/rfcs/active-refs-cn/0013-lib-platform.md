# 平台库

- 开始日期：2023-04-22
- 目标主要版本：3.x
- 参考问题：[#246](https://github.com/lc-soft/LCUI/discussions/246)
- 实现 PR：无

## 概括

将窗口管理、消息循环、系统相关接口整合为 libplatform 平台支持库，为应用程序的平台相关功能提供跨平台统一的接口。

## 基本示例

应用程序的初始化和主循环：

```c
#include <platform.h>
#include <platform/main.h>

int main(int argc, char *argv[])
{
    // 初始化应用程序，传入的字符串用于注册窗口类名
    app_init(L"My Application");

    // 运行主循环
    return app_run();
}
```

事件处理：

```c
#include <stdio.h>
#include <platform.h>

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

窗口管理：

```c
#include <pandagl.h>
#include <platform.h>

void on_window_paint(app_event_t *e, void *arg)
{
    app_window_paint_t paint = { 0 };
    // 开始绘制，创建绘制上下文
    paint = app_window_begin_paint(e->window, &e->paint.rect);
    // 自定义绘制窗口内容，例如填充白色：
    pd_canvas_fill_rect(&paint->canvas, RGB(255, 255, 255), NULL, TRUE);
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

使用步进定时器管理定时循环，典型的用法是在渲染每帧之前调用一次 Tick 回调函数，例如 1 秒内调用 60 次以实现每秒渲染 60 帧画面。

```c
#include <platform.h>

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

## 动机

## 详细设计

### 步进定时器

2.x 版本中的步进定时器是基于互斥锁、条件变量定时等待实现的，每次调用 `StepTimer_Remain()` 时若当前帧耗时低于指定的平均耗时则会主动进入休眠以补全总耗时。这种做法会阻塞事件队列，况且步进定时器的目的只是为了限制渲染帧率，没必要连事件队列也限制，因此，应该重新设计步进定时器，取消它的主动休眠能力。

2.x 版本中的 UWP 适配代码用到了 DirectX 示例项目中自带的 StepTimer，它的用法如下：

```cpp
// 加载应用程序时加载并初始化应用程序资产。
Main::Main(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	// 注册以在设备丢失或重新创建时收到通知
	m_deviceResources->RegisterDeviceNotify(this);

	m_renderer = std::unique_ptr<Renderer>(new Renderer(m_deviceResources));

	// TODO: 如果需要默认的可变时间步长模式之外的其他模式，请更改计时器设置。
	// 例如，对于 60 FPS 固定时间步长更新逻辑，请调用:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

// 每帧更新一次应用程序状态。
void Main::Update()
{
	// 更新场景对象。
	m_timer.Tick([&]() {
		m_renderer->Update(m_timer);
	});
}
```

`m_timer.Tick()` 会让 `m_renderer->Update(m_timer)` 仅在合适的时机调用。显然，这种用法与我们的需求是非常匹配的，我们可以参考[Microsoft/DirectXTK/StepTimer](https://github.com/Microsoft/DirectXTK/wiki/StepTimer)的源码，将其改用 C 语言实现，以供 LCUI 的主循环使用。

## 缺点

无。

## 备选方案

改用 SDL。不建议采用此方案，因为改动较大，成本太高，还要投入成本去学习 SDL。

## 采用策略

这是个破坏性的改动，涉及主循环、窗口管理的代码。
