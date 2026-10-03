import { Button, Widget } from "@lcui/react";
import { ArrowUpRight } from "@lcui/fluent-icons";

export default function Example() {
  return (
    <Widget className="flex items-center gap-4">
      <Button size="icon">
        <ArrowUpRight />
      </Button>
      <Button size="icon" variant="ghost">
        <ArrowUpRight />
      </Button>
      <Button size="icon" variant="destructive">
        <ArrowUpRight />
      </Button>
    </Widget>
  );
}
