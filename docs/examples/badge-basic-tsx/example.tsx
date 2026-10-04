import { Badge } from "@lcui/react";

export default function Example() {
  return (
    <widget className="flex flex-wrap gap-2">
      <Badge>Badge</Badge>
      <Badge variant="secondary">Secondary</Badge>
      <Badge variant="destructive">Destructive</Badge>
      <Badge variant="outline">Outline</Badge>
    </widget>
  );
}
