# Doc-Viewer 顶栏与部件索引页设计

日期：2026-11-05

## 背景

doc-viewer 目前是「左侧侧栏 + 右侧文档」的两段结构，顶栏只有一个 LCUI 文字、一个路径文本和三个自绘样式按钮。
本次改造为文档站建立更像正式站点的导航骨架，并为后续 Home 页面、深色主题打基础。

本设计是三个阶段中的 **阶段 A**：

- 阶段 A（本文档）：顶栏三栏布局、导航项、Widgets 索引页、默认 C 示例、语言下拉、主题切换按钮
- 阶段 B：Home 页面（hero + 场景卡片网格）
- 阶段 C：深色主题样式（`style-nova.css`）

## 目标

1. 顶栏改为左 / 中 / 右三个等宽容器，结构稳定、窄窗口下左导航不被挤压。
2. 左侧显示 `Home`、`Docs`、`Widgets` 三个导航按钮，替代 LCUI 文字。
3. 地址框限制最大宽度 320px，并在中间容器内水平居中。
4. 顶栏所有可点击项改用 Button 部件自带的 nova 主题（`ghost` / `secondary` 变体），删除 `navbar.css` 中的按钮自绘样式。
5. 语言切换改为「图标按钮 + dropdown-menu」，图标使用 `local-language`，当前语言项显示 `checkmark`。
6. 右上角新增主题切换图标按钮，图标 `dark-theme`，切换 root 的 `dark` class 并持久化；深色样式由阶段 C 补齐。
7. 所有文档示例默认选中 C 语言标签。
8. 新增 Widgets 索引页，按字母序列出当前全部部件，条目链接到对应文档页。
9. 新增 Home 路由与占位页，供导航使用；内容由阶段 B 实现。
10. 文档页统一挂在 `/{locale}/docs/...` 下，用嵌套 layout 拆分文档外壳与 Home。

## 非目标

- 不做 Home 页面的实际内容（hero、场景卡片）——阶段 B。
- 不写深色配色——阶段 C；阶段 A 只做状态切换与持久化。
- 不实现响应式断点（LCUI 无 media query）；Widgets 索引固定 3 列，窄窗口跟随文档页一起横向裁剪。
- 不改动 GitHub 外链与版本号展示，不新增搜索框。
- 不在 `sidebars.json` 中为索引页新增条目，避免侧栏出现无文档内容的项。

## 现状

- `app/components/navbar.tsx`：LCUI 文字 + 路径 Text + EN/中文两个按钮 + 版本号 + GitHub 链接。
- `app/components/navbar.c`：监听路由更新路径文本与窗口标题；语言切换通过 `app_set_locale()` 持久化并跳转。
- `app/components/navbar.css`：`.navbar` 布局样式 + `.navbar-btn` 等按钮自绘样式。
- 语言切换的本地化机制：`ui_root()` 带 `en` / `zh-CN` class，页面内容用 `.lang en` / `.lang zh-CN` 两份渲染，由 CSS 显隐。
- 路由由 `app/main.h`（`lcui build` 生成）中的 `app_router_init()` 按 MDX 分页注册。
- 示例变体默认顺序在 `scripts/compiler/page-generator.ts` 的 `buildDemoIndexTsx()` 中排序，tsx 排第一即默认选中。
- 示例预览固定渲染 C 实现（`app/examples/<name>/index.c` 调用 `<name>_init(preview)`），与标签选择无关。

## 设计

### 1. 顶栏结构

```text
<div class="navbar">
  <div class="navbar-left">    flex: 1; 三个导航按钮
  <div class="navbar-center">  flex: 1; justify-content: center; 地址框 max-width: 320px
  <div class="navbar-right">   flex: 1; justify-content: flex-end; 语言 / 主题 / GitHub / 版本
</div>
```

- `.navbar` 保留：`display:flex`、`height:56px`、`padding:0 24px`、`align-items:center`、底边线、背景色。
- 三个子容器均为 `flex: 1 1 0`，`min-width: 0`。
- 左侧导航 `flex-shrink: 0`；地址框 `width: 100%; max-width: 320px`。

### 2. 左侧导航

- 三个 `Button`：
  - 非当前项：`variant="ghost"`
  - 当前项：`variant="secondary"`（由 `navbar.c` 在路由变化时切换，避免新增自定义按钮样式）
- 点击通过 `router_replace` 跳转：
  - Home → `/{locale}/home`
  - Docs → `/{locale}/docs/overview/quick-start`
  - Widgets → `/{locale}/docs/widgets`
- 当前项判定：路径以 `/{locale}/home`、`/{locale}/docs/overview`、`/{locale}/docs/widgets` 开头。

### 3. 地址框

- 仍是 `Text`，`$ref="path"`，沿用现有路由监听更新内容。
- 样式：`width:100%; max-width:320px; text-align:center; padding:4px 12px; border:1px solid; border-radius:6px; background: --muted`。
- 不放进 Button，保持只读展示。

### 4. 右侧操作

**语言**：`Button variant="ghost" size="icon"`（图标 `local-language`）作为 `DropdownMenuTrigger`；
菜单项为 `English` / `中文`，当前语言项前置 `checkmark` 图标；点击调用现有 `navbar_set_locale`。
移除原来的 EN / 中文两个文本按钮及其点击绑定，`navbar.c` 里的 `locale_en` / `locale_zh` refs 相应删除。

**主题**：`Button variant="ghost" size="icon"`（图标 `dark-theme`）。
点击切换 `ui_root()` 的 `dark` class，并通过 `lcui_settings` 写入 `[app] theme = light|dark`。
启动时读取该值并应用 class；默认 `light`。阶段 A 只保证 class 与持久化正确，视觉由阶段 C 提供。

**GitHub**：`Button variant="ghost"` + 文本 `GitHub`，保留 `<a href>` 外链。

**版本号**：`Text`，保留现有 `navbar-version` 样式（不属于按钮）。

### 5. 默认选中 C 示例

`buildDemoIndexTsx()` 的变体排序从「tsx 优先」改为「c 优先」：

- 排序结果：`c` → `xml` → `tsx`（后两者保持原有相对顺序）。
- `demo-provider.c` 挂载时取 `.demo-languages` 的第一个 tab 作为激活项，因此默认选中 C，代码块与文件标签同步为 `main.c`。
- 预览仍渲染 C 实现，与默认标签一致，不额外改动。
- 变体内文件排序保持「框架文件优先」不变，C 变体只有 `main.c`，不受影响。

### 6. Widgets 索引页

- 路由：`/zh-CN/docs/widgets`、`/en/docs/widgets`，位于 docs layout 内（带侧栏）。
- 页面分别放在 `app/zh-CN/docs/widgets/page.tsx` 与 `app/en/docs/widgets/page.tsx`，文本直接写对应语言。
- 页面结构：

```text
H1: 组件 / Components
描述段落（双语）
H2: 全部组件 / All Components
3 列 × N 行网格，每项为组件英文名，链接到 /{locale}/docs/widgets/<slug>
```

- 数据来源：`docs/sidebars.json` 中 `slug === "widgets"` 的分组，读取 `items`，不硬编码列表。
- 行内 3 列用「每 3 项一个 flex row」实现，不依赖 `flex-wrap`。
- 条目名称统一使用英文（与截图一致）；中文名仍在侧栏展示。
- 页面宽度与文档正文一致，左右留白；当前项不高亮。

### 7. 路由结构与布局拆分

采用嵌套 layout，把「顶部导航」和「文档外壳」分开：

```text
app/
  layout.tsx                      顶部导航栏 + children 出口（全站共用）
  zh-CN/
    home/page.tsx                 /zh-CN/home（无侧栏）
    docs/
      layout.tsx                  中文侧栏 + 内容滚动区
      overview/quick-start/page.tsx      /zh-CN/docs/overview/quick-start
      widgets/button-group/page.tsx      /zh-CN/docs/widgets/button-group
      widgets/page.tsx                    /zh-CN/docs/widgets（手写索引页）
  en/
    home/page.tsx                 /en/home
    docs/
      layout.tsx                  英文侧栏 + 内容滚动区
      ...（同上结构）
```

- 根 `layout.tsx` 只保留 Navbar 和子路由出口，不再包含侧栏与 ScrollArea。
- `zh-CN/docs/layout.tsx` 与 `en/docs/layout.tsx` 各自渲染对应语言的侧栏（`sidebar.tsx` 增加 `locale` 参数，不再用 `.lang` 双份渲染），内部保留 ScrollArea。
- Home 页面独立在 `zh-CN/home`、`en/home`，天然不带侧栏，不需要 `home` class 或 CSS 显隐。
- 侧栏链接、地址框路径、`route-titles.h` 全部带 `/docs` 段。

### 8. 落地页与根路由

- 新增 `app/page.tsx`（根 `/`）：读取当前 locale 后重定向到 `/{locale}/home`。
- 新增 `app/zh-CN/docs/page.tsx`、`app/en/docs/page.tsx`：重定向到 `/{locale}/docs/overview/quick-start`。
- 两个重定向都在页面 ready 后调用 `router_replace()`，不渲染实际内容。
- 这样既消灭了「layout 目录没有 page 时会注册空 `root` 组件记录」的坑（`/` 与 `/docs` 都有真实页面），也给用户提供了自然的入口。

### 9. Router 单元测试

在 `lib/router/tests/test.c::test_router_matcher` 已有的路由树上补断言（现有代码已注册 `user-overview` 这条空路径子路由，但没有断言）：

- `router_resolve("/users/root")`
  - `matched[0]` 的 component = `user-show`
  - `matched[1]` 的 component = `user-overview`
- 保留现有 `/users/root/posts` → `matched[1] = user-posts` 的断言。
- 新增一组等价的「layout + page + 子页面」最小用例，确认三层结构：
  - `layout`（父）、`page`（空路径子路由）、`child/page`（子路由）
  - `/widgets` → `[layout, page]`
  - `/widgets/button` → `[layout, child]`

测试失败即说明 matcher 的 path_list 插入顺序被改坏，doc-viewer 的目录结构不能采用。

### 10. 文档页生成路径调整

- `scripts/compiler/index.ts` 中 `localeAppDir` 从 `app/<locale>` 改为 `app/<locale>/docs`，生成页面自动落到 `docs/` 下。
- `generateRouteTitles()` 的路径改为 `/<locale>/docs/<section>/<slug>`。
- `sidebar.tsx` 的 `RouterLink` 目标改为 `/<locale>/docs/<section>/<slug>`。
- watch 模式的重编译路径同步调整。
- 旧的 `app/<locale>/<section>/` 生成目录需要删除（`git rm`），避免残留页面产生重复路由。

## 涉及文件

```text
app/layout.tsx                  顶层：Navbar + 子路由出口
app/zh-CN/docs/layout.tsx       中文侧栏 + 内容滚动区
app/en/docs/layout.tsx          英文侧栏 + 内容滚动区
app/zh-CN/home/page.{tsx,c,h}   Home 占位页（阶段 B 扩展）
app/en/home/page.{tsx,c,h}
app/page.tsx                   根路由，重定向到 /{locale}/home
app/zh-CN/docs/page.tsx        文档落地页，重定向到 quick-start
app/en/docs/page.tsx
app/zh-CN/docs/widgets/page.{tsx,c,h}  中文部件索引页
app/en/docs/widgets/page.{tsx,c,h}     英文部件索引页
app/components/navbar.tsx       三栏结构、nova 按钮、语言下拉、主题按钮
app/components/navbar.c         导航跳转、active 变体切换、主题持久化
app/components/navbar.css       仅保留顶栏容器与地址框样式，删除按钮自绘样式
app/components/sidebar.tsx      增加 locale 参数，链接前缀加 /docs
app/theme.c / app/theme.h       主题状态读取、切换、持久化（新文件）
app/locale.c                    参考其 settings 读写实现
app/global.css                  顶栏三栏、索引页网格样式
lib/router/tests/test.c        补 index page 匹配断言与 layout 三层用例
scripts/compiler/index.ts       页面输出目录改为 app/<locale>/docs
scripts/compiler/page-generator.ts   变体排序 c 优先、route-titles 路径加 /docs
examples/doc-viewer/AGENTS.md  更新目录结构、构建与页面注册说明
```

## 验证

1. `bun run compile`、`npx lcui build --force`、`xmake build doc-viewer` 全部通过。
2. 顶栏在 1920 与 1280 宽度下：左导航不换行、地址框 ≤320 且水平居中、右侧按钮右对齐。
3. 导航：Home / Docs / Widgets 可互相跳转，切语言后路径保持对应页面。
4. 语言下拉：展开显示两项，当前项有 checkmark，切换后路径、内容、持久化都正确。
5. 主题按钮：点击后 `ui_root()` 的 class 在 `dark` 间切换，重启后保持；阶段 A 不验证配色。
6. 默认标签：打开任意部件页，代码区默认选中 C / `main.c`。
7. Widgets 索引页：列出 sidebars 中全部 30 个部件，点击跳转正确，中英文标题随 locale layout 正确显示。
8. `librouter-tests` 通过，新增断言覆盖 `/widgets`、`/widgets/button` 的 matched 链。
9. 访问 `/` 与 `/zh-CN/docs` 都能正确重定向，不出现空白 `root` 页面。
10. 离屏渲染截图核对（不打开窗口）。

## 风险

- 文档 URL 从 `/{locale}/<section>/<slug>` 变为 `/{locale}/docs/<section>/<slug>`，站内硬编码链接需全部同步（navbar、sidebar、route-titles、示例内部链接）。
- 旧生成目录 `app/<locale>/<section>/` 必须删除，否则 `lcui build` 会把它们也注册成路由，出现重复页面。
- 手写 `page.tsx` 与生成的 `page.c/.h` 同目录时，`lcui build` 只重写 `page.tsx.h`，不会覆盖 `page.c/.h`；实现时验证。
- 路由匹配顺序：`/zh-CN/docs/widgets` 与 `/zh-CN/docs/widgets/<slug>` 依赖「空路径子路由排在父记录之前」的匹配顺序；由新增的 router 单元测试锁住，若测试失败需要先修 matcher 再改目录结构。
- 语言切换函数的路径改写假设路径第一段是 locale；`/zh-CN/home` 与 `/zh-CN/docs/...` 都满足。
- 阶段 C 之前 `dark` class 没有实际样式，用户会看到“按钮有效果但页面不变色”，需在界面之外说明这是分阶段交付。
