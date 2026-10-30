import { widgetFields, FieldData } from "./widget";

export const breadcrumbItemFields: FieldData[] = [
  ...widgetFields,
  {
    name: "data-path",
    type: "string",
    default: "-",
    description: {
      en: "Node key for an explicit item. Items without a key cannot emit change and appear disabled in the collapsed menu.",
      "zh-CN":
        "显式 item 的节点 key。没有 key 的 item 不会发出 change，在折叠下拉中显示为禁用。",
    },
  },
  {
    name: "data-key",
    type: "string",
    default: "-",
    description: {
      en: "Node key for custom content appended with append().",
      "zh-CN": "通过 append() 追加的自定义内容使用的节点 key。",
    },
  },
  {
    name: "data-label",
    type: "string",
    default: "-",
    description: {
      en: "Label shown in the collapsed menu. When omitted, the item text is used, then the node key.",
      "zh-CN":
        "折叠下拉中显示的文字；未设置时依次回退到 item 文本、节点 key。",
    },
  },
];
