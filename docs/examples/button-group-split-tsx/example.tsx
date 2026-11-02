import {
  Button,
  ButtonGroup,
  ButtonGroupSeparator,
  Icon,
} from "@lcui/react";

export default function Example() {
  return (
    <ButtonGroup>
      <Button variant="secondary">Button</Button>
      <ButtonGroupSeparator />
      <Button variant="secondary" size="icon">
        <Icon name="add" />
      </Button>
    </ButtonGroup>
  );
}
