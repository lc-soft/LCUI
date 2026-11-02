import { widgetFields, FieldData } from "./widget";

export const buttonGroupFields: FieldData[] = [
  ...widgetFields,
  {
    name: "orientation",
    type: '"horizontal" | "vertical"',
    default: '"horizontal"',
    description: {
      en: "Layout direction of the group.",
      "zh-CN": "按钮组的排列方向。",
    },
  },
];
