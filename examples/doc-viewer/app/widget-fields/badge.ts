import { widgetFields, FieldData } from "./widget";

export const badgeFields: FieldData[] = [
  ...widgetFields,
  {
    name: "variant",
    type: '"default" | "secondary" | "destructive" | "outline" | "ghost"',
    default: '"default"',
    description: {
      en: "Preset visual variant.",
      "zh-CN": "预置的视觉变体。",
    },
  },
  {
    name: "color",
    type: '"blue" | "green" | "sky" | "purple" | "red"',
    default: "-",
    description: {
      en: "Named custom color palette.",
      "zh-CN": "命名的自定义配色方案。",
    },
  },
];