import { widgetFields, FieldData } from "./widget";

export const dropdownMenuSubContentFields: FieldData[] = [
  ...widgetFields,
  {
    name: "side",
    type: '"top" | "right" | "bottom" | "left"',
    default: '"right"',
    description: {
      en: "Side of the submenu trigger where the content opens.",
      "zh-CN": "子菜单内容相对触发项展开的方向。",
    },
  },
  {
    name: "align",
    type: '"start" | "center" | "end"',
    default: '"start"',
    description: {
      en: "Alignment of the submenu content.",
      "zh-CN": "子菜单内容沿触发项边缘的对齐方式。",
    },
  },
  {
    name: "side-offset",
    type: "number",
    default: "4",
    description: {
      en: "Distance in pixels between the submenu trigger and its content.",
      "zh-CN": "子菜单内容与触发项之间的距离，单位为像素。",
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
