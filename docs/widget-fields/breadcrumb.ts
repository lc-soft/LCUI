import { widgetFields, FieldData } from "./widget";

export const breadcrumbFields: FieldData[] = [
  ...widgetFields,
  {
    name: "path",
    type: "string",
    default: "-",
    description: {
      en: "Builds the items from a slash-separated path. Labels fall back to the path segments; use explicit items or the C API for custom labels.",
      "zh-CN":
        "按斜杠分隔的路径自动构建节点，标签回退为路径段；需要自定义标签时使用显式 item 或 C API。",
    },
  },
  {
    name: "separator",
    type: "string",
    default: '"chevron-right"',
    description: {
      en: "Fluent icon name used by the automatically generated separators.",
      "zh-CN": "自动生成的分隔符使用的 Fluent 图标名。",
    },
  },
  {
    name: "collapse-from",
    type: "number",
    default: "1",
    description: {
      en: "Zero-based index where overflow collapsing starts. Trailing items are kept whenever they fit, and the last item is always visible.",
      "zh-CN":
        "溢出折叠的起始下标（从 0 开始）；折叠时优先保留尾部节点，只有放不下才继续向左折叠。最后一项始终可见。",
    },
  },
];
