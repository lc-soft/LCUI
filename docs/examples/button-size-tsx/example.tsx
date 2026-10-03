import { Button, Widget } from "@lcui/react";
import { ArrowUpRight } from "@lcui/fluent-icons";

export default function Example() {
  return (
    <Widget className="flex flex-wrap items-center gap-4">
      <Widget className="flex items-center gap-2">
        <Button size="xs">Extra Small</Button>
        <Button size="icon-xs">
          <ArrowUpRight />
        </Button>
      </Widget>
      <Widget className="flex items-center gap-2">
        <Button size="sm">Small</Button>
        <Button size="icon-sm">
          <ArrowUpRight />
        </Button>
      </Widget>
      <Widget className="flex items-center gap-2">
        <Button>Default</Button>
        <Button size="icon">
          <ArrowUpRight />
        </Button>
      </Widget>
      <Widget className="flex items-center gap-2">
        <Button size="lg">Large</Button>
        <Button size="icon-lg">
          <ArrowUpRight />
        </Button>
      </Widget>
    </Widget>
  );
}
