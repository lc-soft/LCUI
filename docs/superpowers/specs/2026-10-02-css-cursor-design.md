# CSS `cursor` 属性支持设计

- 日期：2026-10-02
- 状态：待评审
- 范围：`lib/css`、`lib/ptk`、`lib/ui`、`lib/ui-server`、`src/widgets`、tests、docs

## 1. 背景与目标

LCUI CSS 引擎未注册 `cursor` 属性，样式表里写 `cursor: pointer` 会产生
`[css-parser] value type not defined` 警告且无任何效果。本设计为 LCUI 增加
`cursor` 属性及全部常用值，并让鼠标悬停部件的 cursor 值真正驱动系统指针外观。

成功标准：

1. `cursor` 的 36 个标准关键字值可被解析、级联、参与 `:hover` 规则；
2. 鼠标悬停时系统指针按 CSS 语义（含继承语义）变化，Win32 与 X11 均生效；
3. 内置部件开箱获得合理默认光标（button→pointer、textinput→text 等）；
4. 有自动化测试与可视 preview；文档（中文先行）同步更新。

## 2. 平台调研结论

### 2.1 Windows（Win32）

- 系统提供标准光标：`LoadCursor(NULL, IDC_*)`，与 CSS 值高度重合，无需自定义图像。
- 类光标（`WNDCLASS.hCursor`）会在每次鼠标移动时被系统重置（`DefWindowProc` 处理
  `WM_SETCURSOR`），因此**必须拦截 `WM_SETCURSOR`**：`LOWORD(lParam) == HTCLIENT`
  时 `SetCursor` 并 `return TRUE`；非客户区交给 `DefWindowProc`（ImGui/GLFW 同款模式）。
- `cursor: none`（隐藏）：`WM_SETCURSOR` 中 `SetCursor(NULL)`，仅在指针位于本窗口客户区
  时生效，不触碰全局 `ShowCursor` 计数。
- `LoadCursor(NULL, IDC_*)` 返回共享系统光标句柄，**不可** `DestroyCursor`。
- 现状：`win32app.c` 类光标固定 `IDC_ARROW`，未处理 `WM_SETCURSOR`。

### 2.2 Linux（X11 / Xlib）

- `XDefineCursor(display, win, cursor)` 是**持久窗口属性**：设置后指针进出窗口自动
  生效，无需逐事件重设（比 Win32 简单）。
- 光标资源用 `XCreateFontCursor(display, XC_*)`（`<X11/cursorfont.h>`，libX11 自带，
  零新增依赖）。
- 局限：cursor font 为传统黑白字形，**不跟随系统光标主题**。主题化需 libXcursor
  （`XcursorLibraryLoadCursor` + freedesktop 命名），本期不做，留作后续可选增强。
- 隐藏：1×1 全透明 `XCreatePixmapCursor`。
- 现状：`x11app.c` 从未调用过 `XDefineCursor`。

### 2.3 其它

- **framebuffer**：LCUI 用 `lib/ui-cursor` 绘制软件指针（图像写死）。本期 `set_cursor`
  仅记录状态（getter 可断言），不切换软件指针图像。
- **Wayland**：PTK 无 Wayland 后端，不在范围内。

## 3. 范围决策（已与需求方确认）

| 决策点 | 结论 |
|---|---|
| 关键字集合 | 全量 36 个标准关键字；**不支持** `url()` 图像光标（文档注明） |
| 内置部件默认光标 | 本期一并加上（§7） |
| X11 实现 | cursor font（零新依赖） |
| Win32/X11 无法 1:1 映射的值 | 系统光标近似，映射成表（§5.4） |
| 驱动方式 | 每帧同步（方案 A），否决事件时读值（方案 B） |

## 4. 架构总览

```
样式数据流：
  CSS 规则/内联样式
    → lib/css：解析 + 级联 → computed_style.type_bits.cursor（6 bit）
    → lib/ui：hover 目标跟踪（已有全局 ui_events.targets[HOVER]）
    → lib/ui-server：ui_server_update() 末尾 ui_update_cursor()
        沿 hover 祖先链解析有效值 → 定位所属 ptk 窗口 → 应用
    → lib/ptk：ptk_window_set_cursor(wnd, PTK_CURSOR_*)
        ├─ win32：记录 + 即时 SetCursor；WM_SETCURSOR 持续生效
        ├─ x11：XDefineCursor（持久）
        └─ fb：仅记录（软件指针不变）
```

每帧同步时序（正确性论证）：

```
ptk 消息泵 → mousemove → ui_on_mouse_event → 命中测试 → add_status("hover")
  → lcui_ui_update → ui_server_update：
      ① 样式重匹配（:hover 规则生效，computed_style 更新）
      ② ui_update_cursor()：读到的一定是当帧新鲜样式
```

否决的替代方案：

- **方案 B（监听 mousemove/mouseover/mouseout，事件时读 target 值存全局）**：事件派发
  先于样式更新，`:hover { cursor: X }` 拿到旧值（悬停不动=箭头、抖动才变化，难以排查）；
  鼠标不动时的动态变更（set_style/class 切换）无事件可触发。且需在 ui_root 维护监听器
  生命周期、复制 `ui_events` 已有的 hover 状态。成本更高、正确性更差。
- **方案 C（部件 update_style 时自报）**：无法处理祖先值变化向子节点的传播，逻辑散落。
- **`ui_get_mouseover()` 返回已解算的 cursor 数值而非 widget 指针**：应用时还需要
  "改哪个窗口"，窗口归属只能由 ui-server 经 `ui_server_get_window(widget)` 解析，
  暴露数值反而拆碎状态。lib/ui 刻意不依赖 ptk，解算放在拥有窗口映射的 ui-server 层。

## 5. 详细设计

### 5.1 lib/css：属性注册

1. `include/css/types.h`
   - `css_prop_key_t`：在 `STYLE_KEY_TOTAL` 前追加 `css_prop_cursor`。
   - 新增枚举：

     ```c
     typedef enum {
             CSS_CURSOR_AUTO,
             CSS_CURSOR_DEFAULT,
             CSS_CURSOR_NONE,
             CSS_CURSOR_CONTEXT_MENU,
             CSS_CURSOR_HELP,
             CSS_CURSOR_POINTER,
             CSS_CURSOR_PROGRESS,
             CSS_CURSOR_WAIT,
             CSS_CURSOR_CELL,
             CSS_CURSOR_CROSSHAIR,
             CSS_CURSOR_TEXT,
             CSS_CURSOR_VERTICAL_TEXT,
             CSS_CURSOR_ALIAS,
             CSS_CURSOR_COPY,
             CSS_CURSOR_MOVE,
             CSS_CURSOR_NO_DROP,
             CSS_CURSOR_NOT_ALLOWED,
             CSS_CURSOR_GRAB,
             CSS_CURSOR_GRABBING,
             CSS_CURSOR_ALL_SCROLL,
             CSS_CURSOR_COL_RESIZE,
             CSS_CURSOR_ROW_RESIZE,
             CSS_CURSOR_N_RESIZE,
             CSS_CURSOR_E_RESIZE,
             CSS_CURSOR_S_RESIZE,
             CSS_CURSOR_W_RESIZE,
             CSS_CURSOR_EW_RESIZE,
             CSS_CURSOR_NS_RESIZE,
             CSS_CURSOR_NE_RESIZE,
             CSS_CURSOR_NW_RESIZE,
             CSS_CURSOR_SE_RESIZE,
             CSS_CURSOR_SW_RESIZE,
             CSS_CURSOR_NESW_RESIZE,
             CSS_CURSOR_NWSE_RESIZE,
             CSS_CURSOR_ZOOM_IN,
             CSS_CURSOR_ZOOM_OUT,
             CSS_CURSOR_TOTAL
     } css_cursor_t;
     ```

   - `css_computed_style_t.type_bits` 新增 `uint8_t cursor : 6;`（AUTO=0 恰为
     memset 零值，与现有属性约定一致）。
   - `css_keyword_value_t` 追加 34 个新关键字（`auto`/`none` 复用现有）。
2. `src/keywords.c`：`css_init_keywords` 中 `css_set_keyword` 注册 34 个新名字。
3. `src/properties/cursor.c`（新文件）：`css_cascade_cursor()`，模式同
   `pointer_events.c`——`switch (input[0].keyword_value)` 映射到
   `computed->type_bits.cursor`，未识别关键字回退 `CSS_CURSOR_AUTO`。
4. `src/properties.c`：

   ```c
   DEFINE_PROP(cursor, "cursor",
               "auto | default | none | context-menu | help | pointer | "
               "progress | wait | cell | crosshair | text | vertical-text | "
               "alias | copy | move | no-drop | not-allowed | grab | grabbing "
               "| all-scroll | col-resize | row-resize | n-resize | e-resize "
               "| s-resize | w-resize | ew-resize | ns-resize | ne-resize "
               "| nw-resize | se-resize | sw-resize | nesw-resize "
               "| nwse-resize | zoom-in | zoom-out",
               "auto");
   ```

默认值回填由 `css_cascade_style` 的 initial_value 机制自动完成；不提供
`css_computed_cursor()` 访问器，上层直接读 `type_bits.cursor`（与 `pointer_events`
一致）。

### 5.2 lib/ptk：平台光标 API

1. `include/ptk/types.h`
   - 新增 `ptk_cursor_type_t` 枚举：与 `css_cursor_t` 值序 1:1（前缀
     `PTK_CURSOR_`，含 `PTK_CURSOR_TOTAL`）。
   - `ptk_window_driver_t` 追加：
     `void (*set_cursor)(ptk_window_t *, ptk_cursor_type_t);`
     `ptk_cursor_type_t (*get_cursor)(ptk_window_t *);`
2. `include/ptk/window.h` 新增公共函数：

   ```c
   PTK_PUBLIC void ptk_window_set_cursor(ptk_window_t *wnd,
                                         ptk_cursor_type_t cursor);
   PTK_PUBLIC ptk_cursor_type_t ptk_window_get_cursor(ptk_window_t *wnd);
   ```

   `get_cursor` 返回最近一次 set 的值（初值 `PTK_CURSOR_AUTO`），供 ui-server 去重
   判断与测试断言。
3. `src/linux/app.c`：两个公共函数转发到 `linux_app.window` driver vtable。

### 5.3 各平台实现

**Win32（`win32app.c`）**

- `struct ptk_window` 追加 `ptk_cursor_type_t cursor; HCURSOR hcursor;`，创建时初始化为
  `PTK_CURSOR_AUTO` / `LoadCursor(NULL, IDC_ARROW)`。
- `ptk_window_set_cursor`：与 `wnd->cursor` 相同则早退；否则记录、查表
  `LoadCursorW(NULL, IDC_*)` 存入 `wnd->hcursor`（`PTK_CURSOR_NONE` → `NULL`），
  并**立即 `SetCursor(wnd->hcursor)`**——指针在本窗口时当帧可见，不在本窗口时
  Win32 保证无副作用。
- `ptk_window_process` 新增分支（置于现有 switch 内）：

  ```c
  case WM_SETCURSOR:
          if (wnd && LOWORD(arg2) == HTCLIENT) {
                  SetCursor(wnd->hcursor); /* NONE 时为 NULL，隐藏指针 */
                  return TRUE;
          }
          break; /* 非客户区走 DefWindowProc（默认箭头） */
  ```

- `WM_DESTROY` 无需释放 `hcursor`（全部为系统共享光标）。

**X11（`x11app.c`）**

- `struct ptk_window` 追加 `ptk_cursor_type_t cursor;`（初值 `PTK_CURSOR_AUTO`）。
- `x11_app` 追加惰性缓存 `Cursor cursors[PTK_CURSOR_TOTAL];`（0 = 未创建）。
- `ptk_x11window__set_cursor`：同值早退；`XCreateFontCursor` 按需创建并缓存
  （`NONE` → 1×1 全空 pixmap 的 `XCreatePixmapCursor`，同样缓存）；
  `XDefineCursor(display, wnd->handle, shape); XFlush(display);` 记录 `wnd->cursor`。
- app `destroy` 时对所有已缓存 cursor `XFreeCursor`。新建窗口不预设光标（默认行为即
  箭头；指针进入并移动后由帧同步接管）。

**Framebuffer（`fbapp.c`）**

- 窗口结构追加 `cursor` 字段，`set_cursor` 仅记录，`get_cursor` 返回；软件指针不变。

### 5.4 平台近似映射表（CSS 值 → 系统光标）

Win32 与 X11 均无精确对应时取最接近的系统光标；下表为唯一权威映射，实现与文档均以此为准：

| CSS 值 | Win32 | X11（cursor font） |
|---|---|---|
| auto / default | IDC_ARROW | XC_left_ptr |
| none | 隐藏（SetCursor NULL） | 1×1 透明 pixmap cursor |
| context-menu | IDC_ARROW | XC_left_ptr |
| help | IDC_HELP | XC_question_arrow |
| pointer | IDC_HAND | XC_hand2 |
| progress | IDC_APPSTARTING | XC_watch |
| wait | IDC_WAIT | XC_watch |
| cell | IDC_CROSS | XC_cross |
| crosshair | IDC_CROSS | XC_crosshair |
| text / vertical-text | IDC_IBEAM | XC_xterm |
| alias | IDC_ARROW | XC_left_ptr |
| copy | IDC_ARROW | XC_plus |
| move | IDC_SIZEALL | XC_fleur |
| no-drop / not-allowed | IDC_NO | XC_circle |
| grab | IDC_HAND | XC_hand1 |
| grabbing | IDC_HAND | XC_hand2 |
| all-scroll | IDC_SIZEALL | XC_fleur |
| col-resize / ew-resize / e-resize / w-resize | IDC_SIZEWE | XC_sb_h_double_arrow |
| row-resize / ns-resize / n-resize / s-resize | IDC_SIZENS | XC_sb_v_double_arrow |
| nw-resize / se-resize / nwse-resize | IDC_SIZENWSE | XC_top_left_corner / XC_bottom_right_corner / XC_top_left_corner |
| ne-resize / sw-resize / nesw-resize | IDC_SIZENESW | XC_top_right_corner / XC_bottom_left_corner / XC_top_right_corner |
| zoom-in / zoom-out | IDC_ARROW | XC_left_ptr |

### 5.5 lib/ui：hover 目标访问器

- `include/ui/events.h`（`ui_get_focus` 旁）新增：

  ```c
  LIBUI_PUBLIC ui_widget_t *ui_get_mouseover(void);
  ```

- `src/ui_events.c` 实现：返回 `ui_events.targets[UI_WIDGET_STATUS_HOVER]`。
  不新增任何状态——hover 目标本就由 `ui_widget_on_mouseover_event` 在 mousemove
  与 `ui_clear_event_target`（部件销毁）两条路径统一维护。

### 5.6 lib/ui-server：每帧同步

`src/server.c` 新增静态逻辑，在 `ui_server_update()` 末尾（所有连接样式/布局更新
完成后）执行：

```c
/* 伪代码（与实现一致） */
static void ui_update_cursor(void)
{
        ui_widget_t *owner = ui_get_mouseover();
        ui_widget_t *w;
        css_cursor_t cursor = CSS_CURSOR_AUTO;
        ptk_window_t *window = NULL;

        /* 单遍上行：取第一个非 auto 的 cursor 值（继承模拟）；
         * 窗口归属必须沿父链回溯到已连接的根部件——
         * ui_server_get_window() 按 conn->widget 精确匹配，对任意后代节点
         * 直接调用会返回 NULL，导致解算结果被丢弃 */
        for (w = owner; w; w = w->parent) {
                if (cursor == CSS_CURSOR_AUTO) {
                        cursor = w->computed_style.type_bits.cursor;
                }
                if (!window) {
                        window = ui_server_get_window(w);
                }
        }
        if (!window) {
                window = last_synced_cursor_window; /* hover 悬空时回写 AUTO */
                cursor = CSS_CURSOR_AUTO;
        }
        if (window) {
                ptk_window_set_cursor(window, cursor_map[cursor]);
                last_synced_cursor_window = window;
        }
}
```

要点：

- 窗口归属由 hover 目标沿父链回溯到已连接的根部件解析（`ui_server_get_window` 按连接根
  精确匹配，不能对后代节点直接调用），与“链上是否找到非 auto 值”无关；全链
  `auto` 时也要把 `AUTO` 写回窗口（清掉上次的自定义光标）。
- **继承模拟**：从 hover 目标沿祖先链取第一个非 `auto` 值。未设置的子节点延续父级
  （浏览器 `cursor` 为继承属性，`auto` 语义为“由上下文决定”，行为一致）；子节点
  显式 `default`/`none` 可截断父级值。
- `cursor_map[CSS_CURSOR_TOTAL]`：静态表，`css_cursor_t → ptk_cursor_type_t`
  （值序 1:1，表体即枚举顺序）。
- 去重依赖平台层 `set_cursor` 的同值早退，ui-server 不加缓存（`last_synced_cursor_window`
  仅用于 hover 悬空场景，在窗口连接销毁/`PTK_EVENT_CLOSE` 时置 NULL，避免悬挂）。
- 开销：每帧一次祖先链指针遍历 + 一次 vtable/直调比较，可忽略。
- 边界：`pointer-events: none` 的部件不参与命中测试，光标自然跳过；`:disabled`
  状态可用（`button:disabled { cursor: not-allowed; }` 当帧生效）。

## 6. 数据与状态一览

| 位置 | 新增 |
|---|---|
| `css_computed_style_t.type_bits` | `cursor : 6` |
| `ptk_window_t`（win32/x11/fb 各自结构） | `cursor`（win32 另有 `hcursor`） |
| `ui_events` | 无（复用 hover 目标） |
| ui-server | `cursor_map` 静态表、`last_synced_cursor_window` |

## 7. 内置部件默认光标（`src/widgets/*.c` 的 `css_string`）

| 部件 | 规则 |
|---|---|
| button | `cursor: pointer` |
| checkbox、radio-group-item、switch | `cursor: pointer` |
| select trigger、combobox、dropdown-menu trigger/item、tabs tab | `cursor: pointer` |
| anchor | `cursor: pointer` |
| textinput | `cursor: text` |
| scrollbar thumb | `cursor: grab` |

纯展示部件（text/label/progress/badge/alert/empty/item/icon/canvas 等）不设默认
cursor；portal-root 空白区自然落回 `auto`（箭头），浮层 content 内部件按自身 hover
链解算，无需特殊规则。

## 8. 测试

1. **libcss-tests**：`lib/css/tests/test_css_cursor.c`
   - 36 个关键字逐个 parse+cascade → `type_bits.cursor` 断言（含 `:hover` 规则内取值）；
   - 非法值（如 `cursor: foobar`、`cursor: url(a.cur)`）→ 回退初始值 `auto`；
   - 未声明时初始值为 `auto`。
   - 参照 `test_css_text_decoration.c` 的注册方式挂入 css 测试 main。
2. **顶层集成**：`tests/integration/test_cursor.c`（注册进 `tests/main.c`）
   - 场景：`lcui_init` + `ui_load_css_string` 构造
     `.hand { cursor: pointer }` 父嵌套未设置子节点、子显式 `default`、
     `.hov:hover { cursor: crosshair }`、`.none { cursor: none }`；
   - 步骤：`ui_dispatch_event` 合成 mousemove → `ui_server_update()` →
     断言 `ptk_window_get_cursor(ui_server_get_window(root))`；
   - 用例：继承（父 pointer 子未设 → POINTER）、截断（子 default → DEFAULT）、
     `:hover` 当帧生效（进入即 CROSSHAIR，无需二次移动）、`none`（NONE）、
     移回普通区域（AUTO）、hover 部件销毁后回写 AUTO。
   - 三平台 `set_cursor` 均记录状态，断言不依赖真实显示（Win32 CI / X11 / fb 可跑）。
3. **可视 preview**：`tests/previews/preview_cursor.c`：网格展示全部 36 值，悬停块
   应显示对应指针（人工验证近似映射观感）。
4. 回归：`xmake build && xmake test`；确认既有样式表无 `cursor` 相关 parser 警告
   （此前为未注册警告，注册后应消失）。

## 9. 文档

- `docs/zh-CN/handbook/styling.mdx`：属性列表新增 `cursor` 行；值表（36 值分组）；
  说明：继承模拟行为、`url()` 不支持、Win32/X11 为系统光标近似（附 §5.4 表）、
  FB 软件指针不变。
- **英文页 `docs/en/handbook/styling.mdx` 待中文评审通过后一次性同步**（AGENTS 规则）。
- `README.md` / `README.zh-cn.md` CSS 属性清单补 `- [x] cursor`。
- 若 doc-viewer 的 styling 页面为 mdx 生成产物，随构建刷新，不手改生成物。

## 10. 已知限制（写入文档）

1. `url()` 图像光标、多光标候选列表（`cursor: url(x), pointer`）不支持。
2. 光标仅在**鼠标移动产生命中测试事件**或本帧 hover 链上样式变化时更新；部件树在静止
   指针下整体替换（浮层弹出盖住指针）不会重算 hover，光标保持原值直至下一次移动——与
   现事件体系一致。
3. X11 使用传统 cursor font 字形，不跟随系统光标主题（后续可加 libXcursor 可选依赖）。
4. Win32 上 grab/copy/zoom 系列为近似观感（§5.4）。
5. framebuffer 软件指针不随 `cursor` 变化。
6. Wayland 无后端，不涉及。

## 11. 实施注意事项

- 遵循根目录 `AGENTS.md`：八空格缩进、`.clang-format` 格式化、文件内代码结构排序、
  未用参数不写 `(void)xxx;`、函数定义按依赖排序优先于前置声明。
- 遵循 lcui 技能：不做参数有效性判断（信任契约）；测试注册与运行规则按 AGENTS。
- `type_bits` 为匿名结构体内位域，新增 `cursor : 6` 时注意现有分组字节对齐惯例，
  放入合适位置避免打散现有字段。
- 提交拆分建议：`feat(css): support cursor property`、`feat(ptk): add window cursor
  api`、`feat(ui-server): sync system cursor with hover widget style`、
  `feat(ui): set default cursors for interactive widgets`、`docs: ...`、
  `test: ...`（scope 为 lib 目录名）。
