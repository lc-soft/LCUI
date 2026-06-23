import { widgetFields, FieldData } from "./widget";

export const anchorFields: FieldData[] = [
  ...widgetFields,
  {
    name: "href",
    type: "string",
    default: "-",
    description: {
      en: "The URL the link points to.",
      "zh-CN": "链接的目标 URL。",
    },
  },
];
