# Breadcrumb 组件设计

## 目标

为 LCUI 增加与 shadcn `Breadcrumb` 对齐的组合式面包屑部件族，并提供 C 端构建 API：

- `append_item(key, label)` 追加标准节点，自动选择 link/page
- `append(widget)` 追加调用方自定义的 widget
- 传入 `/path/to/file` 路径时自动生成节点
- 路径节点标签由调用方通过回调解析，不直接展示原始路径段
- 溢出时按指定起点折叠中间节点，折叠项进入 MoreHorizontal 下拉菜单
- 点击标准节点或折叠项时，通过 `change` 回传节点 key

## 依赖

- 省略号下拉复用 `dropdown-menu` 及其 Portal 生命周期
- 默认分隔符、MoreHorizontal 图标使用 Fluent Icons
- 节点文本使用 `text` 部件承载
- 不新增独立浮层实现，不复制 DropdownMenu 的开关、外部点击和键盘逻辑

## 范围

### 本版实现

- `breadcrumb`
- `breadcrumb-list`
- `breadcrumb-item`
- `breadcrumb-link`
- `breadcrumb-page`
- `breadcrumb-separator`
- `breadcrumb-ellipsis`
- `ui_breadcrumb_append_item()`
- `ui_breadcrumb_append()`
- 路径模式：`path` 属性 / `ui_breadcrumb_build_from_path()`
- 标签回调 `ui_breadcrumb_label_provider_t`
- 自定义 item 的 `data-key` / `data-label` 元数据
- `collapse-from` 折叠起点与响应式折叠/展开
- 省略号 DropdownMenu，包含全部已折叠节点
- `change` 事件，携带节点 key
- 中英文文档、字段表、C/XML/TSX 示例
- 集成测试与预览可视化验证

### 明确暂缓

- 路径百分号解码、URI 语义解析
- 异步标签加载
- 节点间键盘方向键漫游与 roving tabindex
- 折叠/展开动画
- Breadcrumb 内建横向 ScrollArea（外层容器自行提供）
- 节点切换菜单的专用 C API；需要时在 item 内组合 DropdownMenu

## CSS 约束

LCUI CSS 不支持属性选择器、功能伪类和子代选择器，因此：

- 折叠状态使用 item 的 `collapsed` status，规则写在 `breadcrumb-item:collapsed`
- 不依赖 `[data-path]` 参与 CSS 选择器；`data-*` 只用于 C 侧行为查找
- 不使用 `:has()`、`:not()`、`:nth-child()`、`>`、`+`、`~`
- 单行布局通过 flex + `white-space: nowrap` 实现，不使用 CSS grid

## 结构

```text
breadcrumb
└── breadcrumb-list
    ├── breadcrumb-item
    │   ├── breadcrumb-link 或 breadcrumb-page
    │   └── breadcrumb-separator
    ├── breadcrumb-item（折叠时）
    │   └── breadcrumb-ellipsis
    │       └── dropdown-menu
    │           ├── dropdown-menu-trigger
    │           └── dropdown-menu-content（Portal）
    │               └── dropdown-menu-item × N
    └── ...
```

- 每个非最后 item 自带一个分隔符；最后一项不带分隔符
- 折叠时，被隐藏 item 及其分隔符一起隐藏
- ellipsis 作为一个中间 item 插入，并自带一个分隔符
- 标准追加模式下，最后一个 item 使用 `breadcrumb-page`，其余使用 `breadcrumb-link`

## 公共 API

### 工厂

```c
ui_widget_t *ui_create_breadcrumb(void);
ui_widget_t *ui_create_breadcrumb_list(void);
ui_widget_t *ui_create_breadcrumb_item(void);
ui_widget_t *ui_create_breadcrumb_link(void);
ui_widget_t *ui_create_breadcrumb_page(void);
ui_widget_t *ui_create_breadcrumb_separator(void);
ui_widget_t *ui_create_breadcrumb_ellipsis(void);
```

`ui_create_breadcrumb()` 默认创建根节点和空的 `breadcrumb-list`。

### 追加节点

```c
void ui_breadcrumb_append_item(ui_widget_t *breadcrumb,
                               const char *key, const char *label);
void ui_breadcrumb_append(ui_widget_t *breadcrumb,
                          ui_widget_t *content);
```

- `append_item()` 创建并追加一个标准 item，不返回节点对象
  - `key` 必填，只表示节点标识/名称（如 `"b"`、`"file"`），不是完整路径
  - `label` 为 NULL 时使用 key
  - 当前最后一个 item 使用 `breadcrumb-page`；再追加新 item 时，前一个 page 自动转成 link，key/label 保留
  - 自动创建/维护分隔符
- `append()` 追加调用方提供的 widget
  - 如果传入的已经是 `breadcrumb-item`，直接使用
  - 否则把它包进新的 `breadcrumb-item`
  - 自动创建/维护分隔符
  - 自定义内容不自动触发 `change`；点击行为由调用方内容自己处理
- 自定义内容需要在折叠下拉和事件中携带元数据时，在 **append 之前** 给内容 widget 设置 `data-key` / `data-label`
- append 模式由 Breadcrumb 接管最后项状态和分隔符；调用方传入的 item 不应自带 separator
- append API 与显式声明模式不要混用；调用 append 后由 Breadcrumb 管理整个列表

### 自定义 item 元数据

- 标准 item 的 key/label 只来自 `append_item()` 参数
- 自定义 `append()` 内容的 key 取内容 widget 的 `data-key`
- 折叠下拉标签优先取 `data-label`，其次尝试内容文本，最后回退到 `data-key` / key；都没有时使用空串
- `data-key` / `data-label` 在构建折叠下拉时读取；修改后下次折叠变化或重建生效

### 路径与折叠

```c
typedef const char *(*ui_breadcrumb_label_provider_t)(
    const char *segment, const char *path, void *arg);

void ui_breadcrumb_build_from_path(ui_widget_t *w, const char *path,
                                   ui_breadcrumb_label_provider_t provider,
                                   void *arg);
const char *ui_breadcrumb_get_key(ui_widget_t *w);
void ui_breadcrumb_set_collapse_from(ui_widget_t *w, int index);
int ui_breadcrumb_get_collapse_from(ui_widget_t *w);
void ui_breadcrumb_set_separator(ui_widget_t *w, const char *icon_name);
```

- `build_from_path()` 清空当前节点，再按路径逐项调用 `append_item()`
- provider 只在本次构建中调用；传 NULL 时标签使用 segment 原文
- 标签需要更新时重新调用 `build_from_path()`，或改用 append API 重建
- `collapse-from` 默认 `1`
- `set_collapse_from()` 在节点已生成时将值限制在 `[0, node_count - 2]`；节点数小于 2 时不折叠
- 节点尚未生成时，setter 保存原值，下次构建时再按节点数收紧
- `get_key()` 返回最后一个有 key 的节点的 key；没有时返回空串
- key 是节点标识，不是路径；不同节点允许使用相同 key，调用方通过自身状态区分
- `set_separator()` 只影响自动生成的图标分隔符，默认 `chevron-right`

### 属性

| 属性 | 作用 | 默认值 |
| --- | --- | --- |
| `path` | 路径模式输入 | - |
| `collapse-from` | 折叠起点，0-based | `1` |
| `separator` | 自动分隔符图标名 | `chevron-right` |
| `data-key` | 自定义 append 内容的节点 key | - |
| `data-path` | 显式 item 的节点 key | - |
| `data-label` | 自定义 item 的折叠下拉标签 | - |

## 路径与标签模型

### 解析规则

- 按 `/` 切分，忽略空段；`//a//b/` 得到 `a`、`b`
- 节点路径逐级拼接，始终带前导 `/`：`/a`、`/a/b`
- 相对路径 `a/b` 与 `/a/b` 解析结果相同；节点路径仍带前导 `/`
- `/` 生成一个单独节点，标签 `/`，作为当前页，不可点击
- 空字符串或 NULL 生成空面包屑，不显示任何 item
- 路径模式构建的节点：key 为当前 segment（如 `b`）
- 完整节点路径只用于 provider 的 `path` 参数，不作为 key 暴露给事件

### 标签

- provider 对每个节点调用一次：`(segment, node_path, arg)`
  - `segment` 是原始路径段
  - `node_path` 是该节点的完整路径，例如 `/a/b`
- provider 返回 NULL 或空串时回退为 segment 原文
- provider 返回值由 widget 立即复制
- provider 是 `build_from_path()` 的参数，不保存为持久状态
- XML/TSX 的 `path` 属性没有 provider，标签回退为 segment；需要自定义标签时使用显式 item 或 C API
- 路径模式最后一项使用 `breadcrumb-page`，不可点击；其余使用 `breadcrumb-link`
- 显式模式不调用 provider；标签取 item 内文本，key 取 `data-path`

## 事件

- 事件名：`change`，从 `breadcrumb` 根节点发出
- 参数：通过 `emit_arg` 传入 `const char *key`
- 触发来源：
  - 标准 `breadcrumb-link` 点击
  - 省略号下拉项点击
- 不触发：
  - `breadcrumb-page` 点击
  - `disabled` item
  - 自定义 `append()` 内容；其交互由调用方处理
- `e->data` 仍是 `ui_widget_on()` 绑定的监听器上下文
- key 字符串由 widget 持有，仅在本次分发期间有效；监听器需要长期保存时必须复制
- 点击后不自改节点模型、不重建自身、不自动导航

```c
static void on_change(ui_widget_t *w, ui_event_t *e, void *arg)
{
        const char *key = arg;

        /* copy key if it must outlive this dispatch */
}

ui_widget_on(breadcrumb, "change", on_change, app_state);
```

## 显式模式

- 未调用 `build_from_path()` 且未使用 append API 时，使用调用方声明的 `breadcrumb-item` 子节点
- `breadcrumb-list` 缺失时，根节点自动创建一个，并把直接放在 `breadcrumb` 下的 `breadcrumb-item` 移入该 list
- 每个 `breadcrumb-item` 可通过 `data-path` 提供 key
- 有 `data-path` 的 item 可点击并发出 change，key 取 `data-path`；没有 `data-path` 的 item 在省略号下拉中显示为 disabled
- 显式 item 的标签优先 `data-label`，否则取第一个 `breadcrumb-link` / `breadcrumb-page` 的文本
- 显式模式不自动插入分隔符；分隔符由调用方声明
- 折叠算法同样作用于显式 item

## 溢出折叠算法

### 测量

- `breadcrumb-list` 单行排列，item 和分隔符均不收缩
- 容器可用宽度取 `breadcrumb` 内容盒宽度
- 需要折叠时，先恢复全部 item 可见，测量每个 item 的外宽，再决定隐藏范围
- ellipsis 的外宽单独测量或缓存，参与是否容纳的判断
- 缓存测量结果；只有以下情况重新测量：
  - 节点模型、标签、折叠起点或分隔符变化
  - `breadcrumb` 内容盒宽度变化
  - 节点数量变化
- 避免因为隐藏 item 导致的宽度变化反复触发计算

### 折叠步骤

1. 全部 item 可见，总宽不超过容器：不显示 ellipsis
2. 总宽超限：从 `collapse_from` 开始尝试隐藏，优先只隐藏最少数量的中间节点，使尽可能多的尾部节点保持可见
3. 仍超限：逐步扩大隐藏范围（向右增加隐藏项），直到容纳
4. 隐藏范围到达倒数第二项后仍超限：向左扩展隐藏范围，第一项也可被折叠；此时 ellipsis 位于最左侧
5. 最后一项永不隐藏
6. 即使只剩 ellipsis + 最后一项仍超宽，也不再隐藏；内容溢出交给外层横向滚动

### 响应式行为

- 容器变宽时从全部可见状态重新计算，已折叠项自动展开
- 容器变窄时重新计算折叠起点
- 折叠状态通过 item 的 `collapsed` status 表达
- 不改变 item 的顺序；ellipsis 固定放在被隐藏范围的起始位置

### 省略号

- `breadcrumb-ellipsis` 是 MoreHorizontal 图标按钮
- 内部持有 DropdownMenu：
  - trigger：ellipsis 按钮
  - content：Portal，`side="bottom"`、`align="start"`
  - item：每个被折叠节点一个
- 下拉 item 标签与面包屑节点标签一致；key 使用节点 key
- 点击下拉 item：关闭菜单，从 `breadcrumb` 根节点发出 `change`
- 隐藏范围变化后重建下拉 item
- 下拉菜单的开关、外部关闭、Esc 和键盘行为由 DropdownMenu 提供
- ellipsis 销毁时连同内部 DropdownMenu 和 Portal 一起销毁

## 样式

结构规则放在 `src/widgets/breadcrumb.c`，主题规则放在 `src/styles/style-nova.css`。

- `breadcrumb`：`display:block; min-width:0`
- `breadcrumb-list`：`display:flex; flex-direction:row; align-items:center; gap:6px; white-space:nowrap; min-width:0`
- `breadcrumb-item`：`display:inline-flex; align-items:center; gap:6px; flex-shrink:0`
- `breadcrumb-link`：muted 前景色，hover 恢复 foreground，`cursor:pointer`
- `breadcrumb-page`：foreground 色，不可点击
- `breadcrumb-separator`：默认 `chevron-right`，14px，muted
- `breadcrumb-ellipsis`：24×24，透明背景，圆角 4px，hover 使用 accent
- `breadcrumb-item:collapsed`：`display:none`

## 示例与文档

- 顶部默认示例：`breadcrumb-demo`，展示路径模式基础用法
- 示例小节：
  - 自定义标签回调
  - `append_item()` 基础用法
  - 自定义 `append(widget)` 与 `data-*` 元数据
  - 自定义 widget（含 DropdownMenu 切换菜单）
  - 溢出折叠与省略号下拉
  - 显式声明 item 与 `data-path`
- 字段表：Breadcrumb、BreadcrumbList、BreadcrumbItem、BreadcrumbLink、BreadcrumbPage、BreadcrumbSeparator、BreadcrumbEllipsis
- 中文文档定稿后再同步英文
- C/XML/TSX 三种变体，示例样式写入 `examples/doc-viewer/app/global.css`

## 测试

### 单元/集成

- 路径解析：`/`、`/a`、`/a/b/`、`//a//b`、空串、相对路径
- provider 的 `node_path` 拼接：`/a`、`/a/b`、`/a/b/c`
- 路径模式的节点 key 取对应 segment；append_item 的 key 只是节点名，不要求路径前缀
- provider 调用次数、参数、NULL/空串回退、provider 为 NULL 时使用 segment
- 重复调用 `build_from_path()` 会重新解析并重新调用 provider
- 路径模式生成 link/page，最后一项不可点击
- append：
  - `append_item()` 构建的 item key/label 正确
  - 追加新项后前一个 page 转成 link，key/label 保留
  - `append()` 包装普通 widget，并把它加入折叠模型
  - 两种 append 都自动维护分隔符
  - 自定义内容上的 `data-key` / `data-label` 正确进入折叠下拉
- 事件：
  - link 点击发出对应节点 key
  - page 点击不发声
  - disabled item 不发声
  - 自定义 append 内容不自动发声
- 折叠：
  - 宽容器不折叠
  - 窄容器只折叠放不下的节点，尾部节点尽可能保留
  - 仍超宽时向左扩展，第一项可折叠
  - 最后一项始终可见
  - 容器变宽后自动展开
  - 节点数少于 2 时不折叠
- ellipsis：
  - 包含全部折叠节点
  - 点击 item 发出对应节点的 change 并关闭菜单
  - 折叠范围变化时列表同步更新
- 显式模式：
  - `data-path` 可点击，change 的 key 取 data-path
  - 无 `data-path` 为 disabled
  - 折叠行为与 append 模式一致
- 销毁：item、ellipsis、DropdownMenu、Portal 全部清理

### 可视化

- `tests/previews/preview_breadcrumb.c` 提供宽/窄容器、长路径、自定义标签和下拉展开的可视诊断

## 验收

- C、XML、TSX 三种用法都能生成并交互
- 路径模式、append 模式和显式模式都支持 `change`
- `append_item()` 的 key/label 参数正确写入节点
- change 事件携带正确的节点 key
- 折叠/展开在容器宽度变化时正确
- ellipsis 下拉包含全部折叠节点且点击载荷正确
- 不复制 DropdownMenu/Portal 的状态与生命周期逻辑
- 相关集成测试通过，文档示例可运行
