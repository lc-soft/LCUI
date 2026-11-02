import { widgetFields, FieldData } from "./widget";

export const inputGroupTextareaFields: FieldData[] = [
  ...widgetFields,
  {
    name: "placeholder",
    type: "string",
    default: "-",
    description: {
      en: "Placeholder text shown while the textarea is empty.",
      "zh-CN": "多行输入为空时显示的占位文本。",
    },
  },
  {
    name: "disabled",
    type: "boolean",
    default: "false",
    description: {
      en: "Disable the textarea.",
      "zh-CN": "禁用多行输入。",
    },
  },
];
