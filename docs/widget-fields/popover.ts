import { widgetFields, FieldData } from "./widget";

export const popoverFields: FieldData[] = [
  ...widgetFields,
  {
    name: "open",
    type: "boolean",
    default: "false",
    description: {
      en: "Whether the popover is open. Use ui_popover_set_open() to control it; clicking the trigger, clicking outside, or pressing Escape also changes it.",
      "zh-CN":
        "弹出层是否显示。使用 ui_popover_set_open() 控制；点击触发器、点击外部或按 Escape 也会改变状态。",
    },
  },
];
