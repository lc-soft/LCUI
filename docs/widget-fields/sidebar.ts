import { widgetFields, FieldData } from "./widget";

export const sidebarFields: FieldData[] = [
  ...widgetFields,
  {
    name: "collapsible",
    type: '"offcanvas" | "icon" | "none"',
    default: '"offcanvas"',
    description: {
      en: "Collapse behavior when the sidebar is closed.",
      "zh-CN": "侧栏收起时的折叠模式。",
    },
  },
  {
    name: "side",
    type: '"left" | "right"',
    default: '"left"',
    description: {
      en: "Which side of the layout the sidebar occupies.",
      "zh-CN": "侧栏位于布局的左侧还是右侧。",
    },
  },
];
