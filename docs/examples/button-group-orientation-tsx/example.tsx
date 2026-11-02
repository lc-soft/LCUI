import { Button, ButtonGroup, Icon } from "@lcui/react";

export default function Example() {
  return (
    <ButtonGroup orientation="vertical">
      <Button variant="outline" size="icon">
        <Icon name="add" />
      </Button>
      <Button variant="outline" size="icon">
        <Icon name="subtract" />
      </Button>
    </ButtonGroup>
  );
}
