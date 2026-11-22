# Widget 实现、Portal 与事件生命周期

> 新增 widget 的结构约定、prototype 继承、Portal 浮层、自绘动效缓存、事件参数、LINK/UNLINK 与销毁顺序。

## 实现新 Widget

### 标准结构（三处协同）

1. `include/LCUI/widgets/<name>.h` — 公共 API + `LCUI_API ui_register_<name>()` 声明
2. `src/widgets/<name>.c` — `ui_register_<name>()` 实现（prototype 注册）
3. `include/LCUI/widgets.h` 追加 `#include "widgets/<name>.h"`
4. `src/lcui_widgets.c::lcui_widgets_init()` 追加 `ui_register_<name>();`

xmake 的 `target("lcui")` 用 `add_files("src/**.c")`，新 `.c` 自动纳入编译，
不需要改 `xmake.lua`。

### Prototype 初始化：必须链式调用父 prototype 的 init

```c
static ui_widget_prototype_t *ui_label_prototype;

static void ui_label_on_init(ui_widget_t *w)
{
        ui_label_prototype->proto->init(w);   /* ★ 必须先调父 prototype init */
        ui_widget_on(w, "click", ui_label_on_click, NULL);
}

void ui_register_label(void)
{
        ui_label_prototype = ui_create_widget_prototype("label", "text");
        ui_label_prototype->init = ui_label_on_init;
        ui_load_css_string(ui_label_css, __FILE__);
}
```

**忘记链式调用**会让父 prototype 的 `ui_widget_add_data` / 资源初始化未执行，
子 prototype 在使用时立即崩溃（典型：调用 `ui_create_widget("label")` 后任何
操作都会段错误，无任何日志）。这是首次实现 label 时踩到的坑。

参考：`src/widgets/button.c:46-50`、`src/widgets/anchor.c:186-192`。

### 容器型 Widget：parent 传 NULL，不链式 init

容器型 widget（如 radio-group、scrollarea、progress、canvas）不需要继承自
任何具体 widget 类型（不持有 text layer 等内部资源）。注册时 parent 传 `NULL`，
得到默认 prototype。**init 回调里不要** `proto->proto->init(w)`——`proto->proto`
此时是 `NULL`，解引用立即段错误。

```c
void ui_register_radio_group(void)
{
        ui_radio_group_prototype =
            ui_create_widget_prototype("radio-group", NULL);   /* ★ parent=NULL */
        ui_radio_group_prototype->init = ui_radio_group_on_init;
        /* 不要写：ui_radio_group_prototype->proto->init(w);  proto->proto 是 NULL */
}
```

参考：`src/widgets/scrollarea.c`、`src/widgets/progress.c`、`src/widgets/canvas.c`。
radio-group 实现可对照 `src/widgets/radio_group.c`。

### `ui_create_widget(NULL)`：创建通用容器 widget

`ui_create_widget(NULL)` 不注册任何 prototype，返回默认 prototype 的 widget，
`widget->type` 为 `NULL`。用于 shadcn 风格包装 div（`<div class="flex gap-2">`）
或测试场景下的中间层节点。

```c
ui_widget_t *wrap = ui_create_widget(NULL);   /* 通用 div，widget->type == NULL */
ui_widget_add_class(wrap, "flex");
ui_widget_add_class(wrap, "gap-2");
```

⚠️ **任何按 `w->type` 字符串匹配节点类型的回调都必须先判 NULL**，
否则 `strcmp(NULL, "...")` 会段错误。常见于 `ui_widget_each` 回调（详见下文）。

### `ui_widget_each` 回调：必须过滤 NULL 类型节点

`ui_widget_each` 递归遍历**所有**后代节点，包括 `ui_create_widget(NULL)` 创建
的 wrapper div。回调里按 `w->type` 匹配前必须判 NULL：

```c
static bool is_radio_item(ui_widget_t *w)
{
        return w && w->type && strcmp(w->type, "radio-group-item") == 0;
}

static void on_each_descendant(ui_widget_t *w, void *arg)
{
        if (!is_radio_item(w)) {
                return;   /* ★ 必须先过滤 NULL type */
        }
        /* ... 处理 radio-group-item */
}
```

真实踩坑：radio-group 测试里 wrapper div 导致 `strcmp(w->type, ...)` 段错误。
参考：`src/widgets/radio_group.c::ui_widget_is_radio_item`、
`lib/ui/src/ui_tree.c::ui_widget_each`（line 323）。

### 自定义视觉装饰：靠 child widget + CSS 类，不靠字符

LCUI CSS 引擎**不支持伪元素**（`::before` / `::after`）。要做自定义装饰
（如 radio 选中态的实心圆、checkbox 选中态的对勾），方案是：

- 在 widget init 回调里 `ui_create_widget(NULL)` 创建一个子节点
- `ui_widget_add_class(child, "indicator")` 加 class
- CSS 里用 `selector .indicator { display: none; ... }` + `[checked="true"] .indicator { display: block; ... }` 控制可见性
- 用 `background-color` + `border-radius`（长度值，如 `4px`）实现实心圆，**不要用字符**
  - 字符方案依赖字体（缺字体会出现 `□`），不健壮
  - `border-radius` 不支持 `%`，必须写长度值

参考实现：`src/widgets/radio_group.c::ui_radio_group_item_on_init` + 对应 CSS。

### Radio/Group 类组件的 click 事件：父监听 + target 向上找

容器型 widget（radio-group）的子节点（radio-group-item）自己不挂 click 监听器。
容器在 init 回调里挂一个 click 监听，事件触发时从 `e->target` 沿
`target->parent` 链向上找匹配类型的子节点：

```c
static void ui_radio_group_on_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_widget_t *target = e->target;
        while (target && target != w) {
                if (is_radio_item(target)) {   /* 含 NULL type 守卫 */
                        break;
                }
                target = target->parent;
        }
        if (!target || target == w || target->disabled) {
                return;   /* 点空白 / 点自己 / 点 disabled 都不响应 */
        }
        /* 遍历 w 的所有 radio-group-item 后代：target 设 checked=1，其他设 0 */
        /* 最后 emit "change" 事件 */
}
```

优点：支持 wrapper div 嵌套（shadcn `<div class="flex gap-2"><item/></div>`），
因为 click target 可能是 wrapper 或更深层的 indicator 子节点，向上找总会
到达第一个 radio-group-item。

### Radio/Group 类组件的 update 操作：遍历**子孙**而非仅直接子项

容器型 widget 的 item 常被嵌在 wrapper div 内（shadcn 的
`<div class="flex items-center space-x-2"><RadioGroupItem /></div>`）。遍历
item 状态时**必须用 `ui_widget_each` 走全部后代**，不要用 `ui_widget_get_child`
+ `ui_widget_next` 只遍历直接子项。

```c
/* ✅ 正确：找全部后代 */
void ui_radio_group_update(ui_widget_t *w)
{
        struct radio_update_args args;
        args.target_value = ui_widget_get_attr(w, "value");
        ui_widget_each(w, ui_radio_group_sync_each, &args);
}

/* ❌ 错误：只找直接子项，wrapper div 内的 item 被漏掉 */
for (child = ui_widget_get_child(w, 0); child;
     child = ui_widget_next(child)) { ... }
```

回调里必须用 `w->proto == item_prototype` 而非 `strcmp(w->type, ...)` 判断类型：
`ui_widget_each` 会把 wrapper div（`w->type == NULL`）也传进来，
`strcmp(NULL, ...)` 段错误。proto 比较是 O(1) 指针比较，且与 widget 是否有名
type 解耦。

### 事件注册：在 init 回调里挂 `ui_widget_on`

事件监听器**不通过 prototype 字段**注册，而是在每个 widget 实例创建时
（init 回调内）通过 `ui_widget_on(w, "click", handler, NULL)` 绑定。

### disabled 状态：复用内建机制，不要自定义 attr

LCUI 自带：

- `w->disabled`（`lib/ui/include/ui/types.h`）
- `ui_widget_set_disabled(w, bool)`（`lib/ui/include/ui/base.h:77`）
- HTML `disabled` 属性自动同步 `w->disabled`（`lib/ui/src/ui_widget_attributes.c:72`）
- 自动 add/remove `disabled` status（`lib/ui/src/ui_widget_status.c:98`）

Widget 代码直接读 `w->disabled` 判断，不要用自定义 attr 字符串。

### 自定义事件：字符串名 + emit_event

```c
ui_event_t ev;
ui_event_init(&ev, "change");          /* 自定义事件名 */
ui_widget_emit_event(widget, ev, NULL);
```

参考：`src/widgets/textinput.c:375`（`"change"`）、`:656`（`"paste"`）。

事件冒泡由 `ui_widget_emit_event` 自动处理（`lib/ui/src/ui_events.c:512`）：
子 widget 上触发的事件会冒泡到父 widget（除非 `ev.cancel_bubble = true`）。
`event.target` 字段会被自动设置为最初触发的 widget，父级 handler 可以直接
读 `e->target` 获取事件源。

### 复用通用属性系统而非自定义数据槽

普通 widget 状态（`for`、`checked`、`value` 等）直接用通用 attribute 存储：

```c
ui_widget_set_attr(w, "checked", "true");    /* attr API */
ui_widget_get_attr(w, "checked");            /* 返回 const char * */
```

公共 API 用 `static inline` 包装：

```c
static inline int ui_checkbox_set_checked(ui_widget_t *w, int checked)
{ return ui_widget_set_attr(w, "checked", checked ? "true" : "false"); }
```

读取时把字符串比较封装到 getter 里。**不要**为简单状态分配
`ui_widget_add_data(w, proto, sizeof(...))` 数据槽。

例外：text/button 等核心 widget 用 data 槽存复杂状态（pd_text_t 指针、
worker 句柄等），这些是真实资源而不只是字符串 flag。

### CSS 字符串宏

```c
static const char *ui_label_css = css_string(

label { cursor: pointer; }
label:disabled { color: #aaa; }

);
```

`css_string(...)` 在 `lib/css/include/css/types.h:22` 定义为字符串字面量
拼接，编译器优化后零开销。注意：未在 LCUI CSS 引擎注册的值类型（如
`cursor`）会输出 `[css-parser] [property: cursor] value type not defined`
警告但不影响运行。

## Portal 浮层模式

### 为什么需要 Portal

LCUI 渲染管线会**隐式裁剪所有子 widget 到父元素的 padding_box 区域**（通过
`pd_canvas_quote` 和 `ui_rect_overlap` 检查），且没有 `overflow: visible` 属性。
`position: fixed` 也等同于 `absolute`（相对父元素而非视口）。

因此，需要浮出父容器边界的组件（select 下拉面板、dropdown-menu、tooltip、
popover）**不能**作为触发器的子 widget，必须挂载到 `ui_root()` 层级才能不被裁剪。

### 架构：Portal 模块

Portal 模块提供全局浮层容器，供所有需要浮层能力的组件共用：

```
ui_root()
  ├── ... 正常 widget 树 ...
  └── portal-root (单例容器，按需创建)
        ├── SelectContent (position: absolute)
        ├── DropdownMenuContent (未来)
        └── TooltipContent (未来)
```

**portal-root**：按需创建的单例 widget，`ui_create_widget_with_prototype(proto)` 创建，
注册一个简单容器原型，添加 class `"portal-root"`。CSS：
`position: absolute; left: 0; top: 0; width: 100%; height: 100%; z-index: 9999`。

LCUI 中 `position: absolute` 的 `left/top` 始终相对于当前父 widget，
`position: relative` 不会像浏览器语义那样改变子 widget 的定位参考系。因此
portal-root 必须脱离 `ui_root()` 的正常布局流并覆盖整个 root；挂在其下的
浮层 content 仍相对于 portal-root 定位。portal-root 与 ui_root() 保持相同的
原点和尺寸后，Portal 才可以直接把 `ui_widget_get_offset(anchor, ui_root(),
&x, &y)` 得到的 root 坐标写入 content 的 `left/top`，不需要额外做坐标差转换。

**`"portal"` widget 类型**：容器型（parent=NULL），init 时将自身从原父级
`ui_widget_unlink` 再 `ui_widget_append` 到 portal-root。定位属性（side/align/
side-offset/align-offset）通过 widget attribute 存储，update 回调中根据 anchor
widget 的 `ui_widget_get_offset(anchor, ui_root(), &x, &y)` 计算位置。

### 统一转移函数

```c
void ui_portal_transfer(ui_widget_t *content);
```

将 content 从当前父级移动到 portal-root 下，portal-root 不存在时自动创建。
Portal widget 的 init、以及用户手动调用，都通过此函数完成转移。

### 继承 Portal 的组件

需要浮层能力的组件（SelectContent、未来的 DropdownMenuContent 等）将
`"portal"` 设为父原型：

```c
ui_select_content_prototype = ui_create_widget_prototype("select-content", "portal");
```

这样 init 中链式调用 portal 的 init 即可自动完成转移，无需重复实现。

### 多组件通信：直接指针引用

当子组件被 Portal 转移到 portal-root 后，它不再处于原父组件的 widget 子树中。
事件冒泡路径变为 `child → portal → portal-root → ui_root()`，不会经过原父组件。

因此，跨树通信不能用事件冒泡，必须用**直接指针引用**：

- 组件 A 的 init 中，通过 `w->parent` 拿到原父组件 B 的指针
- 将指针存到 A 的私有数据中
- B 也保存 A 的指针（通过 A 在 init 中回调通知 B）
- 运行时通过直接函数调用通信

**缓存引用 vs 按需查找**：只缓存 Portal 转移后会脱离子树的节点引用（如
SelectContent 转移到 portal-root 后，Select 无法通过遍历子节点找到它，必须
缓存 `content` 指针）。仍在当前子树中的节点（如 SelectTrigger 是 Select 的
直接子节点）应**按需查找**，避免缓存引用带来的生命周期管理负担。

```c
/* select-content init 中（transfer 前） */
struct select_content *data = ui_widget_add_data(w, proto, sizeof(*data));
data->select = w->parent;  /* 保存 Select 的引用 */

/* select init 中，被 SelectContent 回调注册 */
select->content = content_widget;

/* trigger 按需查找，不缓存 */
static ui_widget_t *find_trigger(ui_widget_t *select)
{
    ui_widget_t *child;
    int i;
    for (i = 0; (child = ui_widget_get_child(select, i)); i++) {
        if (child->proto == ui_select_trigger_prototype) return child;
    }
    return NULL;
}
```

### 点击外部关闭

通过全局 mousedown 监听器实现：组件打开时在 `ui_root()` 上注册一个 mousedown
handler，检查 `e->target` 的祖先链是否包含浮层内容或触发器，若不在则关闭。
关闭时移除监听器。

```c
static bool is_inside(ui_widget_t *target, ui_widget_t *a, ui_widget_t *b)
{
    while (target) {
        if (target == a || target == b) return true;
        target = target->parent;
    }
    return false;
}
```

## 自绘部件的内容捕获缓存（spinner / shimmer）

自绘部件（`spinner`、`shimmer`）每帧把自己的内容子树 `ui_draw_capture` 到离屏 surface 再做
变换（旋转 / 扫光）。捕获会**重跑整个子树的 `ui_widget_render`**，很贵；但内容通常**帧间不变，
只有变换参数在变**。因此把两类失效拆开：

- `content_dirty`（需要重新捕获）：`init`、`resize`、`set_content`、影响内容外观的样式变化、
  以及**内容子树的任何 mutation**。
- layer 失效（需要重新变换/合成）：动画 tick、角度/相位 setter、`color`、`resize` 等
  （即现有的 `ui_draw_layer_invalidate`）。

`on_paint` 结构：

```c
ui_draw_layer_ensure(layer, cw, ch);
if (ui_draw_layer_needs_render(layer)) {
        /* resize 返回 >0 表示 backing 重建（尺寸或 DPI 变了）→ 也要重捕获 */
        if (ui_draw_surface_resize(content_surface, cw, ch) > 0) {
                content_dirty = true;
        }
        if (content_dirty) {
                ui_draw_capture(content, content_surface);
                content_dirty = false;
        }
        ui_draw_rotate_into(content_surface, display, angle);  /* 每帧照做 */
        ui_draw_layer_mark_rendered(layer);
}
```

失效信号的来源：① 部件自己的钩子（`on_resize` / `set_content` / 颜色同步 /
`UI_TASK_UPDATE_STYLE`）；② **`ui_mutation_observer` 观察自身 + `subtree=true`**（捕获对自定义
内容的样式/属性/子节点编辑；必须观察自身的原因见「Widget destroy 序列」）。

注意：`content_dirty` 置位的地方**必须同时让 layer 失效**（否则 `needs_render` 为 false 会跳过
整段渲染），所以统一走一个 `ui_<widget>_invalidate_content(w)` 辅助函数，内部同时
`content_dirty = true` + `ui_draw_layer_invalidate` + `ui_widget_mark_dirty_rect`。

**已知局限**：observer 只覆盖走 widget 系统的修改。绕过 widget 系统的变化（内容里嵌了另一个
自绘动画、异步字体加载完成等）不会自动失效，需要额外挂事件。

### 失效回归测试要有牙

缓存失效测试必须**在变更后不做样式刷新**（不要调用 `prime_<widget>()` 之类会触发
`UI_TASK_UPDATE_STYLE` 的辅助），否则 `UI_TASK_UPDATE_STYLE` 这条兜底钩子会替 observer 失效，
测试看起来通过其实没测到 observer。正确写法：变更子树后只 `ui_update()` 两次再渲染。
**验证方法**：临时给 `ui_<widget>_observe()` 开头加 `return;`，该测试应当失败。

### 性能场景 / preview / 测试约定

- 共享场景 `tests/scenes/<topic>_perf_scene.{c,h}`：只构造 widget 树 + 样式，不做断言、
  不调用 `lcui_main`。
- 交互 preview `tests/previews/preview_<topic>_perf.c`：注册进 `tests/previews/xmake.lua`
  的 `previews` 列表（target 名由 `topic:gsub("_","-")` 得到，如 `preview-spinner-perf`）；
  帧回调显示 `lcui_get_fps()` 与帧间隔，滚动改变可视数量观察开销。
- headless 测试 `tests/integration/test_<topic>_perf.c`：注册进 `tests/main.c`。headless 下
  用 `clock()` 计时，用 `ui_spinner_set_angle` 之类**显式驱动动画**（不依赖帧时钟），打印
  ms/帧。`ui_widget_render()` 的返回值是**实际渲染的 widget 数**，可用来判断离屏裁剪是否生效。
- 断言要稳健：**不要断言机器相关的绝对耗时阈值**，只断言"每帧有输出"等不变量，其余靠打印。
  测试成本高时把帧数降到最小（如每档 1 帧 + 1 帧预热）。


## MutationObserver

`lib/ui/include/ui/mutation_observer.h` 提供了监听 widget 树变化的 API：

```c
ui_mutation_observer_init_t options = {
    .child_list = true,       /* 监听 children 增删 */
    .subtree = true,          /* 是否包含更深层后代（mutation 沿父链派发） */
    .properties = true,       /* 监听样式/属性变化 */
    .attributes = true        /* 监听 attribute 变化 */
};
observer = ui_mutation_observer_create(callback, callback_arg);
ui_mutation_observer_observe(observer, target, options);
```

要点：
- `ui_mutation_observer_create(cb, arg)` 第二个参数是**回调的 arg**，不是 init
  （写成 `create(cb, &init)` 是错的）；init 是 `observe()` 的第三参数。
- `ui_mutation_observer_disconnect(obs)` 断开全部连接但保留 observer（可再 observe）；
  `ui_mutation_observer_destroy(obs)` 断连并释放（内部会先 disconnect）。
- 回调仅在**有记录**时触发（`ui_process_mutation_observers` 中 `records.length > 0` 才调）。
- 记录是否构造由 `ui_widget_has_observer()` 提前剪枝：没有匹配 option 的 observer 就不生成 record。
- **观察自身 + `subtree=true` 通常优于观察子节点**（生命周期，见「Widget destroy 序列」）。

回调签名：`void callback(ui_mutation_list_t *records, ui_mutation_observer_t *obs, void *arg)`。
遍历 `list_for_each(node, records)` 取 `node->data` 得到 `ui_mutation_record_t *r`，`r->type == UI_MUTATION_RECORD_TYPE_CHILD_LIST` 表示增删事件。

典型用法：容器组件（如 list）在 init 时创建 observer 监听自身 `child_list`，children 变化时重新计算布局或更新标记位。


## `ui_widget_is_visible`

返回 `bool`，用于判断 widget 是否可见（display 非 none、hidden 未设置等）。常用于在 sibling 中过滤当前显示的内容，例如 tab 切换后只取当前 visible 的代码块。

## 事件处理器参数：`e->data` vs `emit_arg`

`ui_event_handler_t` 签名为 `void handler(ui_widget_t *w, ui_event_t *e,
void *emit_arg)`：

- **`e->data`**：注册监听器时传入的 `listener_data` 参数（`ui_widget_on(w, event, handler, listener_data)` 的第四参数）。由 `ui_widget_call_listeners` 在分发前设置 `e.data = listener->data`（`lib/ui/src/ui_events.c:470`）。这是 handler 获取私有数据的标准方式。
- **`emit_arg`**：`ui_widget_emit_event(w, ev, emit_arg)` 的第三参数。大多数 emit 点传 `NULL`，仅在少数显式传入非 NULL 值时才有效。常规监听器不应依赖此参数；它不是 `ui_widget_on()` 的第四参数。

常见陷阱：handler 里误用 `emit_arg`（通常为 NULL）来获取注册时传入的数据，应改用 `e->data`。

硬性规则：`ui_widget_on()` 的第四参数只能通过 `e->data` 获取，不能从 handler 的第三参数获取。handler 的第三参数只有在调用方明确使用 `ui_widget_emit_event()` 的第三参数传递事件发射上下文时才可读取。

## LINK/UNLINK 事件

`ui_widget_on(w, "link", handler, data)` 监听 widget 被链接到父树的事件。关键特性：

- **`cancel_bubble = true`**：LINK/UNLINK 事件不冒泡，只在被操作的那个 widget 上触发。
- **`w->parent` 有效**：handler 执行时 `w->parent` 已指向新父节点（LINK），或仍指向旧父节点（UNLINK 后才 `list_unlink`）。
- **自身监听足够**：对于结构检查（如 `:has()` polyfill），在 LINK 时 self-listen 即可——因为 LINK 时子树已经完整存在（XML 解析或 `ui_widget_append` 都是先建好子树再链接到父）。
- **不需要监听 UNLINK**：子节点移除不在本设计覆盖范围内（CSS `:has()` 不监听子节点 UNLINK，且 UNLINK 后 has-class 自然无需保持）。

## Widget destroy 序列

widget 销毁时先 emit `destroy` 事件（handler 可访问 widget），然后 `list_destroy_without_node` 销毁所有 listeners（每个 listener 的 `destroy_data` 回调释放 `listener->data`）。因此 destroy handler 内可以安全使用 `e->data`（listener 仍存活），但 destroy handler 之后 listener 及其 data 都已释放。

### 跨树缓存的指针必须在 destroy 事件里失效

`ui_widget_destroy()` 是**立即 `free`**，不会把 `state` 标成 `UI_WIDGET_STATE_DELETED`
（只有走 `ui_widget_remove()` 进 trash 的部件才会被标为该状态）。因此**不能**用
`w->state != UI_WIDGET_STATE_DELETED` 判断缓存的 widget 指针是否还活着：指针可能早已
被释放，读到的 `state` 是已释放内存。

持有跨树指针的一方必须同时注册 `destroy` 监听，在目标部件销毁时把指针置 NULL：

```c
ui_widget_on(target, "destroy", on_target_destroy, owner);

static void on_target_destroy(ui_widget_t *w, ui_event_t *e, void *arg)
{
        /* 事件对象的 target 始终是原触发部件；用它确认是目标自身在销毁，
         * 而不是别处 emit 后冒泡上来的事件 */
        if (e->target != w) {
                return;
        }
        /* 把 owner 里缓存的 w 置 NULL */
}
```

`ui_widget_destroy()` 在 emit `destroy` 之前已经把部件从父级 `unlink`，所以常见的销毁路径
不会冒泡到祖先；`e->target == w` 判断是防止其它路径在部件仍挂树时 emit 造成误判。

实际案例：`portal` 的 `scroll_parents` 缓存了祖先 `scrollarea` 指针。切换 locale 时，
外层 router-view 先销毁旧 docs layout（包含 ScrollArea），内层 router-view 已把旧页面
移入 trash；页面里的 dropdown portal 比 scrollarea 活得更久，销毁时 `ui_widget_off`
访问了已释放的 scrollarea。修复方式：绑定滚动父级时同时注册 `destroy` 监听置空数组项，
解绑时把两类监听一起 `ui_widget_off`。

### 子节点先于父节点销毁（影响 mutation observer）

`ui_widget_destroy(w)`（`lib/ui/src/ui_widget.c:46`）的实际顺序是：
`ui_widget_destroy_listeners(w)`（emit destroy 事件）→ `ui_widget_destroy_children(w)`（**递归
销毁所有子节点**）→ `ui_widget_destroy_prototype(w)`（调用自身 prototype 的 destroy 回调）。
即**子节点先被 `free`，父节点自己的 destroy 回调最后才跑**。

后果：若部件在 `on_destroy` 里销毁一个**挂在子节点上**的 `ui_mutation_observer`，此时子节点
已被释放，observer 的连接里 `conn->widget` 是悬挂指针，`ui_mutation_observer_destroy` 会 UAF
崩溃（本仓库 spinner/shimmer 首次实现缓存时踩到，症状是"`ui_spinner_on_init` 里 observe 成功
后立刻段错误"）。

**正确做法**：observer 观察**部件自身**并开 `subtree = true`：

```c
ui_mutation_observer_init_t options = {
        .child_list = true, .subtree = true,
        .properties = true, .attributes = true,
};
observer = ui_mutation_observer_create(on_mutation, w);   /* 第二参是回调参数，不是 init */
ui_mutation_observer_observe(observer, w, options);        /* 观察自身 */
```

`subtree=true` 时 mutation 会沿父链派发到带该选项的祖先（`ui_widget_observer.c:65` 的
`ui_widget_add_mutation_record`），所以观察自身同样收到整个子树的变更；而自身在 destroy 回调
里仍然有效，无悬挂指针。scrollarea / combobox 也是观察自身。


## textinput 编辑内核（stb_textedit）

`textinput` 的编辑状态由 `src/widgets/textedit.c` 封装 `stb_textedit.h` v1.16：

- canonical 文本是唯一事实来源（`wchar_t`，`\r\n`/`\r` 归一化为 `\n`）；
- `pd_text` 只是投影：每次编辑后 `pd_text_clear + pd_text_append` 重建，再
  `pd_text_update`，保证 stb 的后续布局查询看到最新行结构；
- 索引映射：软换行不消耗字符，硬换行（`eol != PD_TEXT_EOL_NONE`）消耗 1 个字符；
  视觉行信息来自 `pd_text_get_line_eol` / `pd_text_get_line_char_start`；
- `ui_textinput_update_caret` 必须使用文本层缓存的 `layer_scale`，不能用事件期的
  `ui_get_actual_scale()`（事件分发发生在 `ui_server_update()` 之外，全局 dpi 可能
  仍是默认值）；
- 选区由 textinput 在 `on_paint` 中先于 `pd_text_render_to` 绘制；
- 选区颜色默认是实色 `primary`，文字用 `primary-foreground`（亮色主题白字、
  暗色主题深字）；可用 `selection-color` / `selection-text-color` 属性或
  `ui_textinput_set_selection_color()` / `ui_textinput_set_selection_text_color()`；
  也支持 CSS 规则 `textinput .selection { background-color: ...; color: ...; }`。
  textinput 内部维护一个 0×0、绝对定位的 `.selection` 隐藏子部件，只用于承载
  这些样式，绘制时读取它的 computed `background-color` / `color`。属性/API 会写入
  该元素的 inline 样式，优先级高于 CSS 规则；两者都没有时回退到默认值；
- 编辑场景不支持样式标签（canonical 索引会被 pd_text 消费）；Windows 下通过
  `GETPREV/NEXTCHARINDEX` 跳过 UTF-16 代理对；
- 当前重建是 O(N)，定位为表单级文本（KB 级）；GB 级文档需要 rope/piece table +
  虚拟化排版，不能沿用该实现。

测试：`tests/integration/test_textinput.c`、`test_textinput_edit.c`；
可视诊断：`tests/previews/preview_textinput_edit.c`。