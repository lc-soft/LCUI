import { widgetFields, FieldData } from "./widget";

export const collapsibleFields: FieldData[] = [
  ...widgetFields,
  {
    name: "defaultOpen",
    type: "boolean",
    default: "false",
    description: {
      en: "Whether the panel is initially open.",
      "zh-CN": "面板是否初始展开。",
    },
  },
  {
    name: "open",
    type: "boolean",
    default: "-",
    description: {
      en: "Controlled open state. Overrides defaultOpen when provided.",
      "zh-CN": "受控展开状态；设置后优先于 defaultOpen。",
    },
  },
];
