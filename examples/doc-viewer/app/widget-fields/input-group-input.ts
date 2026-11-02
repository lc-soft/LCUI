import { widgetFields, FieldData } from "./widget";

export const inputGroupInputFields: FieldData[] = [
  ...widgetFields,
  {
    name: "placeholder",
    type: "string",
    default: "-",
    description: {
      en: "Placeholder text shown while the input is empty.",
      "zh-CN": "输入框为空时显示的占位文本。",
    },
  },
  {
    name: "type",
    type: '"text" | "password"',
    default: '"text"',
    description: {
      en: "Input type; use password to mask typed characters.",
      "zh-CN": "输入类型；使用 password 隐藏输入内容。",
    },
  },
  {
    name: "disabled",
    type: "boolean",
    default: "false",
    description: {
      en: "Disable the input.",
      "zh-CN": "禁用输入框。",
    },
  },
];
