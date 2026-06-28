import { widgetFields, FieldData } from "./widget";

export const checkboxFields: FieldData[] = [
  ...widgetFields,
  {
    name: "checked",
    type: "boolean",
    default: "false",
    description: {
      en: "Whether the checkbox is in the checked state.",
      "zh-CN": "checkbox 是否处于勾选状态。",
    },
  },
  {
    name: "indeterminate",
    type: "boolean",
    default: "false",
    description: {
      en: "Whether the checkbox is in the indeterminate (mixed) state. A click transitions it to the checked state.",
      "zh-CN": "checkbox 是否处于半选状态。点击会转为已选状态。",
    },
  },
];