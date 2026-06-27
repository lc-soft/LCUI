import { widgetFields, FieldData } from "./widget";

export const progressFields: FieldData[] = [
  ...widgetFields,
  {
    name: "value",
    type: "number",
    default: "0",
    description: {
      en: "Current progress value in the range 0~100. Values outside the range are clamped.",
      "zh-CN": "当前进度值，取值范围 0~100。超出范围的值会被截断。",
    },
  },
];