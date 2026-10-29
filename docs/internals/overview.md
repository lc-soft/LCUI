# LCUI 总体架构与平台层

> 描述事件循环、窗口生命周期、PTK 事件、widget 与窗口绑定、尺寸处理等代码机制。工作流与提交规范见项目根 `AGENTS.md`。

## 事件循环

LCUI 的事件循环基于 PTK（平台抽象层）：

```
平台消息泵（ptk_process_native_events）
  ├─ ptk_tick()           ← 分发 PTK_EVENT_TICK
  ├─ 平台消息处理          ← Win32: PeekMessage / DispatchMessage
  │   └─ ptk_window_process()  ← 窗口过程，构造 PTK 事件
  │       └─ ptk_process_event()  ← 遍历 listeners，同步调用 handler
  └─ ptk_process_events() ← 清空内部事件队列

lcui_app_dispatch (通过 ptk_set_event_dispatcher 注册)
  ├─ lcui_dispatch_ui_event()  ← PTK 事件转 UI 事件
  ├─ lcui_worker_run()
  ├─ ptk_steptimer_tick()     ← 驱动动画帧回调
  ├─ lcui_ui_update()         ← ui_server_update + 图片清理
  └─ lcui_app_render_frame()  ← ui_server_render + ptk_app_present
```

每帧只渲染一次，受 `min_render_interval`（1000/fps_cap）节流。

## 窗口生命周期：关闭流程

### Win32 平台

```
ptk_window_close(wnd)
  → SendMessage(WM_CLOSE)
    → ptk_window_process case WM_CLOSE:
        1. 构造 PTK_EVENT_CLOSE
        2. ptk_process_event()  ← 同步分发，窗口此时仍有效
        3. return DefWindowProc()
            → WM_DESTROY
                → ptk_window_process case WM_DESTROY:
                    list_unlink, 释放 GDI 资源, free(wnd)
                    return 0 (不产生 PTK 事件)
```

**关键规则**：
- 在 `PTK_EVENT_CLOSE` handler 中访问窗口属性（x/y/width/height）是**安全的**
- `WM_DESTROY` 处理中 `free(wnd)`，此后对 `ptk_window_t` 的访问无效

### X11 平台

```
ptk_x11window__close()
  → XSendEvent(WM_DELETE_WINDOW ClientMessage)
    → 事件循环 case ClientMessage:
        1. ptk_process_event(PTK_EVENT_CLOSE)
        2. ptk_x11window__on_destroy()
            → ptk_x11window__destroy() (XDestroyWindow)
            → 释放 XImage, GC, free(wnd)
```

### Framebuffer 平台

```
ptk_fb_window_close()
  → ptk_post_event(PTK_EVENT_CLOSE)  ← 异步投递
  → 无实际窗口销毁逻辑（全屏单窗口模式）
```

### ui_server 的 CLOSE 处理

`ui_server_on_window_close` 监听 `PTK_EVENT_CLOSE`，负责销毁 `ui_connection_t`（释放 updater、flash_rects），但**不调用** `ptk_window_destroy()`——窗口销毁由平台标准流程负责。

## PTK 事件监听器

### 注册顺序与执行顺序

`ptk_on_event` → `ptk_add_event_listener` → `list_append`（链表尾部添加）。

`ptk_process_event` 按注册顺序遍历 listeners。**必须用 safe iteration**（提前保存 `next = node->next`），因为 handler 可能在执行中调用 `ptk_off_event` 移除自己：

```c
for (node = list_get_first_node(&listeners); node; node = next) {
    next = node->next;
    listener = node->data;
    if (listener->type == e->type)
        listener->handler(e, listener->data);
}
```

### handler 执行顺序示例（PTK_EVENT_CLOSE）

按注册时机排序（先注册先执行）：
1. `ui_server_on_window_close` — 销毁 connection，释放 ui_connection_t
2. `lcui_ui_on_window_destroy` — 从 windows 链表移除，全部关闭则退出
3. `lcui_settings_on_window_close` — 同步窗口状态到 settings

## Widget 与窗口的绑定

### ui_connection 机制

`ui_server_connect(widget, window)` 创建 `ui_connection_t`，建立 widget ↔ window 的双向映射：

```c
conn->widget = widget;
conn->window = window;
conn->updater = ui_updater_create();
ui_widget_on(widget, "destroy", ui_server_on_destroy_widget, NULL);
```

- `ui_server_get_widget(window)` — 通过 window 查找绑定的 widget
- `ui_server_get_window(widget)` — 通过 widget 查找绑定的 window
- `ui_server_disconnect(widget, NULL)` — 调用 `ptk_window_close` 触发关闭流程

### 三种显示模式

`lcui_ui_set_display(mode)` 控制 widget 树的窗口绑定方式：

| 模式 | 行为 |
|------|------|
| `LCUI_DISPLAY_WINDOWED` | 创建单窗口，`ui_root()` 绑定到它 |
| `LCUI_DISPLAY_FULLSCREEN` | 同上，窗口设为全屏 |
| `LCUI_DISPLAY_SEAMLESS` | 通过 MutationObserver 监听 root 子节点变化，每个顶级子 widget 自动创建独立窗口 |

### 窗口关闭后的生命周期处理

`lcui_ui_on_window_destroy` 监听 `PTK_EVENT_CLOSE`：
- 从 `lcui_ui.windows` 链表移除关闭的窗口
- 如果 `quit_after_all_windows_closed` 为 true 且链表为空，调用 `ptk_app_exit(0)`

`lcui_ui_destroy` 末尾必须重置 `lcui_ui.mode = LCUI_DISPLAY_DEFAULT`，否则重复 `lcui_init()/lcui_destroy()` 循环后，`lcui_ui_set_display` 认为模式未变而跳过窗口创建，导致 `ui_root()` 无法绑定到窗口。

## 尺寸处理

### 客户区 vs 窗口尺寸

`ptk_window_set_size(width, height)` 接收的是**客户区尺寸**（不含边框/标题栏），内部通过 `convert_client_size_to_window_size` 转换为 `SetWindowPos` 需要的窗口尺寸。

`ptk_window_get_width/height` 返回 `wnd->width/height`，由 `WM_SIZE` 回调更新。

### Widget 尺寸查询

不要在测试中使用 `ui_widget_compute_size`（已废弃）。正确做法：

```c
ui_root_append(widget);
ui_update();
(int)widget->border_box.width
(int)widget->border_box.height
```

`ui_update()` 内部完成样式计算、更新、重排，直接读 `border_box` 即可。

### 绑定窗口时设置尺寸

`lcui_bind_window_state` 中有 `ui_widget_resize(widget, state.width, state.height)`，需要显式 `(float)` 转换：

```c
ui_widget_resize(widget, (float)state.width, (float)state.height);
```

## 测试中的消息泵

`helper_process_window_events(rounds)` 是测试专用 helper，循环调用 `ptk_process_native_events(PTK_PROCESS_EVENTS_ALL_IF_PRESENT)` + `sleep_ms(10)`。

Win32 窗口操作是异步的，`SetWindowPos` 等操作异步投递 `WM_SIZE` 等消息，单次 pump 只拿到队列中已有的消息。循环多次确保异步操作的结果返回到队列。

- 8 轮（~80ms）用于暖机
- 12 轮（~120ms）留给尺寸/位置真正生效


## 剪贴板与计时器

### 剪贴板

`ptk_clipboard_set_text(const wchar_t *text, size_t len)`（`lib/ptk/include/ptk/clipboard.h`）写入系统剪贴板。UTF-8 字符串需先用 `decode_utf8(wbuf, src, len+1)`（`lib/yutil/include/yutil/charset.h`）转 wchar_t。

### 计时器

`ptk_set_timeout(long ms, ptk_timer_cb cb, void *arg)` 返回 timer id，`ptk_clear_timeout(id)` 取消。
回调签名：`void cb(void *arg)`（`lib/ptk/include/ptk/events.h`）。

典型用法：UI 反馈（如"已复制！"按钮）在操作成功后设置 1.5s 后回滚 label；连续触发时先 `clear_timeout` 再重新启动，保证反馈始终对应最新操作。


