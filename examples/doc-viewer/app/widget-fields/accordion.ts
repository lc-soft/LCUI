import { widgetFields, FieldData } from "./widget";

export const accordionFields: FieldData[] = [
  ...widgetFields,
  {
    name: "multiple",
    type: "boolean",
    default: "false",
    description: {
      en: "Allow multiple items to stay open at the same time.",
      "zh-CN": "是否允许多项同时展开。",
    },
  },
  {
    name: "defaultValue",
    type: "string",
    default: "-",
    description: {
      en: "Comma-separated item values that start open. Initialization is silent.",
      "zh-CN": "初始展开项的 value，逗号分隔；初始化不会触发 change。",
    },
  },
  {
    name: "value",
    type: "string",
    default: "-",
    description: {
      en: "Comma-separated open item values. Setting it syncs items silently.",
      "zh-CN": "当前展开项的 value，逗号分隔；设置后静默同步子项。",
    },
  },
];
