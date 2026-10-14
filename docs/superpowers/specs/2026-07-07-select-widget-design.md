# Select 组件设计

## 概述

Select 提供下拉单选能力。`<select>` Widget 是状态容器，每个实例在 init 阶段
创建独立的 `ui_select_t` controller。触发器、值显示节点和内容节点优先绑定最近
的 `<select>` 祖先；脱离 Select 子树的普通 Widget 可以通过关联键松散绑定。

下拉内容仍通过 Portal 挂载到 `portal-root`，以绕过 LCUI 父级裁剪限制。Portal
设计见 `docs/superpowers/specs/2026-07-07-portal-widget-design.md`。

## 目标

- SelectTrigger 默认提供可用的 Button，不再出现空 trigger 的 `0x0` 尺寸。
- 允许普通 Button、Text 或其他 Widget 直接充当 trigger、value 或 item。
- trigger、value 和 content 无需嵌套在统一的 Select Widget 中。
- 声明式松散用法在第一次交互时按关联键懒创建 controller。
- C API 使用 `<select>` Widget 作为状态入口，并显式支持创建 trigger、value、content
  和 item。
- JSX `<Select>` 展开为一个运行时 `<select>` Widget，不依赖编译期自增 ID。

## 非目标

- 本阶段不支持多选、搜索、键盘导航或选项分组。
- 本阶段不提供持久化 controller；LCUI 销毁时统一释放 registry。
- 本阶段不自动观察整个 UI 树。controller 在交互或 API 调用时按需解析关联节点。

## 声明式关联协议

### 属性

| 属性 | 含义 |
|------|------|
| `data-select-trigger="<key>"` | 点击该节点时切换对应 Select |
| `data-select-target="<key>"` | 指定对应的浮层内容节点 |
| `data-select-value="<key>"` | 指定显示当前 label 或 placeholder 的节点 |
| `data-select-item` | 将普通 Widget 标记为选项 |
| `data-select-placeholder="<text>"` | value 节点未选中时显示的文本 |
| `value="<value>"` | 选项值；不存在时点击不改变值 |
| `title="<label>"` | 非 text 选项的可选显示 label |

`select-item` 类型天然被识别为选项，不需要额外设置 `data-select-item`。普通
Widget 必须设置 `data-select-item` 才会被当作选项。

### 基本用法

普通 Button 可以同时充当 trigger 和 value：

```html
<button
  data-select-trigger="theme"
  data-select-value="theme"
  data-select-placeholder="Choose a theme"
>
  Choose a theme
</button>

<select-content data-select-target="theme">
  <select-item value="light">Light</select-item>
  <select-item value="dark">Dark</select-item>
  <select-item value="system">System</select-item>
</select-content>
```

选中后，Select 直接替换 Button 的全部文本内容。需要保留箭头等装饰时，应使用
独立的 value 节点：

```html
<button data-select-trigger="theme">
  <select-value
    data-select-value="theme"
    data-select-placeholder="Choose a theme"
  />
  <text>down</text>
</button>

<select-content data-select-target="theme">
  <select-item value="light">Light</select-item>
  <select-item value="dark">Dark</select-item>
</select-content>
```

普通 Widget 也可以作为选项：

```html
<select-content data-select-target="theme">
  <button data-select-item value="light" title="Light theme" />
  <button data-select-item value="dark">Dark</button>
</select-content>
```

## Controller 与 Registry

### 类型

公共头文件只暴露不透明 controller 类型和 Select Widget 工厂：

```c
typedef struct ui_select ui_select_t;
```

`ui_select_t` 由 `<select>` Widget 的 init 创建并持有。`ui_create_select()` 创建
Select Widget，而不是直接创建 controller。controller 生命周期跟随 Select Widget。

内部结构至少保存：

```c
struct ui_select {
    char *value;
    ui_widget_t *trigger;
    ui_widget_t *target;
    ui_widget_t *portal;
    bool open;
    list_node_t node;
};
```

value 节点允许有多个，不长期缓存为单一指针。同步显示时按 key 查找全部匹配的
`data-select-value` 节点。trigger 和 target 的缓存需在节点 unlink/destroy 时失效。

### 创建规则

- `ui_create_select()` 创建一个 Select Widget，并在 init 中创建独立 controller。
- `select-trigger`、`select-value` 和 `select-content` 优先向上查找最近的 `select`
  祖先，并绑定到该 controller。
- 通过 `data-select-trigger/target/value="<key>"` 参与的普通 Widget 没有 Select
  祖先时，才使用 registry 按 key 查找或懒创建独立 controller。
- 同一 key 可以有多个 trigger 和 value，但只能有一个 target。
- 如果存在多个 target，采用 UI 树遍历遇到的第一个，并输出警告。
- Select Widget 销毁时释放其 controller；松散模式懒创建的 controller 在 widgets
  模块销毁时统一释放。

### 声明式懒创建流程

```text
root click
  -> 从 event.target 向上查找最近的 data-select-trigger
  -> 优先查找 Select 祖先；否则按 key 查找或创建 controller
  -> 在 ui_root() 中解析相同 key 的 target 和 value 节点
  -> 创建或更新 Portal，anchor 设为本次 trigger
  -> 将 target 挂载到 portal-root
  -> 切换 open 状态
```

事件处理采用根部件事件委托，不为每个声明式 trigger 单独注册 click listener。

## Widget 类型

| 类型 | 父原型 | 职责 |
|------|--------|------|
| `select-trigger` | NULL | 行为包装器；默认创建 Button 和 SelectValue |
| `select-value` | `text` | 显示当前 label 或 placeholder |
| `select-content` | NULL | 下拉面板，由 controller 交给 Portal 管理 |
| `select-item` | `text` | 文本选项，使用通用 `value` 属性保存值 |

`select` 是状态容器 Widget；`ui_create_select()` 创建它。`select-trigger`、
`select-value`、`select-content` 和 `select-item` 是组合部件。

### SelectTrigger 默认结构

`ui_create_select_trigger()` 创建：

```text
select-trigger
└── button (data-select-trigger=<key>)
    └── select-value (data-select-value=<key>)
```

尺寸和默认视觉样式来自真实 Button，而不是复制 Button CSS 到
`select-trigger`。`select-trigger` 只负责组合和语义，不承担按钮视觉实现。

## 值与 Label

### 读取 value

点击选项时读取通用 `value` 属性：

1. 有 `value`：更新 controller 值并关闭下拉内容。
2. 无 `value`：忽略点击，不修改值，也不关闭内容。

### 读取 label

成功读取 value 后，按以下顺序读取 label：

1. 选项继承自 `text`：调用 `ui_text_get_content_w()`。
2. 否则存在 `title` 属性：使用 `title`。
3. 否则使用 `value` 属性值。

controller 内部使用宽字符串作为一次同步过程中的 label 表示。`title` 和
`value` 的 UTF-8 内容先转换为宽字符串，再写入 value 节点。

### 更新 value 节点

- `select-value` 和 Button 均继承自 `text`，调用 `ui_text_set_content_w()`。
- 普通 text 派生 Widget 同样直接替换全部文本。
- 如果一个 Button 同时设置 `data-select-trigger` 和 `data-select-value`，选中后
  替换其全部文本内容。
- controller value 为空时，显示各 value 节点自身的
  `data-select-placeholder`；没有 placeholder 时显示空文本。
- 同一 key 的所有 value 节点保持同步。

## Item 状态

controller 设置值后遍历 target 的全部后代：

- `select-item` 或带 `data-select-item` 的节点视为选项。
- value 与 controller 当前值相同的选项添加 `checked` status。
- 其他选项移除 `checked` status。
- `select-item:checked` 可通过 CSS定义选中样式。

选项允许嵌套在普通 wrapper 中，因此必须使用后代遍历而不是仅遍历直接子节点。

## Portal 与事件

controller 首次打开时创建 Portal Widget，并将 target 设置为 Portal content。
Portal anchor 始终更新为本次触发的 trigger，因此多个 trigger 可以共享同一 target。

target 转移到 `portal-root` 后，Portal 继续负责：

- content click 的逻辑转发；
- 空白区域的 outside event；
- content 定位；
- portal-root 的显示和隐藏。

Select 的根事件委托同时识别：

- `data-select-trigger`：切换对应 controller；
- target 内的 `select-item` 或 `data-select-item`：更新值并关闭；
- Portal outside event：关闭对应 controller。

## C API

```c
typedef struct ui_select ui_select_t;

ui_widget_t *ui_create_select(void);
ui_select_t *ui_select_get_data(ui_widget_t *select);

ui_widget_t *ui_create_select_trigger(void);
ui_widget_t *ui_create_select_value(void);
ui_widget_t *ui_create_select_content(void);
ui_widget_t *ui_create_select_item(const char *value);

ui_widget_t *ui_select_content_add_item(ui_widget_t *content,
                                        const char *label,
                                        const char *value);
int ui_select_content_append_item(ui_widget_t *content, ui_widget_t *item);
void ui_select_content_clear_items(ui_widget_t *content);

void ui_select_set_value(ui_widget_t *select, const char *value);
const char *ui_select_get_value(ui_widget_t *select);
void ui_select_open(ui_widget_t *select);
void ui_select_close(ui_widget_t *select);
```

### C 用法

```c
ui_widget_t *select = ui_create_select();
ui_widget_t *trigger = ui_create_select_trigger();
ui_widget_t *content = ui_create_select_content();

ui_select_content_add_item(content, "Label 1", "value1");
ui_select_content_add_item(content, "Label 2", "value2");
ui_select_set_value(select, "value2");

ui_widget_append(select, trigger);
ui_widget_append(select, content);
ui_widget_append(container, trigger);
ui_widget_append(container, select);
```

`ui_create_select_trigger()`、`ui_create_select_content()` 和
`ui_create_select_item()` 创建独立 Widget。将 trigger/content 作为 select 的子节点
时，它们自动绑定到最近的 Select controller。第一次 `ui_select_open()` 或 trigger
click 时创建 Portal 并转移 content，使调用方可以先按普通 Widget 完成挂载和配置。

Select Widget 销毁时释放 controller 和 Portal 关联，并按普通 Widget 生命周期销毁
其真实子节点。

## JSX API

```tsx
<Select>
  <SelectTrigger>
    <SelectValue placeholder="Choose a theme" />
    <Text>down</Text>
  </SelectTrigger>
  <SelectContent>
    <SelectItem value="light">Light</SelectItem>
    <SelectItem value="dark">Dark</SelectItem>
    <SelectItem value="system">System</SelectItem>
  </SelectContent>
</Select>
```

`<Select>` 是组合辅助器：

- 每个实例展开为一个运行时 `<select>` Widget；
- `SelectTrigger`、`SelectValue` 和 `SelectContent` 通过祖先 Select 自动绑定；
- 只有 `asChild` 或组件被移出 Select 子树时，才注入 `data-select-*` 关联属性；
- 不使用编译期或 Node 模块级自增 ID。

`SelectTrigger` 默认生成 `select-trigger > button`。设置 `asChild` 时不创建默认
Button，而是将 `data-select-trigger` 注入唯一子元素。C 层不增加 `asChild` API；
C 用户可直接给任意 Widget 设置 `data-select-trigger`。

## 生命周期

- Widget 引用在 unlink/destroy 时从 controller 缓存中清除。
- target 在 Portal 中先销毁时，Portal 清除 content 引用，controller 下次打开时
  重新按 key 查找 target。
- controller 手动销毁时关闭并销毁内部 Portal，但不销毁应用 Widget。
- 声明式懒创建的 controller 可长期存在于 registry；widgets 模块销毁时统一释放。
- key 生成器在 widgets 模块初始化时重置。

## 测试

集成测试至少覆盖：

- `ui_create_select()` 创建独立 controller。
- 默认 trigger 包含 Button 和 SelectValue，布局后尺寸大于零。
- 同一模板创建多个 Select Widget 时 controller 不共享。
- trigger/content/value 作为 Select 后代时自动绑定到最近 Select。
- trigger、value、target 分散在不同父节点时仍可关联。
- 第一次声明式 trigger click 懒创建 controller 并打开 target。
- 多个 trigger 共享 target 时，以本次 trigger 作为 Portal anchor。
- Button 同时作为 trigger/value 时，选中后全部文本被替换。
- label 来源依次覆盖 text、title、value 三条路径。
- 无 value 的 item 不改变值且不关闭菜单。
- 普通 Widget 只有带 `data-select-item` 时才被识别为选项。
- value 同步到同 key 的多个 value 节点。
- item checked status 与当前值同步。
- outside event 关闭对应 controller。
- controller 销毁不销毁调用方 Widget。
- content-first 和 trigger-first 销毁不会留下可访问的悬空引用。

## 文档与示例

更新以下内容以反映新 API：

- `docs/zh-CN/widgets/select.mdx`
- `docs/en/widgets/select.mdx`
- `docs/widget-fields/select*.ts`
- `docs/examples/select-basic/`
- `docs/examples/select-basic-xml/`
- `docs/examples/select-basic-tsx/`
- 生成的 doc-viewer 文件

文档同时展示：

1. JSX 组合用法；
2. 无 `<select>` 控制器的 XML 属性关联用法；
3. 使用不透明 `ui_select_t` 的 C API 用法；
4. trigger/value 合并和拆分两种形式。

## 设计决策

| 决策 | 选择 | 原因 |
|------|------|------|
| 状态所有者 | `<select>` Widget 持有的 `ui_select_t` | 支持运行时多实例且不需要编译期 ID |
| 关联方式 | `data-select-trigger/target/value` + key | 组件类型和职责在属性名中可直接识别 |
| controller 创建 | Select init 创建；松散模式首次交互懒创建 | JSX/C 嵌套简单，脱树节点仍可关联 |
| 默认 trigger | 包装器内创建 Button | 复用 Button 尺寸、交互和样式 |
| 自定义 trigger | JSX `asChild` 或直接设置属性 | 不把行为绑定到固定 Widget 类型 |
| item value | 通用 `value` 属性 | 避免冗长的 `data-select-item-value` |
| item label | text -> title -> value | 减少重复声明并提供稳定回退 |
| 普通 item 标记 | 布尔 `data-select-item` | 避免 target 内普通按钮被误识别 |
| content 挂载 | 首次打开时交给 Portal | 保持调用方挂载流程直观并避免父级裁剪 |
| `<select>` Widget | 保留为状态容器 | 每个运行时 Select 实例拥有独立状态，避免编译期自增 ID |

## 依赖

| 上游 | 内容 |
|------|------|
| Portal | content 转移、定位、事件转发、outside event 和 root 可见性 |
| Button | 默认 SelectTrigger 的视觉与交互基础 |
| Text | SelectValue、SelectItem 以及 label 的宽字符串读写 |
