import { widgetFields, FieldData } from "./widget";

export const dropdownMenuSubFields: FieldData[] = [
  ...widgetFields,
  {
    name: "default-open",
    type: "boolean",
    default: "false",
    description: {
      en: "Opens the submenu on first render.",
      "zh-CN": "首次渲染时展开子菜单。",
    },
  },
  {
    name: "open",
    type: "boolean",
    default: "false",
    description: {
      en: "Controlled open state of the submenu.",
      "zh-CN": "子菜单的受控打开状态。",
    },
  },
];
