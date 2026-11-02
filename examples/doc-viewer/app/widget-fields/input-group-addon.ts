import { widgetFields, FieldData } from "./widget";

export const inputGroupAddonFields: FieldData[] = [
  ...widgetFields,
  {
    name: "align",
    type: '"inline-start" | "inline-end" | "block-start" | "block-end"',
    default: '"inline-start"',
    description: {
      en: "Where the addon sits relative to the input: same row at either end, or above/below as a block.",
      "zh-CN": "附加内容相对输入框的位置：同一行的起始/末尾，或上方/下方的块级位置。",
    },
  },
];
