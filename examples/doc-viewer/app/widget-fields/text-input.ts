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
  {
    name: "multiline",
    type: "boolean",
    default: "false",
    description: {
      en: "Enables multi-line editing: newlines are kept and the widget sizes for multiple lines.",
      "zh-CN": "启用多行编辑：保留换行符，并按多行高度自适应。",
    },
  },
];
