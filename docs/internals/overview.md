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
2. `lcui_window_on_ptk_close` — 按 `opt.id` 落盘窗口状态，标记窗口 closed，清空 ptk 指针，调用 `lcui_window_on_close` 回调；若 `lcui_set_quit_on_last_window(true)`（默认）且已无打开窗口，调用 `lcui_quit()`

## 显式窗口

`lcui_init()` 只初始化子系统，不创建窗口。应用通过 `<LCUI/window.h>` 的薄外观管理窗口：

```c
lcui_window_options_t opt = { 0 };
lcui_window_t *win;

opt.id = "main";        /* 窗口状态读写使用的唯一标识 */
opt.width = 1280;       /* 无保存状态时的默认内容尺寸 */
opt.height = 720;
win = lcui_window_create_for_widget(ui_root(), &opt);
lcui_load_settings("com.example.app");
lcui_window_show(win);
```

- `lcui_window_create` 只建窗，初始**隐藏**；`opt.id` 在配置已加载时立即恢复几何。
- `lcui_window_create_for_widget` 从内容部件继承标题，并在未给出宽高时继承 `border_box` 尺寸。
- `lcui_window_show` 只置 `show_requested`；`lcui_ui_render` 渲染首帧后调用 `lcui_window_flush_show`，等该窗口的 ui-server 连接初始化完成才真正显示，因此窗口首次可见即为最终几何。
- `lcui_window_bind_widget` 可重绑定或解绑内容（解绑走 `ui_server_detach_widget`，不关闭平台窗口），绑定同时为内容注册 `ui_set_event_scope`。一个 widget 只能属于一个窗口，绑定到新窗口时会自动从旧窗口解绑；内容 widget 销毁时窗口自动解绑，但不会关闭窗口。
- 全屏是窗口属性：`opt.fullscreen` 或 `lcui_window_set_fullscreen`；不再有 `lcui_display_t`。多窗口由应用自行创建多个 `lcui_window_t`。

### 配置加载顺序

`lcui_init()` 不再自动读配置。允许两种顺序：

1. 建窗（`opt.id`）→ `lcui_load_settings(app_id)` → `show`：`lcui_load_settings` 会把已存在但尚未应用状态的窗口补上；
2. `lcui_load_settings(app_id)` → 建窗：创建时直接应用状态。

### 窗口关闭后的生命周期处理

`lcui_window_t` 维护自己的窗口链表和退出策略。最后一个窗口关闭时是否退出由
`lcui_set_quit_on_last_window(bool)` 控制（默认 true）。应用需要“关掉窗口但继续运行”
（例如托盘应用）时关闭该策略。`lcui_destroy()` 会兜底销毁仍存活的窗口。

## 窗口图标

窗口图标分两层：LCUI 负责任何图像的解码与多尺寸生成，PTK 只接收
`ptk_icon_image_t`（`0xAARRGGBB` 像素）并交给平台。

- 创建时用 `lcui_window_options_t.icon` 指定图标文件；`NULL` 使用全局默认图标。
- `lcui_window_set_icon(w, file)` / `lcui_window_set_icon_from_data(w, data, size)`
  设置自定义图标；传入 `NULL` / 空数据会清除自定义图标并回退到默认图标。
- `lcui_set_default_window_icon(file)` / `_from_data` 设置全局默认图标；
  `lcui_set_default_window_icon(NULL)` 清除覆盖并回退到内置图标。
- `lcui_set_default_window_icon_enabled(false)` 关闭所有默认图标；只影响没有自定义
  图标的窗口，重新启用时会给这些窗口补上图标。
- 内置默认图标使用 `resources/lcui-icon-{16,24,32,48,64,128}.png` 和
  `resources/lcui-icon.png`（256），经 `npm run build-icon` 生成
  `src/resources/lcui-icon-*.png.inc` 并编进 `lcui`。改图标后必须重新生成包含文件。
- `src/lcui_icon.c` 的内置图标直接解码预渲染尺寸，避免 16/32px 经过运行时缩放；
  自定义图标则从单张源图生成 16/24/32/48/64/128/256 多尺寸，再调用
  `ptk_window_set_icons`。自定义图标在设置完成后即可释放临时 canvas。
- PTK 平台实现：Win32 用 `CreateIconIndirect` 创建 HICON 并 `WM_SETICON`
  （`ICON_SMALL`/`ICON_BIG`）；X11 写 `_NET_WM_ICON`；framebuffer 后端 no-op。
- 可执行文件图标（Explorer/开始菜单）仍由 Windows `.rc` 资源决定，不属于运行时
  窗口图标 API 的范围。仓库提供 `resources/lcui-icon.ico`（外部提供的 16/32/48/256
  多尺寸 ICO）供 `.rc` 引用；它不参与运行时窗口图标。

## 多窗口事件路由

`lib/ui` 的事件状态按 `ui_event_scope_t` 隔离，每个 scope 拥有自己的
`mouse_capturer`、`touch_capturers`、`hover/active/focus` 目标和点击记录：

- `ui_set_event_scope(root)` 把 `root` 子树注册为独立作用域；`lcui_window_bind_widget` 自动调用。
- `ui_dispatch_event_to(root, e)` 以 `root` 作为命中根和事件作用域。
- `ui_get_focus_in(w)` / `ui_get_mouseover_in(w)` 返回 `w` 所属作用域的状态；部件代码（dialog、popover、dropdown、tooltip、input-group、ui-server 光标同步）一律使用它们，不再提供全局焦点/悬停查询，避免多窗口下读错作用域。
- 没有显式 scope 时回落到绑定 `ui_root()` 的 default scope，旧代码行为不变。

`src/lcui_app.c` 按 PTK 事件里的窗口查找内容部件：鼠标/触摸/滚轮直接使用事件窗口；
键盘/文本输入使用静态 `active_window`（由 `PTK_EVENT_FOCUS` 更新，`UNFOCUS`/`CLOSE` 清除），
再经 `ui_dispatch_event_to` 送到对应窗口的作用域。`lib/ui` 本身不引入“活动窗口”概念。

## 尺寸处理

### 客户区 vs 窗口尺寸

`ptk_window_set_size(width, height)` 接收的是**客户区尺寸**（不含边框/标题栏），内部通过 `convert_client_size_to_window_size` 转换为 `SetWindowPos` 需要的窗口尺寸。

`ptk_window_get_width/height` 返回 `wnd->width/height`，由 `WM_SIZE` 回调更新。

### 内容尺寸同步方向

尺寸同步是**单向**的：窗口 → 内容部件。

- 窗口尺寸变化时 `ui_server_on_window_resize` 调用 `ui_widget_resize(widget, size / scale)`。
- 连接初始化时 `ui_server_refresh_window` 用当前客户区尺寸设置内容部件。
- 修改内容部件尺寸**不会**反推窗口；需要改窗口请调用 `lcui_window_set_size`。

### Widget 尺寸查询

不要在测试中使用 `ui_widget_compute_size`（已废弃）。正确做法：

```c
ui_root_append(widget);
ui_update();
(int)widget->border_box.width
(int)widget->border_box.height
```

`ui_update()` 内部完成样式计算、更新、重排，直接读 `border_box` 即可。

## 图片加载

- `lcui_init()` 先完成 `ui_init()`（创建图片缓存、图片列表和互斥锁）再启动 `lcui_ui_image_loader_thread`；该线程启动后立即调用 `ui_load_images()`，顺序不能反。
- `lib/ui/src/ui_image.c` 用 `ui_image_loader.mutex` 序列化加载线程与主线程对缓存、列表和图片状态的访问；`ui_load_images()` 解码期间持锁。
- 主线程仍可显式调用 `ui_load_images()`（测试常用），会与加载线程串行执行。

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


