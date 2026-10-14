import { widgetFields, FieldData } from "./widget";

// `shimmer-content` 继承公共字段；作为 Shimmer 的直属内容子树，其内部节点会被
// 捕获为骨架遮罩，本身不暴露额外可配置属性。
export const shimmerContentFields: FieldData[] = [...widgetFields];
