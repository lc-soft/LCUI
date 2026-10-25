import { widgetFields, FieldData } from "./widget";

export const sidebarGroupLabelFields: FieldData[] = [
  ...widgetFields,
  {
    name: "data-collapsible-trigger",
    type: "boolean",
    default: "false",
    description: {
      en: "Toggles the nearest Collapsible when the label is clicked.",
      "zh-CN": "点击标签时切换最近的 Collapsible。",
    },
  },
];
