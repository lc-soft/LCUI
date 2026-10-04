import { widgetFields, FieldData } from "./widget";

export const alertFields: FieldData[] = [
  ...widgetFields,
  {
    name: "variant",
    type: '"default" | "info" | "success" | "warning" | "destructive"',
    default: '"default"',
    description: { en: "Alert visual variant.", "zh-CN": "提示的视觉变体。" },
  },
];

export const alertContentFields: FieldData[] = [...widgetFields];
export const alertTitleFields: FieldData[] = [...widgetFields];
export const alertDescriptionFields: FieldData[] = [...widgetFields];
export const alertActionFields: FieldData[] = [...widgetFields];