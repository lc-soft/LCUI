# UI 服务器

- 开始日期：2023-04-09
- 目标主要版本：3.x
- 参考问题：无
- 实现 PR：[#272](https://github.com/lc-soft/LCUI/pull/272)

## 概括

添加 UI 服务器，用于实现 UI 部件与系统窗口的数据同步。它基于 UIMutationObserver 接口监听部件的变动，能将部件的图形内容、位置和尺寸等数据同步到与之绑定的系统窗口。

## 基本示例

```c

#include <ui.h>
#include <platform.h>

void example(void)
{
    ui_widget_t *w;
	app_window_t *wnd;

    app_init(L"My Application");
    ui_init();
    // 初始化服务器
    ui_server_init();
    // 创建一个部件
    w = ui_widget_create(NULL);
    // 创建一个窗口
    wnd = app_window_create("My Widget", 0, 0, 0, 0, NULL);
    // 调整部件尺寸
    ui_widget_resize(800, 600);
    // 连接部件和窗口
    ui_server_connect(w, wnd);
    // 运行主循环
    app_run();
    // 解除部件与窗口的连接
    ui_server_disconnect(w, wnd);
    ui_server_destroy();
	ui_destroy();
    return 0;
}
```

## 动机

在 2.x 版本中，显示控制器实现了 UI 部件与窗口的绑定。它会根据显示模式自动处理根部件和子部件的窗口绑定。然而，由于绑定操作接口并未公开，且默认绑定对象是根部件和子部件，我们无法将其它部件与窗口绑定，导致其灵活性较差。

## 详细设计

### 命名

模块的候选名称有：

- `ui-mapper`：widget 与 window 之间是映射关系，那么管理这些映射关系的功能模块就是 mapper，但该名称不足以联想到它的作用，容易被理解为其它功能模块。
- `ui-player`：如果将“widget 内容同步至 window 内”这一过程称为“播放”的话，那么可以将该功能模块命名为 player，但这名字更容易让其他人理解为媒体播放器组件。
- `ui-display`：原模块的名称就是 display，既是动词也是名词，在新的命名风格中可能会产生歧义，例如：`ui_display_map`，该理解为“将 map 显示出来”，还是“调用显示模块的 map 方法”？
- `ui-server`：从命名可知这个模块是为 UI 提供服务的，可以联想到 [X Server](https://www.x.org/wiki/XServer/)。

综合考虑，ui-server 是最为合适的。

### 工作原理

内部维护一个列表用于存储各个部件与窗口的连接， UI 部件到系统窗口的同步基于 UIMutationObserver 实现，而窗口到 UI 部件的同步则通过响应窗口事件来实现。

### 接口设计

出于灵活性上的考虑，不在 ui-server 里提供 2.x 版本中的单窗口模式、全屏模式和无缝模式的切换功能，改为在 lcui 应用层提供，也就是在 `src/lcui_ui.c` 中提供。

ui-server 提供的接口比较少，除去初始化和销毁接口，可分为这几类：

- 连接管理：
    ```c
    ui_widget_t *ui_server_get_widget(app_window_t *window);
    app_window_t *ui_server_get_window(ui_widget_t *widget);
    int ui_server_disconnect(ui_widget_t *widget, app_window_t *window);
    void ui_server_connect(ui_widget_t *widget, app_window_t *window);
    ```
- 渲染：
    ```c
    size_t ui_server_render(void);
    void ui_server_present(void);
    ```
- 设置：
    ```c
    void ui_server_set_threads(int threads);
    void ui_server_set_paint_flashing_enabled(LCUI_BOOL enabled);
    ```

## 缺点

无。

## 备选方案

无。

## 采用策略

这是一个破坏性改动，需要将 2.x 版本中的显示控制模块改为基于 ui-server 实现。
