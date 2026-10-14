import { widgetFields, FieldData } from "./widget";

export const spinnerFields: FieldData[] = [
  ...widgetFields,
  {
    name: "running",
    type: "boolean",
    default: "true",
    description: {
      en: "Whether the rotation animation is running. True by default.",
      "zh-CN": "是否播放旋转动画。默认为 true。",
    },
  },
  {
    name: "speed",
    type: "number",
    default: "1",
    description: {
      en: "Rotation speed in revolutions per second.",
      "zh-CN": "旋转速度，单位为每秒转数 (rev/s)。",
    },
  },
  {
    name: "angle",
    type: "number",
    default: "0",
    description: {
      en: "Current rotation angle in degrees. While running, the animation drives this value.",
      "zh-CN": "当前旋转角度，单位为度。运行状态下由动画持续推进。",
    },
  },
];
