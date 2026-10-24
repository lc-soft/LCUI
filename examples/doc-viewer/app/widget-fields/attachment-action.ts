import { widgetFields, FieldData } from "./widget";

export const attachmentActionFields: FieldData[] = [
  ...widgetFields,
  {
    name: "size",
    type: '"icon-xs" | "icon-sm" | "icon"',
    default: '"icon-xs"',
    description: {
      en: "Action button size.",
      "zh-CN": "操作按钮尺寸。",
    },
  },
];
