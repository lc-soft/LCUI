import {
  Icon,
  InputGroup,
  InputGroupAddon,
  InputGroupInput,
  InputGroupKbd,
  Text,
} from "@lcui/react";

export default function Example() {
  return (
    <InputGroup className="max-w-sm">
      <InputGroupInput placeholder="Search..." />
      <InputGroupAddon>
        <Icon name="search" />
      </InputGroupAddon>
      <InputGroupAddon align="inline-end">
        <InputGroupKbd>
          <Text>Ctrl K</Text>
        </InputGroupKbd>
      </InputGroupAddon>
    </InputGroup>
  );
}
