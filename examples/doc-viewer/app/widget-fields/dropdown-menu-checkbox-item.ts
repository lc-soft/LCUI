import { widgetFields, FieldData } from "./widget";

export const dropdownMenuCheckboxItemFields: FieldData[] = [
  ...widgetFields,
  {
    name: "checked",
    type: "boolean",
    default: "false",
    description: {
      en: "Checked state, toggled when the item is activated.",
      "zh-CN": "勾选状态，激活该项时自动切换。",
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
  {
    name: "variant",
    type: '"default" | "destructive"',
    default: '"default"',
    description: {
      en: "Visual variant of the item.",
      "zh-CN": "该项的视觉变体。",
    },
  },
];
