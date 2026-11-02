import {
  Button,
  ButtonGroup,
  ButtonGroupSeparator,
} from "@lcui/react";

export default function Example() {
  return (
    <ButtonGroup>
      <Button variant="secondary" size="sm">Copy</Button>
      <ButtonGroupSeparator />
      <Button variant="secondary" size="sm">Paste</Button>
    </ButtonGroup>
  );
}
