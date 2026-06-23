import { widgetFields, FieldData } from "./widget";

export const scrollbarFields: FieldData[] = [
  ...widgetFields,
  {
    name: "orientation",
    type: '"vertical" | "horizontal"',
    default: '"vertical"',
    description: {
      en: "The scroll direction of the scrollbar.",
      "zh-CN": "滚动条的滚动方向。",
    },
  },
];
