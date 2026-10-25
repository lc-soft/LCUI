import { widgetFields, FieldData } from "./widget";

export const dropdownMenuSubTriggerFields: FieldData[] = [
  ...widgetFields,
  {
    name: "disabled",
    type: "boolean",
    default: "false",
    description: {
      en: "Prevents the submenu from being opened.",
      "zh-CN": "禁用后不再展开子菜单。",
    },
  },
];
