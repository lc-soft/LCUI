# UI

图形界面核心库。

## API

### UI::ImageLoader

异步图像加载器。图像会在另一个线程中加载，然后通过事件队列告知图像加载完成。

```c
#include <LCUI/util.h>
#include <LCUI/graph.h>
#include <LCUI/ui.h>

// 在图像加载完后调用的回调函数
void on_image_event(ui_image_t *image, void *arg)
{
    // ui_image_t 数据接口是基于 pd_canvas_t 派生的，所以可以强制转换成 pd_canvas_t
    pd_canvas_t *data = (pd_canvas_t*)image;
    // 输出图像尺寸，如果为 0x0 则表明图像加载失败
    printf("image loaded, size: (%d, %d)\n", data->width, data->height);
}

int main(int argc, char *argv[])
{
    ui_image_t *image;

    // 初始化图像加载器
    ui_init_image_loader();
    // 添加图像加载任务
    image = ui_load_image("/path/to/image.png");
    // 为该图片添加事件处理函数
    ui_image_on_event(image, on_image_event, NULL);
    // 增加该图片的引用计数，避免它被自动释放
    ui_image_add_ref(image);
    // 等待一段时间让图像加载完
    for (i = 0; i < 5; i++) {
        ui_process_image_events();
        sleep_s(1);
    }
    // 减少该图片的引用计数
    ui_image_remove_ref(image);
    ui_destroy_image_loader();
    return 0;
}
```

### UI::MutationObserver

提供监听 Widget 的属性、位置、尺寸、子树等变化的能力。

设计参考自：[MutationObserver - Web API 接口参考 | MDN](https://developer.mozilla.org/zh-CN/docs/Web/API/MutationObserver)

```c
void on_widget_mutation(ui_mutation_list_t *mutation_list,
					    ui_mutation_observer_t *observer,
					    void *arg)
{

	list_node_t *node;
	ui_mutation_record_t *mutation;

	for (list_each(node, mutation_list)) {
		mutation = node->data;
		switch (mutation->type) {
        case UI_MUTATION_RECORD_TYPE_PROPERTIES:
            printf("property: %ls\n", mutation->property_name);
            break;
        default:
            break;
        }
	}
}

void example(void)
{
    ui_widget_t *widget;
    ui_mutation_observer_t *observer;
	ui_mutation_observer_init_t options = { 0 };

    ...

    // 创建一个观察器
    observer = ui_mutation_observer_create(on_widget_mutation, NULL);

    ...

    // 监听属性变化
	options.properties = true;
    // 监听子组件的变化
    options.child_list = true;
    // 监听子树的变化
    options.subtree = true;
    // 监听组件
    ui_mutation_observer_observe(observer, widget, options);

    ...

    // 销毁观察器
    ui_mutation_observer_destroy(observer);
}
```
