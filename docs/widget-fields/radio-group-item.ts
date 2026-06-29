import { widgetFields, FieldData } from "./widget";

export const radioGroupItemFields: FieldData[] = [
  ...widgetFields,
  {
    name: "value",
    type: "string",
    default: "",
    description: {
      en: "The value submitted when this item is selected. Must be unique within the same RadioGroup.",
      "zh-CN": "选中本项时提交的值。同一 RadioGroup 内各 item 的 value 必须唯一。",
    },
  },
  {
    name: "checked",
    type: "boolean",
    default: "false",
    description: {
      en: "Whether this item is in the selected state. Driven by the parent RadioGroup's value; you usually don't set this directly.",
      "zh-CN": "本项是否处于选中状态。由父 RadioGroup 的 value 推导，一般不直接设置。",
    },
  },
];