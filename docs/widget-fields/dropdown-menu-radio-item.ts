import { widgetFields, FieldData } from "./widget";

export const dropdownMenuRadioItemFields: FieldData[] = [
  ...widgetFields,
  {
    name: "value",
    type: "string",
    default: "-",
    description: {
      en: "Value committed to the parent radio group when activated.",
      "zh-CN": "激活时写入父级单选组的值。",
    },
  },
  {
    name: "disabled",
    type: "boolean",
    default: "false",
    description: {
      en: "Prevents the item from being activated.",
      "zh-CN": "禁用该项，点击与键盘激活都会被忽略。",
    },
  },
];
