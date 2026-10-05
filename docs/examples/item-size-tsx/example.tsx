import {
  Item,
  ItemContent,
  ItemDescription,
  ItemMedia,
  ItemTitle,
} from "@lcui/react";

export default function Example() {
  return (
    <widget className="flex max-w-md flex-col gap-6">
      <Item variant="outline">
        <ItemMedia variant="image" className="bg-violet-400" />
        <ItemContent>
          <ItemTitle>Default Size</ItemTitle>
          <ItemDescription>
            The standard size for most use cases.
          </ItemDescription>
        </ItemContent>
      </Item>
      <Item variant="outline" size="sm">
        <ItemMedia variant="image" className="bg-violet-400" />
        <ItemContent>
          <ItemTitle>Small Size</ItemTitle>
          <ItemDescription>A compact size for dense layouts.</ItemDescription>
        </ItemContent>
      </Item>
      <Item variant="outline" size="xs">
        <ItemMedia variant="image" className="bg-violet-400" />
        <ItemContent>
          <ItemTitle>Extra Small Size</ItemTitle>
          <ItemDescription>The most compact size available.</ItemDescription>
        </ItemContent>
      </Item>
    </widget>
  );
}
