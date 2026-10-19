import { widgetFields, FieldData } from "./widget";

export const popoverContentFields: FieldData[] = [
  ...widgetFields,
  {
    name: "side",
    type: '"top" | "right" | "bottom" | "left"',
    default: '"bottom"',
    description: {
      en: "Side of the trigger used to place the popover.",
      "zh-CN": "弹出层相对于触发器的方位。",
    },
  },
  {
    name: "align",
    type: '"start" | "center" | "end"',
    default: '"center"',
    description: {
      en: "Alignment of the popover along the side axis.",
      "zh-CN": "弹出层在方位轴上的对齐方式。",
    },
  },
  {
    name: "side-offset",
    type: "number",
    default: "4",
    description: {
      en: "Distance in pixels between the trigger and the popover.",
      "zh-CN": "弹出层与触发器之间的距离，单位像素。",
    },
  },
  {
    name: "align-offset",
    type: "number",
    default: "0",
    description: {
      en: "Offset in pixels along the alignment axis.",
      "zh-CN": "沿对齐轴的偏移，单位像素。",
    },
  },
];
