# libapp

一个为 GUI 应用程序提供窗口和事件管理接口的跨平台开发库。

## 设计参考

- https://docs.microsoft.com/zh-cn/uwp/api/windows.applicationmodel.core.iframeworkview?view=winrt-20348
- https://wiki.libsdl.org/CategoryEvents

## 对象

- **app_dispatcher**：事件分发器，一个线程上只会分配一个分发器，一个分发器可以管理多个窗口的事件。
- **app_window_paint**：窗口的绘制上下文。
- **app_window**： 窗口对象。
- **app**：应用程序，管理应用程序内包括窗口、事件等在内的资源。

## 接口设计

```c
typedef struct app_t app_t;
typedef struct app_framebuffer_t app_framebuffer_t;
typedef struct app_dispatcher_t app_dispatcher_t;
typedef struct app_event_t app_event_t;
typedef struct app_window_t app_window_t;
typedef struct app_window_rect_t app_window_rect_t;
typedef struct app_window_paint_t app_window_paint_t;

typedef enum app_event_type_t {
    APP_EVENT_NONE = 0,
    APP_EVENT_WINDOW_CLOSED,
    APP_EVENT_POINTER_MOVED,
    APP_EVENT_POINTER_PRESSED,
    APP_EVENT_POINTER_RELEASED,
    APP_EVENT_POINTER_WHEEL_CHANGED,
    APP_EVENT_KEY_DOWN,
    APP_EVENT_KEY_UP,
    APP_EVENT_PAINT,
    APP_EVENT_USER = 1000
};

typedef struct app_window_event_t {
    app_event_type_t type
};

typedef struct app_event_t {
    app_window_t window;
    app_window_event_t data;
}

struct app_framebuffer_t {
    unsigned width;
    unsigned height;
    unsigned char *data;
    unsigned bits_per_pixel;
    size_t line_length;
    size_t data_length;
};

struct app_window_rect_t {
    unsigned x;
    unsigned y;
    unsigned width;
    unsigned height;
}

struct app_window_paint_t {
    app_framebuffer_t fb;
    app_window_rect_t rect;
}

// 应用程序基本操作
void app_init(void);
void app_run(void);
void app_stop(void);
void app_free(void);

// 事件功能的内部方法
static void app_dispatcher_new(app_dispatcher_t **dispathcer);
static void app_dispatcher_get_event(app_dispatcher_t *dispathcer);
static void app_dispatcher_post_event(app_dispatcher_t *dispathcer,
                                      app_event_t *e);

// 事件操作
void app_get_event(app_event_t *e);
void app_post_event(app_event_t *e);
void app_poll_event(app_event_t *e);
void app_process_event(app_event_t *e);

// 窗口状态操作
void app_window_new(app_window_t **wnd, wchar_t *title,
                    int x, int y, int width, int height);
void app_window_activate(app_window_t *wnd);
void app_window_close(app_window_t *wnd);

// 窗口的属性操作
void app_window_set_title(app_window_t *wnd, const wchar_t *title);
void app_window_move(app_window_t *wnd, int x, int y);
void app_window_resize(app_window_t *wnd, int width, int width);
void app_window_get_rect(app_window_t *wnd, app_window_rect_t *rect);
void app_window_set_rect(app_window_t *wnd, app_window_rect_t *rect);

// 窗口的绘制操作
void app_window_begin_paint(app_window_t *wnd, app_window_paint_t **paint,
                            app_window_rect_t *rect);
void app_window_end_paint(app_window_t *wnd, app_window_paint_t **paint);
```

## 更新记录

### v0.1.0

采用赋值的形式注册事件回调函数。

```c
int main(void)
{
    app_t *app;

    app_new(&app);
    app->on_pointer_move = on_pointer_move;
    app->on_window_closed = on_window_closed;
    app->on_key_down = on_key_down;
    app_run(app);
    app_free(&app);
}
```

**存在的问题：**

- libui 接入难度大

    这种事件绑定方式限制了一个事件只能绑定一个处理器，只适合让用户代码处理事件，再接入其它库来处理事件较为困难。

### v0.2.0

改为提供专用的函数来绑定事件。

```c
int main(void)
{
    app_t *app;
    app_event_t e;

    ui_init(&app);
    app_new(&app);
    while (app->running) {
        while (app_get_event(app, &e)) {
            ui_process_event(&e);
            switch (e.type) {
            case APP_EVENT_WINDOW_CLSOED:
                // ...
                break;
            default: break;
            }
            app_process_event(app, &e);
            // do somethings
        }
    }
    app_free(&app);
}
```

**问题：**

- 写法与 Win32 消息循环相似，没有特色

  上述示例代码相当于换了种命名风格的 Win32 消息循环处理代码，例如：
  - `app_get_event()` -> `GetMessage()`
  - `app_process_event()` -> `DefWindowProc()`

- 创建和传递 app 实例对象是多余的

  应用程序创建多个 app 对象没有意义，app 应该作为全局存在的单例对象。

## v.0.3.0

```c
int main()
{
    app_event_t e;

    app_init();
    while (app->running) {
        if (app_get_event(&e)) {
            ui_process_event(e);
            switch (e.type) {
            case APP_EVENT_WINDOW_CLSOED:
                // ...
                break;
            default: break;
            }
            app_process_event(e);
        }
    }
    app_free();
}
```
