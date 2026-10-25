import { widgetFields, FieldData } from "./widget";

export const dropdownMenuRadioGroupFields: FieldData[] = [
  ...widgetFields,
  {
    name: "value",
    type: "string",
    default: '""',
    description: {
      en: "Currently selected radio item value.",
      "zh-CN": "当前选中的单选值。",
    },
  },
];
