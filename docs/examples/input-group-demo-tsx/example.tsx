import {
  Icon,
  InputGroup,
  InputGroupAddon,
  InputGroupInput,
  Text,
} from "@lcui/react";

export default function Example() {
  return (
    <InputGroup className="w-[320px]">
      <InputGroupInput placeholder="Search..." />
      <InputGroupAddon>
        <Icon name="search" />
      </InputGroupAddon>
      <InputGroupAddon align="inline-end">
        <Text>12 results</Text>
      </InputGroupAddon>
    </InputGroup>
  );
}
