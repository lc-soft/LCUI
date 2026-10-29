# 头文件结构与设置模块

> 公共头文件的聚合规则、settings 模块分层、window state 绑定机制。

## 头文件结构

```
include/LCUI.h                  ← 顶层聚合头
include/LCUI/settings.h         ← 聚合 settings/base.h + rendering.h + window_state.h
include/LCUI/settings/base.h    ← load/flush/unload/is_loaded（不再暴露 listener）
include/LCUI/settings/rendering.h
include/LCUI/settings/window_state.h
src/lcui_settings.h             ← 内部头（lcui_settings_module、deserialize/serialize 回调、ini_doc_t*）
src/setting_field.h             ← DEFINE_SETTING_INT_FIELD / DEFINE_SETTING_BOOL_FIELD 宏 + setting_field_def_t
src/ini_doc.h                   ← 通用 INI 文档
```

### include 规则：只 include 聚合头

`<LCUI.h>` 是顶层聚合头，**已间接包含**下列子头（见
`include/LCUI.h:15-30`）。已 include `<LCUI.h>` 的源文件**禁止**再
include 它的任一子头，避免重复包含与维护错配：

| 已被 LCUI.h 间接包含 | 不要再写 |
|----------------------|----------|
| `<yutil.h>` | `#include <yutil.h>` |
| `<LCUI/settings.h>`、`<LCUI/settings/base.h>` | `#include <LCUI/settings.h>` / `…/base.h` |
| `<LCUI/app.h>`、`<LCUI/ui.h>`、`<LCUI/widgets.h>` | 对应子头 |
| `<LCUI/base.h>` | `#include <LCUI/base.h>` |
| `<css.h>`、`<pandagl.h>`、`<thread.h>`、`<worker.h>`、`<ptk.h>` | 对应子头 |

正确写法：

```c
#include <LCUI.h>          /* 一行聚合所有 LCUI 公共 API */
#include <stdio.h>          /* 标准库照常 */
#include <i18n.h>           /* 外部独立 lib 头照常 */
#include "locale.h"         /* 本地头照常 */
```

错误写法（不要这样做）：

```c
#include <LCUI.h>
#include <yutil.h>          /* 冗余：LCUI.h 已包含 */
#include <LCUI/settings/base.h>  /* 冗余：LCUI.h → LCUI/settings.h → base.h */
```

> 例外：lib 内部文件（如 `lib/ptk/src/...`、`lib/pandagl/src/...`）按
> 模块依赖单独 include `<yutil.h>` 等子头，**不**依赖聚合头。这条规则
> 只约束顶层应用代码（`app/`、`examples/`、`tests/`）。

### Demo 与示例代码同样只用聚合头

`docs/examples/<name>{,-xml,-tsx}/main.c` 是 doc-viewer 展示的示例代码，
也会被 page-generator 原样渲染到文档页面的代码块里。它们必须遵守同
样的聚合头规则——**只**写 `#include <LCUI.h>`，不要附带
`#include <LCUI/widgets.h>` / `<ptk.h>` / `<ui_xml.h>` 等。冗余的
include 不仅代码冗余，还会在生成的代码块中显式展示出来，对读者误导。

### Settings 模块架构

`lcui_settings.c` 是**基础模块**（infrastructure），提供：
- `ini_doc_t` 生命周期（`ini_doc_load` / `ini_doc_save`）
- 两类回调列表：`deserialize`（load 后读 doc → 更新内存）、`serialize`（flush 前写内存 → doc）
- dirty 标记（deserialize 或任何 setter 都可以置位）

具体模块（`lcui_rendering.c`、`lcui_window_state.c`）**依赖** `lcui_settings.c` 并在自己的 init 里注册回调。**`lcui_settings.c` 不直接依赖它们**。`unload()` 不清空回调列表，由具体模块的 destroy 清理。

`setting_field.h` 提供 schema 驱动序列化：
```c
#define DEFINE_SETTING_INT_FIELD(struct_type, name, def, min, max)
#define DEFINE_SETTING_BOOL_FIELD(struct_type, name, def)
```
schema 包含字段名、类型、`offsetof`、默认值、有效范围。`setting_fields_deserialize/serialize` 在缺省/越界时自动回填默认值并置 dirty。

### Window State 绑定

`lcui_bind_window_state(widget, id)` 在首次调用时内联注册 `PTK_EVENT_CREATE` 和 `PTK_EVENT_CLOSE` 全局监听，记录 `(widget, id)` 绑定，并在绑定时从已加载的配置恢复 widget 尺寸（`ui_widget_resize`）。如果当前 widget 已经绑定了窗口（通过 `ui_server_get_widget` 检查），立即应用状态。

- **CREATE 处理器**：通过 `ui_server_get_widget(e->window)` 找绑定的 widget → 找到 binding → 应用状态。
- **CLOSE 处理器**：通过 `e->window` 找 binding → 同步状态到 entry 列表 → **保留 binding（不清 `widget` 指针）**（因为 widget 在模式切换后仍存活，只是窗口被销毁/重建）。
- **Widget 销毁处理器**：仅当 widget 真正销毁时才移除 binding。

这使得显式模式切换（`lcui_ui_set_display`）正常工作：旧窗口关闭时存储状态；新窗口创建并通过 `ui_server_connect` 重连到同一 widget 时，CREATE 处理器自动把状态应用到新窗口。


