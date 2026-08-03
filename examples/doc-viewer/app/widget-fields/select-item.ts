import { widgetFields, FieldData } from "./widget";

export const selectItemFields: FieldData[] = [
  ...widgetFields,
  {
    name: "value",
    type: "string",
    default: "-",
    description: {
      en: "Value returned when this item is selected.",
      "zh-CN": "选中此选项时返回的值。",
    },
  },
];
