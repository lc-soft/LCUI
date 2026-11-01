# Accordion 设计

## 目标

新增 `accordion` 部件族，对应 shadcn `Accordion`：垂直堆叠的标题/内容组，支持单开与多开、默认展开、禁用项和状态事件。

- 独立实现，不依赖 `collapsible`
- root 统一管理展开集合，item 只持有自己的 open 状态
- 对外以 `change` 事件交付“本次切换的节点 value”，root 的 `value` 属性保存完整集合
- 提供 C API、XML / TSX 声明式用法、主题、集成测试和中文文档示例

## 范围

### 包含

- `accordion`、`accordion-item`、`accordion-trigger`、`accordion-content`
- 单开（默认）/ 多开、`default-value`、受控 `value`
- 禁用项、默认 chevron、Enter/Space 键盘切换
- C / XML / TSX 三变体示例与字段表
- 集成测试

### 暂缓

- 展开/收起动画（LCUI CSS 无 transition/animation）
- 方向键 / Home / End 的 roving tabindex
- orientation / RTL 等布局方向能力

## 结构

```text
accordion
└── accordion-item (value="shipping")
    ├── accordion-trigger
    │   ├── <调用方内容>
    │   └── icon.accordion-trigger-icon（默认自动创建）
    └── accordion-content
```

- 每个 `accordion-item` 需要一个 `value` 作为节点标识；没有 `value` 的 item 仍可独立展开，但不会进入 root `value`，`change` 载荷为空串
- `accordion-trigger` 默认可聚焦（`tab_index = 0`），并自动创建右侧 chevron；`chevron="false"` 时不创建
- 需要完全自定义标题栏时，可以不用 `accordion-trigger`，在任意 item 后代上声明 `data-accordion-trigger="true"`；此时焦点能力与图标由调用方负责
- `accordion-content` 在 item 为 closed 时不参与显示

## 公共 API

```c
ui_widget_t *ui_create_accordion(void);
ui_widget_t *ui_create_accordion_item(void);
ui_widget_t *ui_create_accordion_trigger(void);
ui_widget_t *ui_create_accordion_content(void);

void ui_accordion_set_multiple(ui_widget_t *accordion, bool multiple);
bool ui_accordion_get_multiple(ui_widget_t *accordion);
void ui_accordion_set_value(ui_widget_t *accordion, const char *values);
const char *ui_accordion_get_value(ui_widget_t *accordion);
bool ui_accordion_is_open(ui_widget_t *accordion, const char *value);

void ui_accordion_item_set_value(ui_widget_t *item, const char *value);
void ui_accordion_item_set_open(ui_widget_t *item, bool open);
bool ui_accordion_item_get_open(ui_widget_t *item);

void ui_register_accordion(void);
```

- `values` 为逗号分隔的 value 列表，允许空格并忽略空段，例如 `"shipping, support"`
- `ui_accordion_get_value()` 返回 root 持有的字符串，在下一次状态变化前有效
- `ui_accordion_set_value()` 按列表同步 item；单开模式下只打开列表中的第一个
- `ui_accordion_item_set_open()` 直接设置 item 状态，并通知 root 做互斥和 value 同步

## 属性

| 节点 | 属性 | 类型 | 默认 | 说明 |
|---|---|---|---|---|
| `accordion` | `multiple` | boolean | `false` | 是否允许多项同时展开 |
| `accordion` | `default-value` | string | - | 初始展开项，逗号分隔；root 已有 `value` 时以 `value` 为准 |
| `accordion` | `value` | string | - | 当前展开项，逗号分隔；设置后同步 item |
| `accordion-item` | `value` | string | - | 节点标识，用于 value / change |
| `accordion-item` | `open` | boolean | `false` | 当前状态；设置会通知 root |
| `accordion-item` | `disabled` | boolean | `false` | 复用内建 disabled |
| `accordion-trigger` | `chevron` | boolean | `true` | 是否自动创建右侧箭头 |
| 任意 item 后代 | `data-accordion-trigger` | boolean | - | 声明该节点为 trigger；用于完全自定义的标题栏 |

### data-* 约定

Accordion 沿用仓库现有的**组件作用域角色属性**，不使用 Bootstrap 的 `data-toggle` + `data-target` 组合：

- `data-accordion-trigger="true"` 是角色标记；归属关系由最近的 `accordion-item` 祖先确定
- 不提供 `data-accordion-target`；content 通过 item 子树与 `accordion-item:closed accordion-content` 关联
- 与 `data-collapsible-trigger`、`data-dropdown-menu-trigger` 的写法一致
- 只有需要跨 Portal 配对的组件才用属性值作为 key（如 `data-dialog-trigger="profile"` + `data-dialog-target="profile"`）；Accordion 的 trigger/content 始终在同一 item 子树内，不需要 key

## 状态模型

- item 的 `open` 是每项的事实状态，通过 `open` / `closed` status 表达
- root 维护：
  - `multiple`
  - 初始化标记
  - 同步标记（防止 `value` setter 与子项状态互相递归）
- root 在以下时机重建 `value`：
  - item 展开/收起后
  - `ui_accordion_set_value()` 后
  - 初始化应用 `default-value` 后
- `value` 的构建顺序按 item 在树中的文档顺序，逗号连接；没有展开项时写为空串
- 单开模式：打开某项时关闭其它项；关闭当前项后允许全部收起
- 多开模式：各项独立切换

## 初始化

- root 在 `afterlayout` 中惰性扫描 `accordion-item` 后代：
  - 首次扫描时应用 root `value`（若存在），否则应用 `default-value`
  - 维护最后一项的 `last` status（用于去掉底部分隔线）
- 初始化阶段（首次扫描完成前）应用 `value` / `default-value` / item `open` 只同步状态，不发出事件
- 后续新增的 item 默认 closed；如果调用方需要初始展开，使用 `ui_accordion_item_set_open()` 或重设 root `value`
- item 的 `open` 属性在解析时直接生效；初始化完成后再修改 `open` 视为显式状态变更，会发出事件

## 交互

### 鼠标

- root 监听 `click`，从 `e->target` 沿父链向上找 trigger，再找所属 item
- trigger 判定：`accordion-trigger` 原型，或任意带 `data-accordion-trigger="true"` 的后代
- 命中 trigger 且 item 未禁用时切换该项
- 点击 trigger 内的文本、图标、wrapper 后代同样有效
- 点击 content 不切换
- disabled item 不响应

### 键盘

- `accordion-trigger` 默认 `tab_index = 0`；自定义 trigger 由调用方自行设置焦点能力
- root 监听 `keydown`；当焦点在 trigger 内（原型或 `data-accordion-trigger`）且按键为 `KEY_ENTER` 或 `KEY_SPACE` 时切换该项
- 其它按键不处理；不实现方向键导航
- 切换后设置 `e->cancel_bubble = true`

## 事件

- item 在状态变化后发出 `open` 或 `close`
- root 在 item 状态同步、`value` 重建后发出 `change`
- `change` 的 `emit_arg` 为本次切换 item 的 `value`（没有 value 时为空串）
- 事件来源：
  - 用户点击 trigger 或按 Enter/Space
  - `ui_accordion_item_set_open()`，以及初始化完成后再修改 item 的 `open` 属性
- `ui_accordion_set_value()` 是受控状态同步，静默更新 item；初始化阶段应用 `value` / `default-value` / item `open` 同样静默
- 设置 root `value` 可能同时改变多个 item，但不会逐个发出 `change`
- 监听器需要完整集合时读取 `ui_accordion_get_value(root)`；需要单项状态时使用 `ui_accordion_is_open()`

```c
static void on_change(ui_widget_t *w, ui_event_t *e, void *arg)
{
        const char *toggled = arg;

        printf("toggled: %s, open: %s\n", toggled,
               ui_accordion_get_value(w));
}

ui_widget_on(accordion, "change", on_change, app_state);
```

## Chevron

- `accordion-trigger` init 时创建 `icon` 子节点，class `accordion-trigger-icon`，名称 `chevron-down`，尺寸 16
- item 状态变化时，找到所属 trigger 的该 icon，切换为 `chevron-up` / `chevron-down`
- trigger 的 `update` 回调保证 icon 始终位于子节点末尾（允许调用方在创建后再追加内容）
- `chevron="false"` 时不创建 icon；调用方可以自行放置图标
- 用 `data-accordion-trigger` 声明的自定义 trigger 不创建、不修改任何图标；展开/收起视觉由调用方根据 `accordion-item:open` / `accordion-item:closed` 自行处理

## 样式

### 结构（`src/widgets/accordion.c`）

```css
accordion {
  display: flex;
  flex-direction: column;
  width: 100%;
}
accordion-item {
  display: block;
  position: relative;
}
accordion-trigger {
  display: flex;
  align-items: center;
  justify-content: space-between;
  width: 100%;
  cursor: pointer;
}
accordion-content {
  display: block;
}
accordion-item:closed accordion-content {
  display: none;
}
```

### 主题（`src/styles/style-nova.css`）

- item 默认 `border-bottom: 1px solid #e5e5e5`；`accordion-item:last` 去掉底边
- trigger：`padding: 10px 0`、`text-sm`、`font-medium`、hover underline、`:focus` 使用 `bg-muted`
- `accordion-trigger-icon`：16px、muted
- content：`text-sm`、底部 10px 间距
- `accordion-item:disabled accordion-trigger`：`opacity: 0.5`，不响应 hover

## 测试

新增 `tests/integration/test_accordion.c`，注册到 `tests/main.c`：

- 工厂与类型
- `default-value` 初始展开，且不发 `change`
- 单开互斥、多开独立、再次点击可全部收起
- 点击 trigger 切换；点击 trigger 内 wrapper/图标有效；点击 content 无效；disabled 无效
- `data-accordion-trigger` 自定义 trigger：点击、Enter/Space 可切换；不创建自动 chevron
- Enter/Space 切换；其它按键忽略
- `change` 载荷为本次切换项 value；root `value` 按文档顺序更新；item 发出 `open`/`close`
- `ui_accordion_set_value()` 同步 item；`ui_accordion_is_open()` 查询
- 默认 chevron 存在且 open/closed 名称正确；`chevron="false"` 不创建
- 无 value 的 item 可切换，`change` 载荷为空串
- 销毁清理

不使用 `tests/previews`；示例效果由 doc-viewer 页面验证。

## 文档与示例

- `docs/zh-CN/widgets/accordion.mdx`
- 字段表：`accordion`、`accordion-item`、`accordion-trigger`、`accordion-content`
- 示例（C / XML / TSX 三变体）：
  - `accordion-demo`：顶部 FAQ（shipping / returns / support），默认展开 shipping
  - `accordion-basic-demo`：三项 FAQ，默认展开第一项
  - `accordion-multiple`：多开模式，默认展开 notifications
  - `accordion-disabled`：第二项禁用
  - `accordion-borders`：外层圆角边框，item 有分隔线和内边距
  - `accordion-card`：Card + Accordion 组合
  - `accordion-custom-trigger`：用 `button data-accordion-trigger="true"` 作为自定义标题栏，不自动加箭头
- `defaultValue` / `value` 在 LCUI 使用逗号分隔字符串（如 `"shipping,support"`），不使用 shadcn 的数组
- 文档注明：无展开动画；键盘支持 Enter/Space，不支持方向键导航；`data-accordion-trigger` 是 LCUI 扩展，自定义 trigger 需自行处理焦点和图标
- 更新 `docs/sidebars.json`，示例样式写入 `examples/doc-viewer/app/global.css`

## 验收

- `xmake run lcui-tests --grep="accordion"` 通过
- `xmake test` 通过
- `bun run compile`、`npx lcui build --force`、`xmake build doc-viewer` 通过
- `/zh-CN/widgets/accordion` 可渲染全部示例
- C / XML / TSX 三变体齐全
