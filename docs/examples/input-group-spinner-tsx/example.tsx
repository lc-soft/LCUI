import {
  InputGroup,
  InputGroupAddon,
  InputGroupInput,
  InputGroupText,
  Spinner,
  Text,
  Widget,
} from "@lcui/react";

export default function Example() {
  return (
    <Widget className="flex w-full max-w-sm flex-col gap-4 clip-safe">
      <InputGroup>
        <InputGroupInput placeholder="Searching..." />
        <InputGroupAddon align="inline-end">
          <Spinner className="h-4 w-4" />
        </InputGroupAddon>
      </InputGroup>
      <InputGroup>
        <InputGroupInput placeholder="Processing..." />
        <InputGroupAddon>
          <Spinner className="h-4 w-4" />
        </InputGroupAddon>
      </InputGroup>
      <InputGroup>
        <InputGroupInput placeholder="Saving changes..." />
        <InputGroupAddon align="inline-end">
          <Text>Saving...</Text>
          <Spinner className="h-4 w-4" />
        </InputGroupAddon>
      </InputGroup>
      <InputGroup>
        <InputGroupInput placeholder="Refreshing data..." />
        <InputGroupAddon>
          <Spinner className="h-4 w-4" />
        </InputGroupAddon>
        <InputGroupAddon align="inline-end">
          <InputGroupText>Please wait...</InputGroupText>
        </InputGroupAddon>
      </InputGroup>
    </Widget>
  );
}
