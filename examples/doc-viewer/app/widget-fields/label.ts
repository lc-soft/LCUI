import { widgetFields, FieldData } from "./widget";

export const labelFields: FieldData[] = [
  ...widgetFields,
  {
    name: "for",
    type: "string",
    default: "-",
    description: {
      en: "ID of the target widget that this label is associated with. Clicks on the label forward a click event to the target.",
      "zh-CN": "标签关联的目标部件 ID。点击标签时会把 click 事件转发给目标。",
    },
  },
];