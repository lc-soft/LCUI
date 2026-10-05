import { widgetFields, FieldData } from "./widget";

export const itemFields: FieldData[] = [
  ...widgetFields,
  {
    name: "variant",
    type: '"default" | "outline" | "muted"',
    default: '"default"',
    description: {
      en: "Visual style of the content row.",
      "zh-CN": "内容行的视觉样式。",
    },
  },
  {
    name: "size",
    type: '"default" | "sm" | "xs"',
    default: '"default"',
    description: {
      en: "Compactness of the content row.",
      "zh-CN": "内容行的紧凑程度。",
    },
  },
  {
    name: "interactive",
    type: "boolean",
    default: "false",
    description: {
      en: "Enables hover and active presentation.",
      "zh-CN": "启用 hover 和 active 表现。",
    },
  },
];

export const itemMediaFields: FieldData[] = [
  ...widgetFields,
  {
    name: "variant",
    type: '"default" | "icon" | "image"',
    default: '"default"',
    description: {
      en: "Media slot presentation.",
      "zh-CN": "媒体槽位的呈现方式。",
    },
  },
];
export const itemContentFields: FieldData[] = [...widgetFields];
export const itemTitleFields: FieldData[] = [...widgetFields];
export const itemDescriptionFields: FieldData[] = [...widgetFields];
export const itemActionsFields: FieldData[] = [...widgetFields];