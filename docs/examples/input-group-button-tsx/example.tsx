import {
  Icon,
  InputGroup,
  InputGroupAddon,
  InputGroupButton,
  InputGroupInput,
  Widget,
} from "@lcui/react";

export default function Example() {
  return (
    <Widget className="flex w-full max-w-sm flex-col gap-6 clip-safe">
      <InputGroup>
        <InputGroupInput placeholder="https://example.com/username" />
        <InputGroupAddon align="inline-end">
          <InputGroupButton size="icon-xs">
            <Icon name="copy" />
          </InputGroupButton>
        </InputGroupAddon>
      </InputGroup>
      <InputGroup>
        <InputGroupInput placeholder="Type to search..." />
        <InputGroupAddon align="inline-end">
          <InputGroupButton variant="secondary">Search</InputGroupButton>
        </InputGroupAddon>
      </InputGroup>
      <InputGroup>
        <InputGroupInput placeholder="https://" />
        <InputGroupAddon align="inline-end">
          <InputGroupButton size="icon-xs">
            <Icon name="star" />
          </InputGroupButton>
        </InputGroupAddon>
      </InputGroup>
    </Widget>
  );
}
