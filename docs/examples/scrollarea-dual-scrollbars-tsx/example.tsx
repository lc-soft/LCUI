import { ScrollArea, ScrollAreaContent, Scrollbar, Text } from "@lcui/react";
import "./example.css";

export default function App() {
  return (
    <ScrollArea className="demo-scrollarea">
      <ScrollAreaContent className="demo-grid">
        {Array.from({ length: 100 }, (_, i) => (
          <Text key={i} className="demo-cell">
            {i + 1}
          </Text>
        ))}
      </ScrollAreaContent>
      <Scrollbar orientation="horizontal" />
      <Scrollbar />
    </ScrollArea>
  );
}
