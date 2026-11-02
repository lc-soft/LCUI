import { widgetFields, FieldData } from "./widget";

export const buttonGroupSeparatorFields: FieldData[] = [
  ...widgetFields,
  {
    name: "orientation",
    type: '"vertical" | "horizontal"',
    default: '"vertical"',
    description: {
      en: "Separator axis: a vertical divider in a horizontal group, or a horizontal divider in a vertical group.",
      "zh-CN": "分隔线方向：水平组用竖线，垂直组用横线。",
    },
  },
];
