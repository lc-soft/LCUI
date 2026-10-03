import { Button, Widget } from "@lcui/react";

export default function Example() {
  return (
    <Widget className="flex flex-wrap items-center gap-4">
      <Button disabled>Outline</Button>
      <Button variant="primary" disabled>
        Primary
      </Button>
      <Button variant="secondary" disabled>
        Secondary
      </Button>
      <Button variant="ghost" disabled>
        Ghost
      </Button>
      <Button variant="destructive" disabled>
        Destructive
      </Button>
      <Button variant="link" disabled>
        Link
      </Button>
    </Widget>
  );
}
