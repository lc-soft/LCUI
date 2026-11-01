import { widgetFields, FieldData } from "./widget";

export const accordionTriggerFields: FieldData[] = [
  ...widgetFields,
  {
    name: "chevron",
    type: "boolean",
    default: "true",
    description: {
      en: "Render the built-in chevron icon; set false for a fully custom header.",
      "zh-CN": "是否显示内置展开箭头；自定义标题栏时设为 false。",
    },
  },
];
