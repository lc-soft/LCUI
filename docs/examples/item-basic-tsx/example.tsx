import {
  Button,
  Item,
  ItemActions,
  ItemContent,
  ItemDescription,
  ItemMedia,
  ItemTitle,
} from "@lcui/react";
import { CheckmarkStarburst, ChevronRight } from "@lcui/fluent-icons";

export default function Example() {
  return (
    <widget className="flex max-w-md flex-col gap-6">
      <Item variant="outline">
        <ItemContent>
          <ItemTitle>Basic Item</ItemTitle>
          <ItemDescription>
            A simple item with title and description.
          </ItemDescription>
        </ItemContent>
        <ItemActions>
          <Button variant="outline" size="sm">
            Action
          </Button>
        </ItemActions>
      </Item>
      <Item variant="outline" size="sm" interactive>
        <ItemMedia>
          <CheckmarkStarburst fontSize={20} />
        </ItemMedia>
        <ItemContent>
          <ItemTitle>Your profile has been verified.</ItemTitle>
        </ItemContent>
        <ItemActions>
          <ChevronRight fontSize={16} />
        </ItemActions>
      </Item>
    </widget>
  );
}
