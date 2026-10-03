# Icon 组件设计

## 目标

`icon` 是继承 Text 的基础图标部件，为 `@lcui/fluent-icons` 及未来的图标字体包提供 C、XML 与 TSX 一致的承载节点。它不携带字体文件；字体资源始终由应用依赖的图标包和 `@lcui/cli` 打包。

## 类与字体资源

- Fluent font class：Regular 使用 `fui-icon`，Filled 使用 `fui-icon fui-icon-filled`；分别来自 `icons.css` 与 `icons-filled.css`，负责指定 font-family。
- Fluent glyph class：图标名使用 `fui-icon-<name>`，filled modifier 使用 `fui-icon-filled`，设计尺寸使用 `fui-icon-glyph-<N>`；图标包生成的 scoped CSS 将这些类组合映射到 Unicode codepoint。
- 图标名 `filled` 和匹配 `glyph-<digits>` 的名称为保留名，不能与 modifier token 冲突。
- Lucide 基类为 `lucide-icon`，图标名和设计尺寸分别使用 `lucide-icon-<name>`、`lucide-icon-glyph-<N>`；目前只支持 regular，不使用 filled 风格。
- 应用必须导入图标包的 CSS。未导入时 LCUI 按普通文本字体显示 glyph；核心库不探测字体、不打印警告、不渲染专用占位方框。

## API

```c
typedef enum ui_icon_library {
        UI_ICON_LIBRARY_FLUENT,
        UI_ICON_LIBRARY_LUCIDE,
} ui_icon_library_t;

ui_widget_t *ui_create_icon(void);
ui_widget_t *ui_create_fluent_icon(const char *name);
void ui_icon_set_library(ui_widget_t *icon, ui_icon_library_t library);
void ui_icon_set_name(ui_widget_t *icon, const char *name);
void ui_icon_set_size(ui_widget_t *icon, int size);
void ui_icon_set_filled(ui_widget_t *icon, bool filled);
```

- `ui_icon_set_library()`、`ui_icon_set_name()`、`ui_icon_set_size()` 和 `ui_icon_set_filled()` 共同生成库对应的 font class 与 semantic class。Fluent regular 图标例如为 `fui-icon fui-icon-access-time`；Filled 追加 `fui-icon-filled`，非默认设计尺寸追加 `fui-icon-glyph-N`。Lucide 使用 `lucide-icon` 和 `lucide-icon-access-time`，并固定选择 regular glyph；传入 filled 时不输出 filled class。更新时移除部件自身添加的旧 class；CSS `content` 由 Text 的样式更新链路写入渲染层。
- 新建部件默认 Fluent、regular，内部 `size=0` 表示自动选择，不生成 glyph-size class；基础 CSS 规则提供 regular 24，若缺少 24 则使用包生成器选定的最近可用尺寸。显式 `size=24` 同样使用基础规则；正数且非 24 的尺寸才生成 glyph-size class。核心库不维护图标尺寸表。
- `ui_create_fluent_icon(name)` 是 Fluent 常用情形的快捷构造函数，等价于创建默认 Fluent Icon 并设置 name；需要 filled、size 或其它库时继续使用 setter。
- `@lcui/fluent-icons` 的 TSX `Icon` 包装组件可以继续使用 `iconMap` 按 `convert.js` 规则选择更合适的实际尺寸，但该映射不复制进 LCUI 核心库。
- XML 使用 `library="fluent"`、`name="access-time"`、`size="24"` 和 `filled="false"` 属性，或直接使用等价的 namespaced Fluent classes。C 使用上述 setter；prototype 解析 `library`、`name`、`size`、`filled` 属性。Lucide 不使用 filled。

## 边界与验证

- 不绑定 `@lcui/fluent-icons`、未来 `@lcui/lucide-icons` 的 TTF、CSS 或图标全量映射到 LCUI 动态库。
- 不内建图标映射或码点；各图标包的 CSS 是 glyph class 与 `content` 的唯一来源。所有 glyph selectors 都必须由库基类限定。LCUI Icon 不再缓存独立的 `style_class` 或旧格式 `glyph_class`：只缓存并替换实际添加的 namespaced name/modifier classes。Lucide 使用独立 `lucide-icon` font class，目前只选择 regular per-icon glyph class，不暴露 filled variant。
- 测试工厂、Text 继承、library/filled class 切换和 glyph class 替换。CSS content 到文本层的更新由现有 Text 测试路径覆盖。
