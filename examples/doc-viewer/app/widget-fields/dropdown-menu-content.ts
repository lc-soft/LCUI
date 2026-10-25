import { widgetFields, FieldData } from "./widget";

export const dropdownMenuContentFields: FieldData[] = [
  ...widgetFields,
  {
    name: "side",
    type: '"top" | "right" | "bottom" | "left"',
    default: '"bottom"',
    description: {
      en: "Side of the trigger where the content opens.",
      "zh-CN": "内容面板相对触发器展开的方向。",
    },
  },
  {
    name: "align",
    type: '"start" | "center" | "end"',
    default: '"start"',
    description: {
      en: "Alignment of the content along the trigger edge.",
      "zh-CN": "内容面板沿触发器边缘的对齐方式。",
    },
  },
  {
    name: "side-offset",
    type: "number",
    default: "4",
    description: {
      en: "Distance in pixels between the trigger and the content.",
      "zh-CN": "内容面板与触发器之间的距离，单位为像素。",
    },
  },
  {
    name: "align-offset",
    type: "number",
    default: "0",
    description: {
      en: "Offset in pixels along the alignment axis.",
      "zh-CN": "沿对齐轴方向的偏移量，单位为像素。",
    },
  },
];
