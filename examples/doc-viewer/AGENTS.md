# doc-viewer 文档编写规范

本文档为 AI agent 和人类贡献者编写 doc-viewer 项目文档源文件时的指令参考。

## 目录结构

```
docs/
  sidebars.json                        # 导航栏数据
  widget-fields/
    widget.ts                          # FieldData 接口 + 公共字段
    {name}.ts                          # 各组件专属字段
  {locale}/widgets/{name}.mdx          # 页面源文件
  examples/
    {widget}-basic/main.c              # C 语言 demo
    {widget}-basic-xml/main.c ui.xml   # XML 变体
    {widget}-basic-tsx/main.c example.tsx # TSX 变体
examples/doc-viewer/
  scripts/compiler/                    # MDX → page.tsx 编译管线
```

## Widget 页面模板

每个 widget 的 MDX 文件**必须**按以下固定顺序编写：

```mdx
# WidgetName

一句话功能简介。

<WidgetExample name="widget-basic" />

## 适用场景

- **适用**：...
- **不适用**：...

## 用法

\`\`\`tsx
import { WidgetName } from "@lcui/react"
\`\`\`

\`\`\`tsx
<WidgetName />
\`\`\`

## 组合（可选）

使用以下组合来构建 `WidgetName`：

\`\`\`
WidgetName
├── SubComponent
└── SubComponent
\`\`\`

## API 参考

<FieldTable name="widget" />
```

### 各小节说明

#### # 标题与简介
- H1 标题：组件名（英文，PascalCase）
- 紧跟一段功能描述（一至两句话）

#### WidgetExample（demo 嵌入）
- 紧跟简介后，放在任何 `##` 之前
- `name` 属性对应 `docs/examples/` 目录名（不含 `-tsx` / `-xml` 后缀）
- 一个 widget 页面放一个 basic 示例

#### ## 适用场景
- 每个列表项以 `- **适用**：` 或 `- **不适用**：` 开头
- 中文用全角冒号 `：`，英文用半角冒号 `: `

#### ## 用法（Usage）
- 两个代码块，均为 ` ```tsx `（无 title 属性）
- 第一个代码块：import 语句
- 第二个代码块：组件核心用法（无需构造完整函数实现）
- 代码块**前**无需引导语

#### ## 组合（Composition）—— 仅多部件组件需要
- 仅当组件包含子部件（如 ScrollArea 由 ScrollAreaContent + Scrollbar 组成）时才写此小节，单组件省略
- 代码块标签：` ``` `（无语言标记，纯文本块）
- 内容：用树形结构展示组件层次关系（`├──` / `└──`）
- 代码块**前**写一行引导语：`使用以下组合来构建 `WidgetName`：`（英文：`Use the following composition to build a `WidgetName`:`）
- 代码块后**禁止**写"X 由 Y + Z 组成"等内部实现说明
- **可以**写行为/模式描述（如 Anchor 的 URL 与 XML 视图模式区别）

#### ## 示例（可选）
- 用 `###` 三级标题分组
- 每个示例一段代码（c / xml / tsx），须带 `title="..."` 元数据
- 放在 API 参考之前或之后均可

#### ## API 参考
- 只用 `<FieldTable name="xxx" />`，编译器自动从 `docs/widget-fields/{name}.ts` 读取字段数据
- 不手写 API 小节、属性列表或事件列表
- **多部件组件**：每个 FieldTable 前必须用 `### <WidgetName>` 三级标题 + 一句话描述（参考 `docs/zh-CN/widgets/scrollarea.mdx`）。zh-CN 模板：
  ```mdx
  ## API 参考

  ### RadioGroup

  单选组容器，管理组内各项的互斥选中状态。

  <FieldTable name="radio-group" />

  ### RadioGroupItem

  单选项，每一项代表一个可选值。

  <FieldTable name="radio-group-item" />
  ```
  en 模板：标题 `## API Reference`，子标题 PascalCase，描述同步翻译。

#### ## 注意事项（可选）
- 每个注意点独立一个 admonition 容器
- 容器类型：`:::note` / `:::info` / `:::tip` / `:::caution` / `:::warning`
- 容器内首句加粗：`:::warning` 包裹 `**核心警告**。详细补充...`

#### ## 内联样式标签（可选）
- 仅支持 BBCode 标签的组件（Text、TextInput）需要此小节

## 非 Widget 页面模板

overview、handbook 等页面结构更自由，仅要求：
1. H1 标题 + 简介段落
2. `##` 小节自由组织
3. 示例代码块须带 `title="..."` 元数据

## 文档工作流

以中文文档为主，英文文档在中文文档更新完毕后再全量翻译。

1. **新增组件文档**：先在 `docs/zh-CN/widgets/{name}.mdx` 完成全部内容。
2. **修改/补充文档**：同样先改 zh-CN，确认章节结构、示例、描述完整后再同步。
3. **同步英文**：对照 zh-CN 版本逐节翻译到 `docs/en/widgets/{name}.mdx`。
   - 保留 `<WidgetExample>` / `<FieldTable>` / 代码块 / admonition 标签原样
   - 仅翻译普通文本（标题、段落、list item、admonition 内容）
   - 代码注释无需翻译
4. **不要双语并行开发**：zh-CN 未定稿时不改 en，避免两边结构漂移。

## MDX 语法参考

| 语法 | 渲染效果 |
|------|----------|
| `# H1` | 页面标题 |
| `## H2` | 节标题 |
| `### H3` / `#### H4` | 节内子标题 |
| `**bold**` | `[b]...[/b]` |
| `*italic*` | `[i]...[/i]` |
| `` ``code`` `` | `[bgcolor=#eee] code [/bgcolor]` |
| ` ```lang title="x" ` | 代码块（带 header + Copy 按钮） |
| ` ```lang title="" ` | 代码块（无 header，仅 Copy 按钮） |
| ` ```lang ` | 代码块（带语言名 header + Copy 按钮） |
| `- item` 或 `* item` | 无序列表 |
| `1. item` | 有序列表 |
| `| 表格 |` | flexbox 模拟表格 |
| `:::note` / `info` / `tip` / `caution` / `warning` | admonition 容器 |
| `<WidgetExample name="..." />` | 嵌入 demo |
| `<FieldTable name="..." />` | 嵌入 API 参考表 |

## Demo 示例文件

每个示例是一个目录：

```
docs/examples/{name}/
  main.c
docs/examples/{name}-xml/
  main.c
  ui.xml
docs/examples/{name}-tsx/
  main.c
  example.tsx
```

- **三变体必齐**：C、XML、TSX 三种实现都要提供。即使 widget 暂未在 `@lcui/react` 中导出，也要给出 TSX 变体作为前瞻占位（example.tsx 仅作为代码块展示，不会实际编译）。
- **TSX 优先展示**：编译器按 `tsx > xml > c` 排序变体，TSX 是默认激活的标签。
- C 变体的 main.c 须定义 `{widget}_{variant}_init(ui_widget_t *parent)`
- TSX 变体的 main.c 须 `#include "example.h"` 并调用 `example_load()`
- 编译器自动用 highlight.js 高亮并提取 local symbols
- **`<LCUI.h>` 是聚合头**，已包含 `<LCUI/widgets.h>`、`<ptk.h>`、`<ui_xml.h>`、`<LCUI/app.h>` 等。Demo 的 main.c **只能**写 `#include <LCUI.h>`，不要重复 include 子头。

## widget-fields 数据

```typescript
interface FieldData {
  name: string;
  type: string;
  default: string;        // "-" 表示必填
  description: {
    en: string;
    "zh-CN": string;
  };
}
```

- `docs/widget-fields/widget.ts`：公共字段（className、children），所有组件通过 `...widgetFields` 继承
- `docs/widget-fields/{name}.ts`：用 `...widgetFields` 展开后追加组件专属字段
- 字段数据来源：lcui-toolkit types.d.ts 中的 props 类型信息，现阶段手动维护

## 中英对照

| zh-CN | en |
|-------|-----|
| 适用场景 | Use cases |
| 用法 | Usage |
| 组合 | Composition |
| API 参考 | API Reference |
| 示例 | Examples |
| 注意事项 | Caveats |
| 内联样式标签 | Inline style tags |
| 使用以下组合来构建 `WidgetName`： | Use the following composition to build a `WidgetName`: |

### FieldTable labels

| zh-CN | en |
|-------|-----|
| 属性名 | Prop |
| 类型 | Type |
| 默认值 | Default |
| 描述 | Description |

## 构建验证

```bash
bun run compile                   # 全量编译 MDX → page.tsx
bun run compile --watch           # 监听变更
npx lcui build app --force        # 重生成 C 层 .tsx.h / main.h
xmake build doc-viewer            # 编译链接
xmake run doc-viewer              # 运行验证
```

每次修改 MDX 或 widget-fields 后，须依次运行：

```bash
bun run compile
npx lcui build app --force
xmake build doc-viewer
```

> ⚠️ `npx lcui build app --force` 输出末尾的 `unknown target(lcui) for doc-viewer.deps!` 警告无害，是已知问题，可忽略。`bun run compile` 自动复制 `widget-fields/` 并触发页面生成；任何一步失败都会产生连锁错误。

## Demo 变体命名约定

`docs/examples/` 下的 demo 目录名是文档里 `<WidgetExample name="...">` 的 `name` 属性值：

| 变体后缀 | 含义 | 内容 |
|---------|------|------|
| （无后缀） | 基础 / 唯一示例 | `main.c` 实现 `void <slug>_basic_init(ui_widget_t *parent)` |
| `-xml` | XML 标记声明式 | `ui.xml` + `main.c`（`lcui_init` + `ui_load_xml_file`） |
| `-tsx` | TSX 声明式 | `example.tsx` + `main.c`（`lcui_init` + `example_load()`） |
| `-<feature>` | 特性示例 | 镜像 `-tsx` 三件套结构，如 `-disabled`、`-dual-scrollbars` |

**三变体必齐**：C、XML、TSX 三种实现都要提供。即使 widget 暂未在 `@lcui/react` 中导出，也要给出 TSX 变体作为前瞻占位（`example.tsx` 仅作为代码块展示，不会实际编译）。

## widget-fields 文件命名与目录复制陷阱

`<FieldTable name="radio-group-item" />` 解析为 `docs/widget-fields/radio-group-item.ts`。**文件名必须用 `-`（dash）**，不能用 `_`（underscore）。命名约定是 slug 原文，camelCase 变量名（如 `radioGroupItemFields`）由文件作者自己定。

⚠️ **`examples/doc-viewer/app/widget-fields/` 是编译时副本，不会自动清理**：

`scripts/compiler/index.ts:55` 的 `copyWidgetFields()` 只 `copyFileSync` 不删除。如果在 `docs/widget-fields/` 重命名或删除文件，旧的 `app/widget-fields/<old-name>.ts` 会残留。下次编译时编译器可能：

- 找不到新名（如果旧文件还在 dest）→ 报 `ModuleLoaderError: File does not exist`
- 引用错文件（如果同名但内容已变）

**修正步骤**：删 `examples/doc-viewer/app/widget-fields/` 下 stale 文件，或 `git clean -fd examples/doc-viewer/app/widget-fields` 强制重生成。

## Demo 预览容器布局陷阱

doc-viewer 的 demo preview 区域本身是 flex 容器，所以**多个独立 flex 行直接 append 到 parent 会横排**。需要在 C/XML demo 外层包一个 `display:flex; flex-direction:column; gap:8px` 容器：

```c
ui_widget_t *box = ui_create_widget(NULL);
ui_widget_set_style_string(box, "display", "flex");
ui_widget_set_style_string(box, "flex-direction", "column");
ui_widget_set_style_string(box, "gap", "8px");
ui_widget_append(box, row1);
ui_widget_append(box, row2);
ui_widget_append(parent, box);
```

XML 用外层 `<div style="display:flex; flex-direction:column; gap:8px">` 包裹。TSX 用 `<Widget className="flex flex-col gap-2">...</Widget>` 包裹。

RadioGroup / ScrollArea / Field 等本身是垂直容器，item 直接 append 进 group 即可，无需外层 wrapper。

## 生成的 `index.c` 在 Linux 上的 `static` 不一致问题

`lcui build app --force` 对 `examples/doc-viewer/app/examples/<name>/index.c`
使用 skeleton-once 策略：只在文件不存在时写入，后续构建保留手写改动。
首次生成的 `index.c` 中 `<ns>_demo_update` 函数被声明为 `static`，
但同名的 `<ns>_demo_update(ui_widget_t *)` 在对应 `index.h` 里是非
`static` 的全局声明。MSVC 默认不报警告；GCC/Clang 在开启 `-Wall`
或项目使用 `set_warnings("all", "error")`（LCUI 在 Ubuntu 上的
默认配置）时会因声明/定义不一致而**编译失败**。

**触发场景**：每次新增 demo（含 `WidgetExample` 引用）或首次为新
demo 生成 `index.c` 后。

**手动修复**：在生成的 `index.c` 里删除 `static` 关键字：

```c
// before
static void checkbox_disabled_demo_update(ui_widget_t *w) { ... }

// after
void checkbox_disabled_demo_update(ui_widget_t *w) { ... }
```

⚠️ 只去掉 `<ns>_demo_update` 上的 `static`，**保留** `<ns>_demo_init` /
`<ns>_demo_destroy` 上的 `static`——这两个没有对应 header 声明，是
prototype 内部回调，加 `static` 是正确的。

详细说明见 `~/.config/opencode/skills/lcui-cli/SKILL.md` 的
"Known limitations / open bugs" 节。
