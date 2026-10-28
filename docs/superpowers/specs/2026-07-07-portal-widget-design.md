# Portal 浮层模块设计

## 概述

新增 portal 浮层模块，作为 LCUI 共享浮层基础设施。Portal 本体保留在 owner 的 widget 树中，持有并管理一个挂载到 portal-root（`ui_root()` 层级）的外树 content。后续组件（select、dropdown-menu、tooltip、popover 等）复用这套 owner/content 生命周期，从而让浮层脱离父级裁剪，同时随 owner 自动销毁。

## 调研结论

### shadcn/ui 浮层架构

shadcn 的 select、dropdown-menu、tooltip、popover 共用同一模式：

- **Portal**：将浮层 DOM 挂载到 `document.body`，逃脱父级裁剪
- **定位引擎**：底层依赖 floating-ui 计算位置
- 两套实现路线：Radix UI（Portal → Content）和 Base UI（Portal → Positioner → Popup）

### LCUI 渲染约束

- LCUI 渲染管线**隐式裁剪所有子 widget 到父元素的 padding_box 区域**（无 `overflow: visible`）
- `position: fixed` 等同于 `absolute`（相对父元素，非视口）
- 无任何浮层/弹出层基础设施
- 因此 select 下拉面板、dropdown-menu、tooltip 等必须脱离原父级才能不被裁剪

### 策略选择

新增 `"portal"` widget 类型作为共享浮层基础设施。Portal 是留在原树中的逻辑 owner，浮层组件仍使用自己的类型（如 `select-content`），并通过 `ui_portal_set_content()` 交给 Portal 挂载到 portal-root。Portal 与 content 使用组合而非 prototype 继承。

## 实施计划

### 阶段 1：Portal 浮层模块

#### 1.1 新增文件

| 文件 | 说明 |
|------|------|
| `include/LCUI/widgets/portal.h` | Portal 公共 API |
| `src/widgets/portal.c` | Portal 实现 |

#### 1.2 Portal-root 单例

- 按需创建：首个 Portal 挂载 content 时检测 portal-root 是否存在，不存在则创建
- 创建方式：`ui_create_widget_with_prototype(ui_portal_root_proto)`，注册一个简单容器原型
- 添加 class `"portal-root"`
- 挂载到 `ui_root()` 下
- 运行期创建后常驻；`ui_destroy()` 销毁 ui_root 时随根节点销毁
- CSS：`position: absolute; left: 0; top: 0; width: 100%; height: 100%; z-index: 9999`
- `portal-root` 脱离 `ui_root()` 的正常布局流，覆盖整个 root；content 的 `position: absolute` 仍相对于 `portal-root` 定位。由于 root 从 `(0, 0)` 开始且尺寸与 `ui_root()` 一致，Portal 可直接将 anchor 相对于 `ui_root()` 的 offset 写入 content 的 `left/top`，无需额外计算 root 间坐标差。
- 不设置 `pointer-events: none`。portal-root 需要接收 root 空白区域的 pointer event，统一为 Select、dropdown-menu、popover 等浮层提供 outside-close 分发；content 默认使用 `pointer-events: auto`，保证自身及其后代可交互。

#### 1.3 `"portal"` widget 类型

- parent prototype = NULL（逻辑 owner，不承担浮层绘制）
- Portal 本体始终保留在原父级中，因此 owner 销毁时会随真实 widget 子树一起销毁
- 私有数据保存 `content` 与 `anchor`；content 是逻辑上由 Portal 拥有、结构上挂在 portal-root 下的 widget
- Portal 的 update/build 阶段扫描直接子节点；声明式 `<Portal><Content /></Portal>` 首次更新时，自动调用 `ui_portal_set_content()` 将该直接子节点转移到 portal-root。不能依赖父级 link handler，因为 LCUI 的 link 事件不冒泡
- open/close 修改 content 的 display，不修改 Portal 本体
- 定位属性（通过 widget attribute 存储）：

| 属性 | 值 | 默认 | 说明 |
|------|-----|------|------|
| `anchor` | widget 指针（内部用） | — | 定位参考 widget |
| `side` | `"bottom"` / `"top"` / `"left"` / `"right"` | `"bottom"` | 浮层出现方位 |
| `side-offset` | 数字字符串 | `"0"` | 距锚点像素间距 |
| `align` | `"start"` / `"center"` / `"end"` | `"center"` | 对齐方式 |
| `align-offset` | 数字字符串 | `"0"` | 对齐偏移 |

- update：每帧根据 anchor 的 `ui_widget_get_offset(anchor, ui_root(), &x, &y)` + side/align 参数，计算相对于 root 的 content `left/top` 样式值并设置。由于 portal-root 覆盖 ui_root 且两者原点一致，直接使用 root 坐标，不重复累加或转换 portal-root 坐标

#### 1.4 Content 挂载与所有权

```c
void ui_portal_set_content(ui_widget_t *portal, ui_widget_t *content);
ui_widget_t *ui_portal_get_content(ui_widget_t *portal);
```

- `ui_portal_set_content()` 将 content 从当前父级移动到 portal-root，并建立 `portal -> content` 的唯一所有权关系
- Portal 的声明式子节点扫描与 C API 共用 `ui_portal_set_content()`；因此 lcui/react 将 SelectContent 展开为 `portal + content` 后，无需保存 Select 指针
- 同一个 Portal 只拥有一个 content；替换 content 时先结束旧 content 的生命周期，再挂载新 content
- content 挂载后注册 destroy listener；content 先销毁时，该 listener 将 Portal 缓存的 content 指针置 NULL
- Portal 的 prototype destroy 先解除 content 的 destroy listener，再清空缓存；content 尚未进入 trash 时调用 `ui_widget_remove(content)`
- content 已为 `UI_WIDGET_STATE_DELETED` 时不得再次 remove，也不得直接 destroy，避免重复 remove 断言或 trash 双重销毁
- `ui_portal_get_content()` 在 content 已销毁后返回 NULL

生命周期顺序：

```text
owner 销毁
  → 递归销毁真实子节点 portal
  → portal prototype destroy
      → off(content, "destroy", ...)
      → portal->content = NULL
      → content 未进入 trash：ui_widget_remove(content)
  → content 稍后由 ui_clear_trash() 统一销毁

content 先销毁
  → content destroy listener
      → portal->content = NULL
  → portal 后续销毁时不再操作 content
```

#### 1.5 跨树事件转发

content 被挂到 portal-root 后，原生事件的物理冒泡路径不再经过 Portal 本体及其 owner。Portal 模块在 portal-root 上集中监听需要转发的交互事件，并维护 `content -> Portal` 映射：

```text
SelectItem click
  → SelectContent
  → portal-root 的 click 转发器
      → 从 event.target 沿真实父链找到最近的已注册 content
      → 查映射得到对应 Portal
      → original_event.cancel_bubble = true
      → copied_event.target 保留原 SelectItem
      → copied_event.cancel_bubble = false
      → ui_widget_emit_event(portal, copied_event, NULL)
  → Portal
  → Select
  → Select 的真实祖先
```

- LCUI 事件系统不支持通配监听；第一阶段只注册 Select 所需的 `click`，后续组件按实际需求扩充事件表
- 转发保留原始 `event.target`，因此 Select 仍能识别被点击的 SelectItem
- 转发前必须阻止原事件继续从 portal-root 冒泡，否则 `ui_root()` 等共同祖先会分别收到物理路径和逻辑路径两次事件
- 结构和生命周期事件（`link`、`unlink`、`destroy`、`ready`、`afterlayout`）不转发
- 嵌套 Portal 按 `event.target` 向上遇到的第一个已注册 content 处理，即最近 owner 优先
- content 被替换或销毁时同步删除映射；Portal 销毁时先删除映射，再解除 destroy listener 和 remove content
- portal-root 的 click/mousedown listener 先根据 event.target 判断命中的 content；命中 content 时只执行对应 Portal 的逻辑转发，命中 portal-root 空白区域时向已打开的 Portal 分发 outside event；不允许通过 `pointer-events: none` 绕过 root，否则 root 无法观察空白区域点击

#### 1.6 Portal C API

```c
ui_widget_t *ui_create_portal(void);
void ui_portal_set_content(ui_widget_t *portal, ui_widget_t *content);
ui_widget_t *ui_portal_get_content(ui_widget_t *portal);
void ui_portal_set_anchor(ui_widget_t *portal, ui_widget_t *anchor);
void ui_portal_set_side(ui_widget_t *portal, const char *side);
void ui_portal_set_side_offset(ui_widget_t *portal, int offset);
void ui_portal_set_align(ui_widget_t *portal, const char *align);
void ui_portal_set_align_offset(ui_widget_t *portal, int offset);
void ui_portal_open(ui_widget_t *portal);
void ui_portal_close(ui_widget_t *portal);
ui_widget_t *ui_get_portal_root(void);
```

#### 1.7 注册

- `include/LCUI/widgets.h` 追加 `#include "widgets/portal.h"`
- `src/lcui_widgets.c::lcui_widgets_init()` 追加 `ui_register_portal();`

### 阶段 2：测试

#### 2.1 新增文件

| 文件 | 说明 |
|------|------|
| `tests/integration/test_portal.c` | Portal 模块测试 |

#### 2.2 测试场景

- portal-root 按需懒创建：首个 Portal 设置 content 时创建，挂载到 ui_root()
- Portal 本体保留在 owner 树中，content 挂载到 portal-root
- portal open/close：content 的 `display: block` ↔ `display: none`
- content 挂载幂等：重复设置同一个 content 不重复挂载或创建 portal-root
- 声明式挂载：content 先作为 Portal 直接子节点 append，Portal 首次 update 时自动转移并建立映射
- anchor + side + align：每帧根据 anchor offset 计算 left/top
- click 转发：content 后代的 click 保留原 target，从 Portal 本体重新冒泡到 owner
- outside click/mousedown：portal-root 空白区域事件可被当前打开的 Portal 观察并关闭自身，不影响 content 内部交互
- click 单次分发：原物理路径在 portal-root 处停止，ui_root 不会收到重复事件
- 嵌套 Portal：最近 content owner 获得转发事件
- Portal 先销毁：安全解除 listener，并将未删除的 content 加入 trash
- content 先销毁：Portal 的 content 指针自动清空，getter 返回 NULL
- content 已进入 trash、尚未实际销毁时再销毁 Portal：不重复 remove，不访问已释放数据
- ui_root 销毁时，无论 portal-root 与 owner 子树的销毁顺序如何都不会产生悬空引用

#### 2.3 注册

- `tests/main.c` 的 `suites[]` 表追加 `{ "portal", test_portal }`
- `tests/integration/main.c`（如存在）同步追加

## 文件清单

### 新增

```
include/LCUI/widgets/portal.h
src/widgets/portal.c
tests/integration/test_portal.c
```

### 修改

```
include/LCUI/widgets.h         ← + portal.h
src/lcui_widgets.c             ← + ui_register_portal
tests/main.c                   ← + test_portal suite
tests/integration/main.c       ← + test_portal suite（如存在）
```

## 设计决策记录

| 决策 | 选择 | 原因 |
|------|------|------|
| 浮层架构 | 先建共享 portal 模块 | select、dropdown-menu、tooltip、popover 等都需要浮层能力，一次投入后续零成本 |
| Portal 形式 | widget 类型 | 可被 JSX 声明式使用，与 shadcn 模式一致 |
| Portal 结构 | owner/content 组合 | Portal 本体保留在原树中，外树 content 由 Portal 管理；owner 销毁时可自动清理浮层 |
| Content 所有权 | Portal 是唯一 owner | Portal data 是 content 生命周期的唯一真源，使用 destroy listener 处理 content-first 路径 |
| 跨树事件 | portal-root 集中转发 | 保留原 target，从 Portal 本体重新发射，使事件沿逻辑 owner 树冒泡且避免组件保存跨树 owner 指针 |
| Portal-root 生命周期 | 按需懒创建 | 避免不需要浮层时创建无用 widget |
| Portal-root 创建方式 | `ui_create_widget_with_prototype` | 拥有独立原型，CSS 集中管理 |

## 复用组件

依据本 spec 已实现的 portal，可被以下组件复用：

- **select**：select 持有 Portal 本体，Portal 拥有外树 `select-content`（详见 select 部件 spec）
- **dropdown-menu**：dropdown-menu 持有 Portal，Portal 拥有 `dropdown-menu-content`
- **tooltip**：tooltip 持有 Portal，Portal 拥有 `tooltip-content`
- **popover**：popover 持有 Portal，Portal 拥有 `popover-content`
