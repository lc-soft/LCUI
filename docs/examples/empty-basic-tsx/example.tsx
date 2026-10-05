import {
  Button,
  Empty,
  EmptyContent,
  EmptyHeader,
  EmptyMedia,
  EmptyTitle,
  EmptyDescription,
  EmptyAction,
  Text,
} from "@lcui/react";
import { ArrowUpRight, Folder } from "@lcui/fluent-icons";

export default function Example() {
  return (
    <Empty className="w-full max-w-[400px]">
      <EmptyHeader>
      <EmptyMedia variant="icon">
        <Folder />
      </EmptyMedia>
      <EmptyTitle>No Projects Yet</EmptyTitle>
      <EmptyDescription>
        You haven't created any projects yet. Get started by creating your first
        project.
      </EmptyDescription>
      </EmptyHeader>
      <EmptyContent>
      <EmptyAction>
        <Button variant="primary">
          <Text>Create Project</Text>
        </Button>
        <Button>
          <Text>Import Project</Text>
        </Button>
      </EmptyAction>
      </EmptyContent>
      <Button variant="link" size="sm" className="text-muted-foreground">
        <Text>Learn More</Text>
        <ArrowUpRight />
      </Button>
    </Empty>
  );
}
