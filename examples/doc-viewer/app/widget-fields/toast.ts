import { widgetFields, FieldData } from "./widget";

export const toastFields: FieldData[] = [
  ...widgetFields,
  {
    name: "type",
    type: '"default" | "success" | "info" | "warning" | "error" | "loading"',
    default: '"default"',
    description: {
      en: "Toast type. Selects the status icon; loading uses a spinner.",
      "zh-CN": "轻提示类型。选择状态图标；loading 使用 spinner。",
    },
  },
  {
    name: "title",
    type: "string",
    default: "-",
    description: {
      en: "Toast title.",
      "zh-CN": "轻提示标题。",
    },
  },
  {
    name: "description",
    type: "string",
    default: "-",
    description: {
      en: "Toast description.",
      "zh-CN": "轻提示描述。",
    },
  },
  {
    name: "duration",
    type: "number",
    default: "5000",
    description: {
      en: "Auto-dismiss delay in milliseconds. Use -1 for a persistent toast.",
      "zh-CN": "自动关闭延迟，单位毫秒。使用 -1 表示常驻。",
    },
  },
  {
    name: "action_text",
    type: "string",
    default: "-",
    description: {
      en: "Optional text for one action button.",
      "zh-CN": "可选的单个操作按钮文本。",
    },
  },
];
