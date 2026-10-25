import { widgetFields, FieldData } from "./widget";

export const dropdownMenuItemFields: FieldData[] = [
  ...widgetFields,
  {
    name: "disabled",
    type: "boolean",
    default: "false",
    description: {
      en: "Prevents the item from being activated by pointer or keyboard.",
      "zh-CN": "禁用该项，鼠标点击与键盘激活都会跳过。",
    },
  },
  {
    name: "variant",
    type: '"default" | "destructive"',
    default: '"default"',
    description: {
      en: "Visual variant; destructive renders the item in the danger color.",
      "zh-CN": "视觉变体；destructive 使用危险色显示。",
    },
  },
];
