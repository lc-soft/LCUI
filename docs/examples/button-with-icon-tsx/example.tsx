import { Button, Widget } from "@lcui/react";
import { Branch, BranchFork } from "@lcui/fluent-icons";

export default function Example() {
  return (
    <Widget className="flex items-center gap-4">
      <Button>
        <Branch />
        New Branch
      </Button>
      <Button>
        Fork
        <BranchFork />
      </Button>
    </Widget>
  );
}
