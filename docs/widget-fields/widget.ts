export interface FieldData {
  name: string;
  type: string;
  default: string;
  description: { en: string; "zh-CN": string };
}

export const widgetFields: FieldData[] = [
  {
    name: "className",
    type: "string",
    default: "-",
    description: {
      en: "CSS class name applied to the element.",
      "zh-CN": "应用于元素的 CSS 类名。",
    },
  },
  {
    name: "children",
    type: "any",
    default: "-",
    description: {
      en: "Child elements.",
      "zh-CN": "子元素。",
    },
  },
];
