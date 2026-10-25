import {
  Button,
  Collapsible,
  CollapsibleContent,
  CollapsibleTrigger,
  Icon,
  Text,
  Widget,
} from "@lcui/react";

export default function App() {
  return (
    <Collapsible className="flex w-[350px] flex-col gap-2">
      <Widget className="flex items-center justify-between gap-4 px-4">
        <Text className="text-sm font-semibold">Order #4189</Text>
        <CollapsibleTrigger>
          <Button variant="ghost" size="icon" className="size-8">
            <Icon name="chevron-up-down" />
          </Button>
        </CollapsibleTrigger>
      </Widget>
      <Widget className="flex items-center justify-between rounded-md border px-4 py-2 text-sm">
        <Text className="text-muted-foreground">Status</Text>
        <Text className="font-medium">Shipped</Text>
      </Widget>
      <CollapsibleContent className="flex flex-col gap-2">
        <Widget className="flex flex-col gap-1 rounded-md border px-4 py-2 text-sm">
          <Text className="font-medium">Shipping address</Text>
          <Text className="text-muted-foreground">
            100 Market St, San Francisco
          </Text>
        </Widget>
        <Widget className="flex flex-col gap-1 rounded-md border px-4 py-2 text-sm">
          <Text className="font-medium">Items</Text>
          <Text className="text-muted-foreground">2x Studio Headphones</Text>
        </Widget>
      </CollapsibleContent>
    </Collapsible>
  );
}
