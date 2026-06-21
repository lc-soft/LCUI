import { ScrollArea, ScrollAreaContent, Scrollbar, Text } from "@lcui/react";
import "./example.css";

const content = `这是一段用于演示滚动区域功能的示例文本。
当文本内容超出容器高度时，用户可以通过滚动条来查看其余内容。
滚动区域适合阅读长篇文章、展示数据列表。
在实际项目中，滚动区域的尺寸通常由其父布局决定，开发者只需关注内容本身。
滚动区域可以嵌套使用，构建复杂的多层滚动界面。
合理配置滚动方向和样式，可以让界面更加整洁且易于使用。`;

export default function App() {
  return (
    <ScrollArea className="demo-scrollarea">
      <ScrollAreaContent>
        <Text>{content}</Text>
      </ScrollAreaContent>
      <Scrollbar />
    </ScrollArea>
  );
}
