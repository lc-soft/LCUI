import { widgetFields, FieldData } from "./widget";

export const buttonFields: FieldData[] = [
  ...widgetFields,
  {
    name: "disabled",
    type: "string",
    default: "-",
    description: {
      en: "Disables the button when set to \"true\".",
      "zh-CN": "值为 \"true\" 时禁用按钮。",
    },
  },
];
