import { widgetFields, FieldData } from "./widget";

export const shimmerFields: FieldData[] = [
  ...widgetFields,
  {
    name: "running",
    type: "boolean",
    default: "true",
    description: {
      en: "Whether the sheen animation is running. True by default; when off the content is composited with no highlight added.",
      "zh-CN": "是否播放扫光动画。默认为 true；关闭时不再叠加高光，内容按原色显示。",
    },
  },
  {
    name: "color",
    type: "string",
    default: "rgba(255, 255, 255, 0.86)",
    description: {
      en: "Highlight colour overlaid on the content's opaque pixels, given as a CSS colour string; its alpha is the peak strength of the sheen.",
      "zh-CN": "叠加到内容不透明像素上的高光颜色，采用 CSS 颜色字符串；其 alpha 即扫过处的峰值强度。",
    },
  },
  {
    name: "duration",
    type: "number",
    default: "2000",
    description: {
      en: "Time for one full sweep in milliseconds. Defaults to 2000.",
      "zh-CN": "单次扫过的时长，单位为毫秒。默认为 2000。",
    },
  },
  {
    name: "spread",
    type: "number",
    default: "-",
    description: {
      en: "Width of the highlight band in logical pixels. When unset it defaults to about 45% of the box width and is recomputed on resize; setting it pins a fixed width.",
      "zh-CN": "高光带宽度，单位为逻辑像素。未设置时默认为框宽的约 45% 并在尺寸变化时重新计算；显式设置后固定为该值。",
    },
  },
  {
    name: "angle",
    type: "number",
    default: "20",
    description: {
      en: "Tilt of the highlight band in degrees, shearing its center line across the height. Defaults to 20.",
      "zh-CN": "高光带倾斜角，单位为度，使反光中心线沿高度方向错切。默认为 20。",
    },
  },
  {
    name: "phase",
    type: "number",
    default: "0",
    description: {
      en: "Sweep phase in [0, 1). Added for deterministic tests and manual control; while running the animation advances it by the duration.",
      "zh-CN": "高光带相位，取值范围 [0, 1)，用于确定性测试或手动控制；运行状态下由 duration 持续推进。",
    },
  },
];
