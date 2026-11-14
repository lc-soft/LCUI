import { widgetFields, FieldData } from "./widget";

export const dropdownMenuFields: FieldData[] = [
  ...widgetFields,
  {
    name: "default-open",
    type: "boolean",
    default: "false",
    description: {
      en: "Opens the menu on first render when no explicit open state is set.",
      "zh-CN": "首次渲染时打开菜单；仅在未显式设置 open 时生效。",
    },
  },
  {
    name: "open",
    type: "boolean",
    default: "false",
    description: {
      en: "Controlled open state. Setting it updates the menu immediately.",
      "zh-CN": "受控打开状态，设置后立即同步菜单。",
    },
  },
  {
    name: "match-trigger-width",
    type: "boolean",
    default: "false",
    description: {
      en: "Sizes the menu panel to the trigger width when it opens.",
      "zh-CN": "打开时让菜单面板宽度与触发器一致。",
    },
  },
];
