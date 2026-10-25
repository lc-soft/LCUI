import {
  Button,
  Card,
  CardContent,
  Collapsible,
  CollapsibleContent,
  CollapsibleTrigger,
  Icon,
  Text,
  Widget,
} from "@lcui/react";

export default function App() {
  return (
    <Widget className="w-[384px]">
      <Card className="w-full">
        <CardContent>
          <Collapsible className="rounded-md bg-muted">
            <CollapsibleTrigger>
              <Button
                variant="ghost"
                className="w-full justify-between border-0 bg-transparent"
              >
                <Text>Product details</Text>
                <Icon name="chevron-down" />
              </Button>
            </CollapsibleTrigger>
            <CollapsibleContent className="flex flex-col gap-2 px-2.5 pb-2.5 text-sm">
              <Text className="text-muted-foreground">
                This panel can be expanded or collapsed to reveal additional
                content.
              </Text>
              <Button variant="primary" size="xs">Learn More</Button>
            </CollapsibleContent>
          </Collapsible>
        </CardContent>
      </Card>
    </Widget>
  );
}
