import { Button, Widget } from "@lcui/react";
import { ArrowUp } from "@lcui/fluent-icons";

export default function App() {
  return (
    <Widget className="flex items-center gap-4">
      <Button>Button</Button>
      <Button size="icon">
        <ArrowUp />
      </Button>
    </Widget>
  );
}
