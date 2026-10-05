import {
  Item,
  ItemActions,
  ItemContent,
  ItemDescription,
  ItemMedia,
  ItemTitle,
  Text,
} from "@lcui/react";

export default function Example() {
  return (
    <widget className="flex max-w-md flex-col gap-6">
      <Item variant="outline" interactive>
        <ItemMedia variant="image" className="bg-violet-400" />
        <ItemContent>
          <ItemTitle>Midnight City Lights - Electric Nights</ItemTitle>
          <ItemDescription>Neon Dreams</ItemDescription>
        </ItemContent>
        <ItemActions>
          <Text className="text-xs text-muted-foreground">3:45</Text>
        </ItemActions>
      </Item>
      <Item variant="outline" interactive>
        <ItemMedia variant="image" className="bg-sky-400" />
        <ItemContent>
          <ItemTitle>Coffee Shop Conversations - Urban Stories</ItemTitle>
          <ItemDescription>The Morning Brew</ItemDescription>
        </ItemContent>
        <ItemActions>
          <Text className="text-xs text-muted-foreground">4:05</Text>
        </ItemActions>
      </Item>
      <Item variant="outline" interactive>
        <ItemMedia variant="image" className="bg-emerald-400" />
        <ItemContent>
          <ItemTitle>Digital Rain - Binary Beats</ItemTitle>
          <ItemDescription>Cyber Symphony</ItemDescription>
        </ItemContent>
        <ItemActions>
          <Text className="text-xs text-muted-foreground">3:30</Text>
        </ItemActions>
      </Item>
    </widget>
  );
}
