import { widgetFields, FieldData } from "./widget";

export const sidebarProviderFields: FieldData[] = [
  ...widgetFields,
  {
    name: "defaultOpen",
    type: "boolean",
    default: "true",
    description: {
      en: "Whether the sidebar is initially expanded.",
      "zh-CN": "侧栏是否初始展开。",
    },
  },
  {
    name: "open",
    type: "boolean",
    default: "-",
    description: {
      en: "Controlled expanded state. Overrides defaultOpen when provided.",
      "zh-CN": "受控展开状态；设置后优先于 defaultOpen。",
    },
  },
];
