import { widgetFields, FieldData } from "./widget";

export const attachmentMediaFields: FieldData[] = [
  ...widgetFields,
  {
    name: "variant",
    type: '"icon" | "image"',
    default: '"icon"',
    description: {
      en: "Whether the media slot holds an icon or an image preview.",
      "zh-CN": "媒体槽位显示图标还是图片预览。",
    },
  },
  {
    name: "src",
    type: "string",
    default: "-",
    description: {
      en: "Local image path used as the media background when variant is image.",
      "zh-CN": "variant 为 image 时用作媒体背景的本地图片路径。",
    },
  },
];
