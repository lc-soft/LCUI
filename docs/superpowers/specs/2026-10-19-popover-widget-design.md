# Popover 组件设计

## 目标

为 LCUI 提供与 shadcn `Popover` 对齐的点击式浮层：trigger 点击后在 Portal 中显示可交互内容，
支持方位、对齐和偏移；外部点击与 Esc 关闭；非模态，不阻断外部部件的交互。

## 范围

- 三段式结构：`popover` / `popover-trigger` / `popover-content`。
- 可选头部：`popover-header` / `popover-title` / `popover-description`。
- 非模态：无遮罩、无焦点陷阱；外部点击穿透到下面的部件，同时关闭 Popover。
- 受控/非受控状态：`open` 属性 + `ui_popover_set_open()` / `open` / `close` / `is_open`，
  并发出 `open` / `close` 事件。
- 复用 Portal 的 `anchor` / `side` / `align` / `side-offset` / `align-offset`。
- 新增 Portal 非模态 outside 模式：passthrough portal 仍能在外部点击时收到
  `portal-outside`，且点击不被 portal-root 吃掉。
- 共享 placement 类型：`ui_side_t` / `ui_align_t`；Tooltip 和 Popover 用 typedef 定义
  widget-specific 别名。
- Portal anchor 跟随：scrollarea 滚动时 content 位置随 anchor 更新。
- C / XML / TSX 示例：basic、align、form。
- 中文文档先定稿，确认后同步英文。
- 不包含：`PopoverAnchor`、箭头、遮罩、CSS 动画、焦点陷阱、边缘碰撞翻转、
  resize 重定位、RTL。

## 结构与 API

```text
popover
├── popover-trigger
└── popover-content
    ├── popover-header
    │   ├── popover-title
    │   └── popover-description
    └── （任意可交互内容）
```

### 共享 placement 类型

```c
/* include/LCUI/widgets/placement.h */
typedef enum ui_side {
        UI_SIDE_TOP = 0,
        UI_SIDE_RIGHT,
        UI_SIDE_BOTTOM,
        UI_SIDE_LEFT,
} ui_side_t;

typedef enum ui_align {
        UI_ALIGN_CENTER = 0,
        UI_ALIGN_START,
        UI_ALIGN_END,
} ui_align_t;
```

- 值语义由 Portal 的 `side` / `align` 属性定义；Portal 继续使用字符串 setter 作为底层
  序列化接口。
- Tooltip：

  ```c
  typedef ui_side_t ui_tooltip_side_t;
  typedef ui_align_t ui_tooltip_align_t;
  ```

- Popover：

  ```c
  typedef ui_side_t ui_popover_side_t;
  typedef ui_align_t ui_popover_align_t;
  ```

- 枚举常量统一使用 `UI_SIDE_*` / `UI_ALIGN_*`；Tooltip 迁移到基础常量，并移除原有的
  widget-specific side/align 常量。
- 默认值由部件决定：Tooltip 默认 top / center；Popover 默认 bottom / center。

### Popover C API

```c
ui_widget_t *ui_create_popover(void);
ui_widget_t *ui_create_popover_trigger(void);
ui_widget_t *ui_create_popover_content(void);
ui_widget_t *ui_create_popover_header(void);
ui_widget_t *ui_create_popover_title(void);
ui_widget_t *ui_create_popover_description(void);

void ui_popover_build_children(ui_widget_t *popover);
void ui_popover_set_trigger_text(ui_widget_t *popover, const char *text);
void ui_popover_set_title(ui_widget_t *popover, const char *text);
void ui_popover_set_description(ui_widget_t *popover, const char *text);
ui_widget_t *ui_popover_get_content(ui_widget_t *popover);

void ui_popover_set_open(ui_widget_t *popover, bool open);
void ui_popover_open(ui_widget_t *popover);
void ui_popover_close(ui_widget_t *popover);
bool ui_popover_is_open(ui_widget_t *popover);

void ui_popover_content_set_side(ui_widget_t *content,
                                 ui_popover_side_t side);
void ui_popover_content_set_align(ui_widget_t *content,
                                  ui_popover_align_t align);
void ui_popover_content_set_side_offset(ui_widget_t *content, int offset);
void ui_popover_content_set_align_offset(ui_widget_t *content, int offset);

void ui_register_popover(void);
void ui_unregister_popover(void);
```

- `popover-title` / `popover-description` 继承 `text` 原型，可用 `ui_text_set_content()`。
- `ui_popover_build_children()` 按 Dialog 的模式补齐缺失节点：
  - trigger：缺失时创建 `popover-trigger` + outline button，文本 `Open popover`；
  - content：缺失时创建 `popover-content`；
  - content 内缺失时创建 `popover-header` / `popover-title` / `popover-description`，
    默认文本分别为 `Untitled popover` / `Add supporting text here.`；
  - 已声明的节点不重复创建，子节点顺序保持 trigger 在前、content 在后。
- `ui_popover_set_trigger_text()` / `set_title()` / `set_description()` 只更新已存在的节点；
  `ui_popover_get_content()` 会先补齐结构再返回 content，供表单等内容追加。
- `open` 属性接受 `true` / `false` / `1` / `0`；默认 `false`；结构未绑定时延迟到
  link/update 后生效。
- 触发器和 API 都会同步 `open` 属性；状态变化发出 `open` / `close` 事件。
- side / align 使用 typed setter，字符串只存在于声明层，由 `on_setattr` 解析。

## Portal 非模态 outside

现有 Portal 通过全屏 portal-root 捕获外部点击；这会让非模态 Popover 吃掉外部交互。
新增一个 Portal 选项：

```c
void ui_portal_set_outside_close(ui_widget_t *portal, bool enabled);
```

- 默认 `false`；Popover 同时设置 `ui_portal_set_passthrough(portal, true)` 和
  `ui_portal_set_outside_close(portal, true)`。
- Portal root 的 `pointer-events` 仍按“所有 open portal 都 passthrough”计算：
  - 只有 Popover / Tooltip 等 passthrough portal 时，root 为 `none`，外部点击落到下面
    的部件；content 自身保持 `auto`，内部交互正常。
  - 存在 select / dialog 等捕获式 portal 时，root 为 `auto`，沿用现有 root outside 分发。
- 在 `ui_root` 上注册 click 监听器：对每个 open + `outside_close` 的 portal，
  若点击目标不在该 portal 的 **content 或 anchor** 内，则向该 portal 发
  `portal-outside`。
  - 点击另一个 trigger：旧 Popover 关闭，新 Popover 在同一次点击打开。
  - 点击当前 trigger：anchor 视为内部，由 trigger 的 click 自己切换关闭。
  - 点击当前 content：portal-root 转发 content 点击并取消冒泡，不触发 outside。
  - 目标为 portal-root 本身时跳过，避免与捕获式 root 的 outside 分发重复。
- 捕获式 root 命中某个 portal content 时，除转发给该 portal 外，还要向其他
  `outside_close` portal 发 `portal-outside`，使 Popover 在点击 dialog/select 内容时关闭。
- Tooltip 保持 `outside_close = false`，不会因外部点击被误关。
- Portal 的 `portal-outside` 仍会自动关闭 portal；Popover 监听该事件同步 `open` 状态。

## Portal anchor 跟随

滚动不应关闭 Popover；当 anchor 因 scrollarea 滚动而移动时，content 需要保持相对位置。
该能力由 Portal 提供：

```c
void ui_portal_set_follow_anchor(ui_widget_t *portal, bool follow);
```

- 默认 `false`；Popover 创建 Portal 后启用。
- Portal 在 anchor 的所有 scrollarea 祖先上监听 `scroll`；收到滚动后按 scroll delta 平移
  content，并请求一次更新。
- 嵌套 scrollarea 的 delta 累加；anchor 变更或 Portal 销毁时解除监听并释放记录。
- 该能力不改变 outside 关闭：点击外部仍关闭，滚动只跟随。

## 生命周期与交互

1. `popover` 在 update/link 后按结构查找第一个 `popover-trigger` 与
   `popover-content`；两者都存在时创建 Portal（popover 持有），设置 anchor 并把 content
   移入 portal-root。
2. 同一个时间只允许一个 active Popover：打开新 Popover 时关闭上一个。
3. 打开：
   - 保存 `last_focus` 并监听其销毁；
   - content 先 `visibility: hidden`，`ui_portal_open()` 后用 content 的 `afterlayout`
     重新定位，再 request reflow；
   - 重新布局完成后显示 content，并聚焦 content 内第一个可聚焦部件；没有可聚焦部件时
     保持当前焦点。
4. 关闭：
   - `ui_portal_close()`；
   - 同步 `open` 属性；
   - 只有在焦点原本位于 content 子树时，才恢复 `last_focus`；点击外部控件时不抢焦点；
   - 发出 `close` 事件。
5. 关闭来源：trigger 点击、外部点击、Esc、`ui_popover_close()` / `open=false`。
6. anchor 所在 scrollarea 滚动时，Portal 跟随更新 content 位置，不关闭 Popover。
7. Esc 由 Popover 模块在 `ui_root` 上监听 keydown；只关闭 active Popover。
8. 非模态：无焦点陷阱，Tab 可以离开 content；点击外部控件仍然正常聚焦/触发。
9. `content` 的 `side` / `align` / offset 在打开时写入 Portal，并用于内容定位。
9. 销毁：
   - owner-first：popover 销毁时 Portal 随之销毁，Portal 销毁 content；
   - content-first：Portal 的 content destroy 监听器清空引用，Popover 不重复释放；
   - 释放 `last_focus` 监听器、active 指针和临时字符串。

## 定位与样式

- Portal anchor = trigger。
- 默认 side = bottom、align = center、side offset = 4、align offset = 0。
- 非法 `side` / `align` 回退到 bottom / center。
- content 默认宽度 288px，`p-4`、`border`、`bg-popover`、`rounded-xl`、
  `text-popover-foreground`、阴影；`font-size: 14px`。
- `popover-header` 纵向 flex，间距 6px；`popover-title` 16px medium；`popover-description`
  14px muted。
- 无箭头、无遮罩、无动画、不做碰撞翻转。

## 文档与示例

- 中文页面 `docs/zh-CN/widgets/popover.mdx`，英文在中文确认后同步。
- 字段表：
  - `docs/widget-fields/popover.ts`：`open`；
  - `popover-trigger.ts` / `popover-header.ts` / `popover-title.ts` /
    `popover-description.ts`：继承公共字段；
  - `popover-content.ts`：`side` / `align` / `side-offset` / `align-offset`。
- 侧栏 `docs/sidebars.json` 增加 Popover。
- 示例（每个 C / XML / TSX 三变体）：
  - `popover-basic`：Header / Title / Description + 一个内容按钮；
  - `popover-align`：start / center / end 三个 trigger；
  - `popover-form`：两个 textinput 的表单内容，验证内部交互与焦点。
- C 示例使用 `ui_popover_build_children()` 减少样板；表单使用
  `ui_popover_get_content()` 追加内容。
- 文档说明非模态行为、单 active、焦点策略、外部点击穿透、滚动跟随 anchor、
  无动画/箭头/碰撞翻转。

## 测试

新增 `tests/integration/test_popover.c`，并扩展 `tests/integration/test_portal.c`。

### Portal

1. passthrough + outside_close：root `pointer-events: none`（仅 passthrough portal 时）。
2. 点击外部部件：外部 click 正常触发一次，同时 portal 收到 `portal-outside`。
3. 点击 content：不触发 `portal-outside`。
4. 点击 anchor：不触发 `portal-outside`。
5. 非 `outside_close` 的 passthrough portal（Tooltip 语义）不触发 outside。
6. 捕获式 portal content 点击仍转发，且会关闭其他 `outside_close` portal。
7. follow anchor：scrollarea 滚动后 anchor 与 content 的位移一致。

### Popover

1. 工厂与 `build_children`：trigger / content / header / title / description 结构正确。
2. content 进入 portal-root；Portal 销毁顺序 content-first / owner-first 不崩溃。
3. trigger click 切换；`open` 属性、`ui_popover_set_open()`、`open`/`close` 事件。
4. 同一时间只保留一个 active Popover。
5. 外部点击：点击穿透（外部 handler 执行）且 Popover 关闭；点击另一个 trigger 能一次切换。
6. 外部滚动：Popover 保持打开，content 跟随 anchor 位移。
7. 点击 content 内部不关闭；点击 trigger 关闭。
8. Esc 关闭 active Popover；焦点在 content 内时 Esc 后恢复到打开前焦点；
   外部点击关闭时不恢复焦点。
9. 打开后聚焦 content 内第一个可聚焦部件；无可聚焦部件时保持原焦点。
10. side / align / offset：typed setter、属性解析、非法值回退、content 与 trigger 的相对位置。
11. 标题/描述/触发器文本 setter；`get_content` 可追加表单。
12. content 打开后 header / title / description / button 均有有效布局尺寸。
13. 不写具体主题色断言。

## 涉及文件

- `include/LCUI/widgets/placement.h`、`popover.h`
- `include/LCUI/widgets.h`
- `src/widgets/popover.c`
- `src/lcui_widgets.c`
- `src/widgets/portal.c`、`include/LCUI/widgets/portal.h`
- `src/widgets/tooltip.c`、`include/LCUI/widgets/tooltip.h`
- `src/styles/style-nova.css`、`src/styles/style-nova.css.h`
- `tests/integration/test_popover.c`、`test_portal.c`、`test_tooltip.c`
- `tests/main.c`
- `docs/widget-fields/popover*.ts`
- `docs/zh-CN/widgets/popover.mdx`、`docs/en/widgets/popover.mdx`（中文确认后）
- `docs/examples/popover-{basic,align,form}{,-xml,-tsx}/`
- `docs/sidebars.json`
- `examples/doc-viewer/app/global.css`
- 相关生成文件（doc-viewer、style-nova.css.h）

## 设计决策记录

| 决策 | 选择 | 原因 |
| --- | --- | --- |
| Popover API 范围 | 三段式 + Header/Title/Description，无 Anchor | 对齐最新 shadcn 文档，控制 v1 范围 |
| 外部点击 | 点击穿透 + 关闭 | Popover 非模态，外部控件应能一次点击生效 |
| outside 实现 | Portal 新增 `outside_close` 模式 | 复用 Portal 生命周期，同时满足 AGENTS 的 outside 管理约定 |
| Placement 类型 | `ui_side_t` / `ui_align_t` + widget typedef 别名 | 避免 Tooltip / Popover / 后续部件重复定义同一组值 |
| 默认定位 | bottom / center / side-offset 4 | 与 shadcn Popover 默认值一致 |
| Active 数量 | 同时只保留一个 | 避免多个气泡重叠，Esc 和 outside 行为确定 |
| 焦点 | 首个可聚焦部件 + 条件恢复 | 表单可直接输入；外部点击不抢焦点 |
| C 便利 API | `build_children` + 文本 setter + `get_content` | 对齐 Dialog，减少 C 样板代码 |
