# 窗口状态恢复与窗口生命周期时序设计

> **已被取代（2026-10-16）**：新方案见 `2026-10-16-window-api-design.md`（`lcui_init` 核心化 + 显式 `lcui_window_*`）。本文档的根因分析（A 位置语义 / B 显示耦合 / C 时机）仍有效，但“`lcui_ui` 内部显示门控”的实现思路已被显式窗口 API 取代。

- 日期：2026-10-13（2026-10-15 修订：保留立即建窗，改为延迟显示）
- 状态：待评审
- 范围：`lib/ptk`（win32/x11/fb）、`lib/ui-server`、`src/lcui_ui.c`、`src/lcui_app.c`、`src/lcui_window_state.c`、`tests/integration`

## 1. 背景与目标

LCUI 默认由库创建主窗口并把 `ui_root()` 绑定到它，开发者无需手动建窗。当前窗口状态保存/恢复存在两个可见缺陷：

1. **恢复时没有恢复上次的位置**：位置每次都发生偏移，且连续运行会持续漂移。
2. **窗口先显示、再恢复尺寸**：窗口出现时的尺寸/位置不是最终值，随后才跳到保存值。

成功标准：

1. 保存的 `x/y/width/height/maximized` 在下次启动时精确还原，多次运行不漂移；
2. 窗口第一次可见时，尺寸、位置、最大化状态均已最终确定，无可见跳变、无默认几何闪现；
3. 未绑定窗口状态的普通应用行为不变（默认居中、`ui_widget_resize` 驱动窗口尺寸）；
4. 有覆盖上述行为的自动化回归测试。

## 2. 现状时序与根因

### 2.1 实测时序（doc-viewer 顺序）

以临时探针（已清理）在 `xmake run lcui-tests` 中复现，关键日志：

```
lcui_init()
 └ lcui_app_init()                         # lcui_init 第一步
    └ lcui_ui_init()
       ├ ui_init() / ui_server_init()
       └ lcui_ui_set_display(WINDOWED)     # 窗口在此创建（隐藏）
          ├ ptk_window_create(0,0,0,0)
          └ ui_server_connect(root, wnd)   # conn->state = PENDING
             └ ptk_process_native_events(ALL)   # 经 dispatcher 重入主循环
                └ ui_server_update()
                   ├ PENDING→INITIALIZING
                   ├ ui_server_refresh_window():
                   │    root 的 top/left 为 auto → 居中；默认 1200x900@(636,285)
                   ├ INITIALIZED；派发 PTK_EVENT_CREATE（此刻无 binding，空转）
                   └ 本帧 visible mutation 因 state 仍 PENDING 被丢弃
                └ render 一帧（offscreen）
 └ lcui_window_state_init()                # 窗口建好后才注册监听
 └ lcui_settings_load()                    # 窗口建好后才加载配置
lcui_init() 返回

app: ui_widget_resize(root, 1280,720); lcui_bind_window_state(root, "main")
 └ wstate_apply(): set_position(32,48) → set_size(420,260)
      → set_maximized(false) → ShowWindow(SW_RESTORE)   # 强行显示隐藏窗口
         └ 同步 WM_SHOWWINDOW → 重入 ui_server_update
            → 读到 root 旧布局(630x390) → ptk_window_set_size(630,390) + show
main loop 下一次 update → ptk_window_set_size(420,260)   # 可见状态下再缩
关闭时 wstate_query 用 get_position 回读 → x=46 y=96（原始 32/48）
```

实测数据：

```
after lcui_init   x=650 y=333 w=1200 h=900    # 默认居中（隐藏）
after bind        x=46  y=96  w=420  h=260    # get(set(32,48)) == (46,96)
保存回 settings   x=46  y=96                  # 相对原值漂移 +14/+48，每次运行继续漂
```

### 2.2 根因

**根因 A：位置“假恢复”——存/取坐标原点不是互逆，且逐次漂移。**

- `ptk_window_get_position`（`lib/ptk/src/windows/win32app.c:652`）用
  `ClientToScreen(hwnd, {0,0})`，返回**客户区原点**的屏幕坐标。
- `ptk_window_set_position`（同文件 `:644`）把入参当**窗口外框**，做
  `x += SM_CXFIXEDFRAME; y += SM_CYFIXEDFRAME` 后 `SetWindowPos`。

两者不互逆，实测 `get(set(32,48)) == (46,96)`。`wstate_query` 用 get 存、`wstate_apply`
用 set 还原，于是每次运行都漂移一次。X11 的 `set_position`/`get_position`
（`lib/ptk/src/linux/x11app.c:609-629`）都是客户区语义、自洽；FB 无此概念。**故为 win32
实现 bug。**

**根因 B：几何恢复与显示耦合在同一步。**

`wstate_apply`（`src/lcui_window_state.c:180`）在恢复几何时调用
`ptk_window_set_maximized(false)`，其 win32 实现是 `ShowWindow(SW_RESTORE)`
（`win32app.c:678`），会立刻显示隐藏窗口。此时：①窗口尺寸还是 `refresh_window` 设的默认
值；②`ShowWindow` 同步重入一轮 `ui_server_update`，用尚未应用新布局的旧 box 又设了一次
窗口尺寸。显示动作与几何应用耦合在同一步，导致“先显示、后恢复尺寸”。

**根因 C：显示时机没有门控，窗口可能在状态应用完成前就可见。**

窗口在 `lcui_app_init()`（`lcui_init` 第一步）内创建并 connect本身是可以接受的（保持现有
“库自动建主窗”的模型），问题在于：从创建到应用 `lcui_bind_window_state()` 之间存在多个可能
**显示窗口**的路径——
`ui_server_connect` 内部事件泵会渲染一帧、`ui_server_on_widget_mutation` 会因 visible
mutation 调用 `ptk_window_show`、`wstate_apply` 会经 `set_maximized` 显示。任一路径先把窗口
显示出来，用户就会看到“默认几何 → 再跳到保存值”。

**修复方向（已与需求方确认）**：保留**立即建窗**，但引入**显示门控**——只有在
「`lcui_init` 完成且应用已绑定状态」之后、且「该连接已完成一次成功渲染」时，才第一次
`ptk_window_show`。几何（位置/尺寸/最大化）在窗口显示前全部确定。

### 2.3 现有测试为何漏掉

`tests/integration/test_settings.c:342` 只断言 `width/height`，且是在窗口早已初始化且显示
**之后**才绑定 root，既不会触发位置漂移，也观察不到首次显示的时序问题。

## 3. 范围决策（已与需求方确认）

| 决策点 | 结论 |
|---|---|
| 修复范围 | 完整修复：位置语义 + 显示/几何解耦 + 显示门控 |
| 窗口创建 | **保留立即建窗**（`lcui_ui_init` 内不变） |
| 显示时机 | 新增门控：`lcui_init` 结束 + 该连接首个成功渲染后，才首次显示 |
| 位置语义 | 统一为“客户区原点”；win32 改为与 get 互逆，X11 维持现状 |
| 最大化语义 | `set_maximized` 只改状态不隐式显示；隐藏时记录，`show` 时应用 |

### 3.1 已考虑并否决的替代方案

- **改为应用手动建窗**（`create(hidden)→bind widget→bind state→show`）：能让生命周期显式、天然
  消除根因 C，且无需门控。但它是一次框架契约改动——现有“`ui_root()` 自动获得主窗口”的设计、
  文档/示例，以及 `LCUI_DISPLAY_SEAMLESS`（按 root 顶级子节点自动建窗）都要一并改写；并且
  它并不能修掉 A、B。故否决，保留自动建窗，仅内部改为显式顺序。
- **移除 `ui_server_connect` 内的 `ptk_process_native_events`**：实测移除后连接一直停在
  `PENDING`，窗口在 `lcui_init` 结束前保持 `0x0`，`refresh_window`/`CREATE`/首帧渲染推迟到主
  循环第一次迭代。当前 `window_mutation_list_add` 与 `ui_server_on_window_resize` 已有
  `state == INITIALIZED` 守卫，足以挡住 CreateWindow 排队消息对部件几何的覆盖，因此移除并非
  必需；保留该 pump 更稳（代价仅是隐藏态下多算一次默认几何，无可见影响）。故保留。

## 4. 目标生命周期模型

```
阶段 1  创建（隐藏）          lcui_init 内立即 ptk_window_create，不显示
阶段 2  应用几何              应用 bind；set_position/set_size/set_maximized 全部确定
                              （set_maximized 只记录，不显示）
阶段 3  首帧就绪              门控打开后，offscreen 完成一次成功渲染
阶段 4  显示                  ptk_window_show（几何已是最终值）
```

关键约束：**任何 `ptk_window_show` 之前，几何必须已最终确定，且门控必须已打开。**

修改后流程（门控版）：

```
┌─ lcui_init() ─────────────────────────────────────────────────────────┐
│ lcui_app_init()                                                       │
│   └ lcui_ui_init()                                                    │
│      └ lcui_ui_set_display(WINDOWED)      ← 立刻建窗（隐藏）           │
│         ├ ptk_window_create(0,0,0,0)       hidden                     │
│         └ ui_server_connect(root, wnd)     state = PENDING            │
│            └ ptk_process_native_events → ui_server_update             │
│               ├ refresh_window(): 默认几何(居中)                      │
│               ├ PENDING→INITIALIZED → CREATE(此时无 binding，空转)    │
│               └ render 一帧(仅 offscreen)                             │
│ lcui_window_state_init(); lcui_settings_load()                        │
│ show_enabled = true                       ← 门控在 init 完成时打开    │
└───────────────────────────────────────────────────────────────────────┘
                                │
                                ▼
   应用侧（lcui_init 返回后，主循环前）
     ui_widget_resize(root, ...)
     lcui_bind_window_state(root, "main")
       └ wstate_apply(): set_position(客户区语义)
                         set_size(最终尺寸)
                         set_maximized(仅记录，不显示)
                                │
                                ▼   （窗口仍隐藏，几何已最终确定）
┌─ 主循环每帧 ──────────────────────────────────────────────────────────┐
│ dispatch:                                                             │
│   ① ui_server_update()                                                │
│   ② lcui_ui_render()                                                  │
│        ├ ui_server_render()        （offscreen 渲染，记录 rendered）  │
│        └ if (show_enabled) ui_server_show_windows():                  │
│             state==INITIALIZED && rendered && widget 可见 && !shown   │
│               → ptk_window_show + 应用期望 maximized；shown = true     │
└───────────────────────────────────────────────────────────────────────┘
                                │
                                ▼
              窗口首次可见时 = 已是保存的最终几何
```

## 5. 详细设计

### 5.1 lib/ptk：win32 位置语义（根因 A）

`ptk_window_set_position` 改为按**客户区原点**语义，作为 `ptk_window_get_position` 的
逆操作：

```c
void ptk_window_set_position(ptk_window_t *wnd, int x, int y)
{
        POINT p = { 0, 0 };
        RECT rect;

        ClientToScreen(wnd->hwnd, &p);        /* 当前客户区原点 */
        GetWindowRect(wnd->hwnd, &rect);      /* 当前窗口外框 */
        SetWindowPos(wnd->hwnd, HWND_NOTOPMOST,
                     rect.left + (x - p.x), rect.top + (y - p.y), 0, 0,
                     SWP_NOSIZE | SWP_NOZORDER);
}
```

不再使用 `SM_CXFIXEDFRAME/SM_CYFIXEDFRAME` 偏移。`ptk_window_create` 的 `x/y/width/height`
保持“窗口外框坐标”现状（现有调用方一律传 `0,0,0,0`），需要按客户区原点定位/设尺寸时先
`create` 再调用 `ptk_window_set_position`/`ptk_window_set_size`；在 `ptk/window.h` 中以注释
明确 `create` 与 `set_position` 的坐标系差异。X11/FB 无需改动（X11 已是客户区语义）。

回归目标：对任意 `(x,y)`，`set(x,y); get() == (x,y)` 精确往返。

### 5.2 lib/ptk：显示与最大化解耦（根因 B）

给 `ptk_window_t`（三平台各自结构）增加“期望最大化状态”字段（如 `bool maximized;`），
并调整 API 语义：

- `ptk_window_set_maximized(w, maximized)`：
  - 记录期望状态；
  - **仅当窗口当前可见**时才立即应用（win32 `ShowWindow(SW_MAXIMIZE/SW_RESTORE)`）；
  - 窗口隐藏时仅记录，不显示。
- `ptk_window_show(w)`：
  - win32：`ShowWindow(SW_SHOWNORMAL)`，随后若期望最大化则 `ShowWindow(SW_MAXIMIZE)`；
  - x11：`XMapWindow` 后若期望最大化补发一次 `_NET_WM_STATE`；
  - fb：no-op。

这样 `wstate_apply` 调用 `set_maximized` 不再产生显示副作用，隐藏窗口的几何与最大化状态可
在显示前就绪。新增字段是“期望状态”，`ptk_window_is_maximized` 仍返回窗口的**实际**状态
（win32 用 `IsZoomed`），用于关闭时保存。

### 5.3 `src/lcui_ui.c`：显示门控（保留立即建窗，替代延迟建窗）

窗口创建保持不变（`lcui_ui_init` 内 `lcui_ui_set_display(DEFAULT)`）。新增门控，确保首次
显示发生在 `lcui_init` 完成、应用完成 `lcui_bind_window_state` 之后：

- `struct lcui_ui` 新增 `bool show_enabled;`，初值 `false`。
- 新增内部函数 `lcui_ui_enable_show(void)`（实现于 `src/lcui_ui.c`，声明于内部头
  `src/lcui_app.h`），在 `lcui_init()`（`src/lcui.c`）的最后一步、`lcui_settings_load()`
  之后调用，置 `show_enabled = true`。这样 `ui_server_connect` 内部那一帧渲染（发生在
  `lcui_app_init`/`lcui_ui_init` 期间）早于门控打开，不会提前显示窗口；而 `lcui_init`
  之后到应用 `bind` 之间不会再发生渲染，因此首次显示必然晚于 bind。
- `lcui_ui_render()`（`src/lcui_ui.c:41`，由 `lcui_app_render_frame` 调用）改为：

  ```c
  size_t lcui_ui_render(void)
  {
          size_t count = ui_server_render();

          if (lcui_ui.show_enabled) {
                  ui_server_show_windows();
          }
          return count;
  }
  ```

- 应用显式调用 `lcui_ui_set_display(其它模式)` 时行为不变，立即建/切窗口。

### 5.4 `lib/ui-server`：首帧渲染成功后再显示

- `ui_connection_t` 语义澄清：渲染由“widget 是否可见”驱动；显示由“是否已成功渲染”驱动。
  新增 `bool shown;`（OS 层面是否已 `ptk_window_show`）。
- 新增公共 API（`lib/ui-server/include/ui_server.h`）：

  ```c
  LIBUI_SERVER_PUBLIC void ui_server_show_windows(void);
  ```

  遍历连接，对满足 `state == UI_CONNECTION_STATE_INITIALIZED && conn->rendered &&
  !conn->shown && ui_widget_is_visible(conn->widget)` 的连接调用 `ptk_window_show(conn->window)`
  并置 `shown = true`（`ptk_window_show` 负责应用期望的 maximized 状态，见 5.2）。
- `ui_server_on_widget_mutation` 中 visible 分支调整：**不再**直接 `ptk_window_show`（显示
  统一走 `ui_server_show_windows` 的门控路径）；widget 变为不可见时仍立即
  `ptk_window_hide` 并置 `shown = false`。
- `ui_server_render()` 仍按现有条件（widget 可见）做 offscreen 渲染，保证隐藏期间
  `conn->rendered` 能置位。
- `ui_server_on_window_visibility_change`（原生 `WM_SHOWWINDOW`）同步 `shown`。
- ui-server 不依赖 `src/lcui_ui.c`：门控开关由 `src` 侧决定何时调用
  `ui_server_show_windows()`，满足分层约束（`lib/ui-server` 不反向依赖 `src/`）。

### 5.5 `src/lcui_window_state.c`：职责收窄

- `wstate_apply` 仍设置位置/尺寸/最大化（依赖 5.2 后不再显示），但不负责显示窗口。
- `lcui_bind_window_state` 保持：登记 binding + `ui_widget_resize(widget, w, h)`；窗口已存在
  （立即建窗方案下，绑定 root 时窗口通常已创建）则立即 apply 到隐藏窗口；窗口为 NULL 时
  由 CREATE 处理器在窗口创建时应用（既有 pending 路径）。

## 6. 数据与状态一览

| 位置 | 变更 |
|---|---|
| `ptk_window_t`（win32/x11/fb） | 新增“期望最大化状态”字段 |
| `ui_connection_t`（`lib/ui-server/src/server.c`） | 新增 `bool shown`；`ui_server_show_windows()` |
| `ui_server.h` | 新增 `ui_server_show_windows()` |
| `struct lcui_ui`（`src/lcui_ui.c`） | 新增 `bool show_enabled`；`lcui_ui_render` 门控 |
| `src/lcui_window_state.c` | `wstate_apply` 去掉显示副作用 |
| PTK 位置语义 | win32 `set_position` 改为客户区原点、与 get 互逆 |

## 7. 测试

新增 `tests/integration/test_window_restore.c`（注册进 `tests/main.c`）：

1. **位置互逆**：`ptk_window_set_position(w, x, y)` 后 `ptk_window_get_position` 精确等于
   `(x, y)`；连续多次设置/读取不漂移。
2. **恢复回归（doc-viewer 顺序）**：写配置 `x/y/width/height/maximized` → `lcui_init` →
   `ui_widget_resize(ui_root(), ...)` → `lcui_bind_window_state(ui_root(), "main")` → 泵事件
   多轮 → 断言窗口最终几何与 `maximized` 等于配置值（尤其 `x/y`）。
3. **多次运行不漂移**：关闭窗口同步状态后再读取，`x/y` 与写入值一致。
4. **门控行为**：`lcui_init` 返回后、首次成功渲染前窗口不可见（可断言 `shown` 状态或经由
   平台可见性）；渲染后可见，且可见时几何已是保存值。
5. **未绑定默认行为**：未调用 bind 时，窗口仍能正常显示且默认居中，`ui_widget_resize`
   驱动窗口尺寸。

更新 `tests/integration/test_settings.c::test_window_state_binding`：补 `x/y` 断言。

回归：`xmake build && xmake test`（`xmake run lcui-tests --grep="window"` 聚焦）。

## 8. 兼容与风险

- **保留立即建窗**：`lcui_init()` 返回后 `ui_server_get_window(ui_root())` 仍非 NULL，现有
  依赖不变；无需改动 `tests/integration/test_settings.c` 的获取时机。
- **新增 `ui_server_show_windows()`**：不改变既有公共 API 语义，仅新增；`ui_server_render`
  行为不变。
- **位置语义变更**：调用方为 `ui_server_refresh_window`、`ui_server_on_widget_mutation`、
  `wstate_apply`，均按客户区原点，语义一致；需确认无其它代码依赖旧偏移。
- **显示时机变更**：依赖首个成功渲染；若连接长期不可渲染（widget 不可见），窗口保持隐藏，
  与“不可见即不显示”语义一致。
- **门控打开的时机**：`show_enabled` 在 `lcui_init()` 末尾、settings 加载后打开；
  `ui_server_connect` 内部那帧渲染早于它，故不会抢跑显示。若后续在 `lcui_init` 末期新增
  渲染，需要重新评估。
- **最大化状态**：Hidden→Shown 之间设置的期望状态需在 show 时正确落地（三平台分别验证）。

## 9. 文档

- 无需改动面向用户的 CSS/widget 文档。
- 在 PTK 窗口 API 头（`lib/ptk/include/ptk/window.h`）补充 `set_position`/`set_maximized`
  的语义注释（客户区原点；隐藏时记录最大化状态）。
- 在 `ui_server.h` 为 `ui_server_show_windows()` 补注释。

## 10. 已知限制

1. 位置以“客户区原点”为唯一语义；不提供“窗口外框原点”变体。
2. 首帧后显示依赖至少一次成功渲染；极端情况下（首帧渲染失败）窗口不会自行出现。
3. FB 平台无最大化概念，`maximized` 字段仅记录。

## 11. 实施注意事项

- 遵循根目录 `AGENTS.md`：八空格缩进、`.clang-format` 格式化、文件内代码结构排序、未用
  参数不写 `(void)xxx;`、优先调整函数定义顺序而非前置声明。
- 遵循 lcui 技能：不做参数有效性判断（信任契约）；改 `.h` 后若 `xmake` 未重编依赖方，用
  `xmake build -r <target>` 全量重编（结构体字段变化尤其注意）。
- 改动涉及库（ptk/ui-server）与 `src/`，注意 `lib/ui-server` 不反向依赖 `src/`：门控开关由
  `src` 决定何时调用 `ui_server_show_windows()`。
- 建议提交拆分：
  - `fix(ptk): window position should be the inverse of get_position`
  - `fix(ptk): setting maximized should not implicitly show the window`
  - `fix(ui-server): show window only after the first successful render`
  - `feat(lcui): gate window show until after init and first frame`
  - `test: cover window state restore without drift`
