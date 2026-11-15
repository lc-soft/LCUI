# 新版窗口接口与多窗口事件路由设计

- 日期：2026-10-16
- 状态：已实现
- 范围：`lib/ptk`（win32/x11/fb）、`lib/ui`（事件系统）、`lib/ui-server`、`src/lcui_window.c`（新）、`src/lcui_ui.c`、`src/lcui_app.c`、`src/lcui_window_state.c`、`include/LCUI/window.h`（新）、tests
- 取代：`2026-10-13-window-restore-lifecycle-design.md`（其根因 A/B/C 分析继续有效，但“内部显示门控”方案被本设计的显式窗口 API 取代）

## 1. 背景与目标

`lcui_init()` 目前隐式创建主窗口、绑定 `ui_root()`、并决定显示模式（窗口化/全屏/无缝），导致窗口状态恢复的时机天然错乱（见旧 spec 根因 A/B/C）。本设计把窗口提升为**一等概念、由应用显式管理**：

- `lcui_init()` 只做核心初始化：**不建窗、不设显示模式**。
- 提供一组**薄外观** `lcui_window_*`，语义为「创建（隐藏）→ 绑定 widget → 绑定状态 → 显示」，内部转发到现有 `ptk_*` / `ui_server_*` / 窗口状态模块。
- 支持真多窗口：鼠标/触摸/键盘/文本输入/IME 按窗口路由（P2）。

成功标准：

1. 默认不再自动建窗；应用可用 `lcui_window_*` 显式控制窗口生命周期。
2. 创建时（`opt.id`）在显示前应用保存几何，窗口首次可见即为最终几何，无跳变、无默认几何闪现。
3. 多个窗口各自拥有独立的 hover/active/focus/capturer，事件互不串扰。
4. 旧写法（自动主窗）通过 `lcui_window_create_for_widget(ui_root(), …)` 继续；显示模式枚举（`lcui_display_t`：WINDOWED/FULLSCREEN/SEAMLESS）移除——全屏成为窗口属性，无缝由应用手动创建多个窗口实现。
5. 有覆盖上述行为的自动化测试。

## 2. 范围决策（已确认）

| 决策点 | 结论 |
|---|---|
| `lcui_init()` | **核心化**：移除默认建窗与显示模式设置 |
| 窗口 API 形式 | **薄外观 `lcui_window_*`**（约 6~10 个函数），内部转发到 `ptk_*`/`ui_server_*`，原语保留为逃生口 |
| 内容 API 命名 | `lcui_window_bind_widget()` / `lcui_window_get_widget()`（不转移所有权） |
| 状态 API | `opt.id`（创建时绑定状态）；**不提供**独立的 `bind_state` |
| widget↔window | 一个 widget 同一时刻只绑定一个窗口；重复绑定自动解除旧的 |
| 初始化 | `lcui_init()` **不再自动加载配置**；新增 `lcui_load_settings(app_id)`（设 id + 加载），在 init 之后调用 |
| 退出策略 | `lcui_set_quit_on_last_window(bool)`（默认 true，符合 Qt/wx 惯例） |
| 宽高同步 | **单向：窗口 → 内容部件**；不做内容→窗口（无 `size_to_content`） |
| 窗口标识 | `opt.id` 合并“窗口标识”与“状态持久化键”（原 `state_id`） |
| 交付范围 | **P1 + P2 一次到位**（含多窗口事件路由） |
| 旧自动主窗 | `lcui_window_create_for_widget(ui_root(), {.id=...})` 便利函数 |
| 显示模式 | **移除** `lcui_display_t`/`lcui_ui_set_display`：全屏=窗口属性（`fullscreen`）；SEAMLESS 不迁移，应用手动多窗口 |
| ptk 修复 | 保留旧 spec 的根因 A（位置语义）与 B（`set_maximized` 不隐式显示） |

## 3. 架构总览

```
应用层
  lcui_init() / lcui_destroy()        核心子系统：app/events、widgets、rendering、settings
  lcui_window_*()                    窗口外观：创建/绑定 widget/绑定状态/显示/几何/生命周期
  lcui_run()                         循环：泵事件 + 按窗口路由 + 更新/渲染 + 退出策略

内部
  src/lcui_window.c                  lcui_window_t 实现（转发 ptk + ui_server + window_state）
  lib/ui-server                      widget↔window 连接、渲染、mutation→窗口属性
  lib/ui (ui_events)                 事件作用域（per-window），取代全局单例
  lib/ptk                            平台窗口、位置/显示/最大化/焦点
  src/lcui_window_state.c            窗口状态持久化（窗口维度）
```

## 4. P1：窗口外观与核心化

### 4.1 `lcui_init()` 核心化

- 移除 `lcui_ui_init()` 内的 `lcui_ui_set_display(LCUI_DISPLAY_DEFAULT)`。
- `lcui_init()` 依次初始化 app/events、fonts、widgets、rendering、window_state，**不再调用 `lcui_settings_load()`**。
- **移除显示模式**：删除 `lcui_display_t` 与 `lcui_ui_set_display()`，连同 `lcui_ui` 内的 `windows` 列表、`quit_after_all_windows_closed`、以及 SEAMLESS 的 mutation observer。窗口与退出策略下沉到 `lcui_window.c`（见 §4.6）。

### 4.1.1 配置加载显式化（`lcui_load_settings`）

```c
void lcui_init(void);                                /* 子系统，不加载配置 */
bool lcui_load_settings(const char *app_id);         /* 设 id + 加载配置 */
bool lcui_settings_load(void);                       /* 已有：仅加载（使用当前 app_id） */
```

- 推荐顺序：`lcui_init()` → 建窗（带 `opt.id`，隐藏）→ `lcui_load_settings(app_id)` → `lcui_window_show()`；两种顺序都支持（见 §4.3）。
- `lcui_load_settings(app_id)` 等价于 `lcui_set_app_id(app_id)` + `lcui_settings_load()`；`app_id == NULL` 时使用默认 `"lcui"`。
- `lcui_set_app_id()` 保留，供需要单独设置 id 的场景；它只需早于 `lcui_load_settings()`。
- 配置加载必须发生在 `lcui_rendering_init()` / `lcui_window_state_init()` 注册反序列化回调之后，因此只能在 `lcui_init()` 之后调用（这也正是它没有被并入 `lcui_init` 的原因）。
- 与主流一致：Qt 的 `QSettings`、GTK/Electron/Tauri 的配置都由应用显式加载。
- 代价：忘记调用 `lcui_load_settings()` 时，渲染设置停留在默认值、窗口状态不持久化；文档中明确。

### 4.2 `lcui_window_t` 对象

不透明句柄，内部持有 `ptk_window_t *`、内容 `ui_widget_t *`、状态 id、以及“期望可见/是否需要延迟显示”等标志。生命周期与连接由 `ui_server` 维护（沿用现有 widget↔window 映射）。

### 4.3 API 与语义

```c
/* include/LCUI/window.h（聚合进 LCUI.h） */
typedef struct lcui_window lcui_window_t;

typedef struct lcui_window_options {
        const wchar_t *title;    /* NULL：create_for_widget 时继承 widget->title */
        const char   *id;        /* 稳定窗口标识 + 状态持久化键；NULL = 匿名/不持久化 */
        int width, height;       /* 逻辑尺寸(dp)；0：create_for_widget 时取部件布局尺寸 */
        int x, y;                /* 客户区原点；LCUI_WINDOW_POS_AUTO = 居中 */
        bool maximized;
        bool fullscreen;
} lcui_window_options_t;

lcui_window_t *lcui_window_create(const lcui_window_options_t *opt); /* 隐藏创建 */
void           lcui_window_destroy(lcui_window_t *w);               /* 强制销毁 */
void           lcui_window_close(lcui_window_t *w);                 /* 走关闭流程（先落盘） */

bool           lcui_window_bind_widget(lcui_window_t *w, ui_widget_t *widget);
ui_widget_t   *lcui_window_get_widget(lcui_window_t *w);

void           lcui_window_show(lcui_window_t *w);  /* 请求可见；首帧后真正显示 */
void           lcui_window_hide(lcui_window_t *w);
void           lcui_window_set_size(lcui_window_t *w, int cw, int ch);
void           lcui_window_get_size(lcui_window_t *w, int *cw, int *ch);
void           lcui_window_set_position(lcui_window_t *w, int x, int y);
void           lcui_window_get_position(lcui_window_t *w, int *x, int *y);
void           lcui_window_set_title(lcui_window_t *w, const wchar_t *title);
void           lcui_window_set_maximized(lcui_window_t *w, bool maximized);
void           lcui_window_set_fullscreen(lcui_window_t *w, bool fullscreen);
ptk_window_t  *lcui_window_get_handle(lcui_window_t *w);            /* 逃生口 */

typedef void (*lcui_window_close_cb_t)(lcui_window_t *w, void *arg);
void           lcui_window_on_close(lcui_window_t *w, lcui_window_close_cb_t cb,
                                    void *arg);

/* 便利：创建 + 绑定 widget（+ 状态）；标题/尺寸默认继承部件。取代 create_main */
lcui_window_t *lcui_window_create_for_widget(ui_widget_t *widget,
                                             const lcui_window_options_t *opt);

/* 退出策略 */
void lcui_set_quit_on_last_window(bool enable);
```

语义：

- **create 含状态**：`opt->id` 非空时，窗口记录状态键；创建时若配置已加载则立即应用保存的几何，未加载则等待 `lcui_load_settings()` 应用；关闭时回写。没有独立的 `bind_state` API。未提供 `id` 的窗口不持久化。
- **窗口标识 `id`**：同时作为状态持久化键（`window.<id>`）与运行时标识（唯一、可用于将来 `lcui_window_find(id)`/日志）；`NULL` 表示匿名且不持久化。
- **create_for_widget**：`opt` 可为 `NULL`（等价全默认）；`title`/`width`/`height` 为 0/NULL 时分别继承 `widget->title` 与**部件布局后的尺寸**（一次性测量作为窗口初始尺寸；有保存状态时状态优先）。只创建（隐藏）+ 绑定，**不自动显示**。
- **尺寸单位**：`lcui_window_options_t` 与 `lcui_window_set_size` 使用与 widget 一致的**逻辑尺寸（dp）**，在 ptk 边界按 scale 转物理像素；`lcui_window_get_size` 同理返回逻辑尺寸。
- **不转移所有权**：窗口不销毁绑定的 widget；widget 销毁 → 自动解绑（复用 `ui_server_on_destroy_widget`）。
- **重绑定**：`bind_widget` 会先解绑旧 widget；传 `NULL` 解绑；一个 widget 只能属于一个窗口（绑定新窗口时自动从旧窗口解绑）。
- **几何先于显示**：创建时或 `lcui_load_settings()` 时（依据 `opt->id`）在隐藏态应用保存的位置/尺寸/最大化；`show` 只置“期望可见”，实际 `ptk_window_show` 在该窗口首帧渲染完成后执行（避免未绘制的空窗闪现）。延后显示由 `lcui_ui_render()` 在 `ui_server_render()` 之后 flush。
- **关闭**：用户点 X / `lcui_window_close` → 落盘状态、触发 close 回调 → 连接与窗口拆除；`destroy` 为不带关闭流程的强制拆除。

### 4.4 窗口状态模块（窗口维度）

- `lcui_window_state.c` 的 binding 由 “widget + id” 改为 “window + id”。
- `opt.id` 非空时登记状态键；创建时若配置已加载则立即（隐藏态）应用几何，否则在 `lcui_load_settings()` 完成后对已有窗口批量应用。窗口关闭时回写。该逻辑为 `lcui_window.c` 内部实现，不对外暴露独立函数。
- 仍复用现有 `ini_doc` / settings 序列化与校验逻辑（`window.<id>` 段）。
- 旧 `lcui_bind_window_state(widget, id)` 移除；仓库内调用点迁移为 `opt.id`。

### 4.5 沿用旧 spec 的 ptk 修复（根因 A/B）

- win32 `ptk_window_set_position` 改为与 `get_position` 互逆（客户区原点）。
- `ptk_window_set_maximized` 不再隐式显示：隐藏时仅记录期望状态，`ptk_window_show` 时应用。
- 新增 `ptk_window_is_visible()` 供断言与内部门控。

### 4.6 显示模式迁移（WINDOWED / FULLSCREEN / SEAMLESS）

旧的三个显示模式全部由新窗口 API 表达，`lcui_display_t`/`lcui_ui_set_display()` 移除：

| 旧 | 新 |
|---|---|
| `LCUI_DISPLAY_WINDOWED` | `lcui_window_create_for_widget(ui_root(), opt)` + `lcui_window_show` |
| `LCUI_DISPLAY_FULLSCREEN` | 同上，`opt.fullscreen = true` 或 `lcui_window_set_fullscreen(w, true)` |
| `LCUI_DISPLAY_SEAMLESS` | **不迁移**：应用自行观察 `ui_root()` 顶级子节点，逐个 `lcui_window_create_for_widget(child, opt)` + `show`，并在移除时 `close`/`destroy` |

- WINDOWED 与 FULLSCREEN 不是两种模式，只是「窗口是否全屏」一个属性；`lcui_window_options_t.fullscreen` + `lcui_window_set_fullscreen()` 覆盖。
- SEAMLESS 是“每顶级子节点一个窗口”的策略，用 `lcui_window_*` + `ui_mutation_observer`（child_list）即可搭出，库不再内置。
- `lcui_window.c` 接管原 `lcui_ui` 的窗口职责：连接、`PTK_EVENT_CLOSE` 处理、以及 `lcui_set_quit_on_last_window` 退出策略。

### 4.7 窗口与内容部件的宽高同步（单向：窗口 → 部件）

窗口是客户区尺寸的唯一权威：

- `PTK_EVENT_SIZE`（用户拖拽 / `lcui_window_set_size`）→ `ui_widget_resize(content, 逻辑尺寸)`；逻辑尺寸 = 客户区物理尺寸 / scale。
- 连接建立时（首个 update）用窗口客户区尺寸初始化内容部件。
- 内容部件尺寸变化**不会**改变窗口；`create_for_widget` 的 `width/height = 0` 仅**一次性测量**作为初始窗口尺寸。
- 不做 内容→窗口 的自动 size-to-content；需要“窗口贴合内容”时由应用显式 `lcui_window_set_size()`。

**不做的同步**

- 位置：窗口由 `opt.x/y` / `lcui_window_set_position` 控制（SEAMLESS 已移除，无 widget→window 位置需求）。
- 标题：窗口由 `opt.title` / `lcui_window_set_title` 控制；`create_for_widget` 仅在 `title == NULL` 时一次性继承 `widget->title`。
- 可见性：窗口由 `lcui_window_show/hide` 控制（`ui_widget_hide` 不再隐式隐藏窗口）。

**保留**：内容部件的 CSS `min/max` 仍作为窗口 `min/max` 约束（`ui_server_on_window_minmaxinfo`）。

**迁移影响**：`ui_widget_resize(root, …)` 不再改变窗口大小；改用 `opt.width/height` / `lcui_window_set_size()`。

### 4.8 `lib/ui-server` 的保留职责

`lib/ui-server` 仍需存在——它是唯一同时了解 widget（`lib/ui`）与平台窗口（`lib/ptk`）的层：
`lib/ui` 刻意不依赖 PTK，`lib/ptk` 不依赖 widget，二者的桥接必须有归属。

**保留**

- 连接注册/映射：`ui_server_connect/disconnect`、`get_widget(window)`、`get_window(widget)`。
- 每连接 updater 与渲染：dirty rect 拆分、OpenMP 并行、`ptk_window_begin_paint/end_paint`、paint flashing。
- PTK 监听：`SIZE`（窗口→部件）、`MINMAXINFO`（CSS min/max 约束）、`DPICHANGED`、`PAINT`、`CLOSE`、`CREATE`。
- 光标同步（hover → 窗口系统光标）。

**删除/简化**

- widget→window 属性同步：`ui_server_on_widget_mutation` + `window_mutation_record_t` + `ui_server.observer` 整体移除。
- `ui_server_refresh_window` 的“用部件尺寸设窗口 + 居中”逻辑改为“用窗口客户区尺寸设部件”。
- `on_window_visibility_change` 不再 `ui_widget_show/hide`，只更新渲染可见标志。
- 标题/位置/可见性同步移除（由窗口层负责）。

**为什么不合并进 `lib/ui` 或 `src/`**

- 并入 `lib/ui` 会破坏“lib/ui 不依赖 PTK”的分层；并入 `src/lcui_window.c` 会丢掉可复用的库边界与公开 `ui_server.h`，收益不足。
- 结论：保留 `lib/ui-server`，但代码量明显缩小（渲染与监听仍在，属性同步那一大块删除）。

## 5. P2：多窗口事件路由

### 5.1 现状

`lib/ui/src/ui_events.c` 的事件状态是**全局单例**：`targets[HOVER/ACTIVE/FOCUS]`、`mouse_capturer`、`click` 记录、`touch_capturers`；命中测试以 `ui_root()` 为根；键盘/文本输入打向全局 focus。多窗口下会互相串扰。

### 5.2 事件作用域（per-window scope）

- 新增 `ui_event_scope_t`，承载 per-window 可变事件状态：
  `mouse_capturer`、`touch_capturers`、`targets[3]`、`click`。
- scope 以**窗口内容 widget**（窗口显示的子树的根）为键，由 `void ui_set_event_scope(ui_widget_t *root)` 显式注册（幂等）；`lcui_window_bind_widget` 自动调用。root 销毁时自动清理。
- 每个 scope 拥有独立的 `targets[FOCUS/HOVER/ACTIVE]`：`ui_set_focus(A 的后代)` 只改 A 的焦点，不影响 B。
- 全局 `ui_events` 保留与窗口无关的部分（event id/name 注册表、事件队列 `queue`、`base_event_id`）。

### 5.3 鼠标 / 触摸 / 滚轮

- `lcui_app.c` 的 `lcui_dispatch_ui_event()` 按 `ptk_event_t->window` 找到窗口内容 widget（`ui_server_get_widget(window)`），调用带作用域的派发：
  `ui_dispatch_event_to(content, &e)`。
- 命中测试以 `content` 为根（`ui_widget_get_event_target(content, x, y, ...)` / `ui_widget_at(content, x, y)`）；坐标直接用窗口客户区坐标。**实现期需验证**：窗口渲染以 `content` 原点对齐客户区原点（则客户区坐标即内容相对坐标）；若存在偏移，需按 `content->border_box` 原点做一次坐标变换。
- 保留旧 `ui_dispatch_event(e)`（作用于 `ui_root()` 的 scope，单窗口兼容）；新增 `ui_dispatch_event_to(content, e)`；`lcui_app.c` 鼠标/触摸路径改用后者。
- hover/active/click/mouse_capturer 全部落在该 scope，互不影响其它窗口；鼠标离开窗口时清理对应 scope 的 hover。

### 5.4 键盘 / 文本输入 / IME 与窗口焦点

- 键盘/文本输入/IME 打向“当前获得系统输入焦点的窗口”的 content scope。
- “活动窗口”由 `src/lcui_app.c` 维护（它知道 PTK 窗口与焦点事件）；`lib/ui` 不引入窗口/活动窗口概念，只接收 `ui_dispatch_event_to(content, e)`。

### 5.5 ptk 窗口焦点事件（新增）

- 事件：`PTK_EVENT_FOCUS` / `PTK_EVENT_UNFOCUS`（携带 `window`）。
- win32：`WM_SETFOCUS` / `WM_KILLFOCUS`。
- x11：`FocusIn` / `FocusOut`。
- fb：单窗口，始终视为聚焦。
- `src/lcui_app.c` 监听焦点变化，记录 `active_content = ui_server_get_widget(window)`；键盘/文本/IME 用 `ui_dispatch_event_to(active_content, e)` 派发。**lib/ui 不保存“活动”状态。**

### 5.6 公开访问器语义

- `ui_set_focus(w)`：沿 `w` 的祖先找到最近的已注册 scope root，只修改该 scope 的焦点（含 blur/focus 事件与 `focus` status），不影响其它 scope。
- 新增 `ui_get_focus_in(ui_widget_t *w)` / `ui_get_mouseover_in(ui_widget_t *w)`：返回 `w` 所在 scope 的 focus/hover。
- 新增 `void ui_set_event_scope(ui_widget_t *root)`：显式注册独立作用域；`ui_dispatch_event_to(root, e)` 要求 root 是已注册 scope（`lcui_window_bind_widget` 会预先注册）。
- 旧 `ui_get_focus()` / `ui_get_mouseover()`：保留一个版本，语义等价于 `..._in(ui_root())`（单窗口兼容），标记 deprecated；多窗口代码用 `_in`。
- `ui_update_cursor()`（ui-server）：按连接/内容根读取 mouseover，对鼠标实际所在窗口设置系统光标。
- `lib/ui` 不依赖 PTK、不引入“活动窗口/活动 scope”概念；“哪个窗口接收键盘”由 `src/lcui_app.c` 决定。
- 迁移：`src/widgets/dialog.c` 的 `last_focus` 保存/恢复改用 `ui_get_focus_in(dialog)`；依赖 `ui_get_focus()` 的测试同理。

## 6. 数据与状态一览

| 位置 | 新增/变更 |
|---|---|
| `include/LCUI/window.h`（新） | `lcui_window_t`、options、全部 `lcui_window_*` API |
| `src/lcui_window.c`（新） | 外观实现（ptk + ui_server + window_state 装配） |
| `struct lcui_ui` | 移除默认显示调用、`windows` 列表、退出策略与 SEAMLESS observer（下沉到 `lcui_window.c`） |
| `src/lcui_window_state.c` | binding 改为 window 维度 |
| `lib/ui` | `ui_event_scope_t` + `ui_set_event_scope(root)`；`ui_dispatch_event_to(root, e)`；`*_in(widget)`；**不引入活动状态** |
| `lib/ui-server` | 窗口→部件单向尺寸同步；移除 widget→window 的 position/title/visible 同步；渲染/光标逻辑与内容根作用域对接 |
| `lib/ptk` | `ptk_window_is_visible`、`set_maximized` 解耦、win32 位置语义、`PTK_EVENT_FOCUS/UNFOCUS` |

## 7. 示例用法（新旧对照）

> 所有示例包含 `<LCUI.h>` 与 `<LCUI/main.h>`（入口）；`main` 必须声明为
> `int main(int argc, char **argv)`（`LCUI/main.h` 在 Win32 下定义 `WinMain` 并调用它）。

### 7.1 Hello World

**旧版**（`lcui_init()` 隐式建窗并绑定 `ui_root()`）：

```c
#include <LCUI.h>
#include <LCUI/main.h>

int main(int argc, char **argv)
{
        ui_widget_t *text;

        lcui_init();                 /* 自动创建主窗口并绑定 ui_root() */
        text = ui_create_widget("text");
        ui_text_set_content(text, "Hello, LCUI!");
        ui_root_append(text);
        return lcui_main();          /* run + destroy */
}
```

**新版**（显式创建 + 显示）：

```c
#include <LCUI.h>
#include <LCUI/main.h>

int main(int argc, char **argv)
{
        ui_widget_t *text;
        lcui_window_options_t opt = { 0 };
        lcui_window_t *win;

        lcui_init();                 /* 核心初始化：不建窗 */
        text = ui_create_widget("text");
        ui_text_set_content(text, "Hello, LCUI!");
        ui_root_append(text);

        opt.title = L"Hello";
        opt.width = 800;
        opt.height = 600;
        win = lcui_window_create_for_widget(ui_root(), &opt); /* 隐藏 */
        lcui_window_show(win);       /* 首帧后可见 */
        return lcui_main();
}
```

### 7.2 手动单窗口（含状态恢复）

**旧版**（窗口由 `lcui_init()` 建好，再按 widget 绑状态）：

```c
#include <LCUI.h>
#include <LCUI/main.h>

int main(int argc, char **argv)
{
        lcui_set_app_id("dev.example.app");
        lcui_init();                 /* 窗口已创建并绑定 ui_root() */
        app_build_ui();
        ui_widget_resize(ui_root(), 1280, 720);
        lcui_bind_window_state(ui_root(), "main");
        return lcui_main();
}
```

**新版**（窗口显式创建；状态在隐藏态应用，显示时几何已最终确定）：

```c
#include <LCUI.h>
#include <LCUI/main.h>

int main(int argc, char **argv)
{
        lcui_window_options_t opt = { 0 };
        lcui_window_t *win;

        lcui_init();                        /* 子系统：不建窗、不加载配置 */
        lcui_load_settings("dev.example.app"); /* 设 id + 加载配置 */
        app_build_ui();

        opt.title = L"My App";
        opt.width = 1280;            /* 无保存状态时的默认尺寸 */
        opt.height = 720;
        opt.id = "main";             /* 有保存状态时覆盖上面的几何 */
        win = lcui_window_create_for_widget(ui_root(), &opt);
        lcui_window_show(win);
        return lcui_main();
}
```

需要显式控制顺序时，用原语等价写法（`lcui_window_create` + `lcui_window_bind_widget` + `lcui_window_show`）即可；`create_for_widget` 只是“创建 + 绑定 widget + 应用 `opt.id` 状态”的便捷封装。

### 7.3 手动多窗口

**旧版**（只有 SEAMLESS 能自动多窗，且无法逐窗设置标题/尺寸/状态）：

```c
#include <LCUI.h>
#include <LCUI/main.h>

int main(int argc, char **argv)
{
        lcui_init();
        /* 把每个窗口内容作为 ui_root() 的顶级子节点 */
        ui_root_append(build_editor());
        ui_root_append(build_preview());
        /* 库自动为每个顶级子节点建窗 */
        lcui_ui_set_display(LCUI_DISPLAY_SEAMLESS);
        return lcui_main();
}
```

**新版**（逐窗创建、逐窗标题/尺寸/状态；事件按窗口隔离）：

```c
#include <LCUI.h>
#include <LCUI/main.h>

static void on_preview_close(lcui_window_t *w, void *arg)
{
        lcui_quit();                 /* 关闭预览窗后退出整个应用 */
}

int main(int argc, char **argv)
{
        lcui_window_options_t editor_opt = { 0 };
        lcui_window_options_t preview_opt = { 0 };
        ui_widget_t *editor = build_editor();
        ui_widget_t *preview = build_preview();
        lcui_window_t *editor_win, *preview_win;

        lcui_init();
        ui_root_append(editor);
        ui_root_append(preview);

        editor_opt.title = L"Editor";
        editor_opt.width = 900;
        editor_opt.height = 700;
        editor_opt.id = "editor";
        editor_win = lcui_window_create_for_widget(editor, &editor_opt);

        preview_opt.title = L"Preview";
        preview_opt.width = 640;
        preview_opt.height = 480;
        preview_opt.id = "preview";
        preview_win = lcui_window_create_for_widget(preview, &preview_opt);

        lcui_window_on_close(preview_win, on_preview_close, NULL);
        lcui_window_show(editor_win);
        lcui_window_show(preview_win);
        return lcui_main();
}
```

补充：

- 如果关闭一个窗口后应用要继续运行（托盘、后台），调用
  `lcui_set_quit_on_last_window(false)`。
- 每个窗口拥有独立的 hover/active/focus/capturer（P2），键盘/文本输入路由到当前活动窗口。

### 7.4 新旧对比要点

| 维度 | 旧版 | 新版 |
|---|---|---|
| 建窗 | `lcui_init()` 隐式 | `lcui_window_create*()` 显式 |
| 显示 | 库自动 | `lcui_window_show()` |
| 标题/尺寸/位置 | 受 root widget / CSS 约束 | 每窗口独立设置 |
| 窗口状态 | `lcui_bind_window_state(widget, id)` | `opt.id`（创建时）+ 自动读写 |
| 多窗口 | SEAMLESS 自动（能力受限） | 显式多窗口，逐窗配置 |
| 事件路由 | 全局单例 | 按窗口隔离（P2） |
| 全屏 | `LCUI_DISPLAY_FULLSCREEN` | `opt.fullscreen` / `lcui_window_set_fullscreen()` |

## 8. 兼容与迁移

- **仓库内必迁**：`examples/doc-viewer/app/main.c`（`lcui_bind_window_state(ui_root(), "main")` → 新建窗口 + `opt.id`）、`tests/integration/test_settings.c` 及任何 `lcui_init` 后假设存在窗口的测试。
- **初始化**：`lcui_init()` 不再自动加载配置；改为 `lcui_init(); lcui_load_settings(app_id);` 之后再建窗（带 `opt.id`）。`lcui_set_app_id()` 仍保留，但只需早于 `lcui_load_settings()`。
- **删除显示模式**：`lcui_display_t`、`lcui_ui_set_display()` 移除；全屏用 `opt.fullscreen`/`lcui_window_set_fullscreen()`，SEAMLESS 由应用手动多窗口实现。
- **`lcui_window_create_for_widget(ui_root(), {.id="main"})`**：新的便利入口，等价旧“自动主窗”（建根窗口 + 绑定 `ui_root` + 应用状态）。
- **`lcui_bind_window_state(widget,id)`**：移除；改用 `opt.id`（创建时自动绑定状态）。
- **`ui_widget_resize(root, …)`**：不再驱动窗口尺寸；要改窗口尺寸用 `opt.width/height` 或 `lcui_window_set_size()`。
- **`ui_widget_set_title(root, …)`**：不再设置窗口标题；用 `opt.title` 或 `lcui_window_set_title()`。
- **`lcui_main()`**：语义不变（run + destroy）；但不再隐含建窗。

## 9. 测试

新增 `tests/integration/test_window_api.c`（注册 `tests/main.c`）：

1. `lcui_init()` 后默认**没有**窗口（`ui_server_get_window(ui_root()) == NULL`）。
2. 显式创建（`opt.id`）→ 绑定 widget → 显示：窗口最终几何/`maximized` 等于保存值；位置精确往返（根因 A）。
3. 创建时（`opt.id`）在 `show` 前应用几何；`show` 后首帧可见，且可见时几何已最终确定。
4. `bind_widget` 重绑定/解绑行为；widget 销毁后窗口自动解绑。
5. 关闭流程：状态落盘（多次运行不漂移）。
6. `lcui_window_create_for_widget(ui_root(), {.id=...})` 便利路径与旧行为一致（标题/尺寸继承、状态应用）。
7. `lcui_set_quit_on_last_window(true)`：关闭最后一个窗口后 `lcui_run` 返回。
8. **单向宽高同步**：`lcui_window_set_size()` / 平台 resize 后内容部件被调整；反过来 `ui_widget_resize(content)` **不**改变窗口尺寸。
9. **配置加载显式化**：`lcui_init()` 后 `lcui_settings_is_loaded()` 为 false；调用 `lcui_load_settings(app_id)` 后为 true，且渲染/窗口状态配置生效。

新增 `tests/integration/test_window_events.c`（P2）：

1. 两个窗口各自的 hover/focus 独立：对窗口 A 派发 mouse，A 内容 `mouseover` 生效，B 内容不受影响。
2. 键盘事件按活动窗口路由到对应 focus。
3. 窗口 A 失去焦点 → 其 focus 不再接收键盘。

回归：`xmake build && xmake test`。

## 10. 分阶段与提交拆分

- **P1**
  - `fix(ptk): window position should be the inverse of get_position`
  - `fix(ptk): setting maximized should not implicitly show the window`
  - `feat(ptk): add window visibility and focus events`
  - `feat(lcui): stop creating the default window in lcui_init`
  - `feat(lcui): make settings loading explicit (lcui_load_settings)`
  - `feat(lcui): add lcui_window api and window-based window state`
  - `refactor(ui-server): make content size follow the window (one-way)`
  - `refactor(lcui): remove display modes in favor of the window api`
  - `refactor: migrate doc-viewer and tests to explicit windows`
- **P2**
  - `refactor(ui): extract per-window event scope in ui_events`
  - `feat(lcui): route input events by window`
  - `test: cover multi-window event isolation`

## 11. 已知限制

1. `lcui_window_t` 是薄外观；平台细节仍可通过 `lcui_window_get_handle()` 逃生口访问。
2. 活动窗口依赖新增的 ptk 焦点事件；无焦点事件的后端（fb）视为始终聚焦单窗口。
3. 事件作用域以内容 widget 为键：同一 widget 不可同时作为两个窗口的内容。
4. `lcui_bind_window_state(widget,id)` 移除，改用 `opt.id`；对外属破坏性变更。
5. 显示模式枚举（`lcui_display_t`）整体移除；SEAMLESS 不再内置，需应用自建多窗口管理。
6. 宽高同步为“窗口→部件”单向；不支持内容→窗口的自动尺寸（需要时显式 `lcui_window_set_size()`）。
7. `opt.id` 同时是持久化键；若将来需要“有窗口标识但不持久化”，再加 `persist` 开关。
8. 必须显式调用 `lcui_load_settings()`；否则配置不会加载（渲染设置停留默认值），窗口状态也不会持久化。

## 12. 实施注意事项

- 遵循根目录 `AGENTS.md`：八空格缩进、`.clang-format`、文件内代码结构排序、未用参数不写
  `(void)xxx;`、优先调整函数定义顺序而非前置声明。
- 遵循 lcui 技能：不做参数有效性判断（信任契约）；改 `.h`（结构体字段）后必要时
  `xmake build -r <target>` 全量重编。
- 分层：`lib/ui-server` 与 `lib/ui` 不反向依赖 `src/`；路由在 `src/lcui_app.c`。
- 先完成 P1（含 ptk 修复与迁移，保证旧行为可用），再做 P2，避免一次性改动过大难以定位回归。

## 13. 未来扩展：窗口图标（不在本期范围）

当前设计不阻碍后续加入窗口图标；建议按下述方式扩展。

### API

```c
/* options 增加 */
const char *icon;   /* PNG 路径；NULL = 平台默认 */

/* 运行时设置 */
bool lcui_window_set_icon_from_file(lcui_window_t *w, const char *path);
void lcui_window_set_icon(lcui_window_t *w, const pd_canvas_t *image);
```

### PTK 原语

```c
void ptk_window_set_icon(ptk_window_t *wnd, const pd_canvas_t *icon);
```

- 输入统一为 ARGB `pd_canvas_t`（pandagl 已支持 PNG/JPEG/BMP：`pd_read_image_from_file`）。
- PTK 的 win32 后端已依赖 pandagl，新增该原语不引入新依赖。

### 平台实现

- **Win32**：ARGB canvas → 32bpp DIB + mask → `CreateIconIndirect` 得到 `HICON`；`SendMessage(WM_SETICON, ICON_BIG/ICON_SMALL, hicon)`；替换/销毁时 `DestroyIcon`。
- **X11**：`XChangeProperty(display, wnd, _NET_WM_ICON, CARDINAL, 32, ...)`，数据为 `[width, height, ARGB...]`；建议在 `XMapWindow` 前设置，避免默认图标闪现。
- **FB**：no-op。
- **macOS**：PTK 暂无后端。

### 尺寸与格式

- 首版接受单张图（建议 256×256 PNG），由平台缩放到标题栏/任务栏所需尺寸。
- 多尺寸（16/32/48/256）作为后续增强：Win32 分别设置 `ICON_SMALL/ICON_BIG`，X11 在 `_NET_WM_ICON` 中追加多个尺寸。
- `.ico` 不是 pandagl 的读取格式；Windows 的 exe 图标仍走链接期资源，运行时图标走 PNG。

### 时机与所有权

- `opt.icon` 在创建（隐藏态）时应用，早于 `show`，避免默认图标闪现。
- 窗口拥有转换后的平台图标，替换/销毁时释放；调用方保留 `pd_canvas_t` 的所有权。
