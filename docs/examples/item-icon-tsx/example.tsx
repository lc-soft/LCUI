import {
  Button,
  Item,
  ItemActions,
  ItemContent,
  ItemDescription,
  ItemMedia,
  ItemTitle,
} from "@lcui/react";
import { ShieldError } from "@lcui/fluent-icons";

export default function Example() {
  return (
    <widget className="flex max-w-md flex-col gap-6">
      <Item variant="outline">
        <ItemMedia variant="icon">
          <ShieldError />
        </ItemMedia>
        <ItemContent>
          <ItemTitle>Security Alert</ItemTitle>
          <ItemDescription>
            New login detected from unknown device.
          </ItemDescription>
        </ItemContent>
        <ItemActions>
          <Button variant="outline" size="sm">
            Review
          </Button>
        </ItemActions>
      </Item>
    </widget>
  );
}
