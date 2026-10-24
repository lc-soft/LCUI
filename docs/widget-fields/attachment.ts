import { widgetFields, FieldData } from "./widget";

export const attachmentFields: FieldData[] = [
  ...widgetFields,
  {
    name: "state",
    type: '"idle" | "uploading" | "processing" | "error" | "done"',
    default: '"done"',
    description: {
      en: "Upload lifecycle state. Uploading and processing shimmer the title.",
      "zh-CN": "上传生命周期状态。uploading 和 processing 会让标题显示高光扫过效果。",
    },
  },
  {
    name: "size",
    type: '"default" | "sm" | "xs"',
    default: '"default"',
    description: {
      en: "Attachment size.",
      "zh-CN": "附件尺寸。",
    },
  },
  {
    name: "orientation",
    type: '"horizontal" | "vertical"',
    default: '"horizontal"',
    description: {
      en: "Lay the media beside or above the content.",
      "zh-CN": "媒体放在内容旁边还是上方。",
    },
  },
];
