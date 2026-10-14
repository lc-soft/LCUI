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
  {
    name: "disabled",
    type: "string",
    default: "-",
    description: {
      en: 'Disables this item when set to "true"; clicking it does not change the value.',
      "zh-CN": '值为 "true" 时禁用该选项；点击它不会改变当前值。',
    },
  },
];
