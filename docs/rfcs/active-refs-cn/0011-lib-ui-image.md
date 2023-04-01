# UI 图像缓存和异步加载

- 开始日期：2023-04-05
- 目标主要版本：3.x
- 参考问题：无
- 实现 PR：无

## 概括

新增 UIImage 模块来负责异步图像加载。

## 基本示例

```c
#include <stdio.h>
#include <ui/image.h>

void on_image_load(ui_image_t *image, void *data)
{
        pd_canvas_t *data = (pd_canvas_t *)image;

        if (data) {
                printf("image loaded, size: (%d, %d)\n", data->width, data->height);
        } else {
                printf("image loading failed!\n");
        }
}

void example(void)
{
        ui_image_t *image;

        image = ui_image_create("/path/to/image.png");
        ui_image_on_event(image, on_image_load, NULL);
}
```

## 动机

在 2.x 版本中，部件背景图加载功能的设计是：建立一个以图像路径为索引的缓存表，每个缓存项中记录了引用该图像的部件列表。在计算部件背景样式时，如果指定了图像路径，则会加载它并增加该图像的引用计数。在部件的背景图样式变更以及部件销毁时，会减少图像的引用计数。当图像引用计数为 0 时，释放图像资源。

这个设计中的图像缓存依赖于部件，而图像异步加载能力则依赖于 LCUI 的工作线程。整个功能仅供内部使用。实际上，这两个依赖项并不是必要的。其中对 LCUI 的工作线程的依赖可以通过将图像加载函数改为公共函数来解除，由上层代码决定放在哪个线程上执行。

按照 3.x 版本的架构设计思想，它可以被设计成较为独立通用的模块，以便于 LCUI 应用层代码也能使用异步图像加载功能。

## 详细设计

设计灵感来自：https://developer.mozilla.org/zh-CN/docs/Web/API/HTMLImageElement/Image

新增 `ui_image_t` 类型，基于 `pd_canvas_t` 扩展增加其它成员，在使用时可将 `ui_image_t` 指针转为 `pd_canvas_t` 指针来访问图像的信息。

新增事件绑定函数，用于监听图像的加载结果，回调函数的指针定义如下：

```c
typedef void (*ui_image_event_handler_t)(ui_image_t *, void *);
```

如果图像加载失败，回调函数收到的第一个参数的值会为 NULL。

模块内部维护缓存表、待加载的图像列表、事件列表。

`ui_image_create()` 函数用于创建图像对象，当指定的图像路径已在缓存表中时则直接返回缓存中的图像，并给该缓存增加引用计数。否则，将图像对象追加到待加载的图像列表中。

`ui_image_add_event_listener()` 和 `ui_image_remove_event_listener()` 用于图像事件的绑定与解绑，提供简写宏 `ui_image_on_event` 和 `ui_image_off_event`。

`ui_load_images()` 用于处理待加载的图像列表，该函数应该在 UI 线程外的其它线程中调用。

`ui_process_image_events()` 用于处理已加载的图像的事件，与图像绑定的事件处理函数都会在这时被调用。

## 缺点

图像加载失败时，事件处理函数接收的 `ui_image_t` 指针值为 NULL，无法得知图像信息，可考虑将错误信息写入 `ui_image_t` 结构体内。

## 备选方案

无。

## 采用策略

这是个新增的功能，需要将部件的背景图片加载逻辑改为基于新的 UIImage。
