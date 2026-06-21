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

## 结构

引导语...

\`\`\`tsx
import { WidgetName } from "@lcui/react";

<WidgetName />
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

#### ## 结构（Anatomy）
- 代码块标签：` ```tsx `（无 title 属性）
- 内容：`import { X } from "@lcui/react"` + 组件用法
- 代码块**前**写一行引导语，二选一：
  - 单组件即可使用（如 Button、Text）→ `Import the component:`
  - 需多个部件组合使用（如 ScrollArea + Scrollbar）→ `Import the component and assemble its parts:`
- 代码块后**禁止**写"X 由 Y + Z 组成"等内部实现说明
- **可以**写行为/模式描述（如 Anchor 的 URL 与 XML 视图模式区别）

#### ## 示例（可选）
- 用 `###` 三级标题分组
- 每个示例一段代码（c / xml / tsx），须带 `title="..."` 元数据
- 放在 API 参考之前或之后均可

#### ## API 参考
- 只用 `<FieldTable name="xxx" />`，编译器自动从 `docs/widget-fields/{name}.ts` 读取字段数据
- 不手写 API 小节、属性列表或事件列表

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

- C 变体的 main.c 须定义 `{widget}_{variant}_init(ui_widget_t *parent)`
- TSX 变体的 main.c 须 `#include "example.h"` 并调用 `example_load()`
- 编译器自动用 highlight.js 高亮并提取 local symbols

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
| 结构 | Anatomy |
| API 参考 | API Reference |
| 示例 | Examples |
| 注意事项 | Caveats |
| 内联样式标签 | Inline style tags |
| 导入部件： | Import the component: |
| 导入部件并组装其子组件： | Import the component and assemble its parts: |

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
