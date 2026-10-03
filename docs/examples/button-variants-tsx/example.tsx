import { Button, Widget } from "@lcui/react";

export default function Example() {
  return (
    <Widget className="flex flex-wrap items-center gap-4">
      <Button>Outline</Button>
      <Button variant="primary">Primary</Button>
      <Button variant="secondary">Secondary</Button>
      <Button variant="ghost">Ghost</Button>
      <Button variant="destructive">Destructive</Button>
      <Button variant="link">Link</Button>
    </Widget>
  );
}
