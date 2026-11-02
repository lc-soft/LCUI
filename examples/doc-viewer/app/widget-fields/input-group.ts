import { widgetFields, FieldData } from "./widget";

export const inputGroupFields: FieldData[] = [
  ...widgetFields,
  {
    name: "disabled",
    type: "boolean",
    default: "false",
    description: {
      en: "Dim the whole group; controls keep their own disabled behavior.",
      "zh-CN": "弱化整个输入组；控件仍按自身 disabled 状态工作。",
    },
  },
];
