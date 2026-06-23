import { widgetFields, FieldData } from "./widget";

export const textInputFields: FieldData[] = [
  ...widgetFields,
  {
    name: "placeholder",
    type: "string",
    default: "-",
    description: {
      en: "Placeholder text shown when the input is empty.",
      "zh-CN": "输入为空时显示的占位文本。",
    },
  },
];
