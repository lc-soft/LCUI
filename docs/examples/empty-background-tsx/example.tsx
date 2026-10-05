import {
  Button,
  Empty,
  EmptyContent,
  EmptyHeader,
  EmptyAction,
  EmptyDescription,
  EmptyMedia,
  EmptyTitle,
  Text,
} from "@lcui/react";
import { Alert, ArrowSync } from "@lcui/fluent-icons";

export default function Example() {
  return (
    <Empty className="h-full w-full bg-muted/30">
      <EmptyHeader>
      <EmptyMedia variant="icon">
        <Alert />
      </EmptyMedia>
      <EmptyTitle>No Notifications</EmptyTitle>
      <EmptyDescription>
        You're all caught up. New notifications will appear here.
      </EmptyDescription>
      </EmptyHeader>
      <EmptyContent>
      <EmptyAction>
        <Button variant="outline">
          <ArrowSync />
          <Text>Refresh</Text>
        </Button>
      </EmptyAction>
      </EmptyContent>
    </Empty>
  );
}
