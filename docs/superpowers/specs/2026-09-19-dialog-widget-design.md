# Dialog 组件设计

## 目标

`dialog` 提供可访问的模态内容容器，使用 Portal 脱离父级布局和裁剪。

## 结构与 API

```text
dialog
├── dialog-trigger
└── portal
    └── dialog-overlay
        └── dialog-content
            ├── dialog-header
            ├── dialog-title
            ├── dialog-description
            └── dialog-footer
```

提供 open/close、`ui_dialog_set_open()`、trigger/content 子件工厂和 `ui_dialog_build_children()`。

## 状态与交互

- 打开时记录触发器，聚焦 content 内第一个可聚焦元素；关闭时恢复焦点。
- overlay click 和 Esc 均关闭；content 内点击不关闭。
- 模态 overlay 必须阻断背景 pointer 事件。
- 首版不支持多个 Dialog 的完整 stack manager、焦点陷阱或拖动调整大小。

## Portal 与验证

Portal content 是全屏 overlay wrapper，不使用 trigger 锚定坐标。测试焦点恢复、Esc、遮罩、跨树事件、销毁。

## 文档示例

组件文档小节与 shadcn `dialog.mdx` 对齐：基础示例（对应 shadcn dialog-demo：Edit profile + Name/Username 表单）、
自定义关闭按钮（Custom Close Button）、无关闭按钮（No Close Button）、固定 Footer（Sticky Footer）、
可滚动内容（Scrollable Content）；另设 LCUI 特有的「声明式关联」与「注意事项」小节。
Installation、RTL 小节不采用（LCUI 无安装命令语义/无 RTL 支持，RTL 限制在注意事项说明）。

## 实现确认（2026-10-07）

最终 API（`include/LCUI/widgets/dialog.h`，alert 复用同一开关 API）：
`ui_create_dialog`、`ui_dialog_get_data/build_children/set_open/open/close/is_open/get_content`、
子件工厂 `ui_create_dialog_{trigger,overlay,content,header,title,description,footer,close}`、
`ui_register_dialog/ui_unregister_dialog`。

声明式关联协议（对齐 select 的关联模式，`docs/superpowers/specs/2026-07-07-select-widget-design.md`）：

| 属性 | 含义 |
|------|------|
| `data-dialog-trigger="<key>"` | 点击切换对应 dialog；同键可多个 |
| `data-dialog-target="<key>"` | 指定浮层节点（Portal content，通常为 dialog-overlay）；同键唯一 |
| `data-dialog-close="<key>"` | 点击关闭对应 dialog |
| `data-dialog-content="<key>"` | 可选，覆盖"内部点击不关闭"的边界节点 |

类型化子件优先绑定最近 `dialog` 祖先（原型链匹配）；脱树节点经 registry 按 key 懒创建
controller（`loose=true`、owner=ui_root()），registry 在 `ui_unregister_dialog` 统一释放。
绑定为双路径：子件自身 `link` 事件（覆盖 loose 与声明式）+ dialog 的 BEFORE_UPDATE 扫描
（覆盖 widget 持有与延迟追加）；`ui_dialog_build_children()` 可重扫并补齐默认子树。

行为与状态：
- overlay 是 Portal content（全屏遮罩/内容 wrapper），不使用 anchor 定位；
  结构规则（含 `dialog-overlay/alert-dialog-overlay` 默认 `display: none`、flex 居中、
  content 布局与 `max-width: 384px`）内置于 `src/widgets/dialog.c` 的 `ui_dialog_css`
  （`display:none` 缺失曾导致跨示例泄漏，测试 `dialog overlay isolation` 锁住）；
  style-nova.css 只保留配色/圆角/阴影等视觉调整；`ui_dialog_open()` 打开时内联
  设 overlay `display: flex` 实现居中。
- 事件：open/close 成功后分别在 dialog 上发射 `"open"` / `"close"`（冒泡）。
- 关闭途径：overlay 空白点击、`KEY_ESCAPE` keydown、任意 `dialog-close`/`data-dialog-close`
  节点、`portal-outside`；content 内部点击不关闭；`dismissible=false`（alert-dialog）时前三者失效。
- 焦点：LCUI `tab_index` 默认 -1；trigger 内部按钮与 dialog-close 按钮设 `tab_index=0`。
  open 记录 `ui_get_focus()` 并聚焦 content 内首个可聚焦后代，close 恢复；last_focus 销毁经
  destroy listener 自动失效。
- `dialog-close` 仅在 `dismissible=true` 的默认子树中注入（alert-dialog 无 close 按钮）。
- 子类型原型（alert-dialog-*）的链式 init 必须经静态原型指针调用父 init，禁止
  `w->proto->proto->init`（子类型继承时无限递归）。
- Portal 销毁 owner 时立即销毁 content（`ui_widget_destroy` 而非入 trash；trash 会在
  prototype 表销毁后才处理，跨 suite 场景产生悬垂原型）。

shadcn 能力降级（对照 `F:\code\shadcn-ui-main\apps\v4\registry\styles\style-nova.css`）：
fade/zoom 动画与 backdrop-blur 删除（无动画/模糊能力）；ring → `border + box-shadow`；
`max-w-[calc(100%-2rem)]`/`sm:` 断点 → 固定 `max-width: 384px`；grid/`has-[slot=media]`
头部布局 → flex column 居中；`showCloseButton` → 已实现等价 API
`ui_dialog_show_close_button()`（仅控制默认子树是否注入 `dialog-close`）。
配色：`tailwind.config.js` 新增 `popover`/`popover-foreground` token，content 使用
`bg-popover text-popover-foreground`（不复用 card）。

首版范围外：多 Dialog stack manager、焦点陷阱、异步/拖动调整；Esc 仅在焦点位于
content 子树时生效（文档已注明该边界）。
