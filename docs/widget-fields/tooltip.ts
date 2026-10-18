import { widgetFields, FieldData } from "./widget";

export const tooltipFields: FieldData[] = [
  ...widgetFields,
  {
    name: "delay",
    type: "number",
    default: "0",
    description: {
      en: "Delay before showing the tooltip, in milliseconds.",
      "zh-CN": "显示提示前的延迟，单位毫秒。",
    },
  },
];
