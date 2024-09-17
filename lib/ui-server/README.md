# UI::Server

UI 服务器，提供了将 widget 与系统窗口绑定的能力。它基于 UIMutationObserver 接口监听 widget 的变化，能将 widget 的图形内容、位置和尺寸等数据同步到与之绑定的系统窗口。

## 示例

```c
#include <LCUI/ui.h>
#include <LCUI/ptk.h>

int main(int argc, char *argv)
{
    ui_widget_t *w;
	ptk_window_t *wnd;

    ptk_init(L"My Application");
    ui_init();
    // 初始化服务器
    ui_server_init();
    // 创建一个 widget
    w = ui_widget_create(NULL);
    // 创建一个窗口
    wnd = ptk_window_create("My Widget", 0, 0, 0, 0, NULL);
    // 调整 widget 尺寸
    widget_resize(800, 600);
    // 将 widget 与窗口建立连接
    ui_server_connect(w, wnd);
    // 运行主循环
    app_run();
    // 解除 widget 与窗口之间的连接
    ui_server_disconnect(w, wnd);
    ui_server_destroy();
	ui_destroy();
    return 0;
}
```
