import { widgetFields, FieldData } from "./widget";

export const accordionItemFields: FieldData[] = [
  ...widgetFields,
  {
    name: "value",
    type: "string",
    default: "-",
    description: {
      en: "Item identifier used by defaultValue, value, and the change payload.",
      "zh-CN": "节点标识，用于 defaultValue、value 和 change 载荷。",
    },
  },
  {
    name: "open",
    type: "boolean",
    default: "false",
    description: {
      en: "Open state of the item. Changing it after initialization emits events.",
      "zh-CN": "节点展开状态；初始化完成后再修改会触发事件。",
    },
  },
  {
    name: "disabled",
    type: "boolean",
    default: "false",
    description: {
      en: "Skip click and keyboard activation for this item.",
      "zh-CN": "禁用该项的点击与键盘切换。",
    },
  },
];
