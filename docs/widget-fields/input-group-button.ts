import { widgetFields, FieldData } from "./widget";

export const inputGroupButtonFields: FieldData[] = [
  ...widgetFields,
  {
    name: "variant",
    type: '"ghost" | "outline" | "primary" | "secondary" | "destructive" | "link"',
    default: '"ghost"',
    description: {
      en: "Button variant; defaults to the low-emphasis ghost style.",
      "zh-CN": "按钮变体；默认为低强调的 ghost 样式。",
    },
  },
  {
    name: "size",
    type: '"xs" | "icon-xs" | "sm" | "icon-sm"',
    default: '"xs"',
    description: {
      en: "Button size; use icon-xs for square icon-only buttons.",
      "zh-CN": "按钮尺寸；纯图标按钮使用 icon-xs。",
    },
  },
  {
    name: "disabled",
    type: "boolean",
    default: "false",
    description: {
      en: "Disable the button.",
      "zh-CN": "禁用按钮。",
    },
  },
];
