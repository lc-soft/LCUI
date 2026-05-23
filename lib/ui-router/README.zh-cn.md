# libui-router

([English](README.md)/**中文**)

`libui-router` 是连接 [librouter](../router/README.zh-cn.md) 与 [libui](../ui/) 部件系统的 UI 绑定层。

它提供两个部件：

- `router-link` —— 可点击触发路由跳转的部件，对应 Vue Router 的 `<router-link>`。会根据当前路由切换 `router-link-active` 和 `router-link-exact-active` 类名。
- `router-view` —— 路由出口，渲染当前路由匹配到的组件。

## 用法

```c
#include <ui.h>
#include <ui_router.h>

ui_init();
ui_register_router_link();
ui_register_router_view();

/* 此后 "router-link" 与 "router-view" 部件类型可用。 */
```

在使用完整的 LCUI 时，`lcui_init()` 会通过 `lcui_widgets_init()` 自动调用上述注册函数；只有在脱离 LCUI 顶层壳直接嵌入 `libui-router` 时才需要手动调用。

## 许可

代码基于 [MIT 许可协议](../../LICENSE.TXT) 发布。
