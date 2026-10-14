import { widgetFields, FieldData } from "./widget";

export const selectFields: FieldData[] = [
  ...widgetFields,
  {
    name: "value",
    type: "string",
    default: "",
    description: {
      en: "Currently selected item value.",
      "zh-CN": "当前选中项的值。",
    },
  },
  {
    name: "placeholder",
    type: "string",
    default: "",
    description: {
      en: "Text shown when no item is selected.",
      "zh-CN": "未选择任何选项时显示的文本。",
    },
  },
  {
    name: "disabled",
    type: "string",
    default: "-",
    description: {
      en: 'Disables the select when set to "true": the trigger no longer opens the content panel.',
      "zh-CN": '值为 "true" 时禁用下拉选择：触发器不再打开选项面板。',
    },
  },
  {
    name: "align-item-with-trigger",
    type: "boolean",
    default: "true",
    description: {
      en: "Whether the popup overlaps the trigger so the selected item aligns with it. Falls back to scrolling the selection into view when there is not enough room.",
      "zh-CN": "是否让选项面板覆盖触发器、使选中项与触发器对齐。空间不足时改为把选中项滚动进可视区。",
    },
  },
];
