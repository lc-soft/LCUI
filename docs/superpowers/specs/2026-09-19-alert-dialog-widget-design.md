# AlertDialog 组件设计

## 目标

`alert-dialog` 用于需要用户明确选择的破坏性或不可逆操作确认。

## 结构与 API

```text
alert-dialog
├── alert-dialog-trigger
└── portal
    └── alert-dialog-overlay
        └── alert-dialog-content
            ├── alert-dialog-title
            ├── alert-dialog-description
            └── alert-dialog-footer
                ├── alert-dialog-cancel
                └── alert-dialog-action
```

提供 open/close、trigger、cancel、action 和内容子件工厂；action/cancel 发出明确事件。

## 状态与边界

- 与 Dialog 一样是 Portal 模态层并恢复焦点。
- Esc 和 overlay click 不关闭，必须通过 cancel/action 或显式 API 关闭。
- action/cancel 的业务结果由调用方处理。
- 不实现异步确认等待、二次确认或多步骤流程。

## 依赖与验证

依赖 Dialog/Portal 的 overlay 能力。测试禁止隐式关闭、事件顺序、确认/取消和销毁。

## 文档示例

组件文档示例小节与 shadcn `alert-dialog.mdx` 对齐：Basic（删除确认，按钮右靠齐）、
Small（`size="sm"`，按钮等宽撑满）、Destructive（destructive 动作按钮）；
另设 LCUI 特有的「取消与确认回调」与「注意事项」小节（Esc/遮罩不关闭的行为并入注意事项说明）。
Media、Small with Media 小节不采用（其布局依赖 CSS grid 双列，LCUI 不支持；icon 用法在
Destructive 示例中以纵向居中堆叠呈现）；Installation、RTL 不采用。

## 实现确认（2026-10-07）

实现方式：`alert-dialog` 及其全部子件以对应 `dialog-*` 原型为父注册
（`ui_create_widget_prototype("alert-dialog", "dialog")` 等），共享 dialog.c 的绑定、
开关、遮罩挂载与焦点逻辑；`alert-dialog` init 链式调用 dialog init 后置
`dismissible = false`；widget 持有与 loose 两种模式均生效（loose 经 overlay 的
`alert-dialog-overlay` 类型识别）。开关 API 复用 `ui_dialog_open/close/set_open`。

- 默认子树（`ui_dialog_build_children` 依 `dismissible==false` 分支）：
  header（title "Are you absolutely sure?" / description）、footer（cancel `:outline`
  "Cancel" / action `:primary` "Continue"），不注入 close 按钮。
- 事件顺序：cancel/action 点击 → 先 `ui_dialog_close`（发射 `"close"`）→ 再在 dialog
  上发射 `"cancel"` / `"action"`；业务结果由调用方处理。
- 松散关联扩展：`data-dialog-action="<key>"` / `data-dialog-cancel="<key>"` 标记普通
  Widget，dialog.c 通用扫描绑定（同 `data-dialog-*` 族协议）。
- `size`：`alert-dialog-content` 原型 setattr 回调将 `"sm"` 映射为 widget status
  `:sm`（`alert-dialog-content:sm { max-width: 320px }`），`"default"` 清除；公共 API
  `ui_alert_dialog_content_set_size()`。
- `alert-dialog-media`：容器（父 NULL），内放 icon/image；头部布局降级为 flex column
  居中（shadcn grid + `has-[slot=media]` 不支持）。
- Esc、overlay 点击、`portal-outside` 均不关闭；`portal-outside` 时 dialog.c 立即重开
  portal（Portal 自身默认对 outside 关闭，dialog 在非 dismissible 模式下抵消之）。
- 测试：`tests/integration/test_alert_dialog.c`（structure / dismissal / confirm flow /
  loose actions / size）；文档与 demo 三变体见 `docs/zh-CN/widgets/alert-dialog.mdx`。
