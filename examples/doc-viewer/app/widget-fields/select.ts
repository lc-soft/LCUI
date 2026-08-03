import { widgetFields, FieldData } from "./widget";

export const selectFields: FieldData[] = [
  ...widgetFields,
  {
    name: "value",
    type: "string",
    default: "",
    description: {
      en: "Currently selected item value.",
      "zh-CN": "当前选中项的值。",
    },
  },
  {
    name: "placeholder",
    type: "string",
    default: "",
    description: {
      en: "Text shown when no item is selected.",
      "zh-CN": "未选择任何选项时显示的文本。",
    },
  },
];
