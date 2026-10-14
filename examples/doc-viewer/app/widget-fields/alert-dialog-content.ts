import { widgetFields, FieldData } from "./widget";

export const alertDialogContentFields: FieldData[] = [
  ...widgetFields,
  {
    name: "size",
    type: "'default' | 'sm'",
    default: "'default'",
    description: {
      en: "Width of the alert dialog. 'sm' uses a narrower max width.",
      "zh-CN": "确认对话框宽度。'sm' 使用更小的最大宽度。",
    },
  },
];
