import { widgetFields, FieldData } from "./widget";

export const dialogFields: FieldData[] = [
  ...widgetFields,
  {
    name: "open",
    type: "boolean",
    default: "false",
    description: {
      en: "Whether the dialog is displayed. Use ui_dialog_set_open() to control it; clicking the overlay, pressing Esc (while focus is inside the dialog content), or clicking a dialog-close node also closes the dialog.",
      "zh-CN": "对话框是否显示。使用 ui_dialog_set_open() 控制；点击遮罩、按 Esc（焦点需位于对话框内容内）或点击 dialog-close 节点也会关闭对话框。",
    },
  },
  {
    name: "showCloseButton",
    type: "boolean",
    default: "true",
    description: {
      en: "Whether ui_dialog_build_children() injects the default top-right DialogClose button. Use ui_dialog_show_close_button() to control it.",
      "zh-CN": "ui_dialog_build_children() 是否注入右上角的默认 DialogClose 按钮。使用 ui_dialog_show_close_button() 控制。",
    },
  },
];
