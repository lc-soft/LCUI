import { widgetFields, FieldData } from "./widget";

export const radioGroupFields: FieldData[] = [
  ...widgetFields,
  {
    name: "value",
    type: "string",
    default: "",
    description: {
      en: "Currently selected item value. Items whose value attribute matches are shown as selected.",
      "zh-CN": "当前选中项的值。值匹配项的 value 属性的 item 会显示为选中状态。",
    },
  },
];