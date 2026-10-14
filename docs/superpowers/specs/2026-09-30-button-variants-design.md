# Button 变体设计

## 目标

为 LCUI Button 增加适合常见描边按钮工作流的运行时变体 API，并参考 shadcn/ui `style-nova.css` 实现主题化视觉样式。默认按钮采用不透明背景的 outline 外观；主色按钮显式指定 `primary`。同步完善 Button 文档和 C、XML、TSX 示例，便于在 doc-viewer 中直观比较。

## 范围与行为

- Button 初始变体为 `outline`，使用主题边框和不透明 `background` 色，不用透明背景。
- 支持变体：`outline`、`primary`、`secondary`、`ghost`、`destructive`、`link`。
- `primary` 是少量关键操作使用的主题色实心按钮；`secondary` 是次级实心按钮；`outline` 是默认的有边框按钮；`ghost` 是无常驻边框/背景、交互时显现背景的低强调按钮；`destructive` 用于危险操作；`link` 呈现主题色文本，hover 时出现下划线。
- `primary`、`ghost`、`destructive`、`link` 等变体需显式覆盖默认 outline 的边框/背景；link 使用透明背景与透明边框，使其呈现文本按钮外观。outline 本身必须保持不透明主题背景。
- `icon` 按 shadcn 语义是尺寸预设而非互斥视觉变体。`size` 支持 `default`、`xs`、`sm`、`lg`、`icon`、`icon-xs`、`icon-sm`、`icon-lg`；icon 系列渲染为紧凑方形按钮，示例使用既有 Icon widget。
- C API setter 对超出枚举范围的 variant/size 回退到 `outline` / `default`；未知属性值读取时也按默认值处理。
- XML 和 TSX 通过通用 `variant`、`size` 属性设置外观。C 调用方使用公开 setter/getter；默认值无需显式写入属性。
- 变体和尺寸只改变视觉样式，不改变 Button 的文本、点击、disabled 或布局语义。
- 普通 Button 保持既有 `inline-block` 排版方式；icon 尺寸使用 inline-flex 方形对齐图标。`:active` 使用相对定位 `top: 1px` 下移制造按压效果，同时保留变体各自的按压背景反馈。
- 样式优先用 `src/styles/style-nova.css` 的 Tailwind `@apply` 工具类表达。Tailwind 配置新增 `background: "#fff"` token，outline 使用 `border-border bg-background` 与 `hover:bg-muted`；不得用透明背景替代其主题背景。link 使用主题色和 4px underline offset，hover 时显示下划线。
- 启用 Tailwind `textUnderlineOffset` core plugin，使 `underline-offset-4` 可通过 `@apply` 编译成 LCUI CSS。LCUI 的 CSS 引擎接受 `text-decoration`，不接受 Tailwind 内建 `underline` utility 生成的 `text-decoration-line`；因此 link hover 使用 `@apply [text-decoration:underline]` 这个 Tailwind arbitrary-property utility，输出 LCUI 支持的属性，同时避免在主题 stylesheet 中直接写原始 CSS 属性。
- 清理或调整 `src/widgets/button.c` 中会与主题样式冲突的旧默认规则，保持样式由项目主题 stylesheet 统一管理；重新生成其 CSS header。

## API 设计

在 `include/LCUI/widgets/button.h` 定义：

```c
typedef enum ui_button_variant {
        UI_BUTTON_VARIANT_OUTLINE,
        UI_BUTTON_VARIANT_PRIMARY,
        UI_BUTTON_VARIANT_SECONDARY,
        UI_BUTTON_VARIANT_GHOST,
        UI_BUTTON_VARIANT_DESTRUCTIVE,
        UI_BUTTON_VARIANT_LINK,
} ui_button_variant_t;

typedef enum ui_button_size {
        UI_BUTTON_SIZE_DEFAULT,
        UI_BUTTON_SIZE_XS,
        UI_BUTTON_SIZE_SM,
        UI_BUTTON_SIZE_LG,
        UI_BUTTON_SIZE_ICON,
        UI_BUTTON_SIZE_ICON_XS,
        UI_BUTTON_SIZE_ICON_SM,
        UI_BUTTON_SIZE_ICON_LG,
} ui_button_size_t;
```

并提供 `ui_create_button(void)`、`ui_button_set_variant()` / `ui_button_get_variant()` 与 `ui_button_set_size()` / `ui_button_get_size()`。工厂创建 outline/default-size 按钮。实现复用 `variant`/`size` widget attributes 和 status：属性变化时清除同组旧 status 并设置新 status；unset 时 CSS 的 `button` 基础规则提供 outline 默认样式，getter 返回 `UI_BUTTON_VARIANT_OUTLINE` 或 `UI_BUTTON_SIZE_DEFAULT`。

变体 setter 与工厂分开：LCUI 现有组件工厂（如 `ui_create_badge()`、`ui_create_icon()`）使用无参数创建，再用 setter 配置外观；并且 variant 与 size 可组合。避免以 `ui_create_button(variant)` 取代无参数工厂（C 不支持可选/重载参数），也不增加 `ui_create_primary_button()` 等一组重复工厂，避免按变体和尺寸组合扩张 API。调用方可写 `button = ui_create_button(); ui_button_set_variant(button, UI_BUTTON_VARIANT_PRIMARY);`。本次不增加按钮组、额外图标 API 或事件行为；图标仍由既有 Icon widget 提供。

## 样式结构

- `button` 基础规则设置通用的按钮排版、间距、边框和 outline 默认背景。
- `button:outline` 与默认基础外观保持一致，显式传入时仍得到稳定相同的 outline 外观。
- `button:primary`、`:secondary`、`:ghost`、`:destructive`、`:link` 由对应 variant status 应用主题样式。
- `button:link:hover` 显示文本下划线；link 初始状态不显示下划线。
- `button:icon` 调整为紧凑正方形尺寸；它可以与 outline、ghost 等视觉变体组合。
- disabled 样式继续明确可见，不被 hover 或 variant 规则覆盖。
- 所有可由 Tailwind 工具类表达的视觉声明使用 `@apply`；只有 Tailwind 中不存在且无法由工具类组合表达的 LCUI 专属规则才考虑原始 CSS，并在实现时说明理由。

## 文档和示例

- 按 `examples/doc-viewer/AGENTS.md` 的顺序先完成 `docs/zh-CN/widgets/button.mdx`，再同步 `docs/en/widgets/button.mdx`。
- 保留现有 `button-basic` 示例并对齐 shadcn 开头示例（Button 文案按钮 + ArrowUp 图标按钮）；新增 `button-variants` 示例并在 Button 页面“变体”部分嵌入，默认描边按钮不显式写 `variant="outline"`。
- 变体示例只展示 outline、primary、secondary、ghost、destructive、link；图标相关示例单独成节：`button-icon`（icon 尺寸方形按钮）与 `button-with-icon`（文字前带 Icon 的按钮）；`button-size` 按 shadcn 文案用四组（xs/sm/default/lg 各配对应 icon 尺寸）展示 `size` 预设。
- 图标使用既有 Icon widget/Fluent icon（`arrow-up`、`arrow-up-right`），不新增依赖；多按钮示例容器通过 `button-*-demo` 类（doc-viewer `global.css` 中 `flex flex-wrap items-center gap-4`）获得间距，TSX 示例代码使用 `flex gap-4` 工具类。
- 在 `docs/widget-fields/button.ts` 同步记录 `variant` 和 `size` 的取值、默认值及中英文说明。
- 文档明确 icon 是尺寸预设；展示 link hover 下划线以及 primary 的显式设置方式。

## 测试与验证

- 新增 `tests/integration/test_button.c`，注册至 `tests/main.c`。
- 测试覆盖工厂创建和 Button 类型、默认 outline/default size、C API setter/getter、枚举边界回退、切换变体时旧 status 移除、新 status 添加、variant/size 属性到 status 的同步，以及 Button 仍继承 text 内容能力。
- 集成测试检查 CSS computed style：默认/outline 是不透明主题背景和边框，primary/secondary/destructive 使用相应主题色，link hover 有下划线而普通状态无下划线，icon size 改变尺寸但不覆盖 variant。
- 修改 C 文件后运行 clang-format；构建 style-nova stylesheet；运行 focused Button integration suite。
- 修改 MDX、widget-fields 或 demo 后按仓库规范运行文档编译、`lcui build` 和 doc-viewer 构建；最后运行 `xmake test`。

## 不做的更改

- 不新增按钮组、loading、spinner、as-child/link 渲染模式或图标库功能。
- 不把 `icon` 定义为互斥的视觉 `variant`。
- 不更改 Button 默认行为之外的 widget、全局 Tailwind token 或 shadcn 源码。
