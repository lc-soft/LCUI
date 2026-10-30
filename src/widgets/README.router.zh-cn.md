# Router 部件

([English](README.router.md)/**中文**)

Router 部件把 [librouter](../../lib/router/README.zh-cn.md) 连接到
[libui](../../lib/ui/) 部件系统。

它们属于 LCUI 内核，由 `lcui_init()` 自动注册。该模块提供两个部件：

- `router-link` — 可点击触发路由跳转的部件，对应 Vue Router 的 `<router-link>`。会根据当前路由切换 `router-link-active` 和 `router-link-exact-active` 类名。
- `router-view` — 路由出口，渲染当前路由匹配到的组件。

## 用法

```c
#include <LCUI.h>

lcui_init();

/* 此后 "router-link" 和 "router-view" 部件类型可用。 */
```

在脱离 LCUI 顶层壳、只嵌入 `libui` 时，请包含
`<LCUI/widgets/router.h>` 并手动调用 `ui_register_router_link()` 和
`ui_register_router_view()`。

## 许可

代码基于 [MIT 许可协议](../../LICENSE.TXT) 发布。
