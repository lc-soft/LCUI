import { widgetFields, FieldData } from "./widget";

export const cardFields: FieldData[] = [
  ...widgetFields,
  {
    name: "size",
    type: '"default" | "sm"',
    default: '"default"',
    description: {
      en: "Card spacing size.",
      "zh-CN": "卡片间距尺寸。",
    },
  },
];

export const cardHeaderFields: FieldData[] = [...widgetFields];
export const cardTitleFields: FieldData[] = [...widgetFields];
export const cardDescriptionFields: FieldData[] = [...widgetFields];
export const cardActionFields: FieldData[] = [...widgetFields];
export const cardContentFields: FieldData[] = [...widgetFields];
export const cardFooterFields: FieldData[] = [...widgetFields];
