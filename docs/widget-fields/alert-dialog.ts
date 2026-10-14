import { widgetFields, FieldData } from "./widget";

export const alertDialogFields: FieldData[] = [
  ...widgetFields,
  {
    name: "open",
    type: "boolean",
    default: "false",
    description: {
      en: "Whether the alert dialog is displayed. Unlike Dialog, it stays open until cancel or action is chosen.",
      "zh-CN": "确认对话框是否显示。与 Dialog 不同，除非选择取消或确认，否则保持打开。",
    },
  },
];
