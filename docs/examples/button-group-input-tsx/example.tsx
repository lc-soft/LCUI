import {
  Button,
  ButtonGroup,
  Icon,
  TextInput,
} from "@lcui/react";

export default function Example() {
  return (
    <ButtonGroup className="w-[320px] clip-safe">
      <TextInput placeholder="Search..." />
      <Button variant="outline" size="icon">
        <Icon name="search" />
      </Button>
    </ButtonGroup>
  );
}
