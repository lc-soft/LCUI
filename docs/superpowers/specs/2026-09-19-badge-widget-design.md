# Badge 组件设计

## 目标

`badge` 是承载短标签、状态或计数的轻量文本容器。

## 结构与 API

`badge` 不要求子组件。提供 `ui_create_badge()`、`ui_badge_set_variant()`、`ui_badge_set_outline()` 和 `ui_badge_set_color()`；内容由 Text 原型提供。

## 状态与边界

- 预置 default、secondary、success、warning、destructive、outline 六种视觉语义，`ui_badge_variant_t` 枚举仅用于这些稳定语义。
- `color` 属性和 `ui_badge_set_color()` 使用命名调色板 `blue`、`green`、`sky`、`purple`、`red`。实现为移除旧 `badge-color-*` status、添加新 status，再由内置 CSS 应用前景/背景/边框色。
- 调色板固定复用 shadcn Custom Colors 的亮色配对：blue `#eff6ff/#1d4ed8`、green `#f0fdf4/#15803d`、sky `#f0f9ff/#0369a1`、purple `#faf5ff/#7e22ce`、red `#fef2f2/#b91c1c`（背景/前景）。LCUI CSS 不支持变量，因此颜色字面量写入 `badge.c` 的 CSS 字符串。
- 保留 `ui_badge_set_background_color()` 和 `ui_badge_set_border_color()` 作为 C 低层覆写 API；`ui_badge_set_color()` 改为命名调色板 API，不再接收 `css_color_value_t`。
- variant 不承载业务状态，调用方负责文本与图标。
- 不实现 link、可关闭标签、计数截断或尺寸体系。

## 依赖与验证

仅依赖基础 Widget/CSS 与 `icon`。测试工厂、variant、命名颜色、outline 和禁用状态。圆角必须采用 `26px`，渲染器需将角半径限制为 `min(width, height) / 2`，以匹配浏览器对过大圆角的胶囊形结果。组件文档至少提供 shadcn 对照的默认四项、“变体”、“带图标”和“自定义颜色”示例。