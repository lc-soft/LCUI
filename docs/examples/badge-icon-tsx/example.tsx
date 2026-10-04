import { Badge, Text } from "@lcui/react";
import { Bookmark, CheckmarkStarburst } from "@lcui/fluent-icons";

export default function Example() {
  return (
    <widget className="flex flex-wrap gap-2">
      <Badge variant="secondary">
        <CheckmarkStarburst />
        <Text>Verified</Text>
      </Badge>
      <Badge variant="outline">
        <Text>Bookmark</Text>
        <Bookmark />
      </Badge>
    </widget>
  );
}
