import { widgetFields, FieldData } from "./widget";

export const buttonFields: FieldData[] = [
  ...widgetFields,
  {
    name: "variant",
    type: '"outline" | "primary" | "secondary" | "ghost" | "destructive" | "link"',
    default: '"outline"',
    description: {
      en: "Preset visual style of the button.",
      "zh-CN": "按钮的预置视觉样式。",
    },
  },
  {
    name: "size",
    type: '"default" | "xs" | "sm" | "lg" | "icon" | "icon-xs" | "icon-sm" | "icon-lg"',
    default: '"default"',
    description: {
      en: "Button size preset; icon sizes render compact square buttons.",
      "zh-CN": "按钮尺寸预设；icon 系列尺寸会显示为紧凑的方形按钮。",
    },
  },
  {
    name: "disabled",
    type: "string",
    default: "-",
    description: {
      en: 'Disables the button when set to "true".',
      "zh-CN": '值为 "true" 时禁用按钮。',
    },
  },
];
