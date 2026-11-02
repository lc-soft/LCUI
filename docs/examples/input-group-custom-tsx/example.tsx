import {
  InputGroup,
  InputGroupAddon,
  InputGroupButton,
  InputGroupInput,
} from "@lcui/react";

export default function Example() {
  return (
    <InputGroup className="w-full max-w-sm">
      <InputGroupInput
        className="min-h-16"
        placeholder="Autoresize textarea..."
      />
      <InputGroupAddon align="block-end">
        <InputGroupButton
          variant="primary"
          size="sm"
          className="ml-auto"
        >
          Submit
        </InputGroupButton>
      </InputGroupAddon>
    </InputGroup>
  );
}
